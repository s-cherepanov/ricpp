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

#ifndef _RICPP_RIBPARSER_RIBPARSERCALLBACK_H
#include "ricpp/ribparser/ribparsercallback.h"
#endif // _RICPP_RIBPARSER_RIBPARSERCALLBACK_H

#include <vector>
#include <cassert>
#include <cstdarg>

namespace RiCPP {

/** @brief RenderMan Interface Bridge, the frontend
 *
 * Jobs:
 *
 * - Bridges to a renderer (context) that implements the Ri
 *   and the error handlers.
 * - Calls the Ri...V() routines from the routines with variable parameters
 * - handles the global options (e.g. Option "searchpath" "renderer"),
 * - does the error handling by catching the errors ExceptRiCPPError of the current renderer and
 *   calling the user defined error handler set by errorHandler(), 
 * - matches a frontend context handle RtContextHandle to a backend context creator with
 *   its rendering context (beginV(), end(), context(), getContext()).
 * - Entry point for RIB parsing and RIB Filters
 *
 * Normally only a single instance of CRiCPPBridge is used. However didn't want to
 * restrict it to be a singleton.
 */
class CRiCPPBridge : public IRi, protected IRibParserCallback
{
private:
	/** @brief Current user defined error handler
	 *
	 * CPrintErrorHandler is the default handler. It can be changed by errorHandler()
	 */
	const IErrorHandler *m_curErrorHandler; 
	RtInt m_lastError; ///< The last error number occured, stored by CRiCPPBridgeErrorHandler::handleErrorV()

	/** @brief Error handler used by the bridge
	 *
	 *  Forwards the error to the user defined error handler
	 *  @see CRiCPPBridge::m_ricppErrorHandler
	 */
	class CRiCPPBridgeErrorHandler : public IRiCPPErrorHandler {
		friend class CRiCPPBridge;
	private:
		/** @brief Surronding CRiCPPBridge instance
		 *
		 * The outer instance is the CRiCPPBridge instance belonging to this error handler.
		 * Used to call the user defined error handler.
		 */
		CRiCPPBridge *m_outer;

		/** @brief Is created automatically by the outer class, a CRiCPPBridge instance.
		 * @see CRiCPPBridge::m_ricppErrorHandler
		 */
		inline CRiCPPBridgeErrorHandler() : m_outer(0) { }

		/** @brief Sets the outer object
		 * 
		 * Initialized in the constructor of CRiCPPBridge.
		 *
		 * @param outer The CRiCPPBridge instances that has this object.
		 */
		inline void setOuter(CRiCPPBridge &outer)
		{
			m_outer = &outer;
		}

		/** @brief Handles an error, sets CRiCPPBridge::m_lastError and calls the current error handler
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
	} m_ricppErrorHandler; ///< Error handler instance, used only via ricppErrHandler()

	friend class CRiCPPBridgeErrorHandler;

	/** @defgroup ricpp_tokenlist Tokenlist cache 
	 * @brief Stores the token-parameter list.
	 *
	 * Used to store the token-parameter list of the current
	 * interface call with variable parameters.
	 */
	//@{
	std::vector<RtToken> m_tokens;		///<< The tokens of the parameter list of an interface call
	std::vector<RtPointer> m_params;	///<< The values of the parameter list of an interface call
	//@}

	/** @brief Standard Rib filter.
	 *
	 *  Used by m_ribFilterList to hook in other Rib filters, Options can
	 *  be used to activate/deactivate intrerface calls, if they are invoked
	 *  via rib requsets. E.G. procedure calls can be deactivated.
	 *  @see m_ribFilterList
	 */
	CRibFilter m_ribFilter;

	/** @brief Standard Rib filter list
	 *  @see m_ribFilter
	 */
	CRibFilterList m_ribFilterList;

	/** @brief Factory for stream protocol handlers.
	 *  
	 * At the moment FILE: only.
	 * @todo Add outled to add more protocol handlers.
	 */
	CBackBufferProtocolHandlers m_backBufferProtocolHandlers;
	
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
	/** @brief Gets the error handler to use.
	 *
	 *  This function can be overwritten to change the
	 *  default error handler for backend exceptions.
	 *  Usually it won't be overwritten.
	 *
	 *  @return m_ricppErrorHandler, the defualt CRiCPPBridgeErrorHandler is returned.
	 */
	inline virtual IRiCPPErrorHandler &ricppErrHandler()
	{
		return m_ricppErrorHandler;
	}

	/** @brief Gets the handler for the URI schemes.
	 *
	 *  @return The handler for the URI schemes.
	 */
	inline virtual CBackBufferProtocolHandlers &protocolHandlers()
	{
		return m_backBufferProtocolHandlers;
	};

	/**  @brief Gets the rib filter functions called by the parser.
	*
	 *   @return The rib filter functions called by the parser.
	 */
	inline virtual IRiRoot &ribFilter()
	{
		return firstRibFilter();
	}

