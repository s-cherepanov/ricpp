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
 
#ifndef _RICPP_RIBASE_RICPPTYPES_H
#include "ricpp/ribase/ricpptypes.h"
#endif // _RICPP_RIBASE_RICPPTYPES_H

#ifndef _RICPP_TOOLS_INLINETOOLS_H
#include "ricpp/tools/inlinetools.h"
#endif // _RICPP_TOOLS_INLINETOOLS_H

#ifndef _RICPP_TOOLS_STRINGPATTERN_H
#include "ricpp/tools/stringpattern.h"
#endif // _RICPP_TOOLS_STRINGPATTERN_H

#include <cstddef>
#include <vector>
#include <sstream>

namespace RiCPP {

/** @brief Possible basic types. All types consists of these basic types.
 */
enum EnumBasicTypes {
	BASICTYPE_UNKNOWN = 0,///< Unknown basic type
	BASICTYPE_INTEGER,    ///< Integer basic type
	BASICTYPE_FLOAT,      ///< Float basic type
	BASICTYPE_STRING      ///< String basic type
};

/** @brief Number of basic types.
 */
const int N_BASICTYPES = (int)BASICTYPE_STRING+1;

/** @brief Possible types.
 */
enum EnumTypes {
	TYPE_UNKNOWN       = 0, ///< Unknown type
	TYPE_FLOAT,				///< Float
	TYPE_INTEGER,			///< Integer
	TYPE_STRING,			///< String (const char * pointer)
	TYPE_POINT,				///< Point (three floats)
	TYPE_VECTOR,			///< Vector (three floats)
	TYPE_NORMAL,			///< Normal (three floats)
	TYPE_HPOINT,			///< Homogene point (four floats)
	TYPE_MATRIX,			///< Homogene matrix (16 floats)
	TYPE_COLOR				///< Colour (Pointer to float)

/*
	// Not in parameter lists
	TYPE_MPOINT,			///< Map point (16 floats)
	TYPE_BOOLEAN,			///< Boolean
	TYPE_BASIS,				///< Base matrix (16 floats)
	TYPE_BOUND,				///< Bounding box (6 floats)
	TYPE_TOKEN,				///< Token
	TYPE_POINTER,			///< Arbitrary pointer
	TYPE_VOID,				///< Void type
	TYPE_FILTERFUNC,		///< Pointer to a filter function RtFilterFunc
	TYPE_ERRORHANDLER,		///< Pointer to an errorhandler RtErrorHandler
	TYPE_PROCSUBDIVFUNC,	///< Pointer to a subdivision procedure RtProcSubdivFunc
	TYPE_PROCFREEFUNC,		///< Pointer to a cleanup function RtProcFreeFunc
	TYPE_ARCHIVECALLBACK,	///< Pointer to an archive callback function RtArchiveCallback
	TYPE_OBJECTHANDLE,		///< Handle of an object RtObjectHandle
	TYPE_LIGHTHANDLE,		///< Handle of a light source RtLightSource
	TYPE_CONTEXTHANDLE		///< Handle of a renderer context RtContextHandle
*/
};

/** @brief Number of different types.
 */
const int N_TYPES  = (int)TYPE_COLOR+1;

/** @brief Possible parameter storage classes.
 */
enum EnumClasses {
	CLASS_UNKNOWN	 = 0, ///< Unknown class
	CLASS_CONSTANT,	      ///< Constant class (one value)
	CLASS_UNIFORM,	      ///< Constant class (one value per face, shared corners count one)
	CLASS_VARYING,	      ///< Varying class (one value per corner, shared vertex count one)
	CLASS_VERTEX,	      ///< Vertex class (one value per vertex)
	CLASS_FACEVARYING,	  ///< Vertex class (one value per corner of each face)
	CLASS_FACEVERTEX	  ///< Face Vertex class (one value per vertex of each face)
};

/** @brief Number of different classes.
 */
const int N_CLASSES = (int)CLASS_FACEVERTEX+1;

/** @brief Possible qualifiers for declarations.
 */
enum EnumQualifiers {
	QUALIFIER_UNKNOWN = 0,         ///< Unknown or undefined qualifier
	QUALIFIER_PROJECTION,          ///< Arbitrary projections
	QUALIFIER_IMAGER,              ///< Imager shader
	QUALIFIER_DISPLAY,             ///< Display drivers
	QUALIFIER_HIDER,               ///< Hider algorithms
	QUALIFIER_OPTION,              ///< User defined options
	QUALIFIER_LIGHT_SOURCE,        ///< Light source shaders
	QUALIFIER_AREA_LIGHT_SOURCE,   ///< Area light source shaders
	QUALIFIER_SURFACE,             ///< Surface shaders
	QUALIFIER_ATMOSPHERE,          ///< Atmosphere volume shaders
	QUALIFIER_INTERIOR,            ///< Interior volume shaders
	QUALIFIER_EXTERIOR,            ///< Exterior volume shaders
	QUALIFIER_DISPLACEMENT,        ///< Displacement shaders
	QUALIFIER_DEFORMATION,         ///< Nonlinear deformation shaders
	QUALIFIER_ATTRIBUTE,           ///< User defined attributes
	QUALIFIER_TEXTURE,             ///< Texture resources
	QUALIFIER_CONTROL,             ///< System controls
	QUALIFIER_BEGIN,               ///< Renderer initialization
	QUALIFIER_RESOURCE             ///< Resource handlers
};

/** @brief Number of basic types.
 */
const int N_QUALIFIERS = (int)QUALIFIER_RESOURCE+1;

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
	static RtToken ms_qualifiers[N_QUALIFIERS]; ///< Table qualifiers

