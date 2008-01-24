#ifndef _RICPP_RICPP_TYPES_H
#define _RICPP_RICPP_TYPES_H

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

/** @file types.h
 *  @brief Declaration of some types, classes and data about types needed by the interface.
 *  @author Andreas Pidde (andreas@pidde.de)
 */
 
#ifndef _RICPP_RICPP_RICPPTYPES_H
#include "ricpp/ricpp/ricpptypes.h"
#endif // _RICPP_RICPP_RICPPTYPES_H

#ifndef _RICPP_TOOLS_INLINETOOLS_H
#include "ricpp/tools/inlinetools.h"
#endif // _RICPP_TOOLS_INLINETOOLS_H

#ifndef _RICPP_TOOLS_STRINGPATTERN_H
#include "ricpp/tools/stringpattern.h"
#endif // _RICPP_TOOLS_STRINGPATTERN_H

#include <cstddef>
#include <cmath>
#include <vector>
#include <sstream>

namespace RiCPP {

/** @brief Possible basic types. All types consists of these basic types.
 */
enum EnumBasicTypes {
	BASICTYPE_UNKNOWN = 0,//!< Unknown basic type
	BASICTYPE_INTEGER,    //!< Integer basic type
	BASICTYPE_FLOAT,      //!< Float basic type
	BASICTYPE_STRING      //!< String basic type
};

/** @brief Number of basic types
 */
const int N_BASICTYPES = (int)BASICTYPE_STRING+1;

/** @brief Possible types
 */
enum EnumTypes {
	TYPE_UNKNOWN       = 0, //!< Unknown type
	TYPE_FLOAT,				//!< Float
	TYPE_INTEGER,			//!< Integer
	TYPE_STRING,			//!< String (const char * pointer)
	TYPE_POINT,				//!< Point (three floats)
	TYPE_VECTOR,			//!< Vector (three floats)
	TYPE_NORMAL,			//!< Normal (three floats)
	TYPE_HPOINT,			//!< Homogene point (four floats)
	TYPE_MATRIX,			//!< Homogene matrix (16 floats)
	TYPE_COLOR				//!< Colour (Pointer to float)

/*
	// Not in parameter lists
	TYPE_MPOINT,			//!< Map point (16 floats)
	TYPE_BOOLEAN,			//!< Boolean
	TYPE_BASIS,				//!< Base matrix (16 floats)
	TYPE_BOUND,				//!< Bounding box (6 floats)
	TYPE_TOKEN,				//!< Token
	TYPE_POINTER,			//!< Arbitrary pointer
	TYPE_VOID,				//!< Void type
	TYPE_FILTERFUNC,		//!< Pointer to a filter function RtFilterFunc
	TYPE_ERRORHANDLER,		//!< Pointer to an errorhandler RtErrorHandler
	TYPE_PROCSUBDIVFUNC,	//!< Pointer to a subdivision procedure RtProcSubdivFunc
	TYPE_PROCFREEFUNC,		//!< Pointer to a cleanup function RtProcFreeFunc
	TYPE_ARCHIVECALLBACK,	//!< Pointer to an archive callback function RtArchiveCallback
	TYPE_OBJECTHANDLE,		//!< Handle of an object RtObjectHandle
	TYPE_LIGHTHANDLE,		//!< Handle of a light source RtLightSource
	TYPE_CONTEXTHANDLE		//!< Handle of a renderer context RtContextHandle
*/
};

/** @brief Number of different types
 */
const int N_TYPES  = (int)TYPE_COLOR+1;

/** @brief Possible parameter storage classes
 */
enum EnumClasses {
	CLASS_UNKNOWN	 = 0, //!< Unknown class
	CLASS_CONSTANT,	      //!< Constant class (one value)
	CLASS_UNIFORM,	      //!< Constant class (one value per face, shared corners count one)
	CLASS_VARYING,	      //!< Varying class (one value per corner, shared vertex count one)
	CLASS_VERTEX,	      //!< Vertex class (one value per vertex)
	CLASS_FACEVARYING,	  //!< Vertex class (one value per corner of each face)
	CLASS_FACEVERTEX	  //!< Face Vertex class (one value per vertex of each face)
};

/** @brief Number of different classes
 */
const int N_CLASSES = (int)CLASS_FACEVERTEX+1;

/** @brief Possible table namespaces for declarations
 */
enum EnumNamespaces {
	NAMESPACE_UNKNOWN = 0,
	NAMESPACE_PROJECTION,
	NAMESPACE_IMAGER,
	NAMESPACE_DISPLAY,
	NAMESPACE_HIDER,
	NAMESPACE_OPTION,
	NAMESPACE_LIGHT,
	NAMESPACE_AREALIGHT,
	NAMESPACE_SURFACE,
	NAMESPACE_ATMOSPHERE,
	NAMESPACE_INTERIOR,
	NAMESPACE_EXTERIOR,
	NAMESPACE_DISPLACEMENT,
	NAMESPACE_DEFORMATION,
	NAMESPACE_VOLUME,
	NAMESPACE_GEOMETRICAPPROXYMATION,
	NAMESPACE_COORDINATESYSTEM,
	NAMESPACE_ATTRIBUTE,
	NAMESPACE_GEOMETRY,
	NAMESPACE_TEXTURE,
	NAMESPACE_READARCHIVE,
	NAMESPACE_ARCHIVE,
	NAMESPACE_DISPLAYCHANNEL,
	NAMESPACE_RESOURCE
};

/** @brief Number of basic types
 */
const int N_NAMESPACES = (int)NAMESPACE_RESOURCE+1;

/** @brief Class to query info about Ri-types that can occur in declarations.
 */
class CTypeInfo {
	static const unsigned int ms_basicTypeSizes[N_BASICTYPES]; ///< Number of bytes for the basic types
	static RtToken ms_basicTypeNames[N_BASICTYPES]; ///< Names of basic types
	static const unsigned int ms_typeSizes[N_TYPES+1]; ///< Sizes of the types (+1 to have the same size as m_typeNames, but not needed)
	static RtToken ms_typeNames[N_TYPES+1]; ///< Type names (+1 to recognize 'int', that is internally converted to 'integer' in typePrefix())
	static RtToken ms_classNames[N_CLASSES]; ///< Storage class names
	static const unsigned int ms_typeByteSizes[N_TYPES+1]; ///< Number of bytes for the types
	static const EnumBasicTypes ms_basicTypesForTypes[N_TYPES+1]; ///< Basic types the types consists of.
	static RtToken ms_namespaces[N_NAMESPACES]; ///< Table namespaces

