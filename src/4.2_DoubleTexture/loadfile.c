#include <stdio.h>
#include <stdlib.h>

#include "util.h"
#include "loadfile.h"

char *
loadfile(char *pnm)
{
	FILE *f;
	char *src, *err_name;
	long fsize;
	size_t nread;

	src = NULL;
	if (!(f = fopen(pnm, "r")))
		DO2(perror("fopen"), goto on_err);

	if (fseek(f, 0, SEEK_END) ||
	    (fsize = ftell(f)) < 0 ||
	    fseek(f, 0, SEEK_SET))
		DO2(perror("fseek"), goto on_err);

	if (!(src = malloc(fsize + 1)))
		goto on_err;

	nread = fread(src, 1, fsize, f);
	if (ferror(f))
		DO2(perror("fread"), goto on_err);
	src[fsize] = '\0';

	fclose(f);

	return src;
on_err:
	perror("error");
	if (f)
		fclose(f);
	if (src)
		free(src);
	return NULL;
}
