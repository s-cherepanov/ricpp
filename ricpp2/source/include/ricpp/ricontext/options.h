#ifndef _RICPP_RICONTEXT_OPTIONS_H
#define _RICPP_RICONTEXT_OPTIONS_H

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

/** @file options.h
 *  @author Andreas Pidde (andreas@pidde.de)
 *  @brief Handling of the render options (used by render state).
 */

#ifndef _RICPP_RICONTEXT_OPTIONSBASE_H
#include "ricpp/ricontext/optionsbase.h"
#endif //  _RICPP_RICONTEXT_OPTIONSBASE_H

#ifndef _RICPP_RICONTEXT_DISPLAYDESC_H
#include "ricpp/ricontext/displaydesc.h"
#endif //  _RICPP_RICONTEXT_DISPLAYDESC_H

#ifndef _RICPP_RICPP_FILTERS_H
#include "ricpp/ricpp/filters.h"
#endif // _RICPP_RICPP_FILTERS_H

namespace RiCPP {
	class COptionsFactory;

	/** @brief Option set
	 */
	class COptions : public COptionsBase {
		friend class COptionsFactory;

		COptionsFactory *m_factory;
	public:
		const COptionsFactory *factory() const { return m_factory; }

	protected:
		void factory(COptionsFactory *aFactory) { m_factory = aFactory; }

	public:
		static const RtInt   defXResolution;
		static const RtInt   defYResolution;
		static const RtFloat defPixelAspectRatio;

		static const RtFloat defFrameAspectRatio;

		static const RtFloat defScreenWindowLeft;
		static const RtFloat defScreenWindowRight;
		static const RtFloat defScreenWindowBottom;
		static const RtFloat defScreenWindowTop;

		static const RtFloat defCropWindowLeft;
		static const RtFloat defCropWindowRight;
		static const RtFloat defCropWindowBottom;
		static const RtFloat defCropWindowTop;

		static const RtToken defProjection;
		static const RtFloat defCameraFOV;

		static const RtFloat defNearClip;
		static const RtFloat defFarClip;

		static const RtFloat defFstop;
		static const RtFloat defFocalLength;
		static const RtFloat defFocalDistance;

		static const RtFloat defShutterOpen;
		static const RtFloat defShutterClose;

		static const RtFloat defPixelVariance;

		static const RtFloat defXSamples;
		static const RtFloat defYSamples;

		static const RtFloat defXFilterWidth;
		static const RtFloat defYFilterWidth;

		static const RtFloat defGain;
		static const RtFloat defGamma;

		static const RtInt   defOneRGBA;
		static const RtInt   defMinRGBA;
		static const RtInt   defMaxRGBA;
		static const RtFloat defDitherAmplitudeRGBA;

		static const RtInt   defOneZ;
		static const RtInt   defMinZ;
		static const RtInt   defMaxZ;
		static const RtFloat defDitherAmplitudeZ;

		static const RtToken defImagerName;

		static const RtToken defHiderType;

		static const RtFloat defRelativeDetail;

		typedef std::vector<CClippingPlane> TypeClippingPlanes;
		typedef std::list<CDisplayDescr> TypeDisplays;
	private:
		// CViewPort m_curViewPort;      ///< Viewport data

		CDisplayDescr::TypeDisplayChannels m_displayChannels; ///< Display channels can be as mode by display
		TypeDisplays m_displays; ///< Current displays (CDisplayDescr), set by CRi::display()

		std::map<RtToken, CQuantizer> m_quantizers; ///< Quantizer (def. "RGBA" and "z")

		TypeClippingPlanes m_clippingPlanes;   ///< Additional clipping planes

		bool    m_formatCalled;        ///< true, if CRi::format() has been called
		RtInt	m_xResolution;		   ///< Horizontal Resolution of last CRi::display() call
		RtInt	m_yResolution;		   ///< Vertical Resolution of last CRi::display() call
		RtFloat	m_pixelAspectRatio;    ///< Pixel Aspect Ratio of last CRi::display() call

		bool m_frameAspectRatioCalled; ///< true, if CRi::frameAspectRatio() has been called
		RtFloat m_frameAspectRatio;    ///< Frame Aspect Ratio, only valid if CRi::frameAspectRatio() called

