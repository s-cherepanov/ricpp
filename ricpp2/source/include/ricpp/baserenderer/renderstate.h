#ifndef _RICPP_BASERENDERER_RENDERSTATE_H
#define _RICPP_BASERENDERER_RENDERSTATE_H

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

/** @file renderstate.h
 *  @author Andreas Pidde (andreas@pidde.de)
 *  @brief Container for the render state objects (like modes, options, attributes)
 */

#ifndef _RICPP_RICPP_MODESTACK_H
#include "ricpp/ricpp/modestack.h"
#endif // _RICPP_RICPP_MODESTACK_H

namespace RiCPP {

/** @brief The container for the render state objects
 */
class CRenderState {
	bool m_deleteModeStack;   ///< Delete the modestack if object is destroyed
	CModeStack *m_modeStack; ///< Pointer to a mode stack

public:
	/** @brief Initializes the object
	 *
	 * At creation all is cleared. The state objects
	 * shall get initialized at begin() of the rendering context.
	 * All state objects must get a value != 0, begin shall throw
	 * an error if some objects cannot be created (valid() == false)
	 * and further rendering is not possible (context gets aborted)
	 */
	inline CRenderState()
	{
		m_modeStack = 0;
		m_deleteModeStack = false;
	}

	/** @brief Destroyes the object
	 *
	 *  State objects can get deleted if the delete-boolean of a state object is true
	 */
	~CRenderState();

	/** @brief Tests, if all state objects are set
	 *
	 *  After the begin() of the rendering state all state objects should be set.
	 *  @see CRenderState()
	 */
	inline bool valid() const {
		return m_modeStack != 0;
	}

	/** The mode stack
	 *
	 *  @return A pointer to the mode stack
	 */
	inline CModeStack *modeStack() const { return m_modeStack; }

	/** Sets the mode stack
	 *
	 *  The mode stack will be set typically at begin() of the rendering context
	 *
	 *  @param aModeStack A pointer to a valid mode stack
	 *  @param deleteModeStack Set this to true if aModeStack shall be deleted at the
	 *         destructioon of this CRenderState object
	 */
	void modeStack(CModeStack *aModeStack, bool deleteModeStack)
	{
		m_modeStack = aModeStack;
		m_deleteModeStack = deleteModeStack;
	}

}; // CRenderState

} // namespace RiCPP

#endif // _RICPP_BASERENDERER_RENDERSTATE_H
