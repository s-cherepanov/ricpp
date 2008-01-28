#ifndef _RICPP_RICONTEXT_TRANSFORMATON_H
#define _RICPP_RICONTEXT_TRANSFORMATON_H

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

/** @file transformation.h
 *  @author Andreas Pidde (andreas@pidde.de)
 *  @brief Handling the transformations for the render state.
 */

#ifndef _RICPP_DECLAREATION_TYPES_H
#include "ricpp/ricpp/types.h"
#endif //  _RICPP_DECLAREATION_TYPES_H

namespace RiCPP {

//! Class with the transformation stack and composit transformation matrix
class CTransformation {
	//! Composit transformation matrix
	CMatrix3D m_CTM;

	//! Inverse of the composit transformation matrix
	CMatrix3D m_inverseCTM;

	//! false, did an operation that could not calculate the inverse.
	bool m_isValid;

	//! Space type of the coordinate system (current, world, camera, screen, raster, etc.)
	RtToken m_spaceType;

public:
	//! matrices will be identity.
	CTransformation();

	//! Copy constructor
	inline CTransformation(const CTransformation &rt)
	{
		*this = rt;
	}

	//! Destructor
	inline virtual ~CTransformation() { }

	//! Returns a (deep) copy of this.
	/*! \retrun a copy of this.
	 *  \exception ExceptRiCPPError Thrown if there is not enough memory.
	 */
	virtual CTransformation &duplicate() const;

	//! Assigns instance o to this instance.
	/*! \param o Instance to copy from.
	 *  \return A reference to *this.
	 */
	CTransformation &operator=(const CTransformation &o);

	inline CMatrix3D &getCTM()
	{
		return m_CTM;
	}

	inline virtual const CMatrix3D &getCTM() const
	{
		return m_CTM;
	}

	inline CMatrix3D &getInverseCTM()
	{
		return m_inverseCTM;
	}

	inline virtual const CMatrix3D &getInverseCTM() const
	{
		return m_inverseCTM;
	}

	inline virtual bool isValid() const { return m_isValid; }

	RtVoid identity(void);
	RtVoid transform(RtMatrix aTransform);
	RtVoid concatTransform(RtMatrix aTransform);
	RtVoid perspective(RtFloat fov);
	RtVoid translate(RtFloat dx, RtFloat dy, RtFloat dz);
	RtVoid rotate(RtFloat angle, RtFloat dx, RtFloat dy, RtFloat dz);
	RtVoid scale(RtFloat dx, RtFloat dy, RtFloat dz);
	RtVoid skew(RtFloat angle, RtFloat dx1, RtFloat dy1, RtFloat dz1, RtFloat dx2, RtFloat dy2, RtFloat dz2);

	RtToken spaceType() const { return m_spaceType; }
	void spaceType(RtToken aSpaceType) { m_spaceType = aSpaceType; }
}; // CTransformation

}

#endif // _RICPP_RICONTEXT_TRANSFORMATON_H
