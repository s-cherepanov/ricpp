#ifndef _RICPP_RENDERCONTEXT_RENDEROPTIONS_H
#define _RICPP_RENDERCONTEXT_RENDEROPTIONS_H

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

/*! \file renderoptions.h
 *  \brief Declaration of the option set of a render context
 *  \author Andreas Pidde (andreas@pidde.de)
 */


#include "baseclass.h"
#include "shader.h"
#include "optionmap.h"
#include "displaydesc.h"
#include "matrix3d.h"

#include <vector>

//! Struct for TRi::display() data
/*! Used to store the data for the current display in TRenderOptions, the handling
 *  of the displays must be changed (mutiple displays)
 */
struct TViewPort {
	RtInt   m_iOriginX,     //!< X-origin, initialized with 0.
		    m_iOriginY;     //!< Y-origin, initialized with 0.
	RtInt   m_iWidth,       //!< Width, initialized with -1: Size of device
	        m_iHeight;      //!< Height, initialized with -1: Size of device
	RtFloat m_fPixelAspect; //!< The pixel aspect ration, initialized with -1: Ratio of device pixel

	//! Constructor to set the initial values of the members
	inline TViewPort() { initialize(); }
	//! Sets the initial values of the members
	inline void initialize() {
		m_iOriginX = 0;
		m_iOriginY = 0;
		m_iWidth = -1;
		m_iHeight = -1;
		m_fPixelAspect = (RtFloat)-1.0;
	}
	//! The aspect ratio of the viewport
	/*! Calculates the physical aspect ratio of the viewport, works only if
	 *  m_iHeight and m_iWidth are set with values > 0.0.
	 *  \return Aspect ratio of the viewport: (m_iWidth*m_fPixelAspect)/(RtFloat)m_iHeight
	 */
	inline RtFloat viewPortAspectRatio() const {
		return (RtFloat)( m_iHeight == 0 ? 0.0 : (m_iWidth*m_fPixelAspect)/(RtFloat)m_iHeight );
	}
};

////////////////////////////////////////////////////////////////////////////////
//! Class to store a set of the options of the render interface
class TRenderOptions : public TBaseClass {
protected:
	//!< Assigns all members of o to own members
	//!< \param o TRenderOptions used to copy from
	void assign(const TRenderOptions &o);
public:

	TViewPort m_vpCurrentViewPort;  //!< Viewport data, is filled by the TIntermediateRenderer::establishProjection(), so
	                                //!< can be used in the world block.

	RtBoolean m_bFormatCalled;		//!< TRi::format() was called, default TRi::RI_FALSE
	RtInt	m_iDisplayHRes;			//!< Horizontal Resolution of last TRi::display() call, default: -1 (512, later by interface)
	RtInt	m_iDisplayVRes;			//!< Vertical Resolution of last TRi::display() call, default: -1 (384, later by interface)
	RtFloat	m_fDisplayPixelAspect;	//!< Pixel Aspect Ratio of last TRi::display() call, default: -1
	RtInt   m_iDisplayOriginX,      //!< X-Origin of last TRi::display() call, default: 0.0
		    m_iDisplayOriginY;      //!< Y-Origin of last TRi::display() call, default: 0.0

	RtBoolean m_bFrameAspectRatioCalled; //!< TRi::frameAspectRatio() was called
	RtFloat m_fFrameAspectRatio;         //!< Frame Aspect, default 4/3, later if known: (hres*pixelaspect)/vres

	RtBoolean m_bScreenWindowCalled;	//!< TRi::screenWindow was called
	RtFloat m_fScreenWindowLeft;		//!< Screen Coordinates, left edge, default -4/3
	RtFloat m_fScreenWindowRight;       //!< Screen Coordinates, right edge, default 4/3
	RtFloat m_fScreenWindowBottom;      //!< Screen Coordinates, bottom edge, default -1
	RtFloat m_fScreenWindowTop;         //!< Screen Coordinates, top edge, default 1

