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

/*! \file parameter.cpp
 *  \brief Implementation of a container for one parameter of the parameterlist of an interface call
 *  \author Andreas Pidde (andreas@pidde.de)
 */

#include "stdafx.h"
#include "parameter.h"
#include <assert.h>

////////////////////////////////////////////////////////////////////////////////
/*! The Standard constructur clears all member variables to their defaults
 */
TParameter::TParameter() {
	m_isInline = false;
	m_vertices = 1;
	m_corners = 1;
	m_facets = 1;
	m_faceVertices = 1;
	m_faceCorners = 1;
	m_colorComps = 3;
	m_position = 0;
	m_value = NULL;
	m_strings = NULL;
	m_stringsSize = 0;
	m_valueSize = 0;
}

////////////////////////////////////////////////////////////////////////////////
/*! Copy constructur initializes *this with the values of p
 *  \param p TParameter instance to copy from
 */
TParameter::TParameter(const TParameter &p) {
	m_isInline = false;
	m_vertices = 1;
	m_corners = 1;
	m_facets = 1;
	m_faceVertices = 1;
	m_faceCorners = 1;
	m_colorComps = 3;
	m_position = 0;
	m_value = NULL;
	m_strings = NULL;
	m_stringsSize = 0;
	m_valueSize = 0;
	*this = p;
}

////////////////////////////////////////////////////////////////////////////////
/*! Destructor, clears all memory allocated by an instance
 */
TParameter::~TParameter() {
	releaseAll();
}

////////////////////////////////////////////////////////////////////////////////
/*! Duplicate a complete parameter (deep copy).
 * \return A new instance, a deep copy of *this.
 * \exception TSystemException If memory for a new TParameter
 *            could not be allocated.
 */
TParameter &TParameter::duplicate() const {
	TParameter *p = new TParameter(*this);
	if ( !p )
		throw TSystemException(__FILE__, __LINE__);
	return *p;
}


////////////////////////////////////////////////////////////////////////////////
/*! Frees all used memory (values and strings) used by the instance.
 */
void TParameter::releaseAll() {
	if ( m_value )
		delete[] m_value;
	m_value = NULL;
	m_valueSize = 0;
	if ( m_strings )
		delete[] m_strings;
	m_strings = NULL;
	m_stringsSize = 0;
}


////////////////////////////////////////////////////////////////////////////////
/*! Assigns the element of par to *this (deep copy).
 * \param par The instance to copy from.
 * \return *this.
 * \exception TSystemException If memory for values could not be allocated.
 */
TParameter &TParameter::operator=(const TParameter &par) {
	if ( this == &par )
		return *this;

	releaseAll();

	m_isInline = par.m_isInline;
	m_vertices = par.m_vertices;
	m_corners = par.m_corners;
	m_facets = par.m_facets;
	m_faceVertices = par.m_faceVertices;
	m_faceCorners = par.m_faceCorners;
	m_colorComps = par.m_colorComps;
	m_position = par.m_position;
	m_decl = par.m_decl;

	// Copy the value of the parameter

	if ( par.m_strings && par.m_stringsSize ) {
		// Special handling for strings to get the references right.

		// First copy the strings
		m_stringsSize = par.m_stringsSize;
		m_strings = new char[m_stringsSize];
		if ( m_strings ) {
			memcpy(m_strings, par.m_strings, m_stringsSize);
		} else {
			m_stringsSize = 0;
			throw TSystemException(__FILE__, __LINE__);
		}

		// Rebuild the reference vector as m_value
		if ( par.m_value && par.m_valueSize ) {
			m_valueSize = par.m_valueSize;
			m_value = new unsigned char[m_valueSize];
			if ( m_value ) {
				unsigned int i;
				char *cp = m_strings;
				char **cplistdest = (char **)m_value;
				char **cplist = (char **)(par.m_value);

				// Walk throug the strings and set the pointers in m_value
				for ( i = 0; i < m_valueSize/sizeof(char *);  ++i ) {
					if ( cplist[i] ) {
						cplistdest[i]=cp;
						cp+=strlen(cp)+1;
					} else {
						cplistdest[i]=NULL;
					}
				}
			} else {
				m_valueSize = 0;
				throw TSystemException(__FILE__, __LINE__);
			}
		}

	} else if ( par.m_value && par.m_valueSize ) {
		m_valueSize = par.m_valueSize;
		m_value = new unsigned char[m_valueSize];
		if ( m_value ) {
			memcpy(m_value, par.m_value, m_valueSize);
		} else {
			m_valueSize = 0;
			throw TSystemException(__FILE__, __LINE__);
		}
	}

	return *this;
}


