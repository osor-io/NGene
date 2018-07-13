#include <iostream>

#include "imgui.h"
#include "imgui-SFML.h"

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/OpenGL.hpp>

#include <meta.h>

#include <Debug.h>


int test_opengl();
int test_spritesheets();


int test() {
	return test_opengl();
}

int test_opengl() {


	// create the window
	sf::ContextSettings settings;
	settings.depthBits = 24;
	settings.stencilBits = 8;
	settings.antialiasingLevel = 4;
	settings.majorVersion = 3;
	settings.minorVersion = 0;
	sf::RenderWindow window(sf::VideoMode(800, 600), "OpenGL Test", sf::Style::Default, settings);

	window.setVerticalSyncEnabled(true);

	// activate the window
	window.setActive(true);

	// load resources, initialize the OpenGL states, ...

	// run the main loop
	bool running = true;
	while (running)
	{
		// handle events
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
			{
				// end the program
				running = false;
			}
			else if (event.type == sf::Event::Resized)
			{
				// adjust the viewport when the window is resized
				glViewport(0, 0, event.size.width, event.size.height);
			}
		}

		glViewport(0, 0, window.getSize().x, window.getSize().y);
		{
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			glMatrixMode(GL_MODELVIEW);
			glLoadIdentity();
			glPushMatrix();
			glBegin(GL_QUADS);
			auto size = 1.f;
			glVertex2f(-size / 2.0f, -size / 2.0f);
			glVertex2f(size / 2.0f, -size / 2.0f);
			glVertex2f(size / 2.0f, size / 2.0f);
			glVertex2f(-size / 2.0f, size / 2.0f);
			glEnd();
			glPopMatrix();
		}

		// end the current frame (internally swaps the front and back buffers)
		window.display();
	}

	// release resources...

	return 0;

	return 0;
}



#include "../render/sprites/Spritesheet.h"
int test_spritesheets() {
	auto spritesheet = Spritesheet("./res/assets/spritesheet_test.png", Spritesheet::SpritesheetMorphology::SQUARE);

	LOG_NAMED(spritesheet.get_sprites().size());
	LOG_NAMED(spritesheet.get_rows());
	LOG_NAMED(spritesheet.get_cols());
	LOG_NAMED(spritesheet.get_sprite_width());
	LOG_NAMED(spritesheet.get_sprite_height());

	press_to_continue();
	return 0;
}

