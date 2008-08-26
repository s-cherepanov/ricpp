#ifndef _RICPP_GLRENDERER_GLRENDERER_H
#define _RICPP_GLRENDERER_GLRENDERER_H

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

/** @file glrenderer.h
 *  @author Andreas Pidde (andreas@pidde.de)
 *  @brief Declaration of a context to do some draft GL rendering
 */

#ifndef _RICPP_BASERENDERER_TRIANGLERENDERER_H
#include "ricpp/baserenderer/trianglerenderer.h"
#endif // _RICPP_BASERENDERER_TRIANGLERENDERER_H

#ifndef _RICPP_RICONTEXT_CONTEXTCREATOR_H
#include "ricpp/ricontext/contextcreator.h"
#endif // _RICPP_RICONTEXT_CONTEXTCREATOR_H

#if defined(WIN32)
#include <GL/gl.h>
#else
#if defined(__APPLE__)
#include <OpenGL/gl.h>
#include <OpenGL/OpenGL.h>
#else
#include <GL/gl.h>
#endif
#endif

namespace RiCPP {

#define GLRENDERER_NAME "glrenderer"
#define GLRENDERERCREATOR_NAME "glrenderercreator"

/** @brief Rendering context that does some draft GL rendering
 */
class CGLRenderer : public CTriangleRenderer {
protected:
	typedef CTriangleRenderer TypeParent;
	
private:
	RtToken RI_GLRENDERER;
	RtToken RI_SCREEN;
	RtToken RI_QUAL_SCREEN;
	
	bool m_validGL; ///< Valid GL Context found or created in initGLContext()
	bool m_drawNormals; ///< Drawing normals for tests
	
	inline bool valid() const { return m_validGL; }
	inline void valid(bool setValid) { m_validGL = setValid; }
	
	void initGLContext();
	void initHider();
	void initViewing();
	void initLights();
	
	void setCullFace();
	void setColor();
	
	void clearScreen();
	void finishScreen();

	void drawNormals(const std::vector<RtFloat> &p, const std::vector<RtFloat> &n);

protected:
	virtual void defaultDeclarations();

	virtual void hide(const CFace &f);
	virtual void hideSurface(const CSurface *s);

	virtual bool delayRequest(CRManInterfaceCall &obj);
public:

	CGLRenderer();
	virtual ~CGLRenderer();

	static const char *myName();
	static const char *myType();
	static unsigned long myMajorVersion();
	static unsigned long myMinorVersion();
	static unsigned long myRevision();

	static RtToken myRendererType();

	inline virtual const char *name() const { return myName(); }
	inline virtual const char *type() const { return myType(); }
	inline virtual unsigned long majorVersion() const { return myMajorVersion(); }
	inline virtual unsigned long minorVersion() const { return myMinorVersion(); }
	inline virtual unsigned long revision() const { return myRevision(); }

	inline virtual RtToken rendererType() const { return myRendererType(); }

	using TypeParent::doProcess;
	using TypeParent::preProcess;
	using TypeParent::postProcess;

	virtual RtVoid doProcess(CRiControl &obj);

	virtual RtVoid doProcess(CRiWorldBegin &obj);
	virtual RtVoid doProcess(CRiWorldEnd &obj);
	
	virtual RtVoid doProcess(CRiFrameBegin &obj);
	virtual RtVoid doProcess(CRiFrameEnd &obj);

    virtual RtVoid doProcess(CRiPolygon &obj);
	virtual RtVoid doProcess(CRiGeneralPolygon &obj);
	virtual RtVoid doProcess(CRiPointsPolygons &obj);
    virtual RtVoid doProcess(CRiPointsGeneralPolygons &obj);

    virtual RtVoid doProcess(CRiCone &obj);
    virtual RtVoid doProcess(CRiCylinder &obj);
	virtual RtVoid doProcess(CRiDisk &obj);
    virtual RtVoid doProcess(CRiHyperboloid &obj);
    virtual RtVoid doProcess(CRiParaboloid &obj);
	virtual RtVoid doProcess(CRiSphere &obj);
    virtual RtVoid doProcess(CRiTorus &obj);

	virtual RtVoid doProcess(CRiPatch &obj);
    virtual RtVoid doProcess(CRiPatchMesh &obj);
    virtual RtVoid doProcess(CRiNuPatch &obj);
}; // CGLRenderer

/** @brief Creator for the CGLRenderer rendering context.
 */
class CGLRendererCreator : public CContextCreator {
protected:
	inline virtual IRiContext *getNewContext() { return new CGLRenderer; }

public:
	static const char *myName();
	static const char *myType();
	static unsigned long myMajorVersion();
	static unsigned long myMinorVersion();
	static unsigned long myRevision();

	inline virtual const char *name() const { return myName(); }
	inline virtual const char *type() const { return myType(); }
	inline virtual unsigned long majorVersion() const { return myMajorVersion(); }
	inline virtual unsigned long minorVersion() const { return myMinorVersion(); }
	inline virtual unsigned long revision() const { return myRevision(); }

	inline virtual void startup() {}
	inline virtual void shutdown() {}

	inline virtual unsigned long majorContextVersion() const { return CGLRenderer::myMajorVersion(); }
	inline virtual unsigned long minorContextVersion() const { return CGLRenderer::myMinorVersion(); }
	inline virtual unsigned long contextRevision() const { return CGLRenderer::myRevision(); }
	inline virtual RtToken contextName() const { return CGLRenderer::myName(); }
	inline virtual RtToken contextType() const { return CGLRenderer::myType(); }
	inline virtual RtToken rendererType() const { return CGLRenderer::myRendererType(); }
}; // CGLRendererCreator

} // namespace RiCPP

#endif // _RICPP_GLRENDERER_GLRENDERER_H
