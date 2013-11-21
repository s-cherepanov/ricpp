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
#include "ricpp/ricpp/types.h"

#ifndef _RICPP_RIBASE_RICPPCONST_H
#include "ricpp/ribase/ricppconst.h"
#endif // _RICPP_RIBASE_RICPPCONST_H

#ifndef _RICPP_TOOLS_TEMPLATEFUNCS_H
#include "ricpp/tools/templatefuncs.h"
#endif // _RICPP_TOOLS_TEMPLATEFUNCS_H

#include <cctype>

#ifdef _DEBUG
// #define _TRACE
#endif

using namespace RiCPP;

static const unsigned int ms_basicTypeSizes[N_BASICTYPES] =
{
	0,
	sizeof(RtInt),
	sizeof(RtFloat),
	sizeof(RtString)
};

static RtToken ms_basicTypeNames[N_BASICTYPES] =
{
	RI_EMPTY,
	RI_FLOAT,
	RI_INTEGER,
	RI_STRING
};

static RtToken ms_typeNames[N_TYPES+1] = // +1: Extra type int -> integer
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

static const unsigned int ms_typeSizes[N_TYPES+1] =
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

static const unsigned int ms_typeByteSizes[N_TYPES+1] =
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

static const EnumBasicTypes ms_basicTypesForTypes[N_TYPES+1] =
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

static RtToken ms_classNames[N_CLASSES] =
{
	RI_EMPTY,
	RI_CONSTANT,
	RI_UNIFORM,
	RI_VARYING,
	RI_VERTEX,
	RI_FACEVARYING,
	RI_FACEVERTEX
};

static RtToken ms_qualifiers[N_QUALIFIERS] =
{
	RI_EMPTY,
	RI_PROJECTION,
	RI_IMAGER,
	RI_DISPLAY,
	RI_HIDER,
	RI_OPTION,
	RI_LIGHT_SOURCE,
	RI_AREA_LIGHT_SOURCE,
	RI_SURFACE,
	RI_ATMOSPHERE,
	RI_INTERIOR,
	RI_EXTERIOR,
	RI_DISPLACEMENT,
	RI_DEFORMATION,
	RI_ATTRIBUTE,
	RI_TEXTURE,
	RI_CONTROL,
	RI_BEGIN,
	RI_RESOURCE
};


RtVoid CTypeInfo::init()
{
	if ( !ms_basicTypeNames[0] ) {
		ms_basicTypeNames[0] = RI_EMPTY;
		ms_basicTypeNames[1] = RI_FLOAT;
		ms_basicTypeNames[2] = RI_INTEGER;
		ms_basicTypeNames[3] = RI_STRING;
	}

	if ( !ms_typeNames[0] ) {
		ms_typeNames[ 0] = RI_EMPTY;
		ms_typeNames[ 1] = RI_FLOAT;
		ms_typeNames[ 2] = RI_INTEGER;
		ms_typeNames[ 3] = RI_STRING;
		ms_typeNames[ 4] = RI_POINT;
		ms_typeNames[ 5] = RI_VECTOR;
		ms_typeNames[ 6] = RI_NORMAL;
		ms_typeNames[ 7] = RI_HPOINT;
		ms_typeNames[ 8] = RI_MATRIX;
		ms_typeNames[ 9] = RI_COLOR;
		ms_typeNames[10] = RI_INT;
	}

	if ( !ms_classNames[0] ) {
		ms_classNames[0] = RI_EMPTY;
		ms_classNames[1] = RI_CONSTANT;
		ms_classNames[2] = RI_UNIFORM;
		ms_classNames[3] = RI_VARYING;
		ms_classNames[4] = RI_VERTEX;
		ms_classNames[5] = RI_FACEVARYING;
		ms_classNames[6] = RI_FACEVERTEX;
	}

	if ( !ms_qualifiers[0] ) {
		ms_qualifiers[ 0] = RI_EMPTY;
		ms_qualifiers[ 1] = RI_PROJECTION;
		ms_qualifiers[ 2] = RI_IMAGER;
		ms_qualifiers[ 3] = RI_DISPLAY;
		ms_qualifiers[ 4] = RI_HIDER;
		ms_qualifiers[ 5] = RI_OPTION;
		ms_qualifiers[ 6] = RI_LIGHT_SOURCE;
		ms_qualifiers[ 7] = RI_AREA_LIGHT_SOURCE;
		ms_qualifiers[ 8] = RI_SURFACE;
		ms_qualifiers[ 9] = RI_ATMOSPHERE;
		ms_qualifiers[10] = RI_INTERIOR;
		ms_qualifiers[11] = RI_EXTERIOR;
		ms_qualifiers[12] = RI_DISPLACEMENT;
		ms_qualifiers[13] = RI_DEFORMATION;
		ms_qualifiers[14] = RI_ATTRIBUTE;
		ms_qualifiers[15] = RI_TEXTURE;
		ms_qualifiers[16] = RI_CONTROL;
		ms_qualifiers[17] = RI_BEGIN;
		ms_qualifiers[18] = RI_RESOURCE;
	}
}