	/** @brief Extracts all token-value pairs of an (...) interface call
	 *
	 *  The pairs are stored in m_tokens, m_params
	 *
	 *  @param token First token of the token-value list
	 *  @param marker va_start() marker
	 *  @return Number of token value pairs found
	 *  @see m_tokens, m_params
	 */
	RtInt getTokens(RtToken token, va_list marker);

	/** @brief Gets the frontend for RI-Calls (as used in procedurals)
	 *
	 *  @return Frontend for RI-calls
	 */
	virtual IRi &frontend() { return *this; }

	/** @addtogroup ricpp_contexts
	 */
	//@{
	/** @brief Holds a context creator and one of its rendering contexts.
	 *
	 * The backend context creator/rendering context pairs of the frontend
	 * CRiCPPBridge instance are managed by the CContextManagement instance
	 * m_ctxMgmt.
	 */
	class CContext
	{
	private:
		/** @brief Points to the context creator
		 *
		 *  The context creator can create a rendering context m_renderingContext.
		 *  Can be 0 if it is not set, valid() returns false in that case.
		 */
		CContextCreator *m_contextCreator;

		/** @brief References a rendering context
		 * 
		 *  The rendering context must be created by m_contextCreator.
		 *  Can be 0 if it is not set or after end(), valid() returns false in that case.
		 */
		IRiContext *m_renderingContext;

		/** @brief Context aborted?
		 *
		 *  True, if the context is aborted. If a 'severe' error occurs, the render is
		 *  aborted and no further rendering and error handling is done in this
		 *  context any more.
		 *  @see abort(), aborted()
		 */
		bool m_aborted;

	public:
		/** @brief The standard contructor initializes an empty instance
		 *
		 * The CContext instance referenced by illContextHandle is used
		 * to represent the 'outside' context (MODE_OUTSIDE), there are no
		 * valid m_contextCreator and m_renderingContext objects there.
		 */
		inline CContext() :
			m_contextCreator(0),
			m_renderingContext(0),
			m_aborted(false)
		{
		}

		/** @brief  Constructs an instance with a context creator and its rendering context.
		 *
		 * A context creator can be referenced by many CContext instances.
		 * @param aCreator Context creator
		 * @param aRenderingContext Implementation of a rendering context, created by aCreator
		 */
		inline CContext(CContextCreator *aCreator, IRiContext *aRenderingContext) :
			m_contextCreator(aCreator),
			m_renderingContext(aRenderingContext),
			m_aborted(false)
		{
		}

		/** @brief Copy constructor
		 */
		inline CContext(const CContext &ctx)
		{
			*this = ctx;
		}

		/** @brief Destructor
		 *
		 *  The rendering context is not destroyed here, this was happen in end().
		 *  The context creator destroys the renderer. This can be assured
		 *  because the CContext instance is destroyed by the CRendererBridge
		 *  right after calling it's end() through the CContextManagement.
		 *  @see end()
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

		/** @brief Gets the assigned rendering context.
		 *
		 *  Since a context creator can bear many contexts, all context creator/rendering context
		 *  pairs are stored (CContext objects in CContextManager::m_ctxMap).
		 *  *After* a CContext is activated via the
		 *  CContextManager::context() or beginV() method, renderingContext() and
		 *  contextCreator()->getContext() are the same. Only those active contexts are
		 *  used by other functions than context() and beginV().
		 *
		 *  @return Pointer to the stored backend rendering context
		 */
		inline const IRiContext *renderingContext() const
		{
			return m_renderingContext;
		}
		
		inline IRiContext *renderingContext()
		{
			return m_renderingContext;
		}

		/** @brief Checks the validity of a CContext instance.
		 *  
		 *  The instance is invalid either if
		 *
		 *  - the rendering context is 0,
		 *  - the context creator is 0 or
		 *  - the instance is aborted.
		 *
		 *  @return The validity state
		 */
		inline bool valid() const
		{
			return !m_aborted &&
				m_contextCreator != 0 &&
				m_renderingContext != 0;
		}

		/** @brief Query if the context is aborted.
		 *
		 * Aborted context are not used any more, even if active. This can be used to
		 * continue with unrecoverable errors till end().
		 *
		 * @return true if the renderer state is aborted.
		 * @see abort(), m_aborted
		 */
		inline bool aborted() const
		{
			return m_aborted;
		}

		/** @brief The current context gets aborted by the frontend
		 *  if a severe error occurs.
		 *
		 *  This indicates that no further error handling should be made (it is
		 *  likely that these errors are follow up errors) and no
		 *  more rendering is done because the renderer
		 *  is in an unrecoverable state.
		 *
		 *  However, the context is not destroyed till end().
		 *  The context creator is notified via CContextCreator::abort() that
		 *  the context is aborted.
		 *  @see aborted(), m_aborted
		 */
		inline void abort()
		{
			if ( m_contextCreator != 0 )
				m_contextCreator->abort();
			m_aborted = true;
		}