		bool m_screenWindowCalled;     ///< true, if CRi::screenWindow() has been called
		RtFloat m_screenWindowLeft;    ///< Screen coordinates, left edge, only valid if CRi::screenWindow() called
		RtFloat m_screenWindowRight;   ///< Screen coordinates, right edge, only valid if CRi::screenWindow() called
		RtFloat m_screenWindowBottom;  ///< Screen coordinates, bottom edge, only valid if CRi::screenWindow() called
		RtFloat m_screenWindowTop;     ///< Screen coordinates, top edge, only valid if CRi::screenWindow() called

		bool m_cropWindowCalled;    ///< true, if CRi::cropWindow() has been called
		RtFloat m_cropWindowLeft;   ///< Crop Window (NDC), left edge, only valid if CRi::cropWindow() called (otherwise whol raster window is rendered)
		RtFloat m_cropWindowRight;  ///< Crop Window (NDC), right edge, only valid if CRi::cropWindow() called (otherwise whol raster window is rendered)
		RtFloat m_cropWindowBottom; ///< Crop Window (NDC), bottom edge, only valid if CRi::cropWindow() called (otherwise whol raster window is rendered)
		RtFloat m_cropWindowTop;    ///< Crop Window (NDC), top edge, only valid if CRi::cropWindow() called (otherwise whol raster window is rendered)

		bool m_projectionCalled;           ///< CRi::projection() is called
		RtToken m_projectionName;          ///< Projection type, default "orthographic"
		CParameterList m_projectionParams; ///< Projection parameters
		RtBoolean m_FOVSet;                ///< RI_FOV is set in CRi::projection("perspective");
		RtFloat m_FOV;                     ///< Field of View (perspective projection), default is 90.0 (degrees)

		bool m_nearFarSet;          ///< Near and far clipping plane are set
		RtFloat m_nearClip;         ///< Near Clipping plane, default is epsilon
		RtFloat m_farClip;          ///< Far Clipping plane, default is infinity

		bool m_depthOfFieldCalled;  ///< CRi::depthOfField() has been called
		RtFloat	m_fstop;			///< f-Stop (aperture number) for depth of field
		RtFloat	m_focalLength;      ///< focal length for DOF
		RtFloat	m_focalDistance;    ///< focal distance for DOF

		RtFloat	m_shutterOpen;      ///< Shutter open, motion blur, default 0
		RtFloat	m_shutterClose;     ///< Shutter close, motion blur, default 0

		bool m_pixelVarianceCalled;	///< CRi::pixelVariance() has been called
		RtFloat m_pixelVariance;	///< Accepted astimated variance of the pixel values from the true pixel values (no default)

		RtFloat m_xSamples,         ///< sampling rate in horizontal direction 2
				m_ySamples;         ///< sampling rate in vertical direction 2

		const IFilterFunc *m_filterFunc;  ///< Default filter func for CRi::pixelFilter(), default CGaussianFilter
		RtFloat m_xWidth,           ///< X-width of the filter in pixels, default 2
				m_yWidth;           ///< Y-width of the filter in pixels, default 2

		RtToken m_imagerName;          ///< Name of Imager Shader, default RI_NULL
		CParameterList m_imagerParams; ///< Imager Shader, default no params

		bool m_exposureCalled;		///< CRi::exposure() has been called
		RtFloat m_gain,             ///< Exposure (gain) color = (color*gain)**(1/gamma), default 1.0
				m_gamma;            ///< Exposure (gamma) color = (color*gain)**(1/gamma), default 1.0

		RtToken m_hiderType;          ///< Type of the hidden surface algorithm
		CParameterList m_hiderParams; ///< Hidden surface algorithm, default "hidden" (others are "null" and "paint")

		RtFloat m_relativeDetail;   ///< Sets the relative Detail, default 1.0

		void init();

		void initFormat();
		void initFrameAspectRatio();
		void initScreenWindow();
		void initCropWindow();
		void initProjection();
		void initClipping();
		void initClippingPlane();
		void initDepthOfField();
		void initShutter();
		void initPixelVariance();
		void initPixelSamples();
		void initPixelFilter();
		void initExposure();
		void initImager();
		void initQuantize();
		void initDisplayChannels();
		void initDisplays();
		void initHider();
		void initRelativeDetail();

	public:
		inline COptions()
		{
			m_factory = 0;
			m_filterFunc = 0;
			init();
		}

		inline COptions(const COptions &ro)
		{
			m_factory = 0;
			m_filterFunc = 0;
			*this = ro;
		}

		virtual ~COptions();

		virtual COptionsBase *duplicate() const;

