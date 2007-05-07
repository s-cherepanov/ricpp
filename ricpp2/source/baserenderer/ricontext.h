#ifndef _RICPP_BASERENDERER_RICONTEXT_H
#define _RICPP_BASERENDERER_RICONTEXT_H

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

#ifndef _RICPP_RICPP_RICPP_H
#include "ricpp/ricpp.h"
#endif // _RICPP_RICPP_RICPP_H

namespace RiCPP {

const RtToken ricpp_archive = "archive"; //!< archive 'renderer' type (IRiContext::rendererType()) to archive RIB code
const RtToken ricpp_draft = "draft"; //!< fast draft renderer type (IRiContext::rendererType()) with limited implementation of the RI
const RtToken ricpp_realistic = "realistic"; //!< photo realistic renderer type (IRiContext::rendererType())  implementing the full RI

/** @brief RenderMan Interface for a context.
 *
 * Interface for a render context called from the CRiCPPBridge and CContextCreator. A concrete render context
 * will extend CBaseRenderer not IRiContext.
 */
class IRiContext : public IRiRoot {
	friend class CContextCreator; //!< A CContextCreator can activate and deactivate the context
protected:
	/** @brief Activate the context
	 *
	 * The context is activated either by an CContextCreator::begin() or a CContextCreator::context().
	 */
	virtual RtVoid activate(void) = 0;
	/** @brief Dectivate the context
	 *
	 * The context is activated either by an CContextCreator::end() or by activating
	 * a different context withC ContextCreator::begin() or CContextCreator::context(),
	 * while this context is active.
	 */
	virtual RtVoid deactivate(void) = 0;
public:
	/** @brief The Major Version of the IRiContext interface
	 *
	 *  The major version changes, if either IRiContext's or IRiRoot's signature changes
	 */
	static const unsigned long majorVersion;

	/** The virtual destructor of the interface
	 */
	inline virtual ~IRiContext() {}

	/** @brief Overload to give the renderer a name
	 *
	 *  @return The name of a renderer used for the IRi::begin() of the front end
	 */
	virtual RtToken rendererName() const = 0;
	
	/** @brief Overload to specify the renderer typ
	 *
	 *  @return The type of a renderer: ricpp_archive, ricpp_draft or ricpp_realistic for information
	 */
	virtual RtToken rendererType() const = 0;
	
	/** @brief Aborts a context
	 *
	 * After an severy error occurs the context will be aborted and destroyed, clean up goes here, deactivate() is not
	 * called for an aborted context.
	 */
	virtual RtVoid abort(void) = 0;
}; // IRiContext
} // namespace RiCPP

#endif // _RICPP_BASERENDERER_RICONTEXT_H
