#include "Spritesheet.h"
#include "Debug.h"
#include "File.h"
#include <tuple>
#include "../math/OtherMath.h"
#include "../os/OSManager.h"

#define TEMP_SPRITESHEET_IMAGE_FILENAME "last_spritesheet.png"

Spritesheet::Spritesheet(const std::string & filename, const json & data) {
	from_json(filename, data);
}

Spritesheet::Spritesheet(const std::vector<std::string>& texture_filenames) {

	auto textures = std::vector<TexturePacked>{};
	auto sprites = std::vector<Sprite>{};

	// We read all the filenames into textures and sprites
	for (const auto& filename : texture_filenames) {
		assert(file_exists(filename.c_str()));
		textures.push_back(TexturePacked(TextureManager::get().get_required_resource(filename), filename));
		sprites.push_back(Spritesheet::Sprite(*(textures.back().texture)));
	}

	// We calculate the size of the sprites in the future spritesheet
	auto sprite_width = -1;
	auto sprite_height = -1;
	for (const auto& sprite : sprites) {
		sprite_width = sprite_width > sprite.getTextureRect().width ? sprite_width : sprite.getTextureRect().width;
		sprite_height = sprite_height > sprite.getTextureRect().height ? sprite_height : sprite.getTextureRect().height;
	}
	sprite_width = next_power_of_2(sprite_width);
	sprite_height = next_power_of_2(sprite_height);
	if (sprite_height > sprite_width) {
		sprite_width = sprite_height;
	}
	else {
		sprite_height = sprite_width;
	}

	// We write the sprites to a temporary file with the spritesheet

	auto temporary_spritesheet_filename = OSManager::get().executable_path() + TEMP_SPRITESHEET_IMAGE_FILENAME;

	write_to_file(temporary_spritesheet_filename.c_str(), sprites);

	// We initialize the variables while also reading the texture
	m_sprite_type = SpritesheetMorphology::SQUARE;
	m_texture = TexturePacked(TextureManager::get().get_required_resource(temporary_spritesheet_filename), temporary_spritesheet_filename);
	m_sprite_height = sprite_height;
	m_sprite_width = sprite_width;
	m_rows = m_texture.texture->getSize().y / sprite_height;
	m_cols = m_texture.texture->getSize().x / sprite_width;


	construct(m_texture.filename, m_rows, m_cols, m_sprite_width, m_sprite_height, m_sprite_type);

	for (auto& texture : textures) {
		TextureManager::get().release_required_resource(texture.filename);
	}
}

Spritesheet::Spritesheet(const std::string & texture_filename, SpritesheetMorphology type) :
	m_sprite_type(type),
	m_texture(TexturePacked(TextureManager::get().get_required_resource(texture_filename), texture_filename))
{
	construct(texture_filename, type);
}

Spritesheet::Spritesheet(const std::string & texture_filename, int rows, int cols, SpritesheetMorphology type) :
	m_sprite_type(type),
	m_rows(rows),
	m_cols(cols),
	m_texture(TexturePacked(TextureManager::get().get_required_resource(texture_filename), texture_filename))
{
	construct(texture_filename, rows, cols, type);
}

Spritesheet::Spritesheet(const std::string & texture_filename, int rows, int cols, int sprite_width, int sprite_height, SpritesheetMorphology type) :
	m_sprite_type(type),
	m_rows(rows),
	m_cols(cols),
	m_sprite_width(sprite_width),
	m_sprite_height(sprite_height),
	m_texture(TexturePacked(TextureManager::get().get_required_resource(texture_filename), texture_filename))
{
	construct(texture_filename, rows, cols, m_sprite_width, m_sprite_height, type);
}

Spritesheet::~Spritesheet() {
	if (m_texture.texture) {
		TextureManager::get().release_required_resource(m_texture.filename);
		m_texture.texture = nullptr;
	}
}

void Spritesheet::construct(const std::string & texture_filename, SpritesheetMorphology type) {
	if (!fill_image_cache(texture_filename)) return;

	std::tie(m_rows, m_cols) = infer_rows_and_columns(*m_image_cache);

	construct(texture_filename, m_rows, m_cols, type);
}

void Spritesheet::construct(const std::string & texture_filename, int rows, int cols, SpritesheetMorphology type) {
	if (!fill_image_cache(texture_filename)) return;

	if (type == SpritesheetMorphology::SQUARE) {
		m_sprite_width = m_sprite_height = m_image_cache->getSize().x / cols;
	}
	else  if (type == SpritesheetMorphology::UNIFORMLY_PACKED) {
		m_sprite_width = m_image_cache->getSize().x / cols;
		m_sprite_height = m_image_cache->getSize().y / rows;
	}

	construct(texture_filename, rows, cols, m_sprite_width, m_sprite_height, type);
}