	/** @brief Compares prefixes
	 * Compares if @a token is a prefix of @a search. Behind the
	 * prefix a blank, [ or : must be there as a seperator.
	 * @param token Prefix to search
	 * @param search token is tested to be a prefix of search
	 * @return Like in strcmp():
	 * - -1 if token (prefix) < search,
	 * -  0 if token is a prefix of search,
	 * -  1 token (prefix) > search 
	 */
	static int tokcmp(const char *token, const char *search);

public:
	/** @brief Gets the name of a basic type
	 * @param e Ri basic type
	 * @return The name (like in RIB or declarations) of a basic type
	 * @see EnumBasicTypes
	 */
	static RtToken basicTypeName(EnumBasicTypes e);

	/** @brief Gets the size of a basic types in bytes
	 * @param e Ri basic type
	 * @return The size of a basic types in bytes
	 * @see EnumBasicTypes
	 */
	static unsigned int basicTypeByteSize(EnumBasicTypes e);

	/** @brief Gets the name of a type that can occur in declarations and RIB files
	 * @param e Ri type
	 * @return The name (like in RIB or declarations) of a type
	 * @see EnumTypes
	 */
	static RtToken typeName(EnumTypes e);

	/** @brief Gets the size of a types in elements
	 *
	 * TYPE_POINT returns 3 (for Three floats) for example, TYPE_STRING is one.
	 * TYPE_COLOR has the size of three here, because the status of the renderer with
	 * the current color size is unknown, needs special handling by the caller.
	 *
	 * @param e Ri type
	 * @return The size of a type in elements (color size is returned as 3)
	 * @see EnumTypes
	 */
	static unsigned int typeSize(EnumTypes e);

	/** @brief Gets the size of a types in bytes
	 * @param e Ri type
	 * @return The size of a types in bytes, color needs special handling
	 * @see EnumTypes, typeSize(EnumTypes e)
	 */
	static unsigned int typeByteSize(EnumTypes e);

	/** @brief Gets the basic type a type consists of
	 * @param e Ri type
	 * @return The basic type of a type
	 */
	static EnumBasicTypes basicTypeForType(EnumTypes e);

	/** @brief Gets the name of the storage class
	 * @param e Ri storage class
	 * @return The name of a storage class as used in declarations and RIB
	 */
	static RtToken className(EnumClasses e);

	/** @brief Gets the name of the table namespace [QRM]
	 * @param e (Q)Ri table namespace
	 * @return The name of a table namespace as used in declarations and RIB
	 */
	static RtToken tableNamespace(EnumNamespaces e);

	/** @brief Tries to find a class name as prefix of aclass.
	 * @param aclass string possibly having class name as prefix
	 * @retval pos if found position right behind the prefix
	 * @return CLASS_UNKNOWN if no prefix found, matching EnumClasses if found.
	 */
	static EnumClasses classPrefix(const char *aclass, size_t &pos);

	/** @brief Tries to find a type name as prefix of atype.
	 * @param atype string possibly having type name as prefix
	 * @retval pos if found position right behind the prefix
	 * @return TYPE_UNKNOWN if no prefix found, matching EnumTypes if found.
	 */
	static EnumTypes typePrefix(const char *atype, size_t &pos);

	/** @brief Tries to find an array specifier [n] as prefix of atype.
	 * @param acard string possibly having an array specifier as prefix
	 * @retval pos if found position right behind the prefix
	 * @return The size of the array
	 *  -  < 0, syntax error or [0]
	 *  - == 0, no cardinality found
	 *  -  > 0, [n] found, returns the n
	 */
	static bool arrayPrefix(const char *acard, size_t &pos, unsigned long &arraySize);