CTypeInfo::CTypeInfo() { init(); }


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


EnumQualifiers CTypeInfo::qualifierPrefix(const char *avar, size_t &pos)
{
	int i;
	if ( emptyStr(avar) )
		return QUALIFIER_UNKNOWN;
	for ( i = 1; i < N_QUALIFIERS; ++i ) {
		if ( !tokcmp(ms_qualifiers[i], avar) ) {
			pos = strlen(ms_qualifiers[i]);
			if ( avar[pos] == ':' ) {
				++pos;
			} else {
				if ( avar[pos] && avar[pos] != ':' ) {
					return QUALIFIER_UNKNOWN;
				}
			}
			return (EnumQualifiers)i;
		}
	}
	return QUALIFIER_UNKNOWN;
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

bool CTypeInfo::arrayPrefix(const char *acard, size_t &pos, unsigned long &arraySize)
{
	int state = 0;
	long retval = 1;
	arraySize = 1;

	if ( !acard ) return false;

	for ( pos = 0; acard[pos]; ++pos ) {
		if ( isspace(acard[pos]) && state != 1 )
			continue;
		switch ( state ) {
		case 0:
			if ( acard[pos] == '[' ) {
				retval = 0;
				state = 1;
				break;
			}
			return false;
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
			return false;
		}
		if ( state == 3 ) {
			// arraySize == 0 is possible, if [0]
			++pos;
			break;
		}
	}

	arraySize = retval;
	return (state == 3 || state == 0);
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

RtToken CTypeInfo::qualifier(EnumQualifiers e)
{
	return ms_qualifiers[e];
}

EnumQualifiers CTypeInfo::qualifier(RtToken aName)
{
	for ( int i = 0; i < N_QUALIFIERS; ++i ) {
		if ( ms_qualifiers[i] == aName )
			return (EnumQualifiers)i;
	}
	return QUALIFIER_UNKNOWN;
}


unsigned int CTypeInfo::typeByteSize(EnumTypes e)
{
	return ms_typeByteSizes[e];
}

EnumBasicTypes CTypeInfo::basicTypeForType(EnumTypes e)
{
	return ms_basicTypesForTypes[e];
}

RtToken CTypeInfo::basisName(const RtBasis aBasis)
{
	if ( !memcmp(&(aBasis[0][0]), &(RiBezierBasis[0][0]), sizeof(RtBasis)) )
		return RI_BEZIER;
	if ( !memcmp(&(aBasis[0][0]), &(RiBSplineBasis[0][0]), sizeof(RtBasis)) )
		return RI_B_SPLINE;
	if ( !memcmp(&(aBasis[0][0]), &(RiCatmullRomBasis[0][0]), sizeof(RtBasis)) )
		return RI_CATMULL_ROM;
	if ( !memcmp(&(aBasis[0][0]), &(RiHermiteBasis[0][0]), sizeof(RtBasis)) )
		return RI_HERMITE;
	if ( !memcmp(&(aBasis[0][0]), &(RiPowerBasis[0][0]), sizeof(RtBasis)) )
		return RI_POWER;

	return RI_NULL;
}


bool CTypeInfo::getBasis(RtToken basisToken, RtBasis aBasis)
{
	if ( basisToken == RI_BEZIER ) {
		memcpy(&(aBasis[0][0]), &(RiBezierBasis[0][0]), sizeof(RtBasis));
		return true;
	}
	if ( basisToken == RI_B_SPLINE ) {
		memcpy(&(aBasis[0][0]), &(RiBSplineBasis[0][0]), sizeof(RtBasis));
		return true;
	}
	if ( basisToken == RI_CATMULL_ROM ) {
		memcpy(&(aBasis[0][0]), &(RiCatmullRomBasis[0][0]), sizeof(RtBasis));
		return true;
	}
	if ( basisToken == RI_HERMITE ) {
		memcpy(&(aBasis[0][0]), &(RiHermiteBasis[0][0]), sizeof(RtBasis));
		return true;
	}
	if ( basisToken == RI_POWER ) {
		memcpy(&(aBasis[0][0]), &(RiPowerBasis[0][0]), sizeof(RtBasis));
		return true;
	}
	
	
	return false;
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


RtVoid CColorDescr::colorSamples(RtInt nColorSamples, const RtFloat nRGB[], const RtFloat RGBn[])
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
				(s == 0 || (RtInt)s == nColorSamples + 1 || (RtInt)s == nColorSamples*2 + 2) ?
				static_cast<RtFloat>(1.0) :
				0;
		}
	}

	testident();
}


