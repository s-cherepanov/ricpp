#ifndef _RICPP_RENDERSTATE_OPTIONS_H
#define _RICPP_RENDERSTATE_OPTIONS_H

#ifndef _RICPP_RENDERSTATE_OPTIONSBASE_H
#include "ricpp/renderstate/optionsbase.h"
#endif //  _RICPP_RENDERSTATE_OPTIONSBASE_H

#ifndef _RICPP_RENDERSTATE_DISPLAYDESC_H
#include "ricpp/renderstate/displaydesc.h"
#endif //  _RICPP_RENDERSTATE_DISPLAYDESC_H

#ifndef _RICPP_RICPP_FILTERS_H
#include "ricpp/ricpp/filters.h"
#endif // _RICPP_RICPP_FILTERS_H

namespace RiCPP {

	const RtInt   defXResolution = 640;
	const RtInt   defYResolution = 480;
	const RtFloat defPixelAspectRatio = 1.0;

	const RtFloat defFrameAspectRatio = (defXResolution * defPixelAspectRatio) /
	                                        static_cast<RtFloat>(defYResolution);

	const RtFloat defScreenWindowLeft = -defFrameAspectRatio;
	const RtFloat defScreenWindowRight = defFrameAspectRatio;
	const RtFloat defScreenWindowBottom = -1.0;
	const RtFloat defScreenWindowTop = 1.0;

	const RtFloat defCropWindowLeft = defScreenWindowLeft;
	const RtFloat defCropWindowRight = defScreenWindowRight;
	const RtFloat defCropWindowBottom = defScreenWindowBottom;
	const RtFloat defCropWindowTop = defScreenWindowTop;

	const RtToken defProjection = RI_ORTHOGRAPHIC;
	const RtFloat defCameraFOV = 90.0;

	const RtFloat defNearClip = RI_EPSILON;
	const RtFloat defFarClip = RI_INFINITY;

	const RtFloat defFstop = RI_INFINITY;
	const RtFloat defFocalLength = 0;
	const RtFloat defFocalDistance = 0;

	const RtFloat defShutterOpen = 0;
	const RtFloat defShutterClose = 0;

	const RtFloat defPixelVariance = 0;

	const RtFloat defXSamples = 2;
	const RtFloat defYSamples = 2;

	const RtFloat defXFilterWidth = 2;
	const RtFloat defYFilterWidth = 2;

	const RtFloat defGain = 1;
	const RtFloat defGamma = 1;

	const RtInt defOneRGBA = 255;
	const RtInt defMinRGBA = 0;
	const RtInt defMaxRGBA = 255;
	const RtFloat defDitherAmplitudeRGBA = 0.5;

	const RtInt defOneZ = 0;
	const RtInt defMinZ = 0;
	const RtInt defMaxZ = 0;
	const RtFloat defDitherAmplitudeZ = 0;

	const RtToken defImagerName = RI_NULL;

	const RtToken defHiderType = RI_HIDDEN;

	const RtFloat defRelativeDetail = 1.0;

	class COptionsReader;

	/** @brief
	 */
	class COptions : public COptionsBase {
	public:
		typedef std::vector<CClippingPlane> ClippingPlanes_type;
		typedef std::list<CDisplayDescr> Displays_type;
	private:
		COptionsReader *m_reader;

		// CViewPort m_curViewPort;      //!< Viewport data

		CDisplayDescr::DisplayChannels_type m_displayChannels; //!< Display channels can be as mode by display
		Displays_type m_displays; //!< Current displays (CDisplayDescr), set by CRi::display()

		std::map<RtToken, CQuantizer> m_quantizers; //!< Quantizer (def. "RGBA" and "z")

		ClippingPlanes_type m_clippingPlanes;   //!< Additional clipping planes

		bool    m_formatCalled;        //!< true, if CRi::format() has been called
		RtInt	m_xResolution;		   //!< Horizontal Resolution of last CRi::display() call
		RtInt	m_yResolution;		   //!< Vertical Resolution of last CRi::display() call
		RtFloat	m_pixelAspectRatio;    //!< Pixel Aspect Ratio of last CRi::display() call

		bool m_frameAspectRatioCalled; //!< true, if CRi::frameAspectRatio() has been called
		RtFloat m_frameAspectRatio;    //!< Frame Aspect Ratio, only valid if CRi::frameAspectRatio() called

		bool m_screenWindowCalled;     //!< true, if CRi::screenWindow() has been called
		RtFloat m_screenWindowLeft;    //!< Screen coordinates, left edge, only valid if CRi::screenWindow() called
		RtFloat m_screenWindowRight;   //!< Screen coordinates, right edge, only valid if CRi::screenWindow() called
		RtFloat m_screenWindowBottom;  //!< Screen coordinates, bottom edge, only valid if CRi::screenWindow() called
		RtFloat m_screenWindowTop;     //!< Screen coordinates, top edge, only valid if CRi::screenWindow() called

