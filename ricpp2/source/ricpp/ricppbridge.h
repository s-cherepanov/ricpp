#ifndef _RICPP_RICPP_RICPPBRIDGE_H
#define _RICPP_RICPP_RICPPBRIDGE_H

// RICPP - RenderMan(R) Interface CPP Language Binding
//
//     RenderMan(R) is a registered trademark of Pixar
// The RenderMan(R) Interface Procedures and Protocol are:
//         Copyright 1988, 1989, 200,, 2005 Pixar
//                 All rights Reservered
//
// Copyright © of RiCPP 2007, Andreas Pidde
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


#ifndef _RICPP_RICPP_FILTERS_H
#include "ricpp/filters.h"
#endif

#ifndef _RICPP_RICPP_SUBDIVFUNC_H
#include "ricpp/subdivfunc.h"
#endif

#ifndef _RICPP_RICPP_ERRORHANDLERS_H
#include "ricpp/errorhandlers.h"
#endif

#ifndef _RICPP_RICPP_RENDERERERROR_H
#include "ricpp/renderererror.h"
#endif

#ifndef _RICPP_RICPP_RENDERERCREATOR_H
#include "ricpp/renderercreator.h"
#endif

#include <vector>
#include <stdarg.h>
#include <assert.h>

namespace RiCPP {

/** RenderMan Interface Bridge (final)
 */
class CRiCPPBridge : public IRi {
private:
	/** Build in filters
	 */
	const CBoxFilter m_boxFilter;
	const CCatmullRomFilter m_catmullRomFilter;
	const CGaussianFilter m_gaussianFilter;
	const CSincFilter m_sincFilter;
	const CTriangleFilter m_triangleFilter;

	/** Build in error handlers
	 */
	const CAbortErrorHandler m_abortErrorHandler;
	const CIgnoreErrorHandler m_ignoreErrorHandler;
	const CPrintErrorHandler m_printErrorHandler;

	/** Current error handler
	 */
	const IErrorHandler *m_curErrorHandler; 

	/** Build in procedurals
	 */
	const CProcDelayedReadArchive m_procDelayedReadArchive;
	const CProcRunProgram m_procRunProgram;
	const CProcDynamicLoad m_procDynamicLoad;
	const CProcFreeFunc m_procFreeFunc;

	/** Token list cache
	 */
	std::vector<RtToken> m_tokens;		///<< The tokens of the parameter list of an interface call
	std::vector<RtPointer> m_params;	///<< The values of the parameter list of an interface call

protected:
	/** Extracts all token-value pairs of an interface call (...)
	 *  and stores them at m_tokens, m_params
	 */
	RtInt getTokens(RtToken token, va_list marker);

	/** Render context
	 *  Since multiple renderes can be loaded: A reference to the renderer
	 *  and the RtContextHandle of this renderer
	 */
	class CContext {
	private:
		IRiRenderer *m_renderer;
		RtContextHandle m_handle;
		bool m_valid;
		bool m_aborted;
	public:
		inline CContext() :
			m_renderer((IRiRenderer *)0),
			m_handle((RtContextHandle)0),
			m_valid(false),
			m_aborted(false)
		{
		}
		inline CContext(IRiRenderer *aRenderer, RtContextHandle aHandle) :
			m_renderer(aRenderer),
			m_handle(aHandle),
			m_valid(aRenderer != NULL && aHandle != illContextHandle),
			m_aborted(false)
		{
		}
		inline CContext(const CContext &ctx)
		{
			*this = ctx;
		}
		inline ~CContext() {
			m_valid = false;
		}
		inline void set(IRiRenderer *aRenderer, RtContextHandle aHandle, bool isValid)
		{
			m_renderer = aRenderer;
			m_handle = aHandle;
			m_valid = isValid;
		}
		inline IRiRenderer *renderer() const {return m_valid ? m_renderer : NULL; }
		inline RtContextHandle handle() const {return m_valid ? m_handle : (RtContextHandle)RI_NULL; }
		inline bool valid() const {
			return m_valid && !m_aborted &&
				m_renderer != NULL &&
				m_handle != (RtContextHandle)RI_NULL;
		}
		inline void invalidate() { m_valid = false; }

		inline bool aborted() const {
			return m_aborted;
		}
		inline void abort() {
			m_aborted = true;
		}

