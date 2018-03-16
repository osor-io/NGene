#include "InputManager.h"
#include "../lua/LuaManager.h"


InputManager::InputManager()
{
}


InputManager::~InputManager()
{
}

void InputManager::start_up() {
    register_sfml_input();
}


void InputManager::shut_down() {

}

bool InputManager::is_key_pressed(sf::Keyboard::Key key) {
    return sf::Keyboard::isKeyPressed(key);
}

bool InputManager::is_button_pressed(unsigned int button) {

    /*
    We are currently checking in all connected joysticks
    */
    for (auto i = 0; i < sf::Joystick::Count; ++i) {
        if (sf::Joystick::isConnected(i) && sf::Joystick::isButtonPressed(i, button))
            return true;
    }

    return false;

}

float InputManager::get_axis_position(sf::Joystick::Axis axis) {

    /*
    We are currently checking in all connected joysticks
    */
    for (auto i = 0; i < sf::Joystick::Count; ++i) {
        if (sf::Joystick::isConnected(i)) {
            return sf::Joystick::getAxisPosition(i, axis);
        }
    }

    return .0f;
}


#define KEY(x) #x, sf::Keyboard::Key:: ## x

#define AXIS(x) #x, sf::Joystick::Axis:: ## x


void InputManager::register_sfml_input() {

    //@@TODO: Register Custom Enum for Lua with the buttons of the XBox Controller

#pragma region lua_input_enums
    LUA.new_enum("Key",
        KEY(A),
        KEY(B),
        KEY(C),
        KEY(D),
        KEY(E),
        KEY(F),
        KEY(G),
        KEY(H),
        KEY(I),
        KEY(J),
        KEY(K),
        KEY(L),
        KEY(M),
        KEY(N),
        KEY(O),
        KEY(P),
        KEY(Q),
        KEY(R),
        KEY(S),
        KEY(T),
        KEY(U),
        KEY(V),
        KEY(W),
        KEY(X),
        KEY(Y),
        KEY(Z),
        KEY(Num0),
        KEY(Num1),
        KEY(Num2),
        KEY(Num3),
        KEY(Num4),
        KEY(Num5),
        KEY(Num6),
        KEY(Num7),
        KEY(Num8),
        KEY(Num9),
        KEY(Escape),
        KEY(LControl),
        KEY(LShift),
        KEY(LAlt),
        KEY(LSystem),
        KEY(RControl),
        KEY(RShift),
        KEY(RAlt),
        KEY(RSystem),
        KEY(Menu),
        KEY(LBracket),
        KEY(RBracket),
        KEY(SemiColon),
        KEY(Comma),
        KEY(Period),
        KEY(Quote),
        KEY(Slash),
        KEY(BackSlash),
        KEY(Tilde),
        KEY(Equal),
        KEY(Dash),
        KEY(Space),
        KEY(Return),
        KEY(BackSpace),
        KEY(Tab),
        KEY(PageUp),
        KEY(PageDown),
        KEY(End),
        KEY(Home),
        KEY(Insert),
        KEY(Delete),
        KEY(Add),
        KEY(Subtract),
        KEY(Multiply),
        KEY(Divide),
        KEY(Left),
        KEY(Right),
        KEY(Up),
        KEY(Down),
        KEY(Numpad0),
        KEY(Numpad1),
        KEY(Numpad2),
        KEY(Numpad3),
        KEY(Numpad4),
        KEY(Numpad5),
        KEY(Numpad6),
        KEY(Numpad7),
        KEY(Numpad8),
        KEY(Numpad9),
        KEY(F1),
        KEY(F2),
        KEY(F3),
        KEY(F4),
        KEY(F5),
        KEY(F6),
        KEY(F7),
        KEY(F8),
        KEY(F9),
        KEY(F10),
        KEY(F11),
        KEY(F12),
        KEY(F13),
        KEY(F14),
        KEY(F15),
        KEY(Pause));

    LUA.new_enum("Axis",
        AXIS(X),
        AXIS(Y),
        AXIS(Z),
        AXIS(R),
        AXIS(U),
        AXIS(V),
        AXIS(PovX),
        AXIS(PovY)
    );

#pragma endregion Writing all the enum types necessary for the input


    LUA.set_function("isKeyPressed", InputManager::is_key_pressed);

    LUA.set_function("isButtonPressed", InputManager::is_button_pressed);

    LUA.set_function("getAxisPosition", InputManager::get_axis_position);

}
