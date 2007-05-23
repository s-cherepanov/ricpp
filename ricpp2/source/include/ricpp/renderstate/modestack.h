#ifndef RICPP_RENDERSTATE_MODESTACK_H
#define RICPP_RENDERSTATE_MODESTACK_H

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

/** @file modestack.h
 *  @author Andreas Pidde (andreas@pidde.de)
 *  @brief Handling of the modes (blocks and nesting) of the interface.
 */

#ifndef RICPP_RICPP_RICPP_H
#include "ricpp/ricpp/ricpp.h"
#endif // RICPP_RICPP_RICPP_H

#ifndef RICPP_RENDERSTATE_REQUESTS_H
#include "ricpp/renderstate/requests.h"
#endif // RICPP_RENDERSTATE_REQUESTS_H

#ifndef RICPP_RENDERSTATE_MODES_H
#include "ricpp/renderstate/modes.h"
#endif // RICPP_RENDERSTATE_MODES_H

#include <vector>

namespace RiCPP {

/** @brief Class to test the valid contexts for RenderMan interface calls.
 */
class CValidModes
{
	/** @brief Stores the information for valid modes for requests in a table.
	 *
	 *  m_requests uses the REQ_ constants (e.g. REQ_SPHERE) as indicees.
	 *  It stores a bitfield as content: MODE_BIT_ constants (e.g. MODE_BIT_WORLD)
	 *  for the bits of valid modes for a request. If bitwise 'AND'ed with the
	 *  bit of the current mode, a request is valid, if the result is not 0.
	 *  See also UPS89: Valid contexts for RenderMan Procedurte calls, pp.56
     *  and RISPEC3.2: Definition of the new RI3.2 procs
	 */
	unsigned short m_requests[N_REQUESTS];
public:
	/** @brief Initializes the valid contexts for all RI routines.
	 */
	CValidModes();

	/** @brief Checks the validity of a request for a given mode.
	 *
	 *  @param idxRequest Index of a request (interface call, a @c REQ_ constant like REQ_SPHERE) to test if valid in mode idxMode
	 *  @param idxMode Index of a mode as @c MODE_ constant (e.g. MODE_WORLD), normally the current mode
	 *  @return true, if request is valid inside the mode, false otherwise.
	 */
	bool isValid(EnumRequests idxRequest, EnumModes idxMode) const;
}; // CValidModes

/** @brief Stores nested modes and checks validity of requests
 *
 * The checking is done in the member functions of CBaseRenderer objects
 * @see CBaseRenderer
 */
class CModeStack {
	CValidModes m_validModes; //!< Used to check validy of a request inside a given mode.
	std::vector<EnumModes> m_modes; //!< All modes (nesting of modes), MODE_OUTSIDE is not on the stack
protected:
	/** @brief Enters a new nesting to the modes, do not test if valid (is done by the interface before)
	 *
	 * @param newMode New mode for nesting
	 */
	inline virtual void push(EnumModes newMode)
	{
		m_modes.push_back(newMode);
	}

	/** @brief Removes nesting from the modes, do not test if valid (is done by the interface before)
	 *
	 * @return The previous current mode
	 */
	inline virtual EnumModes pop()
	{
		EnumModes prev = MODE_OUTSIDE;
		if ( !m_modes.empty() ) {	
			m_modes.back();
			m_modes.pop_back();
		}
		return prev;
	}

	/** @brief Clears the mode stack.
	 *
	 * The mode stack is cleared by end()
	 */
	inline virtual void clear()
	{
		m_modes.clear();
	}

public:
	/** @brief Initializing of the mode, normally starts outside any blocks. 
	 */
	inline CModeStack( )
	{
	}

	/** @brief Virtual destructor.
	 */
	inline virtual ~CModeStack() {}

	/** @defgroup mode_group CModeStack, the modes
	 * @brief Stacks the modes of the RenderMan interface
	 *
	 * Called by the CRenderState. The @cbegin - @c end calls must be balanced.
	 */
	//@{
	/** @brief Begins a new rendering context, called once for each context for initialization
	 * @see CBaseRenderer::begin()
	 */
	virtual void begin();
	/** @brief Ends a rendering context, called once for each context before deletion
	 * @see CBaseRenderer::end()
	 */
	virtual void end();

	/** @brief Begins a new frame (optional)
	  * Directly in begin-block, cannot be nested
	  */
	virtual void frameBegin();
	/** @brief Ends a frame
	 */
	virtual void frameEnd();

	/** @brief Begin the world block to place objects
	 *
	 * inside frame block or begin-block, cannot be nested.
	 */
	virtual void worldBegin();
	/** @brief End the world block
	 */
	virtual void worldEnd();

	/** @brief Begins an attribute block
	 *
	 * Can be used inside world, object, solid, transformation and other attribute blocks
	 * @see CBaseRenderer::attributeBegin()
	 */
	virtual void attributeBegin();
	/** @brief Ends a block of attributes
	 * @see CBaseRenderer::attributeEnd()
	 */
	virtual void attributeEnd();

	/** @brief Begins a new transformation block
	 *
	 * Inside other transformation blocks, attribute, solid and object blocks,
	 * world or for camera/light placings inside frame or begin block
	 */
	virtual void transformBegin();
	/** @brief Ends transformation block
	 */
	virtual void transformEnd();

	/** @brief Begins a new solid geometry block
	 *
	 * Inside other solid blocks (primitive-blocks must be the 'leaves'),
	 * transformation, attribute, world and object blocks
	 */
    virtual void solidBegin();
	/** @brief Ends a solid block
	 */
    virtual void solidEnd();

	/** @brief Begins a new object block for retained geometry
	 *
	 * Inside frame and begin block, cannot be nested
	 */
	virtual void objectBegin();
	/** @brief Ends an object block
	 */
	virtual void objectEnd();

	/** @brief Begins a new motion block
	 *
	 * Inside object, world, attribute or transform block, cannot be nested.
	 */
    virtual void motionBegin();
	/** @brief Ends a motion block
	 */
    virtual void motionEnd();
	//@}

	/** @brief Test if a request @a req is valid in the current mode.
	 *
	 *  @param req Index of the request to test.
	 *  @return true, if the request req is valid in the current mode.
	 *  @see EnumRequests  
	 */
	inline virtual bool validRequest(EnumRequests req) const { return m_validModes.isValid(req, curMode()); }

	/** @brief The current mode
	 *  @return The current mode
	 *  @see EnumRequests  
	 */
	inline virtual EnumModes curMode() const { return m_modes.empty() ? MODE_OUTSIDE : m_modes.back(); }
}; // CMode

}

#endif // RICPP_RENDERSTATE_MODESTACK_H