		inline CContext &operator=(const CContext &ctx) {
			if ( &ctx == this )
				return *this;
			m_renderer = ctx.m_renderer;
			m_handle = ctx.m_handle;
			m_valid = ctx.m_valid;
			m_aborted = ctx.m_aborted;
			return *this;
		}
	}; // class CContext

private:
	unsigned long m_ctxIdx;
	std::vector<CContext> m_ctxVect;

protected:
	inline CContext &curCtx() {return m_ctxVect[m_ctxIdx];}
	inline void pushContext(const CContext &ctx) {
		m_ctxVect.push_back(ctx);
		m_ctxIdx = (unsigned long)(m_ctxVect.size()-1);
	}
	inline void pushInvalidContext() {
		CContext ctx;
		pushContext(ctx);
	}

	/** Error Handling
	 */
	RtInt m_lastError;
	RtVoid handleError(RtInt code, RtInt severity, RtString message, ...);
	RtVoid handleErrorV(RtInt code, RtInt severity, RtString message, va_list argList);

	/** Current renderer creator
	 */
	bool m_deleteRendererCreator;
	IRendererCreator *m_curRendererCreator; 

	/** Renderer creation
	 */
	inline virtual IRiRenderer *beginRenderer(RtString name) {
		if ( m_curRendererCreator )
			return m_curRendererCreator->beginRenderer(name);
		return NULL;
	}

	/** Renderer ending, normally nothing needs to be done.
	 *  Do not throw an exception here
	 */
	inline virtual void endRenderer(IRiRenderer *renderer) {
		assert(!curCtx().aborted());
		if ( m_curRendererCreator )
			m_curRendererCreator->endRenderer(renderer);
	}

	/** Renderer abords, called if a severe (RIE_SEVERE) error occurs
	 *  if an renderer is aborted endRenderer() will not be called
	 */
	inline virtual void abortRenderer(IRiRenderer *renderer) {
		curCtx().abort();
		if ( m_curRendererCreator )
			m_curRendererCreator->abortRenderer(renderer);
	}

	/** Options for renderer creator
	 */
	inline virtual RtVoid doOptionV(RtString name, RtInt n, RtToken tokens[], RtPointer params[]) {
		if ( m_curRendererCreator )
			m_curRendererCreator->doOptionV(name, n, tokens, params);
	}

public:
	CRiCPPBridge();
	CRiCPPBridge(IRendererCreator &creator);
	virtual ~CRiCPPBridge();

	inline virtual const IFilterFunc &boxFilter() const { return m_boxFilter; }
	inline virtual const IFilterFunc &catmullRomFilter() const { return m_catmullRomFilter; }
	inline virtual const IFilterFunc &gaussianFilter() const { return m_gaussianFilter; }
	inline virtual const IFilterFunc &sincFilter() const { return m_sincFilter; }
	inline virtual const IFilterFunc &triangleFilter() const { return m_triangleFilter; }

	inline virtual const ISubdivFunc &procDelayedReadArchive() const { return m_procDelayedReadArchive; }
	inline virtual const ISubdivFunc &procRunProgram() const { return m_procRunProgram; }
	inline virtual const ISubdivFunc &procDynamicLoad() const { return m_procDynamicLoad; }
	inline virtual const IFreeFunc &procFreeFunc() const { return m_procFreeFunc; }

	inline virtual const IErrorHandler &errorAbort() const { return m_abortErrorHandler; }
	inline virtual const IErrorHandler &errorIgnore() const { return m_ignoreErrorHandler; }
	inline virtual const IErrorHandler &errorPrint() const {  return m_printErrorHandler; }

	inline virtual RtInt lastError() { return m_lastError; }
	inline virtual RtVoid errorHandler(const IErrorHandler &handler) {
		m_curErrorHandler = &handler;
	}

	virtual RtToken declare(RtString name, RtString declaration);
	virtual RtVoid synchronize(RtToken name);
	virtual RtContextHandle getContext(void);
	virtual RtVoid context(RtContextHandle handle);

	virtual RtVoid begin(RtString name);
	virtual RtVoid end(void);

	virtual RtVoid frameBegin(RtInt number);
	virtual RtVoid frameEnd(void);

	virtual RtVoid worldBegin(void);
	virtual RtVoid worldEnd(void);

	virtual RtVoid attributeBegin(void);
	virtual RtVoid attributeEnd(void);