	/** @brief Tries to find a namespace+':' as prefix of avar.
	 * @param avar string possibly having namespace+':' as prefix
	 * @retval pos if found position right behind the prefix
	 * @return NAMESPACE_UNKNOWN if no prefix found, matching EnumNamespaces if found.
	 */
	static EnumNamespaces namespacePrefix(const char *avar, size_t &pos);
}; // CTypeInfo


/** @brief Color descriptor.
 *
 * An instance of this calss is used to store the declaration of the
 * color type (i.e. option CRi::colorSamples()). You can use this instance
 * to convert a color to it's RGB triple (nRGB) and vice versa (RGBn) if
 * your renderer cannot handle colors with arbitrary samples.
 * 
 */
class CColorDescr {
private:
	RtInt m_nColorSamples;       ///< @brief The number of color samples (>0).
	std::vector<RtFloat> m_nRGB; ///< @brief n times 3 matrix to transform a color into it's RGB values.
	std::vector<RtFloat> m_RGBn; ///< @brief 3 times n matrix to transform a RGB value to a color
	bool m_isIdentity;           ///< @brief true, if no color transformation needed (3 samples, transformation matrix is identity).

	/** @brief Test color transformation for identity.
	 *
	 * Test if the number of color samples (m_nColorSamples) is 3 and m_nRGB and m_RGBn
	 * are identity matrices. If so, the color components can be viewed as RGB components.
	 *
	 * @return True, no color transformation needed (transformation is the identity).
	 */
	bool testident();
public:
	/** @brief Constructor, sets default identity transformation.
	 */
	CColorDescr();

	/** @brief Constructor, sets custom transformation.
	 *
	 * @param nColorSamples Number of color samples (>0)
	 * @param nRGB Transformation matrix from a color to a RGB value
	 * @param RGBn Transformation matrix from a RGB value to a color
	 */
	inline CColorDescr(RtInt nColorSamples, RtFloat nRGB[], RtFloat RGBn[])
	{
		colorSamples(nColorSamples, nRGB, RGBn);
	}
	
	/** @brief Copy constructor.
	 *
	 * @param cd Color descriptor to assign to *this.
	 */
	inline CColorDescr(const CColorDescr &cd)
	{
		*this = cd;
	}

	/** @brief Assignment.
	 *
	 * @param cd Color descriptor to assign to *this.
	 * @return *this
	 */
	CColorDescr &operator=(const CColorDescr &cd);

	/** @brief Compare.
	 *
	 * @param cd Color descriptor to compare *this.
	 * @return true, equal descriptors
	 */
	bool operator==(const CColorDescr &cd) const;

	/** @brief Compare not equal.
	 *
	 * @param cd Color descriptor to compare *this.
	 * @return true, not equal descriptors
	 */
	inline bool operator!=(const CColorDescr &cd) const
	{
		return !(operator==(cd));
	}

	/** @brief Transforms a single color value to it's RGB value.
	 *
	 * @param  c   m_nColorSamples color samples of a single color value.
	 * @retval rgb RGB triple calculated by color transformation.
	 */
	RtVoid nToRGB(RtColor c, RtFloat rgb[3]) const;

	/** @brief Transforms @a n color values to their RGB values.
	 *
	 * @param n number of color values.
	 * @param  c   @a n times m_nColorSamples color samples of a @a n color values.
	 * @retval rgb @a n RGB triples calculated by color transformation.
	 */
	RtVoid nToRGB(RtInt n, RtFloat *c, RtFloat rgb[][3]) const;

	/** @brief Transforms a single RGB triple to a color value.
	 *
	 * @param  rgb RGB triple calculated by color transformation.
	 * @retval c   m_nColorSamples color samples of a single color value.
	 */
	RtVoid RGBToN(RtFloat rgb[3], RtColor c) const;

	/** @brief Transforms @a n RGB triples to the color values.
	 *
	 * @param n number of RGB triples.
	 * @param  rgb @a n RGB triples.
	 * @retval c   m_nColorSamples time @a n color samples for the @a n color values.
	 */
	RtVoid RGBToN(RtInt n, RtFloat rgb[][3], RtFloat c[]) const;

	/** @brief Sets the number color samples and transformation matrices.
	 *
	 * If @a nColorSamples is less or equal 0, the descriptor is set to identity.
	 * If one of the matrices is null a matrix with a diagonally 1 is taken (i.e.
	 * id matrix if 3x3).
	 *
	 * @param nColorSamples The number of color samples (>0).
	 * @param nRGB Matrix color to RGB (should not be 0).
	 * @param RGBn Matrix RGB to color (should not be 0).
	 */
	RtVoid colorSamples(RtInt nColorSamples, RtFloat nRGB[], RtFloat RGBn[]);

	/** @brief Gets the number color samples.
	 *
	 * @return The number of color samples.
	 */
	inline RtInt colorSamples() const
	{
		return m_nColorSamples;
	}

	/** @brief Gets the number color samples and transformation matrices.
	 *
	 * @retval nColorSamples The number of color samples.
	 * @retval nRGB Matrix color to RGB.
	 * @retval RGBn Matrix RGB to color.
	 */
	inline RtVoid getColorSamples(RtInt nColorSamples, std::vector<RtFloat> &nRGB, std::vector<RtFloat> &RGBn) const
	{
		nColorSamples = m_nColorSamples;
		nRGB = m_nRGB;
		RGBn = m_RGBn;
	}

	/** @brief Gets the nRGB matrix (read-only).
	 *
	 *  @return Read-only nRGB matrix.
	 */
	const std::vector<RtFloat> &nRGBMatrix() const
	{
		return m_nRGB;
	}

