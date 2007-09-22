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
 *  @brief Implementation of some types, classes and data about types needed by the interface.
 *  @author Andreas Pidde (andreas@pidde.de)
 */
#include "ricpp/declaration/types.h"
#include "ricpp/tools/inlinetools.h"
#include "ricpp/ricpp/ricpptokens.h"

#include <cstring>
#include <cctype>

#include <string>

using namespace RiCPP;

const unsigned int CTypeInfo::ms_basicTypeSizes[N_BASICTYPES] =
{
	0,
	sizeof(RtInt),
	sizeof(RtFloat),
	sizeof(RtString)
};

RtToken CTypeInfo::ms_basicTypeNames[N_BASICTYPES] =
{
	RI_EMPTY,
	RI_FLOAT,
	RI_INTEGER,
	RI_STRING
};

RtToken CTypeInfo::ms_typeNames[N_TYPES+1] =
{
	RI_EMPTY,
	RI_FLOAT,
	RI_INTEGER,
	RI_STRING,
	RI_POINT,
	RI_VECTOR,
	RI_NORMAL,
	RI_HPOINT,
	RI_MATRIX,
	RI_COLOR,
	RI_INT
};

const unsigned int CTypeInfo::ms_typeSizes[N_TYPES+1] =
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

const unsigned int CTypeInfo::ms_typeByteSizes[N_TYPES+1] =
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

const EnumBasicTypes CTypeInfo::ms_basicTypesForTypes[N_TYPES+1] =
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

RtToken CTypeInfo::ms_classNames[N_CLASSES] =
{
	RI_EMPTY,
	RI_CONSTANT,
	RI_UNIFORM,
	RI_VARYING,
	RI_VERTEX,
	RI_FACEVARYING,
	RI_FACEVERTEX
};

RtToken CTypeInfo::ms_namespaces[N_NAMESPACES] =
{
	RI_EMPTY,
	RI_PROJECTION,
	RI_IMAGER,
	RI_DISPLAY,
	RI_HIDER,
	RI_OPTION,
	RI_LIGHT,
	RI_AREALIGHT,
	RI_SURFACE,
	RI_ATMOSPHERE,
	RI_INTERIOR,
	RI_EXTERIOR,
	RI_DISPLACEMENT,
	RI_DEFORMATION,
	RI_VOLUME,
	RI_GEOMETRICAPPROXYMATION,
	RI_COORDINATESYSTEM,
	RI_ATTRIBUTE,
	RI_GEOMETRY,
	RI_TEXTURE,
	RI_READARCHIVE,
	RI_ARCHIVE,
	RI_DISPLAYCHANNEL,
	RI_RESOURCE
};