		bool m_cropWindowCalled;    //!< true, if CRi::cropWindow() has been called
		RtFloat m_cropWindowLeft;   //!< Crop Window (NDC), left edge, only valid if CRi::cropWindow() called (otherwise whol raster window is rendered)
		RtFloat m_cropWindowRight;  //!< Crop Window (NDC), right edge, only valid if CRi::cropWindow() called (otherwise whol raster window is rendered)
		RtFloat m_cropWindowBottom; //!< Crop Window (NDC), bottom edge, only valid if CRi::cropWindow() called (otherwise whol raster window is rendered)
		RtFloat m_cropWindowTop;    //!< Crop Window (NDC), top edge, only valid if CRi::cropWindow() called (otherwise whol raster window is rendered)

		bool m_projectionCalled;           //!< CRi::projection() is called
		RtToken m_projectionName;          //!< Projection type, default "orthographic"
		CParameterList m_projectionParams; //!< Projection parameters
		RtBoolean m_FOVSet;                //!< RI_FOV is set in CRi::projection("perspective");
		RtFloat m_FOV;                     //!< Field of View (perspective projection), default is 90.0 (degrees)

		bool m_nearFarSet;          //!< Near and far clipping plane are set
		RtFloat m_nearClip;         //!< Near Clipping plane, default is epsilon
		RtFloat m_farClip;          //!< Far Clipping plane, default is infinity

		bool m_depthOfFieldCalled;  //!< CRi::depthOfField() has been called
		RtFloat	m_fstop;			//!< f-Stop (aperture number) for depth of field
		RtFloat	m_focalLength;      //!< focal length for DOF
		RtFloat	m_focalDistance;    //!< focal distance for DOF

		RtFloat	m_shutterOpen;      //!< Shutter open, motion blur, default 0
		RtFloat	m_shutterClose;     //!< Shutter close, motion blur, default 0

		bool m_pixelVarianceCalled;	//!< CRi::pixelVariance() has been called
		RtFloat m_pixelVariance;	//!< Accepted astimated variance of the pixel values from the true pixel values (no default)

		RtFloat m_xSamples,         //!< sampling rate in horizontal direction 2
				m_ySamples;         //!< sampling rate in vertical direction 2

		IFilterFunc *m_filterFunc;  //!< Default filter func for CRi::pixelFilter(), default CGaussianFilter
		RtFloat m_xWidth,           //!< X-width of the filter in pixels, default 2
				m_yWidth;           //!< Y-width of the filter in pixels, default 2

		bool m_exposureCalled;		//!< CRi::exposure() has been called

		CParameterList m_imagerParams; //!< Imager Shader, default no params
		RtToken m_imagerName;          //!< Name of Imager Shader, default RI_NULL

		RtFloat m_gain,             //!< Exposure (gain) color = (color*gain)**(1/gamma), default 1.0
				m_gamma;            //!< Exposure (gamma) color = (color*gain)**(1/gamma), default 1.0

		RtToken m_hiderType;          //!< Type of the hidden surface algorithm
		CParameterList m_hiderParams; //!< Hidden surface algorithm, default "hidden" (others are "null" and "paint")

		RtFloat m_relativeDetail;   //!< Sets the relative Detail, default 1.0

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

	protected:
		virtual void deleteReader(COptionsReader *reader);
		virtual COptionsReader *newReader();

	public:
		inline COptions()
		{
			m_reader = 0;
			m_filterFunc = 0;
			init();
		}

		inline COptions(const COptions &ro)
		{
			m_reader = 0;
			m_filterFunc = 0;
			*this = ro;
		}

		virtual ~COptions();

		inline const COptionsReader &createReader()
		{
			if ( !m_reader ) {
				m_reader = newReader();
				if ( !m_reader ) {
					// throw;
				}
			}
			return *m_reader;
		}

		inline const COptionsReader *reader() const
		{
				return m_reader;
		}

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
		inline const ClippingPlanes_type getClippingPlanes() const
		{
			return m_clippingPlanes;
		}
		inline ClippingPlanes_type::const_iterator clippingBegin() const
		{
			return m_clippingPlanes.begin();
		}
		inline ClippingPlanes_type::const_iterator clippingEnd() const
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
			function = m_filterFunc;
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

		RtVoid displayChannelV(CDeclarationDictionary &dict, RtString channel, RtInt n, RtToken tokens[], RtPointer params[]);
		inline CDisplayDescr::DisplayChannels_type::const_iterator displayChannelBegin() const
		{
			return m_displayChannels.begin();
		}
		inline CDisplayDescr::DisplayChannels_type::const_iterator displayChannelEnd() const
		{
			return m_displayChannels.end();
		}
		CDisplayDescr::DisplayChannels_type::const_iterator findDisplayChannel(RtString channelName) const;