	/** @brief Gets the nRGB matrix.
	 *
	 *  @return nRGB matrix.
	 */
	std::vector<RtFloat> &nRGBMatrix()
	{
		return m_nRGB;
	}

	/** @brief Gets the RGBn matrix (read-only).
	 *
	 *  @return Read-only RGBn matrix.
	 */
	const std::vector<RtFloat> &RGBnMatrix() const
	{
		return m_RGBn;
	}

	/** @brief Gets the RGBn matrix
	 *
	 *  @return RGBn matrix.
	 */
	std::vector<RtFloat> &RGBnMatrix()
	{
		return m_RGBn;
	}
}; // CColorDescr


/** @brief Additional clipping plane.
 */
class CClippingPlane {
	RtFloat m_x;
	RtFloat m_y;
	RtFloat m_z;
	RtFloat m_nx;
	RtFloat m_ny;
	RtFloat m_nz;
public:

	CClippingPlane();
	CClippingPlane(RtFloat aX, RtFloat aY, RtFloat aZ, RtFloat aNX, RtFloat aNY, RtFloat aNZ);
	inline CClippingPlane(const CClippingPlane &p)
	{
		*this = p;
	}

	CClippingPlane &operator=(const CClippingPlane &p);

	void set(RtFloat aX, RtFloat aY, RtFloat aZ, RtFloat aNX, RtFloat aNY, RtFloat aNZ);

	inline RtVoid x(RtFloat aX)
	{
		m_x = aX;
	}
	inline RtFloat x() const
	{
		return m_x;
	}

	inline RtVoid y(RtFloat aY)
	{
		m_y = aY;
	}
	inline RtFloat y() const
	{
		return m_y;
	}

	inline RtVoid z(RtFloat aZ)
	{
		m_z = aZ;
	}
	inline RtFloat z() const
	{
		return m_z;
	}

	inline RtVoid nx(RtFloat aNX)
	{
		m_nx = aNX;
	}
	inline RtFloat nx() const
	{
		return m_nx;
	}

	inline RtVoid ny(RtFloat aNY)
	{
		m_ny = aNY;
	}
	inline RtFloat ny() const
	{
		return m_ny;
	}

	inline RtVoid nz(RtFloat aNZ)
	{
		m_nz = aNZ;
	}
	inline RtFloat nz() const
	{
		return m_nz;
	}
}; // CClippingPlane


/** @brief Quantizer.
 */
class CQuantizer {
	RtToken m_type;
	RtInt m_one, m_min,  m_max;
	RtFloat m_ditherAmplitude;
public:
	CQuantizer();
	CQuantizer(RtToken atype, RtInt aOne, RtInt aMin, RtInt aMax, RtFloat aDitherAmplitude);
	inline CQuantizer(const CQuantizer &quantizer)
	{
		*this = quantizer;
	}
	CQuantizer &operator=(const CQuantizer &q);

	void set(RtToken atype, RtInt aOne, RtInt aMin, RtInt aMax, RtFloat aDitherAmplitude);

	inline RtVoid type(RtToken aType)
	{
		m_type = aType;
	}
	inline RtToken type() const
	{
		return m_type;
	}

	inline RtVoid one(RtInt aOne)
	{
		m_one = aOne;
	}
	inline RtInt one() const
	{
		return m_one;
	}

	inline RtVoid minVal(RtInt aMin)
	{
		m_min = aMin;
	}
	inline RtInt minVal() const
	{
		return m_min;
	}

	inline RtVoid maxVal(RtInt aMax)
	{
		m_max = aMax;
	}
	inline RtInt maxVal() const
	{
		return m_max;
	}

	inline RtVoid ditherAmplitude(RtFloat aDitherAmplitude)
	{
		m_ditherAmplitude = aDitherAmplitude;
	}
	inline RtFloat ditherAmplitude() const
	{
		return m_ditherAmplitude;
	}

	RtFloat quantize(RtFloat value) const;
	void quantize(std::vector<RtFloat> &values) const;
}; // CQuantizer


/** @brief Array sizes of a trim curve
 */
struct CTrimCurveDataInfo {
	RtInt m_nloops; //!< Number of closed loops.

	int m_total;	//!< Total number of curves.
	int m_npoints;	//!< Total number of control points.
	int m_nknots;	//!< Total number of knots.

	/** @brief Standard constructor, initializes with 0 loops.
	 */
	inline CTrimCurveDataInfo() {
		m_nloops = 0;
		m_total = 0;
		m_npoints = 0;
		m_nknots = 0;
	}

	/** @brief Constructor, fills instances with the values of a trim curve interface call.
	 *
	 *  @param nloops   Number if loops (curves with holes).
	 *  @param ncurves  Curves of the loops.
	 *  @param order    Order of the curves.
	 *  @param knot     Knot vectors of the curves.
	 *  @param amin     Minimum parametric values of the curves.
	 *  @param amax     Maximum parametric values of the curves.
	 *  @param n        Number of the homogene u,v,w coordinates.
	 *  @param u        u coordinates of the curves.
	 *  @param v        v coordinates of the curves.
	 *  @param w        w coordinates of the curves.
	 */
	inline CTrimCurveDataInfo(
		RtInt nloops, const RtInt *ncurves,
		const RtInt *order, const RtFloat *knot,
		const RtFloat *amin, const RtFloat *amax,
		const RtInt *n, const RtFloat *u, const RtFloat *v, const RtFloat *w)
	{
		trimCurve(nloops, ncurves, order, knot, amin, amax, n, u, v, w);
	}