void Spritesheet::construct(const std::string & texture_filename, int rows, int cols, int sprite_width, int sprite_height, SpritesheetMorphology type, bool regenerate_image) {

	if (m_image_cache != nullptr) {
		delete m_image_cache;
		m_image_cache = nullptr;
	}

	if (!m_texture.texture) {
		LOG_ERROR("We couldn't load the texture from the file " << m_texture.filename);
		m_valid = false;
		return;
	}

	if (m_sprite_width == 0 || m_sprite_height == 0) {
		LOG_ERROR("Sprite Width or Height is 0 for the texture read from " << m_texture.filename);
		m_valid = false;
		return;
	}

	refill_sprite_container();

	if (m_sprite_container.size() == 0) {
		LOG_ERROR("We weren't able to detect any sprites from the texture read from " << m_texture.filename);
		m_valid = false;
		return;
	}

	if (regenerate_image) {
		generate_image();
	}
	else {
		LOG("Assuming that the spritesheet provided has been created by us and not modifyed");
		m_image = m_texture.texture->copyToImage();
	}

	// We refill with the new image and texture
	LOG("Finally reading sprites from loaded image");
	refill_sprite_container();

	m_valid = true;
}


std::tuple<int, int> Spritesheet::infer_rows_and_columns(const Image& spritesheet_image) {

	auto res = std::tuple<int, int>();

	auto array_clear_rows = std::vector<bool>(spritesheet_image.getSize().y);
	auto array_clear_cols = std::vector<bool>(spritesheet_image.getSize().x);

	for (auto i = 0; i < array_clear_rows.size(); ++i) {
		array_clear_rows[i] = line_empty(spritesheet_image, i, Line::ROW);
	}

	for (auto i = 0; i < array_clear_cols.size(); ++i) {
		array_clear_cols[i] = line_empty(spritesheet_image, i, Line::COLUMN);
	}

	return std::make_tuple(
		count_continuous_regions(array_clear_rows),
		count_continuous_regions(array_clear_cols)
	);
}

bool Spritesheet::line_empty(const Image& image, int line, Line type_to_check) {
	/*
	@@NOTE

	We will take the RGBA value of the (0,0) position in the texture
	as the background colour to detect sprites.
	*/

	auto background_color = image.getPixel(0, 0);

	if (type_to_check == Line::ROW) {
		for (auto i = 0u; i < image.getSize().x; ++i) {
			if (image.getPixel(i, line) != background_color) {
				return false;
			}
		}
	}
	else if (type_to_check == Line::COLUMN) {
		for (auto i = 0u; i < image.getSize().y; ++i) {
			if (image.getPixel(line, i) != background_color) {
				return false;
			}
		}
	}

	return true;
}

int Spritesheet::count_continuous_regions(const std::vector<bool>& vector, int threshold) {
	auto count = 0;

	for (auto i = 0; i < vector.size(); ++i) {

		if (vector[i]) {
			int empty_count = 1;
			while (++i < vector.size() && vector[i]) {
				++empty_count;
			}
			if (empty_count >= threshold)
				++count;
		}
		else {

		}

	}

	return count - 1;
}

void Spritesheet::refill_sprite_container() {

	assert(m_texture.texture);

	m_sprite_container.clear();

	for (auto row = 0; row < m_rows; ++row) {
		for (auto col = 0; col < m_cols; ++col) {
			/*
			@@NOTE

			This might insert some "empty" sprites that are at the
			end of the last row, for example, if there are 3 rows and
			columns but only 7 sprites, two sprites that reference the
			last two positions (3,2) & (3,3) will me inserted but they
			will only contain the background color and not really any sprite.

			This is valid because you might want empty sprites in some animation,
			an empty sprite might represent a real sprite that we want to render.
			*/
			m_sprite_container.push_back(
				Sprite(
					*(m_texture.texture),
					IntRect(
						col*m_sprite_width,
						row*m_sprite_height,
						m_sprite_width,
						m_sprite_height
					))
			);
		}
	}
}

bool Spritesheet::fill_image_cache(const std::string & filename) {
	if (m_image_cache == nullptr) {
		m_image_cache = new Image();
		m_image_cache->loadFromFile(filename);
		if (m_image_cache->getSize().x == 0 || m_image_cache->getSize().y == 0) {
			LOG_ERROR("We weren't able to read any pixels from the file " << m_texture.filename);
			m_valid = false;
			return false;
		}
	}
	return true;
}