		COptions &operator=(const COptions &ra);

		RtVoid format(RtInt xres, RtInt yres, RtFloat aspect);

		inline bool formatCalled() const
		{
			return m_formatCalled;
		}

		inline RtInt xResolution() const
		{
			return m_xResolution;
		}

		inline RtInt yResolution() const
		{
			return m_yResolution;
		}

		inline RtFloat pixelAspectRatio() const
		{
			return m_pixelAspectRatio;
		}

		RtVoid getFormat(RtInt &xres, RtInt &yres, RtFloat &aspect) const
		{
			xres = m_xResolution;
			yres = m_yResolution;
			aspect = m_pixelAspectRatio;
		}

		RtVoid frameAspectRatio(RtFloat aspect);

		inline bool frameAspectRatioCalled() const
		{
			return m_frameAspectRatioCalled;
		}
		RtFloat frameAspectRatio() const;

		RtVoid screenWindow(RtFloat left, RtFloat right, RtFloat bot, RtFloat top);
		inline bool screenWindowCalled() const
		{
			return m_screenWindowCalled;
		}
		RtVoid getScreenWindow(RtFloat &left, RtFloat &right, RtFloat &bot, RtFloat &top) const;
		inline RtFloat screenWindowLeft() const
		{
			return m_screenWindowLeft;
		}

		inline RtFloat screenWindowRight() const
		{
			return m_screenWindowRight;
		}

		inline RtFloat screenWindowBottom() const
		{
			return m_screenWindowBottom;
		}

		inline RtFloat screenWindowTop() const
		{
			return m_screenWindowTop;
		}

		RtVoid cropWindow(RtFloat xmin, RtFloat xmax, RtFloat ymin, RtFloat ymax);
		RtVoid getCropWindow(RtFloat &xmin, RtFloat &xmax, RtFloat &ymin, RtFloat &ymax) const;
		RtVoid getCropWindow(RtInt &xmin, RtInt &xmax, RtInt &ymin, RtInt &ymax) const;

		RtVoid projection(RtToken name, const CParameterList &params);
		inline bool projectionCalled() const
		{
			return m_projectionCalled;
		}
		inline RtToken projectionName() const
		{
			return m_projectionName;
		}
		const CParameterList &projectionParams() const
		{
			return m_projectionParams;
		}
		inline RtFloat fov() const
		{
			return m_FOV;
		}

		RtVoid clipping(RtFloat hither, RtFloat yon);
		inline bool clippingCalled() const
		{
			return m_nearFarSet;
		}
		inline RtVoid getClipping(RtFloat &hither, RtFloat &yon) const
		{
			hither = m_nearClip;
			yon = m_farClip;
		}
		inline RtFloat hither() const
		{
			return m_nearClip;
		}
		inline RtFloat yon() const
		{
			return m_farClip;
		}

		RtVoid clippingPlane(RtFloat x, RtFloat y, RtFloat z, RtFloat nx, RtFloat ny, RtFloat nz);
		RtVoid clippingPlane(const CClippingPlane &s);
		inline const TypeClippingPlanes getClippingPlanes() const
		{
			return m_clippingPlanes;
		}
		inline TypeClippingPlanes::const_iterator clippingBegin() const
		{
			return m_clippingPlanes.begin();
		}
		inline TypeClippingPlanes::const_iterator clippingEnd() const
		{
			return m_clippingPlanes.end();
		}

		RtVoid depthOfField(RtFloat fstop, RtFloat focallength, RtFloat focaldistance);
		inline bool depthOfFieldCalled() const
		{
			return m_depthOfFieldCalled;
		}
		RtFloat fstop() const;
		RtFloat focallength() const;
		inline RtFloat COptions::focaldistance() const
		{
			return m_focalDistance;
		}
		void getDepthofField(RtFloat &fstop, RtFloat &focallength, RtFloat &focaldistance) const;
		RtFloat lensdiameter() const;

		RtVoid shutter(RtFloat smin, RtFloat smax);
		inline RtVoid getShutter(RtFloat &smin, RtFloat &smax) const
		{
			smin = m_shutterOpen;
			smax = m_shutterClose;
		}
		inline RtFloat shutterOpen() const
		{
			return m_shutterOpen;
		}
		inline RtFloat shutterClose() const
		{
			return m_shutterClose;
		}

		RtVoid pixelVariance(RtFloat variation);
		inline RtFloat pixelVariance() const
		{
			return m_pixelVariance;
		}

