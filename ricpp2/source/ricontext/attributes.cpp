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

/** @file attributes.cpp
 *  @author Andreas Pidde (andreas@pidde.de)
 *  @brief Implements the handling of the render attributes (used by render state).
 */

#include "ricpp/ricontext/attributes.h"

#ifndef _RICPP_RICPP_RICPPERROR_H
#include "ricpp/ricpp/ricpperror.h"
#endif // _RICPP_RICPP_RICPPERROR_H

using namespace RiCPP;

void CAttributes::CAttributeInt::sample(RtFloat shutterTime, const TypeMotionTimes &times)
{
}

void CAttributes::CAttributeToken::sample(RtFloat shutterTime, const TypeMotionTimes &times)
{
}

void CAttributes::CAttributeFloat::sample(RtFloat shutterTime, const TypeMotionTimes &times)
{
}

void CAttributes::CAttributeTrimCurve::sample(RtFloat shutterTime, const TypeMotionTimes &times)
{
}

// -----------------------------------------------------------------------------

void CAttributes::CAttributeBasis::sample(RtFloat shutterTime, const TypeMotionTimes &times)
{
}

void CAttributes::CAttributeBasis::set(const RtBasis basis, RtInt step, RtInt n, unsigned long moBegin, unsigned long moEnd)
{
	if ( moBegin > moEnd ) {
		std::swap(moBegin, moEnd);		
	}
	
	m_motionBegin = moBegin;
	m_motionEnd = moEnd;
	
	if ( n == 0 ) {
		memcpy(m_value.m_basis, basis, sizeof(RtBasis));
		m_value.m_step = step;
	}
	
	if ( moBegin < moEnd ) {
		if ( m_movedValue.size() < moEnd - moBegin ) {
			m_movedValue.resize(moEnd - moBegin);
		}
		if ( (unsigned long)n >= moEnd - moBegin ) {
			// ERROR
			return;
		}
		memcpy(m_movedValue[n].m_basis, basis, sizeof(RtBasis));
		m_movedValue[n].m_step = step;
	}
}

void CAttributes::CAttributeBasis::set(const RtBasis basis, RtInt step)
{
	clear();
	memcpy(m_value.m_basis, basis, sizeof(RtBasis));
	m_value.m_step = step;
}


// -----------------------------------------------------------------------------

void CAttributes::CAttributeShader::sample(RtFloat shutterTime, const TypeMotionTimes &times)
{
}

void CAttributes::CAttributeShader::sampleReset()
{
	if ( m_movedValue.size() >= 1 )
		m_value = m_movedValue[0];

	if ( m_shaderTransform )
		m_shaderTransform->sampleReset();
}

void CAttributes::CAttributeShader::set(RtToken name, const CParameterList &params, const CTransformation &transform, RtInt n, unsigned long moBegin, unsigned long moEnd)
{
	if ( moBegin > moEnd ) {
		std::swap(moBegin, moEnd);		
	}

	m_motionBegin = moBegin;
	m_motionEnd = moEnd;

	if ( n == 0 ) {
		m_value.m_name = name;
		m_value.m_params = params;
		CTransformationFactory::deleteTransformation(m_shaderTransform);
		m_shaderTransform = transform.duplicate();
		if ( m_shaderTransform ) {
			m_shaderTransform->spaceType(RI_WORLD); // Shaders are attributes and therefore in world space
		}
	}

	if ( moBegin < moEnd ) {
		if ( m_movedValue.size() < moEnd - moBegin ) {
			m_movedValue.resize(moEnd - moBegin);
		}
		if ( (unsigned long)n >= moEnd - moBegin ) {
			// ERROR
			return;
		}
		m_movedValue[n].m_name = name;
		m_movedValue[n].m_params = params;
	}
}

void CAttributes::CAttributeShader::set(RtToken name, const CParameterList &params, const CTransformation &transform)
{
	clear();
	m_value.m_name = name;
	m_value.m_params = params;
	CTransformationFactory::deleteTransformation(m_shaderTransform);
	m_shaderTransform = transform.duplicate();
	if ( m_shaderTransform ) {
		m_shaderTransform->spaceType(RI_WORLD);
	}
}

// -----------------------------------------------------------------------------

void CAttributes::CAttributeFloatArray::sample(RtFloat shutterTime, const TypeMotionTimes &times)
{
}

void CAttributes::CAttributeFloatArray::sampleReset()
{
	if ( (RtInt)m_movedValue.size() >= m_value.size() ) {
		m_value.assign(m_movedValue.begin(), m_movedValue.begin() + m_value.size());
	}
}

CAttributes::CAttributeFloatArray &CAttributes::CAttributeFloatArray::operator=(const CAttributes::CAttributeFloatArray &c)
{
	if ( this == &c )
		return *this;
	clear();
	m_value = c.m_value;
	m_movedValue = c.m_movedValue;
	m_motionBegin = c.m_motionBegin;
	m_motionEnd = c.m_motionEnd;
	return *this;
}

