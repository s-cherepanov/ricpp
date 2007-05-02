#ifndef _RICPP_RENDERCONTEXT_PARAMETER_H
#define _RICPP_RENDERCONTEXT_PARAMETER_H

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

/*! \file parameter.h
 *  \brief Declaration of a container for one parameter (array of values)
 *         of the parameterlist of an interface call.
 *  \author Andreas Pidde (andreas@pidde.de)
 */

#include "parameterdeclaration.h"
#include "tools/helpers.h"

////////////////////////////////////////////////////////////////////////////////
//! Holds the declaration and value of an interface parameter
/*! Stores all data needed to specify one parameter. The number of faces, corners
 *  etc. can be determined by using the TParameterClasses classes found in
 *  the file paramclasses.h.
 */
//
class TParameter
{
	TParameterDeclaration  m_decl;          //!< Parameter declaration, inline or previous declared and stored
	                                        //!< in TDeclarationMap of the renderer context, s.a. m_isInline.
	                                        //!< Also the name of the parameters is stored in this declaration.
	                                        //!< At the moment, the declaration is stored completely for both
	                                        //!< types of declarations.
	bool		           m_isInline;      //!< Declaration from an "Inline" declaration.

	RtInt				   m_vertices;      //!< Number of vertices in primitive (vertex class).
	RtInt				   m_corners;       //!< Number of corners in primitive (varying class).
	RtInt				   m_facets;        //!< Number of facets in primitive (uniform class).
	RtInt				   m_faceVertices;  //!< Number of vertices per face in primitive (facevertex class).
	RtInt				   m_faceCorners;   //!< Number of corners per face in primitive (facevarying class).

	RtInt				   m_colorComps;    //!< Number of color components used for RtColor type.

	unsigned long          m_position;      //!< Original position in parameter list.
	unsigned long          m_valueSize;	    //!< Size of m_value (number of bytes).
	unsigned long          m_stringsSize;   //!< Size of m_strings (number of bytes).

	unsigned char         *m_value;		    //!< Copy of the value of a parameter as byte vector.
	char                  *m_strings;       //!< Copy of the values of a RtString parameter.
public:
	TParameter();                       //!< Initializes an empty parameter.
	TParameter(const TParameter &p);    //!< Copy constructor.
	~TParameter();                      //!< Erases all data, frees memory.

	TParameter &duplicate() const;              //!< Duplicates *this an returns reference to new instance.
	TParameter &operator=(const TParameter &);  //!< Assignment of a TParameter, performs a deep copy.

	void releaseAll();                  //!< Erases all data and frees memory.

	bool set(
		const TParameterDeclaration *declaration,
		RtInt vertices, RtInt corners, RtInt facets,
		RtInt faceVertices, RtInt faceCorners,
		RtInt colorComps,
		RtInt pos,
		const char *type,
		const RtPointer values);        //!< Enters data into an instance of TParameter.

	const RtPointer getValue() const;   //!< Returns a pointer to the value of the parameter.
	const RtPointer getValue(IndexType pos) const;   //!< Returns a pointer to the value of the parameter at position pos.
	const char *getName() const;           //!< Returns the name of the parameter.
	const char *getParameterName() const;  //!< Returns the name of the parameter (without class and type).
	bool isInline() const;              //!< Returns whether the declaration is inline or not.

	RtInt getVertices() const;          //!< Returns the number of vertices used.
	RtInt getCorners() const;           //!< Returns the number of corners used.
	RtInt getFacets() const;            //!< Returns the number of facets used.
	RtInt getFaceVertices() const;      //!< Returns the number of vertices per face used.
	RtInt getFaceCorners() const;       //!< Returns the number of corners per face used.
	RtInt getColorComps() const;        //!< Returns the numer of color components.

	unsigned long getPosition() const;          //!< Returns the Position of the paramter in the parameter list.
	unsigned long getNumberOfElements() const;  //!< Returns the number of elements (depending on cardinality and class of the declaration), each array element counts 1.
	unsigned long getSize() const;              //!< Returns the number of elements (class of the declaration), arrays counts 1 as a whole.
	unsigned long getBasicSize() const;         //!< Returns the number of all basic elements, i.e. (getCardinality()*getComponents()*getSize()) or (getComponents()*getNumberOfElements())

    ////////////////////////////////////////////////////////////////////////////////
    //!< Gets the declaration of the parameter to retreive component size, cardinality etc.
	/*! A pointer is returned, because maybe the implementation is changed
	 *  to host a pointer of a non inline declaration instead of the whole
	 * declaration in m_decl to save memory.
	 *
	 * \return A pointer the declared type of the parameter.
	 */
	inline const TParameterDeclaration *getDeclaration() const {
		return &m_decl;
	}


    ////////////////////////////////////////////////////////////////////////////////
    //!< Gets the size of 1 basic element (e.g. RtFloat)
	/*!
	 * \return Size of one element
	 */
	inline RtInt getElemSize() const {
		return m_decl.sizeOfType(m_decl.getBasicType());
	}

    ////////////////////////////////////////////////////////////////////////////////
    //!< Gets the cardinality if parameter is an array, 1 if it's not.
	/*!
	 * \return Cardinality
	 */
	inline RtInt getCardinality() const {
		return m_decl.getCardinality();
	}

    ////////////////////////////////////////////////////////////////////////////////
    //!< Gets the number of components of an element (e.g. 3 for 3D Points)
	/*!
	 * \return Components
	 */
	inline RtInt getComponents() const {
		return m_decl.getComponents(getColorComps());
	}

    ////////////////////////////////////////////////////////////////////////////////
    //!< Gets the type of the elements
	/*!
	 * \return Type
	 */
	inline RtInt getBasicType() const {
		return m_decl.getBasicType();
	}

    ////////////////////////////////////////////////////////////////////////////////
    //!< Gets the class of the elements
	/*!
	 * \return Classe
	 */
	inline RtInt getClass() const {
		return m_decl.getClass();
	}

	////////////////////////////////////////////////////////////////////////////////
    //!< Looks if parameter is of a "face" class
	/*!
	 * \return Classe
	 */
	inline bool isFaceClass() const {
		return (m_decl.getClass() == CLASS_FACEVARYING) || (m_decl.getClass() == CLASS_FACEVERTEX);
	}

	////////////////////////////////////////////////////////////////////////////////
    //!< Copies the content of a parameter to a TNamedVector
	/*!
	 * \param vec The vector to copy the values to
	 * \return true: copy is done (types where correct), false: copy not possible
	 */
	bool copyToNamedVector(TNamedVector<RtFloat> &vec) const;

	////////////////////////////////////////////////////////////////////////////////
    //!< Assigns the data pointer of a TNamedVector to the parameter data
	/*!
	 * \param vec The vector to copy the reference to
	 * \return true: assignment done (types where correct), false: assignment not possible
	 */
	bool referNamedVectorTo(TNamedVector<RtFloat> &vec) const;
}; // TParameter

#endif // _RICPP_RENDERCONTEXT_PARAMETER_H
