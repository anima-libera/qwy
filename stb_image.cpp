
/* We ignore the warnings raised by this lib code */
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"

	/* Generate the stb_image code */
	#define STB_IMAGE_IMPLEMENTATION
	#include "stb_image.h"

#pragma GCC diagnostic pop
