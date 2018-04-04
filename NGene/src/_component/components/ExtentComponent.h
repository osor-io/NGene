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

/**

Component that represents the extent that an entity has in relation to
the world. Not to be confused with the possible physical extent of an entity
that would be represented with a CollisionComponent.

It indicates where this entity is relevant in relation to the world and all the entities
that are in it. This is used for example in the our Chunk System and determines which
chunks of the world this entity is in or could be in. This, for example, allows us to
check for collisions only between entities which extents share chunks because this means
that they could be possibly colliding. If they don't share chunks then we consider that it
is impossible that they could be interacting (colliding in this example).

*/
class ExtentComponent : public ComponentTemplate<ExtentComponent> {
    friend auto meta::registerMembers<ExtentComponent>();
public:
    ExtentComponent(EntityId id);
    ExtentComponent(EntityId id, const sol::table& table);
    ~ExtentComponent();

    json to_json() override;
    void load_json(const json& j) override;


    // ====== BEG OF REQUIREMENTS ======
    void draw_component_inspector() override;
    static void expose_to_lua();
    // ====== END OF REQUIREMENTS ======


    void draw_rect();


    // ====== BEG OF MEMBERS ======

    /**

    This represents the extent that the extent has from its center to its
    bounds, that is, from the center to the right and left sides and from the center
    to the up and down sides. It could also be considered a half-extent.

    */
    sf::Vector2f m_extent{ 10.0f,10.0f };

    /**

    This represents the offset that the extent has in relation
    with the center of the entity, generally represented a TransformComponent.

    */
    sf::Vector2f m_offset{ 0.0f,0.0f };

    
    /**
    
    This represents how many chunks we should consider in each direction
    after the extent of the entity is not longer in them. Effectively this is
    a neater way to just increase the size of the extent of the component.

    Generally for normal entities the general value should be zero.

    */
    unsigned int m_extra_chunk_threshold{ 0 };
    
    // ====== END OF MEMBERS ======

};

template<>
inline auto meta::registerName<ExtentComponent>() {
    return "ExtentComponent";
}

template<>
inline auto meta::registerMembers<ExtentComponent>() {
    return members(
        member("extent", &ExtentComponent::m_extent),
        member("offset", &ExtentComponent::m_offset),
        member("extraChunkThreshold", &ExtentComponent::m_extra_chunk_threshold)
    );
}


