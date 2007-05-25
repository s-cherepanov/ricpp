#ifndef _RICPP_DECLARATION_TOKEN_H
#define _RICPP_DECLARATION_TOKEN_H

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

/** @file token.h
 *  @author Andreas Pidde (andreas@pidde.de)
 *  @brief Representation of a Token (index and its string)
 */

#ifndef _RICPP_RICPP_RICPP_H
#include "ricpp/ricpp/ricpp.h"
#endif // _RICPP_RICPP_RICPP_H

#include <string.h>

namespace RiCPP {

	/** @brief A registered token, the tokens are managed by CTokenizer
	 *
	 * Just stores a character pointer. CToken is used by CTokenManager.
	 * The compare functions are used. CToken can be used ouside CTokenizer
	 * to iterate through all declared tokens.
	 *
	 * @see CTokenizer
	 */
	class CToken
	{
		friend class CTokenizer;
		RtToken m_token;

		/** @brief Constructor, is constructed by searching in CTokenizer
		 * @param aToken token
		 */
		inline CToken(RtToken aToken)
		{
			m_token = aToken;
		}

		/** @brief Sets the members of this token, set by CTokenizer
		 * @param aToken name of the token
		 */
		inline void set(RtToken aToken)
		{
			m_token = aToken;
		}
	public:

		/** @brief Standard constructor, empty token
		 */
		inline CToken()
		{
			m_token = RI_NULL;
		}

		/** @brief Copy constructs a token
		 * @param t Token to copy
		 * @return *this
		 */
		inline CToken(const CToken &t) { *this = t; }

		/** @brief Compare a token by its id using ==
		 * @param t Token to compare with
		 * @return true, if tokens are equal because their ids are equal
		 */
		inline bool operator==(const CToken &t) const { return strcmp(name(), t.name()) == 0; }

		/** @brief Compare a token by its id using !=
		 * @param t Token to compare with
		 * @return true, if tokens are not equal because their ids are not equal
		 */
		inline bool operator!=(const CToken &t) const { return strcmp(name(), t.name()) != 0; }

		/** @brief Compare a token by its id using <
		 * @param t Token to compare with
		 * @return true, m_id is less than the m_id of t
		 */
		inline bool operator<(const CToken &t) const { return strcmp(name(), t.name()) < 0; }

		/** @brief Compare a token by its id using <=
		 * @param t Token to compare with
		 * @return true, m_id is less than or equal to the m_id of t
		 */
		inline bool operator<=(const CToken &t) const { return strcmp(name(), t.name()) <= 0; }

		/** @brief Compare a token by its id using >
		 * @param t Token to compare with
		 * @return true, m_id is greater than the m_id of t
		 */
		inline bool operator>(const CToken &t) const { return strcmp(name(), t.name()) > 0; }

		/** @brief Compare a token by its id using >=
		 * @param t Token to compare with
		 * @return true, m_id is greater than or equal to the m_id of t
		 */
		inline bool operator>=(const CToken &t) const { return strcmp(name(), t.name()) >= 0; }

		/** @brief Copy a token
		 * @param t Token to copy
		 * @return *this
		 */
		inline CToken &operator=(const CToken &t)
		{
			if ( &t == this )
				return *this;
			m_token = t.m_token;
			return *this;
		}

		/** Gets the id of a token, thats the pointer to the unique token
		 * @return The id of a token
		 */
		inline RtToken id() const { return m_token; }

		/** Gets the name of a token
		 * @return The name of a token ( not 0 )
		 */
		inline const char *name() const { return m_token && *m_token ? m_token : RI_EMPTY; }
	}; // CToken


} // namespace RiCPP

#endif // _RICPP_DECLARATION_TOKEN_H
