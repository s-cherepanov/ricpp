#ifndef _RICPP_RICPP_RICPPBRIDGE_H
#define _RICPP_RICPP_RICPPBRIDGE_H

// RICPP - RenderMan(R) Interface CPP Language Binding
//
//     RenderMan(R) is a registered trademark of Pixar
// The RenderMan(R) Interface Procedures and Protocol are:
//         Copyright 1988, 1989, 200,, 2005 Pixar
//                 All rights Reservered
//
// Copyright � of RiCPP 2007, Andreas Pidde
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

/** @file ricppbridge.h
 *  @author Andreas Pidde (andreas@pidde.de)
 *  @brief Bridges the interface functions to a current renderer
 *
 *     RenderMan(R) is a registered trademark of Pixar
 * The RenderMan(R) Interface Procedures and Protocol are:
 *         Copyright 1988, 1989, 2000, 2005 Pixar
 *                 All rights Reservered
 */

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
 * Jobs: Bridges to a renderer implementing the Ri.
 * Calls The Ri..V() routines, forwards the global options
 * (Option "searchpath" "renderer") to the renderer creator,
 * does the error handling (RiError) by
 * catching the errors ERendererError of the current renderer, 
 * matching a context number to a renderer with its context
 * (forwarding RiContext, RiGetContext). Normally only a single
 * instance of CRiCPPBridge is used.
 */
class CRiCPPBridge : public IRi {
private:
	//@{
	/** Build in filters
	 */
	const CBoxFilter m_boxFilter; ///< The Box pixel filter
	const CCatmullRomFilter m_catmullRomFilter; ///< The Catmull-Rom pixel filter
	const CGaussianFilter m_gaussianFilter; ///< The Gaussian pixel filter
	const CSincFilter m_sincFilter; ///< The Sinc pixel filter
	const CTriangleFilter m_triangleFilter; ///< The Triangle pixel filter
	//@}

	//@{
	/** Build in error handlers, can be used by CRiCPPBridge::error()
	 */
	const CAbortErrorHandler m_abortErrorHandler; ///< Prints error message to standard output and exits if the error is severe
	const CIgnoreErrorHandler m_ignoreErrorHandler; ///< Ignores the error, does nothing
	const CPrintErrorHandler m_printErrorHandler; ///< Prints error message to standard output
	//@}

	/** Current error handler, m_printErrorHandler is the
	 *  default handler
	 */
	const IErrorHandler *m_curErrorHandler; 

	//@{
	/** Build in procedurals
	 */
	const CProcDelayedReadArchive m_procDelayedReadArchive; ///< Reads a RIB archive
	const CProcRunProgram m_procRunProgram; ///< Runs a program, pipes it output to the renderer
	const CProcDynamicLoad m_procDynamicLoad; ///< Runs a procedure of a DLL passes 'this' as current renderer
	const CProcFree m_procFree; ///< Cleanup function for the data used by the procedurals
	//@}

	//@{
	/** Token list cache
	 */
	std::vector<RtToken> m_tokens;		///<< The tokens of the parameter list of an interface call
	std::vector<RtPointer> m_params;	///<< The values of the parameter list of an interface call
	//@}

protected:
	/** Extracts all token-value pairs of an (...) interface call
	 *  and stores them at m_tokens, m_params
	 *  @param token First token of the token-value list
	 *  @marker va_start() marker
	 *  @return Number of token value pairs found
	 */
	RtInt getTokens(RtToken token, va_list marker);

	/** Holds a render and its context
	 *  Since multiple renderes can be loaded: A reference to the renderer
	 *  and the RtContextHandle of this renderer
	 */
	class CContext {
	private:
		/** References a renderer, NULL if no renderer is set
		 */
		IRiRenderer *m_renderer;

		/** References a context of the renderer m_renderer, illContextHandle if
		 * no handle is set
		 */
		RtContextHandle m_handle;

		/** True if the context is valid, the context is invalid after the matching RiEnd() is called
		 */
		bool m_valid;

		/** True if the context is aborted. If a 'severe' error occurs, the render context gets
		 *  aborted and no further rendering or error handling is done in this context (this is independend
		 * of the current error handler)
		 */
		bool m_aborted;
	public:
		/** The standard contructor initializes an empty, invalid instance
		 */
		inline CContext() :
			m_renderer((IRiRenderer *)0),
			m_handle((RtContextHandle)0),
			m_valid(false),
			m_aborted(false)
		{
		}