		/** @brief Ends the current context regulary.
		 *
		 *  The context creator is notified that the context ends and
		 *  any cleanup can be made before deletion. The rendering
		 *  context is deleted by the context creator's CContextCreator::end().
		 *  Therefore, it is set to 0.
		 */
		inline void end()
		{
			if ( m_contextCreator != 0 )
				m_contextCreator->end();
			m_renderingContext = 0;
		}

		/** @brief Deactivate the current context
		 *
		 *  Called indirectly from CRiCPPBridge::context(RtContextHandle handle)
		 *  and CRiCPPBridge::beginV() through the CContextManagement to deactivate
		 *  the previous active rendering context. These context can be activated
		 *  (activate()) once more and are not destroyed.
		 *
		 *  @see CContextManagement::beginV(),
		 *  CContextManagement::context(RtContextHandle handle),
		 *  CRiCPPBridge::beginV(),
		 *  CRiCPPBridge::context(RtContextHandle handle)
		 */
		inline void deactivate()
		{
			if ( m_contextCreator != 0 )
				m_contextCreator->context(0);
		}


		/** @brief Activate the current context
		 *
		 *  Called from indirectly by CRiCPPBridge::context()
		 *  through  CContextManagement to activate the
		 *  new active rendering context.
		 *
		 *  @see CContextManagement::context(RtContextHandle handle),
		 *  CRiCPPBridge::context(RtContextHandle handle)
		 */
		inline void activate()
		{
			if ( m_contextCreator != 0 )
				m_contextCreator->context(m_renderingContext);
		}

		/** @brief Copies a CContext instance.
		 *
		 * Copies only the pointers.
		 *
		 * @param ctx Context to assign to *this
		 * @return A reference to *this
		 */
		inline CContext &operator=(const CContext &ctx)
		{
			if ( &ctx == this )
				return *this;
			m_contextCreator = ctx.m_contextCreator;
			m_renderingContext = ctx.m_renderingContext;
			m_aborted = ctx.m_aborted;
			return *this;
		}
	}; // class CContext

	/** @brief Context Management, maps frontend to backend.
	 *
	 *  A RtContextHandle is mapped to a CContext, the backend context creator/rendering context pairs.
	 *  A RtContextHandle of the frontend references one of those pairs. The CRiCPPBridge
	 *  forwards the interface calls always to the active backend rendering context. beginV() and
	 *  context() are out of this rule.
	 *
	 *  The frontend illContextHandle has a special meaning: "There is no active rendering context". It
	 *  can be thought of illContextHandle representing the bridge itself, it cannot be removed.
	 */
	class CContextManagement {
		RtContextHandle m_nextCtxHandle; ///< Next value for context handle (counts 1 upwarts), deleted handles are not reused.
		RtContextHandle m_ctxHandle;     ///< Current context handle of the frontend.
		CContext m_curCtx;               ///< Current creator, context pair of the backend, it is a copy of m_ctxMap[m_ctxHandle] for faster access
		std::map<RtContextHandle, CContext> m_ctxMap; ///< Maps used frontend context handles to it's backend CContext context creator/rendering context pair.

		CRiCPPBridge *m_outer; ///< Bridge that contains the context manager.

		/** @brief Loader for backends.
		 *
		 *  Used by to create backends of the type CContextCreator, which in turn
		 *  is used to create the concrete rendering contexts IRiContext
		 */
		CRendererLoader m_rendererLoader;

		/** @brief Removes a context creator/rendering context pair from the context map.
		 *
		 *  illContextHandle, the handle that represents the bridge itself cannot be removed.
		 *
		 * @param handle The RtContextHandle as key, illContextHandle is not removed.
		 */
		void removeContext(RtContextHandle handle);

		/** @brief Adds a new CContext instance.
		 *
		 *  Puts the backend context creator / rendering context pair at the context map
		 *  with the frontend handle as key. The handle is the number m_nextCtxHandle.
		 *  m_nextCtxHandle is also increased at the end of this method to
		 *  serve as the next handle.
		 *	m_ctxHandle, the current context handle, is set by the calling function
		 *  beginV() to refer the new context.
		 *
		 * @param ctx A backend context creator/rendering context pair, to be stored
		 * @return The new frontend handle of the creator
		 * @see beginV()
		 */
		RtContextHandle addContext(const CContext &ctx);
	public:
		/** @brief Initializes context management.
		 *
		 *  The illContextHandle representing the context outside
		 *  (the bridge itself for some means).
		 */
		CContextManagement();

