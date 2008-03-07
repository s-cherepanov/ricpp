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

using namespace RiCPP;

// -----------------------------------------------------------------------------

CTransformation::CMovedRotate &CTransformation::CMovedRotate::operator=(const CMovedRotate &o)
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

void CTransformation::CMovedRotate::set(RtFloat angle, RtFloat dx, RtFloat dy, RtFloat dz, RtInt n, unsigned long moBegin, unsigned long moEnd, CMatrix3D &ctm, CMatrix3D &inverse)
{
	assert ( moBegin <= moEnd);
	assert ( (unsigned long)n < moEnd - moBegin );
	
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
		m_rotate[n].m_dz = dz;
	}
	if ( n == 0 )
		sampleReset(ctm, inverse);
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

// -----------------------------------------------------------------------------

CTransformation::CMovedMatrix &CTransformation::CMovedMatrix::operator=(const CTransformation::CMovedMatrix &o)		
{
	if ( this == &o )
		return *this;

	m_transform = o.m_transform;
	m_concat = o.m_concat;
	m_motionBegin = o.m_motionBegin;
	m_motionEnd = o.m_motionEnd;

	return *this;
}

void CTransformation::CMovedMatrix::clear()
{
	m_transform.clear();
}

void CTransformation::CMovedMatrix::fill(RtInt n)
{
	if ( n == 0 ) {
		return;
	}
	for ( unsigned long i = (unsigned long)n; i < m_motionEnd - m_motionBegin; ++i ) {
		for ( unsigned long j = 0; j < 16; ++j ) {
			m_transform[i*16+j] = m_transform[(i-1)*16+j];
		}
	}
}

void CTransformation::CMovedMatrix::set(const RtMatrix transform,
										bool concat,
										RtInt n,
										unsigned long moBegin,
										unsigned long moEnd,
										CMatrix3D &ctm,
										CMatrix3D &inverse)
{
	assert ( moBegin <= moEnd);
	assert ( (unsigned long)n < moEnd - moBegin );
	
	if ( moBegin > moEnd ) {
		std::swap(moBegin, moEnd);		
	}
	
	m_motionBegin = moBegin;
	m_motionEnd = moEnd;
	m_concat = concat;

	if ( moBegin < moEnd ) {
		if ( m_transform.size() < (moEnd - moBegin) * 16 ) {
			m_transform.resize((moEnd - moBegin) * 16);
		}
		if ( (unsigned long)n >= moEnd - moBegin ) {
			// ERROR
			return;
		}
		for ( RtInt j = 0; j < 4; ++j ) {
			m_transform[n*16+j*4+0] = transform[j][0];
			m_transform[n*16+j*4+1] = transform[j][1];
			m_transform[n*16+j*4+2] = transform[j][2];
			m_transform[n*16+j*4+3] = transform[j][3];
		}
	}
	if ( n == 0 )
		sampleReset(ctm, inverse);
	
}

void CTransformation::CMovedMatrix::sample(RtFloat shutterTime, const TypeMotionTimes &times, CMatrix3D &ctm, CMatrix3D &inverse)
{
}

void CTransformation::CMovedMatrix::sampleReset(CMatrix3D &ctm, CMatrix3D &inverse)
{
	RtMatrix transform;
	for ( RtInt j = 0; j < 4; ++j ) {
		transform[j][0] = m_transform[j*4+0];
		transform[j][1] = m_transform[j*4+1];
		transform[j][2] = m_transform[j*4+2];
		transform[j][3] = m_transform[j*4+3];
	}
	
	if ( m_transform.size() >= 1 ) {
		if ( m_concat )
			ctm.concatTransform(transform);
		else
			ctm.transform(transform);

		CMatrix3D mat(transform);
		RtMatrix inv;
		if ( !mat.getInverse(inv) ) {
			inverse.identity();
			throw ExceptRiCPPError(RIE_MATH, RIE_ERROR, __LINE__, __FILE__, "Could not calculate inverse matrix in %s", "CMovedMatrix::sampleReset()");
		}
		if ( m_concat )
			inverse.concatTransform(inv);
		else
			inverse.transform(inv);
	}
}

// -----------------------------------------------------------------------------