void CAttributes::CAttributeFloatArray::clear()
{
	m_value.clear();
	m_movedValue.clear();
	m_motionBegin = 0;
	m_motionEnd = 0;
}

void CAttributes::CAttributeFloatArray::fill(RtInt n)
{
	// Fill unused values at [n...end]
	if ( n == 0 ) {
		return;
	}
	for ( unsigned long i = (unsigned long)n; i < m_motionEnd - m_motionBegin; ++i ) {
		for ( std::vector<RtFloat>::size_type j = 0; j < m_value.size(); ++j ) {
			m_movedValue[i * m_value.size() + j] = m_movedValue[(i - 1) * m_value.size() + j];
		}
	}
}

void CAttributes::CAttributeFloatArray::set(RtFloat aValue, RtInt n, unsigned long moBegin, unsigned long moEnd)
{
	assert ( moBegin <= moEnd);
	
	if ( moBegin > moEnd ) {
		std::swap(moBegin, moEnd);		
	}
	
	m_motionBegin = moBegin;
	m_motionEnd = moEnd;

	if ( n == 0 ) {
		m_value.assign(m_value.size(), aValue);
	}

	if ( moBegin < moEnd ) {
		if ( m_movedValue.size() < (moEnd - moBegin) * m_value.size() ) {
			m_movedValue.resize((moEnd - moBegin) * m_value.size());
		}
		if ( (unsigned long)n >= moEnd - moBegin ) {
			// ERROR
			return;
		}
		for ( std::vector<RtFloat>::size_type i = 0; i < m_value.size(); ++i )
			m_movedValue[n * m_value.size() + i] = aValue;
	}
}

void CAttributes::CAttributeFloatArray::set(RtFloat *aValue, RtInt n, unsigned long moBegin, unsigned long moEnd)
{
	assert ( moBegin <= moEnd);
	
	if ( moBegin > moEnd ) {
		std::swap(moBegin, moEnd);		
	}
	
	m_motionBegin = moBegin;
	m_motionEnd = moEnd;

	if ( n == 0 ) {
		m_value.assign(aValue, aValue + m_value.size());
	}

	if ( moBegin < moEnd ) {
		if ( m_movedValue.size() < (moEnd - moBegin) * m_value.size() ) {
			m_movedValue.resize((moEnd - moBegin) * m_value.size());
		}
		if ( (unsigned long)n >= moEnd - moBegin ) {
			// ERROR
			return;
		}
		for ( std::vector<RtFloat>::size_type i = 0; i < m_value.size(); ++i )
			m_movedValue[n * m_value.size() + i] = aValue[i];
	}
}

void CAttributes::CAttributeFloatArray::set(RtFloat aValue, RtInt aCard)
{
	m_motionBegin = 0;
	m_motionEnd = 0;
	m_movedValue.clear();
	
	if ( aCard <= 0 ) {
		m_value.resize(0);
		return;
	}
	
	m_value.resize(aCard);
	m_value.assign(aCard, aValue);
}

void CAttributes::CAttributeFloatArray::set(RtFloat aValue)
{
	m_motionBegin = 0;
	m_motionEnd   = 0;
	m_movedValue.clear();
	
	m_value.assign(m_value.size(), aValue);
}

void CAttributes::CAttributeFloatArray::set(RtFloat *aValue, RtInt aCard)
{
	m_motionBegin = 0;
	m_motionEnd = 0;
	m_movedValue.clear();

	if ( aCard <= 0 ) {
		m_value.resize(0);
		return;
	}
	
	m_value.resize(aCard);
	m_value.assign(aValue, aValue + m_value.size());
}

void CAttributes::CAttributeFloatArray::set(RtFloat *aValue)
{
	m_motionBegin = 0;
	m_motionEnd   = 0;
	m_movedValue.clear();

	m_value.assign(aValue, aValue + m_value.size());
}

bool CAttributes::CAttributeFloatArray::get(RtFloat *aValue) const
{
	for ( size_t i = 0; i < m_value.size(); ++i )
		aValue[i] = m_value[i];

	return !m_value.empty();
}

// -----------------------------------------------------------------------------

const RtFloat CAttributes::defColorComponent = (RtFloat)1.0;
const RtFloat CAttributes::defOpacityComponent = (RtFloat)1.0;

const RtFloat CAttributes::defTextureS1 = 0;
const RtFloat CAttributes::defTextureT1 = 0;
const RtFloat CAttributes::defTextureS2 = 1;
const RtFloat CAttributes::defTextureT2 = 0;
const RtFloat CAttributes::defTextureS3 = 0;
const RtFloat CAttributes::defTextureT3 = 1;
const RtFloat CAttributes::defTextureS4 = 1;
const RtFloat CAttributes::defTextureT4 = 1;

const RtFloat CAttributes::defShadingRate = 1;
// RtToken const CAttributes::defShadingInterpolation = RI_CONSTANT;
const RtBoolean CAttributes::defMatte = RI_FALSE;