		/** Construct an instance with a renderer and its context.
		 * The instance is invalidated if the renderer is not set or
		 * the context handle is invalid. The renderer can be referenced
		 * by many CContext instances.
		 * @param aRenderer renderer implementing the Ri
		 * @param aHandle a andle of aRenderer
		 */
		inline CContext(IRiRenderer *aRenderer, RtContextHandle aHandle) :
			m_renderer(aRenderer),
			m_handle(aHandle),
			m_valid(aRenderer != NULL && aHandle != illContextHandle),
			m_aborted(false)
		{
		}

		/** Copy constructor
		 */
		inline CContext(const CContext &ctx)
		{
			*this = ctx;
		}

		/** Destructor, sets m_valid = false for savety reasons,
		 *  the renderer is not destructed here. The renderes should be
		 *  deleted by the renderer creator (see CRendererLoader)
		 */
		inline ~CContext() {
			m_valid = false;
		}

		/** Gets the assigned renderer
		 *  @return The renderer
		 */
		inline IRiRenderer *renderer() const {return m_valid ? m_renderer : NULL; }

		/** Gets the assigned renderer handle
		 *  @return The renderer handle
		 */
		inline RtContextHandle handle() const {return m_valid ? m_handle : (RtContextHandle)RI_NULL; }

		/** The instance is invalid either if the renderer is NULL, the handle is invalid or
		 *  the instance is explicitly invalidated.
		 *  @return The validy state
		 */
		inline bool valid() const {
			return m_valid && !m_aborted &&
				m_renderer != NULL &&
				m_handle != illContextHandle;
		}

		/** Sets the structure in an invalid state (done in CRiCPPBridge::end())
		 */
		inline void invalidate() { m_valid = false; }

		/** Query if the context is aborted
		 * @return true if the renderer state is aborted
		 */
		inline bool aborted() const {
			return m_aborted;
		}

		/** The context can be aborted by the bridge if a severe error occurse
		 */
		inline void abort() {
			m_aborted = true;
		}

		/** Assigns a context
		 * @param ctx Context to assign
		 * @return A reference to *this
		 */
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
	//@{
	/** The used renderer, context pairs, content is destroyed in the destructor
	 * of CRiCPPBridge, Conexts only gets invalidated by CRiCPPBridge::end()
	 */
	unsigned long m_ctxIdx; //< current renderer, context pair
	std::vector<CContext> m_ctxVect; //< All renderer, context pairs used
	//@}

protected:
	//@{
	/** Context handling
	 */
	/** Helper, returns the current renderer, context pair
	 * @return A reference to the current renderer, context pair
	 */
	inline CContext &curCtx() {return m_ctxVect[m_ctxIdx];}

	/** Pushes a renderer, context pair to the context vector
	 * @param ctx A renderer, context pair to be stored
	 */
	inline void pushContext(const CContext &ctx) {
		m_ctxVect.push_back(ctx);
		m_ctxIdx = (unsigned long)(m_ctxVect.size()-1);
	}

	/** Pushes an invalid context. The first context (before a begin() is called, after end()) is
	 *  invalid every time
	 */
	inline void pushInvalidContext() {
		CContext ctx;
		pushContext(ctx);
	}
	//@}

	//@{
	/** Error Handling
	 */
	RtInt m_lastError; //< The last error number occured, stored by handleErrorV()

	/** Forward to handleErrorV(). Variable parameters like in printf()
	 * @param code Error Code (RIE_...)
	 * @param Severity level of the error (RIE_INFO, ..., RIE_SEVERE)
	 * @param message Format string (like in printf())
	 */
	RtVoid handleError(RtInt code, RtInt severity, RtString message, ...);

	/** Handles an error, sets m_lastError and calls the current error handler
	 * @param code Error Code (RIE_...)
	 * @param Severity level of the error (RIE_INFO, ..., RIE_SEVERE)
	 */
	RtVoid handleErrorV(RtInt code, RtInt severity, RtString message, va_list argList);
	//@}

	//@{
	/** Current renderer creator, the renderer creator creats an instance of a renderer implementing the Ri
	 */
	bool m_deleteRendererCreator; //< Destroy m_curRendererCreator at the destructor of the bridge
	IRendererCreator *m_curRendererCreator; //< A renderer creator, default is a CRendererLoader
	//@}

	/** Renderer creation
	 * @name Copy of CRendererLoader::begin(name)
	 */
	inline virtual IRiRenderer *beginRenderer(RtString name) {
		if ( m_curRendererCreator )
			return m_curRendererCreator->beginRenderer(name);
		return NULL;
	}

