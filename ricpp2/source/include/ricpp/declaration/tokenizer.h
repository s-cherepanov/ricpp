#ifndef _RICPP_DECLARATION_TOKENIZER_H
#define _RICPP_DECLARATION_TOKENIZER_H

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

/** @file tokenizer.h
 *  @author Andreas Pidde (andreas@pidde.de)
 *  @brief Token factory
 */

#ifndef _RICPP_DECLARATION_TOKEN_H
#include "ricpp/declaration/token.h"
#endif // _RICPP_DECLARATION_TOKEN_H

#include <map>
#include <string>

namespace RiCPP {

/** @brief A tokenizer to create unique tokens for strings
 *  @see CToken
 */
class CTokenizer {
	std::map<std::string, unsigned long> m_tokenMapper; ///< @brief Maps a token anme to it's id
	unsigned long m_nextIndex; ///< @brief The next id for a token
public:
	/** @brief Const iterator for the token maps.
	 */
	typedef std::map<std::string, unsigned long>::const_iterator const_iterator;

	/** @brief Constructor, "" maps to 0 at the beginning
	 */
	inline CTokenizer() : m_nextIndex(1)
	{
		m_tokenMapper[""] = 0;
	}

	/** @brief Destructor
	 */
	inline ~CTokenizer() {}

	/** @brief Searches for a token and creates one if token name is not found
	 *
	 *  Throws ERendererError if the token cannot be created (out of memory).
	 *
	 *  @param name A pointer to a token name
	 *  @return Token with name and id
	 */
	inline CToken findCreate(const char *name);

	/** @brief Searches for a token
	 *  @param name A pointer to a token name
	 *  @retval c If the token is found, it is copied to c
	 *  @return true, if token could be found
	 */
	bool find(const char *name, CToken &c) const;

	/** @brief Searches for a token by id (normally not used)
	 *  @param id Id of the token
	 *  @retval c If the token is found, it is copied to c
	 *  @return true, if token could be found
	 */
	bool find(unsigned long id, CToken &c) const;
}; // CTokenizer

} // namespace RiCPP

#endif // _RICPP_DECLARATION_TOKENIZER_H