const RtFloat CAttributes::defMinVis  = 0;
const RtFloat CAttributes::defLowTran = 0;
const RtFloat CAttributes::defUpTran  = (RtFloat)(1.0e38);
const RtFloat CAttributes::defMaxVis  = (RtFloat)(1.0e38);

// RtToken const CAttributes::defGeometricApproximationType = RI_NULL;
const RtFloat CAttributes::defGeometricApproximationValue = 0.5;

// RtToken const CAttributes::defGeometricRepresentation = RI_PRIMITIVE;

// RtToken const CAttributes::defOrientation = RI_OUTSIDE;

const RtInt CAttributes::defNSides = 2;

COptionsBase *CAttributes::duplicate() const
{ 
	if ( m_factory )
		return m_factory->newAttributes(*this);
	return new CAttributes(*this);
}

void CAttributes::init()
{
	m_factory = 0;
	m_motionState = 0;

	
	initAttributeVector();
	m_storeCounter = 0;
	m_lastValue = AIDX_ENDMARKER;

	m_illuminated.clear();
	m_inAreaLight = false;
	
	initColor();
	initOpacity();
	initTextureCoordinates();
	initSurface();
	initAtmosphere();
	initInterior();
	initExterior();
	initDisplacement();
	initDeformation();
	initShadingRate();
	initShadingInterpolation();
	initMatte();
	initBound();
	initDetail();
	initDetailRange();
	initGeometricApproximation();
	initGeometricRepresentation();
	initOrientation();
	initSides();
	initBasis();

	initTrimCurve();
}

CAttributes &CAttributes::operator=(const CAttributes &ra)
{
	if ( this == &ra )
		return *this;
	
	m_factory = ra.m_factory;

	m_color = ra.m_color;
	m_opacity = ra.m_opacity;

	m_illuminated = ra.m_illuminated;
	m_inAreaLight = ra.m_inAreaLight;

	m_surface = ra.m_surface;
	m_atmosphere = ra.m_atmosphere;
	m_interior = ra.m_interior;
	m_exterior = ra.m_exterior;
	m_displacement = ra.m_displacement;
	m_deformation = ra.m_deformation;

	m_textureCoordinates = ra.m_textureCoordinates;

	m_shadingRate = ra.m_shadingRate;
	m_shadingInterpolation = ra.m_shadingInterpolation;
	m_matte = ra.m_matte;

	m_bound = ra.m_bound;
	m_boundCalled = ra.m_boundCalled;

	m_detail = ra.m_detail;
	m_detailCalled = ra.m_detailCalled;

	// Detail range only affects the attribute block where it was defined
	m_detailRange = ra.m_detailRange;
	m_detailRangeCalled = ra.m_detailRangeCalled;

	// This value have to be set to false if a new attribute/frame/world block is started (done by CRenderState::pushAttributes(false)).
	m_detailRangeCalledInBlock = ra.m_detailRangeCalledInBlock;

	m_geometricApproximationType = ra.m_geometricApproximationType;
	m_geometricApproximationValue = ra.m_geometricApproximationValue;

	m_geometricRepresentation = ra.m_geometricRepresentation;

	m_orientation = ra.m_orientation;
	m_coordSysOrientation = ra.m_coordSysOrientation;
	m_primitiveOrientation = ra.m_primitiveOrientation;

	m_nSides = ra.m_nSides;

	m_uBasis = ra.m_uBasis;
	m_vBasis = ra.m_vBasis;
	
	m_trimCurve = ra.m_trimCurve;

	m_motionState = ra.m_motionState;

	// The storeCounter is set within CRenderState()::pushAttributes as needed
	m_storeCounter = ra.m_storeCounter;
	
	m_lastValue = ra.m_lastValue;
	
	COptionsBase::operator=(ra);

	return *this;
}

