#include "Spritesheet.h"
#include "../../resources/TextureManager.h"
#include "Debug.h"

Spritesheet::Spritesheet(const std::string & texture_filename, SpritesheetMorphology type) {

    m_texture_filename = texture_filename;
    m_texture = TextureManager::get().get_required_resource(m_texture_filename);

    if (!m_texture) {
        LOG_ERROR("We couldn't load the texture from the file " << m_texture_filename);
        return;
    }

    auto image = Image{};
    image.loadFromFile(m_texture_filename);

    int rows;
    int cols;
    std::tie(rows, cols) = infer_rows_and_columns(image);

    int sprite_width;
    int sprite_height;

    if (type == SpritesheetMorphology::SQUARE) {
        sprite_width = sprite_height = image.getSize().x / cols;
    }
    else  if (type == SpritesheetMorphology::UNIFORMLY_PACKED) {
        sprite_width = image.getSize().x / cols;
        sprite_height = image.getSize().y / rows;
    }

    LOG("Rows: " << rows << "\nCols: " << cols << "\nSprite Width: " << sprite_width << "\nSprite Height: " << sprite_height);

    /*
    @@TODO @@DOING
    FIll the vector of sprites with the sprites on the spritesheet

    We need to take into account the position in the spritesheet, the
    sprite size and the origin/pivot point.

    In this case, where they don't specify a pivot point we will default to the center of the sprite.
    */

}

Spritesheet::~Spritesheet() {
    if (m_texture) {
        TextureManager::get().release_required_resource(m_texture_filename);
        m_texture = nullptr;
    }
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
