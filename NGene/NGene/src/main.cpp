#include <SFML/Graphics.hpp>
#include <sol.hpp> // http://sol2.readthedocs.io/en/latest/tutorial/getting-started.html

int main()
{
	{ //Testing lua
		sol::state lua;
		lua.open_libraries(sol::lib::base);

		lua.script("print('bark bark bark!')");
	}

	{ //Testing SFML
		sf::RenderWindow window(sf::VideoMode(200, 200), "SFML works!");
		sf::CircleShape shape(100.f);
		shape.setFillColor(sf::Color::Green);

		while (window.isOpen())
		{
			sf::Event event;
			while (window.pollEvent(event))
			{
				if (event.type == sf::Event::Closed)
					window.close();
			}

			window.clear();
			window.draw(shape);
			window.display();
		}
	}

	return 0;
}