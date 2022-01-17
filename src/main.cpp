
#ifdef DEBUG
	#include <iostream>
#endif
#include <iostream>

#include "utility/opengl.hpp"
#include "camera.hpp"
#include "shader.hpp"
#include "mesh.hpp"
#include "vertex.hpp"
#include "block.hpp"
#include "chunk.hpp"
#include "world.hpp"
#include "window.hpp"
#include "types.hpp"
#include "texture.hpp"
#include "utility/math.hpp"
#include <cmath>
#include <string>
#include <chrono>
#include <random>
#include <fstream>
#include <SFML/Graphics.hpp>
#include <GL/glew.h>



qwy::Camera g_camera;
qwy::ShaderProgram g_shader_program;
bool g_display_information = true;

float g_moving_forward = 0.0f;
float g_moving_backward = 0.0f;
float g_moving_rightward = 0.0f;
float g_moving_leftward = 0.0f;
glm::vec3 g_player_position(50.0f, 20.0f, 50.0f);
glm::vec3 g_player_motion(0.0f, 0.0f, 0.0f);


bool g_place_block = false;




/* Pause the window until P is pressed */
void pause()
{
	bool return_control = false;

	/* Loop until P is pressed */
	while (not return_control)
	{
		sf::Event event;
		while (qwy::Window::Main.sfml_window().pollEvent(event))
		{
			switch (event.type)
			{
				case sf::Event::Closed:
					qwy::Window::Main.sfml_window().close();
				break;
	
				case sf::Event::Resized:
					/* Resize things to ensure normal behaviour of the rendering process */
					glViewport(0, 0, event.size.width, event.size.height);
					g_camera.set_aspect_ratio(
						static_cast<float>(event.size.width) / static_cast<float>(event.size.height));
					{
						sf::View new_sfml_view(sf::FloatRect(0.f, 0.f,
							static_cast<float>(event.size.width), static_cast<float>(event.size.height)));
						qwy::Window::Main.sfml_window().setView(new_sfml_view);
					}
				break;
	
				case sf::Event::KeyPressed:
					switch (event.key.code)
					{
						case sf::Keyboard::Escape:
						case sf::Keyboard::Return:
							qwy::Window::Main.sfml_window().close();
						break;
	
						case sf::Keyboard::P:
							return_control = true;
						break;
	
						default:
						break;
					}
				break;
	
				default:
				break;
			}
		}
	}
}