		inline void setOuter(CRiCPPBridge &outer) { m_outer = &outer; }

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
		 * Manages the creation of a new backend rendering context,
		 * creates a new CContext and
		 * stores it with add(). The old context is deactivated. To generate a new
		 * active rendering context the CContextCreator::beginV() is called.
		 * The new context is not explicitly activated (because a creating a new
		 * context might be handled by the backend different to activating an
		 * already created one).
		 *
		 * @param name Name of a renderer, forwarded from TRiCPPBridge::beginV() call
		 * @param n number of @a tokens and @a params, forwarded from TRiCPPBridge::beginV() call
		 * @param tokens Token array, forwarded from TRiCPPBridge::beginV() call
		 * @param params Array of pointers to parameter arrays, forwarded from
		 * TRiCPPBridge::beginV() call
		 * @return New frontend context handle
		 * @exception ExceptRiCPPError If thrown, there is no active rendering context any more
		 * @see add(), CContext::activate(), IRiCCPPBridge::beginV()
		 */
		RtContextHandle beginV(RtString name, RtInt n, RtToken tokens[], RtPointer params[]);

		/** @brief Ends the current context and removes it from the list.
		 *
		 * There is no active rendering context after end() ended. The context
		 * is not explicitely deactivated (because ending a rendering context
		 * can be handled by the backend different to deactivating one for
		 * further use). In anny case context() is illContextHandle after
		 * end() (even if an error was thrown)
		 * 
		 * @exception ExceptRiCPPError
		 * @see CContext::end(), removeContext()
		 */
		void end();

		/** @brief Gets the current frontend context handle.
		 *
		 *  @return The handle of the current frontend context handle
		 */
		inline RtContextHandle getContext() const
		{
			return m_ctxHandle;
		}

		/** @brief Test if the context handle is in the map.
		 *
		 * @param handle A frontend context handle
		 * @return true, if the parameter handle is managed (can be illContextHandle or aborted)
		 */
		inline bool isContext(RtContextHandle handle)
		{
			return m_ctxMap.find(handle) != m_ctxMap.end();
		}

		/** @brief Gets the active current creator/rendering context.
		 * 
		 * @return A reference to the backend current creator/context pair,
		 * not neccessairly valid (e.g. if aborted).
		 */
		inline const CContext &curBackend() const
		{
			return m_curCtx;
		}

		inline CContext &curBackend()
		{
			return m_curCtx;
		}

		/** @brief Sets and activates the new current context.
		 *
		 * The previous context is deactivated.
		 *
		 * @param handle A context handle of the new context
		 * @return True, if the parameter handle was found and set
		 *         as active (normally tested before context() is called).
		 */
		bool context(RtContextHandle handle);
		
		/** @brief sets the searchpath for the rendererloader
		 *  @param path The searchpath
		 */
		inline void searchpath(const char *path)
		{
			m_rendererLoader.searchpath(path);
		}

		/** @brief Gets the current searchpath of the rendererloader.
		 *
		 * @return Searchpath, directory seperator '/', pathes separated by ':'.
		 */
		inline const char *searchpath() const
		{
			return m_rendererLoader.searchpath();
		}

		/** @brief Registers a plugin factory for the context creator
		 *
		 *  Registers a context creator factory for a specific name. Normally
		 *  TemplPluginFactory are registered to create specific plugins with
		 *  \c new instead of loading them from a dynamic library.
		 *
		 * @param name Name of the renderer factory (context creator)
		 * @param f Factory for the context creators
		 * @return true, if the plugin factory could be registered
		 */
		inline virtual bool registerFactory(
			const char *name,
			TemplPluginFactory<CContextCreator> *f)
		{
			return m_rendererLoader.registerFactory(name, f);
		}

		/** @brief unregisters a renderer factory
		 * @param name Name of the plugins
		 * @return true, if factory was unregistered
		 */
		inline virtual bool unregisterFactory(const char *name)
		{
			return m_rendererLoader.unregisterFactory(name);
		}

		/** @brief Gets the name of the standard renderer.
		 *
		 * @return Name of the standard renderer
		 */
		inline const char *standardRendererName() const
		{
			return m_rendererLoader.standardRendererName();
		}

		/** @brief Sets the name of the standard renderer.
		 *
		 * @param name Name of the standard renderer, 0 is substituted by "ribwriter"
		 */
		inline void standardRendererName(const char *name)
		{
			m_rendererLoader.standardRendererName(name);
		}
	};
	
	CContextManagement m_ctxMgmt; ///< The instance for the context management
	//@}


	/** @brief Like optionV() but only concerns the bridge itself.
	 *
	 * Forwarded by optionV() if there is no active rendering context.
	 * @param name Option name (likely "searchpath" for the renderer searchpath)
	 * @param n Number token-value pairs
	 * @param tokens Tokens
	 * @param params Parameter values
	 */
	virtual RtVoid doOptionV(RtString name, RtInt n, RtToken tokens[], RtPointer params[]);

