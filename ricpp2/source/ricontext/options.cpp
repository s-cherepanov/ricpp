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

/** @file options.cpp
 *  @author Andreas Pidde (andreas@pidde.de)
 *  @brief Implements the handling of the render options (used by render state).
 */

#include "ricpp/ricontext/options.h"

#ifndef _RICPP_RICPP_RICPPERROR_H
#include "ricpp/ricpp/ricpperror.h"
#endif // _RICPP_RICPP_RICPPERROR_H

using namespace RiCPP;

const RtInt   COptions::defXResolution = 640;
const RtInt   COptions::defYResolution = 480;
const RtFloat COptions::defPixelAspectRatio = 1.0;

const RtFloat COptions::defFrameAspectRatio =
	(defXResolution * defPixelAspectRatio) /
    static_cast<RtFloat>(defYResolution);

const RtFloat COptions::defScreenWindowLeft = -defFrameAspectRatio;
const RtFloat COptions::defScreenWindowRight = defFrameAspectRatio;
const RtFloat COptions::defScreenWindowBottom = -1.0;
const RtFloat COptions::defScreenWindowTop = 1.0;

const RtFloat COptions::defCropWindowLeft = defScreenWindowLeft;
const RtFloat COptions::defCropWindowRight = defScreenWindowRight;
const RtFloat COptions::defCropWindowBottom = defScreenWindowBottom;
const RtFloat COptions::defCropWindowTop = defScreenWindowTop;

const RtToken COptions::defProjection = RI_ORTHOGRAPHIC;
const RtFloat COptions::defCameraFOV = 90.0;

const RtFloat COptions::defNearClip = RI_EPSILON;
const RtFloat COptions::defFarClip = RI_INFINITY;

const RtFloat COptions::defFstop = RI_INFINITY;
const RtFloat COptions::defFocalLength = 0;
const RtFloat COptions::defFocalDistance = 0;

const RtFloat COptions::defShutterOpen = 0;
const RtFloat COptions::defShutterClose = 0;

const RtFloat COptions::defPixelVariance = 0;

const RtFloat COptions::defXSamples = 2;
const RtFloat COptions::defYSamples = 2;

const RtFloat COptions::defXFilterWidth = 2;
const RtFloat COptions::defYFilterWidth = 2;

const RtFloat COptions::defGain = 1;
const RtFloat COptions::defGamma = 1;

const RtInt   COptions::defOneRGBA = 255;
const RtInt   COptions::defMinRGBA = 0;
const RtInt   COptions::defMaxRGBA = 255;
const RtFloat COptions::defDitherAmplitudeRGBA = 0.5;

const RtInt   COptions::defOneZ = 0;
const RtInt   COptions::defMinZ = 0;
const RtInt   COptions::defMaxZ = 0;
const RtFloat COptions::defDitherAmplitudeZ = 0;

const RtToken COptions::defImagerName = RI_NULL;

const RtToken COptions::defHiderType = RI_HIDDEN;

const RtFloat COptions::defRelativeDetail = 1;



COptions::~COptions()
{
}

