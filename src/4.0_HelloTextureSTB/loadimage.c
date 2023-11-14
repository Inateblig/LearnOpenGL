#include <stdio.h>


#include <glad/gl.h>

#include "loadimage.h"
#include "util.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"

int
loadimage(GLuint *tex, const char *imagepath)
{
	int w, h, nchannels;
	unsigned char *data;

	glGenTextures(1, tex);
	glBindTexture(GL_TEXTURE_2D, *tex);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);


	stbi_set_flip_vertically_on_load(1);
	if (!(data = stbi_load(imagepath, &w, &h, &nchannels, 0))) {
		DO2(fprintf(stderr, "loadimage failed!\n"), goto on_err);
	}

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);

	stbi_image_free(data);
	return 1;
on_err:
	stbi_image_free(data);
	return 0;
}