	/** @brief Helper object to get standard pathes
	 *  @see doOptionV()
	 */
	CPathReplace m_pathReplace;
	
	/** @brief Standard path for Rib filters
	 *  
	 *  Set in Constructor by Option "standardpath" "ribfilter" "path"
	 */
	std::string m_standardPathRibFilter;

	/** @brief Standard path for Renderers
	 *  
	 *  Set in Constructor by Option "standardpath" "renderer" "path"
	 */
	std::string m_standardPathRenderer;
	
public:
	/** @brief Creates a bridge.
	 *
	 * CPrintErrorHandler is used as error handler, an invalid context indexed by illContextHandle
	 * is stored to represent 'outside' context and the rib filter list is initialized. 
	 */
	CRiCPPBridge();

	/** @brief Destructor
	 */
	virtual ~CRiCPPBridge();

	/******************************************************************************/

	/** @defgroup ri_addfrontend Additional front end functions
	 *
	 *  @brief User defined Rib filters can be inserted into the filterlist
	 *  @{
	 */

	/**  @brief Gets the first interface to a user defined RIB filter.
	 *  @return A reference of the interface of the first user defined filter.
	 */
	inline virtual IRiRoot &firstRibFilter()
	{
		if ( m_ribFilterList.firstHandler() )
			return *m_ribFilterList.firstHandler();
		return *this;
	}

	/** @brief Tgets the last interface to a user defined RIB filter.
	 *  @return A reference of the interface of the last user defined filter.
	 */
	inline virtual IRiRoot &lastRibFilter()
	{
		if ( m_ribFilterList.lastHandler() )
			return *m_ribFilterList.lastHandler();
		return *this;
	}

	/** @brief Adds a new rib filter to the front.
	 *
	 *  The rib filter at the front is used first
	 *
	 *  @return true, filter was inserted
	 */
	inline virtual bool addFrontRibFilter(CRibFilter *aFilter)
	{
		try {
			return m_ribFilterList.addFront(aFilter);
		} catch ( ExceptRiCPPError &e ) {
			ricppErrHandler().handleError(e);
		}
		return false;
	}

	/** @brief Adds a new rib filter plugin to the front.
	 *
	 *  The rib filter at the front is used first
	 *
	 *  @param name The name of the Rib filter (dll or in memory)
	 *  @return true, filter was inserted
	 */
	inline virtual bool addFrontRibFilter(const char *name)
	{
		try {
			return m_ribFilterList.addFront(name);
		} catch ( ExceptRiCPPError &e ) {
			ricppErrHandler().handleError(e);
		}
		return false;
	}

	/** @brief Removes a rib filter from the top
	 *
	 *  The rib filter is only deleted if it was created by this instance
	 *
	 *  @return true, filter was removed
	 */
	inline virtual bool removeFrontRibFilter()
	{
		return m_ribFilterList.removeFront();
	}

	/** @brief Register a filter factory
	 *
	 *  The filter factory of the name \a name is used to create
	 *  filters directly in memory.
	 *
	 *  @return true, filter factory was registerd
	 */
	inline virtual bool registerRibFilterFactory(const char *name, TemplPluginFactory<CRibFilter> *f)
	{
		try {
			return m_ribFilterList.registerFactory(name, f);
		} catch ( ExceptRiCPPError &e ) {
			ricppErrHandler().handleError(e);
		}
		return false;
	}

	/** @brief Registers a plugin factory for the context creator
	 *
	 *  Registers a context creator factory for a specific name. Normally
	 *  TemplPluginFactory are registered to create specific plugins with
	 *  \c new instead of loading them from a dynamic library.
	 *
	 * @param name Name of the renderer factory (context creator)
	 * @param f Factory for the context creators
	 * @return true, if the plugin factory could be registered
	 */
	inline virtual bool registerRendererFactory(const char *name, TemplPluginFactory<CContextCreator> *f)
	{
		try {
			return m_ctxMgmt.registerFactory(name, f);
		} catch ( ExceptRiCPPError &e ) {
			ricppErrHandler().handleError(e);
		}
		return false;
	}

	/** @brief unregisters a plugin factory
	 * @param name Name of the plugins
	 * @return true, if factory was unregistered
	 */
	inline virtual bool unregisterRendererFactory(const char *name)
	{
		try {
			return m_ctxMgmt.unregisterFactory(name);
		} catch ( ExceptRiCPPError &e ) {
			ricppErrHandler().handleError(e);
		}
		return false;
	}

	/** @brief Gets the name of the standard renderer.
	 *
	 * @return Name of the standard renderer
	 */
	inline const char *standardRendererName() const { return m_ctxMgmt.standardRendererName(); }

	/** @brief Sets the name of the standard renderer.
	 *
	 * @param name Name of the standard renderer, 0 is substituted by "ribwriter"
	 */
	inline void standardRendererName(const char *name) { m_ctxMgmt.standardRendererName(name); }
	//@}

