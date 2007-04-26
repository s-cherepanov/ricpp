#ifndef _RICPP_GLCPP_OPENGL_H
#define _RICPP_GLCPP_OPENGL_H

// RICPP - RenderMan Interface CPP Language Binding
//         RenderMan is (R) by Pixar
//         Windows is (R) by Microsoft
//
// Copyright © 2001 - 2002, Andreas Pidde
//
// Contact: andreas@pidde.de
//
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public
// License as published by the Free Software Foundation; either
// version 2 of the License, or (at your option) any later version.
//  
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Public License for more details.
//
// You should have received a copy of the GNU General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

/*! \file opengl.h
 *  \brief Simple C++ interface to OpenGL
 *  \author Andreas Pidde (andreas@pidde.de)
 */

#include <GL/gl.h>

class TGL
{
public:

// A

	inline static void accum(GLenum op, GLfloat value) {glAccum(op, value);}
	inline static void alphaFunc(GLenum func, GLclampf ref) {glAlphaFunc(func, ref);}
	inline static GLboolean areTexturesResident(GLsizei n, const GLuint *textures, GLboolean *residences) {return glAreTexturesResident(n, textures, residences);}
	inline static void arrayElement(GLint i) {glArrayElement(i);}

// B

// BEGIN

	inline static void begin(GLenum mode) {glBegin(mode);}
	inline static void points(void) {glBegin(GL_POINTS);}
	inline static void lines(void) {glBegin(GL_LINES);}
	inline static void lineStrip(void) {glBegin(GL_LINE_STRIP);}
	inline static void lineLoop(void) {glBegin(GL_LINE_LOOP);}
	inline static void triangles(void) {glBegin(GL_TRIANGLES);}
	inline static void triangleStrip(void) {glBegin(GL_TRIANGLE_STRIP);}
	inline static void triangleFan(void) {glBegin(GL_TRIANGLE_FAN);}
	inline static void quads(void) {glBegin(GL_QUADS);}
	inline static void quadStrip(void) {glBegin(GL_QUAD_STRIP);}
	inline static void polygon(void) {glBegin(GL_POLYGON);}

	inline static void bindTexture(GLenum target, GLuint texture) {glBindTexture(target, texture);}
	inline static void bitmap(GLsizei width, GLsizei height, GLfloat xorig, GLfloat yorig, GLfloat xmove, GLfloat ymove, const GLubyte *bitmap) {glBitmap(width, height, xorig, yorig, xmove, ymove, bitmap);}
	inline static void blendFunc(GLenum sfactor, GLenum dfactor) {glBlendFunc(sfactor, dfactor);}

// C

	inline static void callList(GLuint list) {glCallList(list);}
	inline static void callLists(GLsizei n, GLenum type, const GLvoid *lists) {glCallLists(n, type, lists);}
	inline static void clear(GLbitfield mask) {glClear(mask);}
	inline static void clearAccum(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha) {glClearAccum(red, green, blue, alpha);}
	inline static void clearColor(GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha) {glClearColor(red, green, blue, alpha);}
	inline static void clearDepth(GLclampd depth) {glClearDepth(depth);}
	inline static void clearIndex(GLfloat c) {glClearIndex(c);}
	inline static void clearStencil(GLint s) {glClearStencil(s);}
	inline static void clipPlane(GLenum plane, const GLdouble *equation) {glClipPlane(plane, equation);}

// COLOR

	inline static void color(GLbyte red, GLbyte green, GLbyte blue) {glColor3b(red, green, blue);}
	inline static void color3(const GLbyte *v) {glColor3bv(v);}
	inline static void color(GLdouble red, GLdouble green, GLdouble blue) {glColor3d(red, green, blue);}
	inline static void color3(const GLdouble *v) {glColor3dv(v);}
	inline static void color(GLfloat red, GLfloat green, GLfloat blue) {glColor3f(red, green, blue);}
	inline static void color3(const GLfloat *v) {glColor3fv(v);}
	inline static void color(GLint red, GLint green, GLint blue) {glColor3i(red, green, blue);}
	inline static void color3(const GLint *v) {glColor3iv(v);}
	inline static void color(GLshort red, GLshort green, GLshort blue) {glColor3s(red, green, blue);}
	inline static void color3(const GLshort *v) {glColor3sv(v);}
	inline static void color(GLubyte red, GLubyte green, GLubyte blue) {glColor3ub(red, green, blue);}
	inline static void color3(const GLubyte *v) {glColor3ubv(v);}
	inline static void color(GLuint red, GLuint green, GLuint blue) {glColor3ui(red, green, blue);}
	inline static void color3(const GLuint *v) {glColor3uiv(v);}
	inline static void color(GLushort red, GLushort green, GLushort blue) {glColor3us(red, green, blue);}
	inline static void color3(const GLushort *v) {glColor3usv(v);}
	inline static void color(GLbyte red, GLbyte green, GLbyte blue, GLbyte alpha) {glColor4b(red, green, blue, alpha);}
	inline static void color4(const GLbyte *v) {glColor4bv(v);}
	inline static void color(GLdouble red, GLdouble green, GLdouble blue, GLdouble alpha) {glColor4d(red, green, blue, alpha);}
	inline static void color4(const GLdouble *v) {glColor4dv(v);}
	inline static void color(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha) {glColor4f(red, green, blue, alpha);}
	inline static void color4(const GLfloat *v) {glColor4fv(v);}
	inline static void color(GLint red, GLint green, GLint blue, GLint alpha) {glColor4i(red, green, blue, alpha);}
	inline static void color4(const GLint *v) {glColor4iv(v);}
	inline static void color(GLshort red, GLshort green, GLshort blue, GLshort alpha) {glColor4s(red, green, blue, alpha);}
	inline static void color4(const GLshort *v) {glColor4sv(v);}
	inline static void color(GLubyte red, GLubyte green, GLubyte blue, GLubyte alpha) {glColor4ub(red, green, blue, alpha);}
	inline static void color4(const GLubyte *v) {glColor4ubv(v);}
	inline static void color(GLuint red, GLuint green, GLuint blue, GLuint alpha) {glColor4ui(red, green, blue, alpha);}
	inline static void color4(const GLuint *v) {glColor4uiv(v);}
	inline static void color(GLushort red, GLushort green, GLushort blue, GLushort alpha) {glColor4us(red, green, blue, alpha);}
	inline static void color4(const GLushort *v) {glColor4usv(v);}

