#ifndef _RICPP_RICPPBRIDGE_RICPPBRIDGE_H
#define _RICPP_RICPPBRIDGE_RICPPBRIDGE_H

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

/** @file ricppbridge.h
 *  @author Andreas Pidde (andreas@pidde.de)
 *  @brief Bridges the interface functions to a current renderer
 */

#ifndef _RICPP_RICPP_FILTERS_H
#include "ricpp/ricpp/filters.h"
#endif

#ifndef _RICPP_RICPP_SUBDIVFUNC_H
#include "ricpp/ricpp/subdivfunc.h"
#endif

#ifndef _RICPP_RENDERERLOADER_RENDERERLOADER_H
#include "ricpp/rendererloader/rendererloader.h"
#endif

#ifndef _RICPP_RICPP_ERRORHANDLERS_H
#include "ricpp/ricpp/errorhandlers.h"
#endif

#include <vector>
#include <stdarg.h>
#include <assert.h>

namespace RiCPP {

/** @brief RenderMan Interface Bridge
 * Jobs: Bridges to a renderer (context) that implements the Ri
 * and the error handlers.
 * Calls The Ri..V() routines, handles the global options
 * (Option "searchpath" "renderer"), does the error handling
 * by catching the errors ERendererError of the current renderer and
 * calling the user defined error handler set by errorHandler(), 
 * matches a context handle RtContextHandle to a context creator with
 * its rendering context (begin(), end(), context(), getContext()). Normally only
 * a single instance of CRiCPPBridge is used. However doesn't want to restrict it
 * to be a singleton.
 */
class CRiCPPBridge : public IRi, public IRiCPPErrorHandler
{
private:
	/** @brief Build in filters
	 */
	//@{
	const CBoxFilter m_boxFilter; ///< The Box pixel filter
	const CCatmullRomFilter m_catmullRomFilter; ///< The Catmull-Rom pixel filter
	const CGaussianFilter m_gaussianFilter; ///< The Gaussian pixel filter
	const CSincFilter m_sincFilter; ///< The Sinc pixel filter
	const CTriangleFilter m_triangleFilter; ///< The Triangle pixel filter
	//@}

	/** @brief Build in error handlers, can be used by CRiCPPBridge::error()
	 */
	//@{
	const CAbortErrorHandler m_abortErrorHandler; ///< Prints error message to standard output and exits if the error is severe
	const CIgnoreErrorHandler m_ignoreErrorHandler; ///< Ignores the error, does nothing
	const CPrintErrorHandler m_printErrorHandler; ///< Prints error message to standard output
	//@}

	/** @brief Error Handling
	 */
	//@{
	RtInt m_lastError; //< The last error number occured, stored by handleErrorV()

	/** @brief Current user defined error handler
	 *
	 * m_printErrorHandler is the default handler. It can be changed by errorHandler()
	 */
	const IErrorHandler *m_curErrorHandler; 

	/** @brief Handles an error, sets m_lastError and calls the current error handler
	 *
	 * Implementation of IRiCPPErrorHandler, used through virtual ricppErrHandler(),
	 * never called directly.
	 *
	 * @param code Error Code (RIE_...)
	 * @param severity Severity level of the error (RIE_INFO, ..., RIE_SEVERE)
	 * @param line Line number where error occured
	 * @param file file where error occured
	 * @param message Format string (like in printf()), not formatted if argList==NULL
	 * @param argList variable list of parameters, if 0 message is treted like a string without format symbols
	 */
	virtual RtVoid handleErrorV(RtInt code, RtInt severity, int line, const char *file, RtString message, va_list argList=0);
	//@}

	/** @brief Build in procedurals
	 */
	//@{
	const CProcDelayedReadArchive m_procDelayedReadArchive; ///< Reads a RIB archive
	const CProcRunProgram m_procRunProgram; ///< Runs a program, pipes it output to the renderer
	const CProcDynamicLoad m_procDynamicLoad; ///< Runs a procedure of a DLL passes 'this' as current renderer
	const CProcFree m_procFree; ///< Cleanup function for the data used by the procedurals
	//@}