	/******************************************************************************
	 The RenderMan interface functions
	 ******************************************************************************
	 RenderMan(R) is a registered trademark of Pixar
	 The RenderMan(R) Interface Procedures and Protocol are:
	 Copyright 1988, 1989, 2000, 2005 Pixar
	 All rights Reservered
	 ******************************************************************************/

	/** @addtogroup ricpp_filter
	 *  @brief Returns the appropriate filter functions
     */
	//@{
	inline virtual const IFilterFunc &boxFilter() const { return CBoxFilter::func; }
	inline virtual const IFilterFunc &catmullRomFilter() const { return CCatmullRomFilter::func; }
	inline virtual const IFilterFunc &gaussianFilter() const { return CGaussianFilter::func; }
	inline virtual const IFilterFunc &sincFilter() const { return CSincFilter::func; }
	inline virtual const IFilterFunc &triangleFilter() const { return CTriangleFilter::func; }
	//@}

	/** @addtogroup ricpp_proc
	 *  @brief Returns the appropriate procedurals
     */
	//@{
	inline virtual const ISubdivFunc &procDelayedReadArchive() const { return CProcDelayedReadArchive::func; }
	inline virtual const ISubdivFunc &procRunProgram() const { return CProcRunProgram::func; }
	inline virtual const ISubdivFunc &procDynamicLoad() const { return CProcDynamicLoad::func; }
	inline virtual const IFreeFunc &procFree() const { return CProcFree::func; }
	//@}

	/** @addtogroup ricpp_error
	 *  @brief Returns the appropriate error handlers
     */
	//@{

	inline virtual const IErrorHandler &errorAbort() const { return CAbortErrorHandler::func; }
	inline virtual const IErrorHandler &errorIgnore() const { return CIgnoreErrorHandler::func; }
	inline virtual const IErrorHandler &errorPrint() const { return CPrintErrorHandler::func; };

	/** @brief The last error is set by CRiCPPBridgeErrorHandler::handleError(), CRiCPPBridgeErrorHandler::handleErrorV()
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
		m_curErrorHandler = &handler.singleton();
	}
	//@}

	/******************************************************************************/

	/** @defgroup ri_frontend Remaining Ri requests
	 *
	 *  @brief The RenderMan frontend functions that are forwarded to the backend.
	 *
	 * The rest of the interface functions see ricpp.h. The calls of functions with
	 * variable length parameters are passed to the ...V() functions, these are
	 * forwarded to the current rendering context
	 * (only optionV() may be handled by the frontend, if if there is no active context)
	 * Errors are catched and the current error handler is called with the error found.
     */
	//@{
	virtual RtToken declare(RtToken name, RtString declaration);
	virtual RtVoid synchronize(RtToken name);
	virtual RtVoid system(RtString cmd);

	virtual RtVoid control(RtToken name, RtToken token = RI_NULL, ...);
	virtual RtVoid controlV(RtToken name, RtInt n, RtToken tokens[], RtPointer params[]);

	virtual RtVoid version();

	virtual RtVoid resource(RtToken handle, RtToken type, RtToken token = RI_NULL, ...);
	virtual RtVoid resourceV(RtToken handle, RtToken type, RtInt n, RtToken tokens[], RtPointer params[]);

	/** @brief Returns the current context handle
	 *
	 * Can be called from everywhere.
	 *
	 * @return handle of the current active context, illContextHandle (== RI_NULL)
	 * is returned if there is no active context
	 * @see context(), beginV(), end(), CContextManager::getContext(), IRiRoot::getContext()
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
	 *        you should not use destroyed contexts (end()). \a handle can be
	 *        illContextHandle to disable the backend.
	 *
	 * @see getContext(), beginV(), end(),
	 * CContextManager::context(), IRiRoot::context()
	 */
	virtual RtVoid context(RtContextHandle handle);

	/** @brief Begins a new rendering context.
	 *
	 * Can be called from everywhere.
	 *
	 * If the name is a RIB filename (ends with .rib), the filename is inserted as
	 * new parameter. @a name is set to 0 to indicate that a ribfile should be written.
	 *
	 * @param name RIB file or name of renderer, if empty the Rib writer is used
	 * @param token First token in parameterlist (e.G. RI_FILE for filename) followed by a parameter
	 *        pointer (value array), and so on. Must be ended by a token RI_NULL.
	 * @return The new context handle
	 * @see beginV()
	 */
	virtual RtContextHandle begin(RtString name, RtToken token = RI_NULL, ...);

