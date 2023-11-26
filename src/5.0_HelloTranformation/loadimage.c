#include <stdio.h>
#include <stdlib.h>

#include <glad/gl.h>
#include <lodepng/lodepng.h>

#include "loadimage.h"

void
memswap(unsigned char *a, unsigned char *b, unsigned char *tmpb, size_t n)
{
	memcpy(tmpb, a, n);
	memcpy(a, b, n);
	memcpy(b, tmpb, n);
}


void
flipimg32(unsigned char *img, unsigned int w, unsigned int h)
{
	unsigned int x, y;
	unsigned char tmpb[4];

	for (x = 0; x < w; x++) {
		for (y = 0; y < h / 2; y++) {
			memswap(&img[(x + y * w) * 4],
				&img[((h-1 - y) * w + x) * 4],
				tmpb, 4);
		}
	}

	return;
}


int
loadimage(GLuint *tex, const char *imagepath)
{
	LodePNGInfo info;
	unsigned int err;
	unsigned int w, h;
	unsigned char *data;

	glGenTextures(1, tex);
	glBindTexture(GL_TEXTURE_2D, *tex);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	if ((err = lodepng_decode32_file(&data, &w, &h, imagepath))) {
		fprintf(stderr, "decoder error %u: %s\n", err, lodepng_error_text(err));
		fprintf(stderr, "Tried to load image with path: %s\n", imagepath);

		free(data);
		lodepng_info_cleanup(&info);
		return 0;
	}

	flipimg32(data, w, h);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);

	free(data);
	return 1;
}
