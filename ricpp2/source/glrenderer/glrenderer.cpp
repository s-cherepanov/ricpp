// RICPP - RenderMan(R) Interface CPP Language Binding
//
//     RenderMan(R) is a registered trademark of Pixar
// The RenderMan(R) Interface Procedures and Protocol are:
//         Copyright 1988, 1989, 2000, 2005 Pixar
//                 All rights Reservered
//
// Copyright (c) of RiCPP 2007, Andreas Pidde
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

/** @file glrenderer.cpp
 *  @author Andreas Pidde (andreas@pidde.de)
 *  @brief Implementation of a context to do some draft OpenGL rendering
 */

#include "ricpp/glrenderer/glrenderer.h"

#ifndef _RICPP_RICONTEXT_RIMACRO_H
#include "ricpp/ricontext/rimacro.h"
#endif // _RICPP_RICONTEXT_RIMACRO_H

#include <iostream>

using namespace RiCPP;

#ifdef _DEBUG
// #define _TRACE
// #define _PRINTPOLY
// #define _SHOWFRAMES
// #define _SHOWBACKGROUND
// #define _SHOWPOLYLINES
#endif

#define _OPENGL_TRANSFORM

static const bool _DRAWNORMALS = false;
static const bool _USESTRIPS = false;

// -----------------------------------------------------------------------------

const char *CGLRendererCreator::myName() { return GLRENDERERCREATOR_NAME; }
const char *CGLRendererCreator::myType() { return CContextCreator::myType(); }
unsigned long CGLRendererCreator::myMajorVersion() { return CContextCreator::myMajorVersion(); }
unsigned long CGLRendererCreator::myMinorVersion() { return 1; }
unsigned long CGLRendererCreator::myRevision() { return 1; }

const char *CGLRenderer::myName() { return GLRENDERER_NAME; }
const char *CGLRenderer::myType() { return IRiContext::myType(); }
unsigned long CGLRenderer::myMajorVersion() { return IRiContext::myMajorVersion(); }
unsigned long CGLRenderer::myMinorVersion() { return 1; }
unsigned long CGLRenderer::myRevision() { return 1; }
RtToken CGLRenderer::myRendererType() { return RI_DRAFT; }


CGLRenderer::CGLRenderer() : m_validGL(false), m_drawNormals(_DRAWNORMALS)
{
	useStrips(_USESTRIPS);
	RI_GLRENDERER = RI_NULL;	
	RI_SCREEN = RI_NULL;
	RI_QUAL_SCREEN = RI_NULL;
	RI_DRAW_NORMALS = RI_NULL;
	RI_QUAL_DRAW_NORMALS = RI_NULL;
}

CGLRenderer::~CGLRenderer()
{
}

void CGLRenderer::defaultDeclarations()
{
	TypeParent::defaultDeclarations();
	
	RI_GLRENDERER = renderState()->tokFindCreate("glrenderer");

	RI_SCREEN = renderState()->tokFindCreate("screen");
	RI_QUAL_SCREEN = renderState()->declare("Control:glrenderer:screen",  "string",  true);	

	RI_DRAW_NORMALS = renderState()->tokFindCreate("draw-normals");
	RI_QUAL_DRAW_NORMALS = renderState()->declare("Attribute:glrenderer:draw-normals",  "integer",  true);	
}

void CGLRenderer::clearScreen()
{
	initGLContext(); // Can also be called twice (first time at frameBegin)
	if ( !valid() )
		return;

	/** @todo Option for frame color (world block should also have a controlable background, white at the moment)
	 */
	glDisable(GL_SCISSOR_TEST);
	glClearDepth(1.0);
	glClearColor(0.5, 0.5, 1.0, 1);
	glClear(GL_DEPTH_BUFFER_BIT|GL_COLOR_BUFFER_BIT);
	// glFinish();
}

void CGLRenderer::finishScreen()
{
	if ( !valid() )
		return;

	glFinish();
#if defined(__APPLE__)
	CGLContextObj ctx = CGLGetCurrentContext();
	if ( ctx != 0 ) {
		CGLFlushDrawable(ctx);
	}
#endif
}


