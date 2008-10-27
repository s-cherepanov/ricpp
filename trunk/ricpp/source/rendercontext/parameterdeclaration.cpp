// RICPP - RenderMan Interface CPP Language Binding
//         RenderMan is (R) by Pixar
//         Windows is (R) by Microsoft
//
// Copyright © 2001 - 2002, Andreas Pidde
//
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

/*! \file parameterdeclaration.cpp
 *  \brief Implementation of a class that parses, stores and can write to a std::ostream
 *         the declaration of a parameter type \sa TRi::declare()
 *  \author Andreas Pidde (andreas@pidde.de)
 */

#include "stdafx.h"
#include "parameterdeclaration.h"

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//// class TParameterDeclaration


////////////////////////////////////////////////////////////////////////////////
// static members

////////////////////////////////////////////////////////////////////////////////
// m_typeSizes: table of type (TYPE_...) -> size in bytes
//
int TParameterDeclaration::m_typeSizes[N_TYPESIZES] = {
	0,
	sizeof(RtBoolean),
	sizeof(RtInt),
	sizeof(RtFloat),
	sizeof(RtToken),	// Size f a color Pointer
	sizeof(RtFloat),	// Size of one color component !!! must be multiplied with current color component
	sizeof(RtPoint),
	sizeof(RtVector),
	sizeof(RtNormal),
	sizeof(RtHpoint),
	sizeof(RtMpoint),
	sizeof(RtMatrix),
	sizeof(RtBasis),
	sizeof(RtBound),
	sizeof(RtString),
	sizeof(RtPointer),
	0, // RtVoid
	sizeof(RtFilterFunc),
	sizeof(RtErrorHandler),
	sizeof(RtProcSubdivFunc),
	sizeof(RtProcFreeFunc),
	sizeof(RtArchiveCallback),
	sizeof(RtObjectHandle),
	sizeof(RtLightHandle),
	sizeof(RtContextHandle),
	sizeof(RtInt)
};


////////////////////////////////////////////////////////////////////////////////
// m_typeComponents: table of type (TYPE_...) -> number of components
//
int TParameterDeclaration::m_typeComponents[N_TYPESIZES] = {
	0,
	1,  // sizeof(RtBoolean),
	1,  // sizeof(RtInt),
	1,  // sizeof(RtFloat),
	1,  // sizeof(RtToken),
	0,  // sizeof(RtFloat) Number of color components handled in TParameterDeclaration instance, is not fix
	3,  // sizeof(RtPoint),
	3,  // sizeof(RtVector),
	3,  // sizeof(RtNormal),
	4,  // sizeof(RtHpoint),
	16, // sizeof(RtMpoint),
	16, // sizeof(RtMatrix),
	16, // sizeof(RtBasis),
	6,  // sizeof(RtBound),
	1,  // sizeof(RtString),
	1,  // sizeof(RtPointer),
	0,  // RtVoid
	1,  // sizeof(RtFilterFunc),
	1,  // sizeof(RtErrorHandler),
	1,  // sizeof(RtProcSubdivFunc),
	1,  // sizeof(RtProcFreeFunc),
	1,  // sizeof(RtArchiveCallback),
	1,  // sizeof(RtObjectHandle),
	1,  // sizeof(RtLightHandle),
	1,  // sizeof(RtContextHandle)
	1   // sizeof(RtInt)
};

////////////////////////////////////////////////////////////////////////////////
// m_typeNames: table of type (TYPE_...) -> readable name of the type
//
const char *TParameterDeclaration::m_typeNames[N_TYPESIZES] = {
	"",
	"boolean",
	"integer",
	"float",
	"token",
	"color",
	"point",
	"vector",
	"normal",
	"hpoint",
	"mpoint",
	"matrix",
	"basis",
	"bound",
	"string",
	"pointer",
	"void",
	"filterfunc",
	"errorhandler",
	"procsubdivfunc",
	"procfreefunc",
	"procarchivecallback",
	"procobjecthandle",
	"lighthandle",
	"contexthandle",
	"int"
};


////////////////////////////////////////////////////////////////////////////////
// m_classNames: table of parameter class (CLASS_...)
//               -> readable name of the class
//
const char *TParameterDeclaration::m_classNames[N_CLASSNAMES] = {
	"",
	"constant",
	"uniform",
	"varying",
	"vertex",
	"facevarying",
	"facevertex"
};

////////////////////////////////////////////////////////////////////////////////
// constructors, destructor and member functions