COptions &COptions::operator=(const COptions &ro)
{
	if ( this == &ro )
		return *this;
	
	m_formatCalled = ro.m_formatCalled;
	m_xResolution = ro.m_xResolution;
	m_yResolution = ro.m_yResolution;
	m_pixelAspectRatio = ro.m_pixelAspectRatio;

	m_frameAspectRatioCalled = ro.m_frameAspectRatioCalled;
	m_frameAspectRatio = ro.m_frameAspectRatio;

	m_screenWindowCalled = ro.m_screenWindowCalled;
	m_screenWindowLeft = ro.m_screenWindowLeft;
	m_screenWindowRight = ro.m_screenWindowRight;
	m_screenWindowBottom = ro.m_screenWindowBottom;
	m_screenWindowTop = ro.m_screenWindowTop;

	m_cropWindowCalled = ro.m_cropWindowCalled;
	m_cropWindowLeft = ro.m_cropWindowLeft;
	m_cropWindowRight = ro.m_cropWindowRight;
	m_cropWindowBottom = ro.m_cropWindowBottom;
	m_cropWindowTop = ro.m_cropWindowTop;

	m_projectionCalled = ro.m_projectionCalled;
	m_projectionName = ro.m_projectionName;
	m_projectionParams = ro.m_projectionParams;
	m_FOVSet = ro.m_FOVSet;
	m_FOV = ro.m_FOV;

	m_nearFarSet = ro.m_nearFarSet;
	m_nearClip = ro.m_nearClip;
	m_farClip = ro.m_farClip;

	m_clippingPlanes = ro.m_clippingPlanes;

	m_depthOfFieldCalled = ro.m_depthOfFieldCalled;
	m_fstop = ro.m_fstop;
	m_focalLength = ro.m_focalLength;
	m_focalDistance = ro.m_focalDistance;

	m_shutterOpen = ro.m_shutterOpen;
	m_shutterClose = ro.m_shutterClose;

	m_pixelVarianceCalled = ro.m_pixelVarianceCalled;
	m_pixelVariance = ro.m_pixelVariance;

	m_xSamples = ro.m_xSamples;
	m_ySamples = ro.m_ySamples;

	m_exposureCalled = ro.m_exposureCalled;

	m_imagerName = ro.m_imagerName;
	m_imagerParams = ro.m_imagerParams;

	m_gain = ro.m_gain;
	m_gamma = ro.m_gamma;


	m_quantizers = ro.m_quantizers;

	m_filterFunc = 0;
	if ( ro.m_filterFunc )
		m_filterFunc = &(ro.m_filterFunc->singleton());

	m_xWidth = ro.m_xWidth;
	m_yWidth = ro.m_yWidth;

	m_displayChannels = ro.m_displayChannels;

	m_hiderType = ro.m_hiderType;
	m_hiderParams = ro.m_hiderParams;

	m_relativeDetail = ro.m_relativeDetail;

	COptionsBase::operator=(ro);

	return *this;
}

// ----

void COptions::init()
{
	initFormat();
	initFrameAspectRatio();
	initScreenWindow();
	initCropWindow();
	initProjection();
	initClipping();
	initClippingPlane();
	initDepthOfField();
	initPixelVariance();
	initExposure();
	initImager();
	initQuantize();
	initDisplayChannels();
	initDisplays();
	initHider();
	initRelativeDetail();
}

// ----

void COptions::initFormat()
{
	m_formatCalled = false;
	m_xResolution = defXResolution;
	m_yResolution = defYResolution;
	m_pixelAspectRatio = defPixelAspectRatio;
}

RtVoid COptions::format(RtInt xres, RtInt yres, RtFloat aspect)
{
	m_formatCalled = true;
	m_xResolution = xres;
	m_yResolution = yres;
	m_pixelAspectRatio = aspect;
}

// ----

void COptions::initFrameAspectRatio()
{
	m_frameAspectRatioCalled = false;
	m_frameAspectRatio = defFrameAspectRatio;
}

RtVoid COptions::frameAspectRatio(RtFloat aspect)
{
	m_frameAspectRatioCalled = true;
	m_frameAspectRatio = aspect;
}

RtFloat COptions::frameAspectRatio() const
{
	if ( m_frameAspectRatioCalled )
		return m_frameAspectRatio;

	if ( m_yResolution != 0 )
		return (m_xResolution * m_pixelAspectRatio) / static_cast<RtFloat>(m_yResolution);

	return RI_INFINITY;
}

// ----

RtVoid COptions::initScreenWindow()
{
	m_screenWindowCalled = false;
	m_screenWindowLeft = defScreenWindowLeft;
	m_screenWindowRight = defScreenWindowRight;
	m_screenWindowBottom = defScreenWindowBottom;
	m_screenWindowTop = defScreenWindowTop;
}

RtVoid COptions::screenWindow(RtFloat left, RtFloat right, RtFloat bot, RtFloat top)
{
	m_screenWindowCalled = true;
	m_screenWindowLeft = left;
	m_screenWindowRight = right;
	m_screenWindowBottom = bot;
	m_screenWindowTop = top;
}

RtVoid COptions::getScreenWindow(RtFloat &left, RtFloat &right, RtFloat &bot, RtFloat &top) const
{
	if ( m_screenWindowCalled ) {
		left = m_screenWindowLeft;
		right = m_screenWindowRight;
		bot = m_screenWindowBottom;
		top = m_screenWindowTop;
	} else {
		RtFloat ratio = frameAspectRatio();
		if ( ratio >= 1.0 ) {
			left = -ratio;
			right = ratio;
			bot = -1.0;
			top = 1.0;
		} else {
			left = -1.0;
			right = 1.0;
			bot = -inversetempl(ratio);
			top = inversetempl(ratio);
		}
	}
}

