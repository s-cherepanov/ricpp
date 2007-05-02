#ifndef _RICPP_LIBGLRENDERER_LIBGLRENDERER_H
#define _RICPP_LIBGLRENDERER_LIBGLRENDERER_H

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

/*! \file glrenderer.h
 *  \brief RenderMan C++ Binding for OpenGL output
 *         RenderMan is (R) by Pixar
 *  \author Andreas Pidde (andreas@pidde.de)
 */

#include "glprims.h"
#include "glcpp/wglrc.h"
#include "rendercontext/rendercontext.h"

#include <GL/glu.h>
#include <set>

// Maximal number of concurrent light sources (without ambient light)
#define GLRENDERER_MAXLIGHTS 8

class TGLDisplayDesc {
public:
	BOOL m_isMemoryContext;
	HDC m_hdc;
	HWND m_hwnd;
	TDisplayDesc *m_displayDesc;
	#ifdef LINUX
	XVisualInfo *m_visInfo;
	#endif

	TGLDisplayDesc();
	~TGLDisplayDesc();
};

typedef std::map<std::string, TGLTextureMap *> STRING2TMAP;

class TGLContext : public TRiCPPContext {
public:
	// OpenGL
	HDC m_hdc;
	HWND m_hwnd;

	// Current texture
	STRING2TMAP m_mapTMap;
	TGLTextureMap *m_currTMap;

	// Current bump texture
	STRING2TMAP m_mapBMap;
	TGLTextureMap *m_currBMap;

	std::vector<RtFloat> m_colorVect;
public:

	inline TGLContext() {}
	inline virtual ~TGLContext() {}

	inline static const char *myClassName(void) {return "TGLContext";}
	inline virtual const char *className() const { return TGLContext::myClassName(); }
	inline virtual bool isKindOf(const char *aClass) const {
		if ( !aClass )
			return false;
		if  ( !stricmp(aClass, TGLContext::myClassName()) ) {
			return true;
		}
		return TRiCPPContext::isKindOf(aClass);
	}
};


class TGLOptions : public TRenderOptions {
public:

protected:
	void assign(const TGLOptions &o) {
	}

public:
	inline virtual void initialize(int colorcomponents) {
		TRenderOptions::initialize();
	}

	inline TGLOptions() {}
	inline virtual ~TGLOptions() {}

	inline static const char *myClassName(void) {return "TGLOptions";}
	inline virtual const char *className() const { return TGLOptions::myClassName(); }
	inline virtual bool isKindOf(const char *aClass) const {
		if ( !aClass )
			return false;
		if  ( !stricmp(aClass, TGLOptions::myClassName()) ) {
			return true;
		}
		return TRenderOptions::isKindOf(aClass);
	}

	inline virtual TBaseClass &duplicate() const {
		TGLOptions *o = new TGLOptions(*this);
		if ( !o )
			throw TSystemException(__FILE__, __LINE__);
		return *o;
	}
	inline virtual TBaseClass &operator=(const TBaseClass &o) {
		if ( &o == this )
			return *this;
		if ( !o.isKindOf(TGLOptions::myClassName()) ) {
			// Throw illegal type for assignment
			throw TSystemException(TSystemException::m_errIllegalTypename(), __FILE__, __LINE__);
			return *this;
		}
		TGLOptions::assign((const TGLOptions &)o);
		return TRenderOptions::operator=(o);
	}
};


class TGLAttributes : public TRenderAttributes {
public:
	// general attributes
	RtFloat m_tesselation[2];
	RtFloat m_tessTrim;

protected:
	inline void assign(const TGLAttributes &o) {
		m_tesselation[0] = o.m_tesselation[0];
		m_tesselation[1] = o.m_tesselation[1];
		m_tessTrim = o.m_tessTrim;
	}

public:
	inline virtual void initialize() {
		m_tesselation[0] = (RtFloat)16.0;
		m_tesselation[1] = (RtFloat)16.0;
		m_tessTrim = (RtFloat)16.0;
		TRenderAttributes::initialize();
	}

	inline TGLAttributes(int colorComponents = 3) : TRenderAttributes(colorComponents) {
		// initialize();
	}
	inline TGLAttributes(const TGLAttributes &att) : TRenderAttributes(att) {
		assign(att);
	}
	inline virtual ~TGLAttributes() {}

	inline static const char *myClassName(void) {return "TGLAttributes";}
	inline virtual const char *className() const { return TGLAttributes::myClassName(); }
	inline virtual bool isKindOf(const char *aClass) const {
		if ( !aClass )
			return false;
		if  ( !stricmp(aClass, TGLAttributes::myClassName()) ) {
			return true;
		}
		return TRenderAttributes::isKindOf(aClass);
	}

