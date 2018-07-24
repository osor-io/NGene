#include <iostream>

#include "imgui.h"
#include "imgui-SFML.h"

#include <GL/glew.h>

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/OpenGL.hpp>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>


#include <meta.h>

#include <Debug.h>

#include "../render/buffers/ArrayBuffer.h"
#include "../render/buffers/ElementBuffer.h"
#include "../render/buffers/VertexArray.h"

int test_render_texture_crt_simulation();
int test_modern_opengl_crt_simulation();
int test_modern_opengl_crt_shape();
int test_legacy_opengl_crt_shape();
int test_spritesheets();


int test() {
	return test_render_texture_crt_simulation();
}



#include "../render/crt/CRTRenderer.h"


int test_render_texture_crt_simulation() {


	// Settings for the window
	sf::ContextSettings settings;
	settings.depthBits = 24;
	settings.stencilBits = 8;
	settings.antialiasingLevel = 8;
	settings.majorVersion = 4;
	settings.minorVersion = 6;

	bool exit = false;

	while (!exit)
	{

		// Create the main window

		sf::RenderWindow window(sf::VideoMode(256 * 4, 224 * 4), "Modern OpenGL Test", sf::Style::Default, settings);
		window.setVerticalSyncEnabled(true);
		ImGui::SFML::Init(window);

		// Make the window the active window for OpenGL calls
		window.setActive(true);

		// Enable Z-buffer read and write
		glEnable(GL_DEPTH_TEST);
		glDepthMask(GL_TRUE);
		glClearDepth(1.f);

		glewInit();

		// Set Clear Color
		glClearColor(0.1f, 0.1f, 0.1f, 1.f);


		//
		// Set up things to render HERE
		//
		CRTRenderer crt_renderer(window);

		sf::RenderTexture texture;
		texture.create(200,200);
		texture.setSmooth(false);
		texture.setRepeated(false);

		GLenum error = glGetError();
		if (error != GL_NO_ERROR) {
			LOG_ERROR("OpenGL Error while initializing CRT renderer (" << error << "): " << glewGetErrorString(error));
		}


		// Create a clock for measuring the time elapsed
		sf::Clock clock;
		sf::Clock frame_clock;

		// Start game loop
		while (window.isOpen())
		{

			// Process events
			sf::Event event;
			while (window.pollEvent(event))
			{
				ImGui::SFML::ProcessEvent(event);

				// Close window: exit
				if (event.type == sf::Event::Closed)
				{
					exit = true;
					window.close();
				}

				// Escape key: exit
				if ((event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::Escape))
				{
					exit = true;
					window.close();
				}


				// Adjust the viewport when the window is resized
				if (event.type == sf::Event::Resized)
				{
					// Make the window the active window for OpenGL calls
					window.setActive(true);

					glViewport(0, 0, event.size.width, event.size.height);

					// Make the window no longer the active window for OpenGL calls
					window.setActive(false);
				}
			}


			// Render something to the texture
			texture.clear();
			sf::CircleShape circle;
			circle.setFillColor(sf::Color::Magenta);
			circle.setOrigin(10, 10);
			circle.setPosition(100 + sin(clock.getElapsedTime().asSeconds() * 5) * 50, 50);
			circle.setRadius(20);
			texture.draw(circle);

			window.setActive(true);


			// Check for errors (it's IMPORTANT that the window is active)
#if 0
			GLenum error = glGetError();
			if (error != GL_NO_ERROR) {
				LOG_ERROR("OpenGL Error in render loop (" << error << "): " << glewGetErrorString(error));
			}
#endif

			// Clear the depth buffer
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			//
			// DRAW HERE
			//
			{
				crt_renderer.draw(texture.getTexture());
			}


			auto delta_time = frame_clock.restart();
			ImGui::SFML::Update(window, delta_time);

			crt_renderer.draw_parameter_gui();



			window.pushGLStates();
			ImGui::SFML::Render(window);
			window.popGLStates();



			// Finally, display the rendered frame on screen
			window.display();
		}

		ImGui::SFML::Shutdown();
	}


	//press_to_continue();

	return EXIT_SUCCESS;
}