	RtBoolean m_bCropWindowCalled;      //!< TRi::cropWindow() was called
	RtFloat m_fCropWindowLeft;          //!< Crop Window (NDC), left edge, default 0
	RtFloat m_fCropWindowRight;         //!< Crop Window (NDC), right edge, default 1
	RtFloat m_fCropWindowBottom;        //!< Crop Window (NDC), bottom edge, default 0
	RtFloat m_fCropWindowTop;           //!< Crop Window (NDC), top edge, default 1
	RtInt   m_iRxMin,                   //!< Scissorbox, used for GL (Cropped Window Pixel Coordinates), left edge
	        m_iRxMax,                   //!< Scissorbox, used for GL (Cropped Window Pixel Coordinates), right edge
	        m_iRyMin,                   //!< Scissorbox, used for GL (Cropped Window Pixel Coordinates), top edge
	        m_iRyMax;                   //!< Scissorbox, used for GL (Cropped Window Pixel Coordinates), bottom edge

	RtBoolean m_bProjectionCalled;	//!< TRi::projection() is called
	RtToken m_tCameraProjection;	//!< Projection type, default "orthographic"
	RtBoolean m_bFOVSet;			//!< "FOV" is set in TRi::projection("perspective");
	RtFloat m_tCameraFOV;			//!< Field of View (perspective projection), default is 90.0 (degrees)

	RtBoolean m_bNearFarSet;		//!< Near and far clipping plane are set
	RtFloat m_fCameraNearClip;		//!< Near Clipping plane, default is epsilon
	RtFloat m_fCameraFarClip;		//!< Far Clipping plane, default is infinity

	RtFloat	m_fCameraFStop;			//!< f-Stop depth of field, default infinity
	RtFloat	m_fCameraFocalLength;	//!< focal length d.o.f, no default
	RtFloat	m_fCameraFocalDistance;	//!< focal distance d.o.f, no default
	RtFloat	m_fCameraShutterOpen;	//!< Shutter open, motion blur, default 0
	RtFloat	m_fCameraShutterClose;	//!< Shutter close, motion blur, default 0

	RtBoolean m_bPixelVarianceCalled;	//!< TRi::pixelVariance() is called
	RtFloat m_fPixelVariance;			//!< Accepted astimated variance of the pixel values from the true pixel values (no default)

	RtFloat m_fXSamples,                //!< sampling rate in horizontal direction 2
	        m_fYSamples;                //!< sampling rate in vertical direction 2

	RtFilterFunc m_pFilterFunc;         //!< Default filter func for TRi::pixelFilter(), default TRi::gausianFilter()
	RtFloat m_fXWidth,                  //!< X-width of the filter in pixels, default 2
	        m_fYWitdh;                  //!< Y-width of the filter in pixels, default 2

	RtFloat m_fGain,                    //!< Exposure (gain) color = (color*gain)**(1/gamma), default 1.0
	        m_fGamma;                   //!< Exposure (gamma) color = (color*gain)**(1/gamma), default 1.0

	RtToken m_tImager;					//!< Imager Shader, default RI_NULL
	TParameterMap m_imagerParamMap;		//!< Parameters of imager

	RtInt m_iOneRGBA,                   //!< Quantize RGBA, correspondence to one, default is 255
	      m_iMinRGBA,                   //!< Quantize RGBA, min value, default is 0
	      m_iMaxRGBA;                   //!< Quantize RGBA, max value, default is 255

	RtFloat m_fDitherAmplitudeRGBA;     //!< Dither amplitude for color quantization, default 0.5

	RtInt m_iOneZ,                      //!< Quantize depth (Z), correspondence to one, default is 255
	      m_iMinZ,                      //!< Quantize depth (Z), min value, default is 0
		  m_iMaxZ;                      //!< Quantize depth (Z), max value, default is 255

	RtFloat m_fDitherAmplitudeZ;        //!< Dither amplitude for depth quantization, default 0.5

	RtToken m_tHider;                   //!< Hiden surface algorithm, default "hidden" (others are "null" and "paint")
	TParameterMap m_hiderParamMap;      //!< Parameters of hider

	RtBoolean m_bColorSamplesCalled;    //!< TRi::colorSamples() called to redifine color components
	                                    //!< to be used by TRi::color() and TRi::Opacity()
	RtInt m_iNColor;                    //!< Number of color samples <= 3, default is 3
	std::vector<RtFloat> m_nRGB;        //!< Color conversion matrices from n-component to RGB
	std::vector<RtFloat> m_RGBn;        //!< Color conversion matrices from RGB to n-component