CTransformation::CMovedScale &CTransformation::CMovedScale::operator=(const CMovedScale &o)
{
	if ( this == &o )
		return *this;
	
	clear();
	m_scale = o.m_scale;
	m_motionBegin = o.m_motionBegin;
	m_motionBegin = o.m_motionEnd;
	
	return *this;
}

void CTransformation::CMovedScale::clear()
{
	m_scale.clear();
}

void CTransformation::CMovedScale::fill(RtInt n)
{
	// Fill unused values at [n...end]
	if ( n == 0 ) {
		return;
	}
	for ( unsigned long i = (unsigned long)n; i < m_motionEnd - m_motionBegin; ++i ) {
		m_scale[i] = m_scale[i-1];
	}
}

void CTransformation::CMovedScale::set(RtFloat dx, RtFloat dy, RtFloat dz, RtInt n, unsigned long moBegin, unsigned long moEnd, CMatrix3D &ctm, CMatrix3D &inverse)
{
	assert ( moBegin <= moEnd);
	assert ( (unsigned long)n < moEnd - moBegin );
	
	if ( moBegin > moEnd ) {
		std::swap(moBegin, moEnd);		
	}
	
	m_motionBegin = moBegin;
	m_motionEnd = moEnd;
	
	if ( moBegin < moEnd ) {
		if ( m_scale.size() < moEnd - moBegin ) {
			m_scale.resize(moEnd - moBegin);
		}
		if ( (unsigned long)n >= moEnd - moBegin ) {
			// ERROR
			return;
		}
		m_scale[n].m_dx = dx;
		m_scale[n].m_dy = dy;
		m_scale[n].m_dz = dz;
	}
	if ( n == 0 )
		sampleReset(ctm, inverse);
}


void CTransformation::CMovedScale::sample(RtFloat shutterTime, const TypeMotionTimes &times, CMatrix3D &ctm, CMatrix3D &inverse)
{
}

void CTransformation::CMovedScale::sampleReset(CMatrix3D &ctm, CMatrix3D &inverse)
{
	if ( m_scale.size() >= 1 ) {
		ctm.scale(m_scale[0].m_dx, m_scale[0].m_dy, m_scale[0].m_dz);
		RtFloat dx = m_scale[0].m_dx, dy = m_scale[0].m_dy, dz = m_scale[0].m_dz;
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
		inverse.scale((RtFloat)1.0/dx, (RtFloat)1.0/dy, (RtFloat)1.0/dz);
		if ( throwErr ) {
			// ERROR
		}
	}
}

// -----------------------------------------------------------------------------

CTransformation::CMovedTranslate &CTransformation::CMovedTranslate::operator=(const CMovedTranslate &o)
{
	if ( this == &o )
		return *this;
	
	clear();
	m_translate = o.m_translate;
	m_motionBegin = o.m_motionBegin;
	m_motionBegin = o.m_motionEnd;
	
	return *this;
}

void CTransformation::CMovedTranslate::clear()
{
	m_translate.clear();
}

void CTransformation::CMovedTranslate::fill(RtInt n)
{
	// Fill unused values at [n...end]
	if ( n == 0 ) {
		return;
	}
	for ( unsigned long i = (unsigned long)n; i < m_motionEnd - m_motionBegin; ++i ) {
		m_translate[i] = m_translate[i-1];
	}
}

void CTransformation::CMovedTranslate::set(RtFloat dx, RtFloat dy, RtFloat dz, RtInt n, unsigned long moBegin, unsigned long moEnd, CMatrix3D &ctm, CMatrix3D &inverse)
{
	assert ( moBegin <= moEnd);
	assert ( (unsigned long)n < moEnd - moBegin );
	
	if ( moBegin > moEnd ) {
		std::swap(moBegin, moEnd);		
	}
	
	m_motionBegin = moBegin;
	m_motionEnd = moEnd;
	
	if ( moBegin < moEnd ) {
		if ( m_translate.size() < moEnd - moBegin ) {
			m_translate.resize(moEnd - moBegin);
		}
		if ( (unsigned long)n >= moEnd - moBegin ) {
			// ERROR
			return;
		}
		m_translate[n].m_dx = dx;
		m_translate[n].m_dy = dy;
		m_translate[n].m_dz = dz;
	}
	if ( n == 0 )
		sampleReset(ctm, inverse);
}