	/** @brief Compares prefixes.
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
	/** @brief Gets the name of a basic type.
	 * @param e Ri basic type
	 * @return The name (like in RIB or declarations) of a basic type
	 * @see EnumBasicTypes
	 */
	static RtToken basicTypeName(EnumBasicTypes e);

	/** @brief Gets the size of a basic types in bytes.
	 * @param e Ri basic type
	 * @return The size of a basic types in bytes
	 * @see EnumBasicTypes
	 */
	static unsigned int basicTypeByteSize(EnumBasicTypes e);

	/** @brief Gets the name of a type that can occur in declarations and RIB files.
	 * @param e Ri type
	 * @return The name (like in RIB or declarations) of a type
	 * @see EnumTypes
	 */
	static RtToken typeName(EnumTypes e);

	/** @brief Gets the size of a types in elements.
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

	/** @brief Gets the size of a types in bytes.
	 * @param e Ri type
	 * @return The size of a types in bytes, color needs special handling
	 * @see EnumTypes, typeSize(EnumTypes e)
	 */
	static unsigned int typeByteSize(EnumTypes e);

	/** @brief Gets the basic type.
	 * @param e Ri type
	 * @return The basic type of a type
	 */
	static EnumBasicTypes basicTypeForType(EnumTypes e);

	/** @brief Gets the name of the storage class.
	 * @param e Ri storage class
	 * @return The name of a storage class as used in declarations and RIB
	 */
	static RtToken className(EnumClasses e);

	/** @brief Gets the name of the qualifier (qualifier was named table namespace in [QRM]).
	 * @param aQualifier Ri qualifier
	 * @return The name of a qualifier as used in declarations and RIB
	 */
	static RtToken qualifier(EnumQualifiers aQualifier);

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
	 *  @retval arraySize Size n of the array
	 *  @return The size of the array
	 @verbatim
	   -  < 0, syntax error or [0]
	   - == 0, no cardinality found
	   -  > 0, [n] found, returns the n
	 @endverbatim
	 */
	static bool arrayPrefix(const char *acard, size_t &pos, unsigned long &arraySize);

	/** @brief Tries to find a qualifier+':' as prefix of avar.
	 * @param avar string possibly having qualifier+':' as prefix
	 * @retval pos if found position right behind the prefix
	 * @return QUALIFIER_UNKNOWN if no prefix found, matching EnumQualifierss if found.
	 */
	static EnumQualifiers qualifierPrefix(const char *avar, size_t &pos);

	/** @brief Try to find the token of a specific basis.
	 *
	 *  @param aBasis Spline basis matrix
	 *  @return Token for a specific basis @a aBasis (e.g. RI_BEZIER), RI_NULL, if there is none
	 */
	static RtToken basisName(const RtBasis aBasis);

