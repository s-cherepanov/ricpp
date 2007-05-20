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

namespace RiCPP {
/** @brief A token as token number/char * pair, the tokens are managed by CTokenizer
 *  @see CTokenizer
 */
class CToken {
	friend class CTokenizer;

	const char *m_name; ///< @brief Pointer to the name of the token
	unsigned long m_id; ///< @brief The token number used to identify a token

	/** @brief Constructor, is constructed by searching in CTokenizer
	 * @param aName name of the token
	 * @param anId id of the token
	 */
	inline CToken(const char *aName, unsigned long anId) : m_name(aName), m_id(anId) {}

	/** @brief Sets the members of this token, set by CTokenizer
	 * @param aName name of the token
	 * @param anId id of the token
	 */
	inline void set(const char *aName, unsigned long anId)
	{
		m_name = aName;
		m_id = anId;
	}

public:
	/** @brief Standard constructor, empty token
	 */
	inline CToken()
	{
		m_name = "";
		m_id=0;
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
	inline bool operator==(const CToken &t) const { return m_id == t.m_id; }

	/** @brief Compare a token by its id using !=
	 * @param t Token to compare with
	 * @return true, if tokens are not equal because their ids are not equal
	 */
	inline bool operator!=(const CToken &t) const { return m_id != t.m_id; }

	/** @brief Compare a token by its id using <
	 * @param t Token to compare with
	 * @return true, m_id is less than the m_id of t
	 */
	inline bool operator<(const CToken &t) const { return m_id < t.m_id; }

	/** @brief Compare a token by its id using <=
	 * @param t Token to compare with
	 * @return true, m_id is less than or equal to the m_id of t
	 */
	inline bool operator<=(const CToken &t) const { return m_id <= t.m_id; }

	/** @brief Compare a token by its id using >
	 * @param t Token to compare with
	 * @return true, m_id is greater than the m_id of t
	 */
	inline bool operator>(const CToken &t) const { return m_id > t.m_id; }

	/** @brief Compare a token by its id using >=
	 * @param t Token to compare with
	 * @return true, m_id is greater than or equal to the m_id of t
	 */
	inline bool operator>=(const CToken &t) const { return m_id >= t.m_id; }

	/** @brief Copy a token
	 * @param t Token to copy
	 * @return *this
	 */
	inline CToken &operator=(const CToken &t)
	{
		if ( &t == this )
			return *this;
		m_name = t.m_name;
		m_id = t.m_id;
		return *this;
	}

	/** Gets the name of a token
	 * @return The name of a token
	 */
	const char *name() const { return m_name; }

	/** Gets the id of a token
	 * @return The id of a token
	 */
	unsigned long id() const { return m_id; }
}; // CToken

} // namespace RiCPP

#endif // _RICPP_DECLARATION_TOKEN_H
