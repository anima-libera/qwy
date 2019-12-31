
#include "shader.hpp"

#include "utility/fileio.hpp"
#include <glm/glm.hpp>

#ifdef DEBUG
	#include <iostream>
#endif


qwy::ShaderProgram::ShaderProgram():
	m_compiled(false),
	m_shader_pack_name("")
{}

qwy::ShaderProgram::ShaderProgram(std::string const& shader_pack_name):
	m_compiled(false),
	m_shader_pack_name("")
{
	load_vert_and_frag(shader_pack_name);
}

qwy::ShaderProgram::~ShaderProgram()
{
	if (m_compiled)
	{
		glDeleteProgram(m_shader_program_id);
	}
}

/* Load, read and compile a shader pack into an OpenGL shader program.
 * Param shader_pack_name in the name of a directory in bin/res/shaders,
 * which must contains a "source.vert" and a "source.frag" files. */
void qwy::ShaderProgram::load_vert_and_frag(std::string const& shader_pack_name)
{
	/* Assert that the shader program does not already contains an OpenGL compiled shader program */
	if (m_compiled)
	{
		/* TODO: handle error */

		#ifdef DEBUG
			std::cout << "Error: ShaderProgram load_vert_and_frag method called whereas it was already compiled"
				<< std::endl;
		#endif
		return;
	}

	/* Get the vertex shader source code, which is in the "source.vert" file */
	std::string vertex_path = "res/shaders/"+shader_pack_name+"/source.vert";
	std::string vertex_source;
	if (not qwy::load_file_content(vertex_path, vertex_source))
	{
		/* TODO: handle error */

		#ifdef DEBUG
			std::cout << "Error: Unable to open the file \"" << vertex_path << "\"" << std::endl;
		#endif
		return;
	}
	const GLchar* vertex_source_chra_array = vertex_source.c_str();

	/* Get the framgent shader source code, which is in the "source.frag" file */
	std::string fragment_path = "res/shaders/"+shader_pack_name+"/source.frag";
	std::string fragment_source;
	if (not qwy::load_file_content(fragment_path, fragment_source))
	{
		/* TODO: handle error */

		#ifdef DEBUG
			std::cout << "Error: Unable to open the file \"" << fragment_path << "\"" << std::endl;
		#endif
		return;
	}
	const GLchar* fragment_source_chra_array = fragment_source.c_str();

	/* Create the objects used to check shader compilations and linkage */
	GLint operation_result = GL_FALSE;
	GLint info_log_length;

	/* Compile the vertex shader */
	GLuint vertex_shader_id = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex_shader_id, 1, &vertex_source_chra_array, nullptr);
	glCompileShader(vertex_shader_id);
	/* Check */
	glGetShaderiv(vertex_shader_id, GL_COMPILE_STATUS, &operation_result);
	if (operation_result == GL_FALSE)
	{
		glGetShaderiv(vertex_shader_id, GL_INFO_LOG_LENGTH, &info_log_length);

		/* TODO: output the message */
		/*glGetShaderInfoLog(FragmentShaderID, InfoLogLength, NULL, &FragmentShaderErrorMessage[0]);*/

		#ifdef DEBUG
			std::cout << "Error: Unable to compile the vertex shader from the " << shader_pack_name << " pack"
				<< std::endl;
		#endif
		return;
	}

	/* Compile the framgent shader */
	GLuint fragment_shader_id = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment_shader_id, 1, &fragment_source_chra_array, nullptr);
	glCompileShader(fragment_shader_id);
	/* Check */
	glGetShaderiv(fragment_shader_id, GL_COMPILE_STATUS, &operation_result);
	if (operation_result == GL_FALSE)
	{
		glGetShaderiv(fragment_shader_id, GL_INFO_LOG_LENGTH, &info_log_length);

		/* TODO: output the message */
		/*glGetShaderInfoLog(FragmentShaderID, InfoLogLength, NULL, &FragmentShaderErrorMessage[0]);*/

		#ifdef DEBUG
			std::cout << "Error: Unable to compile the fragment shader from the " << shader_pack_name << " pack"
				<< std::endl;
		#endif

		/* Clean up before abort */
		glDeleteShader(vertex_shader_id);
		return;
	}

	/* Create the shader program and link the shadres to it */
	GLuint shader_program_id = glCreateProgram();
	glAttachShader(shader_program_id, vertex_shader_id);
	glAttachShader(shader_program_id, fragment_shader_id);
	glLinkProgram(shader_program_id);
	/* Check */
	glGetProgramiv(shader_program_id, GL_LINK_STATUS, &operation_result);
	if (operation_result == GL_FALSE)
	{
		glGetProgramiv(shader_program_id, GL_INFO_LOG_LENGTH, &info_log_length);

		/* TODO: output the message */
		/*glGetProgramInfoLog(shader_program_id, InfoLogLength, NULL, &ProgramErrorMessage[0]);*/

		#ifdef DEBUG
			std::cout << "Error: Unable to link the shader program for the " << shader_pack_name << " pack"
				<< std::endl;
		#endif

		/* Clean up before abort */
		glDetachShader(shader_program_id, vertex_shader_id);
		glDetachShader(shader_program_id, fragment_shader_id);
		glDeleteShader(vertex_shader_id);
		glDeleteShader(fragment_shader_id);
		glDeleteProgram(m_shader_program_id);
		return;
	}

	/* Clean up */
	glDetachShader(shader_program_id, vertex_shader_id);
	glDetachShader(shader_program_id, fragment_shader_id);
	glDeleteShader(vertex_shader_id);
	glDeleteShader(fragment_shader_id);

	/* Save the shader program id and update status */
	m_shader_program_id = shader_program_id;
	m_compiled = true;
	m_shader_pack_name = shader_pack_name;

	#ifdef DEBUG
		std::cout << "Success: Shader pack " << shader_pack_name << " compiled" << std::endl;
	#endif
}

/* Select this shader program with the glUseProgram function */
void qwy::ShaderProgram::select() const
{
	#ifdef DEBUG
		if (not m_compiled)
		{
			std::cout << "Error: ShaderProgram method select called whereas it was not compiled" << std::endl;
		}
	#endif

	glUseProgram(m_shader_program_id);
}

/* Get the location of the specified uniform with the glGetUniformLocation function */
GLint qwy::ShaderProgram::get_uniform_location(std::string const& uniform_name) const
{
	return glGetUniformLocation(m_shader_program_id, static_cast<const GLchar*>(uniform_name.c_str()));
}

/* Set the specified mat4 uniform to the passed matrix with the glUniformMatrix4fv function */
void qwy::ShaderProgram::set_uniform_mat4(GLint uniform_location, glm::mat4 const& martix) const
{
	glUniformMatrix4fv(uniform_location, 1, GL_FALSE, &martix[0][0]);
}

void qwy::ShaderProgram::set_uniform_vec3(GLint uniform_location, glm::vec3 const& vector) const
{
	glUniform3f(uniform_location, vector.x, vector.y, vector.z);
}

void qwy::ShaderProgram::set_uniform_texture(GLint uniform_location, GLuint texture_unit) const
{
	glUniform1i(uniform_location, texture_unit);
}