	/** @brief Token list cache
	 */
	//@{
	std::vector<RtToken> m_tokens;		///<< The tokens of the parameter list of an interface call
	std::vector<RtPointer> m_params;	///<< The values of the parameter list of an interface call
	//@}

	CRendererLoader m_rendererCreator;  ///<< Used by CContextCreator to create context creators

	/** @brief  Assignment, not in use, just because of compiler warning
	 *
	 * @param bridge
	 * @return *this
	 */
	inline CRiCPPBridge &operator=(const CRiCPPBridge &bridge)
	{
		if ( &bridge == this )
			return *this;
		return *this;
	}

protected:
	/** @brief Extracts all token-value pairs of an (...) interface call
	 *
	 *  The paires are stored in m_tokens, m_params
	 *
	 *  @param token First token of the token-value list
	 *  @param marker va_start() marker
	 *  @return Number of token value pairs found
	 */
	RtInt getTokens(RtToken token, va_list marker);

	
	/** @brief Returns the error handler to use, can be overwritten but usually
	 *  won't in this framework.
	 *
	 *  @return *this, The bridge itself has the function of an error handler
	 */
	inline virtual IRiCPPErrorHandler &ricppErrHandler() { return *this; }

	/** @brief Context handling
	 */
	//@{

	/** @brief Holds a context creator and one of its rendering contexts.
	 *
	 * The context creator/rendering context pairs of a CRiCPPBridge instance
	 * are managed by a CContextManagement instance m_ctxMgmt.
	 */
	class CContext
	{
	private:
		/** References the context creator of the renderer m_renderer, 0 if
		 *  no handle is set
		 */
		CContextCreator *m_contextCreator;

		/** @brief References a rendering context, 0 if no renderer is set
		 */
		IRiContext *m_renderer;

		/** @brief Context aborted?
		 *
		 *  True, if the context is aborted. If a 'severe' error occurs, the render can be
		 *  aborted by an user defined error handler and no further rendering and error handling
		 *  is done in this context any more.
		 */
		bool m_aborted;

	public:
		/** @brief The standard contructor initializes an empty instance
		 *
		 * The CContext instance referenced by illContextHandle to represent the 'outside' context
		 * is used without m_contextCreator and m_renderer
		 */
		inline CContext() :
			m_contextCreator(0),
			m_renderer(0),
			m_aborted(false)
		{
		}

		/** @brief  Constructs an instance with a rendering context and its creator.
		 *
		 * A context creator can be referenced by many CContext instances.
		 * @param aCreator Context creator
		 * @param aRenderer Implementation of a rendering context, created by aCreator
		 */
		inline CContext(CContextCreator *aCreator, IRiContext *aRenderer) :
			m_contextCreator(aCreator),
			m_renderer(aRenderer),
			m_aborted(false)
		{
		}

		/** @brief Copy constructor
		 */
		inline CContext(const CContext &ctx)
		{
			*this = ctx;
		}

		/** @brief Destructor, the renderer is not destructed here.
		 *
		 *  The renderes should be
		 *  deleted by the renderer creator (see CRendererLoader)
		 */
		inline ~CContext()
		{
		}

		/** @brief Gets the assigned context creator
		 *
		 *  @return Pointer to the context creator
		 */
		inline CContextCreator *contextCreator() const
		{
			return m_contextCreator;
		}

		/** @brief Gets the assigned rendering context
		 *
		 *  Since a context creator can bear many contexts, the context creator/rendering context
		 *  pairs are stored (CContext objects in CContextManager::m_ctxMap).
		 *  *After* a CContext is activated via the
		 *  CContextManager::context() or begin() method, renderer() and
		 *  contextCreator()->renderer() are the same. Only those active contexts are
		 *  used by other functions than context() and begin().
		 *
		 *  @return Pointer to the rendering context
		 */
		inline IRiContext *renderer() const
		{
			return m_renderer;
		}