	/** @brief Try to find a basis fot a token.
	 *
	 *  @param basisToken Token for a specific basis @a aBasis (e.g. RI_BEZIER)
	 *  @retval aBasis Spline basis matrix
	 *  @return true, matrix was found and returnd in @a aBasis
	 */
	static bool getBasis(RtToken basisToken, RtBasis aBasis);
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
	RtInt m_nColorSamples;       ///< @brief The number of color samples (\>0).
	std::vector<RtFloat> m_nRGB; ///< @brief n times 3 matrix to transform a color into it's RGB values.
	std::vector<RtFloat> m_RGBn; ///< @brief 3 times n matrix to transform a RGB value to a color.
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

	/** @brief Duplication.
	 *  @return Clone of this
	 */
	CColorDescr *duplicate() const
	{
		return new CColorDescr(*this);
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

	/** @brief Gets the RGBn matrix.
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
	RtFloat m_x;  ///< @brief X component of a point on the plane.
	RtFloat m_y;  ///< @brief Y component of a point on the plane.
	RtFloat m_z;  ///< @brief Z component of a point on the plane.
	RtFloat m_nx; ///< @brief X component of the normal vector of the plane.
	RtFloat m_ny; ///< @brief X component of the normal vector of the plane.
	RtFloat m_nz; ///< @brief X component of the normal vector of the plane.
public:

	/** @brief Constructor.
	 *
	 *  Initializes plane in origin with [0 0 1] as normal.
	 */
	CClippingPlane();

	/** @brief Constructor, sets values.
	 *
	 *  Initializes plane containing point [aX aY aZ], and has vector [aNX aNY aNZ] as normal.
	 *
	 * @param aX  X component of a point on the plane.
	 * @param aY  Y component of a point on the plane.
	 * @param aZ  Z component of a point on the plane.
	 * @param aNX X component of the normal vector of the plane.
	 * @param aNY Y component of the normal vector of the plane.
	 * @param aNZ Z component of the normal vector of the plane.
	 */
	CClippingPlane(RtFloat aX, RtFloat aY, RtFloat aZ, RtFloat aNX, RtFloat aNY, RtFloat aNZ);

	/** @brief Copy constructor.
	 *  @param p Used to initialize copy
	 */
	inline CClippingPlane(const CClippingPlane &p)
	{
		*this = p;
	}

	/** @brief Duplication.
	 *  @return Clone of this
	 */
	CClippingPlane *duplicate() const
	{
		return new CClippingPlane(*this);
	}


	/** @brief Assignes another plane.
	 *  @param p Plane to assign.
	 *  @return *this
	 */
	CClippingPlane &operator=(const CClippingPlane &p);

	/** @brief Sets values.
	 *
	 *  Initializes plane containing point [aX aY aZ], and has vector [aNX aNY aNZ] as normal.
	 *
	 * @param aX  X component of a point on the plane.
	 * @param aY  Y component of a point on the plane.
	 * @param aZ  Z component of a point on the plane.
	 * @param aNX X component of the normal vector of the plane.
	 * @param aNY Y component of the normal vector of the plane.
	 * @param aNZ Z component of the normal vector of the plane.
	 */
	void set(RtFloat aX, RtFloat aY, RtFloat aZ, RtFloat aNX, RtFloat aNY, RtFloat aNZ);

	/** @brief Sets single component.
	 *  @param aX  X component of a point on the plane.
	 */
	inline RtVoid x(RtFloat aX)
	{
		m_x = aX;
	}

	/** @brief Gets single component.
	 *  @return X component of a point on the plane.
	 */
	inline RtFloat x() const
	{
		return m_x;
	}

	/** @brief Sets single component.
	 *  @param aY  Y component of a point on the plane.
	 */
	inline RtVoid y(RtFloat aY)
	{
		m_y = aY;
	}

	/** @brief Gets single component.
	 *  @return Y component of a point on the plane.
	 */
	inline RtFloat y() const
	{
		return m_y;
	}

	/** @brief Sets single component.
	 *  @param aZ  Z component of a point on the plane.
	 */
	inline RtVoid z(RtFloat aZ)
	{
		m_z = aZ;
	}

	/** @brief Gets single component.
	 *  @return Z component of a point on the plane.
	 */
	inline RtFloat z() const
	{
		return m_z;
	}

	/** @brief Sets single component.
	 *  @param aNX X component of the normal vector of the plane.
	 */
	inline RtVoid nx(RtFloat aNX)
	{
		m_nx = aNX;
	}

	/** @brief Gets single component.
	 *  @return X component of the normal vector of the plane.
	 */
	inline RtFloat nx() const
	{
		return m_nx;
	}

	/** @brief Sets single component.
	 *  @param aNY Y component of the normal vector of the plane.
	 */
	inline RtVoid ny(RtFloat aNY)
	{
		m_ny = aNY;
	}

	/** @brief Gets single component.
	 *  @return Y component of the normal vector of the plane.
	 */
	inline RtFloat ny() const
	{
		return m_ny;
	}

	/** @brief Sets single component.
	 *  @param aNZ Z component of the normal vector of the plane.
	 */
	inline RtVoid nz(RtFloat aNZ)
	{
		m_nz = aNZ;
	}

	/** @brief Gets single component.
	 *  @return Z component of the normal vector of the plane.
	 */
	inline RtFloat nz() const
	{
		return m_nz;
	}
}; // CClippingPlane


/** @brief Quantizer.
 *
 *  Container of the parameters of a IRi::quantize() interface call,
 *  and methods to quantize.

 *
 */
class CQuantizer {
	RtToken m_type; ///< Token of the quantizer type (color RI_RGB or depth RI_Z).
	RtInt m_one,    ///< Integer value to mutiply with input float parameters, 0: no quantization output as floats.
	      m_min,    ///< Minimal integer value output (result is clambed).
	      m_max;    ///< Maximal integer value output (result is clambed).
	RtFloat m_ditherAmplitude; ///< Ditheramplitude to add before clambing (multiplied with a random number between -1 and 1).

public:

