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

/** @file ricontext.h
 *  @author Andreas Pidde (andreas@pidde.de)
 *  @brief Contains the declaration calling interface of the backend.
 */

#ifndef _RICPP_RICPP_RICPP_H
#include "ricpp/ricpp/ricpp.h"
#endif // _RICPP_RICPP_RICPP_H

namespace RiCPP {

/** @brief RenderMan Interface for a backend render context.
 *
 * Interface for a render context called from the CRiCPPBridge and CContextCreator. A concrete render context
 * will extend CBaseRenderer not IRiContext.
 */
class IRiContext : public IRiRoot {
	friend class CContextCreator; //!< A CContextCreator can activate and deactivate the context

protected:
	/** @brief Activate the context
	 *
	 * The context is activated by CContextCreator::context() (indirectly by CRiCPPBridge::context())
	 * Also begin() is used for activation, if a context 'begins' it is *not* explicitely activated
	 * (because the context may need some special handling in this case).
	 */
	virtual RtVoid activate(void) = 0;

	/** @brief Dectivate the context
	 *
	 * The context is deactivated either by an CContextCreator::end() or by activating
	 * a different context with CContextCreator::begin() or CContextCreator::context(),
	 * while this context is active. Again indirectly deactivated by the appropriate
	 * CRiCPPBridge member functions.
	 * Also end() and abort() is used for deactivation, if a context 'ends' or is 'aborted'
	 * it is *not* explicitly deactivated (because the context may need some special handling
	 * in this case).
	 */
	virtual RtVoid deactivate(void) = 0;

public:
	/** @brief The virtual destructor of the interface.
	 */
	inline virtual ~IRiContext() {}

	/** @brief The Major Version of the IRiContext interface.
	 *
	 *  The major version changes, if either IRiContext's or IRiRoot's signature changes -
	 *  that means if the backend calling interface changes.
	 */
	static const unsigned long riContextMajorVersion;

	/** @brief The major version of the IRiContext interface
	 *
	 *  @return The major version number of the context
	 */
	inline virtual unsigned long majorVersion() const { return riContextMajorVersion; }

	/** @brief Overload for the minor version of the interface
	 *
	 *  @return The minor version number of the context
	 */
	inline virtual unsigned long minorVersion() const = 0;

	/** @brief Overload for the revision number of the interface
	 *
	 *  @return The revision number of the context
	 */
	inline virtual unsigned long revision() const = 0;

	/** @brief Overload to give the renderer a name
	 *
	 *  @return The name of a renderer used by the IRi::begin(RtString name) of the frontend
	 */
	virtual RtToken rendererName() const = 0;
	
	/** @brief Overload to specify the renderer type
	 *
	 *  @return The type of a renderer: RI_ARCHIVE, RI_DRAFT, RI_REALISTIC or user defined for information
	 */
	virtual RtToken rendererType() const = 0;
	
	/** @brief Overload to abort a context
	 *
	 * After a severe error occurs the context will be aborted and destroyed, clean up goes here, deactivate()
	 * is *not* called for an aborted context. So if a context need some action after aborted,
	 * this must be done here (because the context can be in a state that do not allow an explicit deactivation)
	 */
	virtual RtVoid abort(void) = 0;
}; // IRiContext
} // namespace RiCPP

#endif // _RICPP_BASERENDERER_RICONTEXT_H