	virtual RtVoid transformBegin(void);
	virtual RtVoid transformEnd(void);

	virtual RtVoid solidBegin(RtToken type);
	virtual RtVoid solidEnd(void);

	virtual RtObjectHandle objectBegin(void);
	virtual RtVoid objectEnd(void);

	virtual RtVoid objectInstance(RtObjectHandle handle);

	virtual RtVoid motionBegin(RtInt N, RtFloat sample, ...);
	virtual RtVoid motionBeginV(RtInt N, RtFloat times[]);

	virtual RtVoid motionEnd(void);

	/******************************************************************************/

	virtual RtVoid format(RtInt xres, RtInt yres, RtFloat aspect);
    virtual RtVoid frameAspectRatio(RtFloat aspect);
    virtual RtVoid screenWindow(RtFloat left, RtFloat right, RtFloat bot, RtFloat top);
    virtual RtVoid cropWindow(RtFloat xmin, RtFloat xmax, RtFloat ymin, RtFloat ymax);

	virtual RtVoid projection(RtString name, RtToken token = RI_NULL, ...);
    virtual RtVoid projectionV(RtString name, RtInt n, RtToken tokens[], RtPointer params[]);

	virtual RtVoid clipping(RtFloat hither, RtFloat yon);
    virtual RtVoid clippingPlane(RtFloat x, RtFloat y, RtFloat z, RtFloat nx, RtFloat ny, RtFloat nz);
    virtual RtVoid depthOfField(RtFloat fstop, RtFloat focallength, RtFloat focaldistance);
    virtual RtVoid shutter(RtFloat smin, RtFloat smax);
	virtual RtVoid pixelVariance(RtFloat variation);
    virtual RtVoid pixelSamples(RtFloat xsamples, RtFloat ysamples);
    virtual RtVoid pixelFilter(const IFilterFunc &function, RtFloat xwidth, RtFloat ywidth);
    virtual RtVoid exposure(RtFloat gain, RtFloat gamma);

    virtual RtVoid imager(RtString name, RtToken token = RI_NULL, ...);
    virtual RtVoid imagerV(RtString name, RtInt n, RtToken tokens[], RtPointer params[]);

	virtual RtVoid quantize(RtToken type, RtInt one, RtInt qmin, RtInt qmax, RtFloat ampl);

	virtual RtVoid display(RtString name, RtToken type, RtToken mode, RtToken token = RI_NULL, ...);
    virtual RtVoid displayV(RtString name, RtToken type, RtToken mode, RtInt n, RtToken tokens[], RtPointer params[]);

	virtual RtVoid hider(RtToken type, RtToken token = RI_NULL, ...);
    virtual RtVoid hiderV(RtToken type, RtInt n, RtToken tokens[], RtPointer params[]);

	virtual RtVoid colorSamples(RtInt N, RtFloat *nRGB, RtFloat *RGBn);

	virtual RtVoid relativeDetail(RtFloat relativedetail);

	virtual RtVoid option(RtString name, RtToken token = RI_NULL, ...);
	virtual RtVoid optionV(RtString name, RtInt n, RtToken tokens[], RtPointer params[]);

	/******************************************************************************/

	virtual RtLightHandle lightSource(RtString name, RtToken token = RI_NULL, ...);
    virtual RtLightHandle lightSourceV(RtString name, RtInt n, RtToken tokens[], RtPointer params[]);
	virtual RtLightHandle areaLightSource(RtString name, RtToken token = RI_NULL, ...);
	virtual RtLightHandle areaLightSourceV(RtString name, RtInt n, RtToken tokens[], RtPointer params[]);

	/******************************************************************************/

	virtual RtVoid attribute(RtString name, RtToken token = RI_NULL, ...);
    virtual RtVoid attributeV(RtString name, RtInt n, RtToken tokens[], RtPointer params[]);

	virtual RtVoid color(RtColor Cs);
	virtual RtVoid opacity(RtColor Cs);

	virtual RtVoid surface(RtString name, RtToken token = RI_NULL, ...);
    virtual RtVoid surfaceV(RtString name, RtInt n, RtToken tokens[], RtPointer params[]);
    virtual RtVoid atmosphere(RtString name, RtToken token = RI_NULL, ...);
    virtual RtVoid atmosphereV(RtString name, RtInt n, RtToken tokens[], RtPointer params[]);
	virtual RtVoid interior(RtString name, RtToken token = RI_NULL, ...);
    virtual RtVoid interiorV(RtString name, RtInt n, RtToken tokens[], RtPointer params[]);
	virtual RtVoid exterior(RtString name, RtToken token = RI_NULL, ...);
	virtual RtVoid exteriorV(RtString name, RtInt n, RtToken tokens[], RtPointer params[]);

