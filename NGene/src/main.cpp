#include "./utils/Debug.h"
#include "./File.h"

#include "./physics/ChunkManager.h"
#include "./debug/LoggingManager.h"
#include "./resources/TextFileManager.h"
#include "./resources/TextureManager.h"
#include "./input/InputManager.h"
#include "./time/TimeManager.h"
#include "./window/WindowManager.h"
#include "./render/RenderManager.h"
#include "./debug/DeveloperModeManager.h"
#include "./lua/LuaManager.h"

#include "./_entity/EntityManager.h"
#include "./_component/ComponentManager.h"
#include "./_system/SystemManager.h"

#include "./_system/systems/TileMapSystem.h"
#include "./_system/systems/InputSystem.h"
#include "./_system/systems/BehaviourSystem.h"
#include "./_system/systems/CameraSystem.h"
#include "./_system/systems/PhysicsSystem.h"
#include "./_system/systems/CollisionSystem.h"
#include "./_system/systems/TransformSystem.h"
#include "./_system/systems/RenderSystem.h"


void start_up() {

    LuaManager::get().start_up();
    ChunkManager::get().start_up();
    LoggingManager::get().start_up();
    TextFileManager::get().start_up();
    TextureManager::get().start_up();
    TimeManager::get().start_up();
    WindowManager::get().start_up();
    RenderManager::get().start_up();
    DeveloperModeManager::get().start_up();
    InputManager::get().start_up();

    SystemManager::get().start_up();
    ComponentManager::get().start_up();
    EntityManager::get().start_up();

    TileMapSystem::get().start_up();
    InputSystem::get().start_up();
    BehaviourSystem::get().start_up();
    CameraSystem::get().start_up();
    PhysicsSystem::get().start_up();
    CollisionSystem::get().start_up();
    TransformSystem::get().start_up();
    RenderSystem::get().start_up();

}

void shut_down() {

    RenderSystem::get().shut_down();
    TransformSystem::get().shut_down();
    CollisionSystem::get().shut_down();
    PhysicsSystem::get().shut_down();
    CameraSystem::get().shut_down();
    BehaviourSystem::get().shut_down();
    InputSystem::get().shut_down();
    TileMapSystem::get().shut_down();

    EntityManager::get().shut_down();
    ComponentManager::get().shut_down();
    SystemManager::get().shut_down();

    InputManager::get().shut_down();
    DeveloperModeManager::get().shut_down();
    RenderManager::get().shut_down();
    WindowManager::get().shut_down();
    TimeManager::get().shut_down();
    TextureManager::get().shut_down();
    TextFileManager::get().shut_down();
    LoggingManager::get().shut_down();
    ChunkManager::get().shut_down();
    LuaManager::get().shut_down();
}


inline void tick() {

    ChunkManager::get().update_entity_chunks();

    TileMapSystem::get().update();
    InputSystem::get().update();
    BehaviourSystem::get().update();
    CameraSystem::get().update();
    PhysicsSystem::get().update();
    CollisionSystem::get().update();

    TransformSystem::get().update();
}


inline void render() {

    WindowManager::get().fill_events();
    RenderManager::get().begin_frame();
    /*
    Everything that renders something goes in the
    next scope
    */
    {
        RenderSystem::get().update();
    }
    RenderManager::get().end_frame();

}


inline void post_render() {
    EntityManager::get().update_entities();
}


void load_default_state() {
	 
//#define LOAD_STATE_PROGRAMMATICALLY
//#define LOAD_STATE_FROM_JSON

#if defined(LOAD_STATE_PROGRAMMATICALLY) && defined(LOAD_STATE_FROM_JSON)
#error We are trying to load the default state both from code and from JSON
#endif

#ifdef LOAD_STATE_PROGRAMMATICALLY

	LOG("Default state will be loaded programatically");
	for (int i = 0; i < 2; ++i) {
		auto id = int{};
		if (i == 0)
			id = EntityManager::get().request_load_entity("DynamicObject");
		else
			id = EntityManager::get().request_load_entity("DummyDynamicObject");
	}
	const auto map_id = EntityManager::get().request_load_entity("DefaultMap");
	const auto camera_id = EntityManager::get().request_load_entity("DefaultCamera");
	return;

#elif defined LOAD_STATE_FROM_JSON

	auto path = "res/states/DefaultState.json";
	LOG("Default state will be loaded from the JSON file: " << path);
	assert(TextFileManager::get().exists_resource(path));
	auto s = TextFileManager::get().get_scoped_resource(path);
	auto j = json::parse(*s.resource);
	EntityManager::get().clear_and_load_entities(j);
	return;

#else

	auto path = "res/scripts/LoadDefaultState.lua";
	LOG("Default state will be loaded from the LUA Script: " << path);
	assert(TextFileManager::get().exists_resource(path));
	auto default_state_script = TextFileManager::get().get_scoped_resource(path);
	LUA.script(*default_state_script.resource);
	return;

#endif

}


#include "./test/testmain.inl"

int main() {

    /*
    @@DEBUG @@TODO @@REMOVE
    Use this to test individual things
    */
    //return test();

    start_up();
    load_default_state();

    TimeManager::get().end_of_frame();

    while (WindowManager::get().is_window_open()) {

        auto beg = TimeManager::get().query_cycle_counter();

        tick();

        auto end = TimeManager::get().query_cycle_counter();
        auto ms = TimeManager::get().cycles_to_ms(end - beg);

        /*
        @@NOTE

        Use this to know the time needed to update the game (without rendering)

        Bear (ROAR!) in mind that using this will tank framerate since we are basically
        printing a number every frame and maybe writing it to a log file, which takes a ton
        of time we don't want to spend in the middle of a frame. That said, the reading for
        how lonk we took to tick the game is still right, it is just that the framerate will
        be bad and keep getting worse with each printing since we are accumulating stuff to print
        to standard output.

        */
        //LOG_NAMED(ms);

        render();
        post_render();

    }

    auto es = EntityManager::get().serialize_entities();

    write_to_file("lastRunState.json", es.dump(4).c_str());

    shut_down();

    //@@NOTE: Use this to see the console when shutting down
    //ENDL;
    //press_to_continue();
    return 0;
}