	inline static void colorMask(GLboolean red, GLboolean green, GLboolean blue, GLboolean alpha) {glColorMask(red, green, blue, alpha);}
	inline static void colorMaterial(GLenum face, GLenum mode) {glColorMaterial(face, mode);}

	inline static void colorPointer(GLint size, GLenum type, GLsizei stride, const GLvoid *pointer) {glColorPointer(size, type, stride, pointer);}
	inline static void colorPointer(GLint size, GLsizei stride, const GLdouble *pointer) {glColorPointer(size, GL_DOUBLE, sizeof(GLdouble)*stride, pointer);}
	inline static void colorPointer(GLint size, GLsizei stride, const GLfloat *pointer) {glColorPointer(size, GL_FLOAT, sizeof(GLfloat)*stride, pointer);}
	inline static void colorPointer(GLint size, GLsizei stride, const GLint *pointer) {glColorPointer(size, GL_INT, sizeof(GLint)*stride, pointer);}
	inline static void colorPointer(GLint size, GLsizei stride, const GLuint *pointer) {glColorPointer(size, GL_UNSIGNED_INT, sizeof(GLuint)*stride, pointer);}
	inline static void colorPointer(GLint size, GLsizei stride, const GLshort *pointer) {glColorPointer(size, GL_SHORT, sizeof(GLshort)*stride, pointer);}
	inline static void colorPointer(GLint size, GLsizei stride, const GLushort *pointer) {glColorPointer(size, GL_UNSIGNED_SHORT, sizeof(GLushort)*stride, pointer);}
	inline static void colorPointer(GLint size, GLsizei stride, const GLbyte *pointer) {glColorPointer(size, GL_BYTE, sizeof(GLbyte)*stride, pointer);}
	inline static void colorPointer(GLint size, GLsizei stride, const GLubyte *pointer) {glColorPointer(size, GL_UNSIGNED_BYTE, sizeof(GLubyte)*stride, pointer);}

	inline static void copyPixels(GLint x, GLint y, GLsizei width, GLsizei height, GLenum type) {glCopyPixels(x, y, width, height, type);}
	inline static void copyTexImage1D(GLenum target, GLint level, GLenum internalFormat, GLint x, GLint y, GLsizei width, GLint border) {glCopyTexImage1D(target, level, internalFormat, x, y, width, border);}
	inline static void copyTexImage2D(GLenum target, GLint level, GLenum internalFormat, GLint x, GLint y, GLsizei width, GLsizei height, GLint border) {glCopyTexImage2D(target, level, internalFormat, x, y, width, height, border);}
	inline static void copyTexSubImage1D(GLenum target, GLint level, GLint xoffset, GLint x, GLint y, GLsizei width) {glCopyTexSubImage1D(target, level, xoffset, x, y, width);}
	inline static void copyTexSubImage2D(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint x, GLint y, GLsizei width, GLsizei height) {glCopyTexSubImage2D(target, level, xoffset, yoffset, x, y, width, height);}
	inline static void cullFace(GLenum mode) {glCullFace(mode);}

// D

	inline static void deleteLists(GLuint list, GLsizei range) {glDeleteLists(list, range);}
	inline static void deleteTextures(GLsizei n, const GLuint *textures) {glDeleteTextures(n, textures);}
	inline static void depthFunc(GLenum func) {glDepthFunc(func);}
	inline static void depthMask(GLboolean flag) {glDepthMask(flag);}
	inline static void depthRange(GLclampd zNear, GLclampd zFar) {glDepthRange(zNear, zFar);}
	inline static void disable(GLenum cap) {glDisable(cap);}
	inline static void disableClientState(GLenum array) {glDisableClientState(array);}
	inline static void drawArrays(GLenum mode, GLint first, GLsizei count) {glDrawArrays(mode, first, count);}
	inline static void drawBuffer(GLenum mode) {glDrawBuffer(mode);}

	inline static void drawElements(GLenum mode, GLsizei count, GLenum type, const GLvoid *indices) {glDrawElements(mode, count, type, indices);}
	inline static void drawElements(GLenum mode, GLsizei count, const GLuint *indices) {glDrawElements(mode, count, GL_UNSIGNED_INT, indices);}
	inline static void drawElements(GLenum mode, GLsizei count, const GLushort *indices) {glDrawElements(mode, count, GL_UNSIGNED_SHORT, indices);}
	inline static void drawElements(GLenum mode, GLsizei count, const GLubyte *indices) {glDrawElements(mode, count, GL_UNSIGNED_BYTE, indices);}