	inline virtual TBaseClass &duplicate() const {
		TGLAttributes *o = new TGLAttributes(*this);
		if ( !o )
			throw TSystemException(__FILE__, __LINE__);
		return *o;
	}
	inline virtual TBaseClass &operator=(const TBaseClass &o) {
		if ( &o == this )
			return *this;
		if ( !o.isKindOf(TGLAttributes::myClassName()) ) {
			// Throw illegal type for assignment
			throw TSystemException(TSystemException::m_errIllegalTypename(), __FILE__, __LINE__);
			return *this;
		}
		assign((const TGLAttributes &)o);
		return TRenderAttributes::operator=(o);
	}
};


class TGLRenderer : public TIntermediateRenderer, TGLTransformRGB, TDrawPrim {
	TGLDisplayDesc m_currGLDisplayDesc;

protected:
	// tokens of additional default declarations
	static RtToken ri_DeviceContext;
	static RtToken ri_WindowHandle;
	static RtToken ri_VisualInfo;
	static RtToken ri_Display;
	static RtToken ri_Tesselation;
	static RtToken ri_TesselationU;
	static RtToken ri_TesselationV;
	static RtToken ri_ConstantPoints;
	static RtToken ri_ConstantLines;
	static RtToken ri_Points;
	static RtToken ri_Lines;

	inline TGLContext &currentContext() const {
		return *(TGLContext *)(m_currentContext);
	}

	inline TGLAttributes &currentAttributes() const {
		return *(TGLAttributes *)(m_currentContext->m_currentAttributes);
	}

	inline TGLOptions &currentOptions() const {
		return *(TGLOptions *)(m_currentContext->m_currentOptions);
	}

	//! \return current tesselation in parametric u
	virtual RtFloat curUTesselation() const;
	//! \return current tesselation in parametric v
	virtual RtFloat curVTesselation() const;
	//! \return current tesselation of a trim curve
	virtual RtFloat curTessTrim()     const;

	// active RenderContext
	bool m_openglIsActive;

	// OpenGL
	TWGLRenderContext m_glrc;

	GLUtesselator *m_tesselator;
	GLUnurbsObj *m_nurbs;

	// light sources
	GLint m_maxLights;

	// set of unused GL lights indexes
	std::set<int, std::less<int> > m_unusedLightSources;

	// LightHandle (Index) -> GL light indexes (>=0 Index, -1 ambient, <= -2 not used)
	std::vector<int> m_lightIndexes;

	// vector of ambient light - is accumulated for glLightModel,
	// contains index to light list
	std::vector<RtLightHandle> m_ambientLights;

	// Current texture
	STRING2TMAP m_mapTMap;
	TGLTextureMap *m_currTMap;

	// Current bump texture
	STRING2TMAP m_mapBMap;
	TGLTextureMap *m_currBMap;

	// Vector for colors
	std::vector<RtFloat> m_surfaceColor;

	// Bilinear patch
	RtFloat m_bilinearTesselation[2];
	std::vector<RtInt> m_bilinearIndices;
	std::vector<RtFloat> m_bilinearVertexStrip;
	std::vector<RtFloat> m_bilinearNormalStrip;

	// Bicubic patch
	RtFloat m_bicubicTesselation[2];
	std::vector<RtFloat> m_uVector;
	std::vector<RtFloat> m_vVector;
	std::vector<RtFloat> m_duVector;
	std::vector<RtFloat> m_dvVector;
	std::vector<RtInt> m_bicubicIndices;
	std::vector<RtFloat> m_bicubicVertexStrip;
	std::vector<RtFloat> m_bicubicNormalStrip;

	// Polygons
	std::vector<RtFloat> m_polygonNormalStrip;

	// Procedural
	virtual RtVoid handleDelayedReadArchive(RtPointer data, RtFloat detail);
	virtual RtVoid handleRunProgram(RtPointer data, RtFloat detail);
	virtual RtVoid handleDynamicLoad(RtPointer data, RtFloat detail);
	// virtual RtVoid handleFree(RtPointer data);

	void glRendererDefaultDeclarations(); // Set default declarations
	void finishGL();            // Flushes GL and disconnects context

	// Reestablish default declarations
	inline virtual void deleteCurrentContext() {
		TIntermediateRenderer::deleteCurrentContext();
		glRendererDefaultDeclarations();
	}

	// geometry
	RtVoid drawGeometricPrim(TGeometricPrimFace &primFace);

