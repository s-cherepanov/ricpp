#ifndef _RICPP_RENDERCONTEXT_RICPPBINDING_H
#define _RICPP_RENDERCONTEXT_RICPPBINDING_H

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

/*! \file ricppbinding.h
 *  \brief Declaration of a base class for RenderMan C++ Binding with parameter handling
 *         RenderMan is (R) by Pixar
 *  \author Andreas Pidde (andreas@pidde.de)
 */

#include "ribparser/ribparser.h"

#include <math.h>
#include <vector>

//! Size, class and type information of one parameter
struct TRiParameterSize {
	RtInt m_class;                  //!< The class of the parameter (vertex, facet, uniform, ...)
	RtInt m_type;                   //!< The type of the parameter (float, integer, color, ...)
	RtInt m_components;             //!< Number of components 1 (normally), can be other if color, point etc.
	RtInt m_cardinality;            //!< cardinality 1 (normally), more if element is an array
	RtInt m_numberOfElements;       //!< Number of all elements of the parameter, dependent of class
	IndexType m_memoryNeeded;       //!< Number of bytes used for all components (string: sum of all string lengthes plus an additional byte per string)
}; // TRiParameterSize


/*! The C++ RIB-Binding base class

    TRiCPPBinding is the interface of the C++ RIB binding. It
	contains the complete interface, tokens and other constants.
	It provides the calling of the V-functions (eg. TRiCPPBinding::sphereV())
	from the ellipsis functions (eg. TRiCPPBinding::sphere()) - so you needn't
	implement any ellipsis functions in derived classes of TRiCPPBinding.
	The tokens of the calls are stored at the std::vector TRiCPPBinding::m_tokens,
	the parameters at TRiCPPBinding::m_params, so that m_params[i] is the value
	of m_tokens[i] for a given index i.
	The V-functions also calculating the sizes of the parameter values and store
	them in TRiCPPBinding::m_sizes so that m_sizes[i] contains size, type and class
	information for a token-parameter pair at position i.
 */
class TRiCPPBinding : public TRi, public TRiCallbackInterface {
	// TRiMacro is a friend to set lineNo
	// friend class TRiMacro;
protected:
	//! Handle parameter lists (filled by TRiCPPBinding::getTokens(), called by the ...-functions, defined only within ... functions)
	//@{
	std::vector<RtToken> m_tokens;		//!< The tokens of the parameter list of an interface call
	std::vector<RtPointer> m_params;	//!< The values of the parameter list of an interface call
	//@}

	//! Sizes of parameter lists of current parameters (filled by TRiCPPBinding::getSizes()), defined only within a function with parameters
	std::vector<TRiParameterSize> m_sizes;

	//! Extracts all token-value pairs of an interface call (...) and stores them at m_tokens, m_params
	RtInt getTokens(RtToken token, va_list marker);
	//! Gets position (return value) and pointer (value) to a value of a given token
	RtInt getValue(RtToken token, RtPointer &value, RtInt n, RtToken tokens[], RtPointer params[]);
	//! Sizes of parameters, called by the V-functions, uses TRiCPPBinding::curColorComponents() and TRiCPPBinding::queryParam() to get the declarations
	RtInt getSizes(RtInt vertices, RtInt corners, RtInt facets, RtInt faceVertices, RtInt faceCorners, RtInt n, RtToken tokens[], RtPointer params[]);
	//! Sizes of parameters, called by the V-functions, uses TRiCPPBinding::curColorComponents() and TRiCPPBinding::queryParam() to get the declarations
	inline RtInt getSizes(const TParameterClasses &classes, RtInt n, RtToken tokens[], RtPointer params[]) {
		return getSizes(classes.vertex(), classes.varying(), classes.uniform(), classes.faceVertex(), classes.faceVarying(), n, tokens, params);
	}

	//! Procedural
	//@{
	inline virtual RtVoid handleDelayedReadArchive(RtPointer data, RtFloat detail) {}
	inline virtual RtVoid handleRunProgram(RtPointer data, RtFloat detail) {}
	inline virtual RtVoid handleDynamicLoad(RtPointer data, RtFloat detail) {}
	inline virtual RtVoid handleFree(RtPointer data) {
		if ( data )
			free(data);
	}
	//@}