	inline static void drawPixels(GLsizei width, GLsizei height, GLenum format, GLenum type, const GLvoid *pixels) {glDrawPixels(width, height, format, type, pixels);}

// E

	inline static void edgeFlag(GLboolean flag) {glEdgeFlag(flag);}
	inline static void edgeFlag(const GLboolean *flag) {glEdgeFlagv(flag);}
	inline static void edgeFlagPointer(GLsizei stride, const GLvoid *pointer) {glEdgeFlagPointer(stride, (GLboolean *)pointer);}
	inline static void edgeFlagPointer(GLsizei stride, const GLboolean *pointer) {glEdgeFlagPointer(sizeof(GLboolean)*stride, pointer);}
	inline static void enable(GLenum cap) {glEnable(cap);}
	inline static void enableClientState(GLenum array) {glEnableClientState(array);}
	inline static void end(void) {glEnd();}
	inline static void endList(void) {glEndList();}

	// EvalCoord...

	inline static void evalCoord(GLdouble u) {glEvalCoord1d(u);}
	inline static void evalCoord1(const GLdouble *u) {glEvalCoord1dv(u);}
	inline static void evalCoord(GLfloat u) {glEvalCoord1f(u);}
	inline static void evalCoord1(const GLfloat *u) {glEvalCoord1fv(u);}
	inline static void evalCoord(GLdouble u, GLdouble v) {glEvalCoord2d(u, v);}
	inline static void evalCoord2(const GLdouble *u) {glEvalCoord2dv(u);}
	inline static void evalCoord(GLfloat u, GLfloat v) {glEvalCoord2f(u, v);}
	inline static void evalCoord2(const GLfloat *u) {glEvalCoord2fv(u);}

	inline static void evalMesh(GLenum mode, GLint i1, GLint i2) {glEvalMesh1(mode, i1, i2);}
	inline static void evalMesh(GLenum mode, GLint i1, GLint i2, GLint j1, GLint j2) {glEvalMesh2(mode, i1, i2, j1, j2);}

	inline static void evalPoint(GLint i) {glEvalPoint1(i);}
	inline static void evalPoint(GLint i, GLint j) {glEvalPoint2(i, j);}
    
// F

	inline static void feedbackBuffer(GLsizei size, GLenum type, GLfloat *buffer) {glFeedbackBuffer(size, type, buffer);}
	inline static void finish(void) {glFinish();}
	inline static void flush(void) {glFlush();}

	inline static void fog(GLenum pname, GLfloat param) {glFogf(pname, param);}
	inline static void fog(GLenum pname, const GLfloat *params) {glFogfv(pname, params);}
	inline static void fog(GLenum pname, GLint param) {glFogi(pname, param);}
	inline static void fog(GLenum pname, const GLint *params) {glFogiv(pname, params);}

	inline static void frontFace(GLenum mode) {glFrontFace(mode);}
	inline static void frustum(GLdouble left, GLdouble right, GLdouble bottom, GLdouble top, GLdouble zNear, GLdouble zFar) {glFrustum(left, right, bottom, top, zNear, zFar);}

// G

	inline static GLuint genLists(GLsizei range) {return glGenLists(range);}
	inline static void genTextures(GLsizei n, GLuint *textures) {glGenTextures(n, textures);}
	inline static void get(GLenum pname, GLboolean *params) {glGetBooleanv(pname, params);}
	inline static void get(GLenum pname, GLdouble *params) {glGetDoublev(pname, params);}
	inline static void get(GLenum pname, GLfloat *params) {glGetFloatv(pname, params);}
	inline static void get(GLenum pname, GLint *params) {glGetIntegerv(pname, params);}
	inline static void getClipPlane(GLenum plane, GLdouble *equation) {glGetClipPlane(plane, equation);}
	inline static GLenum getError(void) {return glGetError();}
	inline static void getLight(GLenum light, GLenum pname, GLfloat *params) {glGetLightfv(light, pname, params);}
	inline static void getLight(GLenum light, GLenum pname, GLint *params) {glGetLightiv(light, pname, params);}
	inline static void getMap(GLenum target, GLenum query, GLdouble *v) {glGetMapdv(target, query, v);}
	inline static void getMap(GLenum target, GLenum query, GLfloat *v) {glGetMapfv(target, query, v);}
	inline static void getMap(GLenum target, GLenum query, GLint *v) {glGetMapiv(target, query, v);}
	inline static void getMaterial(GLenum face, GLenum pname, GLfloat *params) {glGetMaterialfv(face, pname, params);}
	inline static void getMaterial(GLenum face, GLenum pname, GLint *params) {glGetMaterialiv(face, pname, params);}
	inline static void getPixelMap(GLenum map, GLfloat *values) {glGetPixelMapfv(map, values);}
	inline static void getPixelMap(GLenum map, GLuint *values) {glGetPixelMapuiv(map, values);}
	inline static void getPixelMap(GLenum map, GLushort *values) {glGetPixelMapusv(map, values);}
	inline static void getPointerv(GLenum pname, GLvoid* *params) {glGetPointerv(pname, params);}
	inline static void getPolygonStipple(GLubyte *mask) {glGetPolygonStipple(mask);}
	inline static const GLubyte *getString(GLenum name) {return glGetString(name);}
	inline static void getTexEnv(GLenum target, GLenum pname, GLfloat *params) {glGetTexEnvfv(target, pname, params);}
	inline static void getTexEnv(GLenum target, GLenum pname, GLint *params) {glGetTexEnviv(target, pname, params);}
	inline static void getTexGen(GLenum coord, GLenum pname, GLdouble *params) {glGetTexGendv(coord, pname, params);}
	inline static void getTexGen(GLenum coord, GLenum pname, GLfloat *params) {glGetTexGenfv(coord, pname, params);}
	inline static void getTexGen(GLenum coord, GLenum pname, GLint *params) {glGetTexGeniv(coord, pname, params);}
	inline static void getTexImage(GLenum target, GLint level, GLenum format, GLenum type, GLvoid *pixels) {glGetTexImage(target, level, format, type, pixels);}
	inline static void getTexLevelParameter(GLenum target, GLint level, GLenum pname, GLfloat *params) {glGetTexLevelParameterfv(target, level, pname, params);}
	inline static void getTexLevelParameter(GLenum target, GLint level, GLenum pname, GLint *params) {glGetTexLevelParameteriv(target, level, pname, params);}
	inline static void getTexParameter(GLenum target, GLenum pname, GLfloat *params) {glGetTexParameterfv(target, pname, params);}
	inline static void getTexParameter(GLenum target, GLenum pname, GLint *params) {glGetTexParameteriv(target, pname, params);}

// H