void CAttributes::initAttributeVector()
{
	m_allAttributes.resize((int)AIDX_ENDMARKER);

	m_allAttributes[(int)AIDX_COLOR] = &m_color;
	m_allAttributes[(int)AIDX_OPACITY] = &m_opacity;
	m_allAttributes[(int)AIDX_TEXTURE_COORDINATES] = &m_textureCoordinates;
	m_allAttributes[(int)AIDX_SHADING_RATE] = &m_shadingRate;
	m_allAttributes[(int)AIDX_BOUND] = &m_bound;
	m_allAttributes[(int)AIDX_DETAIL] = &m_detail;
	m_allAttributes[(int)AIDX_DETAIL_RANGE] = &m_detailRange;
	m_allAttributes[(int)AIDX_GEOMETRIC_APPROXIMATION_VALUE] = &m_geometricApproximationValue;

	m_allAttributes[(int)AIDX_SURFACE] = &m_surface;
	m_allAttributes[(int)AIDX_ATMOSPHERE] = &m_atmosphere;
	m_allAttributes[(int)AIDX_INTERIOR] = &m_interior;
	m_allAttributes[(int)AIDX_EXTERIOR] = &m_exterior;
	m_allAttributes[(int)AIDX_DISPLACEMENT] = &m_displacement;
	m_allAttributes[(int)AIDX_DEFORMATION] = &m_deformation;

	m_allAttributes[(int)AIDX_SHADING_INTERPOLATION] = &m_shadingInterpolation;
	m_allAttributes[(int)AIDX_GEOMETRIC_APPROXIMATION_TYPE] = &m_geometricApproximationType;
	m_allAttributes[(int)AIDX_GEOMETRIC_REPRESENTATION] = &m_geometricRepresentation;
	m_allAttributes[(int)AIDX_ORIENTATION] = &m_orientation;
	
	m_allAttributes[(int)AIDX_MATTE] = &m_matte;
	m_allAttributes[(int)AIDX_SIDES] = &m_nSides;

	m_allAttributes[(int)AIDX_BASIS] = &m_uBasis;
	
	m_allAttributes[(int)AIDX_TRIM_CURVE] = &m_trimCurve;
}

bool CAttributes::inAreaLight() const
{
	return m_inAreaLight;
}

void CAttributes::inAreaLight(bool flag)
{
	m_inAreaLight = flag;
}

void CAttributes::initColor()
{
	m_color.set(defColorComponent, colorSamples());
}

RtVoid CAttributes::color(RtColor Cs)
{
	if ( m_motionState != 0 ) {
		m_color.set(Cs, m_motionState->curSampleIdx(), m_motionState->firstSampleIdx(), m_motionState->lastSampleIdx());
		m_lastValue = AIDX_COLOR;
	} else {
		m_color.set(Cs);
	}
	dirty(true);
}

RtFloat CAttributes::color(RtInt i) const
{
	if ( i < 0 )
		return 0;

	if ( (unsigned long)i >= m_color.m_value.size() )
		return 0;

	return m_color.m_value[i];
}

bool CAttributes::getColor(RtColor Cs) const
{

	if ( colorSamples() >= (RtInt)m_color.m_value.size() )
		return false;

	return m_color.get(&Cs[0]);
}

void CAttributes::initOpacity()
{
	m_opacity.set(defOpacityComponent, colorSamples());
}

RtVoid CAttributes::opacity(RtColor Os)
{
	if ( m_motionState != 0 ) {
		m_opacity.set(Os, m_motionState->curSampleIdx(), m_motionState->firstSampleIdx(), m_motionState->lastSampleIdx());
		m_lastValue = AIDX_OPACITY;
	} else {
		m_opacity.set(Os);
	}
	dirty(true);
}

RtFloat CAttributes::opacity(RtInt i) const
{
	if ( i < 0 )
		return 0;

	if ( (unsigned long)i >= m_opacity.m_value.size() )
		return 0;

	return m_opacity.m_value[i];
}

bool CAttributes::getOpacity(RtColor Os) const
{

	if ( colorSamples() >= (RtInt)m_opacity.m_value.size() )
		return false;

	return m_opacity.get(&Os[0]);
}

RtVoid CAttributes::illuminate(CLightSource *light, RtBoolean onoff)
{
	TypeLightHandles::iterator i = find(m_illuminated.begin(), m_illuminated.end(), light);
	if ( onoff ) {
		if ( i == m_illuminated.end() ) {
			m_illuminated.push_back(light);
		}
	} else {
		if ( i != m_illuminated.end() ) {
			m_illuminated.erase(i);
		}
	}
	dirty(true);
}

RtBoolean CAttributes::illuminated(CLightSource *light) const
{
	TypeLightHandles::const_iterator i = find(m_illuminated.begin(), m_illuminated.end(), light);
	return (i != m_illuminated.end()) ? RI_TRUE : RI_FALSE;
}

void CAttributes::initSurface()
{
	// None
}

RtVoid CAttributes::surface(RtToken name, const CParameterList &params, const CTransformation &transform)
{
	if ( m_motionState != 0 ) {
		m_surface.set(name, params, transform, m_motionState->curSampleIdx(), m_motionState->firstSampleIdx(), m_motionState->lastSampleIdx());
		m_lastValue = AIDX_SURFACE;
	} else {
		m_surface.set(name, params, transform);
	}
	dirty(true);
}

void CAttributes::initAtmosphere()
{
	// None
}

RtVoid CAttributes::atmosphere(RtToken name, const CParameterList &params, const CTransformation &transform)
{
	if ( m_motionState != 0 ) {
		m_atmosphere.set(name, params, transform, m_motionState->curSampleIdx(), m_motionState->firstSampleIdx(), m_motionState->lastSampleIdx());
		m_lastValue = AIDX_ATMOSPHERE;
	} else {
		m_atmosphere.set(name, params, transform);
	}
	dirty(true);
}

void CAttributes::initInterior()
{
	// None
}

