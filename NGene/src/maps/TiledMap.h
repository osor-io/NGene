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

struct Layer {

    int width;
    int height;

    bool visible;

    std::vector<int> data;  // The indices for the tiles' id 

    bool has_animations{ false };

};

struct TileSet {

    ~TileSet() {
        if (texture)
            TextureManager::get().release_required_resource(image_filename);
    }

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

    std::vector<TileSet> tilesets{};

    std::vector<Layer> layers{};

    std::unordered_map<int, Tile> tiles{};
};
