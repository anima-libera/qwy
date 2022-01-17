
/* Qwy project - Anima Libera <anim.libera@gmail.com> - github.com/anima-libera
 * Header window.hpp - Defines Window class to handle the window */

#ifndef __qwy_window_header__
#define __qwy_window_header__

#include <SFML/Graphics.hpp>

namespace qwy
{


	class Window
	{
	public:
		static qwy::Window Main;

	public:
		Window();
		~Window();

		void create();

		void toggle_vsync();
		void set_vsync(bool is_vsync_enabled);
		bool is_vsync_enabled() const;

		sf::RenderWindow& sfml_window() {return m_sfml_window;}

	public:
		sf::RenderWindow m_sfml_window;
		bool m_is_vsync_enabled;
	};


}

#endif /* __qwy_window_header__ */