RtVoid CAttributes::interior(RtToken name, const CParameterList &params, const CTransformation &transform)
{
	if ( m_motionState != 0 ) {
		m_interior.set(name, params, transform, m_motionState->curSampleIdx(), m_motionState->firstSampleIdx(), m_motionState->lastSampleIdx());
		m_lastValue = AIDX_INTERIOR;
	} else {
		m_interior.set(name, params, transform);
	}
	dirty(true);
}

void CAttributes::initExterior()
{
	// None
}

RtVoid CAttributes::exterior(RtToken name, const CParameterList &params, const CTransformation &transform)
{
	if ( m_motionState != 0 ) {
		m_exterior.set(name, params, transform, m_motionState->curSampleIdx(), m_motionState->firstSampleIdx(), m_motionState->lastSampleIdx());
		m_lastValue = AIDX_EXTERIOR;
	} else {
		m_exterior.set(name, params, transform);
	}
	dirty(true);
}

void CAttributes::initDisplacement()
{
	// None
}

RtVoid CAttributes::displacement(RtToken name, const CParameterList &params, const CTransformation &transform)
{
	if ( m_motionState != 0 ) {
		m_displacement.set(name, params, transform, m_motionState->curSampleIdx(), m_motionState->firstSampleIdx(), m_motionState->lastSampleIdx());
		m_lastValue = AIDX_DISPLACEMENT;
	} else {
		m_displacement.set(name, params, transform);
	}
	dirty(true);
}

void CAttributes::initDeformation()
{
	// None
}

RtVoid CAttributes::deformation(RtToken name, const CParameterList &params, const CTransformation &transform)
{
	if ( m_motionState != 0 ) {
		m_deformation.set(name, params, transform, m_motionState->curSampleIdx(), m_motionState->firstSampleIdx(), m_motionState->lastSampleIdx());
		m_lastValue = AIDX_DEFORMATION;
	} else {
		m_deformation.set(name, params, transform);
	}
	dirty(true);
}

RtVoid CAttributes::initTextureCoordinates()
{
	RtFloat tc[8];
	tc[0] = defTextureS1;
	tc[1] = defTextureT1;
	tc[2] = defTextureS2;
	tc[3] = defTextureT2;
	tc[4] = defTextureS3;
	tc[5] = defTextureT3;
	tc[6] = defTextureS4;
	tc[7] = defTextureT4;
	m_textureCoordinates.set(tc, 8);
}

RtVoid CAttributes::textureCoordinates(RtFloat s1, RtFloat t1, RtFloat s2, RtFloat t2, RtFloat s3, RtFloat t3, RtFloat s4, RtFloat t4)
{
	RtFloat tc[8];
	tc[0] = s1;
	tc[1] = t1;
	tc[2] = s2;
	tc[3] = t2;
	tc[4] = s3;
	tc[5] = t3;
	tc[6] = s4;
	tc[7] = t4;

	if ( m_motionState != 0 ) {
		m_textureCoordinates.set(tc, m_motionState->curSampleIdx(), m_motionState->firstSampleIdx(), m_motionState->lastSampleIdx());
		m_lastValue = AIDX_TEXTURE_COORDINATES;
	} else {
		m_textureCoordinates.set(tc);
	}
	dirty(true);
}

bool CAttributes::getTextureCoordinates(RtFloat &s1, RtFloat &t1, RtFloat &s2, RtFloat &t2, RtFloat &s3, RtFloat &t3, RtFloat &s4, RtFloat &t4) const
{
	if ( 8 >= m_textureCoordinates.m_value.size() )
		return false;

	s1 = m_textureCoordinates.m_value[0];
	t1 = m_textureCoordinates.m_value[1];
	s2 = m_textureCoordinates.m_value[2];
	t2 = m_textureCoordinates.m_value[3];
	s3 = m_textureCoordinates.m_value[4];
	t3 = m_textureCoordinates.m_value[5];
	s4 = m_textureCoordinates.m_value[6];
	t4 = m_textureCoordinates.m_value[7];

	return true;
}


void CAttributes::initShadingRate()
{
	m_shadingRate.set(defShadingRate);
}

RtVoid CAttributes::shadingRate(RtFloat size)
{
	if ( m_motionState != 0 ) {
		m_shadingRate.set(size, m_motionState->curSampleIdx(), m_motionState->firstSampleIdx(), m_motionState->lastSampleIdx());
		m_lastValue = AIDX_SHADING_RATE;
	} else {
		m_shadingRate.set(size);
	}
	dirty(true);
}

RtFloat CAttributes::shadingRate() const
{
	return m_shadingRate.m_value;
}

		
void CAttributes::initShadingInterpolation()
{
	m_shadingInterpolation.set(RI_CONSTANT);
}