////////////////////////////////////////////////////////////////////////////////
/*! set - assigns the parameter values into an imnstance for a given declaration.
 * \param declaration  Points to a declaration of the parameter, if it has no inline
 *                     declaration (type is not used in this case).
 * \param vertices     Number of vertices (used by vertex class).
 * \param corners      Number of parametric corners (used by varying).
 * \param facets       Number of facets (used by uniform).
 * \param facevertices Number of vertices per face (used by facevertex class).
 * \param facecorners  Number of parametric corners per face (used by facevarying).
 * \param colorComps   Number of components per color.
 * \param pos          The position of the parameter in the parameterlist of the
 *                     function call.
 * \param type         Inline declaration, used if (declaration == NULL).
 * \param values       Pointer to the value of the parameter.
 * \return false: Something went wrong, true: OK
 * \exception TSystemException If memory of the parameter valiues could not get allocated.
 * \exception TRendererError   Thrown if the parsing of the inline declaration went wrong.
 */
bool TParameter::set(const TParameterDeclaration *declaration, RtInt vertices, RtInt corners, RtInt facets, RtInt faceVertices, RtInt faceCorners, RtInt colorComps, int pos, const char *type, const RtPointer values) {
	bool rVal = true;

	releaseAll();

	m_vertices = vertices;
	m_corners = corners;
	m_facets = facets;
	m_faceVertices = faceVertices;
	m_faceCorners = faceCorners;
	m_colorComps = colorComps;
	m_position = pos;

	if ( declaration ) {
		m_isInline = false;
		m_decl = *declaration;
	} else {
		m_isInline = true;
		rVal = m_decl.parse(NULL, type);
	}

	if ( rVal ) {
		m_valueSize = m_decl.size(m_vertices, m_corners, m_facets, m_faceVertices, m_faceCorners, m_colorComps);
		if ( !values )
			m_valueSize = 0;
		if ( m_valueSize ) {
			m_value = new unsigned char[m_valueSize];
			if ( !m_value ) {
				m_valueSize = 0;
				throw TSystemException(__FILE__, __LINE__);
				rVal = false;
			}

			if ( m_decl.getType() == TYPE_STRING ) {
				const char **cplist = (const char **)values;
				m_stringsSize = 0;
				if ( cplist ) {
					const char *cp;
					unsigned int i;
					for ( i = 0; i < m_valueSize/sizeof(char *); ++i ) {
						cp = cplist[i];
						if ( cp ) {
							m_stringsSize += (unsigned long)(strlen(cp)+1);
						}
					}
					m_strings = new char[m_stringsSize];
					char *cpdest = m_strings;
					char **cplistdest = (char **)m_value;
					if ( m_strings ) {
						for ( i = 0; i < m_valueSize/sizeof(char *); ++i ) {
							cp = cplist[i];
							if ( cp ) {
								strcpy(cpdest, cp);
								memcpy(cplistdest, &cpdest, sizeof(char *));
								cpdest += strlen(cp)+1;
							} else {
								// NULL
								memcpy(cplistdest, &cp, sizeof(char *));
							}
							cplistdest++;
						}
					} else {
						m_stringsSize = 0;
						throw TSystemException(__FILE__, __LINE__);
						rVal = false;
					}
				}
			} else {
				if ( m_value ) {
					memcpy(m_value, values, m_valueSize);
				} else {
					m_valueSize = 0;
					throw TSystemException(__FILE__, __LINE__);
					rVal = false;
				}
			}
		}
	}

	return rVal;
}

