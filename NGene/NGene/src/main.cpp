#include <iostream>

#include "imgui.h"
#include "imgui-SFML.h"

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/CircleShape.hpp>

#include <sol.hpp> // http://sol2.readthedocs.io/en/latest/tutorial/getting-started.html

#include <meta.h>

#include <Debug.h>
#include "./utils/OtherUtils.h"

struct ExampleStruct {
    friend auto meta::registerMembers<ExampleStruct>();

    int someInt{ 0 };
    float someFloat{ 3.0f };
};

template<>
inline auto meta::registerName<ExampleStruct>() {
    return "ExampleStruct";
}

template<>
inline auto meta::registerMembers<ExampleStruct>() {
    return members(
        member("someInt", &ExampleStruct::someInt),
        member("someFloat", &ExampleStruct::someFloat)
    );
}

int testDependencies() {
    { //Testing lua
        std::cout << " ====== " << "TESTING LUA" << " ====== " << std::endl;
        sol::state lua;
        lua.open_libraries(sol::lib::base);

        lua.script("print('Yeah, this is being printed from LUA!!!')");
        std::cout << std::endl;
    }


    { //Testing SFML + ImGUI + MetaStuff

        ExampleStruct aux{};
        std::cout << " ====== " << "TESTING SFML, ImGUI and MetaStuff" << " ====== " << std::endl;

        std::cout << "The initial values for the ExampleClass are:" << std::endl;
        meta::doForAllMembers<ExampleStruct>([&aux](auto& member) {
            using MemberT = meta::get_member_type<decltype(member)>;
            auto name = member.getName();
            std::cout << name << ": " << member.get(aux) << std::endl;
        });
        std::cout << "Change the values in the debug window and close the main window" << std::endl;
        std::cout << std::endl;

        sf::RenderWindow window(sf::VideoMode(640, 480), "Furrula");
        window.setFramerateLimit(60);
        ImGui::SFML::Init(window);


        sf::Clock deltaClock;
        while (window.isOpen()) {
            sf::Event event;
            while (window.pollEvent(event)) {
                ImGui::SFML::ProcessEvent(event);

                if (event.type == sf::Event::Closed) {
                    window.close();
                }
            }

            ImGui::SFML::Update(window, deltaClock.restart());

            ImGui::Begin(meta::getName<ExampleStruct>());
            meta::doForAllMembers<ExampleStruct>([&aux](auto& member) {
                using MemberT = meta::get_member_type<decltype(member)>;
                auto name = member.getName();

                if (std::is_same<MemberT, int>::value) {
                    ImGui::InputInt(name, (int*)&member.get(aux));
                }
                else if (std::is_same<MemberT, float>::value) {
                    ImGui::InputFloat(name, (float*)&member.get(aux));
                }


            });
            ImGui::End();

            window.clear(sf::Color::White);

            ImGui::SFML::Render(window);
            window.display();
        }

        std::cout << "The final values for the ExampleClass are:" << std::endl;
        meta::doForAllMembers<ExampleStruct>([&aux](auto& member) {
            using MemberT = meta::get_member_type<decltype(member)>;
            auto name = member.getName();
            std::cout << name << ": " << member.get(aux) << std::endl;
        });
        std::cout << std::endl;

        ImGui::SFML::Shutdown();
    }

    return 0;
}


#include "./_entity/Entity.h"
#include "./_component/components/SimplePhraseComponent.h"
#include "./_component/components/SimpleGraphicsComponent.h"
#include "./_entity/EntityManager.h"

int testECS() {

    {
        auto e = Entity{};

        auto comp = e.makeComponent<SimplePhraseComponent>();
        comp = e.makeComponent<SimplePhraseComponent>();

        comp->setPhrase("This is ma phrase");

        std::cout << e.getComponent<SimplePhraseComponent>()->getPhrase() << std::endl;
    }
    std::cout << std::endl << std::endl;

    {
        auto e = Entity{};
        auto t = sol::table{};
        //EntityFactory::get().addComponentToEntity<SimpleGraphicsComponent>(e, t); // the extraterrestrial
        //EntityFactory::get().addComponentToEntity<SimplePhraseComponent>(e, t); // the extraterrestrial

        auto c = std::make_unique<SimpleGraphicsComponent>();
    }

    {
        auto arr = new std::array<bool*, 1048893>();

        for (auto elem : *arr) {
            elem = new bool;
        }

        for (auto elem : *arr) {
            delete elem;
        }
    }
    std::cout << std::endl << std::endl;

    {
        sol::state lua;
        lua.open_libraries(sol::lib::base);

        lua.script(R"(
            Entities = {
                Cosa = {
                    SimplePhraseComponent = {
                        Phrase = "I'm saying hi from this lua Object!! :D"
                    },
                    SimpleGraphicsComponent = {
                        Filename = "file.png"
                    }
                }
            }
        )");

        auto e = EntityManager::get().loadEntity(lua["Entities"], "Cosa");

        LOG_NAMED(e->getComponent<SimplePhraseComponent>()->getPhrase());
        LOG_NAMED(e->getComponent<SimpleGraphicsComponent>()->getFilename());


    }
    std::cout << std::endl << std::endl;

    return 0;
}

int main() {
    testECS();
    pressToContinue();
    return 0;
}