	virtual RtVoid illuminate(RtLightHandle light, RtBoolean onoff);

	virtual RtVoid displacement(RtString name, RtToken token = RI_NULL, ...);
    virtual RtVoid displacementV(RtString name, RtInt n, RtToken tokens[], RtPointer params[]);

	virtual RtVoid textureCoordinates(RtFloat s1, RtFloat t1, RtFloat s2, RtFloat t2, RtFloat s3, RtFloat t3, RtFloat s4, RtFloat t4);
    virtual RtVoid shadingRate(RtFloat size);
	virtual RtVoid shadingInterpolation(RtToken type);
    virtual RtVoid matte(RtBoolean onoff);
	virtual RtVoid bound(RtBound aBound);
	virtual RtVoid detail(RtBound aBound);
	virtual RtVoid detailRange(RtFloat minvis, RtFloat lowtran, RtFloat uptran, RtFloat maxvis);
    virtual RtVoid geometricApproximation(RtToken type, RtFloat value);
	virtual RtVoid geometricRepresentation(RtToken type);
	virtual RtVoid orientation(RtToken anOrientation);
	virtual RtVoid reverseOrientation(void);
	virtual RtVoid sides(RtInt nsides);
    virtual RtVoid basis(RtBasis ubasis, RtInt ustep, RtBasis vbasis, RtInt vstep);
    virtual RtVoid trimCurve(RtInt nloops, RtInt *ncurves, RtInt *order, RtFloat *knot, RtFloat *amin, RtFloat *amax, RtInt *n, RtFloat *u, RtFloat *v, RtFloat *w);

	/******************************************************************************/

	virtual RtVoid identity(void);
	virtual RtVoid transform(RtMatrix aTransform);
	virtual RtVoid concatTransform(RtMatrix aTransform);
	virtual RtVoid perspective(RtFloat fov);
	virtual RtVoid translate(RtFloat dx, RtFloat dy, RtFloat dz);
	virtual RtVoid rotate(RtFloat angle, RtFloat dx, RtFloat dy, RtFloat dz);
	virtual RtVoid scale(RtFloat dx, RtFloat dy, RtFloat dz);
    virtual RtVoid skew(RtFloat angle, RtFloat dx1, RtFloat dy1, RtFloat dz1, RtFloat dx2, RtFloat dy2, RtFloat dz2);

	virtual RtVoid deformation(RtString name, RtToken token = RI_NULL, ...);
	virtual RtVoid deformationV(RtString name, RtInt n, RtToken tokens[], RtPointer params[]);

	virtual RtVoid coordinateSystem(RtToken space);
	virtual RtVoid coordSysTransform(RtToken space);
	virtual RtPoint *transformPoints(RtToken fromspace, RtToken tospace, RtInt npoints, RtPoint points[]);

	/******************************************************************************/
	
	virtual RtVoid polygon(RtInt nvertices, RtToken token = RI_NULL, ...);
    virtual RtVoid polygonV(RtInt nvertices, RtInt n, RtToken tokens[], RtPointer params[]);
	virtual RtVoid generalPolygon(RtInt nloops, RtInt *nverts, RtToken token = RI_NULL, ...);
	virtual RtVoid generalPolygonV(RtInt nloops, RtInt *nverts, RtInt n, RtToken tokens[], RtPointer params[]);
	virtual RtVoid pointsPolygons(RtInt npolys, RtInt *nverts, RtInt *verts, RtToken token = RI_NULL, ...);
	virtual RtVoid pointsPolygonsV(RtInt npolys, RtInt *nverts, RtInt *verts, RtInt n, RtToken tokens[], RtPointer params[]);
	virtual RtVoid pointsGeneralPolygons(RtInt npolys, RtInt *nloops, RtInt *nverts, RtInt *verts, RtToken token = RI_NULL, ...);
    virtual RtVoid pointsGeneralPolygonsV(RtInt npolys, RtInt *nloops, RtInt *nverts, RtInt *verts,  RtInt n, RtToken tokens[], RtPointer params[]);