	//! current error handler (RiIgnore is default)
	RtErrorHandler m_errHandler;

public:
	//! type of an parameter mus be overloaded
	inline virtual bool queryParam(const char *decl, RtInt vertices, RtInt corners, RtInt facets, RtInt faceVertices, RtInt faceCorners, RtInt &aClass, RtInt &aType, RtInt &aCardinality, RtInt &numComps, IndexType &numBytes) {return false;}

	//! line and current ribarchive (get and set)
	//@{
	inline virtual long lineNo() const { return -1; }
	inline virtual long lineNo(long aLineNo ) { return -1; }
	inline virtual const char *archiveName() const { return NULL; }
	inline virtual const char *archiveName(const char *anArchiveName ) { return NULL; }
	//@}

	//! Current ustep and vstep (PatchMesh)
	//@{
	inline virtual RtInt curUStep() const { return 0; }
	inline virtual RtInt curVStep() const { return 0; }
	//@}

	//! \return get current trimCurve
	inline virtual TTrimCurveData *curTrimCurve() const { return NULL; }

	//! Current components of colors and opacities
	inline virtual RtInt curColorComponents() const { return 3; }

	//! code to handle errors with variable args
	//@{
	virtual RtVoid handleError(RtInt code, RtInt severity, const char *message=NULL, ...);
	virtual RtVoid handleErrorV(RtInt code, RtInt severity, const char *message, va_list argList);
	//@}

public:
	inline TRiCPPBinding() : m_errHandler(TRi::errorIgnore) {}
	virtual ~TRiCPPBinding() { }

	//! Interface routines

	/*! The functions with the ellipses are caling the V-funktions,
	    parameters are parsed (m_tokens, m_params) and the
		sizes (m_sizes) are calculated.
		\sa TRi
	 */
	//@{
	inline virtual RtToken declare(const char *name, const char *declaration) { return RI_NULL; }
	inline virtual RtVoid errorHandler(RtErrorHandler handler) { m_errHandler = handler; }

	inline virtual RtVoid begin(RtToken name = (RtToken)RI_NULL) { }
	inline virtual RtVoid end(void) { }

	inline virtual RtContextHandle getContext(void) { return RI_NULL; }
	inline virtual RtVoid context(RtContextHandle handle) { }

	inline virtual RtVoid frameBegin(RtInt number) { }
	inline virtual RtVoid frameEnd(void) { }

	inline virtual RtVoid worldBegin(void) { }
	inline virtual RtVoid worldEnd(void) { }

	inline virtual RtVoid attributeBegin(void) { }
	inline virtual RtVoid attributeEnd(void) { }

	inline virtual RtVoid transformBegin(void) { }
	inline virtual RtVoid transformEnd(void) { }

	inline virtual RtVoid solidBegin(RtToken type) { }
    inline virtual RtVoid solidEnd(void) { }

	inline virtual RtObjectHandle objectBegin(void) { return (RtObjectHandle)-1; }
	inline virtual RtVoid objectEnd(void) { }
    inline virtual RtVoid objectInstance(RtObjectHandle handle) { }

	virtual RtVoid motionBegin(RtInt N, RtFloat sample, ...);
    inline virtual RtVoid motionBeginV(RtInt N, RtFloat times[]) { }
    inline virtual RtVoid motionEnd(void) { }

	inline virtual RtVoid synchronize(RtToken name) { }