void CTransformation::CMovedTranslate::sample(RtFloat shutterTime, const TypeMotionTimes &times, CMatrix3D &ctm, CMatrix3D &inverse)
{
}

void CTransformation::CMovedTranslate::sampleReset(CMatrix3D &ctm, CMatrix3D &inverse)
{
	if ( m_translate.size() >= 1 ) {
		ctm.translate(m_translate[0].m_dx, m_translate[0].m_dy, m_translate[0].m_dz);
		inverse.translate(-m_translate[0].m_dx, -m_translate[0].m_dy, -m_translate[0].m_dz);
	}
}

// -----------------------------------------------------------------------------

CTransformation::CMovedPerspective &CTransformation::CMovedPerspective::operator=(const CMovedPerspective &o)
{
	if ( this == &o )
		return *this;
	
	clear();
	m_fov = o.m_fov;
	m_motionBegin = o.m_motionBegin;
	m_motionBegin = o.m_motionEnd;
	
	return *this;
}

void CTransformation::CMovedPerspective::clear()
{
	m_fov.clear();
}

void CTransformation::CMovedPerspective::fill(RtInt n)
{
	// Fill unused values at [n...end]
	if ( n == 0 ) {
		return;
	}
	for ( unsigned long i = (unsigned long)n; i < m_motionEnd - m_motionBegin; ++i ) {
		m_fov[i] = m_fov[i-1];
	}
}

void CTransformation::CMovedPerspective::set(RtFloat fov, RtInt n, unsigned long moBegin, unsigned long moEnd, CMatrix3D &ctm, CMatrix3D &inverse)
{
	assert ( moBegin <= moEnd);
	assert ( (unsigned long)n < moEnd - moBegin );
	
	if ( moBegin > moEnd ) {
		std::swap(moBegin, moEnd);		
	}
	
	m_motionBegin = moBegin;
	m_motionEnd = moEnd;
	
	if ( moBegin < moEnd ) {
		if ( m_fov.size() < moEnd - moBegin ) {
			m_fov.resize(moEnd - moBegin);
		}
		if ( (unsigned long)n >= moEnd - moBegin ) {
			// ERROR
			return;
		}
		m_fov[n] = fov;
	}
	if ( n == 0 )
		sampleReset(ctm, inverse);
}


void CTransformation::CMovedPerspective::sample(RtFloat shutterTime, const TypeMotionTimes &times, CMatrix3D &ctm, CMatrix3D &inverse)
{
}

void CTransformation::CMovedPerspective::sampleReset(CMatrix3D &ctm, CMatrix3D &inverse)
{
	if ( m_fov.size() >= 1 ) {
		if ( m_fov[0] < (RtFloat)180.0 && m_fov[0] > -(RtFloat)180.0 ) {
			ctm.perspective(m_fov[0]);
			inverse.inversePerspective(m_fov[0]);
		} else {
			// ERROR
		}
	}
}

// -----------------------------------------------------------------------------


CTransformation::CMovedSkew &CTransformation::CMovedSkew::operator=(const CMovedSkew &o)
{
	if ( this == &o )
		return *this;
	
	clear();
	m_skew = o.m_skew;
	m_motionBegin = o.m_motionBegin;
	m_motionBegin = o.m_motionEnd;
	
	return *this;
}

void CTransformation::CMovedSkew::clear()
{
	m_skew.clear();
}

void CTransformation::CMovedSkew::fill(RtInt n)
{
	// Fill unused values at [n...end]
	if ( n == 0 ) {
		return;
	}
	for ( unsigned long i = (unsigned long)n; i < m_motionEnd - m_motionBegin; ++i ) {
		m_skew[i] = m_skew[i-1];
	}
}

