#ifndef _RICPP_GLRENDERER_GLRENDERER_H#define _RICPP_GLRENDERER_GLRENDERER_H// RICPP - RenderMan(R) Interface CPP Language Binding////     RenderMan(R) is a registered trademark of Pixar// The RenderMan(R) Interface Procedures and Protocol are://         Copyright 1988, 1989, 2000, 2005 Pixar//                 All rights Reservered//// Copyright (c) of RiCPP 2007, Andreas Pidde// Contact: andreas@pidde.de//// This library is free software; you can redistribute it and/or// modify it under the terms of the GNU General Public// License as published by the Free Software Foundation; either// version 2 of the License, or (at your option) any later version.//  // This library is distributed in the hope that it will be useful,// but WITHOUT ANY WARRANTY; without even the implied warranty of// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU// General Public License for more details.//// You should have received a copy of the GNU General Public// License along with this library; if not, write to the Free Software// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA/** @file glrenderer.h *  @author Andreas Pidde (andreas@pidde.de) *  @brief Declaration of a context to do some draft GL rendering */#ifndef _RICPP_BASERENDERER_BASERENDERER_H#include "ricpp/baserenderer/baserenderer.h"#endif // _RICPP_BASERENDERER_BASERENDERER_H#ifndef _RICPP_RICONTEXT_CONTEXTCREATOR_H#include "ricpp/ricontext/contextcreator.h"#endif // _RICPP_RICONTEXT_CONTEXTCREATOR_H#ifndef _RICPP_RICONTEXT_POLYGON_H#include "ricpp/ricontext/polygon.h"#endif // _RICPP_RICONTEXT_POLYGON_H#ifndef _RICPP_RICONTEXT_SURFACE_H#include "ricpp/ricontext/surface.h"#endif // _RICPP_RICONTEXT_SURFACE_H#if defined(WIN32)#include <GL/gl.h>#else#if defined(__APPLE__)#include <OpenGL/gl.h>#include <OpenGL/OpenGL.h>#else#include <GL/gl.h>#endif#endifnamespace RiCPP {#define GLRENDERER_NAME "glrenderer"#define GLRENDERERCREATOR_NAME "glrenderercreator"/** @brief Rendering context that does some draft GL rendering */class CGLRenderer : public CBaseRenderer {protected:	typedef CBaseRenderer TypeParent;private:	bool m_validGL; ///< Valid GL Context found or created in initGLContext()	CEarClipper m_earClipper; ///< Triangulation strategy	GLenum m_cullFace; ///< Current cullface set by sides, maintained by setCullFace		///! Delayed request used for "transparency"	class CDelayedRequest {	public:		CRManInterfaceCall *m_req; //!< Request (graphics primitive for delayed call)		CAttributes *m_attributes; //!< Attribute set that was valid while request was called		CTransformation *m_transformation; //!< To world transformation (cuurent valid while request was called)		inline CDelayedRequest(CRManInterfaceCall *req, const CAttributes *attrib, const CTransformation *trans)		: m_req(req), m_attributes(attrib), m_transformation(trans) {}	};	std::list<CDelayedRequest> m_delayedRequests; //!< used to store delayed requests	bool m_replayDelayedMode; //!< true, while replaying of delayed requests (controlled via doWorldEnd())	CAttributes *m_attributes; //!< Attributes of delayed request	CTransformation *m_transformation; //!< Transformation of delayed request	CMatrix3D toCamera();	protected:	inline bool valid() const { return m_validGL; }	inline void valid(bool setValid) { m_validGL = setValid; }		CAttributes &attributes();	CTransformation &transformation();	bool delayRequest(CRManInterfaceCall &obj);	virtual void defaultDeclarations();	virtual void initGLContext();	virtual void initHider();	virtual void initViewing();	virtual void initLights();		inline virtual const IPolygonTriangulationStrategy &triangulationStrategy() const { return m_earClipper; }	inline GLenum cullFace() const { return m_cullFace; }	void toggleCullFace();	void setCullFace();	void setColor();		virtual void hide(const CFace &f);	virtual void hide(const CSurface &s);public:	CGLRenderer();	virtual ~CGLRenderer();	static const char *myName();	static const char *myType();	static unsigned long myMajorVersion();	static unsigned long myMinorVersion();	static unsigned long myRevision();	static RtToken myRendererType();	inline virtual const char *name() const { return myName(); }	inline virtual const char *type() const { return myType(); }	inline virtual unsigned long majorVersion() const { return myMajorVersion(); }	inline virtual unsigned long minorVersion() const { return myMinorVersion(); }	inline virtual unsigned long revision() const { return myRevision(); }	inline virtual RtToken rendererType() const { return myRendererType(); }	virtual RtVoid doBegin(CRiBegin &obj, RtString name, const CParameterList &params);	virtual RtVoid doEnd(CRiEnd &obj);	virtual RtVoid doWorldBegin(CRiWorldBegin &obj);	virtual RtVoid doWorldEnd(CRiWorldEnd &obj);	    virtual RtVoid doPolygon(CRiPolygon &obj, RtInt nvertices, const CParameterList &params);	virtual RtVoid doGeneralPolygon(CRiGeneralPolygon &obj, RtInt nloops, RtInt nverts[], const CParameterList &params);	virtual RtVoid doPointsPolygons(CRiPointsPolygons &obj, RtInt npolys, RtInt nverts[], RtInt verts[], const CParameterList &params);    virtual RtVoid doPointsGeneralPolygons(CRiPointsGeneralPolygons &obj, RtInt npolys, RtInt nloops[], RtInt nverts[], RtInt verts[], const CParameterList &params);}; // CGLRenderer/** @brief Creator for the CGLRenderer rendering context. */class CGLRendererCreator : public CContextCreator {protected:	inline virtual IRiContext *getNewContext() { return new CGLRenderer; }public:	static const char *myName();	static const char *myType();	static unsigned long myMajorVersion();	static unsigned long myMinorVersion();	static unsigned long myRevision();	inline virtual const char *name() const { return myName(); }	inline virtual const char *type() const { return myType(); }	inline virtual unsigned long majorVersion() const { return myMajorVersion(); }	inline virtual unsigned long minorVersion() const { return myMinorVersion(); }	inline virtual unsigned long revision() const { return myRevision(); }	inline virtual void startup() {}	inline virtual void shutdown() {}	inline virtual unsigned long majorContextVersion() const { return CGLRenderer::myMajorVersion(); }	inline virtual unsigned long minorContextVersion() const { return CGLRenderer::myMinorVersion(); }	inline virtual unsigned long contextRevision() const { return CGLRenderer::myRevision(); }	inline virtual RtToken contextName() const { return CGLRenderer::myName(); }	inline virtual RtToken contextType() const { return CGLRenderer::myType(); }	inline virtual RtToken rendererType() const { return CGLRenderer::myRendererType(); }}; // CGLRendererCreator} // namespace RiCPP#endif // _RICPP_GLRENDERER_GLRENDERER_H