	inline virtual RtVoid format(RtInt xres, RtInt yres, RtFloat aspect) { }
    inline virtual RtVoid frameAspectRatio(RtFloat aspect) { }
    inline virtual RtVoid screenWindow(RtFloat left, RtFloat right, RtFloat bot, RtFloat top) { }
    inline virtual RtVoid cropWindow(RtFloat xmin, RtFloat xmax, RtFloat ymin, RtFloat ymax) { }
    virtual RtVoid projection(const char *name, RtToken token = RI_NULL, ...);
    inline virtual RtVoid projectionV(const char *name, RtInt n, RtToken tokens[], RtPointer params[]) { 
		TParameterClasses p;
		getSizes(p, n, tokens, params);
	}
	inline virtual RtVoid clipping(RtFloat hither, RtFloat yon) { }
    inline virtual RtVoid clippingPlane(RtFloat x, RtFloat y, RtFloat z, RtFloat nx, RtFloat ny, RtFloat nz) { } // *New 3.2*
    inline virtual RtVoid depthOfField(RtFloat fstop, RtFloat focallength, RtFloat focaldistance) { }
    inline virtual RtVoid shutter(RtFloat smin, RtFloat smax) { }
	inline virtual RtVoid pixelVariance(RtFloat variance) { }
    inline virtual RtVoid pixelSamples(RtFloat xsamples, RtFloat ysamples) { }
    inline virtual RtVoid pixelFilter(RtFilterFunc function, RtFloat xwidth, RtFloat ywidth) { }
    inline virtual RtVoid exposure(RtFloat gain, RtFloat gamma) { }
    virtual RtVoid imager(RtToken name, RtToken token = RI_NULL, ...);
    inline virtual RtVoid imagerV(RtToken name, RtInt n, RtToken tokens[], RtPointer params[]) {
		TParameterClasses p;
		getSizes(p, n, tokens, params);
	}
    inline virtual RtVoid quantize(RtToken type, RtInt one, RtInt qmin, RtInt qmax, RtFloat ampl) { }
	virtual RtVoid display(RtToken name, RtToken type, RtToken mode, RtToken token = RI_NULL, ...);
    inline virtual RtVoid displayV(RtToken name, RtToken type, RtToken mode, RtInt n, RtToken tokens[], RtPointer params[]) {
		TParameterClasses p;
		getSizes(p, n, tokens, params);
	}
	virtual RtVoid hider(RtToken type, RtToken token = RI_NULL, ...);
    inline virtual RtVoid hiderV(RtToken type, RtInt n, RtToken tokens[], RtPointer params[]) {
		TParameterClasses p;
		getSizes(p, n, tokens, params);
	}
    inline virtual RtVoid colorSamples(RtInt N, RtFloat *nRGB, RtFloat *RGBn) { }
    inline virtual RtVoid relativeDetail(RtFloat relativedetail) { }
    virtual RtVoid option(const char *name, RtToken token = RI_NULL, ...);
    inline virtual RtVoid optionV(const char *name, RtInt n, RtToken tokens[], RtPointer params[]) {
		TParameterClasses p;
		getSizes(p, n, tokens, params);
	}

	virtual RtLightHandle lightSource(const char *name, RtToken token = RI_NULL, ...);
    inline virtual RtLightHandle lightSourceV(const char *name, RtInt n, RtToken tokens[], RtPointer params[]) {
		TParameterClasses p;
		getSizes(p, n, tokens, params);
		return (RtLightHandle)-1;
	}
    virtual RtLightHandle areaLightSource(const char *name, RtToken token = RI_NULL, ...);
    inline virtual RtLightHandle areaLightSourceV(const char *name, RtInt n, RtToken tokens[], RtPointer params[]) {
		TParameterClasses p;
		getSizes(p, n, tokens, params);
		return (RtLightHandle)-1;
	}