void CGLRenderer::drawNormals(const std::vector<RtFloat> &p, const std::vector<RtFloat> &n)
{
	if ( p.empty() || n.size() < 3 ) {
		return;
	}
	
	if ( replayMode()  ) 
		glDepthMask(GL_TRUE);
	
	bool perFace = n.size() == 3;
	
	// line
	const RtFloat sn = (RtFloat).075; // scale normals
	RtFloat fact = 0;
	if ( perFace ) {
		fact = sn / vlen(n[0], n[1], n[2]);
	}
	
	
	glColor4f(0, 0, 0, 1.0);
	glBegin(GL_LINES);
	for ( unsigned int i = 0; i < p.size()-2; i+=3 ) {
		glVertex3f(p[i], p[i+1], p[i+2]);
		if ( perFace ) {
			glVertex3f(p[i]+fact*n[0], p[i+1]+fact*n[1], p[i+2]+fact*n[2]);
		} else {
			fact = sn / vlen(n[i], n[i+1], n[i+2]);
			glVertex3f(p[i]+fact*n[i], p[i+1]+fact*n[i+1], p[i+2]+fact*n[i+2]);
		}
	}
	glEnd();
	
	// tip
	glColor4f(1.0, 0, 0, 1.0);
	glPointSize(3.0); // Point size of the tip
	glBegin(GL_POINTS);
	for ( unsigned int i = 0; i < p.size()-2; i+=3 ) {
		if ( perFace ) {
			glVertex3f(p[i]+fact*n[0], p[i+1]+fact*n[1], p[i+2]+fact*n[2]);
		} else {
			fact = sn / vlen(n[i], n[i+1], n[i+2]);
			glVertex3f(p[i]+fact*n[i], p[i+1]+fact*n[i+1], p[i+2]+fact*n[i+2]);
		}
	}
	glEnd();

	glColor4f(1.0, 1.0, 1.0, 1.0);
	glPointSize(1.0); // Point size of the tip highlight
	glBegin(GL_POINTS);
	for ( unsigned int i = 0; i < p.size()-2; i+=3 ) {
		if ( perFace ) {
			glVertex3f(p[i]+fact*n[0], p[i+1]+fact*n[1], p[i+2]+fact*n[2]);
		} else {
			fact = sn / vlen(n[i], n[i+1], n[i+2]);
			glVertex3f(p[i]+fact*n[i], p[i+1]+fact*n[i+1], p[i+2]+fact*n[i+2]);
		}
	}
	glEnd();
	
	if ( replayMode()  ) 
		glDepthMask(GL_FALSE);
}