	RtFloat m_fRelativeDetail;          //!< Sets the relative Detail, default 1.0

	std::vector<TDisplayDesc> m_displays; //!< Current displays (TDisplayDesc), set by TRi::display()
	std::vector<RtFloat> m_cameraClips;   //!< Additional clipping planes struct SClippingPlane, default empty

	//!< Viewing matrix
	TMatrix3D m_mPreProjectionMatrix;

	// Add. options for RiCPP
	TMatrix3D m_mCamTrans;		//!< Additional: Camera pre transformation (add. ricpp option), default id matrix
	RtBoolean m_writeArchive;	//!< Additional: TRi::readArchive() writes out content of Archive, default TRi::RI_FALSE
	RtBoolean m_disableFormat;	//!< Additional: TRi::format() disable, default TRi::RI_FALSE
	RtBoolean m_writeObject;	//!< Additional: TRi::objectInstance() writes out content of an object, not only reference, default TRi::RI_FALSE

	//! List of implementation-specific options (TRi::option())
	TOptionMap m_optionMap;

	/////////////////////////////////////////////////////////////////////////////////////////
	// Member functions

	//!< Initialize all members with their default values
	void initialize();

	//!< Release all resources used by the members
	void releaseAll();

	//! Default constructor, initializes with default options
	inline TRenderOptions() { initialize(); }
	//! Copy constructor, deep copy
	/*! \param ro Render options used to copy from
	 */
	inline TRenderOptions(const TRenderOptions &ro) : TBaseClass(ro) { initialize(); assign(ro); }
	//! Destructor releases all resources
	inline virtual ~TRenderOptions() { releaseAll(); }

	//! static to return the class name of this class
	/*! \return class name "TRenderOptions"
	 */
	inline static const char *myClassName(void) {return "TRenderOptions";}
	//! Virtual to return the class name of this instance
	/*! \return class name of this instance
	 */
	inline virtual const char *className() const { return TRenderOptions::myClassName(); }
	//! Virtual to test inheritance
	/*! \param aClass, class name
	 *  \return class of the instance is a kind of the class aClass
	 */
	inline virtual bool isKindOf(const char *aClass) const {
		if ( !aClass )
			return false;
		if  ( !stricmp(aClass, TRenderOptions::myClassName()) ) {
			return true;
		}
		return TBaseClass::isKindOf(aClass);
	}

	//! Clones an instance
	/*! \return Reference to a clone
	 *  \exception TSystemException if no new instance could be created
	 */
	inline virtual TBaseClass &duplicate() const {
		TRenderOptions *o = new TRenderOptions(*this);
		if ( !o )
			throw TSystemException(__FILE__, __LINE__);
		return *o;
	}
	//! Assignment
	/*! \param o TRenderOption to assign from
	 *  \return *this
	 *  \exception TSystemException if o and this are not compatible for assignment
	 */
	inline virtual TBaseClass &operator=(const TBaseClass &o) {
		if ( &o == this )
			return *this;
		if ( !o.isKindOf(TRenderOptions::myClassName()) ) {
			// Throw illegal type for assignment
			throw TSystemException(TSystemException::m_errIllegalTypename(), __FILE__, __LINE__);
			return *this;
		}
		assign((const TRenderOptions &)o);
		return TBaseClass::operator =(o);
	}
	
	//! Translates cutom colors to RGB using m_nRGB
	/*! \retval rgb Stores the RGB color values here
	 *  \param color Custom colors to translate from
	 *  \param n number of colors to translate
	 */
	void colorRGB(RtFloat *rgb, const RtFloat *color, int n=1) const;
	//! Translates RGB to cutom colors using m_RGBn
	/*! \retval color Custom colors to translate from
	 *  \parm rgb RGB color values to translate from
	 *  \param n number of colors to translate
	 */
	void colorCustom(RtFloat *color, const RtFloat *rgb, int n=1) const;
}; // TRenderOptions

#endif // _RICPP_RENDERCONTEXT_RENDEROPTIONS_H

