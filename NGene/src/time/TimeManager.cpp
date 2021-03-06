#include "TimeManager.h"
#include "../lua/LuaManager.h"

TimeManager::TimeManager() {}


TimeManager::~TimeManager() {}


void TimeManager::start_up() {
    m_frame_clock = std::make_unique<sf::Clock>();
    m_frame_clock->restart();

    auto frequency = LARGE_INTEGER{};

    QueryPerformanceFrequency(&frequency);
    m_internal_frequency = frequency.QuadPart;

    expose_to_lua();
}


void TimeManager::shut_down() {
    m_frame_clock.reset();
}


sf::Time TimeManager::end_of_frame() {
    m_frame_delta_time = m_frame_clock->restart();
    return m_frame_delta_time;
}

sf::Time TimeManager::get_delta_time() const {
    return m_frame_delta_time;
}

long long TimeManager::query_cycle_counter() {
    auto counter = LARGE_INTEGER{};
    QueryPerformanceCounter(&counter);
    return counter.QuadPart;
}

float TimeManager::cycles_to_ms(CounterType counter) {
    return (1000 * counter) / m_internal_frequency;
}

void TimeManager::expose_to_lua(){
    LUA.set_function("getDeltaTime", [this]() {
        return this->get_delta_time().asSeconds();
    });
}