	// light
	RtVoid switchLightOn(RtLightHandle h);

	virtual void ambientLight(TLight &light, RtLightHandle handle, RtFloat intensity, RtColor lightcolor);
	virtual void distantLight(TLight &light, RtLightHandle handle, RtFloat intensity, RtColor lightcolor, RtPoint from, RtPoint to);
	virtual void pointLight(TLight &light, RtLightHandle handle, RtFloat intensity, RtColor lightcolor, RtPoint from);
	virtual void spotLight(TLight &light, RtLightHandle handle, RtFloat intensity, RtColor lightcolor, RtPoint from, RtPoint to, RtFloat coneangle, RtFloat conedeltaangle, RtFloat beamdistribution);
	virtual void nonStandardLight(TLight &light, RtLightHandle handle, RtFloat intensity, RtColor lightcolor);

	virtual void resetLights();
	virtual void establishAttributes();

	int allocLightIndex();				// Get an light index: GL_LIGHT0+idx = GLLight
										// -1 not used, <= -2 no light available
	void freeLightIndex(int lightIdx);	// make lightIdx available

	// standard shader
	void setTMap(const char *texturename);
	void setBMap(const char *texturename);
	virtual void constantShader(int shaderType);
	virtual void matteShader(int shaderType, RtFloat Ka, RtFloat Kd);
	virtual void metalShader(int shaderType, RtFloat Ka, RtFloat Ks, RtFloat roughness);
	virtual void shinyMetalShader(int shaderType, RtFloat Ka, RtFloat Ks, RtFloat Kr, RtFloat roughness, const char *texturename);
	virtual void plasticShader(int shaderType, RtFloat Ka, RtFloat Kd, RtFloat Ks, RtFloat roughness, RtFloat *specularColor);
	virtual void paintedPlasticShader(int shaderType, RtFloat Ka, RtFloat Kd, RtFloat Ks, RtFloat roughness, RtFloat *specularColor, const char *texturename);
	virtual void bumpyShader(int shaderType, RtFloat amplitude, const char *texturename);
	virtual void depthCueShader(int shaderType, RtFloat mindistance, RtFloat maxdistance, RtFloat *background);
	virtual void fogShader(int shaderType, RtFloat distance, RtFloat *background);

	virtual void nonStandardShader(int shaderType);

	virtual void establishShader(int shaderType);

	// overloaded to return GL specific attributes
	virtual TRenderAttributes *getNewAttributes();
	virtual TRenderAttributes *getNewAttributes(const TRenderAttributes *);

	// overloaded to return GL specific options
	virtual TRenderOptions *getNewOptions();
	virtual TRenderOptions *getNewOptions(const TRenderOptions *);

	// Get a new context, overloaded to return context with GL specific values
	virtual TRiCPPContext *getNewContext();

	// Render stack
	virtual void establishTransformation();

	//! Returns an interface to draw triangulated primaries.
	inline virtual TDrawPrim *getDrawPrim() {
		// return NULL;
		return this;
	}

	// Aux
	virtual RtVoid notifyOrientationChanged();
	virtual RtVoid establishProjection();
	bool getCurrentFrontFace();
	RtVoid coneDisplacedV(RtFloat height, RtFloat radius, RtFloat thetamax, RtFloat displacement, RtInt n, RtToken tokens[], RtPointer params[]);

public:
	TGLRenderer();
	virtual ~TGLRenderer();

	// Aux
	virtual void transformRGB(GLfloat *surfacecolor, IndexType ncolors, RtFloat *colorarray);

protected:
	// Interface functions
	
	// blockstructure
	virtual RtVoid doEnd(void);
	virtual RtVoid doWorldBegin(void);
	virtual RtVoid doWorldEnd(void);

	// synchronize
    virtual RtVoid doSynchronize(RtToken name);

	// camera
    virtual RtVoid doDisplayV(RtToken name, RtToken type, RtToken mode, RtInt n, RtToken tokens[], RtPointer params[]);

	// attributes
	virtual RtVoid doAttributeEnd(void);
	virtual RtVoid doColor(RtColor Cs);
	virtual RtVoid doOpacity(RtColor Cs);
    virtual RtVoid doMatte(RtBoolean onoff);
    virtual RtVoid doGeometricApproximation(RtToken type, RtFloat value);
    virtual RtVoid doGeometricRepresentation(RtToken type);
    virtual RtVoid doSides(RtInt nsides);

