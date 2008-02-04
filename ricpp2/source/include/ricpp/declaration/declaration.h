#ifndef _RICPP_DECLARATION_DECLARATION_H
#define _RICPP_DECLARATION_DECLARATION_H

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

/** @file declaration.h
 *  @author Andreas Pidde (andreas@pidde.de)
 *  @brief Header for Ri declarations
 */
#ifndef _RICPP_DECLARATION_TOKENMAP_H
#include "ricpp/declaration/tokenmap.h"
#endif // _RICPP_DECLARATION_TOKENMAP_H

#ifndef _RICPP_RICPP_PARAMCLASSES_H
#include "ricpp/ricpp/paramclasses.h"
#endif // _RICPP_RICPP_PARAMCLASSES_H

#ifndef _RICPP_RICPP_TYPES_H
#include "ricpp/ricpp/types.h"
#endif // _RICPP_RICPP_TYPES_H

#include <string>

namespace RiCPP {
/** @brief Representation of a (possibly inline) declaration
 *
 * All declarations are stored and handled in a CDeclarationDictionary
 * of the state of a rendering context.
 * If the declaration has a color type, the number of elements @c m_typesize is the
 * size, at the time the declaration is used. If the size changes, a new declaration is
 * created, if a parameter of the declaration is used. Declarations that are not
 * inline, are not destroyed before the rendering context ends.
 * @see CDeclarationDictionary, @subpage page_decl
 */
class CDeclaration {
	/** @brief Name of the declaration.
	 * The name has the form [namespace:][table:]var
	 * (QRM notation, namespace ENamespaces, table e.g. surface shader name, var variable name),
	 * the name as given in the declaration is the token that is stored in the
	 * dictionary. The declaration can be overwritten by excactly the same name.
	 * @see var()
	 */
	std::string m_name;         
	std::string m_declString;   ///< String of the original declaration

	EnumNamespaces m_namespace; ///< Optional name of the namespace (surface, option, ...)
	RtToken m_table;            ///< Optional name of the table
	RtToken m_var;              ///< Stripped name of the variable
	RtToken m_token;            ///< Token for the declaration ([namespace:][table:]var)
	EnumClasses m_class;        ///< Storage class of the declaration
	EnumTypes m_type;           ///< Type of the elements
	EnumBasicTypes m_basicType; ///< Basic type of the elements (according to type)
	unsigned long m_arraySize;  ///< Number of elements in an array, 1 if no array declaration
	CColorDescr m_colorDescr;   ///< Color descrvalid while variable is definded
	
	/** @brief Number of basic types per element type.
	 *
	 * For example 3 for RtPoint, RtColor needs special handling, RtString and RtToken
	 * have the size of a const char * pointer.
	 */
	unsigned long m_typeSize;
	
	bool m_isInline;            ///< True, if it is an inline declaration - can be destroyed with the parameter
	bool m_isDefault;           ///< True, if a default declaration of the renderer (like @c RI_P)


	/** @brief Parses m_name into m_namespace, m_table and m_var
	 *  
	 * @param tokenmap Map of all tokens of a context
	 * @return false, if parsing cannot be done (namespace but no table name, another ':' behind the table name)
	 * @see var()
	 */
	bool stripName(CTokenMap &tokenmap);

	/** @brief Parses a declaration.
	 *
	 * Inline declarations have name == 0. The declaration has the form (@c declare):
	 *
	 * [class] type ['['n']']
	 *
	 * or as inline declaration (parameter):
	 *
	 * [class] type ['['n']'] name
	 *
	 * @param name  Name of the declaration (case sensitive), 0 for inline declarations.
	 * @param decl  The declaration string.
	 * @param tokenmap Map of all tokens of a context
	 * @return true, if parsing was ok and false if an error was found while parsing.
	 */
	bool parse(const char *name, const char *decl, CTokenMap &tokenmap);

	/** @brief Assigns a declaration instance, declaration should not be changed.
	 *  @param decl declaration that is copied
	 *  @return *this
	 */
	CDeclaration &operator=(const CDeclaration &decl);

public:
	/** @brief Standard constructor for inline declarations
	 * @param parameterDeclstr Inline declaration of a parameter. Only a declaration name
	 * is considered as an error because the existence of a declaration for
	 * \a parameterDeclstr has been tested before. Can throw a RIE_SYNTAX parsing error.
	 * @param curColorDescr The current size of color (number of floats) and it's RGB transformation
	 * @exception ExceptRiCPPError
	 */
	CDeclaration(const char *parameterDeclstr, const CColorDescr &curColorDescr, CTokenMap &tokenmap);
	/** @brief Standard constructor for declarations CBaseRenderer::Declare().
	 * Can throw a RIE_SYNTAX parsing error.
	 * @param token The token (unique string, see CTokenMap) for the declaration name, must not represent an empty string.
	 * @param declstr The declaration.
	 * @param curColorDescr The current size of color (number of floats) and it's RGB transformation
	 * @param isDefault A default declaration of the interface?
	 * @exception ExceptRiCPPError
	 */
	CDeclaration(RtToken token, const char *declstr, const CColorDescr &curColorDescr, CTokenMap &tokenmap, bool isDefault);
	/** @brief Copy constructor for declaration with different color size
	 * @param decl The CDeclaration instance to copy
	 * @param newColorDescr The new current size of color (number of floats) and RGB transformation.
	 */
	CDeclaration(const CDeclaration &decl, const CColorDescr &newColorDescr);

	/** @brief Copy constructor
	 * @param decl The CDeclaration instance to copy.
	 */
	CDeclaration(const CDeclaration &decl);

	/** @brief Gets the name of the declaration
	 *  @return Name of the declaration
	 */
	inline const char *name() const { return m_name.c_str() ? m_name.c_str() : ""; }