void CTransformation::CMovedSkew::set(RtFloat angle, RtFloat dx1, RtFloat dy1, RtFloat dz1, RtFloat dx2, RtFloat dy2, RtFloat dz2, RtInt n, unsigned long moBegin, unsigned long moEnd, CMatrix3D &ctm, CMatrix3D &inverse)
{
	assert ( moBegin <= moEnd);
	assert ( (unsigned long)n < moEnd - moBegin );
	
	if ( moBegin > moEnd ) {
		std::swap(moBegin, moEnd);		
	}
	
	m_motionBegin = moBegin;
	m_motionEnd = moEnd;
	
	if ( moBegin < moEnd ) {
		if ( m_skew.size() < moEnd - moBegin ) {
			m_skew.resize(moEnd - moBegin);
		}
		if ( (unsigned long)n >= moEnd - moBegin ) {
			// ERROR
			return;
		}
		m_skew[n].m_angle = angle;
		m_skew[n].m_dx1 = dx1;
		m_skew[n].m_dy1 = dy1;
		m_skew[n].m_dz1 = dz1;
		m_skew[n].m_dx2 = dx2;
		m_skew[n].m_dy2 = dy2;
		m_skew[n].m_dz2 = dz2;
	}
	if ( n == 0 )
		sampleReset(ctm, inverse);
}


void CTransformation::CMovedSkew::sample(RtFloat shutterTime, const TypeMotionTimes &times, CMatrix3D &ctm, CMatrix3D &inverse)
{
}

void CTransformation::CMovedSkew::sampleReset(CMatrix3D &ctm, CMatrix3D &inverse)
{
	if ( m_skew.size() >= 1 ) {
		if ( m_skew[0].m_angle < (RtFloat)90.0  && m_skew[0].m_angle > (RtFloat)-90.0 ) {
			ctm.skew(m_skew[0].m_angle, m_skew[0].m_dx1, m_skew[0].m_dy1, m_skew[0].m_dz1, m_skew[0].m_dx2, m_skew[0].m_dy2, m_skew[0].m_dz2);
			inverse.skew(-m_skew[0].m_angle, m_skew[0].m_dx1, m_skew[0].m_dy1, m_skew[0].m_dz1, m_skew[0].m_dx2, m_skew[0].m_dy2, m_skew[0].m_dz2);
		} else {
			// ERROR
		}
	}
}

// -----------------------------------------------------------------------------

CTransformation::CTransformation()
{
	m_motionState = 0;
	m_storeCounter = 0;
	m_spaceType = RI_CURRENT;
	m_isValid = true;
	m_isValid_onMotionStart = true;
	m_CTM.identity();
	m_CTM.setPreMultiply(true);
	m_inverseCTM.identity();
	m_inverseCTM.setPreMultiply(false);
	// The ...onMotionStart variables are initialized at motionBegin
	// m_CTM_onMotionStart = m_CTM;
	// m_inverseCTM_onMotionStart = m_inverseCTM;
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

	m_isValid_onMotionStart = o.m_isValid_onMotionStart;
	m_CTM_onMotionStart = o.m_CTM_onMotionStart;
	m_inverseCTM_onMotionStart = o.m_inverseCTM_onMotionStart;

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
	if ( !m_motionState ) {
		m_CTM.identity();
		m_inverseCTM.identity();
		m_isValid = true;
	} else {
		if ( m_motionState->curSampleCnt() == 0 ) {
			m_deferedTrans.push_back(new CMovedMatrix);
		}
		assert( !m_deferedTrans.empty() && m_deferedTrans.back() != 0 && m_deferedTrans.back()->reqType() == REQ_TRANSFORM );
		if ( !m_deferedTrans.empty() && m_deferedTrans.back() != 0 && m_deferedTrans.back()->reqType() == REQ_TRANSFORM ) {
			dynamic_cast<CMovedMatrix *>(m_deferedTrans.back())->set(
				RiIdentityMatrix, false,
				m_motionState->curSampleCnt(), m_motionState->firstSampleIdx(), m_motionState->lastSampleIdx(),
				m_CTM, m_inverseCTM);
		} else {
			// ERROR
		}
	}
}