	virtual RtVoid patch(RtToken type, RtToken token = RI_NULL, ...);
    virtual RtVoid patchV(RtToken type, RtInt n, RtToken tokens[], RtPointer params[]);
    virtual RtVoid patchMesh(RtToken type, RtInt nu, RtToken uwrap, RtInt nv, RtToken vwrap, RtToken token = RI_NULL, ...);
    virtual RtVoid patchMeshV(RtToken type, RtInt nu, RtToken uwrap, RtInt nv, RtToken vwrap, RtInt n, RtToken tokens[], RtPointer params[]);
	virtual RtVoid nuPatch(RtInt nu, RtInt uorder, RtFloat *uknot, RtFloat umin, RtFloat umax, RtInt nv, RtInt vorder, RtFloat *vknot, RtFloat vmin, RtFloat vmax, RtToken token = RI_NULL, ...);
    virtual RtVoid nuPatchV(RtInt nu, RtInt uorder, RtFloat *uknot, RtFloat umin, RtFloat umax, RtInt nv, RtInt vorder, RtFloat *vknot, RtFloat vmin, RtFloat vmax,  RtInt n, RtToken tokens[], RtPointer params[]);

	virtual RtVoid subdivisionMesh(RtToken scheme, RtInt nfaces, RtInt nvertices[], RtInt vertices[], RtInt ntags, RtToken tags[], RtInt nargs[], RtInt intargs[], RtFloat floatargs[], RtToken token = RI_NULL, ...);
    virtual RtVoid subdivisionMeshV(RtToken scheme, RtInt nfaces, RtInt nvertices[], RtInt vertices[], RtInt ntags, RtToken tags[], RtInt nargs[], RtInt intargs[], RtFloat floatargs[],  RtInt n, RtToken tokens[], RtPointer params[]);

	virtual RtVoid sphere(RtFloat radius, RtFloat zmin, RtFloat zmax, RtFloat thetamax, RtToken token = RI_NULL, ...);
    virtual RtVoid sphereV(RtFloat radius, RtFloat zmin, RtFloat zmax, RtFloat thetamax, RtInt n, RtToken tokens[], RtPointer params[]);
	virtual RtVoid cone(RtFloat height, RtFloat radius, RtFloat thetamax, RtToken token = RI_NULL, ...);
    virtual RtVoid coneV(RtFloat height, RtFloat radius, RtFloat thetamax, RtInt n, RtToken tokens[], RtPointer params[]);
	virtual RtVoid cylinder(RtFloat radius, RtFloat zmin, RtFloat zmax, RtFloat thetamax, RtToken token = RI_NULL, ...);
    virtual RtVoid cylinderV(RtFloat radius, RtFloat zmin, RtFloat zmax, RtFloat thetamax, RtInt n, RtToken tokens[], RtPointer params[]);
	virtual RtVoid hyperboloid(RtPoint point1, RtPoint point2, RtFloat thetamax, RtToken token = RI_NULL, ...);
    virtual RtVoid hyperboloidV(RtPoint point1, RtPoint point2, RtFloat thetamax, RtInt n, RtToken tokens[], RtPointer params[]);
	virtual RtVoid paraboloid (RtFloat rmax, RtFloat zmin, RtFloat zmax, RtFloat thetamax, RtToken token = RI_NULL, ...);
    virtual RtVoid paraboloidV(RtFloat rmax, RtFloat zmin, RtFloat zmax, RtFloat thetamax, RtInt n, RtToken tokens[], RtPointer params[]);
	virtual RtVoid disk(RtFloat height, RtFloat radius, RtFloat thetamax, RtToken token = RI_NULL, ...);
    virtual RtVoid diskV(RtFloat height, RtFloat radius, RtFloat thetamax, RtInt n, RtToken tokens[], RtPointer params[]);
	virtual RtVoid torus(RtFloat majorrad, RtFloat minorrad, RtFloat phimin, RtFloat phimax, RtFloat thetamax, RtToken token = RI_NULL, ...);
    virtual RtVoid torusV(RtFloat majorrad, RtFloat minorrad, RtFloat phimin, RtFloat phimax, RtFloat thetamax, RtInt n, RtToken tokens[], RtPointer params[]);