	/** @brief Begins a new rendering context.
	 *
	 * Can be called from everywhere.
	 *
	 * The previous active context is deactivated before
	 * the new one created. The deactivated context is not destroyed.
	 * The new context is not explicitly activated.
	 *
	 * @param name RIB file or name of renderer
	 * @param n Number of Token-Value pairs.
	 * @param tokens Tokens in parameterlist (e.g. RI_FILE for filename)
	 * @param params The parameter areay pointers for the tokens (e.g. pointer (char **) to the filename)
	 * @return The new context handle
	 *
	 * @see context(), getContext(), beginV(), end(),
	 * CContextManager::beginV(), IRiRoot::beginV()
	 */
	virtual RtContextHandle beginV(RtString name, RtInt n, RtToken tokens[], RtPointer params[]);

	/** @brief Ends a rendering context.
	 *
	 * The active context is destroyed. There is no active context after calling
	 * this routine until a beginV() or context() is called.
	 * The destroyed context is not explicitly deactivated.
	 * 
	 * @see context(), getContext(), beginV(), CContextManager::end(), IRiRoot::end()
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

	virtual RtVoid resourceBegin(void);
	virtual RtVoid resourceEnd(void);

	virtual RtArchiveHandle archiveBegin(RtToken name, RtToken token = RI_NULL, ...);
	virtual RtArchiveHandle archiveBeginV(RtToken name, RtInt n, RtToken tokens[], RtPointer params[]);
	virtual RtVoid archiveEnd(void);

	/******************************************************************************/

	virtual RtVoid format(RtInt xres, RtInt yres, RtFloat aspect);
    virtual RtVoid frameAspectRatio(RtFloat aspect);
    virtual RtVoid screenWindow(RtFloat left, RtFloat right, RtFloat bot, RtFloat top);
    virtual RtVoid cropWindow(RtFloat xmin, RtFloat xmax, RtFloat ymin, RtFloat ymax);

	virtual RtVoid projection(RtToken name, RtToken token = RI_NULL, ...);
    virtual RtVoid projectionV(RtToken name, RtInt n, RtToken tokens[], RtPointer params[]);

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

	virtual RtVoid displayChannelV(RtString channel, RtInt n, RtToken tokens[], RtPointer params[]);
	virtual RtVoid displayChannel(RtString channel, RtToken token = RI_NULL, ...);

	virtual RtVoid display(RtString name, RtToken type, RtString mode, RtToken token = RI_NULL, ...);
    virtual RtVoid displayV(RtString name, RtToken type, RtString mode, RtInt n, RtToken tokens[], RtPointer params[]);

	virtual RtVoid hider(RtToken type, RtToken token = RI_NULL, ...);
    virtual RtVoid hiderV(RtToken type, RtInt n, RtToken tokens[], RtPointer params[]);

	virtual RtVoid colorSamples(RtInt N, RtFloat nRGB[], RtFloat RGBn[]);

	virtual RtVoid relativeDetail(RtFloat relativedetail);

	virtual RtVoid option(RtToken name, RtToken token = RI_NULL, ...);

	/** @brief Sets an Option
	 *
	 * Sets an option for the renderer. If there is no active rendering context, the option is
	 * interpreted by the bridge. The option "renderer" "searchpath" is used by the
	 * renderer creator CRendererLoader (concrete CRendererLoader)
	 *
	 * @see IRiRoot::optionV()
	 */
	virtual RtVoid optionV(RtToken name, RtInt n, RtToken tokens[], RtPointer params[]);

	/******************************************************************************/

	virtual RtLightHandle lightSource(RtString name, RtToken token = RI_NULL, ...);
    virtual RtLightHandle lightSourceV(RtString name, RtInt n, RtToken tokens[], RtPointer params[]);
	virtual RtLightHandle areaLightSource(RtString name, RtToken token = RI_NULL, ...);
	virtual RtLightHandle areaLightSourceV(RtString name, RtInt n, RtToken tokens[], RtPointer params[]);

	/******************************************************************************/

	virtual RtVoid attribute(RtToken name, RtToken token = RI_NULL, ...);
    virtual RtVoid attributeV(RtToken name, RtInt n, RtToken tokens[], RtPointer params[]);

	virtual RtVoid color(RtColor Cs);
	virtual RtVoid opacity(RtColor Os);

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
    virtual RtVoid trimCurve(RtInt nloops, RtInt ncurves[], RtInt order[], RtFloat knot[], RtFloat amin[], RtFloat amax[], RtInt n[], RtFloat u[], RtFloat v[], RtFloat w[]);

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

	virtual RtVoid scopedCoordinateSystem(RtToken space);
	virtual RtVoid coordinateSystem(RtToken space);
	virtual RtVoid coordSysTransform(RtToken space);
	virtual RtPoint *transformPoints(RtToken fromspace, RtToken tospace, RtInt npoints, RtPoint points[]);

	/******************************************************************************/
	
