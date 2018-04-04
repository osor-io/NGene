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

Marks the different types that a collider might be, this is
important because it defines different behaviours and interactions
when one collider intersects with other and that might be prevent
them from colliding (Physical Objects with Terrain) or trigger
some action.

*/
enum class ColliderType {
    TERRAIN,
    MOVING_OBJECT,
};

/**

This marks the direction that other colliders are colliding with
this particular collider. Not all the collider types set these flags
when they collide, only the relevant ones that represent physical things
such as (TERRAING or MOVING_OBJECT). Things like triggers will not update
these flags.

This is useful for other systems that care about the direction of a collision
with a physical object such as the PhysicsSystem that checks if an entity
is grounded or not (colliding with a physical object that is below it).

*/
enum CollisionDirectionFlags {
    COLLISION_DIRECTION_DOWN = 1,
    COLLISION_DIRECTION_UP = (1 << 1),
    COLLISION_DIRECTION_LEFT = (1 << 2),
    COLLISION_DIRECTION_RIGHT = (1 << 3),
};

/**

Component that represents any sort of collision that might happen in the game.
It does this in a general way and depending on its type it has a different kind
of interactions with other components.

@see ColliderType

*/
class CollisionComponent : public ComponentTemplate<CollisionComponent> {
    friend auto meta::registerMembers<CollisionComponent>();
public:
    CollisionComponent(EntityId id);
    CollisionComponent(EntityId id, const sol::table& table);
    ~CollisionComponent();

    json to_json() override;
    void load_json(const json& j) override;


    // ====== BEG OF REQUIREMENTS ======
    void draw_component_inspector() override;
    static void expose_to_lua();
    // ====== END OF REQUIREMENTS ======

    void draw_rect();



    // ====== BEG OF MEMBERS ======

    /**

    This represents the extent that the collider has from its center to its
    bounds, that is, from the center to the right and left sides and from the center
    to the up and down sides. It could also be considered a half-extent.

    */
    sf::Vector2f m_extent{ 10.0f,10.0f };

    /**
    
    This represents the offset that the collider has in relation
    with the center of the entity, generally represented a TransformComponent.

    */
    sf::Vector2f m_offset{ 0.0f,0.0f };

    /**

    This represents the type of collider that this particular CollisionComponent
    is.

    @see ColliderType

    Here it is a good article outlining some of the interesting types of collisions
    to support, I'm thinking about prioritizing Moving Plattforms and One Way Plattforms:

    http://higherorderfun.com/blog/2012/05/20/the-guide-to-implementing-2d-platformers/

    */
    ColliderType m_type{ ColliderType::TERRAIN };

    /**

    These are the flags set for the moving entities that containg which directions
    have been hit in relation to Terraing-like colliders, this is especially usefull to know
    if an entity is grounded or sliding on a wall.
    
    @see CollisionDirectionFlags

    */
    unsigned int m_moving_collision_direction_flags{ 0u };

    // ====== END OF MEMBERS ======

};

template<>
inline auto meta::registerName<CollisionComponent>() {
    return "CollisionComponent";
}

template<>
inline auto meta::registerMembers<CollisionComponent>() {
    return members(
        member("extent", &CollisionComponent::m_extent),
        member("offset", &CollisionComponent::m_offset),
        member("type", &CollisionComponent::m_type)
    );
}


