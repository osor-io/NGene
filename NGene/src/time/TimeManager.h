#pragma once
#include <Manager.h>
#include <SFML/System/Clock.hpp>
#include <memory>

#define DELTA_TIME TimeManager::get().getDeltaTime()
#define DELTA_SECONDS TimeManager::get().getDeltaTime().asSeconds()

class TimeManager : public Manager<TimeManager> {
    friend class CRSP<TimeManager>;
private:
    TimeManager();
    ~TimeManager();

public:

    void startUp() override;
    void shutDown() override;

    sf::Time endOfFrame();
    sf::Time getDeltaTime();

private:
    std::unique_ptr<sf::Clock> m_frameClock;
    sf::Time m_frameDeltaTime{};
};
