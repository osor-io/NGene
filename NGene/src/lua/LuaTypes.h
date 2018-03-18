#pragma once

#include <SFML/Window.hpp>

struct LuaVector2f {
    static sf::Vector2f to_sf(LuaVector2f vector) { return sf::Vector2f(vector.x, vector.y); }
    LuaVector2f() {}
    LuaVector2f(const sf::Vector2f vector) : x(vector.x), y(vector.y) {}
    float x;
    float y;
};