RtVoid CTransformation::transform(RtMatrix aTransform)
{
	if ( !m_motionState ) {
		m_CTM.transform(aTransform);
		CMatrix3D mat(aTransform);
		RtMatrix inv;
		if ( !mat.getInverse(inv) ) {
			m_isValid = false;
			m_inverseCTM.identity();
			throw ExceptRiCPPError(RIE_MATH, RIE_ERROR, __LINE__, __FILE__, "Could not calculate inverse matrix in %s", "CTransformation::transform()");
		}
		m_inverseCTM.transform(inv);
		m_isValid = true;
	} else {
		if ( m_motionState->curSampleCnt() == 0 ) {
			m_deferedTrans.push_back(new CMovedMatrix);
		}
		assert( !m_deferedTrans.empty() && m_deferedTrans.back() != 0 && m_deferedTrans.back()->reqType() == REQ_TRANSFORM );
		if ( !m_deferedTrans.empty() && m_deferedTrans.back() != 0 && m_deferedTrans.back()->reqType() == REQ_TRANSFORM ) {
			dynamic_cast<CMovedMatrix *>(m_deferedTrans.back())->set(
				aTransform, false,
				m_motionState->curSampleCnt(), m_motionState->firstSampleIdx(), m_motionState->lastSampleIdx(),
				m_CTM, m_inverseCTM);
		} else {
			// ERROR
		}
	}
}

RtVoid CTransformation::concatTransform(RtMatrix aTransform)
{
	if ( !m_motionState ) {
		m_CTM.concatTransform(aTransform);
		CMatrix3D mat(aTransform);
		RtMatrix inv;
		if ( !mat.getInverse(inv) ) {
			m_isValid = false;
			throw ExceptRiCPPError(RIE_MATH, RIE_ERROR, __LINE__, __FILE__, "Could not calculate inverse matrix in %s", "CTransformation::concatTransform()");
		}
		m_inverseCTM.concatTransform(inv);
	} else {
		if ( m_motionState->curSampleCnt() == 0 ) {
			m_deferedTrans.push_back(new CMovedMatrix);
		}
		assert( !m_deferedTrans.empty() && m_deferedTrans.back() != 0 && m_deferedTrans.back()->reqType() == REQ_TRANSFORM );
		if ( !m_deferedTrans.empty() && m_deferedTrans.back() != 0 && m_deferedTrans.back()->reqType() == REQ_TRANSFORM ) {
			dynamic_cast<CMovedMatrix *>(m_deferedTrans.back())->set(
				aTransform, true,
				m_motionState->curSampleCnt(), m_motionState->firstSampleIdx(), m_motionState->lastSampleIdx(),
				m_CTM, m_inverseCTM);
		} else {
			// ERROR
		}
	}
}

RtVoid CTransformation::perspective(RtFloat fov)
{
	if ( !m_motionState ) {
		if ( fov >= (RtFloat)180.0 || fov <= -(RtFloat)180.0 ) {
			throw ExceptRiCPPError(RIE_MATH, RIE_ERROR, __LINE__, __FILE__, "CTransformation::perspective(%f), fov out of range", fov);
		}
		m_CTM.perspective(fov);
		m_inverseCTM.inversePerspective(fov);
	} else {
		if ( m_motionState->curSampleCnt() == 0 ) {
			m_deferedTrans.push_back(new CMovedPerspective);
		}
		assert( !m_deferedTrans.empty() && m_deferedTrans.back() != 0 && m_deferedTrans.back()->reqType() == REQ_TRANSLATE );
		if ( !m_deferedTrans.empty() && m_deferedTrans.back() != 0 && m_deferedTrans.back()->reqType() == REQ_TRANSLATE ) {
			dynamic_cast<CMovedPerspective *>(m_deferedTrans.back())->set(
				fov,
				m_motionState->curSampleCnt(), m_motionState->firstSampleIdx(), m_motionState->lastSampleIdx(),
				m_CTM, m_inverseCTM);
		} else {
			// ERROR
		}
	}
}

