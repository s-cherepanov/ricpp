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
	TypeModeBits m_requests[N_REQUESTS];
public:
	/** @brief Initializes the valid contexts for all RI routines.
	 */
	CValidModes();

	/** @brief Checks the validity of a request for a given mode.
	 *
	 *  @param idxRequest Index of a request (interface call, a @c REQ_ constant like REQ_SPHERE) to test if valid in mode idxMode
	 *  @param idxMode Set of @c MODEBIT_ constant (e.g. MODEBIT_WORLD), combined with or, normally the current mode
	 *  @return true, if request is valid inside the mode, false otherwise.
	 */
	bool isValid(EnumRequests idxRequest, TypeModeBits idxMode) const;
}; // CValidModes


/** @brief Stores nested modes and checks validity of requests
 *
 * The checking is done in the member functions of CBaseRenderer objects
 * @see CBaseRenderer
 */
class CModeStack {
	CValidModes m_validModes; //!< Used to check validy of a request inside a given mode.
	std::vector<TypeMode> m_modes; //!< All modes (nesting of modes), MODE_OUTSIDE is not on the stack
	std::vector<TypeModeBits> m_modeBits; //!< All modes (nesting of modes), transparent modes are 'ored' to the mode bits of outer blocks.

protected:
	/** @brief Enters a new nesting to the modes, do not test if valid (is done by the interface before)
	 *
	 * @param newMode New mode for nesting
	 */
	inline virtual void push(EnumModes newMode, TypeModeBits newModeBits)
	{
		m_modes.push_back(newMode);
		m_modeBits.push_back(newModeBits);
	}

	/** @brief Removes nesting from the modes, do not test if valid (is done by the interface before)
	 */
	inline virtual void pop()
	{
		if ( !m_modes.empty() ) {	
			m_modes.pop_back();
		}
		if ( !m_modeBits.empty() ) {	
			m_modeBits.pop_back();
		}
	}

	/** @brief Clears the mode stack.
	 *
	 * The mode stack is cleared by end()
	 */
	inline virtual void clear()
	{
		m_modes.clear();
		m_modeBits.clear();
	}

public:
	/** @brief Const iterator for the elements.
	 */
	typedef std::vector<TypeMode>::const_iterator const_iterator;

	/** @brief Size type for the number of stored elements
	 */
	typedef std::vector<TypeMode>::size_type size_type;


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
	 * Called by the CRenderState. The @c begin - @c end calls must be balanced.
	 */
	//@{

	/** @brief Begins a new rendering context, called once for each context for initialization
	 * @see CBaseRenderer::begin()
	 */
	virtual void contextBegin();
	/** @brief Ends a rendering context, called once for each context before deletion
	 * @see CBaseRenderer::end()
	 */
	virtual void contextEnd();

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
	 * Inside frame, world, attribute, transform, archive, and begin blocks
	 */
	virtual void objectBegin();
	/** @brief Ends an object block
	 */
	virtual void objectEnd();

	/** @brief Begins a rib archive block
	 *
	 * Inside frame, world, attribute, transform, object, and begin blocks
	 */
	virtual void archiveBegin();

	/** @brief Ends an archive block
	 */
	virtual void archiveEnd();

	/** @brief Begins a (transparent) resource block
	 *
	 * Inside frame, world, attribute, transform, object, and begin blocks
	 */
	virtual void resourceBegin();

	/** @brief Ends a resource block
	 */
	virtual void resourceEnd();

	/** @brief Begins a conditional if block
	 *
	 * Inside frame, world, attribute, transform, object, and begin blocks
	 */
	virtual void ifBegin();

	/** @brief Begins a conditional if-else block
	 *
	 * Inside if blocks
	 */
	virtual void ifElseBegin();

	/** @brief Begins a conditional else block
	 *
	 * Inside if or ifElse blocks
	 */
	virtual void elseBegin();

	/** @brief Ends a conditional block
	 */
	virtual void ifEnd();

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
	inline virtual bool validRequest(EnumRequests req) const { return m_validModes.isValid(req, curModeBits()); }

	/** @brief The current mode
	 *  Transparent modes are not considered
	 *  @return The current mode
	 *  @see EnumRequests  
	 */
	inline virtual EnumModes curMode() const
	{
		return m_modes.empty() ? 0 : m_modes.back();
	}

	/** @brief The current mode bits
	 *  @return The current mode bits (incl. the transparent blocks)
	 *  @see EnumRequests  
	 */
	inline virtual TypeModeBits curModeBits() const
	{
		return m_modes.empty() ? MODE_BIT_OUTSIDE : m_modes.back();
	}

	/** @brief Constant iterator, begin of the modes.
	 * @return Constant iterator, points to the first element of the active dictionary.
	 */
	inline const_iterator begin() const { return m_modes.begin(); }

	/** @brief Constant iterator, end of the modes.
	 * @return Constant iterator, points behind the last element of the active dictionary.
	 */
	inline const_iterator end() const { return m_modes.end(); }
	
	/** @brief Gets the size of the modes.
	 * @return Size of the active dictionary.
	 */
	inline size_type size() const { return m_modes.size(); }
}; // CMode

}

#endif // RICPP_RENDERSTATE_MODESTACK_H
