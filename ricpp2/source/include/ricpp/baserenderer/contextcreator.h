#ifndef _RICPP_BASERENDERER_CONTEXTCREATOR_H
#define _RICPP_BASERENDERER_CONTEXTCREATOR_H

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

/** @file contextcreator.h
 *  @author Andreas Pidde (andreas@pidde.de)
 *  @brief Declaration of a class for creation and basic handling of render contexts.
 *
 *  The CContextCreator is part of the backend. It is managed by CRiCPPBridge and CRendererLoader.
 */

#ifndef _RICPP_PLUGINHANDLER_PLUGINHANDLER_H
#include "ricpp/pluginhandler/pluginhandler.h"
#endif // _RICPP_PLUGINHANDLER_PLUGINHANDLER_H

#ifndef _RICPP_BASERENDERER_RICONTEXT_H
#include "ricpp/baserenderer/ricontext.h"
#endif // _RICPP_BASERENDERER_RICONTEXT_H

namespace RiCPP {

/** @brief Context creation and switching, base class of a concrete context creator.
 *
 * The context creator is the part of the backend and delivers a concrete renderer context.
 * All renderers of the same name shall share the same context creator, e.g. there is
 * one CRibWriterCreator for the "ribwriter" CRibWriter contexts declared in ribwriter.h.
 * There are members to ask for the name, type and versioning information of the concrete contextgroup.
 * If loaded from a shared library a context creator is return by newContextCreator() and can be destroyed
 * by deleteContextCreator() (e.g. winribwriterdll.cpp macribwriterdll.cpp). CRiCPPBridge manages
 * the usage of CContextCreator from the frontend.
 */
class CContextCreator : IPlugin {

	/** @brief List of all contexts created and not yet destroyed.
	 */
	std::list<IRiContext *> m_contextList;

	/** @brief Active context of this creator.
	 *
	 * The context currently active. It has to be an item of m_contextList of the same creator
	 * or 0. There is at most one active context at a time for each bridge. The
	 * CRiCPPBridge can be used to get this context: CRiCPPBridge::getContext().
	 * CRiCPPBridge::context() manages the context switching.
	 * - \a m_curContext is a member of \a m_contextList or 0.
	 * - \a m_curContext == 0 means that there is no active context.
	 */
	IRiContext *m_curContext;

	/** @brief Error handler used by CContextCreator, it is returned by the virtual errHandler().
	 *
	 * This error handler throws an ExceptRiCPPError exception. The exception is
	 * catched by the front end and bridged to the RenderMan error handler.
	 * Member shall be never used directly, so it can be changed by an overwritten ricppErrHandler()
	 * at a child class.
	 */
	CErrorExceptionHandler m_errorHandler;

protected:
	/** @brief Returns the error handler to use. It can but usually
	 *  won't be overwritten in this framework.
	 *
	 *  @return \a m_errorHandler, the default CErrorExceptionHandler is returned.
	 */
	inline virtual IRiCPPErrorHandler &ricppErrHandler() { return m_errorHandler; }

	/** @brief Factory method, must be overwritten to return a new concrete renderer context.
	 *
	 * @return This one returns 0, an overwritten method returns a new concrete render context.
	 */
	inline virtual IRiContext *getNewContext() { return 0; }

	/** @brief Discards the current context.
	 * 
	 * Removes the current context m_curContext from the m_contextList it deletes it.
	 * So it is not available anymore. This member function is called after ending
	 * a context by end() or abort().
	 */
	virtual void deleteContext();

public:
	static const char *myName();
	static const char *myType();
	static unsigned long myMajorVersion();
	static unsigned long myMinorVersion();
	static unsigned long myRevision();

	inline virtual const char *name() const { return myName(); }
	inline virtual const char *type() const { return myType(); }
	inline virtual unsigned long majorVersion() const {return myMajorVersion(); }
	inline virtual unsigned long minorVersion() const {return myMinorVersion(); }
	inline virtual unsigned long revision() const {return myRevision(); }

	inline virtual void startup() {}
	inline virtual void shutdown() {}

	/** @brief Constructor, just initializes the current context to 0
	 */
	inline CContextCreator() : m_curContext(0) { }

