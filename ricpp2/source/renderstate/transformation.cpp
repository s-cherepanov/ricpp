#include "ricpp/renderstate/transformation.h"

#ifndef _RICPP_RICPP_RICPPERROR_H
#include "ricpp/ricpp/ricpperror.h"
#endif // _RICPP_RICPP_RICPPERROR_H

using namespace RiCPP;

//! Initialization with defaults, called by constructor
CTransformation::CTransformation()
{
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
	m_CTM.perspective(fov);
	CMatrix3D mat;
	mat.perspective(fov);
	RtMatrix inv;
	if ( !mat.getInverse(inv) ) {
		m_isValid = false;
		throw ExceptRiCPPError(RIE_MATH, RIE_ERROR, __LINE__, __FILE__, "%s", "CTransformation::perspective()");
	}
	m_inverseCTM.concatTransform(inv);
}

RtVoid CTransformation::translate(RtFloat dx, RtFloat dy, RtFloat dz)
{
	m_CTM.translate(dx, dy, dz);
	m_inverseCTM.translate(dx, dy, dz);
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
	m_CTM.skew(angle, dx1, dy1, dz1, dx2, dy2, dz2);
	m_inverseCTM.skew(-angle, dx1, dy1, dz1, dx2, dy2, dz2);
}