int CTypeInfo::tokcmp(const char *token, const char *search)
{
	if ( emptyStr(token) ) return search && *search ? -1 : 0;
	if ( emptyStr(search) ) return token && *token ? 1 : 0;

	int comp;
	const char *cp1 = token;
	const char *cp2 = search;

	while ( *cp2 && !isspace(*cp2) && *cp2 != '[' && *cp2 != ':' ) {
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


EnumNamespaces CTypeInfo::namespacePrefix(const char *avar, size_t &pos)
{
	int i;
	if ( emptyStr(avar) )
		return NAMESPACE_UNKNOWN;
	for ( i = 1; i < N_NAMESPACES; ++i ) {
		if ( !tokcmp(ms_namespaces[i], avar) ) {
			pos = strlen(ms_namespaces[i]);
			if ( avar[pos] == ':' ) {
				++pos;
			} else {
				if ( avar[pos] && avar[pos] != ':' ) {
					return NAMESPACE_UNKNOWN;
				}
			}
			return (EnumNamespaces)i;
		}
	}
	return NAMESPACE_UNKNOWN;
}

EnumClasses CTypeInfo::classPrefix(const char *aclass, size_t &pos)
{
	int i;
	if ( !aclass ) return CLASS_UNKNOWN;
	for ( i = 1; i < N_CLASSES; ++i ) {
		if ( !tokcmp(ms_classNames[i], aclass) ) {
			pos = strlen(ms_classNames[i]);
			return (EnumClasses)i;
		}
	}
	return CLASS_UNKNOWN;
}


EnumTypes CTypeInfo::typePrefix(const char *type, size_t &pos)
{
	int i;
	if ( !type ) return TYPE_UNKNOWN;
	for ( i = 1; i < N_TYPES+1; ++i ) {
		if ( !tokcmp(ms_typeNames[i], type) ) {
			pos = strlen(ms_typeNames[i]);
			if ( i == N_TYPES ) {
				// let "int" be "integer"
				i = TYPE_INTEGER;
			}
			return (EnumTypes)i;
		}
	}
	return TYPE_UNKNOWN;
}

int CTypeInfo::arrayPrefix(const char *acard, size_t &pos)
{
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


RtToken CTypeInfo::basicTypeName(EnumBasicTypes e)
{
	return ms_basicTypeNames[e];
}

unsigned int CTypeInfo::basicTypeByteSize(EnumBasicTypes e)
{
	return ms_basicTypeSizes[e];
}


RtToken CTypeInfo::typeName(EnumTypes e)
{
	return ms_typeNames[e];
}

unsigned int CTypeInfo::typeSize(EnumTypes e)
{
	return ms_typeSizes[e];
}

RtToken CTypeInfo::className(EnumClasses e)
{
	return ms_classNames[e];
}

RtToken CTypeInfo::tableNamespace(EnumNamespaces e)
{
	return ms_namespaces[e];
}

unsigned int CTypeInfo::typeByteSize(EnumTypes e)
{
	return ms_typeByteSizes[e];
}

EnumBasicTypes CTypeInfo::basicTypeForType(EnumTypes e)
{
	return ms_basicTypesForTypes[e];
}

// ----------------------------------------------------------------------------

CColorDescr::CColorDescr()
{
	m_nColorSamples = 3;

	m_nRGB.resize(9);
	m_nRGB[0] = 1.0; m_nRGB[1] = 0.0; m_nRGB[2] = 0.0;
	m_nRGB[3] = 0.0; m_nRGB[4] = 1.0; m_nRGB[5] = 0.0;
	m_nRGB[6] = 0.0; m_nRGB[7] = 0.0; m_nRGB[8] = 1.0;

	m_RGBn.resize(9);
	m_RGBn[0] = 1.0; m_RGBn[1] = 0.0; m_RGBn[2] = 0.0;
	m_RGBn[3] = 0.0; m_RGBn[4] = 1.0; m_RGBn[5] = 0.0;
	m_RGBn[6] = 0.0; m_RGBn[7] = 0.0; m_RGBn[8] = 1.0;

	m_isIdentity = true;
}


RtVoid CColorDescr::colorSamples(RtInt nColorSamples, RtFloat *nRGB, RtFloat *RGBn)
{
	std::vector<RtFloat>::size_type s;

	if ( nColorSamples <= 0 ) {
		nColorSamples = 3;
		nRGB = 0;
		RGBn = 0;
	}

	m_nColorSamples = nColorSamples;

	m_nRGB.clear();
	m_nRGB.resize(nColorSamples * 3);
	m_RGBn.clear();
	m_RGBn.resize(nColorSamples * 3);

	RtInt smax = nColorSamples * 3;
	for ( s = 0; static_cast<RtInt>(s) < smax; ++s ) {
		if ( nRGB ) {
			m_nRGB[s] = nRGB[s];
		} else {
			m_nRGB[s] =
				(s == 0 || s == 3 + 1 || s == 6 + 2) ?
				static_cast<RtFloat>(1.0) :
				0;
		}
		if ( RGBn ) {
			m_RGBn[s] = RGBn[s];
		} else {
			m_RGBn[s] =
				(s == 0 || s == nColorSamples + 1 || s == nColorSamples*2 + 2) ?
				static_cast<RtFloat>(1.0) :
				0;
		}
	}

	testident();
}


RtVoid CColorDescr::nToRGB(RtColor c, RtFloat rgb[3]) const
{
	if ( m_isIdentity ) {
		rgb[0] = c[0];
		rgb[1] = c[1];
		rgb[2] = c[2];
		return;
	}

	if ( !c )
		return;

	std::vector<RtFloat>::size_type o=0;
	for (int i = 0; i < 3; ++i )
	{
		RtFloat t = 0.0;
		for ( std::vector<RtFloat>::size_type s = 0;
		      static_cast<RtInt>(s) < m_nColorSamples;
		      ++s )
		{
			t += c[s] * m_nRGB[o+s];
		}
		rgb[i] = t;
		o += m_nColorSamples;
	}
}


RtVoid CColorDescr::nToRGB(RtInt n, RtFloat *c, RtFloat rgb[][3]) const
{
	if ( !c )
		return;

	for ( int i = 0; i < n; ++i )
	{
		nToRGB(c, rgb[i]);
		c += m_nColorSamples;
	}
}


RtVoid CColorDescr::RGBToN(RtFloat rgb[3], RtColor c) const
{
	if ( m_isIdentity ) {
		c[0] = rgb[0];
		c[1] = rgb[1];
		c[2] = rgb[2];
		return;
	}

	if ( !c )
		return;

	std::vector<RtFloat>::size_type o=0;
	for ( std::vector<RtFloat>::size_type s = 0;
		  static_cast<RtInt>(s) < m_nColorSamples;
		  ++s )
	{
		RtFloat t = 0.0;
		for ( int i = 0; i < 3; ++i )
		{
			t += rgb[i] * m_RGBn[o+i];
		}
		c[s] = t;
		o += 3;
	}
}


RtVoid CColorDescr::RGBToN(RtInt n, RtFloat rgb[][3], RtFloat *c) const
{
	if ( !c )
		return;

	for ( int i = 0; i < n; ++i )
	{
		RGBToN(rgb[i], c);
		c += m_nColorSamples;
	}
}


CColorDescr &CColorDescr::operator=(const CColorDescr &cd)
{
	if ( this == &cd )
		return *this;
	
	m_nColorSamples = cd.m_nColorSamples;
	m_nRGB = cd.m_nRGB;
	m_RGBn = cd.m_RGBn;
	m_isIdentity = cd.m_isIdentity;

	return *this;
}

bool CColorDescr::operator==(const CColorDescr &cd) const
{
	if ( m_nColorSamples != cd.m_nColorSamples )
		return false;

	if ( m_nRGB != cd.m_nRGB )
		return false;
	if ( m_RGBn != cd.m_RGBn )
		return false;

	return true;
}

bool CColorDescr::testident()
{
	if ( m_nColorSamples != 3 ) {
		m_isIdentity = false;
		return m_isIdentity;
	}

	m_isIdentity = true;

	std::vector<RtFloat>::size_type o=0;

	for ( std::vector<RtFloat>::size_type s = 0;
		  static_cast<RtInt>(s) < m_nColorSamples;
		  ++s )
	{
		for ( int i = 0; i < 3; ++i ) {
			if ( s == i ) {
				if ( m_RGBn[o+i] != 1 ) {
					m_isIdentity = false;
					return m_isIdentity;
				}
			} else {
				if ( m_RGBn[o+i] != 0 ) {
					m_isIdentity = false;
					return m_isIdentity;
				}
			}
		}
		o += 3;
	}

	o = 0;
	for ( int i = 0; i < 3; ++i ) {
		for ( std::vector<RtFloat>::size_type s = 0;
			  static_cast<RtInt>(s) < m_nColorSamples;
			  ++s )
		{
			if ( s == i ) {
				if ( m_nRGB[o+s] != 1 ) {
					m_isIdentity = false;
					return m_isIdentity;
				}
			} else {
				if ( m_nRGB[o+s] != 0 ) {
					m_isIdentity = false;
					return m_isIdentity;
				}
			}
		}
		o += m_nColorSamples;
	}

	return m_isIdentity;
}


// ----------------------------------------------------------------------------


CClippingPlane::CClippingPlane()
{
	m_x = 0;
	m_y = 0;
	m_z = 0;
	m_nx = 0;
	m_ny = 0;
	m_nz = 0;
}

CClippingPlane::CClippingPlane(RtFloat aX, RtFloat aY, RtFloat aZ, RtFloat aNX, RtFloat aNY, RtFloat aNZ)
{
	set(aX, aY, aZ, aNX, aNY, aNZ);
}


void CClippingPlane::set(RtFloat aX, RtFloat aY, RtFloat aZ, RtFloat aNX, RtFloat aNY, RtFloat aNZ)
{
	m_x = aX;
	m_y = aY;
	m_z = aZ;
	m_nx = aNX;
	m_ny = aNY;
	m_nz = aNZ;
}

CClippingPlane &CClippingPlane::operator=(const CClippingPlane &p)
{
	if ( this == &p )
		return *this;

	set(p.m_x, p.m_y, p.m_z, p.m_nx, p.m_ny, p.m_nz);

	return *this;
}

// ----------------------------------------------------------------------------

CQuantizer::CQuantizer()
{
	m_type = RI_NULL;
	m_one = 0; // if m_one is 0, quantization is not done
	m_min = 0;
	m_max = 0;
	m_ditherAmplitude = 0;
}
CQuantizer::CQuantizer(RtToken atype, RtInt aOne, RtInt aMin, RtInt aMax, RtFloat aDitherAmplitude)
{
	set(atype, aOne, aMin, aMax, aDitherAmplitude);
}
void CQuantizer::set(RtToken aType, RtInt aOne, RtInt aMin, RtInt aMax, RtFloat aDitherAmplitude)
{
	m_type = aType;
	m_one = aOne;
	m_min = aMin;
	m_max = aMax;
	m_ditherAmplitude = aDitherAmplitude;
}

CQuantizer &CQuantizer::operator=(const CQuantizer &q)
{
	if ( this == &q )
		return *this;

	set(q.m_type, q.m_one, q.m_min, q.m_max, q.m_ditherAmplitude);

	return *this;
}

RtFloat CQuantizer::quantize(RtFloat value) const
{
	if ( !m_one )
		return value;

	value = roundtempl(m_one*value + m_ditherAmplitude*randf2templ<RtFloat>());
	return clamptempl(value, static_cast<RtFloat>(m_min), static_cast<RtFloat>(m_max));
}

void CQuantizer::quantize(std::vector<RtFloat> &values) const
{
	if ( !m_one )
		return;

	for ( std::vector<RtFloat>::iterator i = values.begin();
		  i != values.end();
		  ++i )
	{
		*i = quantize(*i);
	}
}

// ----------------------------------------------------------------------------

void CTrimCurveData::trimCurve(
	RtInt nloops,
	const RtInt *ncurves,
	const RtInt *order,
	const RtFloat *knot,
	const RtFloat *amin,
	const RtFloat *amax,
	const RtInt *n,
	const RtFloat *u,
	const RtFloat *v,
	const RtFloat *w)
{
	releaseAll();

	if ( nloops == 0 || ncurves==NULL || order==NULL || knot==NULL ||
	     amin==NULL || amax==NULL || n==NULL || u==NULL || v==NULL || w==NULL )
	{
		return;
	}

	m_nLoops = nloops;
	if ( nloops > 0 ) {
		int i, j, sum;	// Counter

		m_total  = 0;	// Total number of curves
		m_npoints = 0;	// Total number of control points
		m_nknots  = 0;	// Total number of knots

		for ( i = 0; i < nloops; ++i ) {
			m_total += ncurves[i];
		}

		for ( i = 0; i < m_total; ++i ) {
			m_npoints += n[i];
			m_nknots += n[i] + order[i];
		}

		if ( m_total==0 || m_npoints==0 || m_nknots==0 ) {
			// Not a valid trim curve
			releaseAll();
			return;
		}

		m_nCurves.resize(nloops);
		m_nCurves.assign(ncurves, ncurves+nloops);
		m_order.resize(m_total);
		m_order.assign(order, order+m_total);
		m_knots.resize(m_nknots);
		m_knots.assign(knot, knot+m_nknots);
		m_min.resize(m_total);
		m_min.assign(amin, amin+m_total);
		m_max.resize(m_total);
		m_max.assign(amax, amax+m_total);
		m_n.resize(m_total);
		m_n.assign(n, n+m_total);
		m_u.resize(m_npoints);
		m_u.assign(u, u+m_npoints);
		m_v.resize(m_npoints);
		m_v.assign(v, v+m_npoints);
		m_w.resize(m_npoints);
		m_w.assign(w, w+m_npoints);

		m_points.resize(m_npoints*3);
		sum = 0;
		for ( i = 0; i < m_total; ++i ) {
			for ( j = 0; j < n[i]; ++j, ++sum ) {
				m_points[sum*3+0] = m_u[sum];
				m_points[sum*3+1] = m_v[sum];
				m_points[sum*3+2] = m_w[sum];
			}
		}
	}
}


// ----------------------------

// IMPLEMENTATION TMatrix3D
// ------------------------


/* Index [i][j]:
 *
 * [0][0] [0][1] [0][2] [0][3]
 * [1][0] [1][1] [1][2] [1][3]
 * [2][0] [2][1] [2][2] [2][3]
 * [3][0] [3][1] [3][2] [3][3]
 */

/*
 * Standard functions
 */

//! The standard constructor.
/*! Initializes the matric with the identity matrix, sets premultiplication.
 */
TMatrix3D::TMatrix3D() {
	m_preMultiply = true;
	identity();
}

//! The copy constructor (deep copy)
/*! \param mat Matrix to copy from
 */
TMatrix3D::TMatrix3D(const TMatrix3D &mat) {
	*this = mat;
}

//! The constructor to copy from a RenderMan RtMatrix.
/*! \param mat Matrix to copy from.
 */
TMatrix3D::TMatrix3D(RtMatrix mat) {
	m_preMultiply = true;
	set(mat);
}

//! Destructor, has nothing to do.
TMatrix3D::~TMatrix3D() {
}


/*
TMatrix3D &TMatrix3D::duplicate() const {
	TMatrix3D *matrix = new TMatrix3D(*this);
	if ( !matrix )
		throw TSystemException(__FILE__, __LINE__);
	return *matrix;
}
*/


//! The assignment (deep copy).
/*! \param mat Matrix to copy from.
 */
TMatrix3D &TMatrix3D::operator=(const TMatrix3D &mat) {
	if ( this == &mat )
		return *this;

	m_preMultiply = mat.m_preMultiply;
	set(mat.m_Matrix);
	return *this;
}


//! The assignment for a RenderMan RtMatrix.
/*! \param mat Matrix to copy from.
 */
TMatrix3D &TMatrix3D::operator=(RtMatrix mat) {
	// m_preMultiply is not changed
	set(mat);
	return *this;
}


#if 0
void TMatrix3D::set(const RtMatrix mat) {
	// m_preMultiply is not changed
	/*
	int i, j;
	for ( i = 0; i < 4; ++i )
		for ( j = 0; j < 4; ++j )
			m_Matrix[i][j] = mat[i][j];
	*/
}
#endif


//! Get the content of a matrix for a RenderMan RtMatrix.
/*! \param mat Matrix to fill.
 */
void TMatrix3D::get(RtMatrix &mat) const {
	int i, j;
	for ( i = 0; i < 4; ++i )
		for ( j = 0; j < 4; ++j )
			mat[i][j] = m_Matrix[i][j];
}


//! Compares two matrices.
/*! \param mat Matrix to compare with.
 *  \return true if all components of mat are equal to the components of the instance.
 */
bool TMatrix3D::operator==(const TMatrix3D &mat) const {
	if ( this == &mat )
		return true;
	int i, j;
	for ( i=0; i<4; ++i )
			for ( j=0; j<4; ++j )
					if ( m_Matrix[i][j] != mat.m_Matrix[i][j] ) return false;
	// m_preMultiply is not used to compare
	return true;
}


//! Compares with RtMatrix
/*! \param mat RenderMan RtMatrix matrix to compare with.
 *  \return true if all components of mat are equal to the components of the instance.
 */
bool TMatrix3D::operator==(RtMatrix mat) const {
        int i, j;
        for ( i=0; i<4; ++i )
                for ( j=0; j<4; ++j )
                        if ( m_Matrix[i][j] != mat[i][j] ) return false;
        return true;
}


//! Compares two matrices.
/*! \param mat Matrix to compare with.
 *  \return false if all components of mat are equal to the components of the instance.
 */
bool TMatrix3D::operator!=(const TMatrix3D &mat) const {
        return !(*this == mat);
}

//! Compares with RtMatrix
/*! \param mat RenderMan RtMatrix matrix to compare with.
 *  \return false if all components of mat are equal to the components of the instance.
 */
bool TMatrix3D::operator!=(RtMatrix mat) const {
        return !(*this == mat);
}

// matrix operations

//! Clears a matrix.
/*! Sets all components to zero.\n
 *  0 0 0 0\n
 *  0 0 0 0\n
 *  0 0 0 0\n
 *  0 0 0 0\n
 */
void TMatrix3D::clear() {
        int i, j;
        for ( i=0; i<4; ++i ) for ( j=0; j<4; ++j ) m_Matrix[i][j] = 0.0;
}

//! Set the identity matrix.
/* Sets the components to:\n
 * 1 0 0 0\n
 * 0 1 0 0\n
 * 0 0 1 0\n
 * 0 0 0 1\n
 */
void TMatrix3D::identity() {
        int i;
        clear();
        for ( i=0; i<4; ++i ) m_Matrix[i][i] = 1.0;
}

//! Transposes the matrix
/*!  Swaps the components as follows:
 *   m[i][j] = m[j][i]
 */
void TMatrix3D::transpose() {
        int i, j;
        RtFloat s;
        for ( i=0; i<3; ++i ) for ( j=1+i; j<4; ++j ) {
                s = m_Matrix[i][j];
                m_Matrix[i][j] = m_Matrix[j][i];
                m_Matrix[j][i] = s;
        }
}


//! Transform
/*! Transform is to copy a matrix:
 * M = mat
 * \param mat Matrix (RenderMan RtMatrix) used for transformation.
 */
void TMatrix3D::transform(RtMatrix mat) {
        set(mat);
}

//! Transform
/*! Transform is to copy a matrix:
 * M = mat
 * \param mat Matrix used for transformation.
 */
void TMatrix3D::transform(TMatrix3D &mat) {
        *this = mat;
}


//! Postmultiplicaton, rightmultiplication
/*! Postmultiplication is used by inverse transformations.
 * M' = M x mat
 * \param mat Matrix (RenderMan RtMatrix) to concatenate on the right.
 */
void TMatrix3D::postMultiply(RtMatrix mat) {
        int i, j, k;
        RtMatrix s;
        for ( i=0; i<4; ++i )
                for ( j=0; j<4; ++j ) {
                        s[i][j] = 0.0;
                        for ( k=0; k<4; ++k )
                                s[i][j] += m_Matrix[i][k] * mat[k][j];
                }
        *this = s;
}

//! Postmultiplicaton, rightmultiplication
/*! Postmultiplication is used by inverse transformations.
 * M' = M x mat
 * \param mat Matrix to concatenate on the right.
 */
void TMatrix3D::postMultiply(TMatrix3D mat) {
        postMultiply(mat.m_Matrix);
}


//! Premultiplication (leftmultiplication)
/* Premultiplication is used by the implementation of the RenderMan interface to concatenate transformations.
 * M' = mat x M
 * \param mat Matrix (RenderMan RtMatrix) to concatenate on the left.
 */
void TMatrix3D::preMultiply(RtMatrix mat) {
        int i, j, k;
        RtMatrix s;
        for ( i=0; i<4; ++i )
                for ( j=0; j<4; ++j ) {
                        s[i][j] = 0.0;
                        for ( k=0; k<4; ++k )
                                s[i][j] += mat[i][k] * m_Matrix[k][j];
                }
        *this = s;
}

//! Premultiplication (leftmultiplication)
/* Premultiplication is used by the implementation of the RenderMan interface to concatenate transformations.
 * M' = mat x M
 * \param mat Matrix to concatenate on the left.
 */
void TMatrix3D::preMultiply(TMatrix3D mat) {
        preMultiply(mat.m_Matrix);
}


// Standard multiplication

//! Matrix multiplication
/* m_preMultiply is used o multiply on the left or on the right
 * M' = mat x M (if m_preMultiply) or M' = M x Mat (if not m_preMultiply)
 * \param mat Matrix (RenderMan RtMatrix) to concatenate.
 */
void TMatrix3D::concatTransform(RtMatrix mat) {
		if ( m_preMultiply )
			preMultiply(mat);
		else
			postMultiply(mat);
}

//! Matrix multiplication
/* m_preMultiply is used o multiply on the left or on the right
 * M' = mat x M (if m_preMultiply) or M' = M x Mat (if not m_preMultiply)
 * \param mat Matrix to concatenate.
 */
void TMatrix3D::concatTransform(TMatrix3D mat) {
		if ( m_preMultiply )
	        preMultiply(mat.m_Matrix);
		else
			postMultiply(mat.m_Matrix);
}


//! Concatenate a translation
/*  The result matrix is:\n
 *  \verbatim
 *  a    b    c    d
 *  e    f    g    h
 *  i    j    k    l
 *  m+dx n+dy o+dz p
 *  \endverbatim
 *
 *  \param dx translate in x direction
 *  \param dy translate in y direction
 *  \param dz translate in z direction
 */
void TMatrix3D::translate(RtFloat dx, RtFloat dy, RtFloat dz) {
        RtMatrix mat;

        mat[0][0] = 1.0; mat[0][1] = 0.0; mat[0][2] = 0.0; mat[0][3] = 0.0;
        mat[1][0] = 0.0; mat[1][1] = 1.0; mat[1][2] = 0.0; mat[1][3] = 0.0;
        mat[2][0] = 0.0; mat[2][1] = 0.0; mat[2][2] = 1.0; mat[2][3] = 0.0;
        mat[3][0] = dx;  mat[3][1] = dy;  mat[3][2] = dz;  mat[3][3] = 1.0;

        concatTransform(mat);
}


//! Concatenate a scale
/*!  The result matrix is:\n
 *  \verbatim
 *  a*sx b    c    d
 *  e    f*sy g    h
 *  i    j    k*sz l
 *  m    n    o    p
 *  \endverbatim
 *
 *  \param sx scale in x direction
 *  \param sy scale in y direction
 *  \param sz scale in z direction
 */
void TMatrix3D::scale(RtFloat sx, RtFloat sy, RtFloat sz) {
        RtMatrix mat;

        mat[0][0] = sx;  mat[0][1] = 0.0; mat[0][2] = 0.0; mat[0][3] = 0.0;
        mat[1][0] = 0.0; mat[1][1] = sy;  mat[1][2] = 0.0; mat[1][3] = 0.0;
        mat[2][0] = 0.0; mat[2][1] = 0.0; mat[2][2] = sz;  mat[2][3] = 0.0;
        mat[3][0] = 0.0; mat[3][1] = 0.0; mat[3][2] = 0.0; mat[3][3] = 1.0;

        concatTransform(mat);
}


//! Transforms a point by the matrix
/*! If m_preMultiply it uses a row vector and left multiplication,
 *  if not m_preMultiply a column vector and left multiplication
 *  \retval x coordinate x and result
 *  \retval y coordinate y and result
 *  \retval z coordinate z and result
 */
void TMatrix3D::transformPoints(RtFloat &x, RtFloat &y, RtFloat &z) {
        int i, k;
        RtFloat d[4] = {0.0, 0.0, 0.0, 0.0};
        RtFloat p[4] = {x, y, z, 1.0};

		if ( m_preMultiply ) {
                for ( i=0; i<4; ++i) for ( k=0; k<4; ++k) d[i] += p[k] * m_Matrix[k][i];
		} else {
                for ( i=0; i<4; ++i) for ( k=0; k<4; ++k) d[i] += m_Matrix[i][k] * p[k];
		}

		x = d[0];
        y = d[1];
        z = d[2];
}


//! Concatenates a rotation around the x-axis
/*! The rotation matrix is:
 *  \verbatim
 *  1  0      0      0
 *  0  cos(w) sin(w) 0
 *  0 -sin(w) cos(w) 0
 *  0  0      0      1
 *  \endverbatim
 *  \param w degrees to rotate
 */
void TMatrix3D::rotateX(RtFloat w) {
        TMatrix3D r;

        w = deg2rad(w);

        RtFloat sinw = (RtFloat)sin(w);
        RtFloat cosw = (RtFloat)cos(w);
        r.m_Matrix[1][1] = cosw;
        r.m_Matrix[1][2] = sinw;
        r.m_Matrix[2][1] = -sinw;
        r.m_Matrix[2][2] = cosw;
        concatTransform(r.m_Matrix);
}


//! Concatenates a rotation around the y-axis
/*! The rotation matrix is:
 *  \verbatim
 *  cos(w) 0 -sin(w) 0
 *  0      1  0      0
 *  sin(w) 0  cos(w) 0
 *  0      0  0      1
 *  \endverbatim
 *  \param w degrees to rotate
 */
void TMatrix3D::rotateY(RtFloat w) {
        TMatrix3D r;

        w = deg2rad(w);

        RtFloat sinw = (RtFloat)sin(w);
        RtFloat cosw = (RtFloat)cos(w);
        r.m_Matrix[0][0] = cosw;
        r.m_Matrix[0][2] = -sinw;
        r.m_Matrix[2][0] = sinw;
        r.m_Matrix[2][2] = cosw;
        concatTransform(r.m_Matrix);
}


//! Concatenates a rotation around the z-axis
/*! The rotation matrix is:
 *  \verbatim
 *  cos(w) sin(w) 0 0
 * -sin(w) cos(w) 0 0
 *  0      0      1 0
 *  0      0      0 1
 *  \endverbatim
 *  \param w degrees to rotate
 */
void TMatrix3D::rotateZ(RtFloat w) {
        TMatrix3D r;

        w = deg2rad(w);

        RtFloat sinw = (RtFloat)sin(w);
        RtFloat cosw = (RtFloat)cos(w);
        r.m_Matrix[0][0] = cosw;
        r.m_Matrix[0][1] = sinw;
        r.m_Matrix[1][0] = -sinw;
        r.m_Matrix[1][1] = cosw;
        concatTransform(r.m_Matrix);
}


//! Rotates w degrees around the axis vector x, y, z
/*! The matrix is:\n
 *  \verbatim
 *  [0][0] = (RtFloat)(x*x + cosw*(1.0 - x*x));
 *  [0][1] = (RtFloat)(x*y*(1.0 - cosw) + z*sinw);
 *  [0][2] = (RtFloat)(z*x*(1.0 - cosw) - y*sinw);
 *  [1][0] = (RtFloat)(x*y*(1.0 - cosw) - z*sinw);
 *  [1][1] = (RtFloat)(y*y + cosw*(1.0 - y*y));
 *  [1][2] = (RtFloat)(y*z*(1.0 - cosw) + x*sinw);
 *  [2][0] = (RtFloat)(z*x*(1.0 - cosw) + y*sinw);
 *  [2][1] = (RtFloat)(y*z*(1.0 - cosw) - x*sinw);
 *  [2][2] = (RtFloat)(z*z + cosw*(1.0 - z*z));
 *  \endverbatim
 *  \param w degrees to rotate
 *  \param x x-coordinate of the axis vector
 *  \param y y-coordinate of the axis vector
 *  \param z z-coordinate of the axis vector
 */
void TMatrix3D::rotate(RtFloat w, RtFloat x, RtFloat y, RtFloat z) {
        TMatrix3D r;

        if ( x > 0.0 && y == 0.0 && z == 0.0 ) { rotateX(w); return; }
        if ( x < 0.0 && y == 0.0 && z == 0.0 ) { rotateX(-w); return; }
        if ( x == 0.0 && y > 0.0 && z == 0.0 ) { rotateY(w); return; }
        if ( x == 0.0 && y < 0.0 && z == 0.0 ) { rotateY(-w); return; }
        if ( x == 0.0 && y == 0.0 && z > 0.0 ) { rotateZ(w); return; }
        if ( x == 0.0 && y == 0.0 && z < 0.0 ) { rotateZ(-w); return; }

        w = deg2rad(w);

        RtFloat length = (RtFloat)sqrt(x*x + y*y + z*z);
        RtFloat sinw = (RtFloat)sin(w);
        RtFloat cosw = (RtFloat)cos(w);

        x /= length;
        y /= length;
        z /= length;


        r.m_Matrix[0][0] = (RtFloat)(x*x + cosw*(1.0 - x*x));
        r.m_Matrix[0][1] = (RtFloat)(x*y*(1.0 - cosw) + z*sinw);
        r.m_Matrix[0][2] = (RtFloat)(z*x*(1.0 - cosw) - y*sinw);
        r.m_Matrix[1][0] = (RtFloat)(x*y*(1.0 - cosw) - z*sinw);
        r.m_Matrix[1][1] = (RtFloat)(y*y + cosw*(1.0 - y*y));
        r.m_Matrix[1][2] = (RtFloat)(y*z*(1.0 - cosw) + x*sinw);
        r.m_Matrix[2][0] = (RtFloat)(z*x*(1.0 - cosw) + y*sinw);
        r.m_Matrix[2][1] = (RtFloat)(y*z*(1.0 - cosw) - x*sinw);
        r.m_Matrix[2][2] = (RtFloat)(z*z + cosw*(1.0 - z*z));
        concatTransform(r.m_Matrix);
}

//! Skews a vector (x1, y1, z1) w degrees towards (x2, y2, z2)
/*! s.a. More Matrices and Transformations: Shear and Pseudo-Perspective
 *      Ronald N. Goldman in Graphics Gems II, Academic Press Inc.
 * \verbatim
 * Shear(Q,v,w,phi) (Q = (0, 0, 0), v = (x1, y1, z1), w = (x2, y2, z2), phi = w)
 * | I+tan phi(v (X) w) 0 | I : id matrix, (X) : tensor product
 * | -(Q.v)w            1 | . : dot product
 * \endverbatim
 */
void TMatrix3D::skew(RtFloat w,
                                         RtFloat x1, RtFloat y1, RtFloat z1,
                                         RtFloat x2, RtFloat y2, RtFloat z2) {
        RtMatrix mat;
        RtFloat t;

        RtFloat length1 = sqrt(x1*x1 + y1*y1 + z1*z1);
        RtFloat length2 = sqrt(x2*x2 + y2*y2 + z2*z2);

        if ( length1 == 0.0 || length2 == 0.0 ) return; // undefined

        w = deg2rad(w);

        x1 /= length1;
        y1 /= length1;
        z1 /= length1;

        x2 /= length2;
        y2 /= length2;
        z2 /= length2;

        if ( fabs(w) >= pi_2<RtFloat>() ) return; // undefined w >= pi/2
        t = tan(w);

        mat[0][0] = (RtFloat)(1.0 + t * x1*x2); mat[0][1] = (RtFloat)(t * x1*y2);       mat[0][2] = (RtFloat)(t * x1*z2);       mat[0][3] = 0.0;
        mat[1][0] = (RtFloat)(t * y1*x2);       mat[1][1] = (RtFloat)(1.0 + t * y1*y2); mat[1][2] = (RtFloat)(t * y1*z2);       mat[1][3] = 0.0;
        mat[2][0] = (RtFloat)(t * z1*x2);       mat[2][1] = (RtFloat)(t * z1*y2);       mat[2][2] = (RtFloat)(1.0 + t * z1*z2); mat[2][3] = 0.0;
        mat[3][0] = 0.0;                        mat[3][1] = 0.0;                        mat[3][2] = 0.0;                        mat[3][3] = 1.0;

        concatTransform(mat);
}


//!Concatenates the RMan perspective transformation
/*! Code copied from the aqsis renderer, concatenates the matrix:
 * \verbatim
 * f = (RtFloat)tan(degtorad(fov)/2.0);
 *
 * 1.0 0.0 0.0 0.0
 * 0.0 1.0 0.0 0.0
 * 0.0 0.0   f   f
 * 0.0 0.0  -f 0.0
 * \endverbatim
 * \param fov The field of view for the perspective nmatrix
 */
void TMatrix3D::perspectiveProject(RtFloat fov) {
        RtMatrix mat;

        if ( fov >= 180.0F ) {
                // It's an error - so do nothing
                return;
        }

        RtFloat f = deg2rad(fov);
        f = (RtFloat)tan(f/2.0);

        mat[0][0] = 1.0; mat[0][1] = 0.0; mat[0][2] = 0.0; mat[0][3] = 0.0;
        mat[1][0] = 0.0; mat[1][1] = 1.0; mat[1][2] = 0.0; mat[1][3] = 0.0;
        mat[2][0] = 0.0; mat[2][1] = 0.0; mat[2][2] =  f;  mat[2][3] = f;
        mat[3][0] = 0.0; mat[3][1] = 0.0; mat[3][2] = -f;  mat[3][3] = 0.0;

        concatTransform(mat);
}

//---------------------------------------------------------------------
//! A utility function, used by determinant()
/*! Calculates the determinant of a 2x2 matrix. Source copied from
 *  the Aqsis renderer.
 *  The matrix:
 *  \verbatim
 *  a b
 *  c d
 *  \endverbatim
 *  \param a Component of a 2x2 matrix
 *  \param b Component of a 2x2 matrix
 *  \param c Component of a 2x2 matrix
 *  \param d Component of a 2x2 matrix
 *  \return The determinant of the 2x2 matrix
 */

static RtFloat det2x2(RtFloat a, RtFloat b, RtFloat c, RtFloat d)
{
    return a * d - b * c;
}

//---------------------------------------------------------------------
//! A utility function, used by determinant()
/*! Calculates the determinant of a 3x3 matrix. Source copied from
 *  the Aqsis renderer
 *  The matrix is:
 *  \verbatim
 *  a1 a2 a3
 *  b1 b2 b3
 *  c1 c2 c3
 *  \endverbatim
 *  \param a1 Component of a 3x3 matrix
 *  \param a2 Component of a 3x3 matrix
 *  \param a3 Component of a 3x3 matrix
 *  \param b1 Component of a 3x3 matrix
 *  \param b2 Component of a 3x3 matrix
 *  \param b3 Component of a 3x3 matrix
 *  \param c1 Component of a 3x3 matrix
 *  \param c2 Component of a 3x3 matrix
 *  \param c3 Component of a 3x3 matrix
 *  \return The determinant of the 3x3 matrix
 */

static RtFloat det3x3(RtFloat a1, RtFloat a2, RtFloat a3,
                       RtFloat b1, RtFloat b2, RtFloat b3,
                       RtFloat c1, RtFloat c2, RtFloat c3)
{
    return a1 * det2x2(b2, b3, c2, c3) -
           b1 * det2x2(a2, a3, c2, c3) +
           c1 * det2x2(a2, a3, b2, b3);
}


//---------------------------------------------------------------------
//! Returns the determinant of this matrix
/*! Using an algorithm from Graphics Gems I (p768). Source copied from
 *  Aqsis renderer.
 *  \return The determinant of the homogene matrix
 */

RtFloat TMatrix3D::determinant() const
{
    // Assign to individual variable names to aid selecting correct elements
    RtFloat a1 = m_Matrix[0][0];
    RtFloat b1 = m_Matrix[0][1];
    RtFloat c1 = m_Matrix[0][2];
    RtFloat d1 = m_Matrix[0][3];

    RtFloat a2 = m_Matrix[1][0];
    RtFloat b2 = m_Matrix[1][1];
    RtFloat c2 = m_Matrix[1][2];
    RtFloat d2 = m_Matrix[1][3];

    RtFloat a3 = m_Matrix[2][0];
    RtFloat b3 = m_Matrix[2][1];
    RtFloat c3 = m_Matrix[2][2];
    RtFloat d3 = m_Matrix[2][3];

    RtFloat a4 = m_Matrix[3][0];
    RtFloat b4 = m_Matrix[3][1];
    RtFloat c4 = m_Matrix[3][2];
    RtFloat d4 = m_Matrix[3][3];

    return a1 * det3x3(b2, b3, b4, c2, c3, c4, d2, d3, d4) -
           b1 * det3x3(a2, a3, a4, c2, c3, c4, d2, d3, d4) +
           c1 * det3x3(a2, a3, a4, b2, b3, b4, d2, d3, d4) -
           d1 * det3x3(a2, a3, a4, b2, b3, b4, c2, c3, c4);
}



//! Test if matrix is the identity matrix
/*! \return true: if matrix is the identity matrix, false: otherwise
 */
bool TMatrix3D::isIdentity() const {
        int i, j;
        bool eq = true;

        for ( i = 0; i < 4 ; ++i ) {
                for ( j = 0; j < 4 ; ++j ) {
                        eq = eq && ((m_Matrix[i][j] == 0.0) || ((i==j) && (m_Matrix[i][j] == 1.0)));
                }
        }

        return eq;
}


//! Returns the inverse of this matrix
/*! Using an algorithm from Graphics Gems IV (p554),
 * Gauss-Jordan elimination with partial pivoting.
 * Source copied from Aqsis renderer
 * \retval mat The inverse matrix
 * \return true: Inverse could be calculated, false: otherwise
 */
bool TMatrix3D::getInverse(RtMatrix &mat) const
{
        if ( isIdentity() )
        {
                get(mat);
                return true;

        }

        TMatrix3D b;                // b evolves from identity into inverse(a)
        TMatrix3D a(*this);        // a evolves from original matrix into identity

        b.identity();

        int i;
        int j;
        int i1;

        // Loop over cols of a from left to right, eliminating above and below diag
        for (j = 0; j < 4; j++)        // Find largest pivot in column j among rows j..3
        {
                i1 = j;
                for(i = j + 1; i < 4; i++)
                {
                        if(fabs(a.m_Matrix[i][j]) > fabs(a.m_Matrix[i1][j]))
                        {
                                i1 = i;
                        }
                }

                if (i1 != j)
                {
                        // Swap rows i1 and j in a and b to put pivot on diagonal
                        RtFloat temp;

                        temp = a.m_Matrix[i1][0];
                        a.m_Matrix[i1][0] = a.m_Matrix[j][0];
                        a.m_Matrix[j][0] = temp;
                        temp = a.m_Matrix[i1][1];
                        a.m_Matrix[i1][1] = a.m_Matrix[j][1];
                        a.m_Matrix[j][1] = temp;
                        temp = a.m_Matrix[i1][2];
                        a.m_Matrix[i1][2] = a.m_Matrix[j][2];
                        a.m_Matrix[j][2] = temp;
                        temp = a.m_Matrix[i1][3];
                        a.m_Matrix[i1][3] = a.m_Matrix[j][3];
                        a.m_Matrix[j][3] = temp;

                        temp = b.m_Matrix[i1][0];
                        b.m_Matrix[i1][0] = b.m_Matrix[j][0];
                        b.m_Matrix[j][0] = temp;
                        temp = b.m_Matrix[i1][1];
                        b.m_Matrix[i1][1] = b.m_Matrix[j][1];
                        b.m_Matrix[j][1] = temp;
                        temp = b.m_Matrix[i1][2];
                        b.m_Matrix[i1][2] = b.m_Matrix[j][2];
                        b.m_Matrix[j][2] = temp;
                        temp = b.m_Matrix[i1][3];
                        b.m_Matrix[i1][3] = b.m_Matrix[j][3];
                        b.m_Matrix[j][3] = temp;
                }

                // Scale row j to have a unit diagonal
                if( a.m_Matrix[j][j] == 0.0 )
                {
                        // Can't invert a singular matrix!
                        return false;
                }
                RtFloat scale = (RtFloat)(1.0 / a.m_Matrix[j][j]);
                b.m_Matrix[j][0] *= scale;
                b.m_Matrix[j][1] *= scale;
                b.m_Matrix[j][2] *= scale;
                b.m_Matrix[j][3] *= scale;
                // all elements to left of a[j][j] are already zero
                for (i1=j+1; i1<4; i1++)
                {
                        a.m_Matrix[j][i1] *= scale;
                }
                a.m_Matrix[j][j] = 1.0;

                // Eliminate off-diagonal elements in column j of a, doing identical ops to b
                for(i = 0; i < 4; i++)
                {
                        if(i != j)
                        {
                                scale = a.m_Matrix[i][j];
                                b.m_Matrix[i][0] -= scale * b.m_Matrix[j][0];
                                b.m_Matrix[i][1] -= scale * b.m_Matrix[j][1];
                                b.m_Matrix[i][2] -= scale * b.m_Matrix[j][2];
                                b.m_Matrix[i][3] -= scale * b.m_Matrix[j][3];

                                // all elements to left of a[j][j] are zero
                                // a[j][j] is 1.0
                                for (i1=j+1; i1<4; i1++)
                                {
                                        a.m_Matrix[i][i1] -= scale * a.m_Matrix[j][i1];
                                }
                                a.m_Matrix[i][j] = 0.0;
                        }
                }
        }
        b.get(mat);
        return true;
}
