#ifndef _RICPP_RENDERCONTEXT_PARAMETERDECLARATION_H
#define _RICPP_RENDERCONTEXT_PARAMETERDECLARATION_H

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

/*! \file parameterdeclaration.h
 *  \brief Declares a class that parses, stores and can write to a std::ostream
 *         the declaration of a parameter type \sa TRi::declare()
 *  \author Andreas Pidde (andreas@pidde.de)
 */

#include "rendererloader/ricpp.h"
#include "ribparser/riconstants.h"
#include "rendererloader/sysexcept.h"

#include <fstream>

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//! TParameterDeclaration - class to hold one parameter declaration
//!                         \sa TRi::declare()
class TParameterDeclaration
{
	//! TDeclarationMap that stores the declarations in a map can use private data
	friend class TDeclarationMap;

	//! tables with static information about types
	//!{
	static int m_typeSizes[N_TYPESIZES];				//!< sizes of the instances types, color sizeof(RtFloat), Strings size of a pointer
	static int m_typeComponents[N_TYPESIZES];			//!< number of the components of the instances types, color has special handling
	static const char *m_typeNames[N_TYPESIZES];		//!< names of the types
	static const char *m_classNames[N_CLASSNAMES];		//!< names of the storage classes
	//!}

	//! member variables for specific types
	//!{
	std::string m_name;     //!< Name of the type
	std::string m_fullname; //!< Name and declaration (inline)
	RtInt m_class;          //!< Storage class of the variable CLASS_...
	RtInt m_type;           //!< Data type of the variable TYPE_...
	RtInt m_cardinality;    //!< Cardinality as positive integer, >= 1
	bool m_isInline;        //!< Parameter declaration was an inline declaration
	bool m_isDefault;       //!< declared type is a default of the renderer (not discarded in TRi::end())
	//!}

	//! Helper, compares 'token' as prefix of 'search'
	/*! The functions looks if 'token' is in the front of the string 'search'.
	 *  After the token there must be a white space or a [ in 'search'.
	 *  \param token A token to look for
	 *  \param search The string that can contain token at it's front
	 *  \return Value like in strcmp(): 0 if 'token' is a prefix of 'search'
	 */
	static int tokcmp(const char *token, const char *search);
	void buildFullname();   //!< Build the full name out of declaration

public:
	//! Default constructor, clears all members
	TParameterDeclaration();

	//! Copy constructor
	/*! \param decl Instance to copy from
	 */
	TParameterDeclaration(const TParameterDeclaration &decl);

	//! Constructor, set all members.
	/*! \param name The name of the class.
	 *  \param aclass The storage class as CLASS_ Constant.
	 *  \param type The data type as TYPE_ constant.
	 *  \param cardinality The Cardinality of the type, 1 if single value, number of indicees [n] for arrays.
	 *  \param isDef Class is a default class, defined by the renderer, these declarations stay valid, even after TRi::end() is called.
	 */
	TParameterDeclaration(
		RtToken name,
		RtInt aclass,
		RtInt type,
		RtInt cardinality,
		bool isDef = false
	);

	//! constructor, set all members by parsing
	TParameterDeclaration(const char *name, const char *type, bool isDef = false);

	//! Destructor.
	~TParameterDeclaration();

	//! Duplication of an instance of TParameterDeclaration.
	/*! \return A duplicate of *this.
	 *  \throw TSystemException if not enough memory.
     */
	TParameterDeclaration &duplicate() const;

	//! Assignment.
	//! \param decl Instance to copy from.
	//! \return Reference to *this.
	TParameterDeclaration &operator=(const TParameterDeclaration &decl);
	
	//! Get the number of bytes used for one component of an instance of this type.
	/*! \param type Index of type (constance TYPE_...).
	 *  \return The number of bytes for requested type (colors, points e.t.c.: only one component).
	 */
	static int sizeOfType(RtInt type);

	//! Get the name of a storage class.
	/*! \param aclass Index of the storage class (constant CLASS_...).
	 *  \return Name of the storage class.
	 */
	static const char *nameOfClass(RtInt aclass);

	//! Get the name of a type.
	/*! \param type Index of the type (constant TYPE_...).
	 *  \return Name of the type.
	 */
	static const char *nameOfType(RtInt type);

	//! Get the index of a storage class, scan aclass to find a class name.
	/*! \param aclass String with a class name as prefix
	 *  \param pos Postition behind the class name
	 *  \return Index (constant CLASS_...) of the storage class found.
	 */
	static int classNum(const char *aclass, size_t &pos);

	//! Get the index of a type name, scan type to find a type name.
	/*! \param type String with a type name as prefix
	 *  \param pos Postition behind the type name
	 *  \return Index (constant TYPE_...) of the type found.
	 */
	static int typeNum(const char *type, size_t &pos);

	//! Get the cardinality (array size), scan acard to find the brackets and the size.
	/*! \param acard String with a cardinality '['<n>']' as prefix
	 *  \param pos Postition behind the cardinality
	 *  \return Array size found, returns
	 *    < 0, syntax error or [0],
	 *   == 0, no cardinality,
	 *    > 0, [n] found.
	 */
	static int cardinality(const char *acard, size_t &pos);