	inline static void hint(GLenum target, GLenum mode) {glHint(target, mode);}

// I

	inline static void indexMask(GLuint mask) {glIndexMask(mask);}

	inline static void indexPointer(GLenum type, GLsizei stride, const GLvoid *pointer) {glIndexPointer(type, stride, pointer);}
	inline static void indexPointer(GLsizei stride, const GLdouble *pointer) {glIndexPointer(GL_DOUBLE, sizeof(GLdouble)*stride, pointer);}
	inline static void indexPointer(GLsizei stride, const GLfloat *pointer) {glIndexPointer(GL_FLOAT, sizeof(GLfloat)*stride, pointer);}
	inline static void indexPointer(GLsizei stride, const GLint *pointer) {glIndexPointer(GL_INT, sizeof(GLint)*stride, pointer);}
	inline static void indexPointer(GLsizei stride, const GLshort *pointer) {glIndexPointer(GL_SHORT, sizeof(GLshort)*stride, pointer);}
	inline static void indexPointer(GLsizei stride, const GLubyte *pointer) {glIndexPointer(GL_UNSIGNED_BYTE, sizeof(GLubyte)*stride, pointer);}

	inline static void index(GLdouble c) {glIndexd(c);}
	inline static void index(const GLdouble *c) {glIndexdv(c);}
	inline static void index(GLfloat c) {glIndexf(c);}
	inline static void index(const GLfloat *c) {glIndexfv(c);}
	inline static void index(GLint c) {glIndexi(c);}
	inline static void index(const GLint *c) {glIndexiv(c);}
	inline static void index(GLshort c) {glIndexs(c);}
	inline static void index(const GLshort *c) {glIndexsv(c);}
	inline static void initNames(void) {glInitNames();}
	inline static void interleavedArrays(GLenum format, GLsizei stride, const GLvoid *pointer) {glInterleavedArrays(format, stride, pointer);}
	inline static GLboolean isEnabled(GLenum cap) {return glIsEnabled(cap);}
	inline static GLboolean isList(GLuint list) {return glIsList(list);}
	inline static GLboolean isTexture(GLuint texture) {return glIsTexture(texture);}

// L

	inline static void lightModel(GLenum pname, GLfloat param) {glLightModelf(pname, param);}
	inline static void lightModel(GLenum pname, const GLfloat *params) {glLightModelfv(pname, params);}
	inline static void lightModel(GLenum pname, GLint param) {glLightModeli(pname, param);}
	inline static void lightModel(GLenum pname, const GLint *params) {glLightModeliv(pname, params);}
	inline static void light(GLenum light, GLenum pname, GLfloat param) {glLightf(light, pname, param);}
	inline static void light(GLenum light, GLenum pname, const GLfloat *params) {glLightfv(light, pname, params);}
	inline static void light(GLenum light, GLenum pname, GLint param) {glLighti(light, pname, param);}
	inline static void light(GLenum light, GLenum pname, const GLint *params) {glLightiv(light, pname, params);}
	inline static void lineStipple(GLint factor, GLushort pattern) {glLineStipple(factor, pattern);}
	inline static void lineWidth(GLfloat width) {glLineWidth(width);}
	inline static void listBase(GLuint base) {glListBase(base);}
	inline static void loadIdentity(void) {glLoadIdentity();}
	inline static void loadMatrix(const GLdouble *m) {glLoadMatrixd(m);}
	inline static void loadMatrix(const GLfloat *m) {glLoadMatrixf(m);}
	inline static void loadName(GLuint name) {glLoadName(name);}
	inline static void logicOp(GLenum opcode) {glLogicOp(opcode);}

// M