////////////////////////////////////////////////////////////////////////////////
// tokcmp()- compares token as prefix of search
//
int TParameterDeclaration::tokcmp(const char *token, const char *search) {
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


////////////////////////////////////////////////////////////////////////////////
// Default constructor - clears all members
//
TParameterDeclaration::TParameterDeclaration() {
	m_name = "";
	m_fullname = "";
	m_class = 0;
	m_type = 0;
	m_cardinality = 0;
	m_isDefault = false;
	m_isInline  = false;
}


////////////////////////////////////////////////////////////////////////////////
// Copy constructor
//
TParameterDeclaration::TParameterDeclaration(const TParameterDeclaration &decl) {
	m_name = "";
	m_fullname = "";
	m_class = 0;
	m_type = 0;
	m_cardinality = 0;
	m_isDefault = false;
	m_isInline  = false;
	*this = decl;
}

////////////////////////////////////////////////////////////////////////////////
// Constructor - Set members with values of the parameters
//
TParameterDeclaration::TParameterDeclaration(RtToken name, RtInt aclass,
											 RtInt type, RtInt cardinality,
											 bool isDef)
{
	m_name = "";
	m_fullname = "";
	m_class = 0;
	m_type = 0;
	m_cardinality = 0;
	m_isDefault = false;
	m_isInline  = false;
	set(name, aclass, type, cardinality, isDef);
}


////////////////////////////////////////////////////////////////////////////////
// Constructor - Set members parsed values of the parameters
//
TParameterDeclaration::TParameterDeclaration(const char *name, const char *type,
											 bool isDef)
{
	m_name = "";
	m_fullname = "";
	m_class = 0;
	m_type = 0;
	m_cardinality = 0;
	m_isDefault = false;
	m_isInline  = false;
	parse(name, type, isDef);
}


////////////////////////////////////////////////////////////////////////////////
// Destructor - not used at the moment
//
TParameterDeclaration::~TParameterDeclaration() {
	// Do nothing
}


////////////////////////////////////////////////////////////////////////////////
// Duplication
//
// Throws TSystemException
//
TParameterDeclaration &TParameterDeclaration::duplicate() const {
	TParameterDeclaration *p = new TParameterDeclaration(*this);
	if ( !p )
		throw TSystemException(__FILE__, __LINE__);
	return *p;
}


////////////////////////////////////////////////////////////////////////////////
//
void TParameterDeclaration::buildFullname() {
	m_fullname = nameOfClass(m_class);
	m_fullname += " ";
	m_fullname += nameOfType(m_type);
	if ( m_cardinality > 1 ) {
		char num[64];
		sprintf(num, "%ld", (long)m_cardinality);
		m_fullname += "[";
		m_fullname += num;
		m_fullname += "]";
	}
	m_fullname += " ";
	m_fullname += m_name;
}


/*
////////////////////////////////////////////////////////////////////////////////
// static changeColorSize - the user of the RenderMan Interface might alter the
//                          dimension or type of color (RiColorSamples)
// Parameters:
//    nColor: Number of components new color element
//
// Returns:
//    Number of components of the old color element
//
int TParameterDeclaration::changeColorSize(int nColor) {
//	int oldSize = m_typeSizes[TYPE_COLOR];
//	m_typeSizes[TYPE_COLOR] = nColor * sizeof(RtFloat);
//	return m_typeSizes[TYPE_COLOR]/sizeof(RtFloat);
	return sizeof(RtFloat);
}
*/

//////////////////////////////////////////////////////////////////////////////////
// static sizeOfType - returns the size of bytes for an instance of type, for
//                     color it's the size of only one component
//
// Parameters:
//    type        : TYPE_ specifies the type
//
// Returns:
//    size needed for an instance of the given type as number of bytes
//
int TParameterDeclaration::sizeOfType(RtInt type) {
	if ( type < 0 || type >= N_TYPESIZES ) return 0;
	return m_typeSizes[type];
}


//////////////////////////////////////////////////////////////////////////////////
// static nameOfClass - returns the ASCII representation of a class CLASS_
//
// Parameters:
//    aclass : CLASS_...
//
// Returns:
//    The name of aclass
//
const char *TParameterDeclaration::nameOfClass(RtInt aclass) {
	if ( aclass < 0 || aclass > N_CLASSNAMES ) return NULL;
	return m_classNames[aclass];
}


//////////////////////////////////////////////////////////////////////////////////
// static nameOfType - returns the ASCII representation of a type TYPE_
//
// Parameters:
//    type : TYPE_...
//
// Returns:
//    The name of type
//
const char *TParameterDeclaration::nameOfType(RtInt type) {
	if ( type < 0 || type > N_TYPESIZES ) return NULL;
	return m_typeNames[type];
}


////////////////////////////////////////////////////////////////////////////////
// static classNum - returns the number of a class name
//
// Parameters:
//    aclass : string to scan, prefix should be a class name
//    pos    : returns length of the found class name if result != 0
//
// Returns:
//    The number of a class (CLASS_...)
//
//    pos: see above
//
int TParameterDeclaration::classNum(const char *aclass, size_t &pos) {
	int i;
	if ( !aclass ) return 0;
	for ( i = 1; i < N_CLASSNAMES; ++i ) {
		if ( !tokcmp(m_classNames[i], aclass) ) {
			pos = strlen(m_classNames[i]);
			return i;
		}
	}
	return 0;
}


////////////////////////////////////////////////////////////////////////////////
// static typeNum - returns the number of a type given by name
//
// Parameters:
//    aclass : string to scan, prefix should be a type name
//    pos    : returns length of the found type name if result != 0
//
// Returns:
//    The number of a class (TYPE_...)
//
//    pos: see above
//
int TParameterDeclaration::typeNum(const char *type, size_t &pos) {
	int i;
	if ( !type ) return 0;
	for ( i = 1; i < N_TYPESIZES; ++i ) {
		if ( !tokcmp(m_typeNames[i], type) ) {
			pos = strlen(m_typeNames[i]);
			// let int be integer
			if ( i == TYPE_INT ) i = TYPE_INTEGER;
			return i;
		}
	}
	return 0;
}


////////////////////////////////////////////////////////////////////////////////
// static cardinality - scans the cardinality part of a declaration [n]
//
// Parameters:
//    acard : string to scan, prefix should be a cardinality: [n]
//    pos   : returns length of the found type name if result > 0
//
// Returns:
//    < 0, syntax error or [0]
//   == 0, no cardinality
//    > 0, [n] found
//
//   pos: see above
//
int TParameterDeclaration::cardinality(const char *acard, size_t &pos) {
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
			if ( retval == 0 ) retval = -1; // [0] is treated as error...
			++pos;
			break;
		}
	}

	return (state != 3 && state > 0) ? -1 : retval;
}