	/** @brief Gets the namespace of the variable
	 *
	 *  [QRM] Can be the name of a request with declareable parameters surface, option, attribute
	 * 
	 *  @return Namespace identifier
	 */
	inline EnumNamespaces tableNamespace() const { return m_namespace; }

	/** @brief Additional tablename if variable name is still not unique
	 *
	 *  [QRM] E.G. for an option "renderer" "searchpath": "option" is the
	 *  namespace, renderer is the "table" and "searchpath" the variable name
	 * 
	 *  @return Table identifier
	 */
	inline RtToken table() const { return m_table; }

	/** @brief Gets the stripped variable name of the declaration
	 *
	 *  (?) If only the stripped variable name is the variable name,
	 *  that can be found in the parameter list,
	 *  the searching for a variable in the dictionary must be done in 3 steps having the
	 *  following order:
	 *  <ol>
	 *  <li> namespace:table:var </li>
	 *  <li> table:var </li>
	 *  <li> var </li>
	 *  </ol>
	 *
	 *  If the qualified name is used where needed also in parameter lists, simple search 
	 *  is enought. The stripped variable name is used in shaders.
	 *
	 *  @return Stripped variable name
	 *  @see CDeclarationDictionary
	 */
	inline RtToken var() const { return m_var; }


	/** @brief Gets the token for the declaration name
	 *  @return Token of the declaration name, id=0 if it is an inline declaration
	 */
	inline RtToken token() const { return m_token; }

	/** @brief Tests if inline declaration
	 *  @return True, if instance represents an inline declaration
	 */
	inline bool isInline() const { return m_isInline; }

	/** @brief Tests if default declaration
	 *
	 * Declarations for basic interface types like RI_P point are marked as default
	 *
	 * @return True, if instance represents a default declaration
	 */
	inline bool isDefault() const { return m_isDefault; }

	/** @brief Gets the storage class of the representation
	 *  @return Storage class
	 */
	inline EnumClasses storageClass() const { return m_class; }

	/** @brief Gets the type of the representation
	 *  @return Type, TYPE_UNKNOWN is an error
	 */
	inline EnumTypes type() const { return m_type; }

	/** @brief Gets the basic type for the type
	 *
	 *  E.g. float is the basic type of point
	 *
	 *  @return Basic type
	 */
	inline EnumBasicTypes basicType() const { return m_basicType; }

	/** @brief Gets the size of the basic type in bytes.
	 * @return The size of the basic type in bytes.
	 */
	inline unsigned int basicTypeByteSize() const
	{
		return CTypeInfo::basicTypeByteSize(m_basicType);
	}

	/** @brief Gets the array size of an declaration
	 *
	 * 1 is returned for no array declaration 
	 *
	 *  @return Array size
	 */
	inline unsigned long arraySize() const { return m_arraySize; }

	/** @brief Gets the type size of an declaration (in basic type elements)
	 *
	 * e.g. 3 is returned for TYPE_POINT (consits of 3 BASICTYPE_FLOAT)
	 *
	 *  @return Type size
	 */
	inline unsigned long typeSize() const { return m_typeSize; }

	/** @brief Gets number of one compound element of the declaration (incl. array)
	 *  @return The type size times the array size
	 */
	inline unsigned long elemSize() const { return m_typeSize * m_arraySize; }

	/** @brief Gets number bytes of one compound element of the declaration (incl. array)
	 *  @return The type size times the array size times number of bytes for the basic type
	 */
	inline unsigned long numberOfBytes() const { return m_typeSize * m_arraySize * basicTypeByteSize(); }

	/** @brief Get the number of elements.
	 *
	 * The appropriate number depends on the storage class m_class. It is one of the given parameters.
	 * The returned number can be used to calculate the size of the value array
	 * of a token-value pair by multipling it with either elemSize() or numberOfBytes().
	 *
	 * @param vertices       Number of vertices (used by vertex class)
	 * @param corners        Number of corners (used by varying class)
	 * @param facets         Number of facets (used by uniform class)
	 * @param facevertices   Number of vertices per face (used by facevertex class)
	 * @param facecorners    Number of corners per face (used by facevarying class)
	 *
	 * @return Number of elements of a parameter depending on storage class.
	 */
	RtInt selectNumberOf(RtInt vertices, RtInt corners, RtInt facets, RtInt faceVertices, RtInt faceCorners) const;

	/** @brief Get the number of elements.
	 *
	 * The appropriate number depends on the storage class m_class. It is one of the given parameters.
	 * The returned number can be used to calculate the size of the value array
	 * of a token-value pair by multipling it with either elemSize() or numberOfBytes().
	 *
	 * @param aCount Number of vertices, corners, etc.
	 *
	 * @return Number of elements of a parameter depending on storage class.
	 */
	inline RtInt selectNumberOf(const CParameterClasses &aCount) const
	{
		return selectNumberOf(aCount.vertices(), aCount.corners(), aCount.facets(),
			                aCount.faceVertices(), aCount.faceCorners());
	}

	/** @brief The color descriptor valid for this declaration instance
	 */
	const CColorDescr &colorDescr() const { return m_colorDescr; }

	/** @brief Gets the inline declaration as string.
	 *
	 * <class> <type> ['[' size ']' name
	 *
	 *  @retval declaration Declaration as string.
	 *  @return @a declaration.c_str()
	 */
	const char *getInlineString(std::string &declaration) const;

	/** @brief Gets the string of the original declaration.
	 *
	 *  @return Original declaration.
	 */
	inline const char *declString() const { return m_declString.c_str(); }
}; // CDeclarartion

} // namespace RiCPP

#endif // _RICPP_DECLARATION_DECLARATION_H
