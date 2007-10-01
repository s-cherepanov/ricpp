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
#include "ricpp/ricpp/ricpperror.h"
#include "ricpp/tools/inlinetools.h"

#include <cassert>

using namespace RiCPP;

bool CDeclaration::stripName(CTokenMap &tokenmap)
{
	m_namespace = NAMESPACE_UNKNOWN;
	m_table = RI_NULL;
	m_var = RI_NULL;

	if ( m_name.empty() ) {
		return false;
	}

	size_t pos = 0;
	std::string table, var;
	const char *str = m_name.c_str();

	m_namespace = CTypeInfo::namespacePrefix(str, pos);
	if ( m_namespace != NAMESPACE_UNKNOWN ) {
		str += pos;
	}
	
	table.reserve(strlen(str)+1);
	for ( pos = 0; str[pos]; ++pos ) {
		if ( str[pos] == ':' ) {
			str += pos+1;
			var = str;
			if ( !table.empty() && !var.empty() && var.find(':') == std::string::npos ) {
				// not an empty table or name, no more :, namespace:table:var or table:var
				m_table = tokenmap.findCreate(table.c_str());
				m_var = tokenmap.findCreate(var.c_str());
				return true;
			}
			return false;
		}
		table += str[pos];
	}
	// No Table found: m_table == RI_NULL

	if ( notEmptyStr(str) )
		m_var = tokenmap.findCreate(str);

	// no table, there should also be no namespace, only a variable name (?)
	//! @todo Does namespace:variable or namespace:tablename:variable or tablename:variable work? Makes sense.
	// return m_namespace == NAMESPACE_UNKNOWN;

	// At least a variable should be found.
	return m_var != RI_NULL;
}


bool CDeclaration::parse(const char *name, const char *decl, CTokenMap &tokenmap)
{
	// Set the name, 0 for inline declarations
	m_name = noNullStr(name);
	
	m_arraySize = 1;
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
		// Types are not optional
		m_type = CTypeInfo::typePrefix(decl, pos);
		if ( m_type ) {
			m_basicType = CTypeInfo::basicTypeForType(m_type);
			decl += pos;
			m_typeSize = CTypeInfo::typeSize(m_type);
			// Color needs special handling
			if ( m_type == TYPE_COLOR ) {
				m_typeSize = m_colorDescr.colorSamples();
			}
		} else {
			return false;
		}

		// Eat the whitespaces
		pos = 0;
		while ( *decl && isspace(*decl) )
			++decl;

		// The optional array specifier [n], defaults to 1
		if ( CTypeInfo::arrayPrefix(decl, pos, m_arraySize) )
			decl += pos;

		if ( m_arraySize == 0 )
			return false;

		// Eat the whitespaces
		pos = 0;
		while ( *decl && isspace(*decl) )
			++decl;

		// The name follows in inline declarations
		if ( m_name.empty() ) {
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

	return stripName(tokenmap);
}

CDeclaration::CDeclaration(const char *parameterDeclstr, const CColorDescr &curColorDescr, CTokenMap &tokenmap)
{
	assert(parameterDeclstr != 0);

	m_namespace = NAMESPACE_UNKNOWN;
	m_table = RI_NULL;
	m_var = RI_NULL;
	m_colorDescr = curColorDescr;

	m_isDefault = false; // inline declarations are never default declarations
	m_isInline = true;   // mark inline
	m_token = RI_NULL;

	if ( !parse(0, parameterDeclstr, tokenmap) ) {
		throw ExceptRiCPPError(RIE_SYNTAX, RIE_ERROR, (int)0, (const char *)0, parameterDeclstr);
	}
}

CDeclaration::CDeclaration(RtToken token, const char *declstr, const CColorDescr &curColorDescr, CTokenMap &tokenmap, bool isDefault)
{
	m_namespace = NAMESPACE_UNKNOWN;
	m_table = RI_NULL;
	m_var = RI_NULL;

	m_colorDescr = curColorDescr;

	m_isDefault = isDefault;
	m_isInline = false;
	m_token = tokenmap.findCreate(token);

	if ( m_token == RI_NULL ) {
		throw ExceptRiCPPError(RIE_SYNTAX, RIE_ERROR, 0, NULL, "Declaration name is empty for \"%s\"", markEmptyStr(declstr));
	}
	if ( !parse(m_token, declstr, tokenmap) ) {
		throw ExceptRiCPPError(RIE_SYNTAX, RIE_ERROR, 0, NULL, "\"%s\": \"%s\"", m_token, markEmptyStr(declstr));
	}
}

CDeclaration::CDeclaration(const CDeclaration &decl, const CColorDescr &newColorDescr)
{
	*this = decl;
	m_colorDescr = newColorDescr;
	if ( decl.m_type == TYPE_COLOR )
		m_typeSize = newColorDescr.colorSamples();
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
	m_namespace = decl.m_namespace;
	m_table = decl.m_table;
	m_var = decl.m_var;
	m_token = decl.m_token;
	m_class = decl.m_class;
	m_type = decl.m_type;
	m_basicType = decl.m_basicType;
	m_arraySize = decl.m_arraySize;
	m_typeSize = decl.m_typeSize;
	m_isInline = decl.m_isInline;
	m_isDefault = decl.m_isDefault;
	m_colorDescr = decl.m_colorDescr;
	return *this;
}

int CDeclaration::selectNumberOf(int vertices, int corners, int facets, int faceVertices, int faceCorners) const
{
	int n = 1;
	switch ( m_class ) {
		case CLASS_CONSTANT:
			break;
		case CLASS_UNIFORM:
			n = facets;
			break;
		case CLASS_VARYING:
			n = corners;
			break;
		case CLASS_VERTEX:
			n = vertices;
			break;
		case CLASS_FACEVARYING:
			n = faceCorners;
			break;
		case CLASS_FACEVERTEX:
			n = faceVertices;
			break;
		default: // CLASS_UNKNOWN
			break;
	}

	return n;
}