////////////////////////////////////////////////////////////////////////////////
// set - sets the member variables
//
// Parameters:
//   name        - the name of the declaration
//   aclass      - number of the class CLASS_...
//   type        - number of the type TYPE_...
//   cardinality - number of fields
//   isDef       - is the declarition a default declaration of the renderer
//
void TParameterDeclaration::set(RtToken name, RtInt aclass, RtInt type, RtInt cardinality, bool isDef) {
	m_name = name ? name : "";
	m_class = aclass;
	m_type = type;
	m_cardinality = cardinality;
	m_isDefault = isDef;
	m_isInline = false;
	buildFullname();
}


////////////////////////////////////////////////////////////////////////////////
// parse - parses a declaration (s.a. TIntermediateRenderer::declare() for syntax
//         of type)
//
// Parameters:
//   name  - optional name of the declaration (case sensitive)
//   type  - type description of the declaration
//   isDef - true if the declarition a default declaration of the renderer
//
// Returns:
//   false - type could not be parsed
//
bool TParameterDeclaration::parse(const char *name, const char *type, bool isDef) {

	m_isDefault = isDef;

	m_name = name ? name : "";
	m_fullname = type ? type : m_name.c_str();
	m_isInline = false;

	if ( type ) {
		size_t pos = 0;
		while ( *type && isspace(*type) )
			++type;

		m_class = classNum(type, pos);
		if ( m_class )
			type += pos;
		else
			m_class = CLASS_UNIFORM;

		pos = 0;
		while ( *type && isspace(*type) )
			++type;

		m_type = typeNum(type, pos);
		if ( m_type )
			type += pos;

		/*
		else // if there is no default type (== 0)
			return false;
		*/

		pos = 0;
		while ( *type && isspace(*type) )
			++type;

		m_cardinality = cardinality(type, pos);
		if ( m_cardinality >= 1 )
			type += pos;
		else
			m_cardinality = 1;

		pos = 0;
		while ( *type && isspace(*type) )
			++type;

		if ( !name || !*name ) {
			// RI_TOKEN name
			while ( *type && !isspace(*type) ) {
				if ( !m_isInline ) {
					m_name = "";
					m_isInline = true;
				}
				m_name += *type;
				++type;
			}
			if ( !m_name.length() )
				return false;
		}

		// now only white space should follow
		while ( *type ) {
			if ( !isspace(*type) )
				return false;
			++type;
		}

	} else {
		m_class = 0;
		m_type = 0;
		m_cardinality = 0;
	}

	buildFullname();
	return true;
}


