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

/** @file declaration.cpp
 *  @author Andreas Pidde (andreas@pidde.de)
 *  @brief Implements a declaration
 */
#include "ricpp/declaration/declaration.h"
#include "ricpp/ricpp/renderererror.h"
#include "ricpp/tools/inlinetools.h"

#include <assert.h>

using namespace RiCPP;

bool CDeclaration::parse(const char *name, const char *decl, unsigned int curColorSize)
{
	// Set the name, 0 for inline declarations
	m_name = name ? name : "";

	m_arraySize = 0;
	m_typeSize = 0;
	m_class = CLASS_UNKNOWN;
	m_type = TYPE_UNKNOWN;
	m_basicType = BASICTYPE_UNKNOWN;

	if ( decl ) {
		size_t pos = 0;

		// Eat the whitespaces
		while ( *decl && isspace(*decl) )
			++decl;

		// The class is the first part of a declaration, defaults to "uniform"
		m_class = CTypeInfo::classPrefix(decl, pos);
		if ( m_class )
			decl += pos;
		else
			m_class = CLASS_UNIFORM;

		// Eat the whitespaces
		pos = 0;
		while ( *decl && isspace(*decl) )
			++decl;

		// The type is the second part of a declaration
		// Types are not optional, however types for
		// parameters can be the typename only, so error handling is
		// defered to the caller
		m_type = CTypeInfo::typePrefix(decl, pos);
		if ( m_type ) {
			m_basicType = CTypeInfo::basicTypeForType(m_type);
			decl += pos;
			m_typeSize = CTypeInfo::typeSize(m_type);
			// Color needs special handling
			if ( m_type == TYPE_COLOR ) {
				m_typeSize = curColorSize;
			}
		}

		// Eat the whitespaces
		pos = 0;
		while ( *decl && isspace(*decl) )
			++decl;

		// The optional array specifier [n], defaults to 1
		m_arraySize = CTypeInfo::arrayPrefix(decl, pos);
		if ( m_arraySize >= 1 )
			decl += pos;
		else
			m_arraySize = 1;

		// Eat the whitespaces
		pos = 0;
		while ( *decl && isspace(*decl) )
			++decl;

		// The name follows in inline declarations
		if ( !name || !*name ) {
			// inline declaration or only a typename
			while ( *decl && !isspace(*decl) ) {
				m_name += *decl;
				++decl;
			}
			// No name in an inline declaration is considered as error
			if ( m_name.empty() )
				return false;
		}

		// Now only white spaces may follow
		while ( *decl ) {
			if ( !isspace(*decl) )
				return false;
			++decl;
		}

	}

	return true;
}

CDeclaration::CDeclaration(const char *parameterDeclstr, unsigned int curColorSize)
{
	assert(parameterDeclstr != 0);

	m_isDefault = false; // inline declarations are never default declarations
	m_isInline = true;   // mark inline

	if ( !parse(0, parameterDeclstr, curColorSize) ) {
		throw ERendererError(RIE_SYNTAX, RIE_ERROR, (int)0, (const char *)0, parameterDeclstr);
	}

	// Already searched for declaration or syntax error
	if ( m_type == TYPE_UNKNOWN ) {
		throw ERendererError(RIE_SYNTAX, RIE_ERROR, (int)0, (const char *)0, "Type or declaration not found: \"%s\"", markemptystr(parameterDeclstr));
	}
	// Declaration name not found in inline declaration
	if ( m_name.empty() ) {
		throw ERendererError(RIE_SYNTAX, RIE_ERROR, (int)0, (const char *)0, "Name not found: \"%s\"", markemptystr(parameterDeclstr));
	}
}

CDeclaration::CDeclaration(CToken &token, const char *declstr, unsigned int curColorSize, bool isDefault)
{
	m_isDefault = isDefault;
	m_isInline = false;
	m_token = token;
	if ( !parse(token.name(), declstr, curColorSize) || (m_type == TYPE_UNKNOWN) ) {
		throw ERendererError(RIE_SYNTAX, RIE_ERROR, 0, NULL, "%s: %s", markemptystr(token.name()), markemptystr(declstr));
	}
}

CDeclaration::CDeclaration(const CDeclaration &decl, unsigned int newColorSize)
{
	*this = decl;
	if ( decl.m_type == TYPE_COLOR )
		m_typeSize = newColorSize;
}

CDeclaration::CDeclaration(const CDeclaration &decl)
{
	*this = decl;
}

CDeclaration &CDeclaration::operator=(const CDeclaration &decl)
{
	if ( this == &decl )
		return *this;
	m_name = decl.m_name;
	m_token = decl.m_token;
	m_class = decl.m_class;
	m_type = decl.m_type;
	m_basicType = decl.m_basicType;
	m_arraySize = decl.m_arraySize;
	m_typeSize = decl.m_typeSize;
	m_isInline = decl.m_isInline;
	m_isDefault = decl.m_isDefault;
	return *this;
}