////////////////////////////////////////////////////////////////////////////////
/*! Get the pointer to the values the stored parameter
 *  \return A pointer the value of the parameter
 */
const RtPointer TParameter::getValue() const {
	return (const RtPointer)m_value;
}

////////////////////////////////////////////////////////////////////////////////
/*! Get the pointer to the values the stored parameter
 *  \return A pointer the value of the parameter at position pos
 */
const RtPointer TParameter::getValue(IndexType pos) const {
	assert(getNumberOfElements() > pos);
	if ( getNumberOfElements() <= pos )
		return NULL;
	return (const RtPointer)(m_value+pos*(getElemSize()*getCardinality()*getComponents()));
}

////////////////////////////////////////////////////////////////////////////////
/*! Get the name of the parameter, type and name if declared inline
 * \return The name or type and name of the parameter, rerieved from the declaration
 */
const char *TParameter::getName() const {
	return m_decl.getName();
}

////////////////////////////////////////////////////////////////////////////////
/*! Get the pure name of the parameter
 * \return The name of the parameter, rerieved from the declaration
 */
const char *TParameter::getParameterName() const {
	return m_decl.getParameterName();
}

////////////////////////////////////////////////////////////////////////////////
/*! \return Whether the declaration is inline or not
 */
bool TParameter::isInline() const {
	return m_isInline;
}

////////////////////////////////////////////////////////////////////////////////
/*! \return The number of color components
 */
RtInt TParameter::getColorComps() const {
	return m_colorComps;
}

////////////////////////////////////////////////////////////////////////////////
/*! \return The number of vertices
 */
RtInt TParameter::getVertices() const {
	return m_vertices;
}

////////////////////////////////////////////////////////////////////////////////
/*! \return The number of corners
 */
RtInt TParameter::getCorners() const {
	return m_corners;
}

////////////////////////////////////////////////////////////////////////////////
/*! \return The number of facets
 */
RtInt TParameter::getFacets() const {
	return m_facets;
}

////////////////////////////////////////////////////////////////////////////////
/*! \return the number of vertices per face
 */
RtInt TParameter::getFaceVertices() const {
	return m_faceVertices;
}

////////////////////////////////////////////////////////////////////////////////
/*! \return The number of corners per face
 */
RtInt TParameter::getFaceCorners() const {
	return m_faceCorners;
}

////////////////////////////////////////////////////////////////////////////////
/*! \return The position of the parameter in the parameterlist of
 *          a function call.
 */
unsigned long TParameter::getPosition() const {
	return m_position;
}

////////////////////////////////////////////////////////////////////////////////
/* \return The number of elements (depending of cardinality and
 *         class of the declaration)
 */
unsigned long TParameter::getNumberOfElements() const {
	return m_decl.numElems(m_vertices, m_corners, m_facets, m_faceVertices, m_faceCorners);
}

////////////////////////////////////////////////////////////////////////////////
/* \return The number of elements (depending of class of the declaration)
 */
unsigned long TParameter::getSize() const {
	return m_decl.getSize(m_vertices, m_corners, m_facets, m_faceVertices, m_faceCorners);
}

////////////////////////////////////////////////////////////////////////////////
/* \return The number of all basic elements
 */
unsigned long TParameter::getBasicSize() const {
	return getComponents() * getNumberOfElements();
}

////////////////////////////////////////////////////////////////////////////////
bool TParameter::copyToNamedVector(TNamedVector<RtFloat> &vec) const {
	if ( getBasicType() != TYPE_FLOAT )
		return false;
	
	vec.clear();
	vec.initialize(getName(), getComponents(), getCardinality(), getBasicSize());
	vec.copyFrom((RtFloat *)getValue());
	return true;
}

////////////////////////////////////////////////////////////////////////////////
bool TParameter::referNamedVectorTo(TNamedVector<RtFloat> &vec) const {
	if ( getBasicType() != TYPE_FLOAT )
		return false;
	
	vec.clear();
	vec.initialize(getName(), getComponents(), getCardinality(), getBasicSize(), (RtFloat *)getValue());
	return true;
}