	//! Sets the name and declaration.
	/*! \param name Name of the declaration.
	 *  \param aclass The storage type of the declaration.
	 *  \param type The type of the declaration.
	 *  \param cardinality The array size.
	 *  \param isDef frue: The declaration is a default declaration (standard declaration of the shader), false: Not a default declaration
	 */
	void set(RtToken name, RtInt aclass, RtInt type, RtInt cardinality, bool isDef = false);

	//! Parses the declaration part, returns true if success.
	/*! \param name Name of the declaration or NULL if inline declaration.
     *  \param type Type of the declaration, or inline declaration.
	 *  \param isDef true, declaration is a default declaration of the renderer, false: uder defined declaration
	 *  \return true: if declaration could get parsed, false: otherwise
	 */
	bool parse(const char *name, const char *type, bool isDef = false);

	//! Get information about a declaration.
	/*! \retval aclass Storage class of the declaration, CLASS_... constant
	 *  \retval type The type of the declaration, TYPE_... constant
	 *  \retval cardinality Cardinality of the declaration (array size)
	 *  \return Name of the declaration
	 */
	RtToken getDeclaration(RtInt &aclass, RtInt &type, RtInt &cardinality) const;

	//! Get information about a declaration
	/*! \param colorComp Number of color components for the declaraton
	 *  \retval aclass Storage class of the declaration, CLASS_... constant
	 *  \retval type The type of the declaration, TYPE_... constant
	 *  \retval cardinality Cardinality of the declaration (array size)
	 *  \retval components Number of components of a single element
	 *  \retval size Number of bytes used to store a complete element of the declaration
	 *  \return Name of the declaration
	 */
	RtToken getDeclaration(RtInt colorComp, RtInt &aclass, RtInt &type, RtInt &cardinality, RtInt &components, RtInt &bytes) const;

	//! Returns the name of the declaration, class, type and cardinality.
	//@{
	RtToken getName() const;                       //!< \return The name or if declared inline name and type of the declaration.
	RtToken getParameterName() const;              //!< \return The name of the declaration.
	RtToken getFullName() const;                   //!< \return The class type cardinality name of the declaration.
	RtInt   getClass() const;                      //!< \return The storage class of the declaration (constant CLAS_...).
	RtInt   getType() const;                       //!< \return The type of the declaration.
	RtInt   getBasicType() const;                  //!< \return The basic type of the declaration, eg. for RtPoint TYPE_FLOAT is returned
	RtInt   getCardinality() const;                //!< \return The cardinality of the declaration.
	RtInt   getComponents(RtInt colorComps) const; //!< \return The number of components of a single element (colorComps if type is color).
	RtInt   getBytes(RtInt colorComps) const;      //!< \return The number of bytes used for an instance of the type.
	//@}

	//! Ask if declaration is a default declaration of the renderer or user defined
	/*! \return true, if the declaration is a renderer default, false otherwise it's user defined
	 */
	bool isDefault() const;

	//! Ask if declaration is an inline declaration
	/*! \return true, if the declaration is an inline declaration, false otherwise
	 */
	bool isInline() const;

	//! Number of elements (elements can be arrays, one array counts 1) of an instance of the declared type
	/*! \param vertices Number of vertices (used by vertex class)
	 *  \param corners Number of corners (used by varying class)
	 *  \param facets Number of facets (used by uniform class)
	 *  \param facevertices Number of vertices per face (used by facevertex class)
	 *  \param facecorners Number of corners per face (used by facevarying class)
	 *  \return Number of elements
	 */
	int getSize(RtInt vertices, RtInt corners, RtInt facets, RtInt faceVertices, RtInt faceCorners) const;

	//! Number of (array) elements of an instance of the declared type
	/*! \param vertices Number of vertices (used by vertex class)
	 *  \param corners Number of corners (used by varying class)
	 *  \param facets Number of facets (used by uniform class)
	 *  \param facevertices Number of vertices per face (used by facevertex class)
	 *  \param facecorners Number of corners per face (used by facevarying class)
	 *  \return Number of the array elements
	 */
	int numElems(RtInt vertices, RtInt corners, RtInt facets, RtInt faceVertices, RtInt faceCorners) const;

	//! Number of components (components are the basic types like RtFloat) of the elements of an instance of the declared type
	/*! \param vertices Number of vertices (used by vertex class)
	 *  \param corners Number of corners (used by varying class)
	 *  \param facets Number of facets (used by uniform class)
	 *  \param facevertices Number of vertices per face (used by facevertex class)
	 *  \param facecorners Number of corners per face (used by facevarying class)
	 *  \return Number of components
	 */
	int numComponents(RtInt vertices, RtInt corners, RtInt facets, RtInt faceVertices, RtInt faceCorners, RtInt colorComps) const;

	//! Number of bytes used for an instance of the declared type
	/*! \param vertices Number of vertices (used by vertex class)
	 *  \param corners Number of corners (used by varying class)
	 *  \param facets Number of facets (used by uniform class)
	 *  \param facevertices Number of vertices per face (used by facevertex class)
	 *  \param facecorners Number of corners per face (used by facevarying class)
	 *  \return Number of bytes
	 */
	int size(RtInt vertices, RtInt corners, RtInt facets, RtInt faceVertices, RtInt faceCorners, RtInt colorComps) const;
}; // TParameterDeclaration

#endif // _RICPP_RENDERCONTEXT_PARAMETERDECLARATION_H