	// transformation
	virtual RtVoid doIdentity(void);
    virtual RtVoid doTransform(RtMatrix transform);
	virtual RtVoid doConcatTransform(RtMatrix transform);
    virtual RtVoid doPerspective(RtFloat fov);
    virtual RtVoid doTranslate(RtFloat dx, RtFloat dy, RtFloat dz);
    virtual RtVoid doRotate(RtFloat angle, RtFloat dx, RtFloat dy, RtFloat dz);
    virtual RtVoid doScale(RtFloat dx, RtFloat dy, RtFloat dz);
    virtual RtVoid doSkew(RtFloat angle, RtFloat dx1, RtFloat dy1, RtFloat dz1, RtFloat dx2, RtFloat dy2, RtFloat dz2);

	// procedural
    virtual RtVoid doProcedural(RtPointer data, RtBound bound, RtProcSubdivFunc subdivfunc, RtProcFreeFunc freefunc);

	// geometry
    virtual RtVoid doPolygonV(RtInt nvertices, RtInt n, RtToken tokens[], RtPointer params[]);
    virtual RtVoid doGeneralPolygonV(RtInt nloops, RtInt *nverts, RtInt n, RtToken tokens[], RtPointer params[]);
    virtual RtVoid doPointsPolygonsV(RtInt npolys, RtInt *nverts, RtInt *verts, RtInt n, RtToken tokens[], RtPointer params[]);
    virtual RtVoid doPointsGeneralPolygonsV(RtInt npolys, RtInt *nloops, RtInt *nverts, RtInt *verts,  RtInt n, RtToken tokens[], RtPointer params[]);
    virtual RtVoid doPatchV(RtToken type, RtInt n, RtToken tokens[], RtPointer params[]);
    virtual RtVoid doPatchMeshV(RtToken type, RtInt nu, RtToken uwrap, RtInt nv, RtToken vwrap, RtInt n, RtToken tokens[], RtPointer params[]);
    virtual RtVoid doNuPatchV(RtInt nu, RtInt uorder, RtFloat *uknot, RtFloat umin, RtFloat umax, RtInt nv, RtInt vorder, RtFloat *vknot, RtFloat vmin, RtFloat vmax,  RtInt n, RtToken tokens[], RtPointer params[]);
    virtual RtVoid doConeV(RtFloat height, RtFloat radius, RtFloat thetamax, RtInt n, RtToken tokens[], RtPointer params[]);
    virtual RtVoid doCylinderV(RtFloat radius, RtFloat zmin, RtFloat zmax, RtFloat thetamax, RtInt n, RtToken tokens[], RtPointer params[]);
    virtual RtVoid doDiskV(RtFloat height, RtFloat radius, RtFloat thetamax, RtInt n, RtToken tokens[], RtPointer params[]);
    virtual RtVoid doHyperboloidV(RtPoint point1, RtPoint point2, RtFloat thetamax, RtInt n, RtToken tokens[], RtPointer params[]);
    virtual RtVoid doParaboloidV(RtFloat rmax, RtFloat zmin, RtFloat zmax, RtFloat thetamax, RtInt n, RtToken tokens[], RtPointer params[]);
    virtual RtVoid doSphereV(RtFloat radius, RtFloat zmin, RtFloat zmax, RtFloat thetamax, RtInt n, RtToken tokens[], RtPointer params[]);
    virtual RtVoid doTorusV(RtFloat majorrad, RtFloat minorrad, RtFloat phimin, RtFloat phimax, RtFloat thetamax, RtInt n, RtToken tokens[], RtPointer params[]);
    virtual RtVoid doGeometryV(RtToken type, RtInt n, RtToken tokens[], RtPointer params[]);
    virtual RtVoid doSubdivisionMeshV(RtToken scheme, RtInt nfaces, RtInt nvertices[], RtInt vertices[], RtInt ntags, RtToken tags[], RtInt nargs[], RtInt intargs[], RtFloat floatargs[],  RtInt n, RtToken tokens[], RtPointer params[]);
	virtual RtVoid doBlobbyV(RtInt nleaf, RtInt ncode, RtInt code[], RtInt nflt, RtFloat flt[], RtInt nstr, RtString str[], RtInt n, RtToken tokens[], RtPointer params[]);
	virtual RtVoid doPointsV(RtInt npts, RtInt n, RtToken tokens[], RtPointer params[]);
	virtual RtVoid doTrimCurve(RtInt nloops, RtInt *ncurves, RtInt *order, RtFloat *knot, RtFloat *amin, RtFloat *amax, RtInt *n, RtFloat *u, RtFloat *v, RtFloat *w);
};

#endif // _RICPP_LIBGLRENDERER_LIBGLRENDERER_H