////////////////////////////////////////////////////////////////////////////////
// getDeclaration - gets the values of the element variables
//
// Parameters:
//   name        - returns the name of the declaration
//   aclass      - number of the class CLASS_...
//   type        - number of the type TYPE_...
//   cardinality - number of fields
//
RtToken TParameterDeclaration::getDeclaration(
	RtInt &aclass, RtInt &type, RtInt &cardinality
	) const
{
	aclass	= getClass();
	type	= getType();
	cardinality = getCardinality();
	return m_isInline ? m_fullname.c_str() : m_name.c_str();
}

RtToken TParameterDeclaration::getDeclaration(
	RtInt colorComp, RtInt &aclass, RtInt &type, RtInt &cardinality, RtInt &components, RtInt &bytes
	) const
{
	aclass	= getClass();
	type	= getType();
	cardinality = getCardinality();
	components = getComponents(colorComp);
	bytes = getBytes(colorComp);
	return m_isInline ? m_fullname.c_str() : m_name.c_str();
}

////////////////////////////////////////////////////////////////////////////////
// getName - name of the declaration or type and name if declaration was inline
//
// Returns:
//   The name of the declaration as RtToken (Pointer to the name)
//
RtToken TParameterDeclaration::getName() const {
	return m_isInline ? m_fullname.c_str() : m_name.c_str();
}


////////////////////////////////////////////////////////////////////////////////
// getFullName - type and name of the declaration
//
// Returns:
//   The name of the declaration as RtToken (Pointer to the name)
//
RtToken TParameterDeclaration::getFullName() const {
	return m_fullname.c_str();
}


////////////////////////////////////////////////////////////////////////////////
// getParameterName - only name of the declaration
//
// Returns:
//   The name of the declaration as RtToken (Pointer to the name)
//
RtToken TParameterDeclaration::getParameterName() const {
	return m_name.c_str();
}


RtInt TParameterDeclaration::getClass() const {
	return m_class;
}

RtInt TParameterDeclaration::getType() const {
	return m_type;
}

RtInt TParameterDeclaration::getBasicType() const {
	switch ( m_type ) {
		case TYPE_UNKNOWN:
			return TYPE_UNKNOWN;
		case TYPE_BOOLEAN:
			return TYPE_INTEGER;
		case TYPE_INTEGER:
			return TYPE_INTEGER;
		case TYPE_FLOAT:
			return TYPE_FLOAT;
		case TYPE_TOKEN:
			return TYPE_POINTER;
		case TYPE_COLOR:
			return TYPE_FLOAT;
		case TYPE_POINT:
			return TYPE_FLOAT;
		case TYPE_VECTOR:
			return TYPE_FLOAT;
		case TYPE_NORMAL:
			return TYPE_FLOAT;
		case TYPE_HPOINT:
			return TYPE_FLOAT;
		case TYPE_MPOINT:
			return TYPE_FLOAT;
		case TYPE_MATRIX:
			return TYPE_FLOAT;
		case TYPE_BASIS:
			return TYPE_FLOAT;
		case TYPE_BOUND:
			return TYPE_FLOAT;
		case TYPE_STRING:
			return TYPE_POINTER;
		case TYPE_POINTER:
			return TYPE_POINTER;
		case TYPE_VOID:
			return TYPE_VOID;
		case TYPE_FILTERFUNC:
			return TYPE_POINTER;
		case TYPE_ERRORHANDLER:
			return TYPE_POINTER;
		case TYPE_PROCSUBDIVFUNC:
			return TYPE_POINTER;
		case TYPE_PROCFREEFUNC:
			return TYPE_POINTER;
		case TYPE_ARCHIVECALLBACK:
			return TYPE_POINTER;
		case TYPE_OBJECTHANDLE:
			return TYPE_POINTER;
		case TYPE_LIGHTHANDLE:
			return TYPE_POINTER;
		case TYPE_CONTEXTHANDLE:
			return TYPE_POINTER;
		case TYPE_INT:
			return TYPE_INTEGER;
		default:
			return TYPE_UNKNOWN;
	}
}