// ----

RtVoid COptions::initCropWindow()
{
	m_cropWindowCalled = false;
	m_cropWindowLeft = defCropWindowLeft;
	m_cropWindowRight = defCropWindowRight;
	m_cropWindowBottom = defCropWindowBottom;
	m_cropWindowTop = defCropWindowTop;
}

RtVoid COptions::cropWindow(RtFloat xmin, RtFloat xmax, RtFloat ymin, RtFloat ymax)
{
	m_cropWindowCalled = true;
	m_cropWindowLeft = xmin;
	m_cropWindowRight = xmax;
	m_cropWindowBottom = ymin;
	m_cropWindowTop = ymax;
}

RtVoid COptions::getCropWindow(RtFloat &xmin, RtFloat &xmax, RtFloat &ymin, RtFloat &ymax) const
{
	if ( m_cropWindowCalled ) {
		xmin = m_cropWindowLeft;
		xmax = m_cropWindowRight;
		ymin = m_cropWindowBottom;
		ymax = m_cropWindowTop;
	} else {
		getScreenWindow(xmin, xmax, ymin, ymax);
	}
}

RtVoid COptions::getCropWindow(RtInt &xmin, RtInt &xmax, RtInt &ymin, RtInt &ymax) const
{
	RtFloat xminf, xmaxf, yminf, ymaxf;
	RtFloat xres = static_cast<RtFloat>(xResolution());
	RtFloat yres = static_cast<RtFloat>(yResolution());

	getCropWindow(xminf, xmaxf, yminf, ymaxf);

	xmin = static_cast<RtInt>(clamptempl(
		static_cast<RtFloat>(ceil(xres*xminf)),
		static_cast<RtFloat>(0),
		static_cast<RtFloat>(xres - 1)
	));
	
	xmax = static_cast<RtInt>(clamptempl(
		static_cast<RtFloat>(ceil(xres*xmaxf)),
		static_cast<RtFloat>(0),
		static_cast<RtFloat>(xres - 1)
	));

	ymin = static_cast<RtInt>(clamptempl(
		static_cast<RtFloat>(ceil(yres*yminf)),
		static_cast<RtFloat>(0),
		static_cast<RtFloat>(yres - 1)
	));
	
	ymax = static_cast<RtInt>(clamptempl(
		static_cast<RtFloat>(ceil(yres*ymaxf)),
		static_cast<RtFloat>(0),
		static_cast<RtFloat>(yres - 1)
	));
}

// ----

void COptions::initProjection()
{
	m_projectionParams.clear();
	m_projectionCalled = false;
	m_projectionName = defProjection;
	m_FOVSet = false;
	m_FOV = defCameraFOV;
}

RtVoid COptions::projection(RtToken name, const CParameterList &params)
{
	m_projectionCalled = true;
	m_projectionName = name;
	m_projectionParams = params;

	m_FOVSet = false;
	if ( m_projectionName == RI_PERSPECTIVE ) {
		const CParameter *p = m_projectionParams.get(RI_FOV);
		if ( p && p->floats().size() > 0 ) {
			m_FOVSet = true;
			m_FOV = p->floats()[0];
		} else {
			m_FOV = defCameraFOV;
		}
	}
}

// ----

void COptions::initClipping()
{
		m_nearFarSet = false;
		m_nearClip = defNearClip;
		m_farClip = defFarClip;
}

RtVoid COptions::clipping(RtFloat hither, RtFloat yon)
{
		m_nearFarSet = true;
		m_nearClip = hither;
		m_farClip = yon;
}

// ----

void COptions::initClippingPlane()
{
	m_clippingPlanes.clear();
}

RtVoid COptions::clippingPlane(RtFloat x, RtFloat y, RtFloat z, RtFloat nx, RtFloat ny, RtFloat nz)
{
	m_clippingPlanes.push_back(CClippingPlane(x, y, z,nx,ny,nz));
}

RtVoid COptions::clippingPlane(const CClippingPlane &s)
{
	m_clippingPlanes.push_back(s);
}

// ----

void COptions::initDepthOfField()
{
	m_depthOfFieldCalled = false;
	m_fstop = defFstop;
	m_focalLength = defFocalLength;
	m_focalDistance = defFocalDistance;
}

