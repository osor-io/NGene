#pragma once
#include "../ComponentTemplate.h"
#include <tmxlite/Map.hpp>
#include <SFMLOrthogonalLayer.hpp>
#include "../maps/TiledMap.h"

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
    static void expose_to_lua();  // Go To Entity.cpp and call this
    // ====== END OF REQUIREMENTS ======

    void load_map();

    /*
    Go to ComponentManager.h and register the name and constructor
    of this component so it can be instantiated.
    */


    /*
    When adding a member reMember to consider registering it
    in "expose_to_lua()" and in "meta::registerMembers" as well
    as add it in "drawComponentInspector()" to be able to see it
    */
    // ====== BEG OF MEMBERS ======
    std::string m_map_filename;
    TiledMap m_map;
    bool m_map_ready{ false };
    bool m_need_to_create_colliders{ false };
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