RtVoid CColorDescr::nToRGB(const RtFloat *c, RtFloat rgb[3]) const
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


RtVoid CColorDescr::nToRGB(RtInt n, const RtFloat *c, RtFloat rgb[][3]) const
{
	if ( !c )
		return;

	for ( int i = 0; i < n; ++i )
	{
		nToRGB(c, rgb[i]);
		c += m_nColorSamples;
	}
}


RtVoid CColorDescr::RGBToN(const RtFloat rgb[3], RtFloat *c) const
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


RtVoid CColorDescr::RGBToN(RtInt n, const RtFloat rgb[][3], RtFloat c[]) const
{
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
		for ( std::vector<RtFloat>::size_type i = 0; i < 3; ++i ) {
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
	for ( std::vector<RtFloat>::size_type i = 0; i < 3; ++i ) {
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
	m_nz = 1;
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

CQuantizer::CQuantizer(RtToken aType, RtInt aOne, RtInt aMin, RtInt aMax, RtFloat aDitherAmplitude)
{
	set(aType, aOne, aMin, aMax, aDitherAmplitude);
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

	value = tround(m_one*value + m_ditherAmplitude*trand2<RtFloat>());
	return clamp(value, static_cast<RtFloat>(m_min), static_cast<RtFloat>(m_max));
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

void CTrimCurveDataInfo::trimCurve(
	RtInt nloops,
	const RtInt ncurves[],
	const RtInt order[],
	const RtFloat knot[],
	const RtFloat amin[],
	const RtFloat amax[],
	const RtInt n[],
	const RtFloat u[],
	const RtFloat v[],
	const RtFloat w[])
{
	m_nloops = 0;
	m_total  = 0;
	m_npoints = 0;
	m_nknots  = 0;

	if ( nloops == 0 || ncurves==0 || order==0 || knot==0 ||
	     amin==0 || amax==0 || n==0 || u==0 || v==0 || w==0 )
	{

		return;
	}

	m_nloops = nloops;
	int i;	// Counter

	for ( i = 0; i < nloops; ++i ) {
		m_total += ncurves[i];
	}

	for ( i = 0; i < m_total; ++i ) {
		m_npoints += n[i];
		m_nknots += n[i] + order[i];
	}

	if ( m_total==0 || m_npoints==0 || m_nknots==0 ) {
		// Not a valid trim curve
		m_nloops = nloops;
		m_total  = 0;	// Total number of curves
		m_npoints = 0;	// Total number of control points
		m_nknots  = 0;	// Total number of knots

		return;
	}
}
	
// ----------------------------------------------------------------------------

void CTrimCurveData::trimCurve(
	RtInt nloops,
	const RtInt ncurves[],
	const RtInt order[],
	const RtFloat knot[],
	const RtFloat amin[],
	const RtFloat amax[],
	const RtInt n[],
	const RtFloat u[],
	const RtFloat v[],
	const RtFloat w[])
{
	releaseAll();

	m_data.trimCurve(nloops, ncurves, order, knot, amin, amax, n, u, v, w);

	if ( m_data.m_nloops == 0 )
	{
		return;
	}

	int i, j, sum;	// Counter

	m_nCurves.resize(m_data.m_nloops);
	m_nCurves.assign(ncurves, ncurves+m_data.m_nloops);
	m_order.resize(m_data.m_total);
	m_order.assign(order, order+m_data.m_total);
	m_knots.resize(m_data.m_nknots);
	m_knots.assign(knot, knot+m_data.m_nknots);
	m_min.resize(m_data.m_total);
	m_min.assign(amin, amin+m_data.m_total);
	m_max.resize(m_data.m_total);
	m_max.assign(amax, amax+m_data.m_total);
	m_n.resize(m_data.m_total);
	m_n.assign(n, n+m_data.m_total);
	m_u.resize(m_data.m_npoints);
	m_u.assign(u, u+m_data.m_npoints);
	m_v.resize(m_data.m_npoints);
	m_v.assign(v, v+m_data.m_npoints);
	m_w.resize(m_data.m_npoints);
	m_w.assign(w, w+m_data.m_npoints);

	m_points.resize(m_data.m_npoints*3);
	sum = 0;
	for ( i = 0; i < m_data.m_total; ++i ) {
		for ( j = 0; j < n[i]; ++j, ++sum ) {
			m_points[sum*3+0] = m_u[sum];
			m_points[sum*3+1] = m_v[sum];
			m_points[sum*3+2] = m_w[sum];
		}
	}

	m_segments.resize(m_data.m_total);
	sum = 0;
	for ( i = 0; i < m_data.m_total; ++i ) {
		m_segments[i] = nuNumSegs(m_n[i], m_order[i]);
		sum += m_segments[i];
	}
	m_nsegments = sum;
}


// ----------------------------

// IMPLEMENTATION CMatrix3D
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

CMatrix3D::CMatrix3D()
{
	m_preMultiply = true;
	identity();
}

CMatrix3D::CMatrix3D(const CMatrix3D &mat)
{
	*this = mat;
}

CMatrix3D::CMatrix3D(const RtMatrix mat)
{
	m_preMultiply = true;
	set(mat);
}

CMatrix3D::CMatrix3D(const RtFloat *mat)
{
	m_preMultiply = true;
	set(mat);
}

/*
CMatrix3D &CMatrix3D::duplicate() const {
	CMatrix3D *matrix = new CMatrix3D(*this);
	if ( !matrix )
		throw TSystemException(__FILE__, __LINE__);
	return *matrix;
}
*/


CMatrix3D &CMatrix3D::operator=(const CMatrix3D &mat)
{
	if ( this == &mat )
		return *this;

	m_preMultiply = mat.m_preMultiply;
	set(mat.m_Matrix);
	return *this;
}


CMatrix3D &CMatrix3D::operator=(const RtMatrix mat)
{
	// m_preMultiply not changed (not part of RtMatrix)

	set(mat);
	return *this;
}

void CMatrix3D::get(RtMatrix mat) const
{
	int i, j;
	for ( i = 0; i < 4; ++i )
		for ( j = 0; j < 4; ++j )
			mat[i][j] = m_Matrix[i][j];
}

void CMatrix3D::get(RtFloat *mat) const
{
	int i, j, k=0;
	for ( i = 0; i < 4; ++i ) {
		for ( j = 0; j < 4; ++j ) {
			mat[k+j] = m_Matrix[i][j];
		}
		k += 4;
	}
}

bool CMatrix3D::operator==(const CMatrix3D &mat) const
{
	if ( this == &mat )
		return true;
	int i, j;
	for ( i=0; i<4; ++i )
			for ( j=0; j<4; ++j )
					if ( m_Matrix[i][j] != mat.m_Matrix[i][j] ) return false;
	// m_preMultiply is not used to compare
	return true;
}


bool CMatrix3D::operator==(const RtMatrix mat) const
{
        int i, j;
        for ( i=0; i<4; ++i )
                for ( j=0; j<4; ++j )
                        if ( m_Matrix[i][j] != mat[i][j] ) return false;
        return true;
}


bool CMatrix3D::operator!=(const CMatrix3D &mat) const
{
        return !(*this == mat);
}

bool CMatrix3D::operator!=(const RtMatrix mat) const
{
        return !(*this == mat);
}

// matrix operations

void CMatrix3D::clear()
{
        int i, j;
        for ( i=0; i<4; ++i ) for ( j=0; j<4; ++j ) m_Matrix[i][j] = 0.0;
}

void CMatrix3D::identity()
{
        int i;
        clear();
        for ( i=0; i<4; ++i ) m_Matrix[i][i] = 1.0;
}

void CMatrix3D::transpose()
{
        int i, j;
        RtFloat s;
        for ( i=0; i<3; ++i ) for ( j=1+i; j<4; ++j ) {
                s = m_Matrix[i][j];
                m_Matrix[i][j] = m_Matrix[j][i];
                m_Matrix[j][i] = s;
        }
}


void CMatrix3D::transform(const RtMatrix mat)
{
        set(mat);
}

void CMatrix3D::transform(const CMatrix3D &mat)
{
        *this = mat;
}


void CMatrix3D::postMultiply(const RtMatrix mat)
{
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

void CMatrix3D::postMultiply(CMatrix3D mat)
{
        postMultiply(mat.m_Matrix);
}


void CMatrix3D::preMultiply(const RtMatrix mat)
{
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

void CMatrix3D::preMultiply(CMatrix3D mat)
{
        preMultiply(mat.m_Matrix);
}


// Standard multiplication

void CMatrix3D::concatTransform(const RtMatrix mat)
{
		if ( m_preMultiply )
			preMultiply(mat);
		else
			postMultiply(mat);
}

void CMatrix3D::concatTransform(CMatrix3D mat)
{
		if ( m_preMultiply )
	        preMultiply(mat.m_Matrix);
		else
			postMultiply(mat.m_Matrix);
}


void CMatrix3D::translate(RtFloat dx, RtFloat dy, RtFloat dz)
{
        RtMatrix mat;

        mat[0][0] = 1.0; mat[0][1] = 0.0; mat[0][2] = 0.0; mat[0][3] = 0.0;
        mat[1][0] = 0.0; mat[1][1] = 1.0; mat[1][2] = 0.0; mat[1][3] = 0.0;
        mat[2][0] = 0.0; mat[2][1] = 0.0; mat[2][2] = 1.0; mat[2][3] = 0.0;
        mat[3][0] = dx;  mat[3][1] = dy;  mat[3][2] = dz;  mat[3][3] = 1.0;

        concatTransform(mat);
}


void CMatrix3D::scale(RtFloat sx, RtFloat sy, RtFloat sz)
{
        RtMatrix mat;

        mat[0][0] = sx;  mat[0][1] = 0.0; mat[0][2] = 0.0; mat[0][3] = 0.0;
        mat[1][0] = 0.0; mat[1][1] = sy;  mat[1][2] = 0.0; mat[1][3] = 0.0;
        mat[2][0] = 0.0; mat[2][1] = 0.0; mat[2][2] = sz;  mat[2][3] = 0.0;
        mat[3][0] = 0.0; mat[3][1] = 0.0; mat[3][2] = 0.0; mat[3][3] = 1.0;

        concatTransform(mat);
}


void CMatrix3D::transformPoint(RtFloat &x, RtFloat &y, RtFloat &z) const
{
        int i, k;
        RtFloat d[4] = {0.0, 0.0, 0.0, 0.0};
        RtFloat p[4] = {x, y, z, 1.0};

		if ( m_preMultiply ) {
                for ( i=0; i<4; ++i) for ( k=0; k<4; ++k) d[i] += p[k] * m_Matrix[k][i];
		} else {
                for ( i=0; i<4; ++i) for ( k=0; k<4; ++k) d[i] += m_Matrix[i][k] * p[k];
		}

	if ( nearlyZero(d[3]) ) {
		x = d[0];
        y = d[1];
        z = d[2];
	} else {
		x = d[0]/d[3];
        y = d[1]/d[3];
        z = d[2]/d[3];
	}
}

bool CMatrix3D::transformNormal(RtFloat &x, RtFloat &y, RtFloat &z) const
{
	CMatrix3D tinv = *this;
	if ( !tinv.invert() )
		return false;
	tinv.transpose();
	tinv.transformPoint(x, y, z);
	normalize(x, y, z);
	return true;
}
	

void CMatrix3D::transformPoints(RtInt n, RtPoint p[]) const
{
	int i, j, k;

	n*=3;
	RtFloat *s = &p[0][0];
	
#ifdef _TRACE
	std::cout << "transformPoints " << std::endl;
	printMatrix(m_Matrix);
#endif // _TRACE

	if ( m_preMultiply ) {
		for ( j = 0; j < n; j+=3 ) {
			RtFloat t[4] = {s[j], s[j+1], s[j+2], 1.0};
			RtFloat d[4] = {0.0, 0.0, 0.0, 0.0};
			for ( i=0; i<4; ++i)
				for ( k=0; k<4; ++k)
					d[i] += t[k] * m_Matrix[k][i];
			if ( nearlyZero(d[3]) ) {
				s[j]   = d[0];
				s[j+1] = d[1];
				s[j+2] = d[2];
			} else {
				s[j]   = d[0]/d[3];
				s[j+1] = d[1]/d[3];
				s[j+2] = d[2]/d[3];
			}
		}
	} else {
		for ( j = 0; j < n; j+=3 ) {
			RtFloat t[4] = {s[j], s[j+1], s[j+2], 1.0};
			RtFloat d[4] = {0.0, 0.0, 0.0, 0.0};
			for ( i=0; i<4; ++i)
				for ( k=0; k<4; ++k)
					d[i] += m_Matrix[i][k] * t[k];
			if ( nearlyZero(d[3]) ) {
				s[j]   = d[0];
				s[j+1] = d[1];
				s[j+2] = d[2];
			} else {
				s[j]   = d[0]/d[3];
				s[j+1] = d[1]/d[3];
				s[j+2] = d[2]/d[3];
			}
		}
	}

}


bool CMatrix3D::transformNormals(RtInt n, RtPoint p[]) const
{
	CMatrix3D tinv = *this;
	if ( !tinv.invert() )
		return false;
	tinv.transpose();
	tinv.transformPoints(n, p);
	for (RtInt i=0; i < n; ++i) {
		normalize(p[i][0],p[i][1],p[i][2]);
	}
	return true;
}

void CMatrix3D::rotateX(RtFloat w)
{
        CMatrix3D r;

        w = deg2rad(w);

        RtFloat sinw = (RtFloat)sin(w);
        RtFloat cosw = (RtFloat)cos(w);
        r.m_Matrix[1][1] = cosw;
        r.m_Matrix[1][2] = sinw;
        r.m_Matrix[2][1] = -sinw;
        r.m_Matrix[2][2] = cosw;
        concatTransform(r.m_Matrix);
}


void CMatrix3D::rotateY(RtFloat w)
{
        CMatrix3D r;

        w = deg2rad(w);

        RtFloat sinw = (RtFloat)sin(w);
        RtFloat cosw = (RtFloat)cos(w);
        r.m_Matrix[0][0] = cosw;
        r.m_Matrix[0][2] = -sinw;
        r.m_Matrix[2][0] = sinw;
        r.m_Matrix[2][2] = cosw;
        concatTransform(r.m_Matrix);
}


void CMatrix3D::rotateZ(RtFloat w)
{
        CMatrix3D r;

        w = deg2rad(w);

        RtFloat sinw = (RtFloat)sin(w);
        RtFloat cosw = (RtFloat)cos(w);
        r.m_Matrix[0][0] = cosw;
        r.m_Matrix[0][1] = sinw;
        r.m_Matrix[1][0] = -sinw;
        r.m_Matrix[1][1] = cosw;
        concatTransform(r.m_Matrix);
}


void CMatrix3D::rotate(RtFloat w, RtFloat x, RtFloat y, RtFloat z)
{
        CMatrix3D r;

		if ( nearlyZero(w) ) { return; }

		
        if ( x > 0.0 && nearlyZero(y) && nearlyZero(z) ) { rotateX(w); return; }
        if ( x < 0.0 && nearlyZero(y) && nearlyZero(z) ) { rotateX(-w); return; }
        if ( nearlyZero(x) && y > 0.0 && nearlyZero(z) ) { rotateY(w); return; }
        if ( nearlyZero(x) && y < 0.0 && nearlyZero(z) ) { rotateY(-w); return; }
        if ( nearlyZero(x) && nearlyZero(y) && z > 0.0 ) { rotateZ(w); return; }
        if ( nearlyZero(x) && nearlyZero(y) && z < 0.0 ) { rotateZ(-w); return; }
		
        if ( nearlyZero(x) && nearlyZero(y) && nearlyZero(z) ) { return; }

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

void CMatrix3D::skew(
	RtFloat w,
	RtFloat x1, RtFloat y1, RtFloat z1,
	RtFloat x2, RtFloat y2, RtFloat z2)
{
        RtMatrix mat;
        RtFloat t;

        if ( nearlyZero(w) ) return; // undefined

		RtFloat length1 = sqrt(x1*x1 + y1*y1 + z1*z1);
        RtFloat length2 = sqrt(x2*x2 + y2*y2 + z2*z2);

        if ( nearlyZero(length1) || nearlyZero(length2) ) return; // undefined

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


void CMatrix3D::perspective(RtFloat fov)
{
        RtMatrix mat;

        if ( fov >= (RtFloat)180.0 ) {
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

void CMatrix3D::inversePerspective(RtFloat fov)
{
        RtMatrix mat;

        if ( fov >= (RtFloat)180.0 ) {
                // It's an error - so do nothing
                return;
        }

        RtFloat f = deg2rad(fov);
        f = (RtFloat)tan(f/2.0);
		if ( (RtFloat)0.0 == f ) {
			return;
		}

		f = (RtFloat)1.0/f;

        mat[0][0] = 1.0; mat[0][1] = 0.0; mat[0][2] = 0.0; mat[0][3] = 0.0;
        mat[1][0] = 0.0; mat[1][1] = 1.0; mat[1][2] = 0.0; mat[1][3] = 0.0;
        mat[2][0] = 0.0; mat[2][1] = 0.0; mat[2][2] = 0.0; mat[2][3] = -f;
        mat[3][0] = 0.0; mat[3][1] = 0.0; mat[3][2] = f;   mat[3][3] = f;

        concatTransform(mat);
}

//---------------------------------------------------------------------
/** @brief A utility function, used by determinant()
 * Calculates the determinant of a 2x2 matrix. Source copied from
 *  the Aqsis renderer.
 *  The matrix:
 @verbatim
 a b
 c d
 @endverbatim
 *  @param a Component of a 2x2 matrix
 *  @param b Component of a 2x2 matrix
 *  @param c Component of a 2x2 matrix
 *  @param d Component of a 2x2 matrix
 *  @return The determinant of the 2x2 matrix
 */

static RtFloat det2x2(RtFloat a, RtFloat b, RtFloat c, RtFloat d)
{
    return a * d - b * c;
}

//---------------------------------------------------------------------
/** @brief A utility function, used by determinant()
 * Calculates the determinant of a 3x3 matrix. Source copied from
 *  the Aqsis renderer
 *  The matrix is:
 @verbatim
 a1 a2 a3
 b1 b2 b3
 c1 c2 c3
 @endverbatim
 *  @param a1 Component of a 3x3 matrix
 *  @param a2 Component of a 3x3 matrix
 *  @param a3 Component of a 3x3 matrix
 *  @param b1 Component of a 3x3 matrix
 *  @param b2 Component of a 3x3 matrix
 *  @param b3 Component of a 3x3 matrix
 *  @param c1 Component of a 3x3 matrix
 *  @param c2 Component of a 3x3 matrix
 *  @param c3 Component of a 3x3 matrix
 *  @return The determinant of the 3x3 matrix
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

RtFloat CMatrix3D::determinant() const
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



bool CMatrix3D::isIdentity() const
{
        int i, j;
        bool eq = true;

        for ( i = 0; i < 4 ; ++i ) {
                for ( j = 0; j < 4 ; ++j ) {
                        eq = eq && ((m_Matrix[i][j] == 0.0) || ((i==j) && (m_Matrix[i][j] == 1.0)));
                }
        }

        return eq;
}


bool CMatrix3D::getInverse(RtMatrix &mat) const
{
        if ( isIdentity() )
        {
                get(mat);
                return true;

        }

        CMatrix3D b;                // b evolves from identity into inverse(a)
        CMatrix3D a(*this);        // a evolves from original matrix into identity

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