void handle_events()
{
	/* Save the mouse position during the event loop to compute a very accurate vector at the end */
	sf::Vector2i mouse_position = sf::Mouse::getPosition(qwy::Window::Main.sfml_window());

	/* Recive all the new incoming events */
	sf::Event event;
	while (qwy::Window::Main.sfml_window().pollEvent(event))
	{
		switch (event.type)
		{
			case sf::Event::Closed:
				qwy::Window::Main.sfml_window().close();
			break;

			case sf::Event::Resized:
				/* Resize things to ensure normal behaviour of the rendering process */
				glViewport(0, 0, event.size.width, event.size.height);
				g_camera.set_aspect_ratio(
					static_cast<float>(event.size.width) / static_cast<float>(event.size.height));
				{
					sf::View new_sfml_view(sf::FloatRect(0.f, 0.f,
						static_cast<float>(event.size.width), static_cast<float>(event.size.height)));
					qwy::Window::Main.sfml_window().setView(new_sfml_view);
				}
			break;

			case sf::Event::MouseMoved:
				mouse_position.x = event.mouseMove.x;
				mouse_position.y = event.mouseMove.y;
			break;

			case sf::Event::MouseWheelScrolled:
				/* Zoom in or out */
				g_camera.augment_fov(event.mouseWheelScroll.delta * 0.1f);
			break;

			case sf::Event::MouseButtonPressed:
				if (event.mouseButton.button == sf::Mouse::Right)
				{
					/* Jump */
					g_player_motion.y += 0.4f;
				}
			break;

			case sf::Event::KeyPressed:
				switch (event.key.code)
				{
					case sf::Keyboard::Escape:
					case sf::Keyboard::Return:
						qwy::Window::Main.sfml_window().close();
					break;

					case sf::Keyboard::V:
						qwy::Window::Main.toggle_vsync();
					break;

					case sf::Keyboard::I:
						/* Toggle the information display in the top-left corner */
						g_display_information = not g_display_information;
					break;

					case sf::Keyboard::P:
						pause();
					break;

					case sf::Keyboard::A:
						g_place_block = true;
					break;

					/* Move the player horizontaly (start moving) */
					case sf::Keyboard::Z:
						g_moving_forward = 1.0f;
					break;
					case sf::Keyboard::S:
						g_moving_backward = 1.0f;
					break;
					case sf::Keyboard::D:
						g_moving_rightward = 1.0f;
					break;
					case sf::Keyboard::Q:
						g_moving_leftward = 1.0f;
					break;

					default:
					break;
				}
			break;

			case sf::Event::KeyReleased:
				switch (event.key.code)
				{
					/* Move the player horizontaly (stop moving) */
					case sf::Keyboard::Z:
						g_moving_forward = 0.0f;
					break;
					case sf::Keyboard::S:
						g_moving_backward = 0.0f;
					break;
					case sf::Keyboard::D:
						g_moving_rightward = 0.0f;
					break;
					case sf::Keyboard::Q:
						g_moving_leftward = 0.0f;
					break;

					default:
					break;
				}
			break;

			default:
			break;
		}
	}

	/* Warp the mouse at the middle of the main window */
	sf::Vector2u main_window_size = qwy::Window::Main.sfml_window().getSize();
	sf::Mouse::setPosition(sf::Vector2i(main_window_size.x/2, main_window_size.y/2), qwy::Window::Main.sfml_window());
	sf::Vector2i center_position = sf::Mouse::getPosition(qwy::Window::Main.sfml_window());

	/* Change the orientation of the camera according to the mouse motion */
	#define CAMERA_ORIENTATION_FACTOR 0.01f
	g_camera.move_angles(
		-CAMERA_ORIENTATION_FACTOR * (static_cast<float>(mouse_position.x) - static_cast<float>(center_position.x)),
		-CAMERA_ORIENTATION_FACTOR * (static_cast<float>(mouse_position.y) - static_cast<float>(center_position.y))
	);
}

void handle_basic_events()
{
	sf::Event event;
	while (qwy::Window::Main.sfml_window().pollEvent(event))
	{
		switch (event.type)
		{
			case sf::Event::Closed:
				qwy::Window::Main.sfml_window().close();
			break;
	
			case sf::Event::Resized:
				/* Resize things to ensure normal behaviour of the rendering process */
				glViewport(0, 0, event.size.width, event.size.height);
				g_camera.set_aspect_ratio(
					static_cast<float>(event.size.width) / static_cast<float>(event.size.height)
				);
			break;
	
			case sf::Event::KeyPressed:
				switch (event.key.code)
				{
					case sf::Keyboard::Escape:
					case sf::Keyboard::Return:
						qwy::Window::Main.sfml_window().close();
					break;
	
					default:
					break;
				}
			break;
	
			default:
			break;
		}
	}
}



/* Wtf the mess here.. xd it's the test phase ^^
 * TODO: clean up the main.cpp file when the war is over */


/* Initialize all kind of stuff */
void initilalize()
{
	/* Initialize GLEW for OpenGL features */
	sf::Context context;
	glewInit();

	#ifdef DEBUG
		std::cout << "Success: Initialization completed" << std::endl;
	#endif
}

/* Cleans things up mostly */
int terminate()
{
	#ifdef DEBUG
		std::cout << "Success: Termination completed" << std::endl;
	#endif
	return 0;
}




/* Noise engine */

int g_noise_cube_size = 15;
int g_noise_seed = 0;


float random_xyz(float x, float y, float z)
{
	/* Randomize some shit and pops out a fucking random number (always the same for the same x, y and z)
	 * Note that the output is between -1.0f and 1.0f */
	float a = std::abs(x+y+z + std::cos((x+z*y+x*z)*g_noise_seed+(x*x*y-562.f)*g_noise_seed))+4565.f;
	int b = static_cast<int>((std::cos(a*a + y-z*61.f + x*x)*0.5f+0.5f)*a*652.f)%324572;
	float c = static_cast<float>(b)/(324571.f/2.f)-1.f;
	return c;
}

float interpolate_lin(float x, float a, float b)
{
	return a * (1.f - x) + b * x;
}