		RtVoid displayV(CDeclarationDictionary &dict, RtString name, RtToken type, RtString mode, RtInt n, RtToken tokens[], RtPointer params[]);
		inline Displays_type::const_iterator displayBegin() const
		{
			return m_displays.begin();
		}
		inline Displays_type::const_iterator displayEnd() const
		{
			return m_displays.end();
		}
		Displays_type::const_iterator findDisplay(RtString name) const;

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

	
	/** @brief
	 */
	class COptionsReader : public COptionsBaseReader {
	private:
		const COptions *m_riOptions;

	public:
		COptionsReader(COptions &options)
			: COptionsBaseReader(options)
		{
			m_riOptions = &options;
		}

		inline bool formatCalled() const
		{
			return m_riOptions->formatCalled();
		}

		inline RtInt xResolution() const
		{
			return m_riOptions->xResolution();
		}

		inline RtInt yResolution() const
		{
			return m_riOptions->yResolution();
		}

		inline RtFloat pixelAspectRatio() const
		{
			return m_riOptions->pixelAspectRatio();
		}

		inline RtVoid getFormat(RtInt &xres, RtInt &yres, RtFloat &aspect) const
		{
			m_riOptions->getFormat(xres, yres, aspect);
		}

		inline RtFloat frameAspectRatio() const
		{
			return m_riOptions->frameAspectRatio();
		}

		inline bool frameAspectRatioCalled() const
		{
			return m_riOptions->frameAspectRatioCalled();
		}

		inline RtVoid getScreenWindow(RtFloat &left, RtFloat &right, RtFloat &bot, RtFloat &top) const
		{
			m_riOptions->getScreenWindow(left, right, bot, top);
		}

		inline RtFloat screenWindowLeft() const
		{
			return m_riOptions->screenWindowLeft();
		}

		inline RtFloat screenWindowRight() const
		{
			return m_riOptions->screenWindowRight();
		}

		inline RtFloat screenWindowBottom() const
		{
			return m_riOptions->screenWindowBottom();
		}

		inline RtFloat screenWindowTop() const
		{
			return m_riOptions->screenWindowTop();
		}

		inline RtVoid getCropWindow(RtFloat &xmin, RtFloat &xmax, RtFloat &ymin, RtFloat &ymax) const
		{
			return m_riOptions->getCropWindow(xmin, xmax, ymin, ymax);
		}

		inline RtVoid getCropWindow(RtInt &xmin, RtInt &xmax, RtInt &ymin, RtInt &ymax) const
		{
			m_riOptions->getCropWindow(xmin, xmax, ymin, ymax);
		}

		inline bool projectionCalled() const
		{
			return m_riOptions->projectionCalled();
		}

		inline RtToken projectionName() const
		{
			return m_riOptions->projectionName();
		}

		const CParameterList &projectionParams() const
		{
			return m_riOptions->projectionParams();
		}

		inline RtFloat fov() const
		{
			return m_riOptions->fov();
		}

		inline bool clippingCalled() const
		{
			return m_riOptions->clippingCalled();
		}

		inline RtVoid getClipping(RtFloat &hither, RtFloat &yon) const
		{
			m_riOptions->getClipping(hither, yon);
		}

		inline RtFloat hither() const
		{
			return m_riOptions->hither();
		}

		inline RtFloat yon() const
		{
			return m_riOptions->yon();
		}

		inline const COptions::ClippingPlanes_type getClippingPlanes() const
		{
			return m_riOptions->getClippingPlanes();
		}

		inline COptions::ClippingPlanes_type::const_iterator clippingBegin() const
		{
			return m_riOptions->clippingBegin();
		}

		inline COptions::ClippingPlanes_type::const_iterator clippingEnd() const
		{
			return m_riOptions->clippingEnd();
		}

		inline RtFloat fstop() const
		{
			return m_riOptions->fstop();
		}

		inline RtFloat focallength() const
		{
			return m_riOptions->focallength();
		}

		inline RtFloat focaldistance() const
		{
			return m_riOptions->focaldistance();
		}

		inline void getDepthofField(RtFloat &fstop, RtFloat &focallength, RtFloat &focaldistance) const
		{
			return m_riOptions->getDepthofField(fstop, focallength, focaldistance);
		}

		inline RtFloat lensdiameter() const
		{
			return m_riOptions->lensdiameter();
		}

		inline RtVoid getShutter(RtFloat &smin, RtFloat &smax) const
		{
			m_riOptions->getShutter(smin, smax);
		}