	inline static void map(GLenum target, GLdouble u1, GLdouble u2, GLint stride, GLint order, const GLdouble *points) {glMap1d(target, u1, u2, stride, order, points);}
	inline static void map(GLenum target, GLfloat u1, GLfloat u2, GLint stride, GLint order, const GLfloat *points) {glMap1f(target, u1, u2, stride, order, points);}
	inline static void map(GLenum target, GLdouble u1, GLdouble u2, GLint ustride, GLint uorder, GLdouble v1, GLdouble v2, GLint vstride, GLint vorder, const GLdouble *points) {glMap2d(target, u1, u2, ustride, uorder, v1, v2, vstride, vorder, points);}
	inline static void map(GLenum target, GLfloat u1, GLfloat u2, GLint ustride, GLint uorder, GLfloat v1, GLfloat v2, GLint vstride, GLint vorder, const GLfloat *points) {glMap2f(target, u1, u2, ustride, uorder, v1, v2, vstride, vorder, points);}
	inline static void mapGrid(GLint un, GLdouble u1, GLdouble u2) {glMapGrid1d(un, u1, u2);}
	inline static void mapGrid(GLint un, GLfloat u1, GLfloat u2) {glMapGrid1f(un, u1, u2);}
	inline static void mapGrid(GLint un, GLdouble u1, GLdouble u2, GLint vn, GLdouble v1, GLdouble v2) {glMapGrid2d(un, u1, u2, vn, v1, v2);}
	inline static void mapGrid(GLint un, GLfloat u1, GLfloat u2, GLint vn, GLfloat v1, GLfloat v2) {glMapGrid2f(un, u1, u2, vn, v1, v2);}
	inline static void material(GLenum face, GLenum pname, GLfloat param) {glMaterialf(face, pname, param);}
	inline static void material(GLenum face, GLenum pname, const GLfloat *params) {glMaterialfv(face, pname, params);}
	inline static void material(GLenum face, GLenum pname, GLint param) {glMateriali(face, pname, param);}
	inline static void material(GLenum face, GLenum pname, const GLint *params) {glMaterialiv(face, pname, params);}
	inline static void matrixMode(GLenum mode) {glMatrixMode(mode);}
	inline static void multMatrix(const GLdouble *m) {glMultMatrixd(m);}
	inline static void multMatrix(const GLfloat *m) {glMultMatrixf(m);}

// N

	inline static void newList(GLuint list, GLenum mode) {glNewList(list, mode);}

	inline static void normal(GLbyte nx, GLbyte ny, GLbyte nz) {glNormal3b(nx, ny, nz);}
	inline static void normal(const GLbyte *v) {glNormal3bv(v);}
	inline static void normal(GLdouble nx, GLdouble ny, GLdouble nz) {glNormal3d(nx, ny, nz);}
	inline static void normal(const GLdouble *v) {glNormal3dv(v);}
	inline static void normal(GLfloat nx, GLfloat ny, GLfloat nz) {glNormal3f(nx, ny, nz);}
	inline static void normal(const GLfloat *v) {glNormal3fv(v);}
	inline static void normal(GLint nx, GLint ny, GLint nz) {glNormal3i(nx, ny, nz);}
	inline static void normal(const GLint *v) {glNormal3iv(v);}
	inline static void normal(GLshort nx, GLshort ny, GLshort nz) {glNormal3s(nx, ny, nz);}
	inline static void normal(const GLshort *v) {glNormal3sv(v);}

	inline static void normalPointer(GLenum type, GLsizei stride, const GLvoid *pointer) {glNormalPointer(type, stride, pointer);}
	inline static void normalPointer(GLsizei stride, const GLdouble *pointer) {glNormalPointer(GL_DOUBLE, sizeof(GLdouble)*stride, pointer);}
	inline static void normalPointer(GLsizei stride, const GLfloat *pointer) {glNormalPointer(GL_FLOAT, sizeof(GLfloat)*stride, pointer);}
	inline static void normalPointer(GLsizei stride, const GLint *pointer) {glNormalPointer(GL_INT, sizeof(GLint)*stride, pointer);}
	inline static void normalPointer(GLsizei stride, const GLshort *pointer) {glNormalPointer(GL_SHORT, sizeof(GLshort)*stride, pointer);}
	inline static void normalPointer(GLsizei stride, const GLbyte *pointer) {glNormalPointer(GL_BYTE, sizeof(GLbyte)*stride, pointer);}

// O

	inline static void ortho(GLdouble left, GLdouble right, GLdouble bottom, GLdouble top, GLdouble zNear, GLdouble zFar) {glOrtho(left, right, bottom, top, zNear, zFar);}

// P

	inline static void passThrough(GLfloat token) {glPassThrough(token);}
	inline static void pixelMap(GLenum map, GLsizei mapsize, const GLfloat *values) {glPixelMapfv(map, mapsize, values);}
	inline static void pixelMap(GLenum map, GLsizei mapsize, const GLuint *values) {glPixelMapuiv(map, mapsize, values);}
	inline static void pixelMap(GLenum map, GLsizei mapsize, const GLushort *values) {glPixelMapusv(map, mapsize, values);}
	inline static void pixelStore(GLenum pname, GLfloat param) {glPixelStoref(pname, param);}
	inline static void pixelStore(GLenum pname, GLint param) {glPixelStorei(pname, param);}
	inline static void pixelTransfer(GLenum pname, GLfloat param) {glPixelTransferf(pname, param);}
	inline static void pixelTransfer(GLenum pname, GLint param) {glPixelTransferi(pname, param);}
	inline static void pixelZoom(GLfloat xfactor, GLfloat yfactor) {glPixelZoom(xfactor, yfactor);}
	inline static void pointSize(GLfloat size) {glPointSize(size);}
	inline static void polygonMode(GLenum face, GLenum mode) {glPolygonMode(face, mode);}
	inline static void polygonOffset(GLfloat factor, GLfloat units) {glPolygonOffset(factor, units);}
	inline static void polygonStipple(const GLubyte *mask) {glPolygonStipple(mask);}
	inline static void popAttrib(void) {glPopAttrib();}
	inline static void popClientAttrib(void) {glPopClientAttrib();}
	inline static void popMatrix(void) {glPopMatrix();}
	inline static void popName(void) {glPopName();}
	inline static void prioritizeTextures(GLsizei n, const GLuint *textures, const GLclampf *priorities) {glPrioritizeTextures(n, textures, priorities);}
	inline static void pushAttrib(GLbitfield mask) {glPushAttrib(mask);}
	inline static void pushClientAttrib(GLbitfield mask) {glPushClientAttrib(mask);}
	inline static void pushMatrix(void) {glPushMatrix();}
	inline static void pushName(GLuint name) {glPushName(name);}

// R

// RASTERPOS...