		RtVoid pixelSamples(RtFloat xsamples, RtFloat ysamples);
		inline RtVoid getPixelSamples(RtFloat &xsamples, RtFloat &ysamples) const
		{
			xsamples = m_xSamples;
			ysamples = m_ySamples;
		}
		inline RtFloat xPixelSamples() const
		{
			return m_xSamples;
		}
		inline RtFloat yPixelSamples() const
		{
			return m_ySamples;
		}

		RtVoid pixelFilter(const IFilterFunc &function, RtFloat xwidth, RtFloat ywidth);
		inline RtVoid getPixelFilter(const IFilterFunc *&function, RtFloat &xwidth, RtFloat &ywidth) const
		{
			function = 0;
			if ( m_filterFunc )
				function = &(m_filterFunc->singleton());
			xwidth = m_xWidth;
			ywidth = m_yWidth;
		}
		inline const IFilterFunc *pixelFilterFunction() const
		{
			return m_filterFunc;
		}
		inline const RtFloat xPixelFilterWidth() const
		{
			return m_xWidth;
		}
		inline const RtFloat yPixelFilterWidth() const
		{
			return m_yWidth;
		}

		RtVoid exposure(RtFloat again, RtFloat agamma);
		inline bool exposureCalled() const
		{
			return m_exposureCalled;
		}
		inline RtVoid getExposure(RtFloat &again, RtFloat &agamma) const
		{
			again = m_gain;
			agamma = m_gamma;
		}
		inline RtFloat gain() const
		{
			return m_gain;
		}
		inline RtFloat gamma() const
		{
			return m_gamma;
		}
		RtVoid colorExposure(RtColor color) const;
		RtVoid colorExposure(std::vector<RtFloat> &colors) const;

		RtVoid imager(RtToken name, const CParameterList &params);
		RtToken imagerName() const
		{
			return m_imagerName;
		}
		const CParameterList &imagerParams() const
		{
			return m_imagerParams;
		}

		RtVoid quantize(RtToken type, RtInt one, RtInt qmin, RtInt qmax, RtFloat ampl);
		const CQuantizer *quantizer(RtToken type) const;
		bool hasQuantizer(RtToken type) const;
		bool getQuantize(RtToken type, RtInt &one, RtInt &qmin, RtInt &qmax, RtFloat &ampl) const;

		RtVoid displayChannel(CDeclarationDictionary &dict, const CColorDescr &colorDescr, RtString channel, const CParameterList &params);
		inline CDisplayDescr::TypeDisplayChannels::const_iterator displayChannelBegin() const
		{
			return m_displayChannels.begin();
		}
		inline CDisplayDescr::TypeDisplayChannels::const_iterator displayChannelEnd() const
		{
			return m_displayChannels.end();
		}
		CDisplayDescr::TypeDisplayChannels::const_iterator findDisplayChannel(RtString channelName) const;

		RtVoid display(RtString name, RtToken type, RtString mode, const CParameterList &params);
		inline TypeDisplays::const_iterator displayBegin() const
		{
			return m_displays.begin();
		}
		inline TypeDisplays::const_iterator displayEnd() const
		{
			return m_displays.end();
		}
		TypeDisplays::const_iterator findDisplay(RtString name) const;

		RtVoid hider(RtToken type, const CParameterList &params);
		inline RtToken hiderType() const
		{
			return m_hiderType;
		}
		inline const CParameterList &hiderParams() const
		{
			return m_hiderParams;
		}

		RtVoid relativeDetail(RtFloat relativedetail);

		inline RtFloat relativeDetail() const
		{
			return m_relativeDetail;
		}
	}; // COptions

	

	/** @brief Factory to create new options sets
	 *  @see CAttributesFactory
	 */
	class COptionsFactory
	{
	protected:
		inline virtual COptions *newOptionsInstance() const
		{
			return new COptions;
		}

		inline virtual COptions *newOptionsInstance(const COptions &opt) const
		{
			return new COptions(opt);
		}

		inline virtual void deleteOptionsInstance(COptions *opt) const
		{
			if ( opt )
				delete opt;
		}

	public:
		inline virtual ~COptionsFactory() {}

		COptions *newOptions() const;
		COptions *newOptions(const COptions &opt) const;
		static void deleteOptions(COptions *opt);
	}; // COptionsFactory
}

#endif // _RICPP_RICONTEXT_OPTIONS_H