int test_modern_opengl_crt_simulation() {


	// Settings for the window
	sf::ContextSettings settings;
	settings.depthBits = 24;
	settings.stencilBits = 8;
	settings.antialiasingLevel = 8;
	settings.majorVersion = 4;
	settings.minorVersion = 6;

	bool exit = false;

	while (!exit)
	{

		// Create the main window

		sf::RenderWindow window(sf::VideoMode(256 * 4, 224 * 4), "Modern OpenGL Test", sf::Style::Default, settings);
		window.setVerticalSyncEnabled(true);
		ImGui::SFML::Init(window);

		// Load a texture to apply to our 3D cube
		sf::Texture texture;
		if (!texture.loadFromFile("res/assets/testNES.png"))
			return EXIT_FAILURE;
		texture.setSmooth(false);

		// Make the window the active window for OpenGL calls
		window.setActive(true);

		// Enable Z-buffer read and write
		glEnable(GL_DEPTH_TEST);
		glDepthMask(GL_TRUE);
		glClearDepth(1.f);

		glewInit();

		// Set Clear Color
		glClearColor(0.1f, 0.1f, 0.1f, 1.f);


		//
		// Set up things to render HERE
		//
		CRTRenderer crt_renderer(window);



		GLenum error = glGetError();
		if (error != GL_NO_ERROR) {
			LOG_ERROR("OpenGL Error while initializing CRT renderer (" << error << "): " << glewGetErrorString(error));
		}
		

		// Create a clock for measuring the time elapsed
		sf::Clock clock;
		sf::Clock frame_clock;

		// Start game loop
		while (window.isOpen())
		{

			// Process events
			sf::Event event;
			while (window.pollEvent(event))
			{
				ImGui::SFML::ProcessEvent(event);

				// Close window: exit
				if (event.type == sf::Event::Closed)
				{
					exit = true;
					window.close();
				}

				// Escape key: exit
				if ((event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::Escape))
				{
					exit = true;
					window.close();
				}


				// Adjust the viewport when the window is resized
				if (event.type == sf::Event::Resized)
				{
					// Make the window the active window for OpenGL calls
					window.setActive(true);

					glViewport(0, 0, event.size.width, event.size.height);

					// Make the window no longer the active window for OpenGL calls
					window.setActive(false);
				}
			}


			// Check for errors (it's IMPORTANT that the window is active)
#if 0
			GLenum error = glGetError();
			if (error != GL_NO_ERROR) {
				LOG_ERROR("OpenGL Error in render loop (" << error << "): " << glewGetErrorString(error));
			}
#endif

			// Clear the depth buffer
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			//
			// DRAW HERE
			//
			{
				crt_renderer.draw(texture);
			}
			
			
			auto delta_time = frame_clock.restart();
			ImGui::SFML::Update(window, delta_time);
			
			crt_renderer.draw_parameter_gui();



			window.pushGLStates();
			ImGui::SFML::Render(window);
			window.popGLStates();
			


			// Finally, display the rendered frame on screen
			window.display();
		}

		ImGui::SFML::Shutdown();
	}


	//press_to_continue();

	return EXIT_SUCCESS;
}


#include "../render/shader/Shader.h"

