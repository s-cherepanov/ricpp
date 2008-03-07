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

#ifndef _RICPP_RIBASE_RICPPTOKENS_H
#include "ricpp/ribase/ricpptokens.h"
#endif // _RICPP_RIBASE_RICPPTOKENS_H

#ifndef _RICPP_TOOLS_TEMPLATEFUNCS_H
#include "ricpp/tools/templatefuncs.h"
#endif // _RICPP_TOOLS_TEMPLATEFUNCS_H

// #include <iostream>

using namespace RiCPP;

inline CTransformation::CMovedRotate &CTransformation::CMovedRotate::operator=(const CMovedRotate &o)
{
	if ( this == &o )
		return *this;
	
	clear();
	m_rotate = o.m_rotate;
	m_motionBegin = o.m_motionBegin;
	m_motionBegin = o.m_motionEnd;
	
	return *this;
}

void CTransformation::CMovedRotate::clear()
{
	m_rotate.clear();
}

void CTransformation::CMovedRotate::fill(RtInt n)
{
	// Fill unused values at [n...end]
	if ( n == 0 ) {
		return;
	}
	for ( unsigned long i = (unsigned long)n; i < m_motionEnd - m_motionBegin; ++i ) {
		m_rotate[i] = m_rotate[i-1];
	}
}

void CTransformation::CMovedRotate::sample(RtFloat shutterTime, const TypeMotionTimes &times, CMatrix3D &ctm, CMatrix3D &inverse)
{
}

void CTransformation::CMovedRotate::sampleReset(CMatrix3D &ctm, CMatrix3D &inverse)
{
	if ( m_rotate.size() >= 1 ) {
		ctm.rotate(m_rotate[0].m_angle, m_rotate[0].m_dx, m_rotate[0].m_dy, m_rotate[0].m_dz);
		inverse.rotate(-m_rotate[0].m_angle, m_rotate[0].m_dx, m_rotate[0].m_dy, m_rotate[0].m_dz);
	}
}

void CTransformation::CMovedRotate::set(RtFloat angle, RtFloat dx, RtFloat dy, RtFloat dz, RtInt n, unsigned long moBegin, unsigned long moEnd, CMatrix3D &ctm, CMatrix3D &inverse)
{
	assert ( moBegin <= moEnd);
	assert ( n < moEnd - moBegin );
	
	if ( moBegin > moEnd ) {
		std::swap(moBegin, moEnd);		
	}
	
	m_motionBegin = moBegin;
	m_motionEnd = moEnd;
	
	if ( moBegin < moEnd ) {
		if ( m_rotate.size() < moEnd - moBegin ) {
			m_rotate.resize(moEnd - moBegin);
		}
		if ( (unsigned long)n >= moEnd - moBegin ) {
			// ERROR
			return;
		}
		m_rotate[n].m_angle = angle;
		m_rotate[n].m_dx = dx;
		m_rotate[n].m_dy = dy;
		m_rotate[0].m_dz = dz;
		fill(n+1);
	}
	if ( n == 0 )
		sampleReset(ctm, inverse);
	
	// std::cout << "# Rotate Set (" << n << ", " << angle << ", " << dx << ", " << dy << ", " << dz << ")" << std::endl;
}

// -----------------------------------------------------------------------------

CTransformation::CTransformation()
{
	m_motionState = 0;
	m_storeCounter = 0;
	m_spaceType = RI_CURRENT;
	m_isValid = true;
	m_CTM.identity();
	m_CTM.setPreMultiply(true);
	m_inverseCTM.identity();
	m_inverseCTM.setPreMultiply(false);
}

CTransformation::~CTransformation()
{
	// delete_contents(m_deferedTrans);
	for ( std::vector<IMovedTransform *>::iterator i = m_deferedTrans.begin(); i != m_deferedTrans.end(); i++ ) {
		if ( (*i) != 0 )
			delete *i;
	}
	m_deferedTrans.clear();
}

CTransformation *CTransformation::duplicate() const
{ 
	return new CTransformation(*this);
}

