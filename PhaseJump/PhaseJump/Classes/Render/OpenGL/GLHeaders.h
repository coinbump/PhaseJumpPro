#ifndef PJGLHEADERS_H
#define PJGLHEADERS_H

#ifdef _WIN32

// Windows.h must come before GL headers
// clang-format off
#include <Windows.h>
#include <gl/gl.h>
#include <gl/glu.h>

// Glchar is missing in gl.h
#ifndef GLchar
using GLchar = char;
#endif
// clang-format on

#else

#include <GLUT/glut.h>
#include <OpenGL/gl3.h>
#include <OpenGL/gl3ext.h>
#include <OpenGL/glext.h>
#include <OpenGL/glu.h>

#endif

#endif