	/** @brief Fills instances with the values of a trim curve interface call.
	 *
	 *  @param nloops   Number if loops (curves with holes).
	 *  @param ncurves  Curves of the loops.
	 *  @param order    Order of the curves.
	 *  @param knot     Knot vectors of the curves.
	 *  @param amin     Minimum parameter of a curve.
	 *  @param amax     Maximum parameter of a curve.
	 *  @param n        Number of the homogene u,v,w coordinates.
	 *  @param u        u coordinates of the curves.
	 *  @param v        v coordinates of the curves.
	 *  @param w        w coordinates of the curves.
	 */
	void trimCurve(RtInt nloops, const RtInt ncurves[], const RtInt order[], const RtFloat knot[], const RtFloat amin[], const RtFloat amax[],
				   const RtInt n[], const RtFloat u[], const RtFloat v[], const RtFloat w[]);
};

/** @brief Contains one trim curve.
 */
struct CTrimCurveData {
	CTrimCurveDataInfo   m_data;    //!< Sizes
	
	std::vector<RtInt>   m_nCurves; //!< Number if loops (curves with holes).
	std::vector<RtInt>   m_order;   //!< Order of the curves.
	std::vector<RtFloat> m_knots;   //!< Knot vectors of the curves.
	std::vector<RtFloat> m_min;     //!< Minimum parametric values of the curves.
	std::vector<RtFloat> m_max;     //!< Maximum parametric values of the curves.
	std::vector<RtInt>   m_n;       //!< Number of coordinates.
	std::vector<RtFloat> m_u;       //!< u coordinates of the curves.
	std::vector<RtFloat> m_v;       //!< v coordinates of the curves.
	std::vector<RtFloat> m_w;       //!< w coordinates of the curves.
	std::vector<RtFloat> m_points;  //!< Points filled with (u[0], v[0], w[0], ... ).

	/////////////////////////////////////////////////////////////////////////////////////////
	// Member functions

	/** @brief Standard constructor.
	 */
	inline CTrimCurveData() {}

	/** @brief Constructor, fills instances with the values of a trim curve interface call.
	 *
	 *  @param nloops   Number if loops (curves with holes).
	 *  @param ncurves  Curves of the loops.
	 *  @param order    Order of the curves.
	 *  @param knot     Knot vectors of the curves.
	 *  @param amin     Minimum parametric values of the curves.
	 *  @param amax     Maximum parametric values of the curves.
	 *  @param n        Number of the homogene u,v,w coordinates.
	 *  @param u        u coordinates of the curves.
	 *  @param v        v coordinates of the curves.
	 *  @param w        w coordinates of the curves.
	 */
	inline CTrimCurveData(
		RtInt nloops, const RtInt ncurves[],
		const RtInt order[], const RtFloat knot[],
		const RtFloat amin[], const RtFloat amax[],
		const RtInt n[], const RtFloat u[], const RtFloat v[], const RtFloat w[])
	{
		trimCurve(nloops, ncurves, order, knot, amin, amax, n, u, v, w);
	}

	/** @brief Copy constructor.
	 *
	 *  @param curve The curve to assign from.
	 */
	CTrimCurveData(const CTrimCurveData &curve)
	{
		*this = curve;
	}

	/** @brief Destructor.
	 */
	~CTrimCurveData()
	{
		releaseAll();
	}

	/** @brief Assignment of another curve.
	 *
	 *  @param curve The curve to assign.
	 *  @return *this.
	 */
	inline CTrimCurveData &operator=(const CTrimCurveData &curve)
	{
		if ( this == &curve )
			return *this;
		if ( curve.m_data.m_nloops > 0 )
			trimCurve(
				curve.m_data.m_nloops,
				curve.m_data.m_nloops == 0 || curve.m_nCurves.empty() ? 0 : &curve.m_nCurves[0],
				curve.m_data.m_nloops == 0 || curve.m_order.empty() ? 0 : &curve.m_order[0],
				curve.m_data.m_nloops == 0 || curve.m_knots.empty() ? 0 : &curve.m_knots[0],
				curve.m_data.m_nloops == 0 || curve.m_min.empty() ? 0 : &curve.m_min[0],
				curve.m_data.m_nloops == 0 || curve.m_max.empty() ? 0 : &curve.m_max[0],
				curve.m_data.m_nloops == 0 || curve.m_n.empty() ? 0 : &curve.m_n[0],
				curve.m_data.m_nloops == 0 || curve.m_u.empty() ? 0 : &curve.m_u[0],
				curve.m_data.m_nloops == 0 || curve.m_v.empty() ? 0 : &curve.m_v[0],
				curve.m_data.m_nloops == 0 || curve.m_w.empty() ? 0 : &curve.m_w[0]
			);
		else {
			releaseAll();
		}
		return *this;
	}

	inline bool operator==(const CTrimCurveData &curve)
	{
		if ( m_data.m_nloops != curve.m_data.m_nloops ) return false;
		if ( m_nCurves != curve.m_nCurves ) return false;
		if ( m_order != curve.m_order )  return false;
		if ( m_knots != curve.m_knots )  return false;
		if ( m_min != curve.m_min )  return false;
		if ( m_max != curve.m_max )  return false;
		if ( m_n != curve.m_n )  return false;
		if ( m_u != curve.m_u )  return false;
		if ( m_v != curve.m_v )  return false;
		if ( m_w != curve.m_w )  return false;
		if ( m_points != curve.m_points )  return false;
		return true;
	}

