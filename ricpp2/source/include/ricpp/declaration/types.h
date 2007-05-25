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
 *  @author Andreas Pidde (andreas@pidde.de)
 *  @brief Possible basic types, types and classes
 */
 
#ifndef _RICPP_RICPP_RICPP_H
#include "ricpp/ricpp/ricpp.h"
#endif // _RICPP_RICPP_RICPP_H

#include <stddef.h>

namespace RiCPP {

/**! @brief Possible basic types. All types consists of these basic types.
 */
enum EnumBasicTypes {
	BASICTYPE_UNKNOWN = 0,//!< Unknown basic type
	BASICTYPE_INTEGER,    //!< Integer basic type
	BASICTYPE_FLOAT,      //!< Float basic type
	BASICTYPE_STRING      //!< String basic type
};

/**! @brief Number of basic types
 */
const int N_BASICTYPES = (int)BASICTYPE_STRING+1;

/**! @brief Possible types
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

/**! @brief Number of different types
 */
const int N_TYPES  = (int)TYPE_COLOR+1;

/**! @brief Possible parameter storage classes
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

/**! @brief Number of different classes
 */
const int N_CLASSES = (int)CLASS_FACEVERTEX+1;

/**! @brief Possible table namespaces for declarations
 */
enum EnumNamespaces {
	NAMESPACE_UNKNOWN = 0,//!< Unknown basic type
	NAMESPACE_AREALIGHT,
	NAMESPACE_LIGHT,
	NAMESPACE_SURFACE,
	NAMESPACE_VOLUME,
	NAMESPACE_IMAGER,
	NAMESPACE_DISPLACEMENT,
	NAMESPACE_DEFORMATION,
	NAMESPACE_INTERIOR,
	NAMESPACE_EXTERIOR,
	NAMESPACE_ATMOSPHERE,
	NAMESPACE_ATTRIBUTE,
	NAMESPACE_OPTION,
	NAMESPACE_GEOMETRY,
	NAMESPACE_HIDER,
	NAMESPACE_RESOURCE
};

/**! @brief Number of basic types
 */
const int N_NAMESPACES = (int)NAMESPACE_RESOURCE+1;

/** @brief Class to query info about Ri-types that can occur in declarations.
 */
class CTypeInfo {
	static const unsigned int m_basicTypeSizes[N_BASICTYPES]; ///< Number of bytes for the basic types
	static RtToken m_basicTypeNames[N_BASICTYPES]; ///< Names of basic types
	static const unsigned int m_typeSizes[N_TYPES+1]; ///< Sizes of the types (+1 to have the same size as m_typeNames, but not needed)
	static RtToken m_typeNames[N_TYPES+1]; ///< Type names (+1 to recognize 'int', that is internally converted to 'integer' in typePrefix())
	static RtToken m_classNames[N_CLASSES]; ///< Storage class names
	static const unsigned int m_typeByteSizes[N_TYPES+1]; ///< Number of bytes for the types
	static const EnumBasicTypes m_basicTypesForTypes[N_TYPES+1]; ///< Basic types the types consists of.
	static RtToken m_namespaces[N_NAMESPACES]; ///< Table namespaces

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
	static int arrayPrefix(const char *acard, size_t &pos);

	/** @brief Tries to find a namespace+':' as prefix of avar.
	 * @param avar string possibly having namespace+':' as prefix
	 * @retval pos if found position right behind the prefix
	 * @return NAMESPACE_UNKNOWN if no prefix found, matching EnumNamespaces if found.
	 */
	static EnumNamespaces namespacePrefix(const char *avar, size_t &pos);
}; // CTypeInfo

} // namespace RiCPP

#endif // _RICPP_RICPP_TYPES_H