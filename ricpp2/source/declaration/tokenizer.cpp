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

/** @file tokenizer.cpp
 *  @author Andreas Pidde (andreas@pidde.de)
 *  @brief Implementation of the representation of a Token (index and its string)
 */

#include "ricpp/declaration/tokenizer.h"

#ifndef _RICPP_RICPP_RENDERERERROR_H
#include "ricpp/ricpp/renderererror.h"
#endif // _RICPP_RICPP_RENDERERERROR_H

using namespace RiCPP;

CToken CTokenizer::findCreate(const char *name)
// throw ERendererError;
{
	const_iterator iter;
	if ( !name )
		name = "";
	if ( (iter = m_tokenMapper.find(name)) == m_tokenMapper.end() ) {
		m_tokenMapper[name] = m_nextIndex++;
		iter = m_tokenMapper.find(name);
		if ( iter == m_tokenMapper.end() ) {
			throw ERendererError(RIE_NOMEM, RIE_SEVERE, __LINE__, __FILE__, "Could not create token \"%s\"", name);
		}
	}
	return CToken(iter->first.c_str(), iter->second);
}

bool CTokenizer::find(const char *name, CToken &c) const
{
	const_iterator iter;
	if ( !name )
		name = "";
	if ( (iter = m_tokenMapper.find(name)) == m_tokenMapper.end() ) {
		return false;
	}
	c.set(iter->first.c_str(), iter->second);
	return true;
}

bool CTokenizer::find(unsigned long id, CToken &c) const
{
	const_iterator iter;

	for ( iter = m_tokenMapper.begin(); iter != m_tokenMapper.end(); iter++ ) {
		if ( iter->second == id ) {
			c.set(iter->first.c_str(), iter->second);
			return true;
		}
	}
	return false;
}