		/** @brief Checks the validity of a CContext Instance
		 *  
		 *  The instance is invalid either if the renderer is NULL, the handle is invalid or
		 *  the instance is aborted.
		 *
		 *  @return The validity state
		 */
		inline bool valid() const
		{
			return !m_aborted &&
				m_contextCreator != 0 &&
				m_renderer != 0;
		}

		/** @brief Query if the context is aborted
		 *
		 * Aborted context are not used any more, even if active. This can be used to
		 * continue with unrecoverable errors.
		 *
		 * @return true if the renderer state is aborted
		 * @see abort()
		 */
		inline bool aborted() const
		{
			return m_aborted;
		}

		/** @brief The current context can be aborted by the frontend
		 *  if a severe error occurs,
		 *  this indicates that no further errors should be print (it is
		 *  likely that these errors are follow up errors)
		 *  @see aborted()
		 */
		inline void abort()
		{
			if ( m_contextCreator != 0 )
				m_contextCreator->abort();
			m_aborted = true;
		}

		/** @brief End the current context regulary
		 */
		inline void end()
		{
			if ( m_contextCreator != 0 )
				m_contextCreator->end();
			m_renderer = 0;
		}

		/** @brief Deactivate the current context, used for context switching
		 */
		inline void deactivate()
		{
			if ( m_contextCreator != 0 )
				m_contextCreator->context(0);
		}


		/** @brief Activate the current context, used for context switching
		 */
		inline void activate()
		{
			if ( m_contextCreator != 0 )
				m_contextCreator->context(m_renderer);
		}

		/** @brief Assigns a context to another
		 *
		 * @param ctx Context to assign to *this
		 * @return A reference to *this
		 */
		inline CContext &operator=(const CContext &ctx)
		{
			if ( &ctx == this )
				return *this;
			m_contextCreator = ctx.m_contextCreator;
			m_renderer = ctx.m_renderer;
			m_aborted = ctx.m_aborted;
			return *this;
		}
	}; // class CContext

	/** @brief Context Management for CContext (context creator/rendering context pairs)
	 *
	 *  A RtContextHandle is mapped to a CContext. A RtContextHandle of the frontend
	 *  references a backend context creator with a rendering context. The CRiCPPBridge
	 *  forwards the interface calls always to the active backend rendering context. begin() and
	 *  context() are out of this rule.
	 *
	 *  The frontend illContextHandle has a special meaning: "There is no active rendering context". It
	 *  can be thought of illContextHandle representing the bridge itself, it cannot be removed.
	 */
	class CContextManagement {
		unsigned long m_nextCtxHandle; //< next value for context handle (counts 1 upwarts), deleted handles are not reused	
		RtContextHandle m_ctxHandle; //< current context handle of the front end
		CContext m_curCtx; //< current creator, context pair of the backend
		std::map<RtContextHandle, CContext> m_ctxMap; //< Maps used context handles to a CContext context creator/rendering context pair.

		/** @brief Removes a context creator/rendering context pair from the context map.
		 *
		 *  illContextHandle, the handle that represents the bridge itself cannot be removed.
		 *
		 * @param handle The RtContextHandle as key, illContextHandle is not removed
		 */
		void removeContext(RtContextHandle handle);

	public:
		/** @brief Initializes context management
		 *
		 *  The illContextHandle context is constructed.
		 */
		inline CContextManagement()
		{
			m_nextCtxHandle = 1;
			// illContextHandle is always the first context (outside begin-end)
			// m_curCtx initially has nor context creator or renderer
			m_ctxHandle = illContextHandle;
			m_ctxMap[m_ctxHandle] = m_curCtx;
		}

		/** @brief Adds a context creator
		 *
		 *  Puts the context creator / rendering context pair at the context map
		 *  with the handle as key. The handle is a number m_nextCtxHandle here
		 *  and is also increased in this method to be used as next handle.
		 *
		 * @param ctx A context creator/rendering context pair, to be stored
		 * @return The new handle of the creator
		 */
		inline RtContextHandle add(const CContext &ctx)
		{
			m_ctxMap[(RtContextHandle)m_nextCtxHandle] = ctx;
			return (RtContextHandle)(m_nextCtxHandle++);
		}