	/** @brief, initializes al parameters to 0 (no quantization).
	 */
	CQuantizer();

	/** @brief Constructor, sets values.
	 *
	 * @param aType Token of the quantizer type (color RI_RGB or depth RI_Z)
	 * @param aOne Integer value to mutiply with input float parameters, 0: no quantization output as floats.
	 * @param aMin Minimal integer value output (result is clambed)
	 * @param aMax Maximal integer value output (result is clambed)
	 * @param aDitherAmplitude Ditheramplitude to add before clambing (multiplied with a random number between -1 and 1)
	 */
	CQuantizer(RtToken aType, RtInt aOne, RtInt aMin, RtInt aMax, RtFloat aDitherAmplitude);

	/** @brief Copy constructor.
	 *  @param quantizer Used to initialize copy
	 */
	inline CQuantizer(const CQuantizer &quantizer)
	{
		*this = quantizer;
	}

	/** @brief Duplication.
	 *  @return Clone of this
	 */
	CQuantizer *duplicate() const
	{
		return new CQuantizer(*this);
	}

	/** @brief Assigns values of another object.
	 *  @param q Object to assign.
	 */
	CQuantizer &operator=(const CQuantizer &q);

	/** @brief Sets values.
	 *
	 * @param aType Token of the quantizer type (color RI_RGB or depth RI_Z)
	 * @param aOne Integer value to mutiply with input float parameters, 0: no quantization output as floats.
	 * @param aMin Minimal integer value output (result is clambed)
	 * @param aMax Maximal integer value output (result is clambed)
	 * @param aDitherAmplitude Ditheramplitude to add before clambing (multiplied with a random number between -1 and 1)
	 */
	void set(RtToken aType, RtInt aOne, RtInt aMin, RtInt aMax, RtFloat aDitherAmplitude);

	/** @brief Sets single value.
	 *  @param aType Token of the quantizer type (color RI_RGB or depth RI_Z)
	 */
	inline RtVoid type(RtToken aType)
	{
		m_type = aType;
	}

	/** @brief Gets single value.
	 *  @return Token of the quantizer type.
	 */
	inline RtToken type() const
	{
		return m_type;
	}

	/** @brief Sets single value.
	 *  @param aOne Integer value to mutiply with input float parameters, 0: no quantization output as floats.
	 */
	inline RtVoid one(RtInt aOne)
	{
		m_one = aOne;
	}

	/** @brief Gets single value.
	 *  @return Integer value to mutiply with input float parameters, 0: no quantization.
	 */
	inline RtInt one() const
	{
		return m_one;
	}