RtVoid COptions::depthOfField(RtFloat fstop, RtFloat focallength, RtFloat focaldistance)
{
	m_depthOfFieldCalled = true;
	m_fstop = fstop;
	m_focalLength = focallength;
	m_focalDistance = focaldistance;
}

RtFloat COptions::fstop() const
{
	return m_fstop;
}

RtFloat COptions::focallength() const
{
	if ( m_depthOfFieldCalled )
		return m_focalLength;

	if ( projectionCalled() && projectionName() == RI_PERSPECTIVE ) {
		RtFloat left, right, bot, top;
		getScreenWindow(left, right, bot, top);
		RtFloat width = right-left;
		RtFloat height = top-bot;
		if ( height > 0 ) {
			return (width/height)/tan(fov()/2);
		}
	}

	return m_focalLength;
}

void COptions::getDepthofField(RtFloat &fstop, RtFloat &focallength, RtFloat &focaldistance) const
{
	fstop = this->fstop();
	focallength = this->focallength();
	focaldistance = this->focaldistance();
}

RtFloat COptions::lensdiameter() const
{
	if ( fstop() == RI_INFINITY )
		return 0;

	if ( fstop() == 0 )
		return RI_INFINITY;

	return focallength()/fstop();
}

// ----

void COptions::initShutter()
{
	m_shutterOpen = defShutterOpen;
	m_shutterClose = defShutterClose;
}

RtVoid COptions::shutter(RtFloat smin, RtFloat smax)
{
	m_shutterOpen = smin;
	m_shutterClose = smax;
}

// ----

void COptions::initPixelVariance()
{
	m_pixelVarianceCalled = false;
	m_pixelVariance = defPixelVariance;
}

RtVoid COptions::pixelVariance(RtFloat variation)
{
	m_pixelVarianceCalled = true;
	m_pixelVariance = variation;
}

// ----

void COptions::initPixelSamples()
{
	m_xSamples = defXSamples;
	m_ySamples = defYSamples;
}

RtVoid COptions::pixelSamples(RtFloat xsamples, RtFloat ysamples)
{
	m_xSamples = xsamples;
	m_ySamples = ysamples;
}

// ----

void COptions::initPixelFilter()
{
	m_filterFunc = &CGaussianFilter::func;

	m_xWidth = defXFilterWidth;
	m_yWidth = defYFilterWidth;
}

RtVoid COptions::pixelFilter(const IFilterFunc &function, RtFloat xwidth, RtFloat ywidth)
{
	m_filterFunc = &function.singleton();

	m_xWidth = xwidth;
	m_yWidth = ywidth;
}

// ----

void COptions::initExposure()
{
	m_exposureCalled = false;
	m_gain = defGain;
	m_gamma = defGamma;
}

RtVoid COptions::exposure(RtFloat gain, RtFloat gamma)
{
	m_exposureCalled = true;
	m_gain = gain;
	m_gamma = gamma;
}

RtVoid COptions::colorExposure(RtColor color) const
{
	if ( m_gain == m_gamma && m_gain == 1 )
		return;

	RtInt iend = colorSamples();
	for ( RtInt i = 0; i < iend; ++i ) {
		color[i] = pow(color[i]*m_gain, ((RtFloat)1.0/m_gamma));
	}
	
}

RtVoid COptions::colorExposure(std::vector<RtFloat> &colors) const
{
	if ( m_gain == m_gamma && m_gain == 1 )
		return;

	for ( std::vector<RtFloat>::iterator i = colors.begin();
		  i != colors.end();
		  ++i )
	{
		*i = pow((*i)*m_gain, ((RtFloat)1.0/m_gamma));
	}
}

// ----

void COptions::initImager()
{
	m_imagerName = RI_NULL;
	m_imagerParams.clear();
}

RtVoid COptions::imager(RtToken name, const CParameterList &params)
{
	m_imagerName = name;
	m_imagerParams = params;
}

// ----

void COptions::initQuantize()
{
	m_quantizers[RI_RGBA] = CQuantizer(RI_RGBA, defOneRGBA, defMinRGBA, defMaxRGBA, defDitherAmplitudeRGBA);
	m_quantizers[RI_Z] = CQuantizer(RI_Z, defOneZ, defMinZ, defMaxZ, defDitherAmplitudeZ);
}

RtVoid COptions::quantize(RtToken type, RtInt one, RtInt qmin, RtInt qmax, RtFloat ampl)
{
	m_quantizers[type] = CQuantizer(type, one, qmin, qmax, ampl);
}

