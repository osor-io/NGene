/**

The general documentation for the components will be in
the Component.h and CompomentTemplate.h respectively. Here you
can find the documentation of the general members that every component
should have such as the serialization methods (to and from Json),
the ones used to expose the component to LUA scripts or draw its inspector
for the developer mode.

@see ComponentTemplate.h
@see Component.h

*/

#pragma once
#include "../ComponentTemplate.h"
#include <tmxlite/Map.hpp>
#include <SFMLOrthogonalLayer.hpp>
#include "../maps/TiledMap.h"

/**

Component that represents a tile based map that will be used during gameplay.
This means that it will contain the tile types and the location of the actual
tiles. It supports varioys layers and animations of the tiles.

It loads its data from the LUA files exported from the Tiled Map Editor software,
a de facto standard for editing tile maps.

@see: http://www.mapeditor.org/

*/
class TiledMapComponent : public ComponentTemplate<TiledMapComponent> {
    friend auto meta::registerMembers<TiledMapComponent>();
public:
    TiledMapComponent(EntityId id);
    TiledMapComponent(EntityId id, const sol::table& table);
    ~TiledMapComponent();

    json to_json() override;
    void load_json(const json& j) override;

    // ====== BEG OF REQUIREMENTS ======
    void draw_component_inspector() override;
    static void expose_to_lua(); 
    // ====== END OF REQUIREMENTS ======


    /**
    
    Loads the data from the LUA files exported from the Tiled Map Editor software.

    */
    void load_map();

   




    // ====== BEG OF MEMBERS ======

    /**
    
    It stores the name of the file that defined the map.

    */
    std::string m_map_filename;
    
    /**
    
    It stores the actual structure that contains the data of the tile map.
    That is the width and height of the map and tiles, their arrangement, its layers,
    the sprites of each tiles, the textures they are in, etc.

    */
    TiledMap m_map;

    /**
    
    Represents wether the map has been loaded and is already ready to be used (rendered
    and updated).
    
    */
    bool m_map_ready{ false };

    /**
    
    Represents the map layer in which gameplay happens. That is, the layers below
    this number will be considered background layers and the ones above it will be
    foreground layers.

    */
    int m_main_layer{ 0 };

    // ====== END OF MEMBERS ======

};

template<>
inline auto meta::registerName<TiledMapComponent>() {
    return "TiledMapComponent";
}

template<>
inline auto meta::registerMembers<TiledMapComponent>() {
    return members(
        member("filename", &TiledMapComponent::m_map_filename),
        member("mainLayer", &TiledMapComponent::m_main_layer)
    );
}