float interpolate_cos(float x, float a, float b)
{
	float y = (1.f - std::cos(x * qwy::tau / 2.f)) / 2.f;
	return (a * (1.f - y) + b * y);
}

float cube_int_noise_3D(int x, int y, int z)
{
	float float_noise_cube_size = static_cast<float>(g_noise_cube_size);

	/* Coords inside the cube */
	int icx = x % g_noise_cube_size, icy = y % g_noise_cube_size, icz = z % g_noise_cube_size;

	/* Lowest coords of the cube vertices in the virtual space of the random_xyz function */
	float vx = static_cast<float>(x - icx) / float_noise_cube_size;
	float vy = static_cast<float>(y - icy) / float_noise_cube_size;
	float vz = static_cast<float>(z - icz) / float_noise_cube_size;

	/* Random values associated with the cube vertices */
	float mmm = random_xyz(vx,     vy,     vz);
	float pmm = random_xyz(vx+1.f, vy,     vz);
	float mpm = random_xyz(vx,     vy+1.f, vz);
	float ppm = random_xyz(vx+1.f, vy+1.f, vz);
	float mmp = random_xyz(vx,     vy,     vz+1.f);
	float pmp = random_xyz(vx+1.f, vy,     vz+1.f);
	float mpp = random_xyz(vx,     vy+1.f, vz+1.f);
	float ppp = random_xyz(vx+1.f, vy+1.f, vz+1.f);

	/* Interpolate on the x-axis, then on the y-axis, and then on the z-axis */
	float xmm = interpolate_cos(static_cast<float>(icx) / float_noise_cube_size, mmm, pmm);
	float xpm = interpolate_cos(static_cast<float>(icx) / float_noise_cube_size, mpm, ppm);
	float xmp = interpolate_cos(static_cast<float>(icx) / float_noise_cube_size, mmp, pmp);
	float xpp = interpolate_cos(static_cast<float>(icx) / float_noise_cube_size, mpp, ppp);
	float xym = interpolate_cos(static_cast<float>(icy) / float_noise_cube_size, xmm, xpm);
	float xyp = interpolate_cos(static_cast<float>(icy) / float_noise_cube_size, xmp, xpp);
	float xyz = interpolate_cos(static_cast<float>(icz) / float_noise_cube_size, xym, xyp);

	return xyz;
}




