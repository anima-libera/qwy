
/* Qwy project - Anima Libera <anim.libera@gmail.com> - github.com/anima-libera
 * Header fileio.hpp - Allows simple input/ouput with files */

#ifndef __qwy_fileio_header__
#define __qwy_fileio_header__

#include <string>

namespace qwy
{

	/* Load the file content into the string passed.
	 * Return true if the file had been loaded, false otherwise. */
	bool load_file_content(std::string const& file_path, std::string & out_string);

}

#endif /* __qwy_fileio_header__ */
