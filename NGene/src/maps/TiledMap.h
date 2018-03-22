#pragma once

#include <vector>
#include <unordered_map>
#include <string>
#include <memory>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <../resources/TextureManager.h>

struct Tile {
    int id;
    sf::Sprite sprite;
    bool animated{ false };
    std::unique_ptr<std::vector<std::tuple<sf::Sprite, float>>> animated_sprites;
};

struct TileReference {
    Tile* tile{ nullptr };
    float current_duration{ 0.0f };
    int current_frame{ 0 };
    float elapsed_in_frame{ 0.0f };
};

struct Layer {

    int width;
    int height;

    bool visible;

    std::vector<int> data;  // The indices for the tiles' id 

    std::vector<TileReference> tile_references;

    bool has_animations{ false };

    std::string name;

};

struct TileSet {

    int first_gid;

    int tilecount;

    std::string image_filename;
    int image_width;
    int image_height;

    sf::Texture* texture;

};

struct TiledMap {

    int width;     // In Tiles
    int height;    // In Tiles

    int tile_width;
    int tile_height;

    std::vector<sf::Vector2f> tile_coordinates;

    std::vector<TileSet> tilesets{};

    std::vector<Layer> layers{};

    std::unordered_map<int, Tile> tiles{};

};
