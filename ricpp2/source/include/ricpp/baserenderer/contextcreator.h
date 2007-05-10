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
 *  @brief Contains the declaration of a class for creation and basic handling of render contexts.
 */

#ifndef _RICPP_RICPP_RENDERERERROR_H
#include "ricpp/baserenderer/renderererror.h"
#endif // _RICPP_RICPP_RENDERERERROR_H

#ifndef _RICPP_BASERENDERER_RICONTEXT_H
#include "ricpp/baserenderer/ricontext.h"
#endif // _RICPP_BASERENDERER_RICONTEXT_H

#include <list>

namespace RiCPP {

/** Context creation and switching, base class of a concrete context creator.
 *
 *  All renderers of the same type shall share the same context creator. The
 *  context creator is the part of the back end delivering a concrete renderer context.
 */
class CContextCreator {

	/** @brief List of all context created and not yet destroyed.
	 */
	std::list<IRiContext *> m_contextList;

	/** @brief Active context of this creator.
	 *
	 * \a m_curContext is a member of \a m_contextList or 0.
	 * \a m_curContext == 0 means that there is no active context.
	 */
	IRiContext *m_curContext;

	/** @brief Error Handler, returned by the virtual \a errHandler().
	 *
	 * This error handler throws an ERendererError exception. The exception is
	 * catched by the front end and bridged to the RenderMan error handler.
	 *  Never used directly, so it can be changed by a overwriting errHandler() in a child class.
	 */
	CErrorHandler m_errorHandler;

protected:
	/** @brief Returns the error handler for use, can be overwritten but usually
	 *  won't in this framework.
	 *
	 *  @return \a m_errorHandler, the default CErrorHandler is returned.
	 */
	inline virtual CErrorHandler &errHandler() { return m_errorHandler; }

	/** @brief Factory method, must be overwritten to return a concrete renderer context.
	 *
	 * @return This one returns 0, a overwritten method one returns a concrete render context.
	 */
	inline virtual IRiContext *getNewContext() { return 0; }

	/** @brief Discards the current context.
	 * 
	 * Removes the current context \a m_curContext from \a m_contextList it deletes it.
	 */
	virtual void deleteContext();

public:
	/** @brief Constructor, just initializes the current context to 0
	 */
	inline CContextCreator() : m_curContext(0) { }

	/** @brief Destructor, deletes all remaining contexts
	 */
	virtual ~CContextCreator();

	/** @brief  The current context
	 *
	 * @return The current context (\a m_curContext) to work with, can be 0.
	 */
	virtual IRiContext *getContext() { return m_curContext; }

	/** @brief Context switching
	 *
	 * Used also to notify the context switching using
	 * IRiContext::deactivate() for the old \a m_curContext and IRiContext::activate()
	 * for the new context (if not 0). This is important for example for OpenGL to activate
	 * or deactivate the OpenGL context. Sets the current context \a m_curContext
	 * to \a context. It can call the error handler if \a context is not a member of
	 * \a m_contextList, i.e. not created by \a begin() of this \a CContextCreator.
	 *
	 * @param context The new current context, can be 0 to deactivate the previous \a m_curContext.
	 *        Must be created by this \a CContextCreator.
	 */
	virtual RtVoid context(IRiContext *context);

	/** @brief Begin a new rendering context
	 *
	 * Called by the CRiCPPBridge::begin() of the framework front end.
	 * deactivates the current context createsa new one by calling getContext().
	 * The new context is activated by calling it's begin() (no explicite activate()),
	 * \a name is used as parameter for this method again. Can call the error handler
	 * if no new context can be created. The current context is cleared in this case.
	 *
	 * @param name The parameter used for the front end's CRiCPPBridge::begin()
	 */
	virtual RtVoid begin(RtString name);

	/** @brief Aborts (irregulary) the current context.
	 *
	 * Called bey the front end if an severe error occurs.
	 * Calls the current context's IRiContext::abort() (no explicit deactivate()) and discards it.
	 * Don't throw an exception here.
	 */
	virtual RtVoid abort(void);

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
	virtual unsigned long majorVersion(void) const { return IRiContext::riContextMajorVersion; }

	/** @return 0, overload to return the minor version of a concrete context.
	 */
	inline virtual unsigned long minorVersion() const { return 0; }

	/** @return 0. overload to return the revision of a concrete context.
	 */
	inline virtual unsigned long revision() const { return 0; }

	/** @return 0, overload to return the name of a concrete context.
	 */
	inline virtual RtToken rendererName() const { return RI_NULL; }

	/** @return RI_NULL, overload to return the type of a concrete context (RI_ARCHIVE, RI_DRAFT, RI_REALISTIC, or user defined).
	 */
	inline virtual RtToken rendererType() const { return RI_NULL; }
	//@}
}; // CContextCreator

} // namespace RiCPP

#endif // _RICPP_BASERENDERER_CONTEXTCREATOR_H
