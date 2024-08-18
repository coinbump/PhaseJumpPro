#ifndef PJGLHEADERS_H
#define PJGLHEADERS_H

#ifdef _WIN32

#include <gl/gl.h>
#include <gl/glu.h>

#else

#include <GLUT/glut.h>
#include <OpenGL/gl3.h>
#include <OpenGL/gl3ext.h>
#include <OpenGL/glext.h>
#include <OpenGL/glu.h>

#endif

#endif