		/** @brief  Aborts the current context
		 *
		 *  The context is not removed until it is ended, so the
		 *  client can call requests until end(). But since it
		 *  is aborted no renderering and error handling will be done.
		 */
		inline void abort()
		{
			m_curCtx.abort();
		}

		/** @brief Begins a new context.
		 *
		 * Manages the creation of a new context, creates a new CContext. The old
		 * context is deactivated. To generate a new active rendering context the
		 * CContextCreator::begin(name) is called. The new context is not
		 * explicitly activated.
		 *
		 * @param name Forwarded from TRiCPPBridge::begin()
		 * @param cc Context Creator used to create the new context
		 */
		void begin(const char *name, CContextCreator *cc);

		/** @brief Ends the current context and removes it from the list.
		 *
		 * There is no active rendering context after end() ended.
		 *
		 */
		void end();

		/** @brief The active context creator/rendering context.
		 * 
		 * @return A reference to the current creator,
		 *          context pair, not neccessairly valid.
		 */
		inline CContext &curCtx()
		{
			return m_curCtx;
		}

		/** @brief The handle of the active context creator/rendering context.
		 *
		 *  @return The handle of the current creator,
		 *          context pair
		 */
		inline RtContextHandle getContext() const
		{
			return m_ctxHandle;
		}


		/** @brief Test if the context handle is in the map.
		 *
		 * @param handle A context handle
		 * @return true if the parameter handle is managed (can be illContextHandle or aborted)
		 */
		inline bool isContext(RtContextHandle handle)
		{
			return m_ctxMap.find(handle) != m_ctxMap.end();
		}

		/** @brief Sets the new current context.
		 *
		 * @param handle A context handle of the new context
		 * @return True, if the parameter handle was found and set
		 *         (normally tested before context() is called).
		 */
		inline bool context(RtContextHandle handle)
		{
			if ( isContext(handle) ) {
				m_curCtx.deactivate();
				m_ctxHandle = handle;
				m_curCtx = m_ctxMap[m_ctxHandle];
				m_curCtx.activate();
				return true;
			}
			m_ctxHandle = illContextHandle;
			m_curCtx = m_ctxMap[m_ctxHandle];
			return false;
		}
	} m_ctxMgmt; ///< The instance for the context management
	//@}

	/** @brief Current renderer creator
	 *
	 * The renderer creator loads a context creator from a
	 * dynamic library or constructs one in memory
	 * @return An object to load context creators
	 */
	IRendererCreator &rendererCreator()
	{
		return m_rendererCreator;
	}

	/** @brief Like RiOption but only concerns the bridge itself.
	 *
	 * Forwarded by optionV() if there is no active rendering context.
	 * @param name Option name (likely "searchpath" for the renderer searchpath)
	 * @param n Number token-value pairs
	 * @param tokens Tokens
	 * @param params Parameter values
	 */
	virtual RtVoid doOptionV(RtString name, RtInt n, RtToken tokens[], RtPointer params[]);

public:
	/** @brief Creates a bridge
	 *
	 * A CRendererLoader is used as m_curRendererCreator, m_printErrorHandler
	 * is used as error handler, an invalid context indexed by illContextHandle
	 * is stored as 'outside' context
	 */
	CRiCPPBridge();

	/** @brief Destructor
	 */
	inline virtual ~CRiCPPBridge() {}

	/** @brief Returns the appropriate filter functions
     */
	//@{
	inline virtual const IFilterFunc &boxFilter() const { return m_boxFilter; }
	inline virtual const IFilterFunc &catmullRomFilter() const { return m_catmullRomFilter; }
	inline virtual const IFilterFunc &gaussianFilter() const { return m_gaussianFilter; }
	inline virtual const IFilterFunc &sincFilter() const { return m_sincFilter; }
	inline virtual const IFilterFunc &triangleFilter() const { return m_triangleFilter; }
	//@}

