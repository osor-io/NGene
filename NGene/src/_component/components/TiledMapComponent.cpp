#include "TiledMapComponent.h"
#include "../lua/LuaManager.h"
#include "File.h"
#include "../debug/LoggingManager.h"
#include <sstream>
#include <cassert>
#include "../resources/TextFileManager.h"
#include "../resources/TextureManager.h"

#define COMPONENT_TYPE TiledMapComponent
#define CTOR(x) x##::##x
#define DTOR(x) x##::##~##x
#define STRINGIFY(s) #s

CTOR(COMPONENT_TYPE)(EntityId id) : ComponentTemplate(id, std::type_index(typeid(COMPONENT_TYPE))) {}

CTOR(COMPONENT_TYPE)(EntityId id, const sol::table& table)
    : ComponentTemplate(id, std::type_index(typeid(COMPONENT_TYPE))) {


    sol::object obj = table["filename"];
    assert(obj.valid());
    m_map_filename = obj.as<std::string>();

    auto path = m_map_filename.substr(0, m_map_filename.find_last_of("\\/"));
    path += '/';

    auto filename = m_map_filename.c_str();

    if (!file_exists(filename)) {
        auto ss = std::stringstream{};
        ss << "Map File [" << filename << "] wasn't found D:";
        LOGF(ss.str().c_str());
    }

    assert(TextFileManager::get().exists_resource(filename));

    auto script = TextFileManager::get().get_scoped_resource(filename);


    auto loaded_script = LUA.load(*script.resource);
    auto loaded_function = loaded_script.get<sol::protected_function>();
    auto loaded_result = loaded_function();

    auto parse_error = [filename](const std::string& msg) {
        auto ss = std::stringstream{};
        ss << "The File [" << filename << "] could not be parsed correctly\n\nMessage Received:\n" << msg;
        LOGF(ss.str().c_str());
    };

    if (!loaded_result.valid()) {
        sol::error err = loaded_result;
        parse_error(err.what());
    }

    sol::table map_table = loaded_result;

    auto map = TiledMap{};

    map.width = map_table["width"];
    map.height = map_table["height"];

    map.tile_width = map_table["tilewidth"];
    map.tile_height = map_table["tileheight"];

    const sol::table& tilesets = map_table["tilesets"];
    tilesets.for_each([&map, &path](const sol::object& key, const sol::object& value) {

        auto tileset_table = value.as<sol::table>();

        auto tileset = TileSet{};

        sol::object o;

        tileset.first_gid = tileset_table["firstgid"];
        tileset.tilecount = tileset_table["tilecount"];
        o = tileset_table["image"];
        tileset.image_filename = path + o.as<std::string>();

        tileset.texture = TextureManager::get().get_required_resource(tileset.image_filename);

        tileset.image_width = tileset_table["imagewidth"];
        tileset.image_height = tileset_table["imageheight"];

        map.tilesets.push_back(tileset);

        auto columns = tileset.image_width / map.tile_width;
        auto rows = tileset.image_height / map.tile_height;

        for (auto i = tileset.first_gid; i <= tileset.tilecount; ++i) {

            auto tile = Tile{};
            tile.id = i;
            tile.animated = false;

            auto real_index = i - tileset.first_gid;

            tile.sprite = sf::Sprite(
                *tileset.texture,
                sf::IntRect(
                (real_index % columns) * map.tile_width,
                    (real_index / columns) * map.tile_height,
                    map.tile_width,
                    map.tile_height
                ));

            map.tiles[i] = std::move(tile);
        }

        const sol::table& tiles = tileset_table["tiles"];

        tiles.for_each([&map](const sol::object& key, const sol::object& value) {
            auto tile_table = value.as<sol::table>();
            auto tile = Tile{};
            tile.id = tile_table["id"];
            sol::object o = tile_table["animation"];
            auto animation_table = o.as<sol::table>();
            tile.animated = true;
            tile.animated_sprites = std::make_unique<std::vector<std::tuple<sf::Sprite, float>>>();

            animation_table.for_each([&map, &tile](const sol::object& key, const sol::object& value) {
                auto frame_table = value.as<sol::table>();

                int frame_id = frame_table["tileid"];
                auto sprite = map.tiles.at(frame_id).sprite;
                float duration = frame_table["duration"];

                tile.animated_sprites->push_back(std::make_tuple(sprite,duration));
            });
            map.tiles[tile.id] = std::move(tile);
        });
    });

    /*
    @@DOING: Reading map .lua files, we already read the tilesets, 
    now we have to read the layers :)
    */
}


DTOR(COMPONENT_TYPE)() {
}


json COMPONENT_TYPE::to_json() {
    auto j = json{};

    j["filename"] = m_map_filename;

    return j;
}

void COMPONENT_TYPE::load_json(const json& j) {
    m_map_filename = j["filename"].get<std::string>();
}

void COMPONENT_TYPE::draw_component_inspector() {

    ImGui::SetNextWindowSize(ImVec2(400, 100), ImGuiCond_FirstUseEver);
    ImGui::Begin(calculate_showname().c_str(), &m_gui_open);

    ImGui::Text("Filename: %s", m_map_filename.c_str());


    ImGui::End();

}

#define REGISTER_METHOD(method) #method , &PhraseComponent::method

void COMPONENT_TYPE::expose_to_lua()
{

    LUA.new_usertype<COMPONENT_TYPE>(STRINGIFY(COMPONENT_TYPE),


        /*
        Methods:
        Add here all the functions we want to expose to lua with REGISTER_METHOD(methodName)
        */

        /*
        Data Members:
        Add here all the members we want to expose to lua with REGISTER_METHOD(methodName)
        */

        "filename", &COMPONENT_TYPE::m_map_filename,
        "mainLayer", &COMPONENT_TYPE::m_main_layer


        );

}