	virtual RtVoid points(RtInt npts, RtToken token = RI_NULL, ...);
    virtual RtVoid pointsV(RtInt npts, RtInt n, RtToken tokens[], RtPointer params[]);
    virtual RtVoid curves(RtToken type, RtInt ncurves, RtInt nverts[], RtToken wrap, RtToken token = RI_NULL, ...);
    virtual RtVoid curvesV(RtToken type, RtInt ncurves, RtInt nverts[], RtToken wrap, RtInt n, RtToken tokens[], RtPointer params[]);

	virtual RtVoid blobby(RtInt nleaf, RtInt ncode, RtInt code[], RtInt nflt, RtFloat flt[], RtInt nstr, RtString str[], RtToken token = RI_NULL, ...);
    virtual RtVoid blobbyV(RtInt nleaf, RtInt ncode, RtInt code[], RtInt nflt, RtFloat flt[], RtInt nstr, RtString str[], RtInt n, RtToken tokens[], RtPointer params[]);

	virtual RtVoid procedural(RtPointer data, RtBound bound, const ISubdivFunc &subdivfunc, const IFreeFunc &freefunc);

	virtual RtVoid geometry(RtToken type, RtToken token = RI_NULL, ...);
    virtual RtVoid geometryV(RtToken type, RtInt n, RtToken tokens[], RtPointer params[]);

	/******************************************************************************/

	virtual RtVoid makeTexture(RtString pic, RtString tex, RtToken swrap, RtToken twrap, const IFilterFunc &filterfunc, RtFloat swidth, RtFloat twidth, RtToken token = RI_NULL, ...);
    virtual RtVoid makeTextureV(RtString pic, RtString tex, RtToken swrap, RtToken twrap, const IFilterFunc &filterfunc, RtFloat swidth, RtFloat twidth, RtInt n, RtToken tokens[], RtPointer params[]);
    virtual RtVoid makeBump(RtString pic, RtString tex, RtToken swrap, RtToken twrap, const IFilterFunc &filterfunc, RtFloat swidth, RtFloat twidth, RtToken token = RI_NULL, ...);
    virtual RtVoid makeBumpV(RtString pic, RtString tex, RtToken swrap, RtToken twrap, const IFilterFunc &filterfunc, RtFloat swidth, RtFloat twidth, RtInt n, RtToken tokens[], RtPointer params[]);
    virtual RtVoid makeLatLongEnvironment(RtString pic, RtString tex, const IFilterFunc &filterfunc, RtFloat swidth, RtFloat twidth, RtToken token = RI_NULL, ...);
    virtual RtVoid makeLatLongEnvironmentV(RtString pic, RtString tex, const IFilterFunc &filterfunc, RtFloat swidth, RtFloat twidth, RtInt n, RtToken tokens[], RtPointer params[]);
    virtual RtVoid makeCubeFaceEnvironment(RtString px, RtString nx, RtString py, RtString ny, RtString pz, RtString nz, RtString tex, RtFloat fov, const IFilterFunc &filterfunc, RtFloat swidth, RtFloat twidth, RtToken token = RI_NULL, ...);
    virtual RtVoid makeCubeFaceEnvironmentV(RtString px, RtString nx, RtString py, RtString ny, RtString pz, RtString nz, RtString tex, RtFloat fov, const IFilterFunc &filterfunc, RtFloat swidth, RtFloat twidth, RtInt n, RtToken tokens[], RtPointer params[]);
    virtual RtVoid makeShadow(RtString pic, RtString tex, RtToken token = RI_NULL, ...);
    virtual RtVoid makeShadowV(RtString pic, RtString tex, RtInt n, RtToken tokens[], RtPointer params[]);

	/******************************************************************************/

	virtual RtVoid archiveRecord(RtToken type, RtString format, ...);
	virtual RtVoid archiveRecordV(RtToken type, RtString line);
    virtual RtVoid readArchive(RtString name, const IArchiveCallback *callback, RtToken token = RI_NULL, ...);
	virtual RtVoid readArchiveV(RtString name, const IArchiveCallback *callback, RtInt n, RtToken tokens[], RtPointer params[]);

	// Not in use, just because of compiler warning
	inline CRiCPPBridge &operator=(const CRiCPPBridge &bridge) {
		if ( &bridge == this )
			return *this;
		return *this;
	}
}; // CRiCPPBridge

} // namespace RiCPP

#endif // _RICPP_RICPP_RICPPBRIDGE_H