Spritesheet::Image Spritesheet::write_to_image(const std::vector<Spritesheet::Sprite>& sprites,
	unsigned int& cell_side_pixel_length,
	unsigned int& total_rows_and_cols) {

	LOG("Creating spritesheet image with current sprites");

	auto max_sprite_width = 0;
	auto max_sprite_height = 0;
	auto sprite_count = sprites.size();

	for (const auto& sprite : sprites) {
		if (sprite.getTextureRect().width > max_sprite_width)
			max_sprite_width = sprite.getTextureRect().width;

		if (sprite.getTextureRect().height > max_sprite_height)
			max_sprite_height = sprite.getTextureRect().height;
	}

	cell_side_pixel_length = next_power_of_2(max_sprite_width > max_sprite_height ? max_sprite_width : max_sprite_height);

	auto spritesheet_side_pixel_length = next_power_of_2(sqrt(cell_side_pixel_length*cell_side_pixel_length*sprite_count));

	total_rows_and_cols = spritesheet_side_pixel_length / cell_side_pixel_length;

	LOG("\tImage size: " << spritesheet_side_pixel_length << "x" << spritesheet_side_pixel_length);
	LOG("\tSprite size: " << cell_side_pixel_length);

	LOG("\tFilling Image . . .");

	auto spritesheet_image = sf::Image();
	spritesheet_image.create(spritesheet_side_pixel_length, spritesheet_side_pixel_length, sf::Color(.0f, .0f, .0f, .0f));

	{
		auto row = 0;
		auto col = 0;
		for (const auto& sprite : sprites) {

			auto texture = sprite.getTexture();
			assert(texture != nullptr);

			auto texture_image = texture->copyToImage();

			auto dest_x = row * cell_side_pixel_length + ((cell_side_pixel_length - sprite.getTextureRect().width) / 2.f);
			auto dest_y = col * cell_side_pixel_length + ((cell_side_pixel_length - sprite.getTextureRect().height) / 2.f);

			spritesheet_image.copy(texture_image, dest_x, dest_y, sprite.getTextureRect(), true);

			if (++row >= total_rows_and_cols) {
				row = 0;
				++col;
				col = col < total_rows_and_cols ? col : 0;
			}
		}
	}

	return spritesheet_image;

}

void Spritesheet::generate_image() {
	auto sprite_side = 0u;
	auto rows_and_cols = 0u;
	m_image = write_to_image(m_sprite_container, sprite_side, rows_and_cols);
	m_rows = m_cols = rows_and_cols;
	m_sprite_width = m_sprite_height = sprite_side;

	m_texture.clear();
	m_texture = TexturePacked(m_image);
	m_sprite_type = SpritesheetMorphology::SQUARE;

	m_valid = true;
}

bool Spritesheet::write_to_file(const std::string & filename) {

	LOG("\tSaving Image...");

	if (!m_image.saveToFile(filename)) {
		LOG_ERROR("Error saving the pixels to the file: " << filename);
		return false;
	}
	LOG("Wrote spritesheet to file: " << filename);

	m_saved_to_file = true;
	m_saved_filename = filename;

	return true;
}

bool Spritesheet::write_to_file(const std::string & filename, const std::vector<Sprite>& sprites) {

	auto sprite_side = 0u;
	auto rows_and_cols = 0u;


	auto spritesheet_image = write_to_image(sprites, sprite_side, rows_and_cols);

	LOG("\tSaving Image...");

	if (!spritesheet_image.saveToFile(filename)) {
		LOG_ERROR("Error saving the pixels to the file: " << filename);
		return false;
	}
	LOG("Wrote spritesheet to file: " << filename);
	return true;


}

Spritesheet::json Spritesheet::to_json() {
	auto j = json{};

	j["rows"] = m_rows;
	j["cols"] = m_cols;
	j["sprite_width"] = m_sprite_width;
	j["sprite_height"] = m_sprite_height;

	return j;
}

bool Spritesheet::from_json(const std::string & filename, const json & j) {

	m_rows = j["rows"];
	m_cols = j["cols"];
	m_sprite_width = j["sprite_width"];
	m_sprite_height = j["sprite_height"];

	assert(m_sprite_width == m_sprite_height); // We require square sprites at the moment since that is what we export.

	m_sprite_type = SpritesheetMorphology::SQUARE;

	m_texture.clear(); // This is called anyway in the destructor so it shouldn't be necessary.
	m_texture = TexturePacked(TextureManager::get().get_required_resource(filename), filename);

	construct(filename, m_rows, m_cols, m_sprite_width, m_sprite_height, m_sprite_type, false);

	return m_valid;
}