	virtual RtVoid attribute(const char *name, RtToken token = RI_NULL, ...);
    inline virtual RtVoid attributeV(const char *name, RtInt n, RtToken tokens[], RtPointer params[]) {
		TParameterClasses p;
		getSizes(p, n, tokens, params);
	}
	inline virtual RtVoid color(RtColor Cs) { }
	inline virtual RtVoid opacity(RtColor Cs) { }
    virtual RtVoid surface(const char *name, RtToken token = RI_NULL, ...);
    inline virtual RtVoid surfaceV(const char *name, RtInt n, RtToken tokens[], RtPointer params[]) {
		TParameterClasses p;
		getSizes(p, n, tokens, params);
	}
    virtual RtVoid atmosphere(const char *name, RtToken token = RI_NULL, ...);
    inline virtual RtVoid atmosphereV(const char *name, RtInt n, RtToken tokens[], RtPointer params[]) {
		TParameterClasses p;
		getSizes(p, n, tokens, params);
	}
    virtual RtVoid interior(const char *name, RtToken token = RI_NULL, ...);
    inline virtual RtVoid interiorV(const char *name, RtInt n, RtToken tokens[], RtPointer params[]) {
		TParameterClasses p;
		getSizes(p, n, tokens, params);
	}
    virtual RtVoid exterior(const char *name, RtToken token = RI_NULL, ...);
    inline virtual RtVoid exteriorV(const char *name, RtInt n, RtToken tokens[], RtPointer params[]) {
		TParameterClasses p;
		getSizes(p, n, tokens, params);
	}
	virtual RtVoid illuminate(RtLightHandle light, RtBoolean onoff) { }
    virtual RtVoid displacement(const char *name, RtToken token = RI_NULL, ...);
    inline virtual RtVoid displacementV(const char *name, RtInt n, RtToken tokens[], RtPointer params[]) {
		TParameterClasses p;
		getSizes(p, n, tokens, params);
	}
    virtual RtVoid textureCoordinates(RtFloat s1, RtFloat t1, RtFloat s2, RtFloat t2, RtFloat s3, RtFloat t3, RtFloat s4, RtFloat t4) { }
    inline virtual RtVoid shadingRate(RtFloat size) { }
    inline virtual RtVoid shadingInterpolation(RtToken type) { }
    inline virtual RtVoid matte(RtBoolean onoff) { }
	inline virtual RtVoid bound(RtBound bound) { }
	inline virtual RtVoid detail(RtBound bound) { }
    inline virtual RtVoid detailRange(RtFloat minvis, RtFloat lowtran, RtFloat uptran, RtFloat maxvis) { }
    inline virtual RtVoid geometricApproximation(RtToken type, RtFloat value) { }
    inline virtual RtVoid geometricRepresentation(RtToken type) { } /* New 3.2 */
    inline virtual RtVoid orientation(RtToken orientation) { }
	inline virtual RtVoid reverseOrientation(void) { }
    inline virtual RtVoid sides(RtInt nsides) { }
    inline virtual RtVoid basis(RtBasis ubasis, RtInt ustep, RtBasis vbasis, RtInt vstep) { }
    inline virtual RtVoid trimCurve(RtInt nloops, RtInt *ncurves, RtInt *order, RtFloat *knot, RtFloat *amin, RtFloat *amax, RtInt *n, RtFloat *u, RtFloat *v, RtFloat *w) { }

	inline virtual RtVoid identity(void) { }
    inline virtual RtVoid transform(RtMatrix transform) { }
	inline virtual RtVoid concatTransform(RtMatrix transform) { }
    inline virtual RtVoid perspective(RtFloat fov) { }
    inline virtual RtVoid translate(RtFloat dx, RtFloat dy, RtFloat dz) { }
    inline virtual RtVoid rotate(RtFloat angle, RtFloat dx, RtFloat dy, RtFloat dz) { }
    inline virtual RtVoid scale(RtFloat dx, RtFloat dy, RtFloat dz) { }
    inline virtual RtVoid skew(RtFloat angle, RtFloat dx1, RtFloat dy1, RtFloat dz1, RtFloat dx2, RtFloat dy2, RtFloat dz2) { }
    virtual RtVoid deformation(const char *name, RtToken token = RI_NULL, ...);
    inline virtual RtVoid deformationV(const char *name, RtInt n, RtToken tokens[], RtPointer params[]) {
		TParameterClasses p;
		getSizes(p, n, tokens, params);
	}
    inline virtual RtVoid coordinateSystem(RtToken space) { }
    inline virtual RtVoid coordSysTransform(RtToken space) { } /* New 3.2 */
	inline virtual RtPoint *transformPoints(RtToken fromspace, RtToken tospace, RtInt npoints, RtPoint points[]) { return (RtPoint *)0L; }

