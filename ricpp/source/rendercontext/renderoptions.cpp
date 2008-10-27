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

/*! \file renderoptions.cpp
 *  \brief Implementation of the option set of a render context
 *  \author Andreas Pidde (andreas@pidde.de)
 */

#include "stdafx.h"
#include "renderoptions.h"

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//// TRenderOptions

void TRenderOptions::releaseAll() {
	m_nRGB.clear();
	m_RGBn.clear();
	m_cameraClips.clear();
	m_displays.clear();

	m_imagerParamMap.releaseAll();
	m_hiderParamMap.releaseAll();

	m_optionMap.releaseAll();
}

void TRenderOptions::initialize() {
	m_vpCurrentViewPort.initialize();

	m_bFormatCalled = FALSE;
	m_iDisplayHRes = (RtInt)-1; // 512
	m_iDisplayVRes = (RtInt)-1; // 384
	m_fDisplayPixelAspect = (RtFloat)-1.0; // 1.0
	m_iDisplayOriginX = 0;
	m_iDisplayOriginY = 0;

	m_bFrameAspectRatioCalled = FALSE;
	m_fFrameAspectRatio = (RtFloat)(4.0/3.0);

	m_bScreenWindowCalled = FALSE;
	m_fScreenWindowLeft = (RtFloat)(-4.0/3.0);
	m_fScreenWindowRight = (RtFloat)(4.0/3.0);
	m_fScreenWindowBottom = (RtFloat)(-1.0);
	m_fScreenWindowTop = (RtFloat)(1.0);

	m_bCropWindowCalled = FALSE;
	m_fCropWindowLeft = (RtFloat)0.0;
	m_fCropWindowRight = (RtFloat)1.0;
	m_fCropWindowTop = (RtFloat)0.0;
	m_fCropWindowBottom = (RtFloat)1.0;

	m_bProjectionCalled = FALSE;
	m_tCameraProjection = TRi::RI_ORTHOGRAPHIC;

	m_bFOVSet = FALSE;
	m_tCameraFOV = (RtFloat)90.0;

	m_bNearFarSet = FALSE;
	m_fCameraNearClip = TRi::RI_EPSILON;
	m_fCameraFarClip = TRi::RI_INFINITY;

	m_fCameraFStop = TRi::RI_INFINITY;
	m_fCameraFocalLength = (RtFloat)0.0;
	m_fCameraFocalDistance = (RtFloat)0.0;
	m_fCameraShutterOpen = (RtFloat)0.0;
	m_fCameraShutterClose = (RtFloat)0.0;

	m_bPixelVarianceCalled = TRi::RI_FALSE;
	m_fPixelVariance = (RtFloat)0.0;

	m_fXSamples = (RtFloat)2.0;
	m_fYSamples = (RtFloat)2.0;

	m_pFilterFunc = TRi::gaussianFilter;
	m_fXWidth = (RtFloat)2.0;
	m_fYWitdh = (RtFloat)2.0;

	m_fGain = (RtFloat)1.0;
	m_fGamma = (RtFloat)1.0;
	m_tImager = TRi::RI_NULL;

	m_iOneRGBA = 255;
	m_iMinRGBA = 0;
	m_iMaxRGBA = 255;

	m_fDitherAmplitudeRGBA = (RtFloat)0.5;

	m_iOneZ = 255;
	m_iMinZ = 0;
	m_iMaxZ = 255;

	m_fDitherAmplitudeZ = (RtFloat)0.5;

	m_tImager = TRi::RI_NULL;
	m_imagerParamMap.releaseAll();

	m_tHider = TRi::RI_HIDDEN;
	m_hiderParamMap.releaseAll();

	m_bColorSamplesCalled = TRi::RI_FALSE;
	m_iNColor = 3;
	m_nRGB.clear();
	m_RGBn.clear();

	m_fRelativeDetail = (RtFloat)1.0;

	m_mPreProjectionMatrix.identity();

	m_cameraClips.clear();

	m_displays.clear();

	// following options can get values outside RiBegin()/RiEnd() so they aren't initialized
	/*
	m_optionMap.releaseAll();
	*/

	m_mPreProjectionMatrix.identity();

	// Add. options for RiCPP
	m_mCamTrans.identity();
	m_writeArchive = TRi::RI_FALSE;
	m_disableFormat = TRi::RI_FALSE;
	m_writeObject = TRi::RI_FALSE;
}