void CGLRenderer::hide(const CFace &f)
{
	const TemplPrimVar<RtFloat> *pptr = f.floats(RI_P);
	if ( !pptr )
		return;

#ifdef _OPENGL_TRANSFORM
	const TemplPrimVar<RtFloat> *nptr = f.floats(RI_N);
		
	const std::vector<RtFloat> *pp = &(pptr->values());
	const std::vector<RtFloat> *np = nptr ? &(nptr->values()) : 0;
#else
	std::vector<RtFloat> p;
	std::vector<RtFloat> n;
	const std::vector<RtFloat> *pp = &p;
	const std::vector<RtFloat> *np = &n;
	
	getPosAndNormals(f, toCamera(), p, n);
	if ( p.empty() )
		return;
#endif
	

	// Draw normals?
	if ( m_drawNormals && np ) {
		drawNormals(*pp, *np);
	}

	// State variables
	setCullFace();
	setColor();

	// vertexArray
	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3, GL_FLOAT, 0, &(*pp)[0]);
	
	// normalArray
	if ( np ) {
		if ( np->size() == np->size() ) {
			glEnableClientState(GL_NORMAL_ARRAY);
			glNormalPointer(GL_FLOAT, 0, &(*np)[0]);
			glEnable(GL_LIGHTING);
			glEnable(GL_LIGHT0);
		} else if ( np->size() == 3 ) {
			glNormal3f((*np)[0], (*np)[1], (*np)[2]);
			glEnable(GL_LIGHTING);
			glEnable(GL_LIGHT0);
		}
	}
	
	// surface opacity
	const TemplPrimVar<RtFloat> *pcSurfOpacity = f.floats(RI_OS);
	std::vector<RtFloat> os;
	if ( pcSurfOpacity ) {
		getAlpha(pcSurfOpacity->declaration().colorDescr(), pcSurfOpacity->values(), os);
	}
	
	// surface color, todo constant & mix alpha
	const TemplPrimVar<RtFloat> *pcSurf = f.floats(RI_CS);
	std::vector<RtFloat> cs;
	if ( pcSurf ) {
		getCs(pcSurf->declaration().colorDescr(), renderState()->options().gain(), renderState()->options().gamma(), pcSurf->values(), cs);
	}

	if ( cs.size() == 3 ) {
		RtFloat alpha = opacityToAlpha(attributes().opacity());
		glColor4f(cs[0], cs[1], cs[2], alpha);		
	} else if ( cs.size() == pp->size() ) {
		glEnableClientState(GL_COLOR_ARRAY);
		glColorPointer(3, GL_FLOAT, 0, &cs[0]);
		if ( replayMode()  ) 
			glDepthMask(GL_TRUE);
	}

	// Draw vertices
	// std::cout << "# *** Size of indices: " << f.indices().size() << std::endl;
	IndexType sizeCnt = 0;
	switch ( f.faceType() ) {
		case FACETYPE_TRIANGLES: {
			for ( std::vector<IndexType>::const_iterator siter = f.sizes().begin(); siter != f.sizes().end(); siter++ ) {
				glDrawElements(GL_TRIANGLES, (*siter), GL_UNSIGNED_INT, &f.indices()[sizeCnt]);
				sizeCnt += (*siter);
			}
		}
		break;
			
		case FACETYPE_TRIANGLESTRIPS: {
			for ( std::vector<IndexType>::const_iterator siter = f.sizes().begin(); siter != f.sizes().end(); siter++ ) {
				glDrawElements(GL_TRIANGLE_STRIP, (*siter), GL_UNSIGNED_INT, &f.indices()[sizeCnt]);
				sizeCnt += (*siter);
			}
		}
		break;
			
		default:
			// std::cout << "# *** Unhandled face type" << std::endl;
		break;
	}
	
	// reset state
	/*
	if ( replayMode()  ) 
		glDepthMask(GL_FALSE);

	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);
	if ( np && (np->size() == pp->size()) ) {
		glDisableClientState(GL_NORMAL_ARRAY);
		glDisable(GL_LIGHTING);
		glDisable(GL_LIGHT0);
	}
	*/
}

void CGLRenderer::hideSurface(const CSurface *s)
{
	if ( !valid() )
		return;
	
	glDisableClientState(GL_EDGE_FLAG_ARRAY);
	glDisableClientState(GL_INDEX_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);

	glDisable(GL_LIGHTING);
	glDisable(GL_LIGHT0);
	
	RtToken type = attributes().geometricRepresentation();

	if ( type == RI_PRIMITIVE || type == RI_SMOOTH ) {
		glShadeModel(GL_SMOOTH);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}  else if ( type == RI_POINTS ) {
		glShadeModel(GL_SMOOTH);
		glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
	} else if ( type == RI_LINES ) {
		glShadeModel(GL_SMOOTH);
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	} else if ( type == RI_CONSTANT ) {
		glShadeModel(GL_FLAT);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	} else if ( !strcmp(type, "constant points") ) {
		glShadeModel(GL_FLAT);
		glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
	} else if ( !strcmp(type, "constant lines") ) {
		glShadeModel(GL_FLAT);
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}

	const CParameter *ctrlDrawNormals = attributes().get(RI_GLRENDERER, RI_DRAW_NORMALS);
	if ( ctrlDrawNormals ) {
		RtInt action;
		if ( ctrlDrawNormals->get(0, action) ) {
			m_drawNormals = action != 0;
		}
	}

#ifdef _OPENGL_TRANSFORM
	glEnable(GL_NORMALIZE);
#endif
	
	setTransformToCamera();
	TypeParent::hideSurface(s);
}


void CGLRenderer::initGLContext()
{
	// If an OpenGL context is valid, take this one
	// else (todo) open a GL window with the current display and frame settings
	// sets m_validGL
	
#if defined(__APPLE__)
	{
		CGLContextObj ctx = CGLGetCurrentContext();
		if ( ctx != 0 ) {
#           ifdef _TRACE
				std::cout << "# GL Context is available" << std::endl;
#           endif // _TRACE
			m_validGL = true;
		} else {
#           ifdef _TRACE
				std::cout << "# GL Context is not available" << std::endl;
#           endif // _TRACE
/** @todo Open a GL window and add window management.
 */
			m_validGL = false;
		}
	}
#else
	m_validGL = true;
#endif
}