	virtual RtVoid polygon(RtInt nvertices, RtToken token = RI_NULL, ...);
    inline virtual RtVoid polygonV(RtInt nvertices, RtInt n, RtToken tokens[], RtPointer params[]) {
		TPolygonClasses p(nvertices);
		getSizes(p, n, tokens, params);
	}
    virtual RtVoid generalPolygon(RtInt nloops, RtInt *nverts, RtToken token = RI_NULL, ...);
	inline virtual RtVoid generalPolygonV(RtInt nloops, RtInt *nverts, RtInt n, RtToken tokens[], RtPointer params[]) {
		TGeneralPolygonClasses p(nloops, nverts);
		getSizes(p, n, tokens, params);
	}
    virtual RtVoid pointsPolygons(RtInt npolys, RtInt *nverts, RtInt *verts, RtToken token = RI_NULL, ...);
	inline virtual RtVoid pointsPolygonsV(RtInt npolys, RtInt *nverts, RtInt *verts, RtInt n, RtToken tokens[], RtPointer params[]) {
		TPointsPolygonsClasses p(npolys, nverts, verts);
		getSizes(p, n, tokens, params);
	}
    virtual RtVoid pointsGeneralPolygons(RtInt npolys, RtInt *nloops, RtInt *nverts, RtInt *verts, RtToken token = RI_NULL, ...);
	inline virtual RtVoid pointsGeneralPolygonsV(RtInt npolys, RtInt *nloops, RtInt *nverts, RtInt *verts,  RtInt n, RtToken tokens[], RtPointer params[]) {
		TPointsGeneralPolygonsClasses p(npolys, nloops, nverts, verts);
		getSizes(p, n, tokens, params);
	}

	virtual RtVoid patch(RtToken type, RtToken token = RI_NULL, ...);
	inline virtual RtVoid patchV(RtToken type, RtInt n, RtToken tokens[], RtPointer params[]) {
		TPatchClasses p(type);
		getSizes(p, n, tokens, params);
	}
    virtual RtVoid patchMesh(RtToken type, RtInt nu, RtToken uwrap, RtInt nv, RtToken vwrap, RtToken token = RI_NULL, ...);
	inline virtual RtVoid patchMeshV(RtToken type, RtInt nu, RtToken uwrap, RtInt nv, RtToken vwrap, RtInt n, RtToken tokens[], RtPointer params[]) {
		TPatchMeshClasses p(type, nu, curUStep(), uwrap, nv, curVStep(), vwrap);
		getSizes(p, n, tokens, params);
	}
    virtual RtVoid nuPatch(RtInt nu, RtInt uorder, RtFloat *uknot, RtFloat umin, RtFloat umax, RtInt nv, RtInt vorder, RtFloat *vknot, RtFloat vmin, RtFloat vmax, RtToken token = RI_NULL, ...);
	inline virtual RtVoid nuPatchV(RtInt nu, RtInt uorder, RtFloat *uknot, RtFloat umin, RtFloat umax, RtInt nv, RtInt vorder, RtFloat *vknot, RtFloat vmin, RtFloat vmax,  RtInt n, RtToken tokens[], RtPointer params[]) {
		TNuPatchClasses p(nu, uorder, nv, vorder);
		getSizes(p, n, tokens, params);
	}

	virtual RtVoid subdivisionMesh(RtToken scheme, RtInt nfaces, RtInt nvertices[], RtInt vertices[], RtInt ntags, RtToken tags[], RtInt nargs[], RtInt intargs[], RtFloat floatargs[], RtToken token = RI_NULL, ...); /* New 3.2 */
	inline virtual RtVoid subdivisionMeshV(RtToken scheme, RtInt nfaces, RtInt nvertices[], RtInt vertices[], RtInt ntags, RtToken tags[], RtInt nargs[], RtInt intargs[], RtFloat floatargs[],  RtInt n, RtToken tokens[], RtPointer params[]) { /* New 3.2 */
		TSubdivisionMeshClasses p(nfaces, nvertices, vertices);
		getSizes(p, n, tokens, params);
	}