// Throws TSystemException (via TParameterMap::operator=, TDeclarationMap::operator=)
void TRenderOptions::assign(const TRenderOptions &ro) {
	releaseAll();

	m_vpCurrentViewPort = 	ro.m_vpCurrentViewPort;

	m_bFormatCalled = ro.m_bFormatCalled;
	m_iDisplayHRes = ro.m_iDisplayHRes;
	m_iDisplayVRes = ro.m_iDisplayVRes;
	m_fDisplayPixelAspect = ro.m_fDisplayPixelAspect;
	m_iDisplayOriginX = ro.m_iDisplayOriginX;
	m_iDisplayOriginY = ro.m_iDisplayOriginY;

	m_bFrameAspectRatioCalled = ro.m_bFrameAspectRatioCalled;
	m_fFrameAspectRatio = ro.m_fFrameAspectRatio;

	m_bScreenWindowCalled = ro.m_bScreenWindowCalled;
	m_fScreenWindowLeft = ro.m_fScreenWindowLeft;
	m_fScreenWindowRight = ro.m_fScreenWindowRight;
	m_fScreenWindowBottom = ro.m_fScreenWindowBottom;
	m_fScreenWindowTop = ro.m_fScreenWindowTop;

	m_bCropWindowCalled = ro.m_bCropWindowCalled;
	m_fCropWindowLeft = ro.m_fCropWindowLeft;
	m_fCropWindowRight = ro.m_fCropWindowRight;
	m_fCropWindowTop = ro.m_fCropWindowTop;
	m_fCropWindowBottom = ro.m_fCropWindowBottom;

	m_bProjectionCalled = ro.m_bProjectionCalled;
	m_tCameraProjection = ro.m_tCameraProjection;

	m_bFOVSet = ro.m_bFOVSet;
	m_tCameraFOV = ro.m_tCameraFOV;

	m_bNearFarSet = ro.m_bNearFarSet;
	m_fCameraNearClip = ro.m_fCameraNearClip;
	m_fCameraFarClip = ro.m_fCameraFarClip;

	m_fCameraFStop = ro.m_fCameraFStop;
	m_fCameraFocalLength = ro.m_fCameraFocalLength;
	m_fCameraFocalDistance = ro.m_fCameraFocalDistance;
	m_fCameraShutterOpen = ro.m_fCameraShutterOpen;
	m_fCameraShutterClose = ro.m_fCameraShutterClose;

	m_bPixelVarianceCalled = ro.m_bPixelVarianceCalled;
	m_fPixelVariance = ro.m_fPixelVariance;

	m_fXSamples = ro.m_fXSamples;
	m_fYSamples = ro.m_fYSamples;

	m_pFilterFunc = ro.m_pFilterFunc;
	m_fXWidth = ro.m_fXWidth;
	m_fYWitdh = ro.m_fYWitdh;

	m_fGain = ro.m_fGain;
	m_fGamma = ro.m_fGamma;

	m_iOneRGBA = ro.m_iOneRGBA;
	m_iMinRGBA = ro.m_iMinRGBA;
	m_iMaxRGBA = ro.m_iMaxRGBA;

	m_fDitherAmplitudeRGBA = ro.m_fDitherAmplitudeRGBA;

	m_iOneZ = ro.m_iOneZ;
	m_iMinZ = ro.m_iMinZ;
	m_iMaxZ = ro.m_iMaxZ;

	m_fDitherAmplitudeZ = ro.m_fDitherAmplitudeZ;

	m_tImager = ro.m_tImager;
	m_imagerParamMap = ro.m_imagerParamMap;

	m_tHider = ro.m_tHider;
	m_hiderParamMap = ro.m_hiderParamMap;

	m_bColorSamplesCalled = ro.m_bColorSamplesCalled;
	m_iNColor = ro.m_iNColor;

	m_nRGB = ro.m_nRGB;
	m_RGBn = ro.m_RGBn;

	m_fRelativeDetail = ro.m_fRelativeDetail;

	m_displays = ro.m_displays;
	m_cameraClips = ro.m_cameraClips;

	m_mPreProjectionMatrix = ro.m_mPreProjectionMatrix;

	m_optionMap = ro.m_optionMap;

	m_mPreProjectionMatrix = ro.m_mPreProjectionMatrix;

	// Add. RiCPP options
	m_mCamTrans = ro.m_mCamTrans;
	m_writeArchive = ro.m_writeArchive;
	m_disableFormat = ro.m_disableFormat;
	m_writeObject = ro.m_writeObject;
}

void TRenderOptions::colorRGB(RtFloat *rgb, const RtFloat *color, int n) const {
	if ( !rgb || !color || n <= 0 )
		return;

	int i, j, k;
	if ( !m_nRGB.empty() ) {
		for ( k = 0; k < n; ++k ) {
			for ( j = 0; j < 3; ++j) {
				rgb[j] = 0.0;
				for ( i = 0; i < m_iNColor; ++i ) {
					rgb[j] += color[i] * m_nRGB[i*3+j];
				}
			}
			rgb += 3;
			color += m_iNColor;
		}
	} else {
		for ( k = 0; k < n; ++k ) {
			rgb[0] = rgb[1] = rgb[2] = 1.0;
			for ( i = 0; i < m_iNColor; ++i ) {
				rgb[i] = color[i];
			}
			rgb += 3;
			color += m_iNColor;
		}
	}
}


void TRenderOptions::colorCustom(RtFloat *color, const RtFloat *rgb, int n) const {
	if ( !color || !rgb || n <= 0 )
		return;

	int i, j, k;
	if ( !m_RGBn.empty() ) {
		for ( k = 0; k < n; ++k ) {
			for ( j = 0; j < m_iNColor; ++j) {
				color[j] = 0.0;
				for ( i = 0; i < 3; ++i ) {
					color[j] += rgb[i] * m_nRGB[i*m_iNColor+j];
				}
			}
			color += m_iNColor;
			rgb += 3;
		}
	} else {
		for ( k = 0; k < n; ++k ) {
			color[0] = color[1] = color[2] = 1.0;
			for ( i = 0; i < m_iNColor; ++i ) {
				color[i] = rgb[i];
			}
			color += m_iNColor;
			rgb += 3;
		}
	}
}
