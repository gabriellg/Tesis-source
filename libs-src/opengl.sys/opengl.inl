// includes de opengl.

#define GL_GLEXT_PROTOTYPES 1
#include <GL/gl.h>
#include <GL/glext.h>
#include <GL/glu.h>
#include <GL/glut.h>

#if !defined(GLUT_WHEEL_UP)
#define GLUT_WHEEL_UP   3
#define GLUT_WHEEL_DOWN 4
#endif