	virtual RtVoid polygon(RtInt nvertices, RtToken token = RI_NULL, ...);
    virtual RtVoid polygonV(RtInt nvertices, RtInt n, RtToken tokens[], RtPointer params[]);
	virtual RtVoid generalPolygon(RtInt nloops, RtInt nverts[], RtToken token = RI_NULL, ...);
	virtual RtVoid generalPolygonV(RtInt nloops, RtInt nverts[], RtInt n, RtToken tokens[], RtPointer params[]);
	virtual RtVoid pointsPolygons(RtInt npolys, RtInt nverts[], RtInt verts[], RtToken token = RI_NULL, ...);
	virtual RtVoid pointsPolygonsV(RtInt npolys, RtInt nverts[], RtInt verts[], RtInt n, RtToken tokens[], RtPointer params[]);
	virtual RtVoid pointsGeneralPolygons(RtInt npolys, RtInt nloops[], RtInt nverts[], RtInt verts[], RtToken token = RI_NULL, ...);
    virtual RtVoid pointsGeneralPolygonsV(RtInt npolys, RtInt nloops[], RtInt nverts[], RtInt verts[], RtInt n, RtToken tokens[], RtPointer params[]);

	virtual RtVoid patch(RtToken type, RtToken token = RI_NULL, ...);
    virtual RtVoid patchV(RtToken type, RtInt n, RtToken tokens[], RtPointer params[]);
    virtual RtVoid patchMesh(RtToken type, RtInt nu, RtToken uwrap, RtInt nv, RtToken vwrap, RtToken token = RI_NULL, ...);
    virtual RtVoid patchMeshV(RtToken type, RtInt nu, RtToken uwrap, RtInt nv, RtToken vwrap, RtInt n, RtToken tokens[], RtPointer params[]);
	virtual RtVoid nuPatch(RtInt nu, RtInt uorder, RtFloat uknot[], RtFloat umin, RtFloat umax, RtInt nv, RtInt vorder, RtFloat vknot[], RtFloat vmin, RtFloat vmax, RtToken token = RI_NULL, ...);
    virtual RtVoid nuPatchV(RtInt nu, RtInt uorder, RtFloat uknot[], RtFloat umin, RtFloat umax, RtInt nv, RtInt vorder, RtFloat vknot[], RtFloat vmin, RtFloat vmax,  RtInt n, RtToken tokens[], RtPointer params[]);

	virtual RtVoid subdivisionMesh(RtToken scheme, RtInt nfaces, RtInt nvertices[], RtInt vertices[], RtInt ntags, RtToken tags[], RtInt nargs[], RtInt intargs[], RtFloat floatargs[], RtToken token = RI_NULL, ...);
    virtual RtVoid subdivisionMeshV(RtToken scheme, RtInt nfaces, RtInt nvertices[], RtInt vertices[], RtInt ntags, RtToken tags[], RtInt nargs[], RtInt intargs[], RtFloat floatargs[],  RtInt n, RtToken tokens[], RtPointer params[]);

	virtual RtVoid hierarchicalSubdivisionMesh(RtToken scheme, RtInt nfaces, RtInt nvertices[], RtInt vertices[], RtInt ntags, RtToken tags[], RtInt nargs[], RtInt intargs[], RtFloat floatargs[],  RtToken stringargs[],  RtToken token = RI_NULL, ...);
    virtual RtVoid hierarchicalSubdivisionMeshV(RtToken scheme, RtInt nfaces, RtInt nvertices[], RtInt vertices[], RtInt ntags, RtToken tags[], RtInt nargs[], RtInt intargs[], RtFloat floatargs[],  RtToken stringargs[],  RtInt n, RtToken tokens[], RtPointer params[]);

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

	virtual RtVoid procedural(RtPointer data, RtBound bound, const ISubdivFunc &subdivfunc, const IFreeFunc *freefunc);

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
    virtual RtVoid makeBrickMap(RtInt nNames, RtString ptcnames[], RtString bkmname, RtToken token = RI_NULL, ...);
    virtual RtVoid makeBrickMapV(RtInt nNames, RtString ptcnames[], RtString bkmname, RtInt n, RtToken tokens[], RtPointer params[]);

	/******************************************************************************/

	virtual RtVoid archiveRecord(RtToken type, RtString format, ...);
	virtual RtVoid archiveRecordV(RtToken type, RtString line);
    virtual RtVoid readArchive(RtString name, const IArchiveCallback *callback, RtToken token = RI_NULL, ...);
	virtual RtVoid readArchiveV(RtString name, const IArchiveCallback *callback, RtInt n, RtToken tokens[], RtPointer params[]);

	/******************************************************************************/

	virtual RtVoid ifBegin(RtString expr);
	virtual RtVoid elseIfBegin(RtString expr);
	virtual RtVoid elseBegin(void);
	virtual RtVoid ifEnd(void);
	//@}
}; // CRiCPPBridge

} // namespace RiCPP

#endif // _RICPP_RICPPBRIDGE_RICPPBRIDGE_H