	virtual RtVoid sphere(RtFloat radius, RtFloat zmin, RtFloat zmax, RtFloat thetamax, RtToken token = RI_NULL, ...);
    inline virtual RtVoid sphereV(RtFloat radius, RtFloat zmin, RtFloat zmax, RtFloat thetamax, RtInt n, RtToken tokens[], RtPointer params[]) {
		TQuadricClasses p;
		getSizes(p, n, tokens, params);
	}
	virtual RtVoid cone(RtFloat height, RtFloat radius, RtFloat thetamax, RtToken token = RI_NULL, ...);
    inline virtual RtVoid coneV(RtFloat height, RtFloat radius, RtFloat thetamax, RtInt n, RtToken tokens[], RtPointer params[]) {
		TQuadricClasses p;
		getSizes(p, n, tokens, params);
	}
	virtual RtVoid cylinder(RtFloat radius, RtFloat zmin, RtFloat zmax, RtFloat thetamax, RtToken token = RI_NULL, ...);
    inline virtual RtVoid cylinderV(RtFloat radius, RtFloat zmin, RtFloat zmax, RtFloat thetamax, RtInt n, RtToken tokens[], RtPointer params[]) {
		TQuadricClasses p;
		getSizes(p, n, tokens, params);
	}
	virtual RtVoid hyperboloid(RtPoint point1, RtPoint point2, RtFloat thetamax, RtToken token = RI_NULL, ...);
    inline virtual RtVoid hyperboloidV(RtPoint point1, RtPoint point2, RtFloat thetamax, RtInt n, RtToken tokens[], RtPointer params[]) {
		TQuadricClasses p;
		getSizes(p, n, tokens, params);
	}
	virtual RtVoid paraboloid (RtFloat rmax, RtFloat zmin, RtFloat zmax, RtFloat thetamax, RtToken token = RI_NULL, ...);
    inline virtual RtVoid paraboloidV(RtFloat rmax, RtFloat zmin, RtFloat zmax, RtFloat thetamax, RtInt n, RtToken tokens[], RtPointer params[]) {
		TQuadricClasses p;
		getSizes(p, n, tokens, params);
	}
	virtual RtVoid disk(RtFloat height, RtFloat radius, RtFloat thetamax, RtToken token = RI_NULL, ...);
    inline virtual RtVoid diskV(RtFloat height, RtFloat radius, RtFloat thetamax, RtInt n, RtToken tokens[], RtPointer params[]) {
		TQuadricClasses p;
		getSizes(p, n, tokens, params);
	}
	virtual RtVoid torus(RtFloat majorrad, RtFloat minorrad, RtFloat phimin, RtFloat phimax, RtFloat thetamax, RtToken token = RI_NULL, ...);
    inline virtual RtVoid torusV(RtFloat majorrad, RtFloat minorrad, RtFloat phimin, RtFloat phimax, RtFloat thetamax, RtInt n, RtToken tokens[], RtPointer params[]) {
		TQuadricClasses p;
		getSizes(p, n, tokens, params);
	}

	virtual RtVoid points(RtInt npts,RtToken token = RI_NULL, ...); /* New 3.2 */
    inline virtual RtVoid pointsV(RtInt npts, RtInt n, RtToken tokens[], RtPointer params[]) { /* New 3.2 */
		TPointsClasses p(npts);
		getSizes(p, n, tokens, params);
	}
    virtual RtVoid curves(RtToken type, RtInt ncurves, RtInt nverts[], RtToken wrap, RtToken token = RI_NULL, ...); /* New 3.2 */
	inline virtual RtVoid curvesV(RtToken type, RtInt ncurves, RtInt nverts[], RtToken wrap, RtInt n, RtToken tokens[], RtPointer params[]) { /* New 3.2 */
		TCurvesClasses p(type, ncurves, nverts, wrap, curVStep());
		getSizes(p, n, tokens, params);
	}

	virtual RtVoid blobby(RtInt nleaf, RtInt ncode, RtInt code[], RtInt nflt, RtFloat flt[], RtInt nstr, RtString str[], RtToken token = RI_NULL, ...); /* New 3.2 */
    inline virtual RtVoid blobbyV(RtInt nleaf, RtInt ncode, RtInt code[], RtInt nflt, RtFloat flt[], RtInt nstr, RtString str[], RtInt n, RtToken tokens[], RtPointer params[]) { /* New 3.2 */
		TBlobbyClasses p(nleaf);
		getSizes(p, n, tokens, params);
	}