	/** @brief Clears instance.
	 *
	 *  Zero loops menas, there is no trim curve.
	 */
	inline void releaseAll()
	{
		m_data.m_nloops = 0;
	}

	/** @brief Fills instances with the values of a trim curve interface call.
	 *
	 *  @param nloops   Number if loops (curves with holes).
	 *  @param ncurves  Curves of the loops.
	 *  @param order    Order of the curves.
	 *  @param knot     Knot vectors of the curves.
	 *  @param amin     Minimum parameter of a curve.
	 *  @param amax     Maximum parameter of a curve.
	 *  @param n        Number of the homogene u,v,w coordinates.
	 *  @param u        u coordinates of the curves.
	 *  @param v        v coordinates of the curves.
	 *  @param w        w coordinates of the curves.
	 */
	void trimCurve(RtInt nloops, const RtInt ncurves[], const RtInt order[], const RtFloat knot[], const RtFloat amin[], const RtFloat amax[],
				   const RtInt n[], const RtFloat u[], const RtFloat v[], const RtFloat w[]);
}; // 


/** @brief A class for a 3D homogene matrix (4x4).
 *
 *  The class implements all matrix operations needed by the implementation of the RenderMan interface.
 */
class CMatrix3D {
	/** @brief The value of the 4x4 matrix, init with identity.
	 *
	 *  Index [i][j]:\n
	 *  [0][0] [0][1] [0][2] [0][3]\n
	 *  [1][0] [1][1] [1][2] [1][3]\n
	 *  [2][0] [2][1] [2][2] [2][3]\n
	 *  [3][0] [3][1] [3][2] [3][3]\n
	 */
	RtMatrix m_Matrix;

	/** @brief true (default), if premultiply when concatenate matrices (e.g. by transform()).
	 */
	bool m_preMultiply;

	// void extract(RtFloat *result, const RtFloat *mat, int i, int size) const;
	// RtFloat det(const RtFloat *mat, int size) const;
public:
	CMatrix3D();
	CMatrix3D(const CMatrix3D &mat);
	CMatrix3D(RtMatrix mat);
	~CMatrix3D();

	// CMatrix3D &duplicate() const;
	CMatrix3D &operator=(const CMatrix3D &mat);
	CMatrix3D &operator=(RtMatrix mat);

	operator const RtFloat *() const { return (const RtFloat *)&m_Matrix[0]; }
	// RtFloat *getPointer() const { return (RtFloat *)&m_Matrix[0]; }

	inline void setPreMultiply(bool preMultiply=true) {
		m_preMultiply = preMultiply;
	}
	inline bool getPreMultiply() const {
		return m_preMultiply;
	}

	inline void set(const RtMatrix mat) {
		memcpy(m_Matrix, mat, 16*sizeof(RtFloat));
	}
	inline void set(const RtFloat *mat) {
		memcpy(m_Matrix, mat, 16*sizeof(RtFloat));
	}
	void get(RtMatrix &mat) const;

	bool operator==(const CMatrix3D &mat) const;
	bool operator==(RtMatrix mat) const;
	bool operator!=(const CMatrix3D &mat) const;
	bool operator!=(RtMatrix mat) const;

	void clear();
	void identity();
	void transpose();
	void translate(RtFloat dx, RtFloat dy, RtFloat dz);
	void scale(RtFloat sx, RtFloat sy, RtFloat sz);

	void transform(RtMatrix mat);
	void transform(CMatrix3D &mat);

	void postMultiply(RtMatrix mat);
	void postMultiply(CMatrix3D mat);
	void preMultiply(RtMatrix mat);
	void preMultiply(CMatrix3D mat);

	void concatTransform(RtMatrix mat);
	void concatTransform(CMatrix3D mat);

	void transformPoints(RtFloat &x, RtFloat &y, RtFloat &z);

	void rotateX(RtFloat w);
	void rotateY(RtFloat w);
	void rotateZ(RtFloat w);
	void rotate(RtFloat w, RtFloat x, RtFloat y, RtFloat z);

	void skew(RtFloat w, RtFloat x1, RtFloat y1, RtFloat z1, RtFloat x2, RtFloat y2, RtFloat z2);

	void perspective(RtFloat fov);

	RtFloat determinant() const;
	bool isIdentity() const;
	bool getInverse(RtMatrix &mat) const;
}; // CMatrix3D


class CValue
{
private:
	RtInt m_intVal;
	RtFloat m_floatVal;
	std::string m_stringVal;
	EnumBasicTypes m_type;
public:
	inline void set(RtInt anInt) { m_type = BASICTYPE_INTEGER; m_intVal = anInt; }
	inline void set(RtFloat aFloat) { m_type = BASICTYPE_FLOAT; m_floatVal = aFloat; }
	inline void set(RtString aString) { m_type = BASICTYPE_STRING; m_stringVal = noNullStr(aString); }
	inline void clear() { m_type = BASICTYPE_UNKNOWN; }

	inline CValue() { clear(); }
	inline CValue(RtInt anInt) { set(anInt); }
	inline CValue(RtFloat aFloat) { set(aFloat); }
	inline CValue(RtString aString) { set(aString); }
	inline CValue(const CValue &val) { *this = val; }