void CGLRenderer::setColor()
{
	RtFloat alpha = opacityToAlpha(attributes().opacity());
	RtFloat colorRGB[3];
	getColor3f(renderState()->options().colorDescr(), renderState()->options().gain(), renderState()->options().gamma(), attributes().color(), colorRGB);
	glColor4f(colorRGB[0], colorRGB[1], colorRGB[2], alpha);
}

void CGLRenderer::setTransformToCamera()
{
	glLoadIdentity();
	// I use 3 flips (2 in viewing one in model) because otherwise the camera position is not considered for lighting correctly (opposite direction)
	glScalef(1.0F, 1.0F, -1.0F);	
#ifdef _OPENGL_TRANSFORM
	glMultMatrixf(toCamera().getFloats());
#endif
}

void CGLRenderer::setCullFace()
{
	// Mirror
	if ( frontFaceCW() ) {
		glFrontFace(GL_CCW);
	} else {
		glFrontFace(GL_CW);
	}

	if ( attributes().sides() == 2 ) {
		glDisable(GL_CULL_FACE);
		glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
		return;
	}

	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_FALSE);

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
}		

void CGLRenderer::initHider()
{
	// Sets the hider type (hidden, painter, null)
}

void CGLRenderer::initViewing()
{
	// Inits the viewing matrices
	RtInt xres, yres;
	RtFloat pa;
	renderState()->options().getFormat(xres, yres, pa);

	RtInt offsetY = yres;
	RtInt originX = 0, originY = 0;

	const CParameter *optYRes = renderState()->options().get(RI_GLRENDERER, RI_DISPYRES);
	if ( optYRes ) {
		optYRes->get(0, offsetY);
	}
	
	const CParameter *optOrigin = renderState()->options().get(RI_GLRENDERER, RI_ORIGIN);
	if ( optOrigin ) {
		optOrigin->get(0, originX);
		optOrigin->get(1, originY);
	}

	/*
	const CDisplayDescr *o = renderState()->options().primaryDisplay();
	if ( o ) {
#		ifdef _TRACE
			std::cout << "# Primary display found." << std::endl;
#		endif
		if ( o->yres() > 0 ) {
			offsetY = o->yres();
#		    ifdef _TRACE
				std::cout << "# offsetY: " << offsetY << std::endl;
#		    endif
		}
		o->getOrigin(originX, originY);
	}
	*/

	glDisable(GL_CULL_FACE);
	
	glDepthFunc(GL_LEQUAL);

	glDisable(GL_ALPHA_TEST);
	glDisable(GL_BLEND);

	glEnable(GL_DEPTH_TEST);
		
	glLineWidth(1.0);
	glPointSize(1.0);

	RtInt xmin=0, xmax=xres, ymin=0, ymax=yres;
	renderState()->options().getCropWindow(xmin, xmax, ymin, ymax);
	
    glViewport(originX+xmin, (offsetY-ymax)-originY, xmax-xmin, ymax-ymin);
    glScissor(originX+xmin, (offsetY-ymax)-originY, xmax-xmin, ymax-ymin);

	glEnable(GL_SCISSOR_TEST);
	
	glClearDepth(1.0);
	glClearColor(1, 1, 1, 1);
	glClear(GL_DEPTH_BUFFER_BIT|GL_COLOR_BUFFER_BIT);

    glMatrixMode(GL_PROJECTION);
	glDepthMask(GL_FALSE);
	
    glLoadIdentity();
	
#   ifdef _TRACE
	{
		std::cout
		<< "# screen left " << 
		renderState()->options().screenWindowLeft()
		<< " right " << 
		renderState()->options().screenWindowRight()
		<< " bottom " << 
		renderState()->options().screenWindowBottom()
		<< " top " << 
		renderState()->options().screenWindowTop()
		<< std::endl;
		
		RtFloat left, right, bottom, top;
		renderState()->getProjectedScreenWindow(left, right, bottom, top);

		std::cout
		<< "# projected screen left " << 
		left
		<< " right " << 
		right
		<< " bottom " << 
		bottom
		<< " top " << 
		top
		<< std::endl;
	}
#   endif
	
	RtToken projectionName = renderState()->options().projectionName();
	
	if ( projectionName != RI_NULL ) {
		// RiCPP Raster to screen raster
		glOrtho(xmin,
				xmax,
				ymax,
				ymin,
				0, 1.0);
	} else {
		// RiCPP Raster to screen raster, clipping
		RtPoint clip[2] = {{0, 0, renderState()->options().hither()}, {0, 0, renderState()->options().yon()}};
		renderState()->cameraToScreen()->getCTM().transformPoints(2, clip);
		glOrtho(xmin,
				xmax,
				ymax,
				ymin,
				1.0 * clip[0][2],
				1.0 * clip[1][2]);
	}

#ifdef _SHOWBACKGROUND	
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	glBegin(GL_QUADS);
	glVertex3f((GLfloat)xmin, (GLfloat)ymin, 0.0f);
	glVertex3f((GLfloat)xmax, (GLfloat)ymin, 0.0f);
	glVertex3f((GLfloat)xmax, (GLfloat)ymax, 0.0f);
	glVertex3f((GLfloat)xmin, (GLfloat)ymax, 0.0f);
	glEnd();

	glClear(GL_DEPTH_BUFFER_BIT);
#endif
	
#ifdef _SHOWFRAMES	
	glColor4f(0.0f, 0.0f, 0.0f, 1.0f);
	glBegin(GL_LINE_LOOP);
	glVertex3f(0.5, 0.5, 0);
	glVertex3f(xres-0.5, 0.5, 0);
	glVertex3f(xres-0.5, yres-0.5, 0);
	glVertex3f(0.5, yres-0.5, 0);
	glEnd();
#endif

#ifdef _SHOWFRAMES	
	glColor4f(1.0f, 0.0f, 0.0f, 1.0f);
	glBegin(GL_LINE_LOOP);
	glVertex3f(xmin+0.5, ymin+0.5, 0);
	glVertex3f(xmax-0.5, ymin+0.5, 0);
	glVertex3f(xmax-0.5, ymax-0.5, 0);
	glVertex3f(xmin+0.5, ymax-0.5, 0);
	glEnd();
#endif

	// see setTransformToCamera()
	glScalef(1.0F, 1.0F, -1.0F);	

	// camera to raster transformation pipeline
	const CTransformation *NDCToRaster = renderState()->NDCToRaster();
	glMultMatrixf(NDCToRaster->getCTM().getFloats());	
	
#ifdef _SHOWFRAMES
	glColor4f(0.5f, 0.5f, 0.5f, 1.0f);
	glEnable(GL_LINE_STIPPLE);
	glLineStipple(1, 0xAAAA);
	glBegin(GL_LINE_LOOP);
	glVertex3f(0, 0, 0);
	glVertex3f(0, 1, 0);
	glVertex3f(1, 1, 0);
	glVertex3f(1, 0, 0);
	glEnd();
	glLineStipple(1, 0xFFFF);
	glDisable(GL_LINE_STIPPLE);
#endif

	const CTransformation *screenToNDC = renderState()->screenToNDC();
	glMultMatrixf(screenToNDC->getCTM().getFloats());	

	const CTransformation *cameraToScreen = renderState()->cameraToScreen();
	glMultMatrixf(cameraToScreen->getCTM().getFloats());	

	// see setTransformToCamera()
	glScalef(1.0F, 1.0F, -1.0F);	
	glMatrixMode(GL_MODELVIEW);
	setTransformToCamera();
	glDepthMask(GL_TRUE);
}