RtVoid CTransformation::translate(RtFloat dx, RtFloat dy, RtFloat dz)
{
	if ( !m_motionState ) {
		m_CTM.translate(dx, dy, dz);
		m_inverseCTM.translate(-dx, -dy, -dz);
	} else {
		if ( m_motionState->curSampleCnt() == 0 ) {
			m_deferedTrans.push_back(new CMovedTranslate);
		}
		assert( !m_deferedTrans.empty() && m_deferedTrans.back() != 0 && m_deferedTrans.back()->reqType() == REQ_TRANSLATE );
		if ( !m_deferedTrans.empty() && m_deferedTrans.back() != 0 && m_deferedTrans.back()->reqType() == REQ_TRANSLATE ) {
			dynamic_cast<CMovedTranslate *>(m_deferedTrans.back())->set(
				dx, dy, dz,
				m_motionState->curSampleCnt(), m_motionState->firstSampleIdx(), m_motionState->lastSampleIdx(),
				m_CTM, m_inverseCTM);
		} else {
			// ERROR
		}
	}
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
	if ( !m_motionState ) {
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
	} else {
		if ( m_motionState->curSampleCnt() == 0 ) {
			m_deferedTrans.push_back(new CMovedScale);
		}
		assert( !m_deferedTrans.empty() && m_deferedTrans.back() != 0 && m_deferedTrans.back()->reqType() == REQ_SCALE );
		if ( !m_deferedTrans.empty() && m_deferedTrans.back() != 0 && m_deferedTrans.back()->reqType() == REQ_SCALE ) {
			dynamic_cast<CMovedScale *>(m_deferedTrans.back())->set(
				dx, dy, dz,
				m_motionState->curSampleCnt(), m_motionState->firstSampleIdx(), m_motionState->lastSampleIdx(),
				m_CTM, m_inverseCTM);
		} else {
			// ERROR
		}
	}
}

RtVoid CTransformation::skew(RtFloat angle, RtFloat dx1, RtFloat dy1, RtFloat dz1, RtFloat dx2, RtFloat dy2, RtFloat dz2)
{
	if ( !m_motionState ) {
		if ( angle >= (RtFloat)90.0  || angle <= (RtFloat)-90.0 ) {
			throw ExceptRiCPPError(RIE_MATH, RIE_ERROR, __LINE__, __FILE__, "CTransformation::skew(%f), skew out of range", angle);
		}
		m_CTM.skew(angle, dx1, dy1, dz1, dx2, dy2, dz2);
		m_inverseCTM.skew(-angle, dx1, dy1, dz1, dx2, dy2, dz2);
	} else {
		if ( m_motionState->curSampleCnt() == 0 ) {
			m_deferedTrans.push_back(new CMovedSkew);
		}
		assert( !m_deferedTrans.empty() && m_deferedTrans.back() != 0 && m_deferedTrans.back()->reqType() == REQ_SKEW );
		if ( !m_deferedTrans.empty() && m_deferedTrans.back() != 0 && m_deferedTrans.back()->reqType() == REQ_SKEW ) {
			dynamic_cast<CMovedSkew *>(m_deferedTrans.back())->set(
				angle, dx1, dy1, dz1, dx2, dy2, dz2,
				m_motionState->curSampleCnt(), m_motionState->firstSampleIdx(), m_motionState->lastSampleIdx(),
				m_CTM, m_inverseCTM);
		} else {
			// ERROR
		}
	}
}

RtVoid CTransformation::deformation(RtString name, RtInt n, RtToken tokens[], RtPointer params[])
{
}

RtVoid CTransformation::motionBegin(const CMotionState &state)
{
	assert(m_motionState==0);
	m_motionState = &state;
	if ( m_deferedTrans.empty() ) {
		m_isValid_onMotionStart = m_isValid;
		m_CTM_onMotionStart = m_CTM;
		m_inverseCTM_onMotionStart = m_inverseCTM;
	}
}

RtVoid CTransformation::motionEnd()
{
	assert(m_motionState != 0);
	if ( m_motionState != 0 ) {
		if ( !m_deferedTrans.empty() && m_deferedTrans.back() != 0 ) {
			m_deferedTrans.back()->fill(m_motionState->curSampleCnt());
		}	
	}
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
	if ( !m_deferedTrans.empty() ) {
		m_isValid = m_isValid_onMotionStart;
		m_CTM = m_CTM_onMotionStart;
		m_inverseCTM = m_inverseCTM_onMotionStart;
		std::vector<IMovedTransform *>::iterator i = m_deferedTrans.begin();
		for ( ; i != m_deferedTrans.end(); ++i ) {
			(*i)->sampleReset(m_CTM, m_inverseCTM);
		}
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
