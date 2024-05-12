#ifndef PJGLHEADERS_H
#define PJGLHEADERS_H

#ifdef _WIN32

#include <gl/gl.h> 
#include <gl/glu.h>

#else

#include <OpenGL/glu.h>
#include <OpenGL/glext.h>
#include <GLUT/glut.h>
#include <OpenGL/gl3.h>
#include <OpenGL/gl3ext.h>

#endif

#endif
