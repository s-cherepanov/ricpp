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

/*! \file renderattributes.cpp
 *  \brief Implements TRenderAttributes, an object to store a complete attribute set of a render context
 *  \author Andreas Pidde (andreas@pidde.de)
 */

#include "stdafx.h"
#include "renderattributes.h"


// TRenderAttributes - Class with all attributes
TRenderAttributes::TRenderAttributes(int colorComponents) {
	m_colorComponents = colorComponents;
	m_currCoordSysName = TRi::RI_SCREEN;

	// m_CTM.identity();
	// m_inverseCTM.identity();
	// m_inverseCTM.setPreMultiply(false);

	// initialize();
}

TRenderAttributes::TRenderAttributes(const TRenderAttributes &ra) : TBaseClass(ra) {
	// m_CTM.identity();
	// m_inverseCTM.identity();
	// m_inverseCTM.setPreMultiply(false);
	m_currCoordSysName = TRi::RI_SCREEN;

	TRenderAttributes::assign(ra);
}

TRenderAttributes::~TRenderAttributes() {
	releaseAll();
}

TBaseClass &TRenderAttributes::duplicate() const {
	TRenderAttributes *ra = new TRenderAttributes(*this);
	if ( !ra )
		throw TSystemException(__FILE__, __LINE__);
	return *ra;
}

 void TRenderAttributes::assign(const TRenderAttributes &ra) {
	int i, j;

	releaseAll();

	m_colorComponents = ra.m_colorComponents;

	m_areaLightBlock = ra.m_areaLightBlock;

	m_color = ra.m_color;
	m_opacity = ra.m_opacity;
	
	for ( i = 0; i < 8 ; ++i ) {
		m_textureCoordinates[i] = ra.m_textureCoordinates[i];
	}

	m_shadingRate = ra.m_shadingRate;
	m_shadingInterpolation = ra.m_shadingInterpolation;
	m_matte = ra.m_matte;

	for ( i = 0; i < 6 ; ++i ) {
		m_bound[i] = ra.m_bound[i];
		m_detail[i] = ra.m_detail[i];
	}
	m_boundCalled = ra.m_boundCalled;
	m_detailCalled = ra.m_detailCalled;

	for ( i = 0; i < 4 ; ++i ) {
		m_detailRange[i] = ra.m_detailRange[i];
	}
	m_detailRangeCalled = ra.m_detailRangeCalled;

	m_geometricApproximationType = ra.m_geometricApproximationType;
	m_geometricApproximationValue = ra.m_geometricApproximationValue;

	m_geometricRepresentation = ra.m_geometricRepresentation;

	m_orientation = ra.m_orientation;
	m_currentOrientation = ra.m_currentOrientation;
	m_nSides = ra.m_nSides;
	m_currCoordSysName = ra.m_currCoordSysName;

	m_attributeMap = ra.m_attributeMap;
	m_lightsOn = ra.m_lightsOn;

	// m_CTM = ra.m_CTM;
	// Since attributes and transformation blocks are nested, only the current CTM is needed
	// m_matrixStack = ra.m_matrixStack;
	// m_matrixStack.clear();
	// m_matrixStack.push_back(m_CTM);

	// m_inverseCTM = ra.m_inverseCTM;
	// Since attributes and transformation blocks are nested, only the current CTM is needed
	// m_inverseMatrixStack = ra.m_inverseMatrixStack;
	// m_inverseMatrixStack.clear();
	// m_inverseMatrixStack.push_back(m_inverseCTM);

	for ( i = 0; i < N_SHADERS ; ++i ) {
		m_shaders[i] = ra.m_shaders[i];
		m_shaderMatrices[i] = ra.m_shaderMatrices[i];
		m_shaderInverseMatrices[i] = ra.m_shaderInverseMatrices[i];
	}

	m_uStep = ra.m_uStep;
	m_vStep = ra.m_vStep;

	for ( i = 0; i < 4; ++i ) {
		for ( j = 0; j < 4; ++j ) {
			m_uBasis[i][j] = ra.m_uBasis[i][j];
			m_vBasis[i][j] = ra.m_vBasis[i][j];
		}
	}

	m_trimCurve = ra.m_trimCurve;
}

void TRenderAttributes::releaseAll() {
	m_color.clear();
	m_opacity.clear();
	m_attributeMap.releaseAll();
	// m_matrixStack.clear();
	// m_inverseMatrixStack.clear();
	m_lightsOn.clear();
}


void TRenderAttributes::initialize() {
	int i, j;

	m_color.clear();
	for ( i=0; i<m_colorComponents; ++i) {
		m_color.push_back((RtFloat)1.0);
	}

	m_opacity.clear();
	for ( i=0; i<m_colorComponents; ++i) {
		m_opacity.push_back((RtFloat)1.0);
	}

	m_textureCoordinates[0] = (RtFloat)0.0;
	m_textureCoordinates[1] = (RtFloat)0.0;
	m_textureCoordinates[2] = (RtFloat)1.0;
	m_textureCoordinates[3] = (RtFloat)0.0;
	m_textureCoordinates[4] = (RtFloat)0.0;
	m_textureCoordinates[5] = (RtFloat)1.0;
	m_textureCoordinates[6] = (RtFloat)1.0;
	m_textureCoordinates[7] = (RtFloat)1.0;

	m_lightsOn.clear();
	m_areaLightBlock = TRi::RI_FALSE;

	m_shaders[IDX_SHADER_SURFACE].shader(TRi::RI_MATTE);
	for ( i = 1; i < N_SHADERS; ++i ) {
		m_shaders[i].shader(TRi::RI_NULL);
		m_shaderMatrices[i].identity();
		m_shaderInverseMatrices[i].identity();
	}

	m_shadingRate = (RtFloat)1.0;
	m_shadingInterpolation = TRi::RI_CONSTANT;
	m_matte = TRi::RI_FALSE;

	for ( i=0; i<6 ; ++i ) {
		m_bound[i] = (RtFloat)0.0;
		m_detail[i] = (RtFloat)0.0;
	}
	m_boundCalled = TRi::RI_FALSE;
	m_detailCalled = TRi::RI_FALSE;
	
	for ( i=0; i<4 ; ++i ) {
		m_detailRange[i] = (RtFloat)0.0;
	}
	m_detailRangeCalled = TRi::RI_FALSE;

	m_geometricApproximationType = "";
	m_geometricApproximationValue = (RtFloat)1.0;

	m_geometricRepresentation = "smooth";

	m_orientation        = TRi::RI_OUTSIDE;
	m_currentOrientation = true;

	m_nSides = 2;

	m_uStep = TRi::RI_BEZIERSTEP;
	m_vStep = TRi::RI_BEZIERSTEP;

	for ( i = 0; i < 4; ++i ) {
		for ( j = 0; j < 4; ++j ) {
			m_uBasis[i][j] = TRi::bezierBasis[i][j];
			m_vBasis[i][j] = TRi::bezierBasis[i][j];
		}
	}


	m_currCoordSysName = "";

	// m_CTM.identity(); // Not here
	// m_matrixStack.clear();
	// m_inverseCTM.identity(); // Not here
	// m_inverseMatrixStack.clear();
	m_attributeMap.releaseAll();

	m_trimCurve.releaseAll();
}
