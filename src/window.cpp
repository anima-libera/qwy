
#include "window.hpp"

#include <SFML/Graphics.hpp>


qwy::Window qwy::Window::Main;


qwy::Window::Window()
{
	;
}

qwy::Window::~Window()
{
	;
}

void qwy::Window::create()
{
	/* Set up some settings for SFML to create the OpenGL context */
	sf::ContextSettings desired_opengl_context_settings(
		24, /* Depth bits */
		0,  /* Stencil bits */
		4,  /* Antialiasing level */
		4,  /* Major OpenGL version */
		6   /* Minor OpenGL version */
	);

	/* Create the SFML window */
	m_sfml_window.create(sf::VideoMode(1200, 900), "Qwy", sf::Style::Default, desired_opengl_context_settings);

	/* Set things right */
	set_vsync(true);
	m_sfml_window.setKeyRepeatEnabled(false);

	/* Center the window */
	sf::VideoMode screen_video_mode = sf::VideoMode::getDesktopMode();
	sf::Vector2u window_size = m_sfml_window.getSize();
	m_sfml_window.setPosition(sf::Vector2i(
		(screen_video_mode.width - window_size.x) / 2, (screen_video_mode.height - window_size.y) / 2));
}

void qwy::Window::toggle_vsync()
{
	set_vsync(not is_vsync_enabled());
}

void qwy::Window::set_vsync(bool is_vsync_enabled)
{
	m_sfml_window.setVerticalSyncEnabled(is_vsync_enabled);
	m_is_vsync_enabled = is_vsync_enabled;
}

bool qwy::Window::is_vsync_enabled() const
{
	return m_is_vsync_enabled;
}
