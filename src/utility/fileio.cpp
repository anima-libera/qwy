
#include "fileio.hpp"

#include <fstream>
#include <string>


/* Load the file content into the string passed.
 * Return true if the file had been loaded, false otherwise. */
bool qwy::load_file_content(std::string const& file_path, std::string & out_string)
{
	/* Open the file and check for errors */
	std::ifstream file(file_path, std::ios::in);
	if (not file)
	{
		/* TODO: handle error */

		out_string = "";
		return false;
	}

	/* Resize the string to optimize the copy */
	file.seekg(0, std::ios::end);
	out_string.resize(file.tellg());

	/* Just copy the file content into the string */
	file.seekg(0, std::ios::beg);
	file.read(&out_string[0], out_string.size());

	/* Ended */
	file.close();
	return true;
}
