#include "PhysicsSystem.h"
#include <../time/TimeManager.h>

#include <../_component/components/PlatformerPhysicsComponent.h>
#include <../_component/components/TransformComponent.h>
#include <../_component/components/CollisionComponent.h>

PhysicsSystem::PhysicsSystem() {}

PhysicsSystem::~PhysicsSystem() {}

void PhysicsSystem::start_up() {

	/*
	We register the system so we can receive the entities with the components we are interested in.
	*/
	registerSystem();
}

void PhysicsSystem::shut_down() {

}

void PhysicsSystem::update() {


	auto delta_time = DELTA_SECONDS;
	if (delta_time > config::max_timestep) delta_time = config::max_timestep;

	for (auto& e : m_entities) {

		/*
		Useful resource for the jumps: https://www.youtube.com/watch?v=hG9SzQxaCm8&t=453s
		*/

		auto entity = e.second;
		auto platformer_component = entity->get_component<PlatformerPhysicsComponent>();
		auto transform_component = entity->get_component<TransformComponent>();
		auto collision_component = entity->get_component<CollisionComponent>();


		// We recalculate our physics constants based on input variables if needed
		if (platformer_component->m_need_recalculation) {

			platformer_component->m_need_recalculation = false;

			// Recalculate the initial jump velocity based on jump height, foot speed and horizontal distance to the peak of the jump.
			platformer_component->m_initial_jump_velocity.y =
				(2.f * platformer_component->m_jump_height * platformer_component->m_max_foot_speed)
				/
				(platformer_component->m_horizontal_distance_to_peak);

			// Recalculate our gravity based on jump height, foot speed and horizontal distance to the peak of the jump.
			platformer_component->m_our_gravity.y =
				(-2.f * platformer_component->m_jump_height * platformer_component->m_max_foot_speed * platformer_component->m_max_foot_speed)
				/
				(platformer_component->m_horizontal_distance_to_peak * platformer_component->m_horizontal_distance_to_peak);

			// We calculate the max gravity (when it's scaled when the entity is falling)
			auto scaled_gravity = (platformer_component->m_our_gravity.y * platformer_component->m_downwards_gravity_scale);

			/*
			We are limiting the falling velocity to the velocity that then entity
			has when jumping and landing from an in-place jump where the initial Y
			position is the same as the final Y position.
			*/
			platformer_component->m_max_velocity.y = abs(
				scaled_gravity
				*
				sqrt(-platformer_component->m_jump_height / (scaled_gravity / 2.f)));

			// And the max velocity horizontally is our max foot velocity.
			platformer_component->m_max_velocity.x = platformer_component->m_max_foot_speed;


			// Update the horizontal acceleration based on foot speed and the time it takes to reach it
			platformer_component->m_our_horizontal_acceleration.y = 0.f;
			if (platformer_component->m_time_to_max_foot_speed != 0.f) {
				platformer_component->m_our_horizontal_acceleration.x = platformer_component->m_max_foot_speed / platformer_component->m_time_to_max_foot_speed;
			}
			else {
				platformer_component->m_our_horizontal_acceleration.x = std::numeric_limits<float>::max();
			}

			// Update the horizontal deceleration based on foot speed and the time it takes to reach zero from it
			platformer_component->m_our_horizontal_deceleration.y = 0.f;
			if (platformer_component->m_time_to_max_foot_speed != 0.f) {
				platformer_component->m_our_horizontal_deceleration.x = -platformer_component->m_max_foot_speed / platformer_component->m_time_to_stop;
			}
			else {
				platformer_component->m_our_horizontal_deceleration.x = -std::numeric_limits<float>::max();
			}

		}


		// We check if we are grounded or not
		platformer_component->m_grounded = ((collision_component->m_moving_collision_direction_flags & CollisionDirectionFlags::COLLISION_DIRECTION_DOWN) != 0u);

		//
		// @@TODO: Check why this flag seems to not be set every time.
		//
		auto hitting_top = ((collision_component->m_moving_collision_direction_flags & CollisionDirectionFlags::COLLISION_DIRECTION_UP) != 0u);

		auto acceleration = sf::Vector2f(0.0f, 0.0f);

		if (hitting_top) { // The entity can't go upwards
			
			// The velocity going up is reduced to zero
			platformer_component->m_current_velocity.y = 0.f;
		}
		else if (platformer_component->m_grounded) { // Update the velocity and acceleration when we are grounded

			// Y Gravity stays at zero since the ground applies an exact force that opposes it
			platformer_component->m_current_velocity.y = 0.f;

			/*
			We allow the entity to jump and we don't push it downwards since
			it is going to collide with terrain anyway. This way we avoid an
			extra collision check each frame we are grounded.
			*/

			if (platformer_component->m_requested_jump) {
				platformer_component->m_current_velocity += platformer_component->m_initial_jump_velocity;
				platformer_component->m_requested_jump = false;
				platformer_component->m_grounded = false;
			}

		}
		else {	// Update the velocity and acceleration when we are flying

			//
			// If we are not grounded then we have to apply downwards gravity
			//

			if (platformer_component->m_current_velocity.y > 0.f) {
				// Going up (normal gravity)
				acceleration += platformer_component->m_our_gravity;
			}
			else {
				// Going Down (gravity scaled for a plattformy feel)
				acceleration += platformer_component->m_our_gravity * platformer_component->m_downwards_gravity_scale;
			}

		}

		// Update the velocity and acceleration based on horizontal movement
		auto stop_horizontal_velocity_at_zero = false;
		{
			auto add_stop_acceleration = false;

			if (platformer_component->m_requested_movement[INPUT_MOVEMENT_LEFT] ^ platformer_component->m_requested_movement[INPUT_MOVEMENT_RIGHT]) { // Input in one direction

				//
				// We add the acceleration towards the proper direction and if we were moving in the opposite
				// direction of the one we are going to move we add the "deceleration" used to stop so we
				// can move faster towards where we want to go.
				//

				if (platformer_component->m_requested_movement[INPUT_MOVEMENT_LEFT]) {
					add_stop_acceleration = platformer_component->m_current_velocity.x > 0;
					acceleration -= (platformer_component->m_our_horizontal_acceleration);
				}

				if (platformer_component->m_requested_movement[INPUT_MOVEMENT_RIGHT]) {
					add_stop_acceleration = platformer_component->m_current_velocity.x < 0;
					acceleration += (platformer_component->m_our_horizontal_acceleration);
				}
			}
			else {  // Input in both direction or none (so we should stop)
				add_stop_acceleration = true;	// We should add acceleration to stop the entity
				stop_horizontal_velocity_at_zero = true;	// And the entity shouldn't accelerate in the other direction it was going
			}

			// We only add the "deceleration" to stop if we were actually moving
			if (add_stop_acceleration && abs(platformer_component->m_current_velocity.x) > config::velocity_zero_threshold) {
				acceleration +=
					(platformer_component->m_current_velocity.x > 0 ?
						platformer_component->m_our_horizontal_deceleration :
						-platformer_component->m_our_horizontal_deceleration);
			}

			//
			// Reset the movement flags
			//
			for (auto i = 0; i < INPUT_MOVEMENT_DIRECTION_COUNT; ++i) {
				platformer_component->m_requested_movement[i] = false;
			}
		}


		//
		// Update the current velocity given the acceleration
		//
		auto previous_velocity = platformer_component->m_current_velocity;
		if (stop_horizontal_velocity_at_zero) {	// If the acceleration causes the velocity to change sign we stop at zero
			platformer_component->m_current_velocity += acceleration * delta_time;
			if (platformer_component->m_current_velocity.x * previous_velocity.x < 0) {
				platformer_component->m_current_velocity.x = 0.f;
				// We make sure we set the right acceleration that would have taken us to that velocity to use it in the Verlet Integration formula
				acceleration.x = (platformer_component->m_current_velocity.x - previous_velocity.x) / delta_time;
			}
		}
		else {	// We apply acceleration normally
			platformer_component->m_current_velocity += acceleration * delta_time;
		}

		// Restrict the Y component of the velocity to the max velocity
		// to limit the speed at which the entity can fall
		if (abs(platformer_component->m_current_velocity.y) > platformer_component->m_max_velocity.y) {
			platformer_component->m_current_velocity.y = platformer_component->m_current_velocity.y > 0 ?
				platformer_component->m_max_velocity.y :
				-platformer_component->m_max_velocity.y;
			// We make sure we set the right acceleration that would have taken us to that velocity to use it in the Verlet Integration formula
			acceleration.y = (platformer_component->m_current_velocity.y - previous_velocity.y) / delta_time;
		}


		// And restrict the horizontal velocity to the maximum foot speed
		if (abs(platformer_component->m_current_velocity.x) > platformer_component->m_max_velocity.x) {
			platformer_component->m_current_velocity.x = platformer_component->m_current_velocity.x > 0 ?
				platformer_component->m_max_velocity.x :
				-platformer_component->m_max_velocity.x;
			// We make sure we set the right acceleration that would have taken us to that velocity to use it in the Verlet Integration formula
			acceleration.x = (platformer_component->m_current_velocity.x - previous_velocity.x) / delta_time;
		}



		// If velocity is really close to zero we just zero it
		if (abs(platformer_component->m_current_velocity.x) < config::velocity_zero_threshold) platformer_component->m_current_velocity.x = 0.f;
		if (abs(platformer_component->m_current_velocity.y) < config::velocity_zero_threshold) platformer_component->m_current_velocity.y = 0.f;


		//
		// @@NOTE: Y is being flipped because SFML's Y coordinate
		// grows when going down (The origin point (0,0) is top-left) which means
		// that a negative Y velocity/acceleration would mean that the entity is going
		// up. For our logic I prefer to have a negative velocity that means
		// something is going down.
		//
		auto applied_velocity = platformer_component->m_current_velocity;
		applied_velocity.y = -applied_velocity.y;
		acceleration.y = -acceleration.y;


		/*
		@@NOTE

		Using Verlet Integration to calculate new velocity and position.
		This seems to be more than good enough for our purposes, that is,
		better than Euler integration (which is inconsistent with variable
		delta times) but not as expensive as Runge-Kutta methods while being
		quite good still.

		@see: https://en.wikipedia.org/wiki/Euler_method
		@see: https://en.wikipedia.org/wiki/Verlet_integration
		@see: https://gafferongames.com/post/integration_basics/
		@see: https://www.av8n.com/physics/symplectic-integrator.htm
		@see: https://www.saylor.org/site/wp-content/uploads/2011/06/MA221-6.1.pdf

		*/


		//
		// Finally we use Verlet Integration to calculate the position at
		// which the entity should be based on the velocity and acceleration.
		//
		// This method is more numerically stable and time reversable. It's
		// error is also constrained to each iteration which means that we 
		// don't have to worry about it accumulating error in subsequent frames.
		// Also, with variable time steps ("delta_time"s) this method is still
		// quite reliable and stable.
		//
		transform_component->m_position +=
			(applied_velocity * delta_time)
			+
			((acceleration * delta_time * delta_time) / 2.f);


	} // end of loop for each entity

}

void PhysicsSystem::register_entity(Entity& entity) {
	if (entity.has_component<PlatformerPhysicsComponent>() && entity.has_component<TransformComponent>() && entity.has_component<CollisionComponent>()) {
		m_entities[entity.get_id()] = &entity;
	}
}

void PhysicsSystem::deregister_entity(EntityId id) {
	m_entities.erase(id);
}