void CGLRenderer::initLights()
{
	glLoadIdentity();
	glScalef(1.0F, 1.0F, -1.0F);	

	// Inits the global light sources or the default light source - the default lightsource will be removed, if a light source is defined
	GLfloat mat_specular[]   = { 1, 1, 1, 1 };
	GLfloat mat_shininess[]  = { 50 };
	GLfloat mat_emission[]   = { 0, 0, 0, 1 };

	GLfloat light_ambient[]  = { 0.2F, 0.2F, 0.2F, 1 };
	GLfloat light_diffuse[]  = { .6F, .6F, .6F, 1 };
	GLfloat light_specular[] = { .2F, .2F, .2F, 1 };

	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, mat_shininess);
	glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, mat_emission);

	glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
	
	GLfloat light_position[] = { -3.0F, 3.0F, -3.0F, 0.0F };
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	
	glEnable(GL_COLOR_MATERIAL);
	glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
	
}

bool CGLRenderer::delayRequest(CRManInterfaceCall &obj)
{
	// Delay transparent primitives (delayRequest is called for primitives only)
	RtFloat alpha = opacityToAlpha(attributes().opacity());
	if ( alpha < 1.0 ) {
		return TypeParent::delayRequest(obj);
	}
	return false;
}

RtVoid CGLRenderer::doProcess(CRiControl &obj)
{
	TypeParent::doProcess(obj);

	if ( obj.name() == RI_GLRENDERER ) {
		CParameterList::const_iterator i;
		for ( i = obj.parameters().begin(); i != obj.parameters().end(); i++ ) {
			if ( (*i).var() == RI_SCREEN ) {
				std::string action;
				if ( (*i).get(0, action) ) {
					if ( action == std::string("clear") )
						clearScreen();
					else if ( action == std::string("finish") )
						finishScreen();
				}
			}
		}
	}
}

