#pragma once
#include <string>
#include <vector>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Image.hpp>


class Spritesheet {
    using Sprite = sf::Sprite;
    using Texture = sf::Texture;
    using Image = sf::Image;
    using SpriteContainer = std::vector<Sprite>;

public:

    enum class SpritesheetMorphology {
        SQUARE,
        UNIFORMLY_PACKED,
    };

    Spritesheet(const std::string& texture_filename, SpritesheetMorphology type);
    ~Spritesheet();

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


private: // Member Attributes

    SpriteContainer m_sprite_container{};
    Texture* m_texture{ nullptr };
    std::string m_texture_filename{};
};

