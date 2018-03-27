#pragma once
#include <Manager.h>
#include <SFML/System/Clock.hpp>
#include <memory>

//@@NOTE  @@PLATTFORM
#include <windows.h>


#define DELTA_TIME TimeManager::get().get_delta_time()
#define DELTA_SECONDS TimeManager::get().get_delta_time().asSeconds()
#define DELTA_MILLISECONDS TimeManager::get().get_delta_time().asMilliseconds();
#define DELTA_MICROSECONDS TimeManager::get().get_delta_time().asMicroseconds();


using CounterType = long long;

class TimeManager : public Manager<TimeManager> {
    friend class CRSP<TimeManager>;
private:
    TimeManager();
    ~TimeManager();

public:

    void start_up() override;
    void shut_down() override;

    sf::Time end_of_frame();
    sf::Time get_delta_time() const ;

    CounterType query_cycle_counter();

    float cycles_to_ms(CounterType counter);

    void expose_to_lua();

private:
    std::unique_ptr<sf::Clock> m_frame_clock;
    sf::Time m_frame_delta_time{};
    CounterType m_internal_frequency{};
};
