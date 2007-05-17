#ifndef RICPP_RICPP_MODESTACK_H
#define RICPP_RICPP_MODESTACK_H

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

#ifndef RICPP_RICPP_MODES_H
#include "ricpp/ricpp/modes.h"
#endif // RICPP_RICPP_MODES_H

#ifndef RICPP_RICPP_REQUESTS_H
#include "ricpp/ricpp/requests.h"
#endif // RICPP_RICPP_REQUESTS_H

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
	 *  for the bits of valid modes for a request.
	 *  see also UPS89: Valid contexts for RenderMan Procedurte calls, pp.56
     *  and RISPEC3.2: Definition of the new RI3.2 procs
	 */
	unsigned short m_requests[N_REQUESTS];
public:
	/** @brief Initializes the valid contexts for all RI routines.
	 */
	CValidModes();

	/** @brief Queries the validity of a request for a given mode.
	 *
	 *  @param idxRequest Index of a request (interface call) to test if valid in mode idxMode, is a REQ_ constant (e.g. REQ_SPHERE)
	 *  @param idxMode Index of a mode as MODE_ constant (e.g. MODE_WORLD), norally the current mode
	 *  @return true if request is valid, false otherwise.
	 */
	bool isValid(EnumRequests idxRequest, EnumModes idxMode) const;
}; // CValidModes

/** @brief Stores modes and check validity of requests
 *
 *  Tries to restaurate the stack, if an illegal block is closed, e.g.
 *  if an attribute block is closed all blocks nested inside this
 *  attribute block are also closed.
 */
class CModeStack {
	CValidModes m_validModes; //!< Used to check validy of a request inside a given mode.
	std::vector<EnumModes> m_modes; //!< All modes (nesting of modes), MODE_OUTSIDE is not on the stack
	std::vector<RtToken> m_solidBlocks; //!< Tokens of nested solid blocks

protected:
	/** @brief Enters a new nesting to the modes, do not test if valid (is done by the interface before)
	 *
	 * @param newMode New mode nesting
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

public:
	/** @brief Initializing of the mode, normally starts outside any blocks. 
	 */
	inline CModeStack( )
	{
	}

	/** @brief Virtual destructor.
	 */
	inline virtual ~CModeStack() {}

	/** @brief The modes
	 */
	//@{
	virtual void begin();
	virtual void end();

	virtual void frameBegin();
	virtual void frameEnd();

	virtual void worldBegin();
	virtual void worldEnd();

	virtual void attributeBegin();
	virtual void attributeEnd();

	virtual void transformBegin();
	virtual void transformEnd();

    virtual void solidBegin(RtToken type);
    virtual RtToken solidEnd();

	virtual void objectBegin();
	virtual void objectEnd();

    virtual void motionBegin();
    virtual void motionEnd();
	//@}

	/** @brief Test if a request req is valid in the current mode.
	 *
	 *  @param req Index of the request to test.
	 *  @return true if the request req is valid in the current mode.
	 */
	inline virtual bool validRequest(EnumRequests req) { return m_validModes.isValid(req, curMode()); }

	/** @brief The current mode
	 *  @return The current mode
	 */
	inline virtual EnumModes curMode() const { return m_modes.empty() ? MODE_OUTSIDE : m_modes.back(); }
}; // CMode

}

#endif // RICPP_RICPP_MODESTACK_H
