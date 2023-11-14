/* draw.h */

int compile_shader(GLuint *sh, GLenum shtype, const char *src);
int link_prog(GLuint *prog, size_t n, GLuint shds[n]);
