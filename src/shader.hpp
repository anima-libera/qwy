
/* Qwy project - Anima Libera <anim.libera@gmail.com> - github.com/anima-libera
 * Header shader.hpp - Handle OpenGL shaders and OpenGL shader programs
 * and defines the ShaderProgram class
 * Note that the shader sources are in bin/res/shaders */

#ifndef __qwy_shader_header__
#define __qwy_shader_header__

#include <string>
#include <GL/glew.h>
#include <glm/glm.hpp>

namespace qwy
{

	class ShaderProgram
	{
	public:
		ShaderProgram();
		ShaderProgram(std::string const& shader_pack_name);
		~ShaderProgram();

		/* Load, read and compile a shader pack into an OpenGL shader program.
		 * Param shader_pack_name in the name of a directory in bin/res/shaders,
		 * which must contains a "source.vert" and a "source.frag" files. */
		void load_vert_and_frag(std::string const& shader_pack_name);

		/* Select this shader program with the glUseProgram function */
		void select() const;

		/* Get the location of the specified uniform with the glGetUniformLocation function */
		GLint get_uniform_location(std::string const& uniform_name) const;

		/* Set the specified mat4 uniform to the passed matrix with the glUniformMatrix4fv function */
		void set_uniform_mat4(GLint uniform_location, glm::mat4 const& martix) const;

		void set_uniform_vec3(GLint uniform_location, glm::vec3 const& vector) const;
		void set_uniform_texture(GLint uniform_location, GLuint texture_unit) const;

	private:
		bool        m_compiled;
		GLuint      m_shader_program_id;
		std::string m_shader_pack_name;
	};

}

#endif /* __qwy_shader_header__ */
