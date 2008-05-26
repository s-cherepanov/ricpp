// RICPP - RenderMan(R) Interface CPP Language Binding////     RenderMan(R) is a registered trademark of Pixar// The RenderMan(R) Interface Procedures and Protocol are://         Copyright 1988, 1989, 2000, 2005 Pixar//                 All rights Reservered//// Copyright (c) of RiCPP 2007, Andreas Pidde// Contact: andreas@pidde.de//// This library is free software; you can redistribute it and/or// modify it under the terms of the GNU General Public// License as published by the Free Software Foundation; either// version 2 of the License, or (at your option) any later version.//  // This library is distributed in the hope that it will be useful,// but WITHOUT ANY WARRANTY; without even the implied warranty of// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU// General Public License for more details.//// You should have received a copy of the GNU General Public// License along with this library; if not, write to the Free Software// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA/** @file glrenderer.cpp *  @author Andreas Pidde (andreas@pidde.de) *  @brief Implementation of a context to do some draft OpenGL rendering */#include "ricpp/glrenderer/glrenderer.h"#ifndef _RICPP_RICONTEXT_RIMACRO_H#include "ricpp/ricontext/rimacro.h"#endif // _RICPP_RICONTEXT_RIMACRO_H#if defined(WIN32)#include <GL/gl.h>#else#if defined(__APPLE__)#include <OpenGL/gl.h>#include <OpenGL/OpenGL.h>#else#include <GL/gl.h>#endif#endif#include <iostream>using namespace RiCPP;#ifdef _DEBUG// #define _TRACE// #define _PRINTPOLY// #define _SHOWFRAMES// #define _SHOWBACKGROUND// #define _SHOWPOLYLINES#endif// -----------------------------------------------------------------------------const char *CGLRendererCreator::myName() { return GLRENDERERCREATOR_NAME; }const char *CGLRendererCreator::myType() { return CContextCreator::myType(); }unsigned long CGLRendererCreator::myMajorVersion() { return CContextCreator::myMajorVersion(); }unsigned long CGLRendererCreator::myMinorVersion() { return 1; }unsigned long CGLRendererCreator::myRevision() { return 1; }const char *CGLRenderer::myName() { return GLRENDERER_NAME; }const char *CGLRenderer::myType() { return IRiContext::myType(); }unsigned long CGLRenderer::myMajorVersion() { return IRiContext::myMajorVersion(); }unsigned long CGLRenderer::myMinorVersion() { return 1; }unsigned long CGLRenderer::myRevision() { return 1; }RtToken CGLRenderer::myRendererType() { return RI_DRAFT; }CGLRenderer::CGLRenderer() : m_validGL(false){}CGLRenderer::~CGLRenderer(){}void CGLRenderer::defaultDeclarations(){	CBaseRenderer::defaultDeclarations();}void CGLRenderer::initGLContext(){#if defined(__APPLE__)	{		CGLContextObj ctx = CGLGetCurrentContext();		if ( ctx != 0 ) {#           ifdef _TRACE				std::cout << "# GL Context is available" << std::endl;#           endif // _TRACE			m_validGL = true;		} else {#           ifdef _TRACE				std::cout << "# GL Context is not available" << std::endl;#           endif // _TRACE			m_validGL = false;		}	}#endif	// If an OpenGL context is valid, take this one	// else open a GL window with the current display and frame settings	// sets m_validGL}void CGLRenderer::initHider(){	// Sets the hider type (hidden, painter, null)}void CGLRenderer::initViewing(){	// Inits the viewing matrices	RtInt xres, yres;	RtFloat pa;	renderState()->options().getFormat(xres, yres, pa);	RtFloat pixelAspect = pa;	RtInt offsetY = 0;	RtInt originX = 0, originY = 0;	const CDisplayDescr *o = renderState()->options().primaryDisplay();	if ( o ) {#		ifdef _TRACE			std::cout << "# Primary display found." << std::endl;#		endif		if ( o->height() > 0 ) {			offsetY = o->height();#		    ifdef _TRACE				std::cout << "# offsetY: " << offsetY << std::endl;#		    endif		}		o->getOrigin(originX, originY);	}	glEnable(GL_CULL_FACE);	glCullFace(GL_FRONT);	// orientation has to be left handed for RenderMan -> GL Front is culled		glDepthFunc(GL_LEQUAL);	glEnable(GL_DEPTH_TEST);		glEnable(GL_ALPHA_TEST);	glAlphaFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);		glEnable(GL_BLEND);	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);		glLineWidth(1.0);	glPointSize(1.0);	RtInt xmin=0, xmax=xres, ymin=0, ymax=yres;	renderState()->options().getCropWindow(xmin, xmax, ymin, ymax);	    glViewport(originX+xmin, (offsetY-ymax)-originY, xmax-xmin, ymax-ymin);    glScissor(originX+xmin, (offsetY-ymax)-originY, xmax-xmin, ymax-ymin);	glEnable(GL_SCISSOR_TEST);		glClearDepth(1.0);	glClearColor(1.0, 1.0, 1.0, 0);	glClear(GL_DEPTH_BUFFER_BIT|GL_COLOR_BUFFER_BIT);    glMatrixMode(GL_PROJECTION);	    glLoadIdentity();#   ifdef _TRACE	{		std::cout		<< "# screen left " << 		renderState()->options().screenWindowLeft()		<< " right " << 		renderState()->options().screenWindowRight()		<< " bottom " << 		renderState()->options().screenWindowBottom()		<< " top " << 		renderState()->options().screenWindowTop()		<< std::endl;				RtFloat left, right, bottom, top;		renderState()->getProjectedScreenWindow(left, right, bottom, top);		std::cout		<< "# projected screen left " << 		left		<< " right " << 		right		<< " bottom " << 		bottom		<< " top " << 		top		<< std::endl;	}#   endif		// RiCPP Raster to screen raster	glOrtho(xmin,			xmax,			ymax,			ymin,			0, -1.0);#ifdef _SHOWBACKGROUND		glColor4f(0.0f, 0.0f, 0.0f, 1.0f);	glBegin(GL_QUADS);	glVertex3f(xmin, ymin, 0);	glVertex3f(xmax, ymin, 0);	glVertex3f(xmax, ymax, 0);	glVertex3f(xmin, ymax, 0);	glEnd();	glClear(GL_DEPTH_BUFFER_BIT);#endif	#ifdef _SHOWFRAMES		glColor4f(0.0f, 0.0f, 0.0f, 1.0f);	glBegin(GL_LINE_LOOP);	glVertex3f(0.5, 0.5, 0);	glVertex3f(xres-0.5, 0.5, 0);	glVertex3f(xres-0.5, yres-0.5, 0);	glVertex3f(0.5, yres-0.5, 0);	glEnd();#endif#ifdef _SHOWFRAMES		glColor4f(1.0f, 0.0f, 0.0f, 1.0f);	glBegin(GL_LINE_LOOP);	glVertex3f(xmin+0.5, ymin+0.5, 0);	glVertex3f(xmax-0.5, ymin+0.5, 0);	glVertex3f(xmax-0.5, ymax-0.5, 0);	glVertex3f(xmin+0.5, ymax-0.5, 0);	glEnd();#endif	// camera to raster transformation pipeline	CTransformation *NDCToRaster = renderState()->NDCToRaster();	glMultMatrixf(NDCToRaster->getCTM().getFloats());	#ifdef _SHOWFRAMES	glColor4f(0.5f, 0.5f, 0.5f, 1.0f);	glEnable(GL_LINE_STIPPLE);	glLineStipple(1, 0xAAAA);	glBegin(GL_LINE_LOOP);	glVertex3f(0, 0, 0);	glVertex3f(0, 1, 0);	glVertex3f(1, 1, 0);	glVertex3f(1, 0, 0);	glEnd();	glLineStipple(1, 0xFFFF);	glDisable(GL_LINE_STIPPLE);#endif	CTransformation *screenToNDC = renderState()->screenToNDC();	glMultMatrixf(screenToNDC->getCTM().getFloats());		CTransformation *cameraToScreen = renderState()->cameraToScreen();	glMultMatrixf(cameraToScreen->getCTM().getFloats());		glMatrixMode(GL_MODELVIEW);			// World to camera - shading in camera space	// CTransformation *worldToCamera = renderState()->worldToCamera();	// glMultMatrixf(worldToCamera->getCTM().getFloats());	}void CGLRenderer::initLights(){	// Inits the global light sources or the default light source - the default lightsource will be removed, if a light source is defined}RtVoid CGLRenderer::doBegin(CRiBegin &obj, RtString name, const CParameterList &params){}RtVoid CGLRenderer::doEnd(CRiEnd &obj){	if ( !valid() )		return;	// if own window: Wait for window to get closed and destroy opengl context}RtVoid CGLRenderer::doWorldBegin(CRiWorldBegin &obj){	initGLContext();	if ( !valid() )		return;	initHider();	initViewing();	initLights();}RtVoid CGLRenderer::doWorldEnd(CRiWorldEnd &obj){	if ( !valid() )		return;	// Render postponed surfaces	// Flush GL	glDisable(GL_SCISSOR_TEST);	glFinish();}RtVoid CGLRenderer::doGeneralPolygon(CRiGeneralPolygon &obj, RtInt nloops, RtInt nverts[], const CParameterList &params){	if ( !valid() )		return;		// Just a q&d test	const CTriangulatedPolygon *tp = obj.triangulate(triangulationStrategy());	if ( !tp )		return;	CParameter *par = params.get(RI_P, BASICTYPE_FLOAT);	if ( !par )		return;		const std::vector<RtFloat> &p = par->floats();	const std::vector<unsigned long> &triangles = tp->triangles();	std::vector<unsigned long>::const_iterator iter = triangles.begin();		glColor4f(0.3f, 1.0f, 0.3f, 1.0f);#ifdef _PRINTPOLY	std::cout << "# Print Polygon " << p.size() << std::endl;#endif	RtFloat x1, y1, z1;	RtFloat x2, y2, z2;	RtFloat x3, y3, z3;	glBegin(GL_TRIANGLES);	while ( iter != triangles.end() ) {		assert(p.size() >= (*iter)*3+2);		x1 = p[(*iter)*3];		y1 = p[(*iter)*3+1];		z1 = p[(*iter)*3+2];#ifdef _PRINTPOLY		std::cout << "# " << (*iter) << " x " << x1 << " " << y1 << " " << z1 << std::endl;#endif		++iter;		assert(p.size() >= (*iter)*3+3);				x2 = p[(*iter)*3];		y2 = p[(*iter)*3+1];		z2 = p[(*iter)*3+2];#ifdef _PRINTPOLY		std::cout << "# " << (*iter) << " x " << x2 << " " << y2 << " " << z2 << std::endl;#endif		++iter;		assert(p.size() >= (*iter)*3+3);		x3 = p[(*iter)*3];		y3 = p[(*iter)*3+1];		z3 = p[(*iter)*3+2];#ifdef _PRINTPOLY		std::cout << "# " << (*iter) << " x " << x3 << " " << y3 << " " << z3 << std::endl;#endif		++iter;		if ( renderState()->attributes().primitiveOrientation() == RI_LH ) {			glVertex3f(x1, y1, z1);			glVertex3f(x2, y2, z2);			glVertex3f(x3, y3, z3);		} else {			glVertex3f(x3, y3, z3);			glVertex3f(x2, y2, z2);			glVertex3f(x1, y1, z1);		}		#ifdef _PRINTPOLY		std::cout << "# ----" << std::endl;#endif	}	glEnd();		#ifdef _SHOWPOLYLINES	RtFloat x, y, z;	glColor4f(0.0f, 0.0f, 0.0f, 1.0f);	iter = triangles.begin();	while ( iter != triangles.end() ) {		glBegin(GL_LINE_LOOP);		x = p[(*iter)*3];		y = p[(*iter)*3+1];		z = p[(*iter)*3+2]-.01;		glVertex3f(x, y, z);		++iter;						x = p[(*iter)*3];		y = p[(*iter)*3+1];		z = p[(*iter)*3+2]-.01;		glVertex3f(x, y, z);		++iter;				x = p[(*iter)*3];		y = p[(*iter)*3+1];		z = p[(*iter)*3+2]-.01;		glVertex3f(x, y, z);		++iter;		glEnd();	}#endif		// Build surfaces (mind: motion, depth of field, LOD, opacity)	// Manage Surfaces:	//     shaders	//     if opaque: hide surface and remove surface	//     else if transparent: postpone surface (order by z)}