		inline RtFloat shutterOpen() const
		{
			return m_riOptions->shutterOpen();
		}

		inline RtFloat shutterClose() const
		{
			return m_riOptions->shutterClose();
		}

		inline RtFloat pixelVariance() const
		{
			return m_riOptions->pixelVariance();
		}

		inline RtVoid getPixelSamples(RtFloat &xsamples, RtFloat &ysamples) const
		{
			m_riOptions->getPixelSamples(xsamples, ysamples);
		}

		inline RtFloat xPixelSamples() const
		{
			return m_riOptions->xPixelSamples();
		}
		inline RtFloat yPixelSamples() const
		{
			return m_riOptions->yPixelSamples();
		}

		inline RtVoid getPixelFilter(const IFilterFunc *&function, RtFloat &xwidth, RtFloat &ywidth) const
		{
			m_riOptions->getPixelFilter(function, xwidth, ywidth);
		}
		inline const IFilterFunc *pixelFilterFunction() const
		{
			return m_riOptions->pixelFilterFunction();
		}
		inline const RtFloat xPixelFilterWidth() const
		{
			return m_riOptions->xPixelFilterWidth();
		}
		inline const RtFloat yPixelFilterWidth() const
		{
			return m_riOptions->yPixelFilterWidth();
		}

		inline RtVoid getExposure(RtFloat &aGain, RtFloat &aGamma) const
		{
			m_riOptions->getExposure(aGain, aGamma);
		}
		inline bool exposureCalled() const
		{
			return m_riOptions->exposureCalled();
		}
		inline RtFloat gain() const
		{
			return m_riOptions->gain();
		}
		inline RtFloat gamma() const
		{
			return m_riOptions->gamma();
		}
		inline RtVoid colorExposure(RtColor color) const
		{
			return m_riOptions->colorExposure(color);
		}
		inline RtVoid colorExposure(std::vector<RtFloat> &colors) const
		{
			return m_riOptions->colorExposure(colors);
		}

		RtToken imagerName() const
		{
			return m_riOptions->imagerName();
		}
		const CParameterList &imagerParams() const
		{
			return m_riOptions->imagerParams();
		}

		inline const CQuantizer *quantizer(RtToken type) const
		{
			return m_riOptions->quantizer(type);
		}

		inline bool hasQuantizer(RtToken type) const
		{
			return m_riOptions->hasQuantizer(type);
		}

		inline bool getQuantize(RtToken type, RtInt &one, RtInt &qmin, RtInt &qmax, RtFloat &ampl) const
		{
			return m_riOptions->getQuantize(type, one, qmin, qmax, ampl);
		}

		inline CDisplayDescr::DisplayChannels_type::const_iterator displayChannelBegin() const
		{
			return m_riOptions->displayChannelBegin();
		}

		inline CDisplayDescr::DisplayChannels_type::const_iterator displayChannelEnd() const
		{
			return m_riOptions->displayChannelEnd();
		}

		inline CDisplayDescr::DisplayChannels_type::const_iterator findDisplayChannel(RtString channelName) const
		{
			return m_riOptions->findDisplayChannel(channelName);
		}

		inline COptions::Displays_type::const_iterator displayBegin() const
		{
			return m_riOptions->displayBegin();
		}

		inline COptions::Displays_type::const_iterator displayEnd() const
		{
			return m_riOptions->displayEnd();
		}

		inline COptions::Displays_type::const_iterator findDisplay(RtString name) const
		{
			return m_riOptions->findDisplay(name);
		}

		inline RtToken hiderType() const
		{
			return m_riOptions->hiderType();
		}

		inline const CParameterList &hiderParams() const
		{
			return m_riOptions->hiderParams();
		}

		inline RtFloat relativeDetail() const
		{
			return m_riOptions->relativeDetail();
		}
	}; // COptionsReader

	/** @brief
	 */
	class COptionsFactory
	{
	protected:
		inline virtual COptions *newOptionsInstance()
		{
			return new COptions;
		}

		inline virtual COptions *newOptionsInstance(const COptions &opt)
		{
			return new COptions(opt);
		}

	public:
		inline virtual ~COptionsFactory() {}

		inline virtual COptions *newOptions()
		{
			COptions *o = newOptionsInstance();
			if ( o ) {
				o->createReader();
			} else {
				// throw
			}
			return o;
		}

		inline virtual COptions *newOptions(const COptions &opt)
		{
			COptions *o = new COptions(opt);
			if ( o ) {
				o->createReader();
			} else {
				// throw
			}
			return o;
		}

		inline virtual void deleteOptions(COptions *o)
		{
			if ( o )
				delete o;
		}
	}; // COptionsFactory
}

#endif // _RICPP_RENDERSTATE_OPTIONS_H
