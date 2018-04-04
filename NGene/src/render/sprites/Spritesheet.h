#pragma once
#include <string>
#include <vector>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Image.hpp>
#include <SFML/Graphics/Rect.hpp>


class Spritesheet {
    using Sprite = sf::Sprite;
    using IntRect = sf::IntRect;
    using Texture = sf::Texture;
    using Image = sf::Image;
    using SpriteContainer = std::vector<Sprite>;

public:

    enum class SpritesheetMorphology {
        SQUARE,
        UNIFORMLY_PACKED,
    };

    Spritesheet(const std::string& texture_filename, SpritesheetMorphology type);
    Spritesheet(const std::string& texture_filename, SpritesheetMorphology type, int rows, int cols);
    Spritesheet(const std::string& texture_filename, SpritesheetMorphology type, int rows, int cols, int sprite_width, int sprite_height);
    ~Spritesheet();

    const SpriteContainer& get_sprites() const { return m_sprite_container; }
    int get_rows() const { return m_rows; }
    int get_cols() const { return m_cols; }
    int get_sprite_width() const { return m_sprite_width; }
    int get_sprite_height() const { return m_sprite_height; }

private: // Member Functions

    std::tuple<int, int> infer_rows_and_columns(const Image&);

    enum class Line {
        ROW,
        COLUMN
    };

    bool line_empty(const Image& image, int line, Line type_to_check);

    /**

    We allow for regions that have empty lines up to 4 pixels wide,
    this is because a sprite drawn with pieces that are not all
    connected to each other might be detected as new sprites incorrectly.
    */
    int count_continuous_regions(const std::vector<bool>& vector, int threshold = 4);

    void fill_sprite_container();

private: // Member Attributes

    SpriteContainer m_sprite_container{};
    Texture* m_texture{ nullptr };
    std::string m_texture_filename{};

    int m_rows{};
    int m_cols{};
    int m_sprite_width{};
    int m_sprite_height{};
    SpritesheetMorphology m_sprite_type{};


    Image* m_image_cache{ nullptr };
};