	inline static void rasterPos(GLdouble x, GLdouble y) {glRasterPos2d(x, y);}
	inline static void rasterPos2(const GLdouble *v) {glRasterPos2dv(v);}
	inline static void rasterPos(GLfloat x, GLfloat y) {glRasterPos2f(x, y);}
	inline static void rasterPos2(const GLfloat *v) {glRasterPos2fv(v);}
	inline static void rasterPos(GLint x, GLint y) {glRasterPos2i(x, y);}
	inline static void rasterPos2(const GLint *v) {glRasterPos2iv(v);}
	inline static void rasterPos(GLshort x, GLshort y) {glRasterPos2s(x, y);}
	inline static void rasterPos2(const GLshort *v) {glRasterPos2sv(v);}
	inline static void rasterPos(GLdouble x, GLdouble y, GLdouble z) {glRasterPos3d(x, y, z);}
	inline static void rasterPos3(const GLdouble *v) {glRasterPos3dv(v);}
	inline static void rasterPos(GLfloat x, GLfloat y, GLfloat z) {glRasterPos3f(x, y, z);}
	inline static void rasterPos3(const GLfloat *v) {glRasterPos3fv(v);}
	inline static void rasterPos(GLint x, GLint y, GLint z) {glRasterPos3i(x, y, z);}
	inline static void rasterPos3(const GLint *v) {glRasterPos3iv(v);}
	inline static void rasterPos(GLshort x, GLshort y, GLshort z) {glRasterPos3s(x, y, z);}
	inline static void rasterPos3(const GLshort *v) {glRasterPos3sv(v);}
	inline static void rasterPos(GLdouble x, GLdouble y, GLdouble z, GLdouble w) {glRasterPos4d(x, y, z, w);}
	inline static void rasterPos4(const GLdouble *v) {glRasterPos4dv(v);}
	inline static void rasterPos(GLfloat x, GLfloat y, GLfloat z, GLfloat w) {glRasterPos4f(x, y, z, w);}
	inline static void rasterPos4(const GLfloat *v) {glRasterPos4fv(v);}
	inline static void rasterPos(GLint x, GLint y, GLint z, GLint w) {glRasterPos4i(x, y, z, w);}
	inline static void rasterPos4(const GLint *v) {glRasterPos4iv(v);}
	inline static void rasterPos(GLshort x, GLshort y, GLshort z, GLshort w) {glRasterPos4s(x, y, z, w);}
	inline static void rasterPos4(const GLshort *v) {glRasterPos4sv(v);}

	inline static void readBuffer(GLenum mode) {glReadBuffer(mode);}
	inline static void readPixels(GLint x, GLint y, GLsizei width, GLsizei height, GLenum format, GLenum type, GLvoid *pixels) {glReadPixels(x, y, width, height, format, type, pixels);}

// Rect...

	inline static void rect(GLdouble x1, GLdouble y1, GLdouble x2, GLdouble y2) {glRectd(x1, y1, x2, y2);}
	inline static void rect(const GLdouble *v1, const GLdouble *v2) {glRectdv(v1, v2);}
	inline static void rect(GLfloat x1, GLfloat y1, GLfloat x2, GLfloat y2) {glRectf(x1, y1, x2, y2);}
	inline static void rect(const GLfloat *v1, const GLfloat *v2) {glRectfv(v1, v2);}
	inline static void rect(GLint x1, GLint y1, GLint x2, GLint y2) {glRecti(x1, y1, x2, y2);}
	inline static void rect(const GLint *v1, const GLint *v2) {glRectiv(v1, v2);}
	inline static void rect(GLshort x1, GLshort y1, GLshort x2, GLshort y2) {glRects(x1, y1, x2, y2);}
	inline static void rect(const GLshort *v1, const GLshort *v2) {glRectsv(v1, v2);}

	inline static GLint renderMode(GLenum mode) {return glRenderMode(mode);}
	inline static void rotate(GLdouble angle, GLdouble x, GLdouble y, GLdouble z) {glRotated(angle, x, y, z);}
	inline static void rotate(GLfloat angle, GLfloat x, GLfloat y, GLfloat z) {glRotatef(angle, x, y, z);}

// S

