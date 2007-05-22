#ifndef _RICPP_DECLARATION_DECLDICT_H
#define _RICPP_DECLARATION_DECLDICT_H

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

/** @file decldict.h
 *  @author Andreas Pidde (andreas@pidde.de)
 *  @brief Dictionary for declarations
 */

#ifndef _RICPP_DECLARATION_DECLARATION_H
#include "ricpp/declaration/declaration.h"
#endif // _RICPP_DECLARATION_DECLARATION_H

#ifndef _RICPP_TOOLS_OBJPTRREGISTRY_H
#include "ricpp/tools/objptrregistry.h"
#endif // _RICPP_TOOLS_OBJPTRREGISTRY_H

#include <list>

namespace RiCPP {

/** @brief Dictionary for the declarations of a rendering context
 */
class CDeclarationDictionary
{
	class TObjPtrRegistry<CToken, const CDeclaration *> m_active;
	class std::list<const CDeclaration *> m_all;
public:
	/** @brief Const iterator for the elements.
	 */
	typedef TObjPtrRegistry<CToken, CDeclaration const *>::const_iterator const_iterator;
	/** @brief Size type for the number of stored elements
	 */
	typedef TObjPtrRegistry<CToken, CDeclaration const *>::size_type size_type;

public:
	inline CDeclarationDictionary() : m_active(false) {}
	~CDeclarationDictionary();
	inline const CDeclaration *find(const CToken &name) const
	{
		return m_active.findObj(name);
	}
	const CDeclaration *find(const CToken &name, unsigned int curColorSize);
	bool add(CDeclaration *decl);
	inline const_iterator begin() const { return m_active.begin(); }
	inline const_iterator end() const { return m_active.end(); }
	inline size_type size() const { return m_active.size(); }
}; // CDeclarationDictionary

} // namespace RiCPP

#endif // _RICPP_DECLARATION_DECLDICT_H