const CQuantizer *COptions::quantizer(RtToken type) const
{
	std::map<RtToken, CQuantizer>::const_iterator iter = m_quantizers.find(type);
	if ( iter != m_quantizers.end() ) {
		return &(*iter).second;
	}
	return 0;
}

bool COptions::hasQuantizer(RtToken type) const
{
	return quantizer(type) != 0;
}

bool COptions::getQuantize(RtToken type, RtInt &one, RtInt &qmin, RtInt &qmax, RtFloat &ampl) const
{
	const CQuantizer *q = quantizer(type);
	if ( q ) {
		one = q->one();
		qmin = q->minVal();
		qmax = q->maxVal();
		ampl = q->ditherAmplitude();
		return true;
	}

	return false;
}

// ----

void COptions::initDisplayChannels()
{
	m_displayChannels.clear();
}

RtVoid COptions::displayChannel(CDeclarationDictionary &dict, const CColorDescr &colorDescr, RtString channel, const CParameterList &params)
{
	CDisplayChannelDescr dcd;
	dcd.displayChannel(dict, colorDescr, channel, params);

	CDisplayDescr::TypeDisplayChannels::iterator iter;
	for ( iter = m_displayChannels.begin();
	      iter != m_displayChannels.end();
		  ++iter )
	{
		if ( (*iter).channel() == dcd.channel() )
		{
			// == 0
			break;
		}
		if ( (*iter).channel() && dcd.channel() && !strcmp((*iter).channel()->name(), dcd.channel()->name()) )
		{
			break;
		}
	}

	if ( iter != m_displayChannels.end() )
	{
		m_displayChannels.erase(iter);
	}

	m_displayChannels.push_back(dcd);
}

CDisplayDescr::TypeDisplayChannels::const_iterator COptions::findDisplayChannel(RtString channelName) const
{
	channelName = noNullStr(channelName);

	CDisplayDescr::TypeDisplayChannels::const_iterator iter;
	for ( iter = displayChannelBegin();
	      iter != displayChannelEnd();
		  ++iter )
	{
		if ( (*iter).channel() && !strcmp((*iter).channel()->name(), channelName) )
		{
			return iter;
		}
	}
	return iter;
}

// ----

void COptions::initDisplays()
{
	m_displays.clear();
}

RtVoid COptions::display(RtString name, RtToken type, RtString mode, const CParameterList &params)
{
	if ( name && name[0] != '+' ) {
		m_displays.clear();
	}
	if ( name && name[0] == '+' ) {
		++name;
	}

	m_displays.push_back(CDisplayDescr());
	CDisplayDescr &dd = m_displays.back();
	dd.display(m_displayChannels, name, type, mode, params);
}

COptions::TypeDisplays::const_iterator COptions::findDisplay(RtString name) const
{
	name = noNullStr(name);

	TypeDisplays::const_iterator iter;
	for ( iter = displayBegin();
	      iter != displayEnd();
		  ++iter )
	{
		if ( !strcmp((*iter).name(), name) )
		{
			return iter;
		}
	}
	return iter;
}

// ----

void COptions::initHider()
{
	m_hiderType = defHiderType;
	m_hiderParams.clear();
}

RtVoid COptions::hider(RtToken type, const CParameterList &params)
{
	if ( type == RI_NULL_LIT ) // "null" as null
		type = RI_NULL;

	m_hiderType = type;
	m_hiderParams = params;
}

// ----

void COptions::initRelativeDetail()
{
	m_relativeDetail = defRelativeDetail;
}

RtVoid COptions::relativeDetail(RtFloat relativedetail)
{
	m_relativeDetail = relativedetail;
}


// ----------------------------------------------------------------------------

COptions *COptionsFactory::newOptions()
{
	COptions *o = newOptionsInstance();
	if ( !o ) {
		throw ExceptRiCPPError(RIE_NOMEM, RIE_SEVERE, "in newOptions()", __LINE__, __FILE__);
	}
	return o;
}

COptions *COptionsFactory::newOptions(const COptions &opt)
{
	COptions *o = new COptions(opt);
	if ( !o ) {
		throw ExceptRiCPPError(RIE_NOMEM, RIE_SEVERE, "in newOptions(const COptions &opt)", __LINE__, __FILE__);
	}
	return o;
}
