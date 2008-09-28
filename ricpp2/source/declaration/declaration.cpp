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

#ifndef _RICPP_RICPP_RICPPERROR_H
#include "ricpp/ricpp/ricpperror.h"
#endif // _RICPP_RICPP_RICPPERROR_H

#include <cassert>

using namespace RiCPP;

bool CDeclaration::devideName(CTokenMap &tokenmap)
{
	m_qualifier = QUALIFIER_UNKNOWN;
	m_table = RI_NULL;
	m_var = RI_NULL;

	if ( m_qualifiedName.empty() ) {
		return false;
	}

	size_t pos = 0;
	std::string table, var;
	const char *str = m_qualifiedName.c_str();

	m_qualifier = CTypeInfo::qualifierPrefix(str, pos);
	if ( m_qualifier != QUALIFIER_UNKNOWN ) {
		str += pos;
	}
	
	table.reserve(strlen(str)+1);
	for ( pos = 0; str[pos]; ++pos ) {
		if ( str[pos] == ':' ) {
			str += pos+1;
			var = str;
			if ( !table.empty() && !var.empty() && var.find(':') == std::string::npos ) {
				// not an empty table or name, no more :, qualifier:table:var or table:var
				m_table = tokenmap.findCreate(table.c_str());
				m_var = tokenmap.findCreate(var.c_str());
				return true;
			}
			// Simply take the rest as variable name
			m_var = tokenmap.findCreate(str);
			return true;
		}
		table += str[pos];
	}
	// No Table found: m_table == RI_NULL

	if ( notEmptyStr(str) ) {
		m_var = tokenmap.findCreate(str);
	} else if ( m_qualifier != QUALIFIER_UNKNOWN ) {
		// Only Qualifier - considered as name
		m_var = tokenmap.findCreate(m_qualifiedName.c_str());
		m_qualifier = QUALIFIER_UNKNOWN;
	}

	// At least a variable should be found.
	return m_var != RI_NULL;
}


bool CDeclaration::parse(const char *name, const char *decl, CTokenMap &tokenmap)
{
	// Set the name, 0 for parametrs and inline declarations (m_qualifiedName will be parsed out of decl later)
	m_qualifiedName = noNullStr(name);
	
	m_isArray = false;
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
		if ( CTypeInfo::arrayPrefix(decl, pos, m_arraySize) ) {
			m_isArray = true;
			decl += pos;
		}

		if ( m_arraySize == 0 )
			return false;

		// Eat the whitespaces
		pos = 0;
		while ( *decl && isspace(*decl) )
			++decl;

		// The name follows an inline declarations
		if ( m_qualifiedName.empty() ) {
			// inline declaration or only a typename - names can contain blanks e.g. "an option"
			while ( *decl ) {
				m_qualifiedName += *decl;
				++decl;
			}

			// CBaseRenderer::declare() also strips trailing and leading blanks of names
			trimRight(m_qualifiedName);
			
			// No name in an inline declaration is considered as error
			if ( m_qualifiedName.empty() )
				return false;
			m_token = tokenmap.findCreate(m_qualifiedName.c_str());
		}

		// Now only white spaces may follow
		while ( *decl ) {
			if ( !isspace(*decl) )
				return false;
			++decl;
		}

	}

	return devideName(tokenmap);
}

CDeclaration::CDeclaration(const char *parameterDeclstr, const CColorDescr &curColorDescr, CTokenMap &tokenmap)
{
	assert(parameterDeclstr != 0);

	m_qualifier = QUALIFIER_UNKNOWN;
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
	m_qualifier = QUALIFIER_UNKNOWN;
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

CDeclaration::CDeclaration(const CDeclaration &decl, CTokenMap &aMap)
{
	assignRemap(decl, aMap);
}


CDeclaration &CDeclaration::operator=(const CDeclaration &decl)
{
	if ( this == &decl )
		return *this;
	m_qualifiedName = decl.m_qualifiedName;
	m_qualifier = decl.m_qualifier;
	m_table = decl.m_table;
	m_var = decl.m_var;
	m_token = decl.m_token;
	m_class = decl.m_class;
	m_type = decl.m_type;
	m_basicType = decl.m_basicType;
	m_isArray = decl.m_isArray;
	m_arraySize = decl.m_arraySize;
	m_typeSize = decl.m_typeSize;
	m_isInline = decl.m_isInline;
	m_isDefault = decl.m_isDefault;
	m_colorDescr = decl.m_colorDescr;
	return *this;
}

CDeclaration &CDeclaration::assignRemap(const CDeclaration &decl, CTokenMap &aMap)
{
	if ( this == &decl )
		return *this;
	
	m_qualifiedName = decl.m_qualifiedName;
	m_qualifier = decl.m_qualifier;
	
	m_table = aMap.findCreate(decl.m_table);
	m_var = aMap.findCreate(decl.m_var);
	m_token = aMap.findCreate(decl.m_token);
	
	m_class = decl.m_class;
	m_type = decl.m_type;
	m_basicType = decl.m_basicType;
	
	m_isArray = decl.m_isArray;
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


const char *CDeclaration::getInlineString(std::string &declaration) const
{
	declaration = "";
	declaration += CTypeInfo::className(m_class);
	declaration += " ";
	declaration += CTypeInfo::typeName(m_type);
	if ( m_isArray || m_arraySize > 1 ) {
		declaration += " [";
		char num[64];
		declaration += valToStr(num, sizeof(num), m_arraySize);
		declaration += "]";
	}
	declaration += " ";
	declaration += m_qualifiedName;
	return declaration.c_str();
}

/*
const char *CDeclaration::getDeclString(std::string &declaration) const
{
	if ( isInline() ) {
		return getInlineString(declaration);
	}
	declaration = m_qualifiedName;
	return declaration.c_str();
}
*/

bool CDeclaration::matches(EnumQualifiers aQualifier, RtToken aTable, RtToken aVar) const
{
	if ( aQualifier != QUALIFIER_UNKNOWN && qualifier() != QUALIFIER_UNKNOWN && aQualifier != qualifier() )
		return false;
	if ( aTable != RI_NULL && table() != RI_NULL && aTable != table() )
		return false;
	return aVar == var();
}

bool CDeclaration::matches(RtToken aQualifierName, RtToken aTable, RtToken aVar) const
{
	EnumQualifiers aQualifier = CTypeInfo::qualifier(aQualifierName);
	if ( aQualifierName != RI_NULL && aQualifier == QUALIFIER_UNKNOWN )
		return false;
	return matches(aQualifier, aTable, aVar);
}
