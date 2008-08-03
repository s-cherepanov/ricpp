// RICPP - RenderMan(R) Interface CPP Language Binding////     RenderMan(R) is a registered trademark of Pixar// The RenderMan(R) Interface Procedures and Protocol are://         Copyright 1988, 1989, 2000, 2005 Pixar//                 All rights Reservered//// Copyright (c) of RiCPP 2007, Andreas Pidde// Contact: andreas@pidde.de//// This library is free software; you can redistribute it and/or// modify it under the terms of the GNU General Public// License as published by the Free Software Foundation; either// version 2 of the License, or (at your option) any later version.//  // This library is distributed in the hope that it will be useful,// but WITHOUT ANY WARRANTY; without even the implied warranty of// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU// General Public License for more details.//// You should have received a copy of the GNU General Public// License along with this library; if not, write to the Free Software// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA/** @file glrenderer.cpp *  @author Andreas Pidde (andreas@pidde.de) *  @brief Implementation of a context to do some draft OpenGL rendering */#include "ricpp/glrenderer/glrenderer.h"#ifndef _RICPP_RICONTEXT_RIMACRO_H#include "ricpp/ricontext/rimacro.h"#endif // _RICPP_RICONTEXT_RIMACRO_H#include <iostream>using namespace RiCPP;#ifdef _DEBUG// #define _TRACE// #define _PRINTPOLY// #define _SHOWFRAMES// #define _SHOWBACKGROUND// #define _SHOWPOLYLINES#define _DRAWNORMALS true#else#define _DRAWNORMALS false#endif// -----------------------------------------------------------------------------const char *CGLRendererCreator::myName() { return GLRENDERERCREATOR_NAME; }const char *CGLRendererCreator::myType() { return CContextCreator::myType(); }unsigned long CGLRendererCreator::myMajorVersion() { return CContextCreator::myMajorVersion(); }unsigned long CGLRendererCreator::myMinorVersion() { return 1; }unsigned long CGLRendererCreator::myRevision() { return 1; }const char *CGLRenderer::myName() { return GLRENDERER_NAME; }const char *CGLRenderer::myType() { return IRiContext::myType(); }unsigned long CGLRenderer::myMajorVersion() { return IRiContext::myMajorVersion(); }unsigned long CGLRenderer::myMinorVersion() { return 1; }unsigned long CGLRenderer::myRevision() { return 1; }RtToken CGLRenderer::myRendererType() { return RI_DRAFT; }CGLRenderer::CGLRenderer() : m_validGL(false), m_drawNormals(_DRAWNORMALS){}CGLRenderer::~CGLRenderer(){}void CGLRenderer::defaultDeclarations(){	TypeParent::defaultDeclarations();}void CGLRenderer::hide(const CFace &f){	const TemplPrimVar<RtFloat> *pp = f.floats(RI_P);	if ( !pp )		return;	std::vector<RtFloat> p = pp->value();	toCamera().transformPoints(p.size()/3, (RtPoint *)&p[0]);		glEnableClientState(GL_VERTEX_ARRAY);	glVertexPointer(3, GL_FLOAT, 0, &p[0]);		const TemplPrimVar<RtFloat> *np = f.floats(RI_N);	std::vector<RtFloat> n;	if ( np ) {		if ( np->value().size() == p.size() ) {			// n = np->value();			// toCamera().transformNormals(n.size()/3, (RtPoint *)&n[0]);						n = pp->value();			for ( unsigned int i = 0; i < n.size()-2; i+=3 ) {				n[i]   += np->value()[i];				n[i+1] += np->value()[i+1];				n[i+2] += np->value()[i+2];			}			toCamera().transformPoints(n.size()/3, (RtPoint *)&n[0]);						for ( unsigned int i = 0; i < n.size()-2; i+=3 ) {				n[i]   -= p[i];				n[i+1] -= p[i+1];				n[i+2] -= p[i+2];				normalize(n[i], n[i+1], n[i+2]);			}						glEnableClientState(GL_NORMAL_ARRAY);			glNormalPointer(GL_FLOAT, 0, &n[0]);			glEnable(GL_LIGHTING);			glEnable(GL_LIGHT0);			glEnable(GL_COLOR_MATERIAL);		} else {			np = 0;		}	}		IndexType sizeCnt = 0;	switch ( f.faceType() ) {		case FACETYPE_TRIANGLES: {			for ( std::vector<IndexType>::const_iterator siter = f.sizes().begin(); siter != f.sizes().end(); siter++ ) {				glDrawElements(GL_TRIANGLES, (*siter), GL_UNSIGNED_INT, &f.indices()[sizeCnt]);				sizeCnt += (*siter);			}		}		break;					case FACETYPE_TRIANGLESTRIPS: {			for ( std::vector<IndexType>::const_iterator siter = f.sizes().begin(); siter != f.sizes().end(); siter++ ) {				glDrawElements(GL_TRIANGLE_STRIP, (*siter), GL_UNSIGNED_INT, &f.indices()[sizeCnt]);				sizeCnt += (*siter);			}		}		break;					default:		break;	}		glDisableClientState(GL_VERTEX_ARRAY);	if ( np ) {		glDisableClientState(GL_NORMAL_ARRAY);		glDisable(GL_LIGHTING);		glDisable(GL_LIGHT0);	}		if ( np != 0 && m_drawNormals ) {		RtFloat sn=.075;		glColor4f(0, 0, 0, 1.0);		glBegin(GL_LINES);		for ( unsigned int i = 0; i < p.size()-2; i+=3 ) {			glVertex3f(p[i], p[i+1], p[i+2]);			glVertex3f(p[i]+sn*n[i], p[i+1]+sn*n[i+1], p[i+2]+sn*n[i+2]);		}		glEnd();		glColor4f(1.0, 0, 0, 1.0);		glPointSize(3.0);		glBegin(GL_POINTS);		for ( unsigned int i = 0; i < p.size()-2; i+=3 ) {			glVertex3f(p[i]+sn*n[i], p[i+1]+sn*n[i+1], p[i+2]+sn*n[i+2]);		}		glEnd();	}}void CGLRenderer::hide(const CSurface *s){	setCullFace();	setColor();	glDisableClientState(GL_EDGE_FLAG_ARRAY);	glDisableClientState(GL_INDEX_ARRAY);	glDisableClientState(GL_TEXTURE_COORD_ARRAY);	glDisableClientState(GL_COLOR_ARRAY);	glDisableClientState(GL_NORMAL_ARRAY);	glDisableClientState(GL_TEXTURE_COORD_ARRAY);	TypeParent::hide(s);}void CGLRenderer::initGLContext(){	// If an OpenGL context is valid, take this one	// else (todo) open a GL window with the current display and frame settings	// sets m_validGL	#if defined(__APPLE__)	{		CGLContextObj ctx = CGLGetCurrentContext();		if ( ctx != 0 ) {#           ifdef _TRACE				std::cout << "# GL Context is available" << std::endl;#           endif // _TRACE			m_validGL = true;		} else {#           ifdef _TRACE				std::cout << "# GL Context is not available" << std::endl;#           endif // _TRACE/** @todo Open a GL window and add window management. */			m_validGL = false;		}	}#endif}void CGLRenderer::toggleCullFace(){	if ( m_cullFace == GL_FRONT ) m_cullFace = GL_BACK;	else if ( m_cullFace == GL_BACK ) m_cullFace = GL_FRONT;	else return;	glCullFace(m_cullFace);}void CGLRenderer::setColor(){	RtFloat alpha = opacityToAlpha(attributes().opacity());	RtFloat colorRGB[3];	getColor3f(attributes().color(), colorRGB, renderState()->options().colorDescr(), renderState()->options().gain(), renderState()->options().gamma());	glColor4f(colorRGB[0], colorRGB[1], colorRGB[2], alpha);	GLfloat mat_diffuse[4] = {colorRGB[0], colorRGB[1], colorRGB[2], alpha};	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mat_diffuse);}void CGLRenderer::setCullFace(){	if ( attributes().sides() == 2 ) {		if ( m_cullFace != 0 ) {			glDisable(GL_CULL_FACE);			m_cullFace = 0;		}		// glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);		return;	}	// glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_FALSE);	GLenum front = GL_FRONT;	GLenum back = GL_BACK;	if ( renderState()->viewingOrientation() == RI_RH ) {		front = GL_BACK;		back = GL_FRONT;	}	// sides = 1	if ( attributes().primitiveOrientation() == RI_LH && m_cullFace != back ) {		m_cullFace = back;		glEnable(GL_CULL_FACE);		glCullFace(m_cullFace);		return;	}	if ( attributes().primitiveOrientation() == RI_RH && m_cullFace != front ) {		m_cullFace = front;		glEnable(GL_CULL_FACE);		glCullFace(m_cullFace);		return;	}}		void CGLRenderer::initHider(){	// Sets the hider type (hidden, painter, null)}void CGLRenderer::initViewing(){	// Inits the viewing matrices	RtInt xres, yres;	RtFloat pa;	renderState()->options().getFormat(xres, yres, pa);	RtInt offsetY = 0;	RtInt originX = 0, originY = 0;	const CDisplayDescr *o = renderState()->options().primaryDisplay();	if ( o ) {#		ifdef _TRACE			std::cout << "# Primary display found." << std::endl;#		endif		if ( o->yres() > 0 ) {			offsetY = o->yres();#		    ifdef _TRACE				std::cout << "# offsetY: " << offsetY << std::endl;#		    endif		}		o->getOrigin(originX, originY);	}	glEnable(GL_CULL_FACE);	m_cullFace = GL_FRONT;	glCullFace(GL_FRONT);	// orientation has to be left handed for RenderMan -> GL Front is culled		glDepthFunc(GL_LEQUAL);	glEnable(GL_DEPTH_TEST);	glDisable(GL_ALPHA_TEST);	glDisable(GL_BLEND);			glLineWidth(1.0);	glPointSize(1.0);	RtInt xmin=0, xmax=xres, ymin=0, ymax=yres;	renderState()->options().getCropWindow(xmin, xmax, ymin, ymax);	    glViewport(originX+xmin, (offsetY-ymax)-originY, xmax-xmin, ymax-ymin);    glScissor(originX+xmin, (offsetY-ymax)-originY, xmax-xmin, ymax-ymin);	glEnable(GL_SCISSOR_TEST);		glClearDepth(1.0);	glClearColor(1, 1, 1, 1);	glClear(GL_DEPTH_BUFFER_BIT|GL_COLOR_BUFFER_BIT);    glMatrixMode(GL_PROJECTION);	    glLoadIdentity();#   ifdef _TRACE	{		std::cout		<< "# screen left " << 		renderState()->options().screenWindowLeft()		<< " right " << 		renderState()->options().screenWindowRight()		<< " bottom " << 		renderState()->options().screenWindowBottom()		<< " top " << 		renderState()->options().screenWindowTop()		<< std::endl;				RtFloat left, right, bottom, top;		renderState()->getProjectedScreenWindow(left, right, bottom, top);		std::cout		<< "# projected screen left " << 		left		<< " right " << 		right		<< " bottom " << 		bottom		<< " top " << 		top		<< std::endl;	}#   endif		RtToken projectionName = renderState()->options().projectionName();		if ( projectionName != RI_NULL ) {		// RiCPP Raster to screen raster		glOrtho(xmin,				xmax,				ymax,				ymin,				0, -1.0);	} else {		// RiCPP Raster to screen raster, clipping		RtPoint clip[2] = {{0, 0, renderState()->options().hither()}, {0, 0, renderState()->options().yon()}};		renderState()->cameraToScreen()->getCTM().transformPoints(2, clip);		glOrtho(xmin,				xmax,				ymax,				ymin,				-1.0 * clip[0][2],				-1.0 * clip[1][2]);	}#ifdef _SHOWBACKGROUND		glColor4f(0.0f, 0.0f, 0.0f, 1.0f);	glBegin(GL_QUADS);	glVertex3f(xmin, ymin, 0);	glVertex3f(xmax, ymin, 0);	glVertex3f(xmax, ymax, 0);	glVertex3f(xmin, ymax, 0);	glEnd();	glClear(GL_DEPTH_BUFFER_BIT);#endif	#ifdef _SHOWFRAMES		glColor4f(0.0f, 0.0f, 0.0f, 1.0f);	glBegin(GL_LINE_LOOP);	glVertex3f(0.5, 0.5, 0);	glVertex3f(xres-0.5, 0.5, 0);	glVertex3f(xres-0.5, yres-0.5, 0);	glVertex3f(0.5, yres-0.5, 0);	glEnd();#endif#ifdef _SHOWFRAMES		glColor4f(1.0f, 0.0f, 0.0f, 1.0f);	glBegin(GL_LINE_LOOP);	glVertex3f(xmin+0.5, ymin+0.5, 0);	glVertex3f(xmax-0.5, ymin+0.5, 0);	glVertex3f(xmax-0.5, ymax-0.5, 0);	glVertex3f(xmin+0.5, ymax-0.5, 0);	glEnd();#endif	// camera to raster transformation pipeline	CTransformation *NDCToRaster = renderState()->NDCToRaster();	glMultMatrixf(NDCToRaster->getCTM().getFloats());	#ifdef _SHOWFRAMES	glColor4f(0.5f, 0.5f, 0.5f, 1.0f);	glEnable(GL_LINE_STIPPLE);	glLineStipple(1, 0xAAAA);	glBegin(GL_LINE_LOOP);	glVertex3f(0, 0, 0);	glVertex3f(0, 1, 0);	glVertex3f(1, 1, 0);	glVertex3f(1, 0, 0);	glEnd();	glLineStipple(1, 0xFFFF);	glDisable(GL_LINE_STIPPLE);#endif	CTransformation *screenToNDC = renderState()->screenToNDC();	glMultMatrixf(screenToNDC->getCTM().getFloats());		CTransformation *cameraToScreen = renderState()->cameraToScreen();	glMultMatrixf(cameraToScreen->getCTM().getFloats());		glMatrixMode(GL_MODELVIEW);	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_FALSE);}void CGLRenderer::initLights(){	// Inits the global light sources or the default light source - the default lightsource will be removed, if a light source is defined	GLfloat mat_diffuse[] = { 1, 1, 1, 1.0 };	GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };	GLfloat mat_shininess[] = { 50.0 };	GLfloat light_ambient[] = { 0.1, 0.1, 0.1, 1.0 };	GLfloat light_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };	GLfloat light_specular[] = { 1.0, 1.0, 1.0, 1.0 };	GLfloat light_position[] = { -1.0, 1.0, -1.0, 0.0};		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mat_diffuse);	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_specular);	glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, mat_shininess);	glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);	glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);	glLightfv(GL_LIGHT0, GL_POSITION, light_position);	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, light_ambient);}RtVoid CGLRenderer::doBegin(CRiBegin &obj, RtString name, const CParameterList &params){}RtVoid CGLRenderer::doEnd(CRiEnd &obj){	if ( !valid() )		return;	// if own window: Wait for window to get closed and destroy opengl context}RtVoid CGLRenderer::doWorldBegin(CRiWorldBegin &obj){	initGLContext();	if ( !valid() )		return;	initHider();	initViewing();	initLights();}RtVoid CGLRenderer::doWorldEnd(CRiWorldEnd &obj){	if ( !valid() )		return;		// Render postponed surfaces	// glEnable(GL_ALPHA_TEST);	// glAlphaFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);		glEnable(GL_BLEND);	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);	glDepthMask(GL_FALSE);	replayDelayed();		glDisable(GL_ALPHA_TEST);	glDisable(GL_BLEND);	glDepthMask(GL_TRUE);	// Flush GL	glDisable(GL_SCISSOR_TEST);	glFinish();}RtVoid CGLRenderer::doPolygon(CRiPolygon &obj, RtInt nvertices, const CParameterList &params){	if ( nvertices == 0 || !valid() || delayRequest(obj) )		return;	triangulate(obj);}RtVoid CGLRenderer::doGeneralPolygon(CRiGeneralPolygon &obj, RtInt nloops, RtInt nverts[], const CParameterList &params){	if ( nloops == 0 || !valid() || delayRequest(obj) )		return;	triangulate(obj);}RtVoid CGLRenderer::doPointsPolygons(CRiPointsPolygons &obj, RtInt npolys, RtInt nverts[], RtInt verts[], const CParameterList &params){	if ( npolys == 0 || !valid() || delayRequest(obj) )		return;		triangulate(obj);}RtVoid CGLRenderer::doPointsGeneralPolygons(CRiPointsGeneralPolygons &obj, RtInt npolys, RtInt nloops[], RtInt nverts[], RtInt verts[], const CParameterList &params){	if ( npolys == 0 || !valid() || delayRequest(obj) )		return;	triangulate(obj);}RtVoid CGLRenderer::doCone(CRiCone &obj, RtFloat height, RtFloat radius, RtFloat thetamax, const CParameterList &params){	if ( !valid() || delayRequest(obj) || nearlyZero(radius) || nearlyZero(thetamax) )		return;		triangulate(obj);}RtVoid CGLRenderer::doCylinder(CRiCylinder &obj, RtFloat radius, RtFloat zmin, RtFloat zmax, RtFloat thetamax, const CParameterList &params){	if ( !valid() || delayRequest(obj) || nearlyZero(radius) || nearlyZero(thetamax) || nearlyZero(zmax-zmin) )		return;		triangulate(obj);}RtVoid CGLRenderer::doParaboloid(CRiParaboloid &obj, RtFloat rmax, RtFloat zmin, RtFloat zmax, RtFloat thetamax, const CParameterList &params){	if ( !valid() || delayRequest(obj)|| nearlyZero(rmax) || nearlyZero(thetamax) || nearlyZero(zmax-zmin) )		return;		triangulate(obj);}RtVoid CGLRenderer::doSphere(CRiSphere &obj, RtFloat radius, RtFloat zmin, RtFloat zmax, RtFloat thetamax, const CParameterList &params){	if ( !valid() || delayRequest(obj) || nearlyZero(radius) || nearlyZero(thetamax) || nearlyZero(zmax-zmin) )		return;		triangulate(obj);}