	inline EnumBasicTypes type() const { return m_type; }

	inline CValue &operator=(const CValue &val)
	{
		if ( this == &val )
			return *this;
		m_intVal = val.m_intVal;
		m_floatVal = val.m_floatVal;
		m_stringVal = val.m_stringVal;
		m_type = val.m_type;
		return *this;
	}

	inline bool get(RtInt &anInt) const
	{
		if ( m_type == BASICTYPE_INTEGER ) {
			anInt = m_intVal;
			return true;
		}
		if ( m_type == BASICTYPE_FLOAT ) {
			anInt = (RtInt)m_floatVal;
			return true;
		}
		if ( m_type == BASICTYPE_STRING ) {
			anInt = (RtInt)atol(m_stringVal.c_str());
			return true;
		}
		return false;
	}

	inline bool get(RtFloat &aFloat) const
	{
		if ( m_type == BASICTYPE_INTEGER ) {
			aFloat = (RtFloat)m_intVal;
			return true;
		}
		if ( m_type == BASICTYPE_FLOAT ) {
			aFloat = m_floatVal;
			return true;
		}
		if ( m_type == BASICTYPE_STRING ) {
			aFloat = (RtFloat)atof(m_stringVal.c_str());
			return true;
		}
		return false;
	}

	inline bool get(std::string &aString) const
	{
		if ( m_type == BASICTYPE_INTEGER ) {
			std::stringstream stst(aString);
			stst << m_intVal;
			return true;
		}
		if ( m_type == BASICTYPE_FLOAT ) {
			std::stringstream stst(aString);
			stst << m_floatVal;
			return true;
		}
		if ( m_type == BASICTYPE_STRING ) {
			aString = m_stringVal;
			return true;
		}
		return false;
	}

	inline void append(const std::string &aString)
	{
		if ( m_type != BASICTYPE_STRING ) {
			m_stringVal = "";
			get(m_stringVal);
			m_type = BASICTYPE_STRING;
		}

		m_stringVal += aString;
	}

	inline CValue &operator^=(const CValue &op)
	{
		RtInt i1=0, i2=0;
		get(i1);
		op.get(i2);

		set(i1^i2);
		return *this;
	}

	inline CValue &operator|=(const CValue &op)
	{
		RtInt i1=0, i2=0;
		get(i1);
		op.get(i2);

		set(i1^i2);
		return *this;
	}

	inline CValue &operator&=(const CValue &op)
	{
		RtInt i1=0, i2=0;
		get(i1);
		op.get(i2);

		set(i1&i2);
		return *this;
	}

	inline CValue &operator+=(const CValue &op)
	{
		if ( m_type == BASICTYPE_INTEGER && op.type() == BASICTYPE_INTEGER ) {
			RtInt i1=0, i2=0;
			get(i1);
			op.get(i2);
			set(i1+i2);
		} else {
			RtFloat f1=0, f2=0;
			get(f1);
			op.get(f2);

			set(f1+f2);
		}
		return *this;
	}

	inline CValue &operator-=(const CValue &op)
	{
		if ( m_type == BASICTYPE_INTEGER && op.type() == BASICTYPE_INTEGER ) {
			RtInt i1=0, i2=0;
			get(i1);
			op.get(i2);
			set(i1-i2);
		} else {
			RtFloat f1=0, f2=0;
			get(f1);
			op.get(f2);

			set(f1-f2);
		}
		return *this;
	}

	inline CValue &operator*=(const CValue &op)
	{
		if ( m_type == BASICTYPE_INTEGER && op.type() == BASICTYPE_INTEGER ) {
			RtInt i1=0, i2=0;
			get(i1);
			op.get(i2);
			set(i1*i2);
		} else {
			RtFloat f1=0, f2=0;
			get(f1);
			op.get(f2);

			set(f1*f2);
		}
		return *this;
	}

	inline CValue &operator/=(const CValue &op)
	{
		if ( m_type == BASICTYPE_INTEGER && op.type() == BASICTYPE_INTEGER ) {
			RtInt i1=0, i2=0;
			get(i1);
			op.get(i2);
			set(i1/i2);
		} else {
			RtFloat f1=0, f2=0;
			get(f1);
			op.get(f2);

			set(f1/f2);
		}
		return *this;
	}

	inline CValue &powBy(const CValue &op)
	{
		RtFloat f1=0;
		get(f1);

		if ( op.type() == BASICTYPE_INTEGER ) {
			RtInt i2 = 0;
			op.get(i2);
			set((RtFloat)pow(f1,i2));
		} else {
			RtFloat f2 = 0;
			op.get(f2);
			set((RtFloat)pow(f1,f2));
		}

		return *this;
	}

	inline CValue &setNegative()
	{
		if ( m_type == BASICTYPE_INTEGER ) {
			RtInt v = 0;
			get(v);
			set(-v);
		} else if ( m_type == BASICTYPE_FLOAT ) {
			RtFloat v = 0;
			get(v);
			set(-v);
		} else if ( m_type == BASICTYPE_STRING ) {
			std::string v;
			get(v);
			if ( !v.empty() ) {
				RtFloat v;
				get(v);
				set(-v);
			}
		}

		return *this;
	}


