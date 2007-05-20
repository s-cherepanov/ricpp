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

/** @file types.cpp
 *  @author Andreas Pidde (andreas@pidde.de)
 *  @brief Implements helpers for ri types
 */
#include "ricpp/declaration/types.h"
#include "ricpp/ricpp/ricpp.h"

#include <string.h>
#include <ctype.h>

using namespace RiCPP;

const unsigned int CTypeInfo::m_basicTypeSizes[N_BASICTYPES] = {
	0,
	sizeof(RtInt),
	sizeof(RtFloat),
	sizeof(RtString)
};

const char *CTypeInfo::m_basicTypeNames[N_BASICTYPES] = {
	"",
	"float",
	"integer",
	"string"
};

const char *CTypeInfo::m_typeNames[N_TYPES+1] = {
	"",
	"float",
	"integer",
	"string",
	"point",
	"vector",
	"normal",
	"hpoint",
	"matrix",
	"color"
	"int"
};

const unsigned int CTypeInfo::m_typeSizes[N_TYPES+1] =
{
	0,  // Unknown
	1,  // (RtFloat)
	1,  // (RtInt)
	1,  // (RtString)
	3,  // (RtPoint)
	3,  // (RtVector)
	3,  // (RtNormal)
	4,  // (RtHpoint)
	16, // (RtMatrix)
	3,  // (RtColor)
	1  // (RtInt)
};

const unsigned int CTypeInfo::m_typeByteSizes[N_TYPES+1] =
{
	0,                  // Unknown
	sizeof(RtFloat),    // (RtFloat)
	sizeof(RtInt),      // (RtInt)
	sizeof(RtString),   // (RtString)
	3*sizeof(RtFloat),  // (RtPoint)
	3*sizeof(RtFloat),  // (RtVector)
	3*sizeof(RtFloat),  // (RtNormal)
	4*sizeof(RtFloat),  // (RtHpoint)
	16*sizeof(RtFloat), // (RtMatrix)
	3*sizeof(RtFloat),  // (RtColor)
	sizeof(RtInt)       // (RtInt)
};

const EnumBasicTypes CTypeInfo::m_basicTypesForTypes[N_TYPES+1] =
{
	BASICTYPE_UNKNOWN,  // Unknown
	BASICTYPE_FLOAT,    // (RtFloat)
	BASICTYPE_INTEGER,  // (RtInt)
	BASICTYPE_STRING,   // (RtString)
	BASICTYPE_FLOAT,    // (RtPoint)
	BASICTYPE_FLOAT,    // (RtVector)
	BASICTYPE_FLOAT,    // (RtNormal)
	BASICTYPE_FLOAT,    // (RtHpoint)
	BASICTYPE_FLOAT,    // (RtMatrix)
	BASICTYPE_FLOAT,    // (RtColor)
	BASICTYPE_INTEGER   // (RtInt)
};

const char *CTypeInfo::m_classNames[N_CLASSES] = {
	"",
	"constant",
	"uniform",
	"varying",
	"vertex",
	"facevarying",
	"facevertex"
};

int CTypeInfo::tokcmp(const char *token, const char *search) {
	if ( !token ) return search && *search ? -1 : 0;
	if ( !search ) return token && *token ? 1 : 0;

	int comp;
	const char *cp1 = token;
	const char *cp2 = search;

	while ( *cp2 && !isspace(*cp2) && *cp2 != '[' ) {
		if ( !*cp1 )
			return -1;
		comp = *cp1 - *cp2;
		if ( comp )
			return comp;
		++cp1;
		++cp2;
	}
	if ( !*cp1 )
		return 0;
	return 1;
}


EnumClasses CTypeInfo::classPrefix(const char *aclass, size_t &pos) {
	int i;
	if ( !aclass ) return CLASS_UNKNOWN;
	for ( i = 1; i < N_CLASSES; ++i ) {
		if ( !tokcmp(m_classNames[i], aclass) ) {
			pos = strlen(m_classNames[i]);
			return (EnumClasses)i;
		}
	}
	return CLASS_UNKNOWN;
}


EnumTypes CTypeInfo::typePrefix(const char *type, size_t &pos) {
	int i;
	if ( !type ) return TYPE_UNKNOWN;
	for ( i = 1; i < N_TYPES+1; ++i ) {
		if ( !tokcmp(m_typeNames[i], type) ) {
			pos = strlen(m_typeNames[i]);
			if ( i == N_TYPES ) {
				// let "int" be "integer"
				i = TYPE_INTEGER;
			}
			return (EnumTypes)i;
		}
	}
	return TYPE_UNKNOWN;
}

int CTypeInfo::arrayPrefix(const char *acard, size_t &pos) {
	int state = 0;
	int retval = 0;
	if ( !acard ) return 0;

	for ( pos = 0; acard[pos]; ++pos ) {
		if ( isspace(acard[pos]) && state != 1 )
			continue;
		switch ( state ) {
		case 0:
			if ( acard[pos] == '[' ) {
				state = 1;
				break;
			}
			return -1;
		case 1:
			if ( isdigit(acard[pos]) )
				retval = retval*10+(int)(acard[pos]-'0');
			else {
				--pos;
				state = 2;
			}
			break;
		case 2:
			if ( acard[pos] == ']' ) {
				state = 3;
				break;
			}
			return -1;
		}
		if ( state == 3 ) {
			if ( retval == 0 ) {
				// [0] is treated as error...
				retval = -1;
			}
			++pos;
			break;
		}
	}

	return (state != 3 && state > 0) ? -1 : retval;
}


const char *CTypeInfo::basicTypeName(EnumBasicTypes e)
{
	return m_basicTypeNames[e];
}

unsigned int CTypeInfo::basicTypeByteSize(EnumBasicTypes e)
{
	return m_basicTypeSizes[e];
}


const char *CTypeInfo::typeName(EnumTypes e)
{
	return m_typeNames[e];
}

unsigned int CTypeInfo::typeSize(EnumTypes e)
{
	return m_typeSizes[e];
}

const char *CTypeInfo::className(EnumClasses e)
{
	return m_classNames[e];
}

unsigned int CTypeInfo::typeByteSize(EnumTypes e)
{
	return m_typeByteSizes[e];
}

EnumBasicTypes CTypeInfo::basicTypeForType(EnumTypes e)
{
	return m_basicTypesForTypes[e];
}
