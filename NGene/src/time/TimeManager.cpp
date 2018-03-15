#include "TimeManager.h"



TimeManager::TimeManager() {}


TimeManager::~TimeManager() {}


void TimeManager::start_up() {
    m_frameClock = std::make_unique<sf::Clock>();
    m_frameClock->restart();
}


void TimeManager::shut_down() {
    m_frameClock.reset();
}


sf::Time TimeManager::endOfFrame() {
    m_frameDeltaTime = m_frameClock->restart();
    return m_frameDeltaTime;
}

sf::Time TimeManager::getDeltaTime() {
    return m_frameDeltaTime;
}