	inline static void scale(GLdouble x, GLdouble y, GLdouble z) {glScaled(x, y, z);}
	inline static void scale(GLfloat x, GLfloat y, GLfloat z) {glScalef(x, y, z);}
	inline static void scissor(GLint x, GLint y, GLsizei width, GLsizei height) {glScissor(x, y, width, height);}
	inline static void selectBuffer(GLsizei size, GLuint *buffer) {glSelectBuffer(size, buffer);}
	inline static void shadeModel(GLenum mode) {glShadeModel(mode);}
	inline static void stencilFunc(GLenum func, GLint ref, GLuint mask) {glStencilFunc(func, ref, mask);}
	inline static void stencilMask(GLuint mask) {glStencilMask(mask);}
	inline static void stencilOp(GLenum fail, GLenum zfail, GLenum zpass) {glStencilOp(fail, zfail, zpass);}

// T

// TexCoord...

	inline static void texCoord(GLdouble s) {glTexCoord1d(s);}
	inline static void texCoord1(const GLdouble *v) {glTexCoord1dv(v);}
	inline static void texCoord(GLfloat s) {glTexCoord1f(s);}
	inline static void texCoord1(const GLfloat *v) {glTexCoord1fv(v);}
	inline static void texCoord(GLint s) {glTexCoord1i(s);}
	inline static void texCoord1(const GLint *v) {glTexCoord1iv(v);}
	inline static void texCoord(GLshort s) {glTexCoord1s(s);}
	inline static void texCoord1(const GLshort *v) {glTexCoord1sv(v);}
	inline static void texCoord(GLdouble s, GLdouble t) {glTexCoord2d(s, t);}
	inline static void texCoord2(const GLdouble *v) {glTexCoord2dv(v);}
	inline static void texCoord(GLfloat s, GLfloat t) {glTexCoord2f(s, t);}
	inline static void texCoord2(const GLfloat *v) {glTexCoord2fv(v);}
	inline static void texCoord(GLint s, GLint t) {glTexCoord2i(s, t);}
	inline static void texCoord2(const GLint *v) {glTexCoord2iv(v);}
	inline static void texCoord(GLshort s, GLshort t) {glTexCoord2s(s, t);}
	inline static void texCoord2(const GLshort *v) {glTexCoord2sv(v);}
	inline static void texCoord(GLdouble s, GLdouble t, GLdouble r) {glTexCoord3d(s, t, r);}
	inline static void texCoord3(const GLdouble *v) {glTexCoord3dv(v);}
	inline static void texCoord(GLfloat s, GLfloat t, GLfloat r) {glTexCoord3f(s, t, r);}
	inline static void texCoord3(const GLfloat *v) {glTexCoord3fv(v);}
	inline static void texCoord(GLint s, GLint t, GLint r) {glTexCoord3i(s, t, r);}
	inline static void texCoord3(const GLint *v) {glTexCoord3iv(v);}
	inline static void texCoord(GLshort s, GLshort t, GLshort r) {glTexCoord3s(s, t, r);}
	inline static void texCoord3(const GLshort *v) {glTexCoord3sv(v);}
	inline static void texCoord(GLdouble s, GLdouble t, GLdouble r, GLdouble q) {glTexCoord4d(s, t, r, q);}
	inline static void texCoord4(const GLdouble *v) {glTexCoord4dv(v);}
	inline static void texCoord(GLfloat s, GLfloat t, GLfloat r, GLfloat q) {glTexCoord4f(s, t, r, q);}
	inline static void texCoord4(const GLfloat *v) {glTexCoord4fv(v);}
	inline static void texCoord(GLint s, GLint t, GLint r, GLint q) {glTexCoord4i(s, t, r, q);}
	inline static void texCoord4(const GLint *v) {glTexCoord4iv(v);}
	inline static void texCoord(GLshort s, GLshort t, GLshort r, GLshort q) {glTexCoord4s(s, t, r, q);}
	inline static void texCoord4(const GLshort *v) {glTexCoord4sv(v);}

	inline static void texCoordPointer(GLint size, GLenum type, GLsizei stride, const GLvoid *pointer) {glTexCoordPointer(size, type, stride, pointer);}
	inline static void texCoordPointer(GLint size, GLsizei stride, const GLdouble *pointer) {glTexCoordPointer(size, GL_DOUBLE, sizeof(GLdouble)*stride, pointer);}
	inline static void texCoordPointer(GLint size, GLsizei stride, const GLfloat *pointer) {glTexCoordPointer(size, GL_FLOAT, sizeof(GLfloat)*stride, pointer);}
	inline static void texCoordPointer(GLint size, GLsizei stride, const GLint *pointer) {glTexCoordPointer(size, GL_INT, sizeof(GLint)*stride, pointer);}
	inline static void texCoordPointer(GLint size, GLsizei stride, const GLshort *pointer) {glTexCoordPointer(size, GL_SHORT, sizeof(GLshort)*stride, pointer);}

// TexEnv...

	inline static void texEnv(GLenum target, GLenum pname, GLfloat param) {glTexEnvf(target, pname, param);}
	inline static void texEnv(GLenum target, GLenum pname, const GLfloat *params) {glTexEnvfv(target, pname, params);}
	inline static void texEnv(GLenum target, GLenum pname, GLint param) {glTexEnvi(target, pname, param);}
	inline static void texEnv(GLenum target, GLenum pname, const GLint *params) {glTexEnviv(target, pname, params);}

// TexGen...