RtInt TParameterDeclaration::getCardinality() const {
	return m_cardinality;
}

RtInt TParameterDeclaration::getComponents(RtInt colorComps) const {
	RtInt typeNum = getType();
	if ( typeNum == TYPE_COLOR )
		return colorComps;
	return m_typeComponents[typeNum];
}

RtInt TParameterDeclaration::getBytes(RtInt colorComps) const {
	RtInt typeNum = getType();
	if ( typeNum == TYPE_COLOR )
		return getCardinality() * colorComps * m_typeSizes[typeNum];
	return getCardinality() * m_typeSizes[typeNum];
}


////////////////////////////////////////////////////////////////////////////////
// isDefault - Is the declaration a default declaration
//
// Returns:
//   false if the declaration is not a default declaration
//
bool TParameterDeclaration::isDefault() const {
	return m_isDefault;
}


////////////////////////////////////////////////////////////////////////////////
// isInline - Is the declaration an inline declaration
//
// Returns:
//   false if the declaration is not an inline declaration
//
bool TParameterDeclaration::isInline() const {
	return m_isInline;
}


////////////////////////////////////////////////////////////////////////////////
// operator = - copies decl int *this
//
// Parameters:
//   decls - TParameterDeclaration to be copied to *this
//
// Returns:
//   the instance itself (*this)
//
TParameterDeclaration &TParameterDeclaration::operator=(const TParameterDeclaration &decl) {
	if ( &decl == this )
		return *this;

	m_name = decl.m_name;
	m_fullname = decl.m_fullname;
	m_class = decl.m_class;
	m_type = decl.m_type;
	m_cardinality = decl.m_cardinality;
	m_isInline = decl.m_isInline;
	m_isDefault = decl.m_isDefault;

	return *this;
}


////////////////////////////////////////////////////////////////////////////////
// getSize - returns the number of elements for this declaration
//
// Parameters:
//   vertices       : Number of vertices (used by vertex class)
//   corners        : Number of corners (used by varying class)
//   facets         : Number of facets (used by uniform class)
//   facevertices   : Number of vertices per face (used by facevertex class)
//   facecorners    : Number of corners per face (used by facevarying class)
//
// Returns:
//   number of elements of a parameter
//
int TParameterDeclaration::getSize(RtInt vertices, RtInt corners, RtInt facets, RtInt faceVertices, RtInt faceCorners) const {
	int n = 1;
	switch ( m_class ) {
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
		default: // CLASS_UNKNOWN, CLASS_CONSTANT
			break;
	}

	if ( n < 1 )
		n = 1;

	return n;
}


////////////////////////////////////////////////////////////////////////////////
// numElems - returns the number of elements for this declaration, color
//            are the number of colorcomps, depending on the cardinality
//
// Parameters:
//   vertices       : Number of vertices (used by vertex class)
//   corners        : Number of corners (used by varying class)
//   facets         : Number of facets (used by uniform class)
//   facevertices   : Number of vertices per face (used by facevertex class)
//   facecorners    : Number of corners per face (used by facevarying class)
//
// Returns:
//   number of array elements of a parameter
//
int TParameterDeclaration::numElems(RtInt vertices, RtInt corners, RtInt facets, RtInt faceVertices, RtInt faceCorners) const {
	return getSize(vertices, corners, facets, faceVertices, faceCorners) * getCardinality();
}


////////////////////////////////////////////////////////////////////////////////
// size - returns the bytes used for a parameter of this declaration
//
// Parameters:
//   vertices       : Number of vertices (used by vertex class)
//   corners        : Number of corners (used by varying class)
//   facets         : Number of facets (used by uniform class)
//   facevertices   : Number of vertices per face (used by facevertex class)
//   facecorners    : Number of corners per face (used by facevarying class)
//   colorComps     : Number of components per color
//
// Returns:
//   number of bytes
//
int TParameterDeclaration::size(RtInt vertices, RtInt corners, RtInt facets, RtInt faceVertices, RtInt faceCorners, RtInt colorComps) const {
	int n = numElems(vertices, corners, facets, faceVertices, faceCorners);
	if ( m_type == TYPE_COLOR )
		n *= colorComps;
	return n*sizeOfType(m_type);
}

int TParameterDeclaration::numComponents(RtInt vertices, RtInt corners, RtInt facets, RtInt faceVertices, RtInt faceCorners, RtInt colorComps) const {
	int n = numElems(vertices, corners, facets, faceVertices, faceCorners);
	return n*getComponents(colorComps);;
}