int test_modern_opengl_crt_shape() {


	// create the window
	sf::ContextSettings settings;
	settings.depthBits = 24;
	settings.stencilBits = 8;
	settings.antialiasingLevel = 8;
	settings.majorVersion = 4;
	settings.minorVersion = 0;

	bool exit = false;

	while (!exit)
	{

		// Create the main window
		sf::RenderWindow window(sf::VideoMode(960, 540), "Modern OpenGL Test", sf::Style::Default, settings);
		window.setVerticalSyncEnabled(true);
		ImGui::SFML::Init(window);

		// Load a texture to apply to our 3D cube
		sf::Texture texture;
		if (!texture.loadFromFile("res/assets/testNES.png"))
			return EXIT_FAILURE;
		texture.setSmooth(false);

		// Make the window the active window for OpenGL calls
		window.setActive(true);

		// Enable Z-buffer read and write
		glEnable(GL_DEPTH_TEST);
		glDepthMask(GL_TRUE);
		glClearDepth(1.f);

		glewInit();

		// Set Clear Color
		glClearColor(0.1f, 0.1f, 0.1f, 1.f);


		//
		// Set up things to render HERE
		//

		GLfloat vertices[] =
		{
			0, 0, 0,
			0, 3, 0,
			8, 3, 0,
			8, 0, 0
		};

		GLushort indices[] =
		{
			0, 1, 2,
			2, 3, 0
		};

		GLfloat colorsA[] =
		{
			1, 0, 1, 1,
			1, 0, 1, 1,
			1, 0, 1, 1,
			1, 0, 1, 1
		};

		GLfloat colorsB[] =
		{
			0.2f, 0.3f, 0.8f, 1,
			0.2f, 0.3f, 0.8f, 1,
			0.2f, 0.3f, 0.8f, 1,
			0.2f, 0.3f, 0.8f, 1
		};

		VertexArray sprite1, sprite2;
		ElementBuffer ibo(indices, 6, BufferUsage::STATIC_DRAW);

		sprite1.addBuffer(std::make_unique<ArrayBuffer>(vertices, 4 * 3, 3, BufferUsage::STATIC_DRAW),
			0);
		sprite1.addBuffer(std::make_unique<ArrayBuffer>(colorsA, 4 * 4, 4, BufferUsage::STATIC_DRAW),
			1);

		sprite2.addBuffer(std::make_unique<ArrayBuffer>(vertices, 4 * 3, 3, BufferUsage::STATIC_DRAW),
			0);
		sprite2.addBuffer(std::make_unique<ArrayBuffer>(colorsB, 4 * 4, 4, BufferUsage::STATIC_DRAW),
			1);

		auto ortho = glm::ortho(0.0f, 16.0f, 0.0f, 9.0f, -1.0f, 1.0f);



		Shader shader("res/shaders/test.shader");
		shader.bind();

		shader.set_uniform_mat4("pr_matrix", ortho);
		shader.set_uniform_mat4("ml_matrix", glm::translate(glm::mat4(), glm::vec3(4, 3, 0)));

		shader.set_uniform_2f("light_pos", glm::vec2(4.0f, 1.5f));
		shader.set_uniform_4f("colour", glm::vec4(0.2f, 0.3f, 0.8f, 1.0f));

		shader.unbind();




		// Make the window no longer the active window for OpenGL calls
		window.setActive(false);

		// Create a clock for measuring the time elapsed
		sf::Clock clock;
		sf::Clock frame_clock;

		// Start game loop
		while (window.isOpen())
		{

			// Process events
			sf::Event event;
			while (window.pollEvent(event))
			{
				ImGui::SFML::ProcessEvent(event);

				// Close window: exit
				if (event.type == sf::Event::Closed)
				{
					exit = true;
					window.close();
				}

				// Escape key: exit
				if ((event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::Escape))
				{
					exit = true;
					window.close();
				}


				// Adjust the viewport when the window is resized
				if (event.type == sf::Event::Resized)
				{
					// Make the window the active window for OpenGL calls
					window.setActive(true);

					glViewport(0, 0, event.size.width, event.size.height);

					// Make the window no longer the active window for OpenGL calls
					window.setActive(false);
				}
			}

			// Make the window the active window for OpenGL calls
			window.setActive(true);


			// Check for errors (it's IMPORTANT that the window is active)
			GLenum error = glGetError();
			if (error != GL_NO_ERROR) {
				LOG_ERROR("OpenGL Error (" << error << "): " << glewGetErrorString(error));
			}

			// Clear the depth buffer
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			//
			// DRAW HERE
			//
			{
				shader.bind();

				auto mouse_pos = sf::Mouse::getPosition(window);
				shader.set_uniform_2f("light_pos", glm::vec2((float)(mouse_pos.x * 16.0f / 960.0f), (float)(9.0f - mouse_pos.y * 9.0f / 540.0f)));

				sprite1.bind();
				ibo.bind();
				shader.set_uniform_mat4("ml_matrix", glm::translate(glm::mat4(), glm::vec3(4, 3, 0)));
				glDrawElements(GL_TRIANGLES, ibo.get_component_count(), GL_UNSIGNED_SHORT, 0);
				ibo.unbind();
				sprite1.unbind();

				sprite2.bind();
				ibo.bind();
				shader.set_uniform_mat4("ml_matrix", glm::translate(glm::mat4(), glm::vec3(0, 0, 0)));
				glDrawElements(GL_TRIANGLES, ibo.get_component_count(), GL_UNSIGNED_SHORT, 0);
				ibo.unbind();
				sprite2.unbind();

				shader.unbind();

			}


			auto delta_time = frame_clock.restart();
			ImGui::SFML::Update(window, delta_time);
			ImGui::Begin("Parameters");

			ImGui::End();



			window.pushGLStates();
			ImGui::SFML::Render(window);
			window.popGLStates();

			// Finally, display the rendered frame on screen
			window.display();
		}

		ImGui::SFML::Shutdown();
	}


	return EXIT_SUCCESS;


}