RtVoid CAttributes::shadingInterpolation(RtToken type)
{
	if ( m_motionState != 0 ) {
		m_shadingInterpolation.set(type, m_motionState->curSampleIdx(), m_motionState->firstSampleIdx(), m_motionState->lastSampleIdx());
		m_lastValue = AIDX_SHADING_INTERPOLATION;
	} else {
		m_shadingInterpolation.set(type);
	}
	dirty(true);
}


void CAttributes::initMatte()
{
	m_matte.set(defMatte);
}

RtVoid CAttributes::matte(RtBoolean onoff)
{
	if ( m_motionState != 0 ) {
		m_matte.set(onoff, m_motionState->curSampleIdx(), m_motionState->firstSampleIdx(), m_motionState->lastSampleIdx());
		m_lastValue = AIDX_MATTE;
	} else {
		m_matte.set(onoff);
	}
	dirty(true);
}

void CAttributes::initBound()
{
	RtFloat bnd[6];
	bnd[0] = bnd[1] = bnd[2] = -RI_INFINITY;
	bnd[3] = bnd[4] = bnd[5] =  RI_INFINITY;
	m_bound.set(bnd, 6);
	m_boundCalled = false;
}

RtVoid CAttributes::bound(RtBound aBound)
{
	m_boundCalled = true;

	if ( m_motionState != 0 ) {
		m_bound.set(&aBound[0], m_motionState->curSampleIdx(), m_motionState->firstSampleIdx(), m_motionState->lastSampleIdx());
		m_lastValue = AIDX_BOUND;
	} else {
		m_bound.set(&aBound[0]);
	}
	dirty(true);
}

bool CAttributes::getBound(RtBound aBound) const
{
	return m_bound.get(&aBound[0]);
}

void CAttributes::initDetail()
{
	RtFloat bnd[6];
	bnd[0] = bnd[1] = bnd[2] = -RI_INFINITY;
	bnd[3] = bnd[4] = bnd[5] =  RI_INFINITY;
	m_detail.set(bnd, 6);
	m_detailCalled = false;
}

RtVoid CAttributes::detail(RtBound aBound)
{
	m_detailCalled = true;
	if ( m_motionState != 0 ) {
		m_detail.set(&aBound[0], m_motionState->curSampleIdx(), m_motionState->firstSampleIdx(), m_motionState->lastSampleIdx());
		m_lastValue = AIDX_DETAIL;
	} else {
		m_detail.set(&aBound[0]);
	}
	dirty(true);
}

bool CAttributes::getDetail(RtBound aBound) const
{
	return m_detail.get(&aBound[0]);
}

void CAttributes::initDetailRange()
{
	RtFloat d[4];
	d[0] = defMinVis;
	d[1] = defLowTran; 
	d[2] = defUpTran;
	d[3] = defMaxVis; 
	m_detailRange.set(d, 4);

	m_detailRangeCalled = false;
	m_detailRangeCalledInBlock = false;
	dirty(true);
}

RtVoid CAttributes::detailRange(RtFloat minvis, RtFloat lowtran, RtFloat uptran, RtFloat maxvis)
{
	m_detailRangeCalled = true;
	m_detailRangeCalledInBlock = true;
	RtFloat d[4];
	d[0] = minvis;
	d[1] = lowtran; 
	d[2] = uptran;
	d[3] = maxvis; 

	if ( m_motionState != 0 ) {
		m_detailRange.set(d, m_motionState->curSampleIdx(), m_motionState->firstSampleIdx(), m_motionState->lastSampleIdx());
		m_lastValue = AIDX_DETAIL_RANGE;
	} else {
		m_detailRange.set(d);
	}
}

bool CAttributes::getDetailRange(RtFloat &minvis, RtFloat &lowtran, RtFloat &uptran, RtFloat &maxvis) const
{
	if ( 4 >= m_detailRange.m_value.size() )
		return false;

	minvis = m_detailRange.m_value[0];
	lowtran = m_detailRange.m_value[1]; 
	uptran = m_detailRange.m_value[2];
	maxvis = m_detailRange.m_value[3];
	return true;
}

void CAttributes::initGeometricApproximation()
{
	m_geometricApproximationType.set(RI_NULL);
	m_geometricApproximationValue.set(defGeometricApproximationValue);
}

RtVoid CAttributes::geometricApproximation(RtToken type, RtFloat value)
{
	if ( m_motionState != 0 ) {
		m_geometricApproximationType.set(type, m_motionState->curSampleIdx(), m_motionState->firstSampleIdx(), m_motionState->lastSampleIdx());
		m_geometricApproximationValue.set(value, m_motionState->curSampleIdx(), m_motionState->firstSampleIdx(), m_motionState->lastSampleIdx());
		m_lastValue = AIDX_GEOMETRIC_APPROXIMATION_VALUE;
	} else {
		m_geometricApproximationType.set(type);
		m_geometricApproximationValue.set(value);
	}
	dirty(true);
}

void CAttributes::initGeometricRepresentation()
{
	m_geometricRepresentation.set(RI_PRIMITIVE);
}