int main()
{
	/* Let's go :D */
	#ifdef DEBUG
		std::cout << "COMPILED WITH ACTIVE DEBUG MODE" << std::endl;
	#endif

	/* Initialize and reate some global objects */
	initilalize();
	qwy::Window::Main.create();
	g_shader_program.load_vert_and_frag("default");

	/* Depth test */
	glEnable(GL_DEPTH_TEST);

	/* OpenGL face culling settings */
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CW);

	/* Load the font used to display text */
	sf::Font font;
	if (not font.loadFromFile("res/fonts/cabin/Cabin-Medium-TTF.ttf"))
	{
		/* TODO: handle the error */
		#ifdef DEBUG
			std::cout << "Error: Unable to load the font " << "Cabin-Medium-TTF.ttf" << std::endl;
		#endif
	}

	/* Create the SFML text drawable objects to display the tick duration and the fps count */
	sf::Text text_tick_duration;
	text_tick_duration.setFont(font);
	text_tick_duration.setString("-");
	text_tick_duration.setCharacterSize(20);
	text_tick_duration.setFillColor(sf::Color::Black);
	sf::Text text_fps;
	text_fps.setFont(font);
	text_fps.setString("-");
	text_fps.setCharacterSize(20);
	text_fps.setFillColor(sf::Color::Black);
	text_fps.setPosition(0, 22);
	sf::Text text_camera_fov;
	text_camera_fov.setFont(font);
	text_camera_fov.setString("-");
	text_camera_fov.setCharacterSize(20);
	text_camera_fov.setFillColor(sf::Color::Black);
	text_camera_fov.setPosition(0, 44);
	sf::Text text_int_coords;
	text_int_coords.setFont(font);
	text_int_coords.setString("-");
	text_int_coords.setCharacterSize(20);
	text_int_coords.setFillColor(sf::Color::Black);
	text_int_coords.setPosition(0, 66);
	sf::Text text_world_chunk_coords;
	text_world_chunk_coords.setFont(font);
	text_world_chunk_coords.setString("-");
	text_world_chunk_coords.setCharacterSize(20);
	text_world_chunk_coords.setFillColor(sf::Color::Black);
	text_world_chunk_coords.setPosition(0, 88);
	
	/* Init use of shaders */
	g_shader_program.select();
	GLint u_camera_matrix_location = g_shader_program.get_uniform_location("u_camera_matrix");
	GLint u_light_direction_location = g_shader_program.get_uniform_location("u_light_direction");



	/* TEST texturing */
	qwy::TextureAtelas texture_atelas(256, 6, 16);
	texture_atelas.texture.load_from_file("default/mainset.png");

	GLint u_texture_0_location = g_shader_program.get_uniform_location("u_texture_0");

	texture_atelas.texture.activate(0);

	g_shader_program.set_uniform_texture(u_texture_0_location, texture_atelas.texture.id());




	/* Init the tick duration counter */
	std::chrono::high_resolution_clock::time_point loop_beginning_date, loop_end_date;
	int tick_duration = 0;


	/* Load some settings */
	int chunk_table_side_size;
	float g_world_landforms_amplitude;

	std::ifstream settings_file;
	settings_file.open ("settings");
	settings_file >> chunk_table_side_size;
	settings_file >> g_noise_cube_size;
	settings_file >> g_noise_seed;
	settings_file >> g_world_landforms_amplitude;
	settings_file.close();


	/* Create some block types */
	qwy::BlockTypeTable::Current.add_block_type(qwy::BlockType("Air", false,
		qwy::TileCoords(), qwy::TileCoords(), qwy::TileCoords(), false));
	int air_type_index = qwy::BlockTypeTable::Current.last_index();

	qwy::BlockTypeTable::Current.add_block_type(qwy::BlockType("Stone", true, 
		qwy::TileCoords(1, 0, &texture_atelas), 
		qwy::TileCoords(1, 0, &texture_atelas),
		qwy::TileCoords(1, 0, &texture_atelas), true));
	int stone_type_index = qwy::BlockTypeTable::Current.last_index();

	qwy::BlockTypeTable::Current.add_block_type(qwy::BlockType("Grass", true, 
		qwy::TileCoords(0, 0, &texture_atelas), 
		qwy::TileCoords(0, 1, &texture_atelas),
		qwy::TileCoords(0, 2, &texture_atelas), true));
	int grass_type_index = qwy::BlockTypeTable::Current.last_index();

	qwy::BlockTypeTable::Current.add_block_type(qwy::BlockType("Dirt", true, 
		qwy::TileCoords(0, 2, &texture_atelas), 
		qwy::TileCoords(0, 2, &texture_atelas),
		qwy::TileCoords(0, 2, &texture_atelas), true));
	int dirt_type_index = qwy::BlockTypeTable::Current.last_index();




	/* Loadbar rectangle */
	sf::RectangleShape loadbar_rectangle(sf::Vector2f(0.0f, 25.0f));
	loadbar_rectangle.setFillColor(sf::Color::Blue);
	loadbar_rectangle.setPosition(sf::Vector2f(100.0f, (qwy::Window::Main.sfml_window().getSize().y - 25.0f) / 2.0f));
	float generation_progression = 0.0f;


	#define DISPLAY_LOAD_BAR \
		glDisable(GL_DEPTH_TEST); \
		glDisable(GL_CULL_FACE); \
		glBindBuffer(GL_ARRAY_BUFFER, 0); \
		glBindTexture(GL_TEXTURE_2D, 0); \
		glBindVertexArray(0); \
		glUseProgram(0); \
		qwy::Window::Main.sfml_window().pushGLStates(); \
		qwy::Window::Main.sfml_window().resetGLStates(); \
			qwy::Window::Main.sfml_window().clear(sf::Color::Black); \
			loadbar_rectangle.setSize(sf::Vector2f( \
				generation_progression * (qwy::Window::Main.sfml_window().getSize().x - 200.0f), 25.f)); \
			qwy::Window::Main.sfml_window().draw(loadbar_rectangle); \
			qwy::Window::Main.sfml_window().display(); \
		qwy::Window::Main.sfml_window().popGLStates(); \
		glEnable(GL_DEPTH_TEST); \
		glEnable(GL_CULL_FACE); \
		glCullFace(GL_BACK); \
		glFrontFace(GL_CW)




	/* TEST create the chunks */
	qwy::World::Current.m_chunk_table.set_side_size(chunk_table_side_size);
	
	for (int wcx = 0; wcx < qwy::World::Current.m_chunk_table.get_side_size(); wcx ++)
	{
		for (int wcy = 0; wcy < qwy::World::Current.m_chunk_table.get_side_size(); wcy ++)
		{
			for (int wcz = 0; wcz < qwy::World::Current.m_chunk_table.get_side_size(); wcz ++)
			{
				qwy::World::Current.m_chunk_table.chunk(wcx, wcy, wcz).m_parent_world_ptr = &qwy::World::Current;

				qwy::World::Current.m_chunk_table.chunk(wcx, wcy, wcz).set_min_position(
					wcx * qwy::Chunk::Side, wcy * qwy::Chunk::Side, wcz * qwy::Chunk::Side);
				//qwy::World::Current.m_chunk_table.chunk(wcx, wcy, wcz).generate_blocks();
				//qwy::World::Current.m_chunk_table.chunk(wcx, wcy, wcz).generate_mesh();

				generation_progression = 
					static_cast<float>(wcx * qwy::World::Current.m_chunk_table.get_side_size()
						* qwy::World::Current.m_chunk_table.get_side_size()
						+ wcy * qwy::World::Current.m_chunk_table.get_side_size() + wcz)
						/ static_cast<float>(qwy::World::Current.m_chunk_table.get_side_size()
						* qwy::World::Current.m_chunk_table.get_side_size()
						* qwy::World::Current.m_chunk_table.get_side_size() );

				handle_basic_events();
			}

			DISPLAY_LOAD_BAR;

		}
	}

	std::cout << "Success: Chunks created" << std::endl;



	const int world_side = qwy::World::Current.m_chunk_table.get_side_size() * qwy::Chunk::Side;
	std::cout << "Info: World side size: " << world_side << std::endl;




	/* Generate the terrain */
	loadbar_rectangle.setFillColor(sf::Color::Green);
	generation_progression = 0.0f;
	for (int x = 0; x < world_side; x ++)
	{
		for (int y = 0; y < world_side; y ++)
		{
			for (int z = 0; z < world_side; z ++)
			{
				/* IT'S HERE */
				float noise = cube_int_noise_3D(x, y, z);
				float solid_required_level = static_cast<float>(y) / g_world_landforms_amplitude - 3.f;
				if (noise > solid_required_level)
				{
					qwy::World::Current.block(x, y, z).set_type_index(stone_type_index);
				}
			}
		}

		handle_basic_events();
		generation_progression = static_cast<float>(x) / static_cast<float>(world_side);

		DISPLAY_LOAD_BAR;
	}

	std::cout << "Success: Terrain generated" << std::endl;

	/* Put grass and dirt */
	loadbar_rectangle.setFillColor(sf::Color::Yellow);
	generation_progression = 0.0f;
	for (int x = 0; x < world_side; x ++)
	{
		for (int y = 0; y < world_side - 3; y ++)
		{
			for (int z = 0; z < world_side; z ++)
			{
				#define TYPE_INDEX(X, Y, Z) qwy::World::Current.block(X, Y, Z).type_index()

				if (TYPE_INDEX(x, y, z) == stone_type_index && TYPE_INDEX(x, y+1, z) == air_type_index)
				{
					qwy::World::Current.block(x, y, z).set_type_index(grass_type_index);
				}
				else if (TYPE_INDEX(x, y, z) == stone_type_index && TYPE_INDEX(x, y+2, z) == air_type_index)
				{
					qwy::World::Current.block(x, y, z).set_type_index(dirt_type_index);
				}
				else if (TYPE_INDEX(x, y, z) == stone_type_index && TYPE_INDEX(x, y+3, z) == air_type_index
					&& random_xyz(static_cast<float>(x), static_cast<float>(y), static_cast<float>(z)) > 0.0f)
				{
					qwy::World::Current.block(x, y, z).set_type_index(dirt_type_index);
				}

				#undef TYPE_INDEX
			}
		}

		handle_basic_events();
		generation_progression = static_cast<float>(x) / static_cast<float>(world_side);

		DISPLAY_LOAD_BAR;

	}

	std::cout << "Success: Terrain terramormed" << std::endl;

	
	#if 0
	std::default_random_engine re(std::chrono::system_clock::now().time_since_epoch().count());
	std::uniform_real_distribution<float> df{-100.0f, 100.0f};
	for (int i = 0; i < 20; i ++)
	{
		std::cout << random_xyz(df(re), df(re), df(re)) << std::endl;
	}
	#endif


	/* TEST create the MESHes */
	loadbar_rectangle.setFillColor(sf::Color::Red);
	generation_progression = 0.0f;
	for (int wcx = 0; wcx < qwy::World::Current.m_chunk_table.get_side_size(); wcx ++)
	{
		for (int wcy = 0; wcy < qwy::World::Current.m_chunk_table.get_side_size(); wcy ++)
		{
			for (int wcz = 0; wcz < qwy::World::Current.m_chunk_table.get_side_size(); wcz ++)
			{
				qwy::World::Current.m_chunk_table.chunk(wcx, wcy, wcz).generate_mesh();

				generation_progression = 
					static_cast<float>(wcx * qwy::World::Current.m_chunk_table.get_side_size()
						* qwy::World::Current.m_chunk_table.get_side_size()
						+ wcy * qwy::World::Current.m_chunk_table.get_side_size() + wcz)
						/ static_cast<float>(qwy::World::Current.m_chunk_table.get_side_size()
						* qwy::World::Current.m_chunk_table.get_side_size()
						* qwy::World::Current.m_chunk_table.get_side_size() );

				handle_basic_events();
			}

			DISPLAY_LOAD_BAR;

		}
	}


	std::cout << "Success: Meshes created" << std::endl;




	#if 0
	/* TEST light */
	glm::vec3 light_direction = glm::normalize(glm::vec3(-1.f, -1.f, -1.f));
	float evolution = 0.0f;
	#endif



	/* Player chunk world coords */
	int player_wcx = 0, player_wcy = 0, player_wcz = 0;

	/* Loop until window is closed */
	while (qwy::Window::Main.sfml_window().isOpen())
	{
		/* Save the date at the beginning to compute the tick duration at the end */
		loop_beginning_date = std::chrono::high_resolution_clock::now();

		/* Handle events */
		handle_events();

		/* Controls */
		#define PLAYER_MOTION_FACTOR 0.2f
		g_player_position += PLAYER_MOTION_FACTOR * (
			g_camera.get_horizontal_direction() * (g_moving_forward -   g_moving_backward) + 
			g_camera.get_horizontal_right() *     (g_moving_rightward - g_moving_leftward) );

		/* Chunk coords */
		player_wcx = static_cast<int>(g_player_position.x) / qwy::Chunk::Side;
		player_wcy = static_cast<int>(g_player_position.y) / qwy::Chunk::Side;
		player_wcz = static_cast<int>(g_player_position.z) / qwy::Chunk::Side;

		/* Player and camera */
		if (qwy::World::Current.m_chunk_table.chunk(player_wcx, player_wcy, player_wcz).is_in_solid_block(
				g_player_position.x, g_player_position.y - 0.01f, g_player_position.z)
			|| qwy::World::Current.m_chunk_table.chunk(player_wcx, player_wcy, player_wcz).is_in_solid_block(
				g_player_position.x, g_player_position.y - 0.06f, g_player_position.z)
			|| g_player_position.y <= -6.f)
		{
			/* On a solid block */
			g_player_position.y = std::ceil(g_player_position.y);
			if (g_player_motion.y <= 0.0f)
			{
				g_player_motion.y = 0.0f;
			}
		}
		else
		{
			/* Gravity */
			g_player_motion.y -= 0.03f;
			if (g_player_motion.y > 0.6f)
			{
				g_player_motion.y = 0.6f;
			}
			else if (g_player_motion.y < -0.6f)
			{
				g_player_motion.y = -0.6f;
			}
		}
		g_player_position += g_player_motion;
		g_camera.set_position(g_player_position.x, g_player_position.y + 2.0f, g_player_position.z);


		/* TEST place a block */
		if (g_place_block)
		{
			g_place_block = false;
			qwy::World::Current.block(
				std::round(g_player_position.x), std::round(g_player_position.y-1.f), std::round(g_player_position.z)
			).set_type_index(dirt_type_index);

			qwy::World::Current.m_chunk_table.chunk(player_wcx, player_wcy, player_wcz).generate_mesh();
		}


		#if 0
		/* TEST light rotation */
		evolution += 0.01f;
		light_direction = glm::normalize(glm::vec3(
			std::cos(evolution),
			-2.f,
			std::sin(evolution)
		));
		#endif


		/* Handle graphics */
		/* Clear */
		glClearColor(0.4, 1.0, 1.0, 1.0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


		/* Select the shader and update uniforms */
		g_shader_program.select();
		g_shader_program.set_uniform_mat4(u_camera_matrix_location, g_camera.get_matrix());
		//g_shader_program.set_uniform_vec3(u_light_direction_location, light_direction);
		texture_atelas.texture.activate(0);
		g_shader_program.set_uniform_texture(u_texture_0_location, 0);
		
		/* Draw the chunks ^^ */
		for (int wcx = 0; wcx < qwy::World::Current.m_chunk_table.get_side_size(); wcx ++)
		{
			for (int wcy = 0; wcy < qwy::World::Current.m_chunk_table.get_side_size(); wcy ++)
			{
				for (int wcz = 0; wcz < qwy::World::Current.m_chunk_table.get_side_size(); wcz ++)
				{
					qwy::World::Current.m_chunk_table.chunk(wcx, wcy, wcz).draw_block_mesh();
				}
			}
		}

		/* Drawing with SFML */
		glDisable(GL_DEPTH_TEST);
		glDisable(GL_CULL_FACE);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindTexture(GL_TEXTURE_2D, 0);
		glBindVertexArray(0);
		glUseProgram(0);
		qwy::Window::Main.sfml_window().pushGLStates();
		qwy::Window::Main.sfml_window().resetGLStates();


		/* Display a bunch of information in the top-left corner */
		if (g_display_information)
		{
			/* Display the tick duration in micro seconds */
			text_tick_duration.setString(std::to_string(tick_duration));
			qwy::Window::Main.sfml_window().draw(text_tick_duration);

			/* Display the frame rate (frame per second) with one digit after the decimal point
			 * (note that this is very bad code, TODO: re-code this better) */
			std::string string_fps = std::to_string(1000000.0f / static_cast<float>(tick_duration));
			if (size_t point_index = string_fps.find_first_of(".") != std::string::npos)
			{
				string_fps.resize(point_index+3);
			}
			text_fps.setString(string_fps);
			qwy::Window::Main.sfml_window().draw(text_fps);

			/* Display the current camera fov (vertical field of view), aka zoom xd */
			text_camera_fov.setString(std::to_string(g_camera.get_fov()));
			qwy::Window::Main.sfml_window().draw(text_camera_fov);

			/* Display the current int coords of the player */
			text_int_coords.setString(
				std::to_string(static_cast<int>(g_player_position.x)) + ", " +
				std::to_string(static_cast<int>(g_player_position.y)) + ", " +
				std::to_string(static_cast<int>(g_player_position.z)) );
			qwy::Window::Main.sfml_window().draw(text_int_coords);

			/* Display the current world chunk coords in which the player is */
			text_world_chunk_coords.setString(
				std::to_string(player_wcx) + ", " + std::to_string(player_wcy) + ", " + std::to_string(player_wcz));
			qwy::Window::Main.sfml_window().draw(text_world_chunk_coords);
		}

		qwy::Window::Main.sfml_window().popGLStates();
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);
		glFrontFace(GL_CW);
		/* Stop drawing with SFML */
		
		/* Refresh the display */
		qwy::Window::Main.sfml_window().display();
		
		/* Clock timing managment and fps calculation */
		loop_end_date = std::chrono::high_resolution_clock::now();
		tick_duration = std::chrono::duration_cast<std::chrono::microseconds>(
			(loop_end_date - loop_beginning_date)).count();
	}


	/* Clean up */
	#if 0
	glDeleteBuffers(1, &coords_buffer_id);
	glDeleteBuffers(1, &color_buffer_id);
	#endif
	
	/* Terminate */
	return terminate();
}
