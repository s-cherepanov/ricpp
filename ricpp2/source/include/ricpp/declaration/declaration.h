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
 *  @brief A declaration
 */
#ifndef _RICPP_DECLARATION_TOKEN_H
#include "ricpp/declaration/token.h"
#endif // _RICPP_DECLARATION_TOKEN_H

#ifndef _RICPP_DECLARATION_TYPES_H
#include "ricpp/declaration/types.h"
#endif // _RICPP_DECLARATION_TYPES_H

#include <string>

namespace RiCPP {
/** @brief Representation of a (possibly inline) declaration
 *
 * If the declaration has a color type, the number of elements m_typesize is the
 * size, when the declaration is used. If the size changes, a new declaration is
 * created, if a parameter of the declaration is used. Declarations that are not
 * inline are not destroyed before the rendering context ends.
 */
class CDeclaration {
	std::string m_name;         ///< Name of the declaration
	CToken m_token;             ///< Token for the declaration, if not inline
	EnumClasses m_class;        ///< Storage class of the declaration
	EnumTypes m_type;           ///< Type of the elements
	EnumBasicTypes m_basicType; ///< Basic type of the elements (according to type)
	unsigned long m_arraySize;  ///< Number of elements in an array, 1 if no array declaration
	unsigned long m_typeSize;   ///< Number of basic types per element type (eg. 3 for RtPoint, RtColor needs special handling, RtString and RtToken have the size of a const char * pointer)
	bool m_isInline;            ///< True, if it is an inline declaration - can be destroyed with the parameter
	bool m_isDefault;           ///< True, if a default declaration of the renderer (like RI_P)

	/** @brief Parses a declaration.
	 *
	 * Inline declarations have name == 0.
	 *
	 * @param name  Name of the declaration (case sensitive), 0 for inline declarations.
	 * @param decl  The declaration string.
	 * @param curColorSize The current size of color (number of floats).
	 * @return true, if parsing was ok and false if an error was found while parsing.
	 */
	bool parse(const char *name, const char *decl, unsigned int curColorSize);

public:
	/** @brief Standard constructor for inline declarations, throws ERendererError
	 * @param parameterDeclstr inline declaration of a parameter. Only a declaration name
	 * is considered as an error because the existence of a declaration for
	 * parameterDeclstr has been tested before.
	 * @param curColorSize The current size of color (number of floats).
	 */
	CDeclaration(const char *parameterDeclstr, unsigned int curColorSize);
	/** @brief Standard constructor for declarations TRi::Declare(), throws ERendererError.
	 * @param token The token for the declaration name.
	 * @param declstr The declaration.
	 * @param curColorSize The current size of color (number of floats).
	 * @param isDefault A default declaration of the interface
	 */
	CDeclaration(CToken &token, const char *declstr, unsigned int curColorSize, bool isDefault);
	/** @brief Copy constructor for declaration with different color size
	 * @param decl The CDeclaration instance to copy
	 * @param newColorSize The new current size of color (number of floats).
	 */
	CDeclaration(const CDeclaration &decl, unsigned int newColorSize);

	/** @brief Copy constructor
	 * @param decl The CDeclaration instance to copy.
	 */
	CDeclaration(const CDeclaration &decl);

	/** @brief Gets the name of the declaration
	 *  @return Name of the declaration
	 */
	inline const char *name() const { return m_name.c_str(); }

	/** @brief Gets the token for the declaration name
	 *  @return Token of the declaration name, id=0 if it is an inline declaration
	 */
	inline CToken token() const { return m_token; }

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

	/** @brief Gets number of all elements of the declaration (incl. array)
	 *  @return The type size times the array size
	 */
	inline unsigned long elemSize() const { return m_typeSize * m_arraySize; }

	/** @brief Gets number bytes of all elements of the declaration (incl. array)
	 *  @return The type size times the array size times number of bytes for the basic type
	 */
	inline unsigned long numberOfBytes() const { return m_typeSize * m_arraySize * CTypeInfo::basicTypeByteSize(m_basicType); }

	/** @brief Assigns a declaration instance
	 *  @param decl declaration that is copied
	 *  @return *this
	 */
	CDeclaration &operator=(const CDeclaration &decl);
}; // CDeclarartion

} // namespace RiCPP

#endif // _RICPP_DECLARATION_DECLARATION_H