CTransformation &CTransformation::operator=(const CTransformation &o)
{
	if ( &o == this )
		return *this;

	m_isValid = o.m_isValid;
	m_CTM = o.m_CTM;
	m_inverseCTM = o.m_inverseCTM;
	m_spaceType = o.m_spaceType;

	// The storeCounter is set within CRenderState()::pushTransform as needed
	m_storeCounter = o.m_storeCounter;
	
	m_motionState = o.m_motionState;

	// delete_contents(m_deferedTrans);
	for ( std::vector<IMovedTransform *>::iterator i = m_deferedTrans.begin(); i != m_deferedTrans.end(); i++ ) {
		if ( (*i) != 0 )
			delete *i;
	}
	m_deferedTrans.clear();

	// push_back_duplicate(m_deferedTrans, o.m_deferedTrans);
	for ( std::vector<IMovedTransform *>::const_iterator j = o.m_deferedTrans.begin(); j != o.m_deferedTrans.end(); j++ ) {
		if ( (*j) != 0 )
			m_deferedTrans.push_back((*j)->duplicate());
		else
			m_deferedTrans.push_back(0);
	}
	
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
		throw ExceptRiCPPError(RIE_MATH, RIE_ERROR, __LINE__, __FILE__, "Could not calculate inverse matrix in %s", "CTransformation::transform()");
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
		throw ExceptRiCPPError(RIE_MATH, RIE_ERROR, __LINE__, __FILE__, "Could not calculate inverse matrix in %s", "CTransformation::concatTransform()");
	}
	m_inverseCTM.concatTransform(inv);
}

RtVoid CTransformation::perspective(RtFloat fov)
{
	if ( fov >= (RtFloat)180.0 || fov <= -(RtFloat)180.0 ) {
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
	if ( !m_motionState ) {
		m_CTM.rotate(angle, dx, dy, dz);
		m_inverseCTM.rotate(-angle, dx, dy, dz);
	} else {
		if ( m_motionState->curSampleCnt() == 0 ) {
			m_deferedTrans.push_back(new CMovedRotate);
		}
		assert( !m_deferedTrans.empty() && m_deferedTrans.back() != 0 && m_deferedTrans.back()->reqType() == REQ_ROTATE );
		if ( !m_deferedTrans.empty() && m_deferedTrans.back() != 0 && m_deferedTrans.back()->reqType() == REQ_ROTATE ) {
			dynamic_cast<CMovedRotate *>(m_deferedTrans.back())->set(
				angle, dx, dy, dz,
				m_motionState->curSampleCnt(), m_motionState->firstSampleIdx(), m_motionState->lastSampleIdx(),
				m_CTM, m_inverseCTM);
		} else {
			// ERROR
		}
	}
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
		throw ExceptRiCPPError(RIE_MATH, RIE_ERROR, __LINE__, __FILE__, "Could not calculate inverse matrix in %s", "CTransformation::scale()");
	}
}

RtVoid CTransformation::skew(RtFloat angle, RtFloat dx1, RtFloat dy1, RtFloat dz1, RtFloat dx2, RtFloat dy2, RtFloat dz2)
{
	if ( angle >= (RtFloat)90.0  || angle <= (RtFloat)-90.0 ) {
		throw ExceptRiCPPError(RIE_MATH, RIE_ERROR, __LINE__, __FILE__, "CTransformation::skew(%f), skew out of range", angle);
	}
	m_CTM.skew(angle, dx1, dy1, dz1, dx2, dy2, dz2);
	m_inverseCTM.skew(-angle, dx1, dy1, dz1, dx2, dy2, dz2);
}

RtVoid CTransformation::motionBegin(const CMotionState &state)
{
	assert(m_motionState==0);
	m_motionState = &state;
}

RtVoid CTransformation::motionEnd()
{
	assert(m_motionState!=0);
	m_motionState = 0;
}

RtVoid CTransformation::motionSuspend()
{
	m_motionState = 0;
}

RtVoid CTransformation::sample(RtFloat shutterTime, const TypeMotionTimes &times)
{
	std::vector<IMovedTransform *>::iterator i = m_deferedTrans.begin();
	for ( ; i != m_deferedTrans.end(); ++i ) {
		(*i)->sample(shutterTime, times, m_CTM, m_inverseCTM);
	}
}

RtVoid CTransformation::sampleReset()
{
	std::vector<IMovedTransform *>::iterator i = m_deferedTrans.begin();
	for ( ; i != m_deferedTrans.end(); ++i ) {
		(*i)->sampleReset(m_CTM, m_inverseCTM);
	}
}


// ----------------------------------------------------------------------------

CTransformation *CTransformationFactory::newTransformation()
{
	CTransformation *trans = newTransformationInstance();
	if ( !trans ) {
		throw ExceptRiCPPError(RIE_NOMEM, RIE_SEVERE, "in newTransformation()", __LINE__, __FILE__);
	}
	return trans;
}

CTransformation *CTransformationFactory::newTransformation(const CTransformation &trans)
{
	CTransformation *transPtr = newTransformationInstance(trans);
	if ( !transPtr ) {
		throw ExceptRiCPPError(RIE_NOMEM, RIE_SEVERE, "in newTransformation(const CTransformation &)", __LINE__, __FILE__);
	}
	return transPtr;
}