	inline static void texGen(GLenum coord, GLenum pname, GLdouble param) {glTexGend(coord, pname, param);}
	inline static void texGen(GLenum coord, GLenum pname, const GLdouble *params) {glTexGendv(coord, pname, params);}
	inline static void texGen(GLenum coord, GLenum pname, GLfloat param) {glTexGenf(coord, pname, param);}
	inline static void texGen(GLenum coord, GLenum pname, const GLfloat *params) {glTexGenfv(coord, pname, params);}
	inline static void texGen(GLenum coord, GLenum pname, GLint param) {glTexGeni(coord, pname, param);}
	inline static void texGen(GLenum coord, GLenum pname, const GLint *params) {glTexGeniv(coord, pname, params);}

	inline static void texImage1D(GLenum target, GLint level, GLint components, GLsizei width, GLint border, GLenum format, GLenum type, const GLvoid *pixels) {glTexImage1D(target, level, components, width, border, format, type, pixels);}
	inline static void texImage2D(GLenum target, GLint level, GLint components, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const GLvoid *pixels) {glTexImage2D(target, level, components, width, height, border, format, type, pixels);}
	inline static void texParameter(GLenum target, GLenum pname, GLfloat param) {glTexParameterf(target, pname, param);}
	inline static void texParameter(GLenum target, GLenum pname, const GLfloat *params) {glTexParameterfv(target, pname, params);}
	inline static void texParameter(GLenum target, GLenum pname, GLint param) {glTexParameteri(target, pname, param);}
	inline static void texParameter(GLenum target, GLenum pname, const GLint *params) {glTexParameteriv(target, pname, params);}
	inline static void texSubImage1D(GLenum target, GLint level, GLint xoffset, GLsizei width, GLenum format, GLenum type, const GLvoid *pixels) {glTexSubImage1D(target, level, xoffset, width, format, type, pixels);}
	inline static void texSubImage2D(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLenum type, const GLvoid *pixels) {glTexSubImage2D(target, level, xoffset, yoffset, width, height, format, type, pixels);}
	inline static void translate(GLdouble x, GLdouble y, GLdouble z) {glTranslated(x, y, z);}
	inline static void translate(GLfloat x, GLfloat y, GLfloat z) {glTranslatef(x, y, z);}

// V

// VERTEX

	inline static void vertex(GLdouble x, GLdouble y) {glVertex2d(x, y);}
	inline static void vertex2(const GLdouble *v) {glVertex2dv(v);}
	inline static void vertex(GLfloat x, GLfloat y) {glVertex2f(x, y);}
	inline static void vertex2(const GLfloat *v) {glVertex2fv(v);}
	inline static void vertex(GLint x, GLint y) {glVertex2i(x, y);}
	inline static void vertex2(const GLint *v) {glVertex2iv(v);}
	inline static void vertex(GLshort x, GLshort y) {glVertex2s(x, y);}
	inline static void vertex2(const GLshort *v) {glVertex2sv(v);}

	inline static void vertex(GLdouble x, GLdouble y, GLdouble z) {glVertex3d(x, y, z);}
	inline static void vertex3(const GLdouble *v) {glVertex3dv(v);}
	inline static void vertex(GLfloat x, GLfloat y, GLfloat z) {glVertex3f(x, y, z);}
	inline static void vertex3(const GLfloat *v) {glVertex3fv(v);}
	inline static void vertex(GLint x, GLint y, GLint z) {glVertex3i(x, y, z);}
	inline static void vertex3(const GLint *v) {glVertex3iv(v);}
	inline static void vertex(GLshort x, GLshort y, GLshort z) {glVertex3s(x, y, z);}
	inline static void vertex3(const GLshort *v) {glVertex3sv(v);}

	inline static void vertex(GLdouble x, GLdouble y, GLdouble z, GLdouble w) {glVertex4d(x, y, z, w);}
	inline static void vertex4(const GLdouble *v) {glVertex4dv(v);}
	inline static void vertex(GLfloat x, GLfloat y, GLfloat z, GLfloat w) {glVertex4f(x, y, z, w);}
	inline static void vertex4(const GLfloat *v) {glVertex4fv(v);}
	inline static void vertex(GLint x, GLint y, GLint z, GLint w) {glVertex4i(x, y, z, w);}
	inline static void vertex4(const GLint *v) {glVertex4iv(v);}
	inline static void vertex(GLshort x, GLshort y, GLshort z, GLshort w) {glVertex4s(x, y, z, w);}
	inline static void vertex4(const GLshort *v) {glVertex4sv (v);}

	inline static void vertexPointer(GLint size, GLenum type, GLsizei stride, const GLvoid *pointer) {glVertexPointer(size, type, stride, pointer);}
	inline static void vertexPointer(GLint size, GLsizei stride, const GLdouble *pointer) {glVertexPointer(size, GL_DOUBLE, sizeof(GLdouble)*stride, pointer);}
	inline static void vertexPointer(GLint size, GLsizei stride, const GLfloat *pointer) {glVertexPointer(size, GL_FLOAT, sizeof(GLfloat)*stride, pointer);}
	inline static void vertexPointer(GLint size, GLsizei stride, const GLint *pointer) {glVertexPointer(size, GL_INT, sizeof(GLint)*stride, pointer);}
	inline static void vertexPointer(GLint size, GLsizei stride, const GLshort *pointer) {glVertexPointer(size, GL_SHORT, sizeof(GLshort)*stride, pointer);}

// Viewport...

	inline static void viewport(GLint x, GLint y, GLsizei width, GLsizei height) {glViewport(x, y, width, height);}
};

#endif // _RICPP_GLCPP_OPENGL_H