	/** @brief Sets single value.
	 *  @param aMin Minimal integer value output (result is clambed).
	 */
	inline RtVoid minVal(RtInt aMin)
	{
		m_min = aMin;
	}

	/** @brief Gets single value.
	 *  @return Minimal integer value output.
	 */
	inline RtInt minVal() const
	{
		return m_min;
	}

	/** @brief Sets single value.
	 *  @param aMax Maximal integer value output (result is clambed).
	 */
	inline RtVoid maxVal(RtInt aMax)
	{
		m_max = aMax;
	}

	/** @brief Gets single value.
	 *  @return Maximal integer value output.
	 */
	inline RtInt maxVal() const
	{
		return m_max;
	}

	/** @brief Sets single value.
	 *  @param aDitherAmplitude Ditheramplitude to add before clambing (multiplied with a random number between -1 and 1).
	 */
	inline RtVoid ditherAmplitude(RtFloat aDitherAmplitude)
	{
		m_ditherAmplitude = aDitherAmplitude;
	}

	/** @brief Gets single value.
	 *  @return Ditheramplitude.
	 */
	inline RtFloat ditherAmplitude() const
	{
		return m_ditherAmplitude;
	}

	/** @brief Quantize a single value.
	 *  @param value Value to quantize.
	 *  @return Quantized value (or copy of @a value, if aOne==0).
	 */
	RtFloat quantize(RtFloat value) const;

	/** @brief Quantize a vector of values.
	 *  @retval values Values to quantize (nothing done if aOne == 0).
	 */
	void quantize(std::vector<RtFloat> &values) const;
}; // CQuantizer


/** @brief Array sizes of a trim curve.
 */
struct CTrimCurveDataInfo {
	RtInt m_nloops; ///< Number of closed loops.

	int m_total;	///< Total number of curves.
	int m_npoints;	///< Total number of control points.
	int m_nknots;	///< Total number of knots.

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
	CTrimCurveDataInfo   m_data;    ///< Sizes.
	
	std::vector<RtInt>   m_nCurves; ///< Number if loops (curves with holes).
	std::vector<RtInt>   m_order;   ///< Order of the curves.
	std::vector<RtFloat> m_knots;   ///< Knot vectors of the curves.
	std::vector<RtFloat> m_min;     ///< Minimum parametric values of the curves.
	std::vector<RtFloat> m_max;     ///< Maximum parametric values of the curves.
	std::vector<RtInt>   m_n;       ///< Number of coordinates.
	std::vector<RtFloat> m_u;       ///< u coordinates of the curves.
	std::vector<RtFloat> m_v;       ///< v coordinates of the curves.
	std::vector<RtFloat> m_w;       ///< w coordinates of the curves.
	std::vector<RtFloat> m_points;  ///< Points filled with (u[0], v[0], w[0], ... ).

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
	 *  @param curve Used to initialize copy.
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


