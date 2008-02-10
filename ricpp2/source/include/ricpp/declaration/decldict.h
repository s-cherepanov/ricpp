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

namespace RiCPP {

/** @brief Dictionary for the declarations of a rendering context
 *
 * CBaseRenderer::declare() is used to declare the elements that
 * are stored in the dictionary. The declaration
 * dictionary is part of the render state CRenderState. 
 *
 * @see CBaseRenderer::declare(), CRenderState
 */
class CDeclarationDictionary
{
	/** @brief Current active declarations.
	 *
	 * Only the copied pointers to objects of m_all.
	 *
	 */
	class TemplObjPtrRegistry<RtToken, const CDeclaration *> m_active;
	
	/** @brief All declarations.
	 *
	 * All declarations are remembered, because even not active ones
	 * can be referenced by stored parameters. They are
	 * freed at the end of their rendering context.
	 */
	class std::list<const CDeclaration *> m_all;

	CTokenMap m_tokenMap;              ///< Registered tokens

public:
	/** @brief Const iterator for the elements.
	 */
	typedef TemplObjPtrRegistry<RtToken, CDeclaration const *>::const_iterator const_iterator;

	/** @brief Size type for the number of stored elements
	 */
	typedef TemplObjPtrRegistry<RtToken, CDeclaration const *>::size_type size_type;

	/** @brief Constructor.
	 *
	 * @a m_active holds only pointers to declarations, therefore no memory management
	 * is done for it. The @a m_all is used to free the declarations at the end of
	 * their rendering context.
	 *
	 * @see m_active, m_all
	 */
	inline CDeclarationDictionary()
		: m_active(false)
	{
		m_tokenMap.defaultTokens();
	}
	
	/** @brief Destructor.
	 *
	 * Objects are deleted at the end of the rendering context. All
	 * declarations are destroyed here (they were kept in @a m_all).
	 */
	~CDeclarationDictionary();
	
	/** @brief Finds a declaration for a token.
	 *
	 * This member function is used to get a declaration without modifying
	 * the dictionary. Can only be used, if the number of color components
	 * has not changed since declaration.
	 *
	 * @param token Token (unique string)  of the name of the declaration
	 * @return 0, if not found, pointer to declaration of @a name otherwise
	 * @see find(RtToken name, const CColorDescr &colorDescr), CTokenMap
	 */
	inline const CDeclaration *find(RtToken token) const
	{
		return m_active.findObj(token);
	}
	
	/** @brief Searches a declaration using three steps.
	 *
	 *  The three steps are
	 *  <ol>
	 *  <li> Search for namespace:table:var </li>
	 *  <li> table:var </li>
	 *  <li> var </li>
	 *  </ol>
	 *
	 * @param tableNamespace The namespace
	 * @param table The table
	 * @param var The stripped name of the variable
	 * @return 0, if not found, pointer to declaration of @a name otherwise
	 * @see find(RtToken, unsigned int), CDeclaration
	 */
	const CDeclaration *find(RtToken tableNamespace, const char *table, const char *var) const;

	/** @brief Finds a declaration for a token, may change number of color components.
	 *
	 * This member finds a declaration and checks the number of color components, if
	 * the type of the declaration is a color. If the number of components has changed, a copy
	 * of the corrected declaration is stored as the new active one. The old, overwritten
	 * declaration remains stored in m_all.
	 *
	 * @param token Token of the name of the declaration.
	 * @param curColorDescr The current number of color components and RGB transformation
	 * @return 0, if not found, pointer to declaration of @a name otherwise.
	 * @exception ExceptRiCPPError Can throw this if @c RIE_NOMEM for a new color declaration.
	 * @see find(RtToken), CTokenMap
	 */
	const CDeclaration *findAndUpdate(RtToken token, const CColorDescr &curColorDescr)
	// throw (ExceptRiCPPError)
	;

	/** @brief Searches a declaration with color update, using three steps.
	 *
	 * @param tableNamespace The namespace
	 * @param table The table
	 * @param var The stripped name of the variable
	 * @param curColorDescr The current number of color components and RGB transformation.
	 * @return 0, if not found, pointer to declaration of @a name otherwise
	 * @see find(const char *, const char *, const char *, const CTokenMap &), CDeclaration
	 */
	const CDeclaration *findAndUpdate(
		RtToken tableNamespace,
		const char *table,
		const char *var,
		const CColorDescr &curColorDescr)
	// throw(ExceptRiCPPError)
	;

	/** @brief Adds (overwrites) a new declaration.
	 * 
	 * Adds a new declaration to @a m_active and @a m_all, it can overwrite an existing one
	 * with the same name.
	 * 
	 * @warning The old declaration is kept in @a m_all. The referenced
	 * object is used, it is not copied, so do not destroy it yourself.
	 * 
	 * @param decl New declaration, must not be 0
	 */
	void add(CDeclaration *decl);

	/** @brief Constant iterator, begin of the active dictionary.
	 * @return Constant iterator, points to the first element of the active dictionary.
	 */
	inline const_iterator begin() const { return m_active.begin(); }

	/** @brief Constant iterator, end of the active dictionary.
	 * @return Constant iterator, points behind the last element of the active dictionary.
	 */
	inline const_iterator end() const { return m_active.end(); }
	
	/** @brief Gets the size of the active dictionary.
	 * @return Size of the active dictionary.
	 */
	inline size_type size() const { return m_active.size(); }

	/** @brief Gets the interface of the tokenmap.
	 * @return Reference to the tokenmap
	 */
	inline CTokenMap &tokenMap() { return m_tokenMap; }
	inline const CTokenMap &tokenMap() const { return m_tokenMap; }

}; // CDeclarationDictionary

} // namespace RiCPP

#endif // _RICPP_DECLARATION_DECLDICT_H
