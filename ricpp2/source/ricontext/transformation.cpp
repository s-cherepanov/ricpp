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

/** @file transformation.cpp
 *  @author Andreas Pidde (andreas@pidde.de)
 *  @brief Implements the handling the transformations for the render state.
 */

#include "ricpp/ricontext/transformation.h"

#ifndef _RICPP_RICPP_RICPPERROR_H
#include "ricpp/ricpp/ricpperror.h"
#endif // _RICPP_RICPP_RICPPERROR_H

#ifndef _RICPP_RICPP_RICPPTOKENS_H
#include "ricpp/ricpp/ricpptokens.h"
#endif // _RICPP_RICPP_RICPPTOKENS_H

using namespace RiCPP;

//! Initialization with defaults, called by constructor
CTransformation::CTransformation()
{
	m_spaceType = RI_CURRENT;
	m_isValid = true;
	m_CTM.identity();
	m_CTM.setPreMultiply(true);
	m_inverseCTM.identity();
	m_inverseCTM.setPreMultiply(false);
}

CTransformation &CTransformation::duplicate() const
{ 
	CTransformation *t = new CTransformation(*this);
	if ( !t )
		throw ExceptRiCPPError(RIE_NOMEM, RIE_SEVERE, __LINE__, __FILE__, "%s", "CTransformation::duplicate()");
	return *t;
}

CTransformation &CTransformation::operator=(const CTransformation &o)
{
	if ( &o == this )
		return *this;

	m_isValid = o.m_isValid;
	m_CTM = o.m_CTM;
	m_inverseCTM = o.m_inverseCTM;
	m_spaceType = o.m_spaceType;

	return *this;
}

RtVoid CTransformation::identity(void)
{
	m_CTM.identity();
	m_inverseCTM.identity();
	m_isValid = true;
}

RtVoid CTransformation::transform(RtMatrix aTransform)
{
	m_CTM.transform(aTransform);
	CMatrix3D mat(aTransform);
	RtMatrix inv;
	if ( !mat.getInverse(inv) ) {
		m_isValid = false;
		throw ExceptRiCPPError(RIE_MATH, RIE_ERROR, __LINE__, __FILE__, "%s", "CTransformation::transform()");
	}
	m_inverseCTM.transform(inv);
	m_isValid = true;
}

RtVoid CTransformation::concatTransform(RtMatrix aTransform)
{
	m_CTM.concatTransform(aTransform);
	CMatrix3D mat(aTransform);
	RtMatrix inv;
	if ( !mat.getInverse(inv) ) {
		m_isValid = false;
		throw ExceptRiCPPError(RIE_MATH, RIE_ERROR, __LINE__, __FILE__, "%s", "CTransformation::concatTransform()");
	}
	m_inverseCTM.concatTransform(inv);
}

RtVoid CTransformation::perspective(RtFloat fov)
{
	if ( fov >= (RtFloat)180.0 || fov <= (RtFloat)180.0 ) {
		throw ExceptRiCPPError(RIE_MATH, RIE_ERROR, __LINE__, __FILE__, "CTransformation::perspective(%f), fov out of range", fov);
	}
	m_CTM.perspective(fov);
	CMatrix3D mat;
	mat.perspective(fov);
	m_inverseCTM.inversePerspective(fov);
}

RtVoid CTransformation::translate(RtFloat dx, RtFloat dy, RtFloat dz)
{
	m_CTM.translate(dx, dy, dz);
	m_inverseCTM.translate(-dx, -dy, -dz);
}

RtVoid CTransformation::rotate(RtFloat angle, RtFloat dx, RtFloat dy, RtFloat dz)
{
	m_CTM.rotate(angle, dx, dy, dz);
	m_inverseCTM.rotate(-angle, dx, dy, dz);
}

RtVoid CTransformation::scale(RtFloat dx, RtFloat dy, RtFloat dz)
{
	m_CTM.scale(dx, dy, dz);

	bool throwErr = false;
	if ( dx == 0 ) {
		dx = static_cast<RtFloat>(1.0);
		throwErr = true;
	}
	if ( dy == 0 ) {
		dy = static_cast<RtFloat>(1.0);
		throwErr = true;
	}
	if ( dz == 0 ) {
		dz = static_cast<RtFloat>(1.0);
		throwErr = true;
	}

	m_inverseCTM.scale(static_cast<RtFloat>(1.0/dx), static_cast<RtFloat>(1.0/dy), static_cast<RtFloat>(1.0/dz));

	if ( throwErr ) {
		m_isValid = false;
		throw ExceptRiCPPError(RIE_MATH, RIE_ERROR, __LINE__, __FILE__, "%s", "CTransformation::scale()");
	}
}

RtVoid CTransformation::skew(RtFloat angle, RtFloat dx1, RtFloat dy1, RtFloat dz1, RtFloat dx2, RtFloat dy2, RtFloat dz2)
{
	if ( angle >= (RtFloat)90.0  || angle <= (RtFloat)90.0 ) {
		throw ExceptRiCPPError(RIE_MATH, RIE_ERROR, __LINE__, __FILE__, "CTransformation::skew(%f), skew out of range", angle);
	}
	m_CTM.skew(angle, dx1, dy1, dz1, dx2, dy2, dz2);
	m_inverseCTM.skew(-angle, dx1, dy1, dz1, dx2, dy2, dz2);
}