	/** Renderer ending, normally nothing needs to be done.
	 *  Do not throw an exception here. The context should not be aborted, since then
	 *  the renderer in this context is already finished by abortRenderer().
	 *  @renderer The renderer to finish
	 */
	inline virtual void endRenderer(IRiRenderer *renderer) {
		assert(!curCtx().aborted());
		if ( m_curRendererCreator )
			m_curRendererCreator->endRenderer(renderer);
	}

	/** Renderer aborts, called if a severe (RIE_SEVERE) error occurs.
	 *  If an renderer is aborted, endRenderer() will not be called any more
	 *  @renderer The renderer to abort
	 */
	inline virtual void abortRenderer(IRiRenderer *renderer) {
		curCtx().abort();
		if ( m_curRendererCreator )
			m_curRendererCreator->abortRenderer(renderer);
	}

	/** Options for renderer creator, like RiOption but only concerns the m_curRendererCreator.
	 * Forwarded option() calls before the first begin() and between end() and begin().
	 * @param name Option name (likely "searchpath" for the renderer
	 * @param n Number token-value pairs
	 * @param Tokens Tokens
	 * @param params Parameter values
	 */
	inline virtual RtVoid doOptionV(RtString name, RtInt n, RtToken tokens[], RtPointer params[]) {
		if ( m_curRendererCreator )
			m_curRendererCreator->doOptionV(name, n, tokens, params);
	}

	/** Creates a bridge, with a different renderer creator. The creator is not destroyed
	 * by the destructor. The pointer to creator is stored. The rest like CRiCPPBridge().
	 * Can be used from constructors of children of CRiCPPBridge:
	 * CRiCPPChildBridge() : CRiCPPBridge(&m_myRendererLoader) {}
	 * @param creator, a different renderer creator
	 */
	CRiCPPBridge(IRendererCreator &creator);

public:
	/** Creates a bridge, a CRendererLoader is used as m_curRendererCreator, m_printErrorHandler
	 * is used as error handler, an invalid context is stored
	 */
	CRiCPPBridge();

	/** Destructor, deletes the m_curRendererCreator only if created by the constructor CRiCPPBridge()
	 */
	virtual ~CRiCPPBridge();

	/** Assignment, not in use, just because of compiler warning
	 * @param bridge
	 * @return *this
	 */
	inline CRiCPPBridge &operator=(const CRiCPPBridge &bridge) {
		if ( &bridge == this )
			return *this;
		return *this;
	}

	//@{
	/** Returns the appropriate filter functions
     */
	inline virtual const IFilterFunc &boxFilter() const { return m_boxFilter; }
	inline virtual const IFilterFunc &catmullRomFilter() const { return m_catmullRomFilter; }
	inline virtual const IFilterFunc &gaussianFilter() const { return m_gaussianFilter; }
	inline virtual const IFilterFunc &sincFilter() const { return m_sincFilter; }
	inline virtual const IFilterFunc &triangleFilter() const { return m_triangleFilter; }
	//@}

	//@{
	/** Returns the appropriate procedurals
     */
	inline virtual const ISubdivFunc &procDelayedReadArchive() const { return m_procDelayedReadArchive; }
	inline virtual const ISubdivFunc &procRunProgram() const { return m_procRunProgram; }
	inline virtual const ISubdivFunc &procDynamicLoad() const { return m_procDynamicLoad; }
	inline virtual const IFreeFunc &procFree() const { return m_procFree; }
	//@}

	//@{
	/** Returns the appropriate error handlers
     */
	inline virtual const IErrorHandler &errorAbort() const { return m_abortErrorHandler; }
	inline virtual const IErrorHandler &errorIgnore() const { return m_ignoreErrorHandler; }
	inline virtual const IErrorHandler &errorPrint() const {  return m_printErrorHandler; }
	//@}

	/** The last error is set by handleErrorV()
	 * @return Recent error number
     */
	inline virtual RtInt lastError() { return m_lastError; }
	/** Sets the current error handler, default is errorPrint()
	 *  @param handler reference to an error handler, pointer is stored
	 */
	inline virtual RtVoid errorHandler(const IErrorHandler &handler) {
		m_curErrorHandler = &handler;
	}

	//@{
	/** The rest of the interface functions see ricpp.h. The functions with variable length parameters
	 *  are forwarded to the ..V() functions, these are forwarded to the current renderer
	 *  (only optionV() may be forwared to a renderer loader, if if there is no active renderer)
	 *  Errors are catched and the current error handler is called with the error found.
     */
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
	//@}
}; // CRiCPPBridge

} // namespace RiCPP

#endif // _RICPP_RICPP_RICPPBRIDGE_H