RtVoid CAttributes::geometricRepresentation(RtToken type)
{
	if ( m_motionState != 0 ) {
		m_geometricRepresentation.set(type, m_motionState->curSampleIdx(), m_motionState->firstSampleIdx(), m_motionState->lastSampleIdx());
		m_lastValue = AIDX_GEOMETRIC_REPRESENTATION;
	} else {
		m_geometricRepresentation.set(type);
	}
	dirty(true);
}

RtVoid CAttributes::resetCoordSysOrientation(RtToken anOrientation)
{
	assert(anOrientation == RI_RH || anOrientation == RI_LH);
	m_coordSysOrientation = anOrientation;
	
	anOrientation = m_orientation.m_value;
	assert(anOrientation == RI_LH || anOrientation == RI_RH || anOrientation == RI_INSIDE || anOrientation == RI_OUTSIDE);
	if ( anOrientation == RI_OUTSIDE ) {
		// Primitive orientation is the same as the current coord sys orientation
		m_primitiveOrientation = m_coordSysOrientation;
	} else if ( anOrientation == RI_INSIDE ) {
		// Primitive orientation is the opposite of the current coord sys orientation
		m_primitiveOrientation = RI_LH;
		if ( m_coordSysOrientation == m_primitiveOrientation )
			m_primitiveOrientation = RI_RH;
	} else {
		// Explicit orientation (RI_LH, RI_RH)
		m_primitiveOrientation = anOrientation;
	}
}

void CAttributes::initOrientation()
{
	m_orientation.set(RI_OUTSIDE);
	m_coordSysOrientation = RI_LH;
	m_primitiveOrientation = RI_LH;
}

RtVoid CAttributes::orientation(RtToken anOrientation)
{
	assert(anOrientation == RI_LH || anOrientation == RI_RH || anOrientation == RI_INSIDE || anOrientation == RI_OUTSIDE);

	if ( m_motionState != 0 ) {
		m_orientation.set(anOrientation, m_motionState->curSampleIdx(), m_motionState->firstSampleIdx(), m_motionState->lastSampleIdx());
		m_lastValue = AIDX_ORIENTATION;
	} else {
		m_orientation.set(anOrientation);
	}
	resetCoordSysOrientation(m_coordSysOrientation);
	dirty(true);
}

RtVoid CAttributes::reverseOrientation(void)
{
	if ( m_motionState == 0 || m_motionState->curSampleIdx() == 0 ) {
		if ( m_orientation.m_value == RI_INSIDE ) m_orientation.m_value = RI_OUTSIDE;
		else if ( m_orientation.m_value == RI_OUTSIDE ) m_orientation.m_value = RI_INSIDE;
		else if ( m_orientation.m_value == RI_LH ) m_orientation.m_value = RI_RH;
		else if ( m_orientation.m_value == RI_RH ) m_orientation.m_value = RI_LH;
		
		unsigned long size = m_orientation.m_motionEnd - m_orientation.m_motionBegin;
		for ( unsigned long i = 0; i < size; ++i ) {
			if ( m_orientation.m_movedValue[i] == RI_INSIDE ) m_orientation.m_movedValue[i] = RI_OUTSIDE;
			else if ( m_orientation.m_movedValue[i] == RI_OUTSIDE ) m_orientation.m_movedValue[i] = RI_INSIDE;
			else if ( m_orientation.m_movedValue[i] == RI_LH ) m_orientation.m_movedValue[i] = RI_RH;
			else if ( m_orientation.m_movedValue[i] == RI_RH ) m_orientation.m_movedValue[i] = RI_LH;
		}
		m_lastValue = AIDX_ENDMARKER;
	}

	m_primitiveOrientation = toggledOrientation(m_primitiveOrientation);
	dirty(true);
}

RtVoid CAttributes::toggleOrientation()
{
	m_coordSysOrientation = toggledOrientation(m_coordSysOrientation);
	m_primitiveOrientation = toggledOrientation(m_primitiveOrientation);
	dirty(true);
}

RtVoid CAttributes::coordSysOrientation(RtToken anOrientation)
{
	assert(anOrientation == RI_RH || anOrientation == RI_LH);
	if ( m_coordSysOrientation != anOrientation ) {
		toggleOrientation();
	}
}

void CAttributes::initSides()
{
	m_nSides.set(defNSides);
}

RtVoid CAttributes::sides(RtInt nsides)
{
	if ( m_motionState != 0 ) {
		m_nSides.set(nsides, m_motionState->curSampleIdx(), m_motionState->firstSampleIdx(), m_motionState->lastSampleIdx());
		m_lastValue = AIDX_SIDES;
	} else {
		m_nSides.set(nsides);
	}
	dirty(true);
}

void CAttributes::initBasis()
{
	m_uBasis.set(RiBezierBasis, RI_BEZIERSTEP);
	m_vBasis.set(RiBezierBasis, RI_BEZIERSTEP);
}

