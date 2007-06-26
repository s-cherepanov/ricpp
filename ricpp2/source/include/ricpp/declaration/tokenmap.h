#ifndef _RICPP_DECLARATION_TOKENMAP_H
#define _RICPP_DECLARATION_TOKENMAP_H

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

/** @file tokenmap.h
 *  @author Andreas Pidde (andreas@pidde.de)
 *  @brief Token dictionary
 */

#ifndef _RICPP_DECLARATION_TOKEN_H
#include "ricpp/declaration/token.h"
#endif // _RICPP_DECLARATION_TOKEN_H

#include <map>
#include <list>

namespace RiCPP {

/** @brief A tokenmap to create unique tokens for strings
 *  @see CToken
 */
class CTokenMap {
	std::map<CToken, RtToken> m_tokenMapper; ///< @brief Maps a token name to it's id
	std::list<const char *> m_strList; ///< @brief List of strings to delete at destructor

public:
	/** @brief Const iterator for the token maps.
	 */
	typedef std::map<CToken, RtToken>::const_iterator const_iterator;
	typedef std::map<CToken, RtToken>::size_type size_type;

	/** @brief Constructor
	 * 
	 * Maps the RtToken out of ricpp.h at the beginning
	 * @exception ExceptRiCPPError if a token cannot be created (out of memory while filling map).
	 */
	CTokenMap();

	/** @brief Destructor
	 */
	inline ~CTokenMap()
	{
		std::list<const char *>::iterator i;
		for ( i = m_strList.begin(); i != m_strList.end(); i++ ) {
			if ( (*i) != 0 )
				delete [](*i);
		}
	}

	/** @brief Searches for a token and creates one if token name is not found.
	 *  @param name A pointer to a token name, the name is copied to a new memory area,
	 *  if a new token must be created.
	 *  @return Unique token
	 *  @exception ExceptRiCPPError if the token cannot be created (out of memory).
	 */
	RtToken findCreate(const char *name) // throw(ERendererException)
	;

	/** @brief Searches for a token and creates one if token name is not found.
	 *  @param token A pointer to a token name, the pointer is stored as is
	 *  @return Unique token (pointer of @a token)
	 *  @exception ExceptRiCPPError if the token cannot be created (out of memory while filling map).
	 */
	RtToken staticFindCreate(RtToken token) // throw(ERendererException)
	;

	/** @brief Searches for a token.
	 *  @param name A pointer to a token name.
	 *  @return != RI_NULL, if token could be found.
	 */
	RtToken find(const char *name) const;
	
	/** @brief Gets the const iterator.
	 *  @return const_iterator for the token map.
	 */
	inline const const_iterator begin() const { return m_tokenMapper.begin(); }

	/** @brief Gets the end const iterator.
	 *  @return end condition for const_iterator for the token map.
	 */
	inline const const_iterator end() const { return m_tokenMapper.end(); }

	/** @brief Gets the size of the token map.
	 *  @return Size of the token map.
	 */
	inline size_type size() const { return m_tokenMapper.size(); }
}; // CTokenMap

} // namespace RiCPP

#endif // _RICPP_DECLARATION_TOKENMAP_H