	/** @brief Destructor, deletes all remaining contexts
	 */
	virtual ~CContextCreator();

	/** @brief Aborts (irregulary) the current context.
	 *
	 * Called bey the front end if an severe error occurs.
	 * Calls the current context's IRiContext::abort() (no explicit deactivate()).
	 * It is not discarded here, an explicit end must be called.
	 * Don't throw an exception here.
	 */
	virtual RtVoid abort(void);

	/** @brief The current render context for the library managed by this CContextCreator.
	 *
	 * @return The current context (m_curContext) to work with. Mind, it can return 0 if there is no active context.
	 */
	virtual IRiContext *getContext() { return m_curContext; }

	/** @brief Context switching
	 *
	 * Used also to notify the context switching using
	 * IRiContext::deactivate() for the old m_curContext and IRiContext::activate()
	 * for the new context (if not 0). This is important for example for OpenGL to activate
	 * or deactivate the OpenGL context. Sets the current context m_curContext
	 * to the value of the parameter context. It can call the error handler if the parameter
	 * context is not a member of m_contextList, i.e. not created by beginV() of this CContextCreator.
	 * CRiCPPBridge::context() manages the context switching.
	 *
	 * @param context The new current context, can be 0 to deactivate the previous m_curContext.
	 *        Must be created by this CContextCreator.
	 */
	virtual RtVoid context(IRiContext *context);

	/** @brief Begins a new rendering context
	 *
	 * Called by the CRiCPPBridge::beginV() of the framework front end.
	 * deactivates the current context createsa new one by calling getContext().
	 * The new context is activated by calling it's beginV() (no explicite activate()),
	 * \a name is used as parameter for this method again. Can call the error handler
	 * if no new context can be created. The current context is cleared in this case.
	 *
	 * @param name Name of a renderer
	 * @param n number of parameters
	 * @param tokens Token array
	 * @param params Prameter pointer array
	 * @return The new backend context handle
	 * @see CRiCPPBridge::beginV()
	 */
	virtual IRiContext *beginV(RtString name, RtInt n, RtToken tokens[], RtPointer params[]);

	/** @brief Regulary ends the current context.
	 *
	 * Called by the front end if end() is reached.
	 * Calls the current context's IRiContext::end() (no explicit deactivate()) and discards it.
	 * Don't throw an exception here.
	 */
	virtual RtVoid end(void);

	/** @brief Versioning information
	 * 
	 * Only majorVersion() is implemented in CContextCreator.
	 * The minorVersion(), revision(), rendereName(), rendererType() shall be
	 * overloaded by child classes to forward the information of concrete contexts.
	 */
	//@{
	/** @brief Just forwards the major version of the interface IRiContext
	 *
	 * The methods of IRiContext are called from the front end, so it must be the same as the one
	 * the front end uses. 
	 *
	 * @return the major version of IRiContext
	 */
	virtual unsigned long majorContextVersion(void) const { return IRiContext::myMajorVersion(); }

	/** @brief The minor version is the version of a concrete context (must be overwritten)
	 *  @return 0, overload to return the minor version of a concrete context.
	 */
	inline virtual unsigned long minorContextVersion() const { return 0; }

	/** @brief The revision of a concrete context (must be overwritten)
	 *  @return 0, overload to return the revision of a concrete context.
	 */
	inline virtual unsigned long contextRevision() const { return 0; }

	/** @brief The name of a concrete context (must be overwritten)
	 *  @return 0, overload to return the name of a concrete context.
	 */
	inline virtual RtToken contextName() const { return RI_NULL; }

	/** @brief The type of a concrete context interface (that's IRiContext)
	 *  @return Type of IRiContext
	 */
	inline virtual RtToken contextType() const { return IRiContext::myType(); }

	/** @brief The renderer type of a concrete context (must be overwritten)
	 *  @return RI_NULL, overload to return the type of a concrete context (RI_ARCHIVE, RI_DRAFT, RI_REALISTIC, or user defined).
	 */
	inline virtual RtToken rendererType() const { return RI_NULL; }
	//@}
}; // CContextCreator

} // namespace RiCPP

#endif // _RICPP_BASERENDERER_CONTEXTCREATOR_H