	inline virtual RtVoid procedural(RtPointer data, RtBound bound, RtProcSubdivFunc subdivfunc, RtProcFreeFunc freefunc) { }

	virtual RtVoid geometry(RtToken type, RtToken token = RI_NULL, ...);
    inline virtual RtVoid geometryV(RtToken type, RtInt n, RtToken tokens[], RtPointer params[]) {
		TParameterClasses p;
		getSizes(p, n, tokens, params);
	}

	virtual RtVoid makeTexture(const char *pic, const char *tex, RtToken swrap, RtToken twrap, RtFilterFunc filterfunc, RtFloat swidth, RtFloat twidth, RtToken token = RI_NULL, ...);
    inline virtual RtVoid makeTextureV(const char *pic, const char *tex, RtToken swrap, RtToken twrap, RtFilterFunc filterfunc, RtFloat swidth, RtFloat twidth, RtInt n, RtToken tokens[], RtPointer params[]) {
		TParameterClasses p;
		getSizes(p, n, tokens, params);
	}
    virtual RtVoid makeBump(const char *pic, const char *tex, RtToken swrap, RtToken twrap, RtFilterFunc filterfunc, RtFloat swidth, RtFloat twidth, RtToken token = RI_NULL, ...);
    inline virtual RtVoid makeBumpV(const char *pic, const char *tex, RtToken swrap, RtToken twrap, RtFilterFunc filterfunc, RtFloat swidth, RtFloat twidth, RtInt n, RtToken tokens[], RtPointer params[]) {
		TParameterClasses p;
		getSizes(p, n, tokens, params);
	}
    virtual RtVoid makeLatLongEnvironment(const char *pic, const char *tex, RtFilterFunc filterfunc, RtFloat swidth, RtFloat twidth, RtToken token = RI_NULL, ...);
    inline virtual RtVoid makeLatLongEnvironmentV(const char *pic, const char *tex, RtFilterFunc filterfunc, RtFloat swidth, RtFloat twidth, RtInt n, RtToken tokens[], RtPointer params[]) {
		TParameterClasses p;
		getSizes(p, n, tokens, params);
	}
    virtual RtVoid makeCubeFaceEnvironment(const char *px, const char *nx, const char *py, const char *ny, const char *pz, const char *nz, const char *tex, RtFloat fov, RtFilterFunc filterfunc, RtFloat swidth, RtFloat twidth, RtToken token = RI_NULL, ...);
    inline virtual RtVoid makeCubeFaceEnvironmentV(const char *px, const char *nx, const char *py, const char *ny, const char *pz, const char *nz, const char *tex, RtFloat fov, RtFilterFunc filterfunc, RtFloat swidth, RtFloat twidth, RtInt n, RtToken tokens[], RtPointer params[]) {
		TParameterClasses p;
		getSizes(p, n, tokens, params);
	}
    virtual RtVoid makeShadow(const char *pic, const char *tex, RtToken token = RI_NULL, ...);
    inline virtual RtVoid makeShadowV(const char *pic, const char *tex, RtInt n, RtToken tokens[], RtPointer params[]) {
		TParameterClasses p;
		getSizes(p, n, tokens, params);
	}

	virtual RtVoid archiveRecord(RtToken type, const char *format, ...); /* New 3.2 */
	inline virtual RtVoid archiveRecordV(RtToken type, const char *line) { } /* Added */
    virtual RtVoid readArchive(RtString filename, RtArchiveCallback callback, RtToken token = RI_NULL, ...); /* New 3.2 */
	inline virtual RtVoid readArchiveV(RtString filename, RtArchiveCallback callback, RtInt n, RtToken tokens[], RtPointer params[]) { /* New 3.2 */
		TParameterClasses p;
		getSizes(p, n, tokens, params);
	}
	//@}
}; // TRiCPPBinding

#endif // _RICPP_RENDERCONTEXT_RICPPBINDING_H