	/** @brief Duplication.
	 *  @return Clone of this.
	 */
	CTrimCurveData *duplicate() const
	{
		return new CTrimCurveData(*this);
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

	/** @brief Compares to objects by components.
	 *
	 *  @param curve The curve to compare to.
	 *  @return true, all components of *this and @a curve are equal.
	 */
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
	/** @brief The value of the 4x4 row major matrix, init with identity.
	 *
	 @verbatim
	 Index [i][j]:
	 [0][0] [0][1] [0][2] [0][3]
	 [1][0] [1][1] [1][2] [1][3]
	 [2][0] [2][1] [2][2] [2][3]
	 [3][0] [3][1] [3][2] [3][3]
	 @endverbatim
	 */
	RtMatrix m_Matrix;

	/** @brief true (default), if premultiply when concatenate matrices (e.g. by transform()).
	 */
	bool m_preMultiply;

	// void extract(RtFloat *result, const RtFloat *mat, int i, int size) const;
	// RtFloat det(const RtFloat *mat, int size) const;
public:

	/** @brief The standard constructor.
	 *
	 *  Initializes the matric with the identity matrix, sets premultiplication.
	 */
	CMatrix3D();

	/** @brief The copy constructor (deep copy).
	 *  @param mat Matrix to copy from.
	 */
	CMatrix3D(const CMatrix3D &mat);

	/** @brief The constructor to copy from a RenderMan RtMatrix.
	 *  @param mat Matrix to copy from.
	 */
	CMatrix3D(RtMatrix mat);
	
	/** @brief Dupliactes a matrix.
	 *  @return a clone of *this.
	 */
	inline CMatrix3D *duplicate() const
	{
		return new CMatrix3D(*this);
	}

	/** @brief Destructor, has nothing to do.
	 */
	inline ~CMatrix3D() { }

	/** @brief The assignment (deep copy).
	 *  @param mat Matrix to copy from.
	 */
	CMatrix3D &operator=(const CMatrix3D &mat);

	/** @brief The assignment for a RenderMan RtMatrix.
	 *
	 *  @c m_preMultiply is not changed because there
	 *  no flag in RtMatrix.
	 *
	 *  @param mat Matrix to copy from.
	 */
	CMatrix3D &operator=(RtMatrix mat);

	/** @brief Gets a pointer to the first component of a matrix.
	 *  @return Pointer to the first float value of the matrix.
	 */
	operator const RtFloat *() const { return (const RtFloat *)&m_Matrix[0]; }
	// RtFloat *getPointer() const { return (RtFloat *)&m_Matrix[0]; }

	/** @brief Sets the pre multification flag.
	 *  @param preMultiply true, for standard pre multiplication (left multiplication).
	 */
	inline void setPreMultiply(bool preMultiply=true) {
		m_preMultiply = preMultiply;
	}

	/** @brief Gets the pre multification flag.
	 *  @return true, if standard pre multiplication (left multiplication).
	 */
	inline bool getPreMultiply() const {
		return m_preMultiply;
	}

	/** @brief Sets the components by coping a RenderMan RtMatrix.
	 *  @param mat Matric to copy
	 */
	inline void set(const RtMatrix mat) {
		memcpy(m_Matrix, mat, sizeof(RtMatrix));
	}

	/** @brief Sets the components by coping a RtFloat array (16 values).
	 *  @param mat RtFloat array with at least 16 values.
	 */
	inline void set(const RtFloat *mat) {
		memcpy(m_Matrix, mat, sizeof(RtMatrix));
	}

	/** @brief Gets the content of a matrix for a RenderMan RtMatrix.
	 *  @retval mat Matrix to fill.
	 */
	void get(RtMatrix &mat) const;

	/** @brief Compares two matrices.
	 *  @param mat Matrix to compare with.
	 *  @return true, if all components of mat are equal to the components of the instance.
	 */
	bool operator==(const CMatrix3D &mat) const;

	/** @brief Compares with RtMatrix.
	 *  @param mat RenderMan RtMatrix matrix to compare with.
	 *  @return true, if all components of mat are equal to the components of the instance.
	 */
	bool operator==(RtMatrix mat) const;

	/** @brief Compares two matrices.
	 *  @param mat Matrix to compare with.
	 *  @return false, if all components of mat are equal to the components of the instance.
	 */
	bool operator!=(const CMatrix3D &mat) const;

	/** @brief Compares with RtMatrix.
	 *  @param mat RenderMan RtMatrix matrix to compare with.
	 *  @return false if all components of mat are equal to the components of the instance.
	 */
	bool operator!=(RtMatrix mat) const;

	/** @brief Clears a matrix.
	 * Sets all components to zero.
	 @verbatim
	 0 0 0 0
	 0 0 0 0
	 0 0 0 0
	 0 0 0 0
	 @endverbatim
	 */
	void clear();

	/** @brief Set the identity matrix.
	/* Sets the components to:
	 @verbatim
	 1 0 0 0
	 0 1 0 0
	 0 0 1 0
	 0 0 0 1
	 @endverbatim
	 */
	void identity();

	/** @brief Transposes the matrix.
	 *  Swaps the components as follows:
	 @verbatim
	 m[i][j] = m[j][i]
	 @endverbatim
	 */
	void transpose();

	/** @brief Transform.
	 * Transform is to copy a matrix:
	 @verbatim
	 M = mat
	 @endverbatim
	 * @param mat Matrix (RenderMan RtMatrix) used for transformation.
	 */
	void transform(RtMatrix mat);

	/** @brief Transform.
	 * Transform is to copy a matrix:
	 @verbatim
	 M = mat
	 @endverbatim
	 * @param mat Matrix used for transformation.
	 */
	void transform(CMatrix3D &mat);

	/** @brief Postmultiplicaton, right multiplication.
	 * Postmultiplication is used by inverse transformations.
	 @verbatim
	 M' = M x mat
	 @endverbatim
	 * @param mat Matrix (RenderMan RtMatrix) to concatenate on the right.
	 */
	void postMultiply(RtMatrix mat);

	/** @brief Postmultiplicaton, right multiplication.
	 * Postmultiplication is used by inverse transformations.
	 @verbatim
	 M' = M x mat
	 @endverbatim
	 * @param mat Matrix to concatenate on the right.
	 */
	void postMultiply(CMatrix3D mat);

	/** @brief Premultiplication (left multiplication).
	/* Premultiplication is used by the implementation of the RenderMan interface to concatenate transformations.
	 @verbatim
	 M' = mat x M
	 @endverbatim
	 * @param mat Matrix (RenderMan RtMatrix) to concatenate on the left.
	 */
	void preMultiply(RtMatrix mat);

	/** @brief Premultiplication (left multiplication).
	/* Premultiplication is used by the implementation of the RenderMan interface to concatenate transformations.
	 @verbatim
	 M' = mat x M
	 @endverbatim
	 * @param mat Matrix to concatenate on the left.
	 */
	void preMultiply(CMatrix3D mat);

	/** @brief Matrix multiplication.
	/* m_preMultiply is used o multiply on the left or on the right
	 @verbatim
	 M' = mat x M (if m_preMultiply) or M' = M x Mat (if not m_preMultiply)
	 @endverbatim
	 * @param mat Matrix (RenderMan RtMatrix) to concatenate.
	 */
	void concatTransform(RtMatrix mat);

	/** @brief Matrix multiplication.
	/* m_preMultiply is used o multiply on the left or on the right
	 @verbatim
	 M' = mat x M (if m_preMultiply) or M' = M x Mat (if not m_preMultiply)
	 @endverbatim
	 * @param mat Matrix to concatenate.
	 */
	void concatTransform(CMatrix3D mat);

	/** @brief Concatenate a translation.
	/*  The result matrix is:
	 @verbatim
	 a    b    c    d
	 e    f    g    h
	 i    j    k    l
	 m+dx n+dy o+dz p
	 @endverbatim
	 *
	 *  @param dx translate in x direction
	 *  @param dy translate in y direction
	 *  @param dz translate in z direction
	 */
	void translate(RtFloat dx, RtFloat dy, RtFloat dz);

	/** @brief Concatenate a scale.
	 *  The result matrix is:
	 @verbatim
	 a*sx b    c    d
	 e    f*sy g    h
	 i    j    k*sz l
	 m    n    o    p
	 @endverbatim
	 *
	 *  @param sx scale in x direction
	 *  @param sy scale in y direction
	 *  @param sz scale in z direction
	 */
	void scale(RtFloat sx, RtFloat sy, RtFloat sz);

	/** @brief Transforms a point by the matrix.
	 *
	 *  If m_preMultiply it uses a row vector and left multiplication,
	 *  if not m_preMultiply a column vector and left multiplication
	 *
	 *  @retval x coordinate x and result
	 *  @retval y coordinate y and result
	 *  @retval z coordinate z and result
	 */
	void transformPoints(RtFloat &x, RtFloat &y, RtFloat &z);

	/** @brief Concatenates a rotation around the x-axis.
	 * The rotation matrix is:
	 @verbatim
	 1  0      0      0
	 0  cos(w) sin(w) 0
	 0 -sin(w) cos(w) 0
	 0  0      0      1
	 @endverbatim
	 *  @param w degrees to rotate
	 */
	void rotateX(RtFloat w);

	/** @brief Concatenates a rotation around the y-axis.
	 * The rotation matrix is:
	 @verbatim
	 cos(w) 0 -sin(w) 0
	 0      1  0      0
	 sin(w) 0  cos(w) 0
	 0      0  0      1
	 @endverbatim
	 *  @param w degrees to rotate
	 */
	void rotateY(RtFloat w);

	/** @brief Concatenates a rotation around the z-axis.
	 * The rotation matrix is:
	 @verbatim
	  cos(w) sin(w) 0 0
	 -sin(w) cos(w) 0 0
	 0      0      1 0
	 0      0      0 1
	 @endverbatim
	 *  @param w degrees to rotate
	 */
	void rotateZ(RtFloat w);

	/** @brief Rotates w degrees around the axis vector x, y, z.
	 * The matrix is:
	 @verbatim
	 [0][0] = (RtFloat)(x*x + cosw*(1.0 - x*x));
	 [0][1] = (RtFloat)(x*y*(1.0 - cosw) + z*sinw);
	 [0][2] = (RtFloat)(z*x*(1.0 - cosw) - y*sinw);
	 [1][0] = (RtFloat)(x*y*(1.0 - cosw) - z*sinw);
	 [1][1] = (RtFloat)(y*y + cosw*(1.0 - y*y));
	 [1][2] = (RtFloat)(y*z*(1.0 - cosw) + x*sinw);
	 [2][0] = (RtFloat)(z*x*(1.0 - cosw) + y*sinw);
	 [2][1] = (RtFloat)(y*z*(1.0 - cosw) - x*sinw);
	 [2][2] = (RtFloat)(z*z + cosw*(1.0 - z*z));
	 @endverbatim
	 *  @param w degrees to rotate
	 *  @param x x-coordinate of the axis vector
	 *  @param y y-coordinate of the axis vector
	 *  @param z z-coordinate of the axis vector
	 */
	void rotate(RtFloat w, RtFloat x, RtFloat y, RtFloat z);

	/** @brief Skews a vector (x1, y1, z1) w degrees towards (x2, y2, z2).
	 * s.a. More Matrices and Transformations: Shear and Pseudo-Perspective
	 *      Ronald N. Goldman in Graphics Gems II, Academic Press Inc.
	 * @verbatim
	 Shear(Q,v,w,phi) (Q = (0, 0, 0), v = (x1, y1, z1), w = (x2, y2, z2), phi = w)
	 | I+tan phi(v (X) w) 0 | I : id matrix, (X) : tensor product
	 | -(Q.v)w            1 | . : dot product
	 @endverbatim
	 */
	void skew(RtFloat w, RtFloat x1, RtFloat y1, RtFloat z1, RtFloat x2, RtFloat y2, RtFloat z2);

	/** @brief Concatenates the RMan perspective transformation.
	 *  Code copied from the aqsis renderer, concatenates the matrix:
	 @verbatim
	 f = (RtFloat)tan(degtorad(fov)/2.0);
	 1.0 0.0 0.0 0.0
	 0.0 1.0 0.0 0.0
	 0.0 0.0   f   f
	 0.0 0.0  -f 0.0
	 @endverbatim
	 * @param fov The field of view for the perspective nmatrix
	 */
	void perspective(RtFloat fov);

	/** @brief Concatenates the inverse RMan perspective transformation.
	 @verbatim
	 f = (RtFloat)tan(degtorad(fov)/2.0);
	 1.0 0.0 0.0 0.0
	 0.0 1.0 0.0 0.0
	 0.0 0.0 0.0  -f
	 0.0 0.0   f   f
	 @endverbatim
	 * @param fov The field of view for the perspective nmatrix
	 */
	void inversePerspective(RtFloat fov);

	/** @brief Returns the determinant of this matrix.
	 *  Using an algorithm from Graphics Gems I (p768). Source copied from
	 *  Aqsis renderer.
	 *  @return The determinant of the homogene matrix.
	 */
	RtFloat determinant() const;

	/** @brief Test if matrix is the identity matrix
	 * @return true: if matrix is the identity matrix, false: otherwise
	 */
	bool isIdentity() const;

	/** @brief Returns the inverse of this matrix.
	 * Using an algorithm from Graphics Gems IV (p554),
	 * Gauss-Jordan elimination with partial pivoting.
	 * Source copied from Aqsis renderer.
	 * @retval mat The inverse matrix.
	 * @return true: Inverse could be calculated, false: otherwise.
	 */
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
			char buf[32];
			aString=valToStr(buf, sizeof(buf), m_intVal);
			return true;
		}
		if ( m_type == BASICTYPE_FLOAT ) {
			char buf[32];
			aString=valToStr(buf, sizeof(buf), m_floatVal);
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