	/** @brief Returns the appropriate procedurals
     */
	//@{
	inline virtual const ISubdivFunc &procDelayedReadArchive() const { return m_procDelayedReadArchive; }
	inline virtual const ISubdivFunc &procRunProgram() const { return m_procRunProgram; }
	inline virtual const ISubdivFunc &procDynamicLoad() const { return m_procDynamicLoad; }
	inline virtual const IFreeFunc &procFree() const { return m_procFree; }
	//@}

	/** @brief Returns the appropriate error handlers
     */
	//@{
	inline virtual const IErrorHandler &errorAbort() const { return m_abortErrorHandler; }
	inline virtual const IErrorHandler &errorIgnore() const { return m_ignoreErrorHandler; }
	inline virtual const IErrorHandler &errorPrint() const {  return m_printErrorHandler; }
	//@}

	/** @brief The last error is set by handleError(), handleErrorV()
	 *
	 * @return Most recent error number
     */
	inline virtual RtInt lastError() { return m_lastError; }

	/** @brief Sets the current error handler, default is errorPrint()
	 *
	 *  @param handler Reference to an error handler, pointer is stored
	 */
	inline virtual RtVoid errorHandler(const IErrorHandler &handler)
	{
		m_curErrorHandler = &handler;
	}

	/** The rest of the interface functions see ricpp.h. The functions with variable length parameters
	 *  are forwarded to the ..V() functions, these are forwarded to the current rendering context
	 *  (only optionV() may be forwared to a CRendererCreator, if if there is no active context)
	 *  Errors are catched and the current error handler is called with the error found.
     */
	//@{
	virtual RtToken declare(RtString name, RtString declaration);
	virtual RtVoid synchronize(RtToken name);

	/** @brief Returns the current context handle
	 *
	 * Can be called from everywhere.
	 *
	 * @return handle of the current active context, illContextHandle (== RI_NULL)
	 * is returned if there is no active context
	 * @see context(), begin(), end(), CContextManager::getContext(), IRiRoot::getContext()
	 */
	inline virtual RtContextHandle getContext(void) { return m_ctxMgmt.getContext(); }

	/** @brief Sets the current context handle.
	 *
	 * The previous active context is deactivated before
	 * the new one is activated. The deactivated context is not destroyed.
	 *
	 * Can be called from everywhere.
	 *
	 * @param handle Handle that references a rendering context (the backend renderer),
	 *        you should not use destroyed contexts (end()).
	 *
	 * @see getContext(), begin(), end(), CBaseRenderer::context(), IRiRoot::context()
	 */
	virtual RtVoid context(RtContextHandle handle);

	/** @brief Begins a new rendering context.
	 *
	 * Can be called from everywhere.
	 *
	 * The previous active context is deactivated before
	 * the new one created. The deactivated context is not destroyed.
	 * The new context is not explicitly activated.
	 *
	 * @param name RIB file or name of renderer with parameters separated by blanks
	 * @see context(), getContext(), begin(), CBaseRenderer::begin(), IRiRoot::begin()
	 */
	virtual RtVoid begin(RtString name);

	/** @brief Ends a rendering context.
	 *
	 * The active context is destroyed. There is no active context after calling
	 * this routine until a begin() or context() is called.
	 * The destroyed context is not explicitly deactivated.
	 * 
	 * @see context(), getContext(), end(), CBaseRenderer::end(), IRiRoot::end()
	 */
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

	/** @brief Sets an Option
	 *
	 * Sets an option for the renderer. If there is no active rendering context, the option is
	 * interpreted by the bridge. The option "renderer" "searchpath" is used by the
	 * renderer creator CRendererCreator (concrete CRendererLoader)
	 *
	 * @see IRiRoot::optionV()
	 */
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

#endif // _RICPP_RICPPBRIDGE_RICPPBRIDGE_H