RtVoid CGLRenderer::doProcess(CRiWorldBegin &obj)
{
	TypeParent::doProcess(obj);

	initGLContext(); // Can also be called twice (first time at clearScreen)
	if ( !valid() )
		return;
	initHider();
	initViewing();
	initLights();
}

RtVoid CGLRenderer::doProcess(CRiWorldEnd &obj)
{
	TypeParent::doProcess(obj);

	if ( !valid() )
		return;
	
	// Render postponed surfaces

	// glEnable(GL_ALPHA_TEST);
	// glAlphaFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDepthMask(GL_FALSE);

	replayDelayed();
	
	glDisable(GL_ALPHA_TEST);
	glDisable(GL_BLEND);
	glDepthMask(GL_TRUE);

	// Flush GL
	glDisable(GL_SCISSOR_TEST);
	glFinish();
}

RtVoid CGLRenderer::doProcess(CRiPolygon &obj)
{
	if ( delayRequest(obj) )
		return;

	TypeParent::doProcess(obj);
}


RtVoid CGLRenderer::doProcess(CRiGeneralPolygon &obj)
{
	if ( delayRequest(obj) )
		return;

	TypeParent::doProcess(obj);
}


RtVoid CGLRenderer::doProcess(CRiPointsPolygons &obj)
{
	if ( delayRequest(obj) )
		return;
	
	TypeParent::doProcess(obj);
}

RtVoid CGLRenderer::doProcess(CRiPointsGeneralPolygons &obj)
{
	if ( delayRequest(obj) )
		return;

	TypeParent::doProcess(obj);
}

RtVoid CGLRenderer::doProcess(CRiCone &obj)
{
	if ( delayRequest(obj) )
		return;
	
	TypeParent::doProcess(obj);
}

RtVoid CGLRenderer::doProcess(CRiCylinder &obj)
{
	if ( delayRequest(obj) )
		return;
	
	TypeParent::doProcess(obj);
}

RtVoid CGLRenderer::doProcess(CRiDisk &obj)
{
	if ( delayRequest(obj) )
		return;
	
	TypeParent::doProcess(obj);
}

RtVoid CGLRenderer::doProcess(CRiHyperboloid &obj)
{
	if ( delayRequest(obj) )
		return;
	
	TypeParent::doProcess(obj);
}

RtVoid CGLRenderer::doProcess(CRiParaboloid &obj)
{
	if ( delayRequest(obj) )
		return;
	
	TypeParent::doProcess(obj);
}

RtVoid CGLRenderer::doProcess(CRiSphere &obj)
{
	if ( delayRequest(obj) )
		return;
	
	TypeParent::doProcess(obj);
}

RtVoid CGLRenderer::doProcess(CRiTorus &obj)
{
	if ( delayRequest(obj) )
		return;
	
	TypeParent::doProcess(obj);
}

RtVoid CGLRenderer::doProcess(CRiPatch &obj)
{
	if ( delayRequest(obj) )
		return;
	
	TypeParent::doProcess(obj);
}

RtVoid CGLRenderer::doProcess(CRiPatchMesh &obj)
{
	if ( delayRequest(obj) )
		return;
	
	TypeParent::doProcess(obj);
}

RtVoid CGLRenderer::doProcess(CRiNuPatch &obj)
{
	if ( delayRequest(obj) )
		return;
	
	TypeParent::doProcess(obj);
}
