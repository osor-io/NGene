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
#include <SFML/System/Vector2.hpp>

/**

Enum that represents the possible directions of movement
for a character in a 2D plattformer.

*/
enum InputMovementDirection {
    INPUT_MOVEMENT_RIGHT,
    INPUT_MOVEMENT_LEFT,
    INPUT_MOVEMENT_NONE
};

/**

Component that represents the functionality and behavious of an entity that is
being controlled and moved in a 2D platformer type of environment (e.g. A Mario Game
or a Super Meat Boy, etc).

It exposes paratemers that could be tweaked in favour of gameplay behaviour and then
the proper calculations are made so the proper forces, velocities, etc are applyed when
we want to perform an action. For example, based on how high we want to jump, how far we 
want to go, how fast we are, etc. we calculate the initial jump velocity that we should
apply to the entity when it jumps.

This is very useful when tweaking gameplay from a design point of view because we define
the behaviour of things like running and jumping in a much gameplay focused and easier to 
understand way. Instead of having to go through a trial and error process of determining the
right jump velocity so we can jump exactly 3 tiles high and 2 tiles far we can just set those
distances and the necessary parameters will be calculated for us.

*/
class PlatformerPhysicsComponent : public ComponentTemplate<PlatformerPhysicsComponent> {
    friend auto meta::registerMembers<PlatformerPhysicsComponent>();
public:
    PlatformerPhysicsComponent(EntityId id);
    PlatformerPhysicsComponent(EntityId id, const sol::table& table);
    ~PlatformerPhysicsComponent();

    json to_json() override;
    void load_json(const json& j) override;


    // ====== BEG OF REQUIREMENTS ======
    void draw_component_inspector() override;
    static void expose_to_lua();  // Go To Entity.cpp and call this
    // ====== END OF REQUIREMENTS ======

    /**
    
    Function called when trying to make the entity move in a particular horizontal direction.

    @see InputMovementDirection

    */
    void move(InputMovementDirection direction);
    
    /**
    
    Function called when we want to request that our entity performs a jump.

    This doesn't necessarily mean that our entity will jump, it just means that
    we want it to based on some interaction or some input from the player. But in
    certain situations, for example when the entity is already in the air and it is
    not able to do a double jump the entity will simply not jump even if we request it.
    
    */
    void jump();


    /**
    
    These are just general getters and setters for the parameters that define the movement
    of the entity that either return those parameters or modify them (while setting a flag
    to indicate that we need to perform internal recalculations of other members before
    using them)

    */

    float get_max_foot_speed() const { return m_max_foot_speed; }
    float get_horizontal_distance_to_peak() const { return m_horizontal_distance_to_peak; }
    float get_jump_height() const { return m_jump_height; }
    float get_downwards_gravity_scale() const { return m_downwards_gravity_scale; }

    void set_max_foot_speed(float value) { m_max_foot_speed = value; m_need_recalculation = true; }
    void set_horizontal_distance_to_peak(float value) { m_horizontal_distance_to_peak = value; m_need_recalculation = true; }
    void set_jump_height(float value) { m_jump_height = value; m_need_recalculation = true; }
    void set_downwards_gravity_scale(float value) { m_downwards_gravity_scale = value; m_need_recalculation = true; }







    // ====== BEG OF MEMBERS ======

    /**
    
    This represents that some important parameter has changed so we need to recalculate
    the necessary members before we use them in any sort of way to move or do a jump.

    */
    bool m_need_recalculation{ true };

    /**
    
    Represents the max speed that our character can reach horizontally.

    */
    float m_max_foot_speed;

    /**
    
    Represents the horizontal distance that our entity will travel
    if moving at max horizontal speed from the moment it jumps until it
    reaches the peak of the jump.

    If the '#' are tiles and our character jumps when going to the right
    at the end of the bottom tiles. And it barely reaches the top tiles, the
    double ended arrow represents this distance.


                                O
                                ######
                         O<---->          
                    ######              


    */
    float m_horizontal_distance_to_peak;

    /**
    
    Represents the vertical height that our entity can jump.

    If the '#' are tiles and our character jumps from the bottom 'O'
    to the top 'O' the double ended arrow represents this distance.

                           O ^
                             |    
                             |   
                           O v
                        #######
    
    */
    float m_jump_height;

    /**
    
    This represents how much the gravity is scaled when the entity is 
    traveling downwards. This is used commonly in platformers to avoid the 
    feeling of a jump being "floaty" and make the jumping mechanic feel
    crisper.

    General values tend to be arround 2x to 3x.
    
    */
    float m_downwards_gravity_scale;




    /**

    Here we store the gravity that is applyed normally to this entity based on the
    set parameters.

    */
    sf::Vector2f m_our_gravity;

    /**
    
    Here we store the initial jump velocity that we have to apply to the entity
    when it jumps to follow the parameters set previously.
    
    */
    sf::Vector2f m_initial_jump_velocity;

    /**
    
    Here we store the max velocity that the entity can move at any moment, 
    (especially vertically) even when we are applying some force that would
    make it move faster.

    */
    sf::Vector2f m_max_velocity;

    /**
    
    Here we store the current velocity that will be used to modify the position
    of the entity.

    */
    sf::Vector2f m_current_velocity;

    /**
    
    Here we store wether the entity is grounded or not
    (to know if it can jump, etc).
    
    */
    bool m_grounded{ false };

    /**
    
    Here we store if some event or player input has requested a jump to be 
    performed.
    
    */
    bool m_requested_jump{ false };

    // ====== END OF MEMBERS ======

};

template<>
inline auto meta::registerName<PlatformerPhysicsComponent>() {
    return "PlatformerPhysicsComponent";
}

template<>
inline auto meta::registerMembers<PlatformerPhysicsComponent>() {
    return members(
        member("maxFootSpeed", &PlatformerPhysicsComponent::m_max_foot_speed),
        member("distanceToPeak", &PlatformerPhysicsComponent::m_horizontal_distance_to_peak),
        member("jumpHeight", &PlatformerPhysicsComponent::m_jump_height),
        member("downwardsGravityScale", &PlatformerPhysicsComponent::m_downwards_gravity_scale),
        member("ourGravity", &PlatformerPhysicsComponent::m_our_gravity),
        member("initialJumpVelocity", &PlatformerPhysicsComponent::m_initial_jump_velocity),
        member("maxVelocity", &PlatformerPhysicsComponent::m_max_velocity),
        member("currentVelocity", &PlatformerPhysicsComponent::m_current_velocity),
        member("grounded", &PlatformerPhysicsComponent::m_grounded)
    );
}