int test_legacy_opengl_crt_shape() {

	// create the window
	sf::ContextSettings settings;
	settings.depthBits = 24;
	settings.stencilBits = 8;
	settings.antialiasingLevel = 8;
	settings.majorVersion = 4;
	settings.minorVersion = 0;

	bool exit = false;

	while (!exit)
	{

		// Create the main window
		sf::RenderWindow window(sf::VideoMode(256 * 4, 224 * 4), "CRT Shape", sf::Style::Default, settings);
		window.setVerticalSyncEnabled(true);
		ImGui::SFML::Init(window);

		// Load a texture to apply to our 3D cube
		sf::Texture texture;
		if (!texture.loadFromFile("res/assets/testNES.png"))
			return EXIT_FAILURE;
		texture.setSmooth(false);

		// Make the window the active window for OpenGL calls
		window.setActive(true);

		// Enable Z-buffer read and write
		glEnable(GL_DEPTH_TEST);
		glDepthMask(GL_TRUE);
		glClearDepth(1.f);

		glewInit();

		// Set Clear Color
		glClearColor(0.1f, 0.1f, 0.1f, 1.f);

		// Disable lighting
		glDisable(GL_LIGHTING);

		// CCW Faces
		glFrontFace(GL_CCW);

		// Configure the viewport (the same size as the window)
		glViewport(0, 0, window.getSize().x, window.getSize().y);

		// Setup a perspective projection
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		GLfloat ratio = static_cast<float>(window.getSize().x) / window.getSize().y;
		glFrustum(-ratio, ratio, -1.f, 1.f, 1.f, 500.f);

		// Bind the texture
		glEnable(GL_TEXTURE_2D);
		sf::Texture::bind(&texture);

		static auto columns = 300;
		static auto rows = 300;
		static auto exp_factor = 10.f;
		static auto max_depth = 4.5f;

		std::vector<GLfloat> tesselated_quad;

		auto generate_tesselated_quad = [&]() {

			tesselated_quad.clear();

			auto x_uv_increment = 1.f / columns;
			auto y_uv_increment = 1.f / rows;

			auto x_total_side = 256.f;
			auto y_total_side = 224.f;

			auto x_side = x_total_side / columns;
			auto y_side = y_total_side / rows;

			for (auto j = 0; j < rows; ++j) {
				for (auto i = 0; i < columns; ++i) {

					auto x_offset = (i * x_side) - (x_total_side / 2.f);
					auto y_offset = (j * y_side) - (y_total_side / 2.f);

					auto calc_z = [x_total_side, y_total_side](float x, float y) -> float {

						x /= (x_total_side / 2.f);
						y /= (y_total_side / 2.f);

						auto ret = -pow(sqrt(x*x + y * y), exp_factor) / exp_factor;

						return ret * max_depth;
					};

					auto x_uv_0 = (i)* x_uv_increment;
					auto x_uv_1 = (i)* x_uv_increment + x_uv_increment;

					auto y_uv_1 = (rows - j - 1) * y_uv_increment;
					auto y_uv_0 = (rows - j - 1) * y_uv_increment + y_uv_increment;


					const GLfloat quad[] =
					{
						x_offset,         +y_offset,
						calc_z(x_offset,y_offset)
						,  x_uv_0, y_uv_0,
						x_offset,  y_side + y_offset,
						calc_z(x_offset,y_side + y_offset),
						x_uv_0, y_uv_1,
						x_side + x_offset, +y_offset,
						calc_z(x_side + x_offset,+y_offset),
						x_uv_1, y_uv_0,
						x_side + x_offset,         +y_offset,
						calc_z(x_side + x_offset,+y_offset),
						x_uv_1, y_uv_0,
						x_offset,  y_side + y_offset,
						calc_z(x_offset,  y_side + y_offset),
						x_uv_0, y_uv_1,
						x_side + x_offset,  y_side + y_offset,
						calc_z(x_side + x_offset,  y_side + y_offset),
						x_uv_1, y_uv_1,
					};

					for (auto v = 0; v < 30; ++v) {
						tesselated_quad.push_back(quad[v]);
					}

				}
			}

			// Enable position and texture coordinates vertex components
			glEnableClientState(GL_VERTEX_ARRAY);
			glEnableClientState(GL_TEXTURE_COORD_ARRAY);
			glVertexPointer(3, GL_FLOAT, 5 * sizeof(GLfloat), tesselated_quad.data());
			glTexCoordPointer(2, GL_FLOAT, 5 * sizeof(GLfloat), tesselated_quad.data() + 3);

		};
		generate_tesselated_quad();


		// Disable normal and color vertex components
		glDisableClientState(GL_NORMAL_ARRAY);
		glDisableClientState(GL_COLOR_ARRAY);

		// Make the window no longer the active window for OpenGL calls
		window.setActive(false);

		// Create a clock for measuring the time elapsed
		sf::Clock clock;
		sf::Clock frame_clock;

		// Start game loop
		while (window.isOpen())
		{
			// Process events
			sf::Event event;
			while (window.pollEvent(event))
			{
				ImGui::SFML::ProcessEvent(event);

				// Close window: exit
				if (event.type == sf::Event::Closed)
				{
					exit = true;
					window.close();
				}

				// Escape key: exit
				if ((event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::Escape))
				{
					exit = true;
					window.close();
				}


				// Adjust the viewport when the window is resized
				if (event.type == sf::Event::Resized)
				{
					// Make the window the active window for OpenGL calls
					window.setActive(true);

					glViewport(0, 0, event.size.width, event.size.height);

					// Make the window no longer the active window for OpenGL calls
					window.setActive(false);
				}
			}

			// Make the window the active window for OpenGL calls
			window.setActive(true);

			// Clear the depth buffer
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			// We get the position of the mouse cursor, so that we can move the box accordingly
			float x = sf::Mouse::getPosition(window).x * 200.f / window.getSize().x - 100.f;
			float y = -sf::Mouse::getPosition(window).y * 200.f / window.getSize().y + 100.f;

			x = 0.f;
			y = 0.f;

			// Apply some transformations
			glMatrixMode(GL_MODELVIEW);
			glLoadIdentity();
			glTranslatef(x, y, -130.f);

			// Draw the quad
			glDrawArrays(GL_TRIANGLES, 0, 6 * columns * rows);


			auto delta_time = frame_clock.restart();
			ImGui::SFML::Update(window, delta_time);
			ImGui::Begin("CRT Parameters");


			if (
				ImGui::InputInt("colums", &columns) ||
				ImGui::InputInt("rows", &rows) ||
				ImGui::SliderFloat("exp_factor", &exp_factor, 1.f, 20.f) ||
				ImGui::SliderFloat("max_depth", &max_depth, 1.f, 30.f)
				) {
				generate_tesselated_quad();
			}


			ImGui::End();



			window.pushGLStates();
			ImGui::SFML::Render(window);
			window.popGLStates();

			// Finally, display the rendered frame on screen
			window.display();
		}

		ImGui::SFML::Shutdown();
	}


	return EXIT_SUCCESS;

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