	inline bool matchedBy(const char *pattern) const
	{
		CStringPattern pat(pattern);
		std::string s;
		get(s);
		return pat.matches(s.c_str());
	}
}; // CValue

inline CValue operator||(const CValue &op1, const CValue &op2)
{
	RtInt i1=0, i2=0;
	op1.get(i1);
	op2.get(i2);

	return CValue((i1||i2)?1:0);
}

inline CValue operator&&(const CValue &op1, const CValue &op2)
{
	RtInt i1=0, i2=0;
	op1.get(i1);
	op2.get(i2);

	return CValue((i1&&i2)?1:0);
}

inline CValue operator^(const CValue &op1, const CValue &op2)
{
	RtInt i1=0, i2=0;
	op1.get(i1);
	op2.get(i2);

	return CValue(i1^i2);
}

inline CValue operator|(const CValue &op1, const CValue &op2)
{
	RtInt i1=0, i2=0;
	op1.get(i1);
	op2.get(i2);

	return CValue(i1|i2);
}

inline CValue operator&(const CValue &op1, const CValue &op2)
{
	RtInt i1=0, i2=0;
	op1.get(i1);
	op2.get(i2);

	return CValue(i1&i2);
}

inline CValue operator!(const CValue &op1)
{
	RtInt i1=0;
	op1.get(i1);

	return CValue(i1?0:1);
}

inline CValue operator==(const CValue &op1, const CValue &op2)
{
	switch(op1.type()) {
		case BASICTYPE_INTEGER:
			switch(op1.type()) {
				case BASICTYPE_INTEGER:
				{
					RtInt i1=0, i2=0;
					op1.get(i1);
					op2.get(i2);
					return i1 == i2;
				}
				case BASICTYPE_FLOAT:
				case BASICTYPE_STRING:
				{
					RtFloat f1=0, f2=0;
					op1.get(f1);
					op2.get(f2);
					return f1 == f2;
				}
				case BASICTYPE_UNKNOWN:
					return false;
			}
			return false;
			break;
		case BASICTYPE_FLOAT:
			switch(op1.type()) {
				case BASICTYPE_INTEGER:
				case BASICTYPE_FLOAT:
				case BASICTYPE_STRING:
				{
					RtFloat f1=0, f2=0;
					op1.get(f1);
					op2.get(f2);
					return f1 == f2;
				}
				case BASICTYPE_UNKNOWN:
					return false;
			}
			return false;
			break;
		case BASICTYPE_STRING:
			switch(op1.type()) {
				case BASICTYPE_INTEGER:
				case BASICTYPE_FLOAT:
				{
					RtFloat f1=0, f2=0;
					op1.get(f1);
					op2.get(f2);
					return f1 == f2;
				}
				case BASICTYPE_STRING:
				{
					std::string s1, s2;
					op1.get(s1);
					op2.get(s2);
					return s1 == s2;
				}
				case BASICTYPE_UNKNOWN:
					return false;
			}
			return false;
		case BASICTYPE_UNKNOWN:
			return op2.type() == BASICTYPE_UNKNOWN;
	}
	return false;
}

inline CValue operator!=(const CValue &op1, const CValue &op2)
{
	return !(op1 == op2);
}

inline CValue operator>(const CValue &op1, const CValue &op2)
{
	switch(op1.type()) {
		case BASICTYPE_INTEGER:
			switch(op1.type()) {
				case BASICTYPE_INTEGER:
				{
					RtInt i1=0, i2=0;
					op1.get(i1);
					op2.get(i2);
					return i1 > i2;
				}
				case BASICTYPE_FLOAT:
				case BASICTYPE_STRING:
				{
					RtFloat f1=0, f2=0;
					op1.get(f1);
					op2.get(f2);
					return f1 > f2;
				}
				case BASICTYPE_UNKNOWN:
					return false;
			}
			return false;
			break;
		case BASICTYPE_FLOAT:
			switch(op1.type()) {
				case BASICTYPE_INTEGER:
				case BASICTYPE_FLOAT:
				case BASICTYPE_STRING:
				{
					RtFloat f1=0, f2=0;
					op1.get(f1);
					op2.get(f2);
					return f1 > f2;
				}
				case BASICTYPE_UNKNOWN:
					return false;
			}
			return false;
			break;
		case BASICTYPE_STRING:
			switch(op1.type()) {
				case BASICTYPE_INTEGER:
				case BASICTYPE_FLOAT:
				{
					RtFloat f1=0, f2=0;
					op1.get(f1);
					op2.get(f2);
					return f1 > f2;
				}
				case BASICTYPE_STRING:
				{
					std::string s1, s2;
					op1.get(s1);
					op2.get(s2);
					return s1 > s2;
				}
				case BASICTYPE_UNKNOWN:
					return false;
			}
			return false;
		case BASICTYPE_UNKNOWN:
			return op2.type() == BASICTYPE_UNKNOWN;
	}
	return false;
}

inline CValue operator>=(const CValue &op1, const CValue &op2)
{
	return (op1 > op2) || (op1 == op2);
}

inline CValue operator<=(const CValue &op1, const CValue &op2)
{
	return !(op1 > op2);
}

inline CValue operator<(const CValue &op1, const CValue &op2)
{
	return !(op1 >= op2);
}


} // namespace RiCPP

#endif // _RICPP_RICPP_TYPES_H