RtVoid CAttributes::basis(RtBasis ubasis, RtInt ustep, RtBasis vbasis, RtInt vstep)
{
	if ( m_motionState != 0 ) {
		m_uBasis.set(ubasis, ustep, m_motionState->curSampleIdx(), m_motionState->firstSampleIdx(), m_motionState->lastSampleIdx());
		m_vBasis.set(vbasis, vstep, m_motionState->curSampleIdx(), m_motionState->firstSampleIdx(), m_motionState->lastSampleIdx());
		m_lastValue = AIDX_BASIS;
	} else {
		m_uBasis.set(ubasis, ustep);
		m_vBasis.set(vbasis, vstep);
	}
	dirty(true);
}

bool CAttributes::getBasis(RtBasis ubasis, RtInt &ustep, RtBasis vbasis, RtInt &vstep) const
{
	memcpy(ubasis, m_uBasis.m_value.m_basis, sizeof(RtBasis));
	ustep = m_uBasis.m_value.m_step;
	memcpy(vbasis, m_vBasis.m_value.m_basis, sizeof(RtBasis));
	vstep = m_vBasis.m_value.m_step;
	return true;
}

void CAttributes::initTrimCurve()
{
	m_trimCurve.clear();
	m_trimCurve.m_value.releaseAll();
}

RtVoid CAttributes::trimCurve(RtInt nloops, RtInt ncurves[], RtInt order[], RtFloat knot[], RtFloat amin[], RtFloat amax[], RtInt n[], RtFloat u[], RtFloat v[], RtFloat w[])
{
	if ( m_motionState != 0 ) {
		CTrimCurveData td(nloops, ncurves, order, knot, amin, amax, n, u, v, w);
		m_trimCurve.set(td, m_motionState->curSampleIdx(), m_motionState->firstSampleIdx(), m_motionState->lastSampleIdx());
		m_lastValue = AIDX_TRIM_CURVE;
	} else {
		m_trimCurve.clear();
		m_trimCurve.m_value.trimCurve(nloops, ncurves, order, knot, amin, amax, n, u, v, w);
	}	
	dirty(true);
}

RtVoid CAttributes::trimCurve(const CTrimCurveData &trimCurveData)
{
	m_trimCurve.clear();
	m_trimCurve.m_value = trimCurveData;
	dirty(true);
}


RtVoid CAttributes::motionBegin(const CMotionState &state)
{
	assert(m_motionState == 0);
	m_motionState = &state;
	m_lastValue = AIDX_ENDMARKER;
}

RtVoid CAttributes::motionEnd()
{
	assert(m_motionState!=0);
	if ( m_motionState != 0 && m_lastValue != AIDX_ENDMARKER ) {
		assert(m_allAttributes[(int)m_lastValue] != 0);
		if (m_allAttributes[(int)m_lastValue] != 0)
			m_allAttributes[(int)m_lastValue]->fill(m_motionState->curSampleIdx());
		if ( m_lastValue == AIDX_GEOMETRIC_APPROXIMATION_VALUE ) {
			m_geometricApproximationType.fill(m_motionState->curSampleIdx());
		} else if ( m_lastValue == AIDX_BASIS ) {
			m_vBasis.fill(m_motionState->curSampleIdx());
		}
	}
	m_motionState = 0;
	m_lastValue = AIDX_ENDMARKER;
}

RtVoid CAttributes::motionSuspend()
{
	m_motionState = 0;
}

RtVoid CAttributes::sample(RtFloat shutterTime, const TypeMotionTimes &times)
{
	std::vector<IMovedValue *>::iterator i;
	for ( i = m_allAttributes.begin(); i != m_allAttributes.end(); ++i ) {
		(*i)->sample(shutterTime, times);
	}
}

RtVoid CAttributes::sampleReset()
{
	std::vector<IMovedValue *>::iterator i;
	for ( i = m_allAttributes.begin(); i != m_allAttributes.end(); ++i ) {
		(*i)->sampleReset();
	}
}

// ----------------------------------------------------------------------------

CAttributes *CAttributesFactory::newAttributes(const CColorDescr &c) const
{
	CAttributes *a = newAttributesInstance(c);
	if ( !a ) {
		throw ExceptRiCPPError(RIE_NOMEM, RIE_SEVERE, "in newAttributes(const CColorDescr &)", __LINE__, __FILE__);
	}
	return a;
}

CAttributes *CAttributesFactory::newAttributes(const CAttributes &attr) const
{
	CAttributes *a = newAttributesInstance(attr);
	if ( !a ) {
		throw ExceptRiCPPError(RIE_NOMEM, RIE_SEVERE, "in newAttributes(const CAttributes &)", __LINE__, __FILE__);
	}
	return a;
}

void CAttributesFactory::deleteAttributes(CAttributes *a)
{
	if ( !a )
		return;

	if ( a->factory() ) {
		const CAttributesFactory *f = a->factory();
		f->deleteAttributesInstance(a);
	} else {
		delete a;
	}
}

