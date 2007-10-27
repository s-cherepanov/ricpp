#ifndef _RICPP_RENDERSTATE_RIMACROOPTIONS_H
#define _RICPP_RENDERSTATE_RIMACROOPTIONS_H

#ifndef _RICPP_RENDERSTATE_RIMACROBASE_H
#include "ricpp/renderstate/rimacrobase.h"
#endif // _RICPP_RENDERSTATE_RIMACROBASE_H

namespace RiCPP {

// ----------------------------------------------------------------------------
// Options
// ----------------------------------------------------------------------------

///////////////////////////////////////////////////////////////////////////////
/** @brief The raster resolution and pixel aspect ratio of the image to be rendered.
 */
class CRiFormat : public CRManInterfaceCall {
private:
	RtInt m_xres,     //!< Width of the image to be rendered.
	      m_yres;     //!< Height of the image to be rendered.
	RtFloat m_aspect; //!< Pixel aspect ratio (width to the height).

public:
	/** @brief Gets the name for the class.
	 *
	 *  @return The name of the class (can be used as atomized string)
	 */
	inline static const char *myClassName(void) { return "CRiFormat"; }

	inline virtual const char *className() const { return CRiFormat::myClassName(); }

	inline virtual bool isA(const char *atomizedClassName) const
	{
		return ( atomizedClassName == myClassName() );
	}

	inline virtual bool isKindOf(const char *atomizedClassName) const
	{
		if ( atomizedClassName == myClassName() )
			return true;
		return CRManInterfaceCall::isKindOf(atomizedClassName);
	}

	/** @brief Default Constructor.
	 *
	 *  @param aLineNo The line number to store, if aLineNo is initialized to -1 (a line number is not known)
	 *  @param aXRes The x (horizontal) resolution of the image to be rendered.
	 *  @param aYRes The y (vertical) resolution of the image to be rendered.
	 *  @param anAspect The pixel aspect ratio of the image to be rendered.
	 */
	inline CRiFormat(
		long aLineNo = -1,
		RtInt aXRes = COptions::defXResolution,
		RtInt aYRes = COptions::defYResolution,
		RtFloat anAspect = COptions::defPixelAspectRatio) :
		CRManInterfaceCall(aLineNo), m_xres(aXRes), m_yres(aYRes), m_aspect(anAspect)
	{
	}

	/** @brief Copy constructor.
	 *
	 *  @param c Object to copy.
	 */
	inline CRiFormat(const CRiFormat &c)
	{
		*this = c;
	}

	/** @brief Destructor.
	 */
	inline virtual ~CRiFormat()
	{
	}

	inline virtual CRManInterfaceCall *duplicate() const
	{
		return new CRiFormat(*this);
	}

	inline virtual EnumRequests interfaceIdx() const { return REQ_FORMAT; }

	/** @brief Gets the x (horizontal) resolution of the image.
	 *
	 *  @return The x (horizontal) resolution of the image to be rendered.
	 */
	inline RtInt xres() const
	{
		return m_xres;
	}

	/** @brief Sets the x (horizontal) resolution of the image.
	 *
	 *  @param aXRes The x (horizontal) resolution of the image to be rendered.
	 */
	inline void xres(RtInt aXRes)
	{
		m_xres = aXRes;
	}

	/** @brief Gets the y (vertical) resolution of the image.
	 *
	 *  @return The y (vertical) resolution of the image to be rendered.
	 */
	inline RtInt yres() const
	{
		return m_yres;
	}

	/** @brief Sets the y (vertical) resolution of the image.
	 *
	 *  @param aYRes The y (vertical) resolution of the image to be rendered.
	 */
	inline void yres(RtInt aYRes)
	{
		m_yres = aYRes;
	}

	/** @brief Gets the pixel aspect ratio of the image.
	 *
	 * The pixel aspect ration matches a physical display.
	 * The viewport aspect ratio is calculated by (xres() * aspect())/yres()
	 *
	 *  @return The pixel aspect ratio of the image to be rendered.
	 */
	inline RtFloat aspect() const
	{
		return m_aspect;
	}

	/** @brief Sets the pixel aspect ratio of the image.
	 *
	 *  @param anAspect The pixel aspect ratio of the image to be rendered.
	 */
	inline void aspect(RtFloat anAspect)
	{
		m_aspect = anAspect;
	}

	/** @brief Gets the values of the screen format.
	 *
	 *  @retval aXRes The x (horizontal) resolution of the image to be rendered.
	 *  @retval aYRes The y (vertical) resolution of the image to be rendered.
	 *  @retval anAspect The pixel aspect ratio of the image to be rendered.
	 */
	inline void set(
		RtInt &aXRes,
		RtInt &aYRes,
		RtFloat &anAspect) const
	{
		aXRes = m_xres;
		aYRes = m_yres;
		anAspect = m_aspect;
	}

	/** @brief Sets the values of the screen format.
	 *
	 *  @param aXRes The x (horizontal) resolution of the image to be rendered.
	 *  @param aYRes The y (vertical) resolution of the image to be rendered.
	 *  @param anAspect The pixel aspect ratio of the image to be rendered.
	 */
	inline void set(
		RtInt aXRes,
		RtInt aYRes,
		RtFloat anAspect)
	{
		m_xres = aXRes;
		m_yres = aYRes;
		m_aspect = anAspect;
	}

	inline virtual void preProcess(IDoRender &ri, const IArchiveCallback *cb)
	{
		ri.preFormat(m_xres, m_yres, m_aspect);
	}

	inline virtual void doProcess(IDoRender &ri, const IArchiveCallback *cb)
	{
		ri.doFormat(m_xres, m_yres, m_aspect);
	}

	inline virtual void postProcess(IDoRender &ri, const IArchiveCallback *cb)
	{
		ri.postFormat(m_xres, m_yres, m_aspect);
	}

	/** @brief Assignment.
	 *
	 *  @param c CRManInterfaceCall to assign
	 *  @return A reference to this object.
	 */
	inline CRiFormat &operator=(const CRiFormat &c)
	{
		if ( this == &c )
			return *this;

		set(c.xres(), c.yres(), c.aspect());

		CRManInterfaceCall::operator=(c);
		return *this;
	}
}; // CRiFormat


///////////////////////////////////////////////////////////////////////////////
/** @brief Frame aspect ratio of the image to be rendered
 *
 * If the frame aspect ration do not match the setting made by calling a IRi::display()
 * and IRi::format(), die image should be aligned with the upper left corner (origin)
 * of the image so that the longer edge of the image fills the appropriate edge of the display.
 *
 * If a IRi::frameAspectRatio() is not called, the frame aspect is calculated by the settings
 * made by IRi::format().
 */
class CRiFrameAspectRatio : public CRManInterfaceCall {
private:
	RtFloat m_aspect; //!< frame aspect ratio

public:
	/** @brief Gets name for the class.
	 *
	 *  @return The name of the class (can be used as atomized string)
	 */
	inline static const char *myClassName(void) { return "CRiFrameAspectRatio"; }

	inline virtual const char *className() const { return CRiFrameAspectRatio::myClassName(); }

	inline virtual bool isA(const char *atomizedClassName) const
	{
		return ( atomizedClassName == myClassName() );
	}

	inline virtual bool isKindOf(const char *atomizedClassName) const
	{
		if ( atomizedClassName == myClassName() )
			return true;
		return CRManInterfaceCall::isKindOf(atomizedClassName);
	}

	/** @brief Default Constructor.
	 *
	 *  @param aLineNo The line number to store, if aLineNo is initialized to -1 (a line number is not known)
	 *  @param anAspect The frame aspect ratio of the image to be rendered.
	 */
	inline CRiFrameAspectRatio(
		long aLineNo = -1,
		RtFloat aspect = COptions::defFrameAspectRatio) :
		CRManInterfaceCall(aLineNo), m_aspect(aspect)
	{
	}

	/** @brief Copy constructor.
	 *
	 *  @param c Object to copy.
	 */
	inline CRiFrameAspectRatio(const CRiFrameAspectRatio &c)
	{
		*this = c;
	}

	/** @brief Destructor.
	 */
	inline virtual ~CRiFrameAspectRatio()
	{
	}

	inline virtual CRManInterfaceCall *duplicate() const
	{
		return new CRiFrameAspectRatio(*this);
	}

	inline virtual EnumRequests interfaceIdx() const { return REQ_FRAME_ASPECT_RATIO; }
	
	/** @brief Gets the frame aspect ratio of the image.
	 *
	 *  @return The frame aspect ratio of the image to be rendered.
	 */
	inline RtFloat aspect() const
	{
		return m_aspect;
	}

	/** @brief Sets the frame aspect ratio of the image.
	 *
	 *  @param anAspect The frame aspect ratio of the image to be rendered.
	 */
	inline void aspect(RtFloat anAspect)
	{
		m_aspect = anAspect;
	}

	inline virtual void preProcess(IDoRender &ri, const IArchiveCallback *cb)
	{
		ri.preFrameAspectRatio(m_aspect);
	}

	inline virtual void doProcess(IDoRender &ri, const IArchiveCallback *cb)
	{
		ri.doFrameAspectRatio(m_aspect);
	}

	inline virtual void postProcess(IDoRender &ri, const IArchiveCallback *cb)
	{
		ri.postFrameAspectRatio(m_aspect);
	}

	/** @brief Assignment.
	 *
	 *  @param c CRManInterfaceCall to assign.
	 *  @return A reference to this object.
	 */
	inline CRiFrameAspectRatio &operator=(const CRiFrameAspectRatio &c)
	{
		if ( this == &c )
			return *this;

		aspect(c.aspect());

		CRManInterfaceCall::operator=(c);
		return *this;
	}
}; // CRiFrameAspectRatio

///////////////////////////////////////////////////////////////////////////////
/** @brief Sets the size of the screen window (in screen coordinates)
 *
 *  Has to match the frame aspect ratio (frameaspect = abs(right-left)/abs(top-bot))
 *  to avoid distortion at the displayed image. If the
 *  IRi::screenWindow() is nor called, the values default to
 *  frameaspectratio >= 1: (-frameaspectratio, frameaspectratio, -1, 1), and
 *  frameaspectratio < 1: (-1, 1, -1/frameaspectratio, 1/frameaspectratio).
 *  For perspective projection the smaller range will be centered at the displayed
 *  image.
 */
class CRiScreenWindow : public CRManInterfaceCall {
private:
	RtFloat m_left,   //!< coordinate of the left screen window edge
	        m_right,  //!< coordinate of the right screen window edge
	        m_bottom, //!< coordinate of the bottom screen window edge
	        m_top;    //!< coordinate of the top screen window edge

public:
	/** @brief Gets name for the class.
	 *
	 *  @return The name of the class (can be used as atomized string)
	 */
	inline static const char *myClassName(void) { return "CRiScreenWindow"; }

	inline virtual const char *className() const { return CRiScreenWindow::myClassName(); }

	inline virtual bool isA(const char *atomizedClassName) const
	{
		return ( atomizedClassName == myClassName() );
	}

	inline virtual bool isKindOf(const char *atomizedClassName) const
	{
		if ( atomizedClassName == myClassName() )
			return true;
		return CRManInterfaceCall::isKindOf(atomizedClassName);
	}

	/** @brief Default Constructor.
	 *
	 *  @param aLineNo The line number to store, if aLineNo is initialized to -1 (a line number is not known)
	 *  @param aLeft The left edge of the screen window (screen coordinates)
	 *  @param aRight The right edge of the screen window (screen coordinates)
	 *  @param aBot The bottom edge of the screen window (screen coordinates)
	 *  @param aTop The top edge of the screen window (screen coordinates)
	 */
	inline CRiScreenWindow(
		long aLineNo = -1,
		RtFloat aLeft = COptions::defScreenWindowLeft,
		RtFloat aRight = COptions::defScreenWindowRight,
		RtFloat aBottom = COptions::defScreenWindowBottom,
		RtFloat aTop = COptions::defScreenWindowTop ) :
		CRManInterfaceCall(aLineNo),
		m_left(aLeft), m_right(aRight), m_bottom(aBottom), m_top(aTop)
	{
	}

	/** @brief Copy constructor.
	 *
	 *  @param c Object to copy.
	 */
	inline CRiScreenWindow(const CRiScreenWindow &c)
	{
		*this = c;
	}

	/** @brief Destructor.
	 */
	inline virtual ~CRiScreenWindow()
	{
	}

	inline virtual CRManInterfaceCall *duplicate() const
	{
		return new CRiScreenWindow(*this);
	}

	inline virtual EnumRequests interfaceIdx() const { return REQ_SCREEN_WINDOW; }

	/** @brief Gets the left edge.
	 *
	 *  @return The left edge of the screen window (screen coordinates).
	 */
	inline RtFloat left() const
	{
		return m_left;
	}

	/** @brief Sets the left edge.
	 *
	 *  @param aLeft The left edge of the screen window (screen coordinates).
	 */
	inline void left(RtFloat aLeft)
	{
		m_left = aLeft;
	}

	/** @brief Gets the left edge.
	 *
	 *  @return The right edge of the screen window (screen coordinates).
	 */
	inline RtFloat right() const
	{
		return m_right;
	}

	/** @brief Sets the right edge.
	 *
	 *  @param aRight The right edge of the screen window (screen coordinates).
	 */
	inline void right(RtFloat aRight)
	{
		m_right = aRight;
	}

	/** @brief Gets the bottom edge.
	 *
	 *  @return The bottom edge of the screen window (screen coordinates).
	 */
	inline RtFloat bottom() const
	{
		return m_bottom;
	}

	/** @brief Sets The bottom edge.
	 *
	 *  @param aBot The bottom edge of the screen window (screen coordinates).
	 */
	inline void bottom(RtFloat aBottom)
	{
		m_bottom = aBottom;
	}

	/** @brief Gets the top edge.
	 *
	 *  @return The top edge of the screen window (screen coordinates).
	 */
	inline RtFloat top() const
	{
		return m_top;
	}

	/** @brief Sets The top edge.
	 *
	 *  @param aTop The top edge of the screen window (screen coordinates).
	 */
	inline void top(RtFloat aTop)
	{
		m_top = aTop;
	}

	/** @brief Gets the values of the screen window.
	 *
	 *  @retval aLeft The left edge of the screen window (screen coordinates)
	 *  @retval aRight The right edge of the screen window (screen coordinates)
	 *  @retval aBot The bottom edge of the screen window (screen coordinates)
	 *  @retval aTop The top edge of the screen window (screen coordinates)
	 */
	inline void get(
		RtFloat &aLeft,
		RtFloat &aRight,
		RtFloat &aBottom,
		RtFloat &aTop) const
	{
		aLeft = m_left;
		aRight = m_right;
		aBottom = m_bottom;
		aTop = m_top;
	}

	/** @brief Sets the values of the screen window.
	 *
	 *  @param aLeft The left edge of the screen window (screen coordinates)
	 *  @param aRight The right edge of the screen window (screen coordinates)
	 *  @param aBot The bottom edge of the screen window (screen coordinates)
	 *  @param aTop The top edge of the screen window (screen coordinates)
	 */
	inline void set(
		RtFloat aLeft,
		RtFloat aRight,
		RtFloat aBottom,
		RtFloat aTop)
	{
		m_left = aLeft;
		m_right = aRight;
		m_bottom = aBottom;
		m_top = aTop;
	}

	inline virtual void preProcess(IDoRender &ri, const IArchiveCallback *cb)
	{
		ri.preScreenWindow(m_left, m_right, m_bottom, m_top);
	}

	inline virtual void doProcess(IDoRender &ri, const IArchiveCallback *cb)
	{
		ri.doScreenWindow(m_left, m_right, m_bottom, m_top);
	}

	inline virtual void postProcess(IDoRender &ri, const IArchiveCallback *cb)
	{
		ri.postScreenWindow(m_left, m_right, m_bottom, m_top);
	}

	/** @brief Assignment.
	 *
	 *  @param c CRManInterfaceCall to assign.
	 *  @return A reference to this object.
	 */
	inline CRiScreenWindow &operator=(const CRiScreenWindow &c)
	{
		if ( this == &c )
			return *this;

		set(c.left(), c.right(), c.bottom(), c.top());

		CRManInterfaceCall::operator=(c);
		return *this;
	}
}; // CRiScreenWindow


///////////////////////////////////////////////////////////////////////////////
/** @brief Give a rectangular cropped region of the image in screen coordinates.
 */
class CRiCropWindow : public CRManInterfaceCall {
private:
	RtFloat m_xmin,  //!< Crop window minimal x
	        m_xmax,  //!< Crop window maximal x
	        m_ymin,  //!< Crop window minimal y
	        m_ymax;  //!< Crop window maximal y

public:
	/** @brief Gets name for the class.
	 *
	 *  @return The name of the class (can be used as atomized string)
	 */
	inline static const char *myClassName(void) { return "CRiCropWindow"; }

	inline virtual const char *className() const { return CRiCropWindow::myClassName(); }

	inline virtual bool isA(const char *atomizedClassName) const
	{
		return ( atomizedClassName == myClassName() );
	}

	inline virtual bool isKindOf(const char *atomizedClassName) const
	{
		if ( atomizedClassName == myClassName() )
			return true;
		return CRManInterfaceCall::isKindOf(atomizedClassName);
	}

	/** @brief Default Constructor.
	 *
	 *  @param aLineNo The line number to store, if aLineNo is initialized to -1 (a line number is not known)
	 *  @param aXmin The left edge of the crop window (screen coordinates)
	 *  @param aXmax The right edge of the crop window (screen coordinates)
	 *  @param aYmin The bottom edge of the crop window (screen coordinates)
	 *  @param aYmax The top edge of the crop window (screen coordinates)
	 */
	inline CRiCropWindow(
		long aLineNo = -1,
		RtFloat aXmin = COptions::defCropWindowLeft,
		RtFloat aXmax = COptions::defCropWindowRight,
		RtFloat aYmin = COptions::defScreenWindowBottom,
		RtFloat aYmax = COptions::defScreenWindowTop )
		: CRManInterfaceCall(aLineNo), m_xmin(aXmin), m_xmax(aXmax), m_ymin(aYmin), m_ymax(aYmax)
	{
	}

	/** @brief Copy constructor.
	 *
	 *  @param c Object to copy.
	 */
	inline CRiCropWindow(const CRiCropWindow &c)
	{
		*this = c;
	}

	/** @brief Destructor.
	 */
	inline virtual ~CRiCropWindow()
	{
	}

	inline virtual CRManInterfaceCall *duplicate() const
	{
		return new CRiCropWindow(*this);
	}

	inline virtual EnumRequests interfaceIdx() const { return REQ_CROP_WINDOW; }


	/** @brief Gets the left edge.
	 *
	 *  @return The left edge of the crop window (screen coordinates).
	 */
	inline RtFloat xmin() const
	{
		return m_xmin;
	}

	/** @brief Sets the left edge.
	 *
	 *  @param aXmin The left edge of the crop window (screen coordinates).
	 */
	inline void xmin(RtFloat aXmin)
	{
		m_xmin = aXmin;
	}

	/** @brief Gets the left edge.
	 *
	 *  @return The right edge of the crop window (screen coordinates).
	 */
	inline RtFloat xmax() const
	{
		return m_xmax;
	}

	/** @brief Sets the right edge.
	 *
	 *  @param aXmax The right edge of the crop window (screen coordinates).
	 */
	inline void xmax(RtFloat aXmax)
	{
		m_xmax = aXmax;
	}

	/** @brief Gets the bottom edge.
	 *
	 *  @return The bottom edge of the crop window (screen coordinates).
	 */
	inline RtFloat ymin() const
	{
		return m_ymin;
	}

	/** @brief Sets The bottom edge.
	 *
	 *  @param aYmin The bottom edge of the crop window (screen coordinates).
	 */
	inline void ymin(RtFloat aYmin)
	{
		m_ymin = aYmin;
	}

	/** @brief Gets the top edge.
	 *
	 *  @return The top edge of the crop window (screen coordinates).
	 */
	inline RtFloat ymax() const
	{
		return m_ymax;
	}

	/** @brief Sets The top edge.
	 *
	 *  @param aYmax The top edge of the crop window (screen coordinates).
	 */
	inline void ymax(RtFloat aYmax)
	{
		m_ymax = aYmax;
	}

	/** @brief Gets the values of a crop window.
	 *
	 *  @retval aXmin The left edge of the crop window (screen coordinates)
	 *  @retval aXmax The right edge of the crop window (screen coordinates)
	 *  @retval aYmin The bottom edge of the crop window (screen coordinates)
	 *  @retval aYmax The top edge of the crop window (screen coordinates)
	 */
	inline void set(
		RtFloat &aXmin,
		RtFloat &aXmax,
		RtFloat &aYmin,
		RtFloat &aYmax) const
	{
		aXmin = m_xmin;
		aXmax = m_xmax;
		aYmin = m_ymin;
		aYmax = m_ymax;
	}

	/** @brief Sets the values of a crop window.
	 *
	 *  @param aXmin The left edge of the crop window (screen coordinates)
	 *  @param aXmax The right edge of the crop window (screen coordinates)
	 *  @param aYmin The bottom edge of the crop window (screen coordinates)
	 *  @param aYmax The top edge of the crop window (screen coordinates)
	 */
	inline void set(
		RtFloat aXmin,
		RtFloat aXmax,
		RtFloat aYmin,
		RtFloat aYmax)
	{
		m_xmin = aXmin;
		m_xmax = aXmax;
		m_ymin = aYmin;
		m_ymax = aYmax;
	}

	inline virtual void preProcess(IDoRender &ri, const IArchiveCallback *cb)
	{
		ri.preCropWindow(m_xmin, m_xmax, m_ymin, m_ymax);
	}

	inline virtual void doProcess(IDoRender &ri, const IArchiveCallback *cb)
	{
		ri.doCropWindow(m_xmin, m_xmax, m_ymin, m_ymax);
	}

	inline virtual void postProcess(IDoRender &ri, const IArchiveCallback *cb)
	{
		ri.postCropWindow(m_xmin, m_xmax, m_ymin, m_ymax);
	}

	inline CRiCropWindow &operator=(const CRiCropWindow &c)
	{
		if ( this == &c )
			return *this;

		set(c.xmin(), c.xmax(), c.ymin(), c.ymax());

		CRManInterfaceCall::operator=(c);
		return *this;
	}
}; // CRiCropWindow


///////////////////////////////////////////////////////////////////////////////
/** @brief Sets the projection type of the camera.
 */
class CRiProjection : public CVarParamRManInterfaceCall {
private:
	RtToken m_name; //!< Atomized projection name (RI_PERSPECTIVE, RI_ORTHOGRAPHIC)

public:
	/** @brief Gets name for the class.
	 *
	 *  @return The name of the class (can be used as atomized string)
	 */
	inline static const char *myClassName(void) { return "CRiProjection"; }

	inline virtual const char *className() const { return CRiProjection::myClassName(); }

	inline virtual bool isA(const char *atomizedClassName) const
	{
		return ( atomizedClassName == myClassName() );
	}

	inline virtual bool isKindOf(const char *atomizedClassName) const
	{
		if ( atomizedClassName == myClassName() )
			return true;
		return CVarParamRManInterfaceCall::isKindOf(atomizedClassName);
	}

	/** @brief Default Constructor.
	 *
	 *  @param aLineNo The line number to store, if aLineNo is initialized to -1 (a line number is not known)
	 *  @param name Atomized name of the projection
	 */
	inline CRiProjection(
		long aLineNo = -1,
		RtToken aName = COptions::defProjection)
		: CVarParamRManInterfaceCall(aLineNo)
	{
		m_name = aName;
	}

	/** @brief Constructor.
	 *
	 *  @param aLineNo The line number to store.
	 *  @param dict Dictonary with the current declarations.
	 *  @param p Counters (vertices, corners etc.) of the request.
	 *  @param curColorDescr Current color descriptor.
	 *  @param aName Atomized name of the projection
	 *  @param n Number of parameters (size of @a tokens, @a params).
	 *  @param tokens Tokens of the request.
	 *  @param params Parameter values of the request.
	 */
	inline CRiProjection(
		long aLineNo, CDeclarationDictionary &decl, const CColorDescr &curColorDescr,
		RtToken aName,
		RtInt n, RtToken tokens[], RtPointer params[])
		: CVarParamRManInterfaceCall(aLineNo, decl, CParameterClasses(), curColorDescr, n, tokens, params)
	{
		m_name = aName;
	}

	/** @brief Constructor.
	 *
	 * @param aLineNo The line number to store, if aLineNo is initialized to -1 (a line number is not known)
	 * @param aName Atomized name of the projection
	 * @param parameters Parsed parameter list.
	 */
	inline CRiProjection(
		long aLineNo,
		RtToken aName,
		const CParameterList &theParameters
		)
		: CVarParamRManInterfaceCall(aLineNo, theParameters)
	{
		m_name = aName;
	}


	/** @brief Copy constructor.
	 *
	 *  @param c Object to copy.
	 */
	inline CRiProjection(const CRiProjection &c)
	{
		*this = c;
	}

	/** @brief Destructor.
	 */
	inline virtual ~CRiProjection()
	{
	}

	inline virtual CRManInterfaceCall *duplicate() const
	{
		return new CRiProjection(*this);
	}

	inline virtual EnumRequests interfaceIdx() const { return REQ_PROJECTION; }

	/** @brief Gets the name of the projection.
	 *
	 *  @return The atomized name of the projection.
	 */
	inline RtToken name() const
	{
		return m_name;
	}

	/** @brief Sets the name of the projection.
	 *
	 *  @param An atomized name of a projection.
	 */
	inline void name(RtToken aName)
	{
		m_name = aName;
	}

	inline virtual void preProcess(IDoRender &ri, const IArchiveCallback *cb)
	{
		ri.preProjection(name(), parameters());
	}

	inline virtual void doProcess(IDoRender &ri, const IArchiveCallback *cb)
	{
		ri.doProjection(name(), parameters());
	}

	inline virtual void postProcess(IDoRender &ri, const IArchiveCallback *cb)
	{
		ri.postProjection(name(), parameters());
	}

	/** @brief Assignment.
	 *
	 *  @param c CRManInterfaceCall to assign
	 *  @return A reference to this object.
	 */
	inline CRiProjection &operator=(const CRiProjection &c)
	{
		if ( this == &c )
			return *this;

		name(c.name());

		CVarParamRManInterfaceCall::operator=(c);
		return *this;
	}
}; // CRiProjection


///////////////////////////////////////////////////////////////////////////////
/** @brief The near/far camera clipping planes.
 */
class CRiClipping : public CRManInterfaceCall {
private:
	RtFloat m_hither, //!< Near clipping plane
	        m_yon;    //!< Far clipping plane

public:
	/** @brief Gets name for the class.
	 *
	 *  @return The name of the class (can be used as atomized string)
	 */
	inline static const char *myClassName(void) { return "CRiClipping"; }

	inline virtual const char *className() const { return CRiClipping::myClassName(); }

	inline virtual bool isA(const char *atomizedClassName) const
	{
		return ( atomizedClassName == myClassName() );
	}

	inline virtual bool isKindOf(const char *atomizedClassName) const
	{
		if ( atomizedClassName == myClassName() )
			return true;
		return CRManInterfaceCall::isKindOf(atomizedClassName);
	}

	/** @brief Default Constructor.
	 *
	 *  @param aLineNo The line number to store, if aLineNo is initialized to -1 (a line number is not known)
	 *  @param hither Near clipping plane
	 *  @param yon Far clipping plane
	 */
	inline CRiClipping(
		long aLineNo=-1,
		RtFloat aHither=COptions::defNearClip,
		RtFloat aYon=COptions::defFarClip)
		: CRManInterfaceCall(aLineNo), m_hither(aHither), m_yon(aYon)
	{
	}

	/** @brief Copy constructor.
	 *
	 *  @param c Object to copy.
	 */
	inline CRiClipping(const CRiClipping &c)
	{
		*this = c;
	}

	/** @brief Destructor.
	 */
	inline virtual ~CRiClipping()
	{
	}

	inline virtual CRManInterfaceCall *duplicate() const
	{
		return new CRiClipping(*this);
	}

	inline virtual EnumRequests interfaceIdx() const { return REQ_CLIPPING; }

	/** @brief Gets the near clipping plane for the camera.
	 *
	 *  @return The near clipping plane for the camera.
	 */
	inline RtFloat hither() const
	{
		return m_hither;
	}

	/** @brief Sets the near clipping plane for the camera.
	 *
	 *  @param aHither Near clipping plane for the camera.
	 */
	inline void hither(RtFloat aHither)
	{
		m_hither = aHither;
	}

	/** @brief Gets the far clipping plane for the camera.
	 *
	 *  @return The far clipping plane for the camera.
	 */
	inline RtFloat yon() const
	{
		return m_yon;
	}

	/** @brief Sets the far clipping plane for the camera.
	 *
	 *  @param aYon Far clipping plane for the camera.
	 */
	inline void yon(RtFloat aYon)
	{
		m_yon = aYon;
	}

	/** @brief Gets the values of the camera clipping planes.
	 *
	 *  @retval hither Near clipping plane
	 *  @retval yon Far clipping plane
	 */
	inline void get(RtFloat &aHither, RtFloat &aYon) const
	{
		aHither = m_hither;
		aYon = m_yon;
	}

	/** @brief Sets the values of the camera clipping planes.
	 *
	 *  @param hither Near clipping plane
	 *  @param yon Far clipping plane
	 */
	inline void set(RtFloat aHither, RtFloat aYon)
	{
		m_hither = aHither;
		m_yon = aYon;
	}

	inline virtual void preProcess(IDoRender &ri, const IArchiveCallback *cb)
	{
		ri.preClipping(m_hither, m_yon);
	}

	inline virtual void doProcess(IDoRender &ri, const IArchiveCallback *cb)
	{
		ri.doClipping(m_hither, m_yon);
	}

	inline virtual void postProcess(IDoRender &ri, const IArchiveCallback *cb)
	{
		ri.postClipping(m_hither, m_yon);
	}

	/** @brief Assignment.
	 *
	 *  @param c CRManInterfaceCall to assign
	 *  @return A reference to this object.
	 */
	inline CRiClipping &operator=(const CRiClipping &c)
	{
		if ( this == &c )
			return *this;

		set(c.hither(), c.yon());

		CRManInterfaceCall::operator=(c);
		return *this;
	}
}; // CRiClipping

///////////////////////////////////////////////////////////////////////////////
/** @brief Additional clipping plane.
 */
class CRiClippingPlane : public CRManInterfaceCall {
private:
	RtFloat m_x,  //!< Point of an additional clipping plane (x coordinate).
	        m_y,  //!< Point of an additional clipping plane (y coordinate).
	        m_z,  //!< Point of an additional clipping plane (z coordinate).
	        m_nx, //!< Normal of an additional clipping plane (x coordinate).
	        m_ny, //!< Normal of an additional clipping plane (y coordinate).
	        m_nz; //!< Normal of an additional clipping plane (z coordinate).

public:
	/** @brief Gets name for the class.
	 *
	 *  @return The name of the class (can be used as atomized string)
	 */
	inline static const char *myClassName(void) { return "CRiClippingPlane"; }

	inline virtual const char *className() const { return CRiClippingPlane::myClassName(); }

	inline virtual bool isA(const char *atomizedClassName) const
	{
		return ( atomizedClassName == myClassName() );
	}

	inline virtual bool isKindOf(const char *atomizedClassName) const
	{
		if ( atomizedClassName == myClassName() )
			return true;
		return CRManInterfaceCall::isKindOf(atomizedClassName);
	}

	/** @brief Default Constructor.
	 *
	 *  @param aLineNo The line number to store, if aLineNo is initialized to -1 (a line number is not known)
	 *  @param x x component of a point at the clipping plane.
	 *  @param y y component of a point at the clipping plane.
	 *  @param z z component of a point at the clipping plane.
	 *  @param nx x component of the normal vector of the clipping plane.
	 *  @param ny y component of the normal vector of the clipping plane.
	 *  @param nz z component of the normal vector of the clipping plane.
	 */
	inline CRiClippingPlane(
		long aLineNo = -1,
		RtFloat aX = 0,
		RtFloat aY = 0,
		RtFloat aZ = 0,
		RtFloat aNx = 0,
		RtFloat aNy = 0,
		RtFloat aNz = -1) :
	CRManInterfaceCall(aLineNo), m_x(aX), m_y(aY), m_z(aZ), m_nx(aNx), m_ny(aNy), m_nz(aNz)
	{
	}

	/** @brief Copy constructor.
	 *
	 *  @param c Object to copy.
	 */
	inline CRiClippingPlane(const CRiClippingPlane &c)
	{
		*this = c;
	}

	/** @brief Destructor.
	 */
	inline virtual ~CRiClippingPlane()
	{
	}

	inline virtual CRManInterfaceCall *duplicate() const
	{
		return new CRiClippingPlane(*this);
	}

	inline virtual EnumRequests interfaceIdx() const { return REQ_CLIPPING_PLANE; }

	/** @brief Gets the x component of a point at the clipping plane.
	 *
	 *  @return The x component of a point at the clipping plane.
	 */
	inline RtFloat x() const
	{
		return m_x;
	}

	/** @brief Sets the x component of a point at the clipping plane.
	 *
	 *  @param aX The x component of a point at the clipping plane.
	 */
	inline void x(RtFloat aX)
	{
		m_x = aX;
	}

	/** @brief Gets the y component of a point at the clipping plane.
	 *
	 *  @return The y component of a point at the clipping plane.
	 */
	inline RtFloat y() const
	{
		return m_y;
	}

	/** @brief Sets the y component of a point at the clipping plane.
	 *
	 *  @param aY The y component of a point at the clipping plane.
	 */
	inline void y(RtFloat aY)
	{
		m_y = aY;
	}

	/** @brief Gets the z component of a point at the clipping plane.
	 *
	 *  @return The z component of a point at the clipping plane.
	 */
	inline RtFloat z() const
	{
		return m_z;
	}

	/** @brief Sets the z component of a point at the clipping plane.
	 *
	 *  @param aZ The z component of a point at the clipping plane.
	 */
	inline void z(RtFloat aZ)
	{
		m_z = aZ;
	}

	/** @brief Gets the components of a point at the clipping plane.
	 *
	 *  @retval p The components of a point at the clipping plane.
	 */
	inline void getPoint(RtPoint &p) const
	{
		p[0] = m_x;
		p[1] = m_y;
		p[2] = m_z;
	}

	/** @brief Sets the components of a point at the clipping plane.
	 *
	 *  @param p The components of a point at the clipping plane.
	 */
	inline void setPoint(const RtPoint &p)
	{
		m_x = p[0];
		m_y = p[1];
		m_z = p[2];
	}

	/** @brief Gets the x component of the normal vector of the clipping plane.
	 *
	 *  @return The x component of the normal vector of the clipping plane.
	 */
	inline RtFloat nx() const
	{
		return m_nx;
	}

	/** @brief Sets the x component of the normal vector of the clipping plane.
	 *
	 *  @param aNx The x component of the normal vector the clipping plane.
	 */
	inline void nx(RtFloat aNx)
	{
		m_nx = aNx;
	}

	/** @brief Gets the y component of the normal vector of the clipping plane.
	 *
	 *  @return The y component of the normal vector of the clipping plane.
	 */
	inline RtFloat ny() const
	{
		return m_ny;
	}

	/** @brief Sets the y component of the normal vector of the clipping plane.
	 *
	 *  @param aNy The y component of the normal vector the clipping plane.
	 */
	inline void ny(RtFloat aNy)
	{
		m_ny = aNy;
	}

	/** @brief Gets the z component of the normal vector of the clipping plane.
	 *
	 *  @return The z component of the normal vector of the clipping plane.
	 */
	inline RtFloat nz() const
	{
		return m_nz;
	}

	/** @brief Sets the z component of the normal vector of the clipping plane.
	 *
	 *  @param aNz The z component of the normal vector the clipping plane.
	 */
	inline void nz(RtFloat aNz)
	{
		m_nz = aNz;
	}

	/** @brief Gets the components of the normal of the clipping plane.
	 *
	 *  @retval p The components of the normal of the clipping plane.
	 */
	inline void getNormal(RtPoint &p) const
	{
		p[0] = m_nx;
		p[1] = m_ny;
		p[2] = m_nz;
	}

	/** @brief Sets the components of the normal of the clipping plane.
	 *
	 *  @param p The components of the normal of the clipping plane.
	 */
	inline void setNormal(const RtPoint &p)
	{
		m_nx = p[0];
		m_ny = p[1];
		m_nz = p[2];
	}

	/** @brief Gets the values of the clipping plane
	 *
	 *  @retval x x component of a point at the clipping plane.
	 *  @retval y y component of a point at the clipping plane.
	 *  @retval z z component of a point at the clipping plane.
	 *  @retval nx x component of the normal vector of the clipping plane.
	 *  @retval ny y component of the normal vector of the clipping plane.
	 *  @retval nz z component of the normal vector of the clipping plane.
	 */
	inline void get(
		RtFloat &aX,
		RtFloat &aY,
		RtFloat &aZ,
		RtFloat &aNx,
		RtFloat &aNy,
		RtFloat &aNz) const
	{
		aX = m_x;
		aY = m_y;
		aZ = m_z;
		aNx = m_nx;
		aNy = m_ny;
		aNz =m_nz;
	}

	/** @brief Sets the values of the clipping plane
	 *
	 *  @param x x component of a point at the clipping plane.
	 *  @param y y component of a point at the clipping plane.
	 *  @param z z component of a point at the clipping plane.
	 *  @param nx x component of the normal vector of the clipping plane.
	 *  @param ny y component of the normal vector of the clipping plane.
	 *  @param nz z component of the normal vector of the clipping plane.
	 */
	inline void set(
		RtFloat aX,
		RtFloat aY,
		RtFloat aZ,
		RtFloat aNx,
		RtFloat aNy,
		RtFloat aNz)
	{
		m_x = aX;
		m_y = aY;
		m_z = aZ;
		m_nx = aNx;
		m_ny = aNy;
		m_nz = aNz;
	}

	inline virtual void preProcess(IDoRender &ri, const IArchiveCallback *cb)
	{
		ri.preClippingPlane(m_x, m_y, m_z, m_nx, m_ny, m_nz);
	}

	inline virtual void doProcess(IDoRender &ri, const IArchiveCallback *cb)
	{
		ri.doClippingPlane(m_x, m_y, m_z, m_nx, m_ny, m_nz);
	}

	inline virtual void postProcess(IDoRender &ri, const IArchiveCallback *cb)
	{
		ri.postClippingPlane(m_x, m_y, m_z, m_nx, m_ny, m_nz);
	}

	/** @brief Assignment.
	 *
	 *  @param c CRManInterfaceCall to assign
	 *  @return A reference to this object.
	 */
	inline CRiClippingPlane &operator=(const CRiClippingPlane &c)
	{
		if ( this == &c )
			return *this;

		set(c.x(), c.y(), c.z(), c.nx(), c.ny(), c.nz());

		CRManInterfaceCall::operator=(c);
		return *this;
	}
}; // CRiClippingPlane


///////////////////////////////////////////////////////////////////////////////
/** @brief Depth of field of the camera projection.
 */
class CRiDepthOfField : public CRManInterfaceCall {
private:
	RtFloat
		m_fstop,         //!< Aperture number to calculate the lens diameter.
		m_focallength,   //!< Focal length of the camera.
		m_focaldistance; //!< Distance along the cameras z axis in which objects will be in focus.
public:
	/** @brief Gets name for the class.
	 *
	 *  @return The name of the class (can be used as atomized string)
	 */
	inline static const char *myClassName(void) { return "CRiDepthOfField"; }

	inline virtual const char *className() const { return CRiDepthOfField::myClassName(); }

	inline virtual bool isA(const char *atomizedClassName) const
	{
		return ( atomizedClassName == myClassName() );
	}

	inline virtual bool isKindOf(const char *atomizedClassName) const
	{
		if ( atomizedClassName == myClassName() )
			return true;
		return CRManInterfaceCall::isKindOf(atomizedClassName);
	}

	/** @brief Default constructor
	 *
	 *  @param aLineNo The line number to store, if aLineNo is initialized to -1 (a line number is not known)
	 *  @param aFstop Aperture number to calculate the lens diameter.
	 *  @param aFocallength Focal length of the camera.
	 *  @param aFocaldistance Distance along the cameras z axis in which objects will be in focus.
	 */
	inline CRiDepthOfField(
		long aLineNo = -1,
		RtFloat aFstop = COptions::defFstop,
		RtFloat aFocallength = COptions::defFocalLength,
		RtFloat aFocaldistance = COptions::defFocalDistance)
		: CRManInterfaceCall(aLineNo),
		  m_fstop(aFstop), m_focallength(aFocallength), m_focaldistance(aFocaldistance)
	{
	}

	/** @brief Copy constructor.
	 *
	 *  @param c Object to copy.
	 */
	inline CRiDepthOfField(const CRiDepthOfField &c)
	{
		*this = c;
	}

	/** @brief Destructor.
	 */
	inline virtual ~CRiDepthOfField()
	{
	}

	inline virtual CRManInterfaceCall *duplicate() const
	{
		return new CRiDepthOfField(*this);
	}

	inline virtual EnumRequests interfaceIdx() const { return REQ_DEPTH_OF_FIELD; }

	/** @brief Gets the aperture number.
	 *
	 *  @return The aperture number.
	 */
	inline RtFloat fstop() const
	{
		return m_fstop;
	}

	/** @brief Sets the aperture number.
	 *
	 *  @param aFstop The aperture number.
	 */
	inline void fstop(RtFloat aFstop)
	{
		m_fstop = aFstop;
	}

	/** @brief Gets the focal length.
	 *
	 *  @return The focal length.
	 */
	inline RtFloat focallength() const
	{
		return m_focallength;
	}

	/** @brief Sets the focal length.
	 *
	 *  @param  aFocallength The focal length.
	 */
	inline void focallength(RtFloat aFocallength)
	{
		m_focallength = aFocallength;
	}

	/** @brief Gets the focal distance.
	 *
	 *  @return The focal distance.
	 */
	inline RtFloat focaldistance() const
	{
		return m_focaldistance;
	}

	/** @brief Sets the focal distance.
	 *
	 *  @param  aFocallength The focal distance.
	 */
	inline void focaldistance(RtFloat aFocaldistance)
	{
		m_focaldistance = aFocaldistance;
	}

	/** @brief Gets the values of the depth of field option.
	 *
	 *  @retval aFstop Aperture number to calculate the lens diameter.
	 *  @retval aFocallength Focal length of the camera.
	 *  @retval aFocaldistance Distance along the cameras z axis in which objects will be in focus.
	 */
	inline void get(
		RtFloat &aFstop,
		RtFloat &aFocallength,
		RtFloat &aFocaldistance) const
	{
		aFstop = m_fstop;
		aFocallength = m_focallength;
		aFocaldistance = m_focaldistance;
	}

	/** @brief Sets the values for the depth of field option.
	 *
	 *  @param aFstop Aperture number to calculate the lens diameter.
	 *  @param aFocallength Focal length of the camera.
	 *  @param aFocaldistance Distance along the cameras z axis in which objects will be in focus.
	 */
	inline void set(
		RtFloat aFstop,
		RtFloat aFocallength,
		RtFloat aFocaldistance)
	{
		m_fstop = aFstop;
		m_focallength = aFocallength;
		m_focaldistance = aFocaldistance;
	}

	inline virtual void preProcess(IDoRender &ri, const IArchiveCallback *cb)
	{
		ri.preDepthOfField(m_fstop, m_focallength, m_focaldistance);
	}

	inline virtual void doProcess(IDoRender &ri, const IArchiveCallback *cb)
	{
		ri.doDepthOfField(m_fstop, m_focallength, m_focaldistance);
	}

	inline virtual void postProcess(IDoRender &ri, const IArchiveCallback *cb)
	{
		ri.postDepthOfField(m_fstop, m_focallength, m_focaldistance);
	}

	/** @brief Assignment.
	 *
	 *  @param c CRManInterfaceCall to assign
	 *  @return A reference to this object.
	 */
	inline CRiDepthOfField &operator=(const CRiDepthOfField &c)
	{
		if ( this == &c )
			return *this;

		set(c.fstop(), c.focallength(), c.focaldistance());

		CRManInterfaceCall::operator=(c);
		return *this;
	}
}; // CRiDepthOfField


///////////////////////////////////////////////////////////////////////////////
/** @brief The camera shuter open/close times
 */
class CRiShutter : public CRManInterfaceCall {
private:
	RtFloat
		m_smin, //!< Time when shutter opens.
		m_smax; //!< Time when shutter closes.
public:
	/** @brief Gets name for the class.
	 *
	 *  @return The name of the class (can be used as atomized string)
	 */
	inline static const char *myClassName(void) { return "CRiShutter"; }

	inline virtual const char *className() const { return CRiShutter::myClassName(); }

	inline virtual bool isA(const char *atomizedClassName) const
	{
		return ( atomizedClassName == myClassName() );
	}

	inline virtual bool isKindOf(const char *atomizedClassName) const
	{
		if ( atomizedClassName == myClassName() )
			return true;
		return CRManInterfaceCall::isKindOf(atomizedClassName);
	}

	/** @brief Default constructor
	 *
	 *  @param aLineNo The line number to store, if aLineNo is initialized to -1 (a line number is not known)
	 *  @param smin Time when shutter opens.
	 *  @param smax Time when shutter closes.
	 */
	inline CRiShutter(
		long aLineNo = -1,
		RtFloat smin = COptions::defShutterOpen,
		RtFloat smax = COptions::defShutterClose)
		: CRManInterfaceCall(aLineNo),
		  m_smin(smin), m_smax(smax)
	{
	}

	/** @brief Copy constructor.
	 *
	 *  @param c Object to copy.
	 */
	inline CRiShutter(const CRiShutter &c)
	{
		*this = c;
	}

	/** @brief Destructor.
	 */
	inline virtual ~CRiShutter()
	{
	}

	inline virtual CRManInterfaceCall *duplicate() const
	{
		return new CRiShutter(*this);
	}

	inline virtual EnumRequests interfaceIdx() const { return REQ_SHUTTER; }

	/** @brief Gets the time at which the shutter opens.
	 *
	 *  @return The time at which the shutter opens.
	 */
	inline RtFloat shutterOpen() const
	{
		return m_smin;
	}

	/** @brief Sets the time at which the shutter opens.
	 *
	 *  @param smin The time at which the shutter opens.
	 */
	inline void shutterOpen(RtFloat smin)
	{
		m_smin = smin;
	}

	/** @brief Gets the time at which the shutter closes.
	 *
	 *  @return The time at which the shutter closes.
	 */
	inline RtFloat shutterClose() const
	{
		return m_smax;
	}

	/** @brief Sets the time at which the shutter closes.
	 *
	 *  @param smax The time at which the shutter closes.
	 */
	inline void shutterClose(RtFloat smax)
	{
		m_smax = smax;
	}

	/** @brief Gets the shutter times.
	 *
	 *  @retval smin Time when shutter opens.
	 *  @retval smax Time when shutter closes.
	 */
	inline void get(
		RtFloat &smin,
		RtFloat &smax) const
	{
		smin = m_smin;
		smax = m_smax;
	}

	/** @brief Sets the shutter times.
	 *
	 *  @param smin Time when shutter opens.
	 *  @param smax Time when shutter closes.
	 */
	inline void set(
		RtFloat smin,
		RtFloat smax)
	{
		m_smin = smin;
		m_smax = smax;
	}

	inline virtual void preProcess(IDoRender &ri, const IArchiveCallback *cb)
	{
		ri.preShutter(m_smin, m_smax);
	}

	inline virtual void doProcess(IDoRender &ri, const IArchiveCallback *cb)
	{
		ri.doShutter(m_smin, m_smax);
	}

	inline virtual void postProcess(IDoRender &ri, const IArchiveCallback *cb)
	{
		ri.postShutter(m_smin, m_smax);
	}

	/** @brief Assignment.
	 *
	 *  @param c CRManInterfaceCall to assign
	 *  @return A reference to this object.
	 */
	inline CRiShutter &operator=(const CRiShutter &c)
	{
		if ( this == &c )
			return *this;

		set(c.shutterOpen(), c.shutterClose());

		CRManInterfaceCall::operator=(c);
		return *this;
	}
}; // CRiShutter


///////////////////////////////////////////////////////////////////////////////
/** @brief Estimated variance of the computed pixel value from the true pixel value.
 */
class CRiPixelVariance : public CRManInterfaceCall {
private:
	RtFloat m_variation; //!< Estimated variance of the computed pixel value from the true pixel value.
public:
	/** @brief Gets name for the class.
	 *
	 *  @return The name of the class (can be used as atomized string)
	 */
	inline static const char *myClassName(void) { return "CRiPixelVariance"; }

	inline virtual const char *className() const { return CRiPixelVariance::myClassName(); }

	inline virtual bool isA(const char *atomizedClassName) const
	{
		return ( atomizedClassName == myClassName() );
	}

	inline virtual bool isKindOf(const char *atomizedClassName) const
	{
		if ( atomizedClassName == myClassName() )
			return true;
		return CRManInterfaceCall::isKindOf(atomizedClassName);
	}

	/** @brief Default constructor
	 *
	 *  @param aLineNo The line number to store, if aLineNo is initialized to -1 (a line number is not known)
	 *  @param aVariation Estimated variance of the computed pixel value from the true pixel value.
	 */
	inline CRiPixelVariance(
		long aLineNo = -1,
		RtFloat aVariation = COptions::defPixelVariance)
		: CRManInterfaceCall(aLineNo),
		  m_variation(aVariation)
	{
	}

	/** @brief Copy constructor.
	 *
	 *  @param c Object to copy.
	 */
	inline CRiPixelVariance(const CRiPixelVariance &c)
	{
		*this = c;
	}

	/** @brief Destructor.
	 */
	inline virtual ~CRiPixelVariance()
	{
	}

	inline virtual CRManInterfaceCall *duplicate() const
	{
		return new CRiPixelVariance(*this);
	}

	inline virtual EnumRequests interfaceIdx() const { return REQ_PIXEL_VARIANCE; }

	/** @brief Gets the pixel color variation.
	 *
	 *  @return Estimated variance of the computed pixel value from the true pixel value.
	 */
	inline RtFloat variation() const
	{
		return m_variation;
	}

	/** @brief Sets the pixel color variation.
	 *
	 *  @param aVariation Estimated variance of the computed pixel value from the true pixel value.
	 */
	inline void variation(RtFloat aVariation)
	{
		m_variation = aVariation;
	}

	inline virtual void preProcess(IDoRender &ri, const IArchiveCallback *cb)
	{
		ri.prePixelVariance(m_variation);
	}

	inline virtual void doProcess(IDoRender &ri, const IArchiveCallback *cb)
	{
		ri.doPixelVariance(m_variation);
	}

	inline virtual void postProcess(IDoRender &ri, const IArchiveCallback *cb)
	{
		ri.postPixelVariance(m_variation);
	}

	/** @brief Assignment.
	 *
	 *  @param c CRManInterfaceCall to assign
	 *  @return A reference to this object.
	 */
	inline CRiPixelVariance &operator=(const CRiPixelVariance &c)
	{
		if ( this == &c )
			return *this;

		variation(c.variation());

		CRManInterfaceCall::operator=(c);
		return *this;
	}
}; // CRiPixelVariance


///////////////////////////////////////////////////////////////////////////////
/** @brief Samples per pixel
 */
class CRiPixelSamples : public CRManInterfaceCall {
private:
	RtFloat
		m_xsamples, //!< Samples per pixel in horizontal direction.
		m_ysamples; //!< Samples per pixel in vertical direction.
public:
	/** @brief Gets name for the class.
	 *
	 *  @return The name of the class (can be used as atomized string)
	 */
	inline static const char *myClassName(void) { return "CRiPixelSamples"; }

	inline virtual const char *className() const { return CRiPixelSamples::myClassName(); }

	inline virtual bool isA(const char *atomizedClassName) const
	{
		return ( atomizedClassName == myClassName() );
	}

	inline virtual bool isKindOf(const char *atomizedClassName) const
	{
		if ( atomizedClassName == myClassName() )
			return true;
		return CRManInterfaceCall::isKindOf(atomizedClassName);
	}

	/** @brief Default constructor
	 *
	 *  @param aLineNo The line number to store, if aLineNo is initialized to -1 (a line number is not known)
	 *  @param aXsamples Samples per pixel in horizontal direction.
	 *  @param aYsamples Samples per pixel in vertical direction.
	 */
	inline CRiPixelSamples(
		long aLineNo = -1,
		RtFloat aXsamples = COptions::defXSamples,
		RtFloat aYsamples = COptions::defYSamples)
		: CRManInterfaceCall(aLineNo),
		  m_xsamples(aXsamples), m_ysamples(aYsamples)
	{
	}

	/** @brief Copy constructor.
	 *
	 *  @param c Object to copy.
	 */
	inline CRiPixelSamples(const CRiPixelSamples &c)
	{
		*this = c;
	}

	/** @brief Destructor.
	 */
	inline virtual ~CRiPixelSamples()
	{
	}

	inline virtual CRManInterfaceCall *duplicate() const
	{
		return new CRiPixelSamples(*this);
	}

	inline virtual EnumRequests interfaceIdx() const { return REQ_PIXEL_SAMPLES; }

	/** @brief Gets the samples per pixel in horizontal direction.
	 *
	 *  @return The samples per pixel in horizontal direction.
	 */
	inline RtFloat xsamples() const
	{
		return m_xsamples;
	}

	/** @brief Sets the samples per pixel in horizontal direction.
	 *
	 *  @param aXsamples The samples per pixel in horizontal direction.
	 */
	inline void xsamples(RtFloat aXsamples)
	{
		m_xsamples = aXsamples;
	}

	/** @brief Sets the samples per pixel in vertical direction.
	 *
	 *  @return The samples per pixel in verical direction.
	 */
	inline RtFloat ysamples() const
	{
		return m_ysamples;
	}

	/** @brief Gets the samples per pixel in vertical direction.
	 *
	 *  @param aYsamples The samples per pixel in verical direction.
	 */
	inline void ysamples(RtFloat aYsamples)
	{
		m_ysamples = aYsamples;
	}

	/** @brief Gets the samples per pixel
	 *
	 *  @retval aXsamples Samples per pixel in horizontal direction.
	 *  @retval aYsamples Samples per pixel in vertical direction.
	 */
	inline void set(
		RtFloat &aXsamples,
		RtFloat &aYsamples) const
	{
		aXsamples = m_xsamples;
		aYsamples = m_ysamples;
	}

	/** @brief Sets the samples per pixel
	 *
	 *  @param aXsamples Samples per pixel in horizontal direction.
	 *  @param aYsamples Samples per pixel in vertical direction.
	 */
	inline void set(
		RtFloat aXsamples,
		RtFloat aYsamples)
	{
		m_xsamples = aXsamples;
		m_ysamples = aYsamples;
	}

	inline virtual void preProcess(IDoRender &ri, const IArchiveCallback *cb)
	{
		ri.prePixelSamples(m_xsamples, m_ysamples);
	}

	inline virtual void doProcess(IDoRender &ri, const IArchiveCallback *cb)
	{
		ri.doPixelSamples(m_xsamples, m_ysamples);
	}

	inline virtual void postProcess(IDoRender &ri, const IArchiveCallback *cb)
	{
		ri.postPixelSamples(m_xsamples, m_ysamples);
	}

	/** @brief Assignment.
	 *
	 *  @param c CRManInterfaceCall to assign
	 *  @return A reference to this object.
	 */
	inline CRiPixelSamples &operator=(const CRiPixelSamples &c)
	{
		if ( this == &c )
			return *this;

		set(c.xsamples(), c.ysamples());

		CRManInterfaceCall::operator=(c);
		return *this;
	}
}; // CRiPixelSamples

///////////////////////////////////////////////////////////////////////////////
/** @brief Pixel anti-aliasing filter.
 */
class CRiPixelFilter : public CRManInterfaceCall {
private:
	IFilterFunc *m_function; //!< Filter function.
	RtFloat m_xwidth,        //!< Horizontal influence in pixels.
	        m_ywidth;        //!< Vertical influence in pixels.
public:
	/** @brief Gets name for the class.
	 *
	 *  @return The name of the class (can be used as atomized string)
	 */
	inline static const char *myClassName(void) { return "CRiPixelFilter"; }

	inline virtual const char *className() const { return CRiPixelFilter::myClassName(); }

	inline virtual bool isA(const char *atomizedClassName) const
	{
		return ( atomizedClassName == myClassName() );
	}

	inline virtual bool isKindOf(const char *atomizedClassName) const
	{
		if ( atomizedClassName == myClassName() )
			return true;
		return CRManInterfaceCall::isKindOf(atomizedClassName);
	}

	/** @brief Default constructor
	 *
	 *  The default is a gaussian filter with the influence of defXFilterWidth x defYFilterWidth pixels.
	 *
	 *  @param aLineNo The line number to store, if aLineNo is initialized to -1 (a line number is not known)
	 */
	inline CRiPixelFilter(
		long aLineNo = -1)
		: CRManInterfaceCall(aLineNo)
	{
		m_function = new CGaussianFilter();
		if ( !m_function ) {
			throw ExceptRiCPPError(RIE_NOMEM, RIE_SEVERE, (RtString)"Constructor of CRiPixelFilter, m_function=CGaussianFilter", __LINE__, __FILE__);
		}
		m_xwidth = COptions::defXFilterWidth;
		m_ywidth = COptions::defYFilterWidth;
	}

	/** @brief Default constructor
	 *
	 *  @param aLineNo The line number to store, if aLineNo is initialized to -1 (a line number is not known)
	 *  @param aFunction Filter function, is duplicated.
	 *  @param aXwidth Horizontal influence in pixels.
	 *  @param aYwidth Vertical influence in pixels.
	 */
	inline CRiPixelFilter(
		long aLineNo,
		const IFilterFunc &aFunction,
		RtFloat aXwidth, RtFloat aYwidth)
		: CRManInterfaceCall(aLineNo),
		  m_xwidth(aXwidth), m_ywidth(aYwidth)
	{
		m_function = aFunction.duplicate();
		if ( !m_function ) {
			throw ExceptRiCPPError(RIE_NOMEM, RIE_SEVERE, (RtString)"Constructor of CRiPixelFilter, m_function", __LINE__, __FILE__);
		}
	}

	/** @brief Copy constructor.
	 *
	 *  @param c Object to copy.
	 */
	inline CRiPixelFilter(const CRiPixelFilter &c)
	{
		*this = c;
	}

	/** @brief Destructor, frees resources.
	 */
	inline virtual ~CRiPixelFilter()
	{
		if ( m_function )
			delete m_function;
	}

	inline virtual CRManInterfaceCall *duplicate() const
	{
		return new CRiPixelFilter(*this);
	}

	inline virtual EnumRequests interfaceIdx() const { return REQ_PIXEL_FILTER; }

	/** @brief Gets a reference of the filter function.
	 *
	 *  @return Reference of the filter function.
	 */
	inline const IFilterFunc &function() const
	{
		if ( !m_function ) {
			throw ExceptRiCPPError(RIE_NOMEM, RIE_SEVERE, (RtString)"filterfunction is undefined", __LINE__, __FILE__);
		}
		return *m_function;
	}

	/** @brief Sets a new filter function (@a aFunction is duplicated).
	 *
	 *  @param aFunction Reference of a filter function.
	 */
	inline void function(const IFilterFunc &aFunction)
	{
		m_function = aFunction.duplicate();
		if ( !m_function ) {
			throw ExceptRiCPPError(RIE_NOMEM, RIE_SEVERE, (RtString)"pixel filter function could not be set", __LINE__, __FILE__);
		}
	}

	/** @brief Gets the horizontal influence in pixels.
	 *
	 *  @return The horizontal influence in pixels.
	 */
	inline RtFloat xwidth() const
	{
		return m_xwidth;
	}

	/** @brief Sets the horizontal influence in pixels.
	 *
	 *  @param aXWidth The horizontal influence in pixels.
	 */
	inline void xwidth(RtFloat aXWidth)
	{
		m_xwidth = aXWidth;
	}

	/** @brief Gets the vertical influence in pixels.
	 *
	 *  @return The vertical influence in pixels.
	 */
	inline RtFloat ywidth() const
	{
		return m_ywidth;
	}

	/** @brief Sets the vertical influence in pixels.
	 *
	 *  @param aYWidth The vertical influence in pixels.
	 */
	inline void ywidth(RtFloat aYWidth)
	{
		m_xwidth = aYWidth;
	}

	/** @brief Gets the pixel filter.
	 *
	 *  @param aFunction Pointer to a Filter function, gets a reference of the current function.
	 *  @param aXwidth Horizontal influence in pixels.
	 *  @param aYwidth Vertical influence in pixels.
	 */
	inline void get(
		const IFilterFunc **function,
		RtFloat &aXwidth, RtFloat &aYwidth) const
	{
		if ( function )
			*function = m_function;

		aXwidth = m_xwidth;
		aYwidth = m_ywidth;
	}

	/** @brief Sets the pixel filter.
	 *
	 *  @param aFunction Filter function, is duplicated.
	 *  @param aXwidth Horizontal influence in pixels.
	 *  @param aYwidth Vertical influence in pixels.
	 */
	inline void set(
		const IFilterFunc &function,
		RtFloat aXwidth, RtFloat aYwidth)
	{
		if ( m_function )
			delete m_function;

		m_function = function.duplicate();
		if ( !m_function ) {
			throw ExceptRiCPPError(RIE_NOMEM, RIE_SEVERE, (RtString)"Set pixelfilter, m_function", __LINE__, __FILE__);
		}
		m_xwidth = aXwidth;
		m_ywidth = aYwidth;
	}

	inline virtual void preProcess(IDoRender &ri, const IArchiveCallback *cb)
	{
		assert(m_function != 0);
		if ( m_function ) {
			ri.prePixelFilter(*m_function, m_xwidth, m_ywidth);
		}
	}

	inline virtual void doProcess(IDoRender &ri, const IArchiveCallback *cb)
	{
		assert(m_function != 0);
		if ( m_function ) {
			ri.doPixelFilter(*m_function, m_xwidth, m_ywidth);
		}
	}

	inline virtual void postProcess(IDoRender &ri, const IArchiveCallback *cb)
	{
		assert(m_function != 0);
		if ( m_function ) {
			ri.postPixelFilter(*m_function, m_xwidth, m_ywidth);
		}
	}

	/** @brief Assignment.
	 *
	 *  @param c CRManInterfaceCall to assign
	 *  @return A reference to this object.
	 */
	inline CRiPixelFilter &operator=(const CRiPixelFilter &c)
	{
		if ( this == &c )
			return *this;

		set(c.function(), c.xwidth(), c.ywidth());

		CRManInterfaceCall::operator=(c);
		return *this;
	}
}; // CRiPixelFilter

///////////////////////////////////////////////////////////////////////////////
/** @brief Gamma correction.
 */
class CRiExposure : public CRManInterfaceCall {
private:
	RtFloat m_gain,  //!< Gain of the brightness correction.
	        m_gamma; //!< Gamma value of the brightness correction.
public:
	/** @brief Gets name for the class.
	 *
	 *  @return The name of the class (can be used as atomized string)
	 */
	inline static const char *myClassName(void) { return "CRiExposure"; }

	inline virtual const char *className() const { return CRiExposure::myClassName(); }

	inline virtual bool isA(const char *atomizedClassName) const
	{
		return ( atomizedClassName == myClassName() );
	}

	inline virtual bool isKindOf(const char *atomizedClassName) const
	{
		if ( atomizedClassName == myClassName() )
			return true;
		return CRManInterfaceCall::isKindOf(atomizedClassName);
	}

	/** @brief Default constructor
	 *
	 *  @param aLineNo The line number to store, if aLineNo is initialized to -1 (a line number is not known)
	 *  @param aGain Gain value.
	 *  @param aGamma Gamma value.
	 */
	inline CRiExposure(
		long aLineNo = -1,
		RtFloat aGain = COptions::defGain,
		RtFloat aGamma = COptions::defGamma)
		: CRManInterfaceCall(aLineNo),
		   m_gain(aGain), m_gamma(aGamma)
	{
	}

	/** @brief Copy constructor.
	 *
	 *  @param c Object to copy.
	 */
	inline CRiExposure(const CRiExposure &c)
	{
		*this = c;
	}

	/** @brief Destructor.
	 */
	inline virtual ~CRiExposure()
	{
	}

	inline virtual CRManInterfaceCall *duplicate() const
	{
		return new CRiExposure(*this);
	}

	inline virtual EnumRequests interfaceIdx() const { return REQ_EXPOSURE; }

	/** @brief Gets the gain value for the brightness correction.
	 *
	 *  @return Gain value.
	 */
	inline RtFloat gain() const
	{
		return m_gain;
	}

	/** @brief Sets the gain value for the brightness correction.
	 *
	 *  @param aGain Gain value.
	 */
	inline void gain(RtFloat aGain)
	{
		m_gain = aGain;
	}

	/** @brief Gets the gamma value for the brightness correction.
	 *
	 *  @return Gamma value.
	 */
	inline RtFloat gamma() const
	{
		return m_gamma;
	}

	/** @brief Sets the gamma value for the brightness correction.
	 *
	 *  @param aGamma Gamma value.
	 */
	inline void gamma(RtFloat aGamma)
	{
		m_gamma = aGamma;
	}

	/** @brief Gets the values for the brightness correction.
	 *
	 *  @retval aGain Gain value.
	 *  @retval aGamma Gamma value.
	 */
	inline void get(
		RtFloat &aGain,
		RtFloat &aGamma)
	{
		m_gain = aGain;
		m_gamma = aGamma;
	}

	/** @brief Sets the values for the brightness correction.
	 *
	 *  @param aGain Gain value.
	 *  @param aGamma Gamma value.
	 */
	inline void set(
		RtFloat aGain,
		RtFloat aGamma)
	{
		m_gain = aGain;
		m_gamma = aGamma;
	}

	inline virtual void preProcess(IDoRender &ri, const IArchiveCallback *cb)
	{
		ri.preExposure(m_gain, m_gamma);
	}

	inline virtual void doProcess(IDoRender &ri, const IArchiveCallback *cb)
	{
		ri.doExposure(m_gain, m_gamma);
	}

	inline virtual void postProcess(IDoRender &ri, const IArchiveCallback *cb)
	{
		ri.postExposure(m_gain, m_gamma);
	}

	/** @brief Assignment.
	 *
	 *  @param c CRManInterfaceCall to assign
	 *  @return A reference to this object.
	 */
	inline CRiExposure &operator=(const CRiExposure &c)
	{
		if ( this == &c )
			return *this;

		set(c.gain(), c.gamma());

		CRManInterfaceCall::operator=(c);
		return *this;
	}
}; // CRiExposure

///////////////////////////////////////////////////////////////////////////////
/** @brief The imager shader.
 */
class CRiImager : public CVarParamRManInterfaceCall {
private:
	RtToken m_name; //!< Atomized name of the imager shader.
public:
	/** @brief Gets name for the class.
	 *
	 *  @return The name of the class (can be used as atomized string)
	 */
	inline static const char *myClassName(void) { return "CRiImager"; }

	inline virtual const char *className() const { return CRiImager::myClassName(); }

	inline virtual bool isA(const char *atomizedClassName) const
	{
		return ( atomizedClassName == myClassName() );
	}

	inline virtual bool isKindOf(const char *atomizedClassName) const
	{
		if ( atomizedClassName == myClassName() )
			return true;
		return CVarParamRManInterfaceCall::isKindOf(atomizedClassName);
	}

	/** @brief Default constructor
	 *
	 *  @param aLineNo The line number to store, if aLineNo is initialized to -1 (a line number is not known)
	 *  @param aName The name of the imager.
	 */
	inline CRiImager(
		long aLineNo = -1,
		RtToken aName = COptions::defImagerName)
		: CVarParamRManInterfaceCall(aLineNo)
	{
		m_name = aName;
	}

	/** @brief Constructor
	 *
	 *  @param aLineNo The line number to store, if aLineNo is initialized to -1 (a line number is not known)
	 *  @param dict Dictonary with the current declarations.
	 *  @param p Counters (vertices, corners etc.) of the request.
	 *  @param curColorDescr Current color descriptor.
	 *  @param aName Atomized name of the imager.
	 *  @param n Number of parameters (size of @a tokens, @a params).
	 *  @param tokens Tokens of the request.
	 *  @param params Parameter values of the request.
	 */
	inline CRiImager(long aLineNo, CDeclarationDictionary &decl, const CColorDescr &curColorDescr,
		RtToken aName,
		RtInt n, RtToken tokens[], RtPointer params[])
		: CVarParamRManInterfaceCall(aLineNo)
	{
		CParameterClasses p;
		setParams(decl, p, curColorDescr, n, tokens, params);
		m_name = aName;
	}

	/** @brief Constructor.
	 *
	 *  @param aLineNo The line number to store, if aLineNo is initialized to -1 (a line number is not known)
	 *  @param aName Atomized name of the imager.
	 *  @param parameters Parsed parameter list.
	 */
	inline CRiImager(
		long aLineNo,
		RtToken aName,
		const CParameterList &theParameters
		)
		: CVarParamRManInterfaceCall(aLineNo, theParameters)
	{
		m_name = aName;
	}

	/** @brief Copy constructor.
	 *
	 *  @param c Object to copy.
	 */
	inline CRiImager(const CRiImager &c)
	{
		*this = c;
	}

	/** @brief Destructor.
	 */
	inline virtual ~CRiImager()
	{
	}

	inline virtual CRManInterfaceCall *duplicate() const
	{
		return new CRiImager(*this);
	}

	inline virtual EnumRequests interfaceIdx() const { return REQ_IMAGER; }

	/** @brief Gets the name of the imager.
	 *
	 *  @return Atomized name of the imager.
	 */
	inline RtToken name() const
	{
		return m_name;
	}

	/** @brief Sets the name of the imager.
	 *
	 *  @param aName Atomized name of the imager.
	 */
	inline void name(RtToken aName)
	{
		m_name = aName;
	}

	inline virtual void preProcess(IDoRender &ri, const IArchiveCallback *cb)
	{
		ri.preImager(m_name, parameters());
	}

	inline virtual void doProcess(IDoRender &ri, const IArchiveCallback *cb)
	{
		ri.doImager(m_name, parameters());
	}

	inline virtual void postProcess(IDoRender &ri, const IArchiveCallback *cb)
	{
		ri.postImager(m_name, parameters());
	}

	/** @brief Assignment.
	 *
	 *  @param c CRManInterfaceCall to assign
	 *  @return A reference to this object.
	 */
	inline CRiImager &operator=(const CRiImager &c)
	{
		if ( this == &c )
			return *this;

		name(c.name());

		CVarParamRManInterfaceCall::operator=(c);
		return *this;
	}
}; // CRiImager

///////////////////////////////////////////////////////////////////////////////
/** @brief Quantize the color or depth values by dithering.
 */
class CRiQuantize : public CRManInterfaceCall {
private:
	RtToken m_type;     //!< Atomized type of quantization (RI_RGBA or RI_Z).
	RtInt m_one,        //!< The corresponding resulting integer value for RtFloat 1.0.
	      m_qmin,       //!< Lower clamp for the dither value (absoule).
	      m_qmax;       //!< Higher clamp for the dither value (absoule).
	RtFloat m_ditheramplitude; //!< Amplitude (+/-) for the dither offset.

public:
	/** @brief Gets name for the class.
	 *
	 *  @return The name of the class (can be used as atomized string)
	 */
	inline static const char *myClassName(void) { return "CRiQuantize"; }

	inline virtual const char *className() const { return CRiQuantize::myClassName(); }

	inline virtual bool isA(const char *atomizedClassName) const
	{
		return ( atomizedClassName == myClassName() );
	}

	inline virtual bool isKindOf(const char *atomizedClassName) const
	{
		if ( atomizedClassName == myClassName() )
			return true;
		return CRManInterfaceCall::isKindOf(atomizedClassName);
	}

	/** @brief Default constructor
	 *
	 *  @param aLineNo The line number to store, if aLineNo is initialized to -1 (a line number is not known)
	 *  @param aType Atomized type of quantization (RI_RGBA or RI_Z).
	 *  @param aOne The corresponding resulting integer value for RtFloat 1.0.
	 *  @param aQmin Lower clamp for the dither value (absoule).
	 *  @param aQmax Higher clamp for the dither value (absoule).
	 *  @param anAmpl Amplitude (+/-) for the dither offset.
	 */
	inline CRiQuantize(
		long aLineNo = -1,
		RtToken aType = RI_RGBA,
		RtInt aOne = COptions::defOneRGBA,
		RtInt aQmin = COptions::defMinRGBA,
		RtInt aQmax = COptions::defMaxRGBA,
		RtFloat aDitherAmplitude = COptions::defDitherAmplitudeRGBA)
		: CRManInterfaceCall(aLineNo),
		m_type(aType), m_one(aOne), m_qmin(aQmin), m_qmax(aQmax), m_ditheramplitude(aDitherAmplitude)
	{
	}

	/** @brief Copy constructor.
	 *
	 *  @param c Object to copy.
	 */
	inline CRiQuantize(const CRiQuantize &c)
	{
		*this = c;
	}

	/** @brief Destructor.
	 */
	inline virtual ~CRiQuantize()
	{
	}

	inline virtual CRManInterfaceCall *duplicate() const
	{
		return new CRiQuantize(*this);
	}

	inline virtual EnumRequests interfaceIdx() const { return REQ_QUANTIZE; }

	/** @brief Gets the quantization type.
	 *
	 * @return Quantization type.
	 */
	inline RtToken type() const
	{
		return m_type;
	}

	/** @brief Sets the quantization type.
	 *
	 * @param aType Atomized quantization type.
	 */
	inline void type(RtToken aType)
	{
		m_type = aType;
	}

	/** @brief Gets the corresponding resulting integer value for RtFloat 1.0.
	 *
	 *  @return The corresponding resulting integer value for RtFloat 1.0.
	 */
	inline RtInt one() const
	{
		return m_one;
	}

	/** @brief Sets the corresponding resulting integer value for RtFloat 1.0.
	 *
	 *  @param aOne The corresponding resulting integer value for RtFloat 1.0.
	 */
	inline void one(RtInt aOne)
	{
		m_one = aOne;
	}

	/** @brief Gets the lower clamp for the dither value (absoule).
	 *
	 *  @return Lower clamp for the dither value (absoule).
	 */
	inline RtInt qmin() const
	{
		return m_qmin;
	}

	/** @brief Sets the lower clamp for the dither value (absoule).
	 *
	 *  @param aQMin Lower clamp for the dither value (absoule).
	 */
	inline void qmin(RtInt aQMin)
	{
		m_qmin = aQMin;
	}

	/** @brief Gets the higher clamp for the dither value (absoule).
	 *
	 *  @return Higher clamp for the dither value (absoule).
	 */
	inline RtInt qmax() const
	{
		return m_qmax;
	}

	/** @brief Sets the higher clamp for the dither value (absoule).
	 *
	 *  @param aQMax Higher clamp for the dither value (absoule).
	 */
	inline void qmax(RtInt aQMax)
	{
		m_qmax = aQMax;
	}

	/** @brief Gets the dither amplitude.
	 *
	 *  @return The dither amplitude.
	 */
	inline RtFloat ditheramplitude() const
	{
		return m_ditheramplitude;
	}

	/** @brief Sets the dither amplitude.
	 *
	 *  @param aDitherAmplitude The dither amplitude.
	 */
	inline void ditheramplitude(RtFloat aDitherAmplitude)
	{
		m_ditheramplitude = aDitherAmplitude;
	}

	/** @brief Gets the values of the quantizer
	 *
	 *  @retval aType The type of quantization (RI_RGBA or RI_Z).
	 *  @retval aOne The corresponding resulting integer value for RtFloat 1.0.
	 *  @retval aQmin Lower clamp for the dither value (absoule).
	 *  @retval aQmax Higher clamp for the dither value (absoule).
	 *  @retval aDitherAmplitude Amplitude (+/-) for the dither offset.
	 */
	inline void get(
		RtToken &aType,
		RtInt &aOne,
		RtInt &aQmin,
		RtInt &aQmax,
		RtFloat &aDitherAmplitude) const
	{
		aType = m_type;
		aOne = m_one;
		aQmin = m_qmin;
		aQmax = m_qmax;
		aDitherAmplitude = m_ditheramplitude;
	}

	/** @brief Sets the values of the quantizer
	 *
	 *  @param aType The type of quantization (RI_RGBA or RI_Z).
	 *  @param aOne The corresponding resulting integer value for RtFloat 1.0.
	 *  @param aQmin Lower clamp for the dither value (absoule).
	 *  @param aQmax Higher clamp for the dither value (absoule).
	 *  @param aDitherAmplitude Amplitude (+/-) for the dither offset.
	 */
	inline void set(
		RtToken aType,
		RtInt aOne,
		RtInt aQmin,
		RtInt aQmax,
		RtFloat aDitherAmplitude)
	{
		m_type = aType;
		m_one = aOne;
		m_qmin = aQmin;
		m_qmax = aQmax;
		m_ditheramplitude = aDitherAmplitude;
	}

	inline virtual void preProcess(IDoRender &ri, const IArchiveCallback *cb)
	{
		ri.preQuantize(m_type, m_one, m_qmin, m_qmax, m_ditheramplitude);
	}

	inline virtual void doProcess(IDoRender &ri, const IArchiveCallback *cb)
	{
		ri.doQuantize(m_type, m_one, m_qmin, m_qmax, m_ditheramplitude);
	}

	inline virtual void postProcess(IDoRender &ri, const IArchiveCallback *cb)
	{
		ri.postQuantize(m_type, m_one, m_qmin, m_qmax, m_ditheramplitude);
	}

	/** @brief Assignment.
	 *
	 *  @param c CRManInterfaceCall to assign
	 *  @return A reference to this object.
	 */
	inline CRiQuantize &operator=(const CRiQuantize &c)
	{
		if ( this == &c )
			return *this;

		set(c.type(), c.one(), c.qmin(), c.qmax(), c.ditheramplitude());

		CRManInterfaceCall::operator=(c);
		return *this;
	}
}; // CRiQuantize

///////////////////////////////////////////////////////////////////////////////
/** @brief New display channel for output
 */
class CRiDisplayChannel : public CVarParamRManInterfaceCall {
private:
	std::string m_channel; //!< Name of the display channel

public:
	/** @brief Gets name for the class.
	 *
	 *  @return The name of the class (can be used as atomized string)
	 */
	inline static const char *myClassName(void) { return "CRiDisplayChannel"; }

	inline virtual const char *className() const { return CRiDisplayChannel::myClassName(); }

	inline virtual bool isA(const char *atomizedClassName) const
	{
		return ( atomizedClassName == myClassName() );
	}

	inline virtual bool isKindOf(const char *atomizedClassName) const
	{
		if ( atomizedClassName == myClassName() )
			return true;
		return CVarParamRManInterfaceCall::isKindOf(atomizedClassName);
	}

	/** @brief Default constructor
	 *
	 *  @param aLineNo The line number to store, if aLineNo is initialized to -1 (a line number is not known)
	 *  @param aChannel Name of the display channel.
	 */
	inline CRiDisplayChannel(
		long aLineNo = -1,
		RtString aChannel = RI_NULL)
		: CVarParamRManInterfaceCall(aLineNo)
	{
		m_channel = noNullStr(aChannel);
	}

	/** @brief Constructor
	 *
	 *  @param aLineNo The line number to store, if aLineNo is initialized to -1 (a line number is not known)
	 *  @param dict Dictonary with the current declarations.
	 *  @param p Counters (vertices, corners etc.) of the request.
	 *  @param curColorDescr Current color descriptor.
	 *  @param aChannel Name of the display channel.
	 *  @param n Number of parameters (size of @a tokens, @a params).
	 *  @param tokens Tokens of the request.
	 *  @param params Parameter values of the request.
	 */
	inline CRiDisplayChannel(
		long aLineNo, CDeclarationDictionary &decl, const CColorDescr &curColorDescr,
		RtString aChannel,
		RtInt n, RtToken tokens[], RtPointer params[])
		: CVarParamRManInterfaceCall(aLineNo)
	{
		CParameterClasses p;
		setParams(decl, p, curColorDescr, n, tokens, params);
		m_channel = noNullStr(aChannel);
	}

	/** @brief Constructor.
	 *
	 *  @param aLineNo The line number to store, if aLineNo is initialized to -1 (a line number is not known)
	 *  @param aChannel Name of the display channel.
	 *  @param parameters Parsed parameter list.
	 */
	inline CRiDisplayChannel(
		long aLineNo,
		RtToken aChannel,
		const CParameterList &theParameters
		)
		: CVarParamRManInterfaceCall(aLineNo, theParameters)
	{
		m_channel = noNullStr(aChannel);
	}

	/** @brief Copy constructor.
	 *
	 *  @param c Object to copy.
	 */
	inline CRiDisplayChannel(const CRiDisplayChannel &c)
	{
		*this = c;
	}

	/** @brief Destructor.
	 */
	inline virtual ~CRiDisplayChannel()
	{
	}

	inline virtual CRManInterfaceCall *duplicate() const
	{
		return new CRiDisplayChannel(*this);
	}

	inline virtual EnumRequests interfaceIdx() const { return REQ_DISPLAY_CHANNEL; }

	/** @brief Gets the name of the display channel.
	 *
	 *  @return The name of the display channel
	 */
	inline RtString channel() const
	{
		return m_channel.c_str();
	}

	/** @brief Sets the name of the display channel.
	 *
	 *  @param aChannel The name of the display channel
	 */
	inline void channel(RtToken aChannel)
	{
		m_channel = noNullStr(aChannel);
	}

	inline virtual void preProcess(IDoRender &ri, const IArchiveCallback *cb)
	{
		ri.preDisplayChannel(m_channel.c_str(), parameters());
	}

	inline virtual void doProcess(IDoRender &ri, const IArchiveCallback *cb)
	{
		ri.doDisplayChannel(m_channel.c_str(), parameters());
	}

	inline virtual void postProcess(IDoRender &ri, const IArchiveCallback *cb)
	{
		ri.postDisplayChannel(m_channel.c_str(), parameters());
	}

	/** @brief Assignment.
	 *
	 *  @param c CRManInterfaceCall to assign
	 *  @return A reference to this object.
	 */
	inline CRiDisplayChannel &operator=(const CRiDisplayChannel &c)
	{
		if ( this == &c )
			return *this;

		channel(c.channel());

		CVarParamRManInterfaceCall::operator=(c);
		return *this;
	}
}; // CRiDisplayChannel

///////////////////////////////////////////////////////////////////////////////
/** @brief Set an output display description.
 */
class CRiDisplay : public CVarParamRManInterfaceCall {
private:
	std::string m_name; //!< Name of the display.
	RtToken m_type, //!< Atomized type of the display.
	        m_mode; //!< Atomized mode of the output.

public:
	/** @brief Gets name for the class.
	 *
	 *  @return The name of the class (can be used as atomized string)
	 */
	inline static const char *myClassName(void) { return "CRiDisplay"; }

	inline virtual const char *className() const { return CRiDisplay::myClassName(); }

	inline virtual bool isA(const char *atomizedClassName) const
	{
		return ( atomizedClassName == myClassName() );
	}

	inline virtual bool isKindOf(const char *atomizedClassName) const
	{
		if ( atomizedClassName == myClassName() )
			return true;
		return CVarParamRManInterfaceCall::isKindOf(atomizedClassName);
	}

	/** @brief Default constructor
	 *
	 *  @param aLineNo The line number to store, if aLineNo is initialized to -1 (a line number is not known)
	 *  @param aName Name of the display.
	 *  @param aType Atomized type of the display.
	 *  @param aMode Atomized mode of the output.
	 */
	inline CRiDisplay(
		long aLineNo = -1,
		RtString aName = RI_NULL, RtToken aType = RI_NULL, RtToken aMode = RI_NULL)
		: CVarParamRManInterfaceCall(aLineNo),
		m_name(noNullStr(aName)), m_type(aType), m_mode(aMode)
	{
	}

	/** @brief Constructor
	 *
	 *  @param aLineNo The line number to store, if aLineNo is initialized to -1 (a line number is not known)
	 *  @param dict Dictonary with the current declarations.
	 *  @param p Counters (vertices, corners etc.) of the request.
	 *  @param curColorDescr Current color descriptor.
	 *  @param aName Name of the display.
	 *  @param aType Atomized type of the display.
	 *  @param aMode Atomized mode of the output.
	 *  @param n Number of parameters (size of @a tokens, @a params).
	 *  @param tokens Tokens of the request.
	 *  @param params Parameter values of the request.
	 */
	inline CRiDisplay(
		long aLineNo, CDeclarationDictionary &decl, const CColorDescr &curColorDescr,
		RtString aName, RtToken aType, RtToken aMode,
		RtInt n, RtToken tokens[], RtPointer params[])
		: CVarParamRManInterfaceCall(aLineNo),
		  m_name(noNullStr(aName)), m_type(aType), m_mode(aMode)
	{
		CParameterClasses p;
		setParams(decl, p, curColorDescr, n, tokens, params);
	}

	/** @brief Constructor.
	 *
	 *  @param aLineNo The line number to store, if aLineNo is initialized to -1 (a line number is not known)
	 *  @param aName Name of the display.
	 *  @param aType Atomized type of the display.
	 *  @param aMode Atomized mode of the output.
	 *  @param parameters Parsed parameter list.
	 */
	inline CRiDisplay(
		long aLineNo,
		RtString aName, RtToken aType, RtToken aMode,
		const CParameterList &theParameters
		)
		: CVarParamRManInterfaceCall(aLineNo, theParameters),
		  m_name(noNullStr(aName)), m_type(aType), m_mode(aMode)
	{
	}

	/** @brief Copy constructor.
	 *
	 *  @param c Object to copy.
	 */
	inline CRiDisplay(const CRiDisplay &c)
	{
		*this = c;
	}

	/** @brief Destructor.
	 */
	inline virtual ~CRiDisplay()
	{
	}

	inline virtual CRManInterfaceCall *duplicate() const
	{
		return new CRiDisplay(*this);
	}

	inline virtual EnumRequests interfaceIdx() const { return REQ_DISPLAY; }

	/** @brief Gets the name of the display.
	 *
	 *  @return The name of the display.
	 */
	inline RtString name() const
	{
		return m_name.c_str();
	}

	/** @brief Sets the name of the display.
	 *
	 *  @param aName The name of the display.
	 */
	inline void name(RtString aName)
	{
		m_name = noNullStr(aName);
	}

	/** @brief Gets the type of the display.
	 *
	 *  @return Atomized type of the display.
	 */
	inline RtToken type() const
	{
		return m_type;
	}

	/** @brief Dets the type of the display.
	 *
	 *  @param aType Atomized type of the display.
	 */
	inline void type(RtToken aType)
	{
		m_type = aType;
	}

	/** @brief Gets the mode of the output.
	 *
	 *  @return Atomized mode of the output.
	 */
	inline RtToken mode() const
	{
		return m_mode;
	}

	/** @brief Sets the mode of the output.
	 *
	 *  @param aMode Atomized mode of the output.
	 */
	inline void mode(RtToken aMode)
	{
		m_mode = aMode;
	}

	/** @brief Gets the display values.
	 *
	 *  @retval aName Name of the display.
	 *  @retval aType Atomized type of the display.
	 *  @retval aMode Atomized mode of the output.
	 */
	inline void get(RtString *aName, RtToken &aType, RtToken &aMode) const
	{
		if ( aName )
			*aName = m_name.c_str();
		aType = m_type;
		aMode = m_mode;
	}

	/** @brief Sets the display values.
	 *
	 *  @param aName Name of the display.
	 *  @param aType Atomized type of the display.
	 *  @param aMode Atomized mode of the output.
	 */
	inline void set(RtString aName, RtToken aType, RtToken aMode)
	{
		m_name = noNullStr(aName);
		m_type = aType;
		m_mode = aMode;
	}

	inline virtual void preProcess(IDoRender &ri, const IArchiveCallback *cb)
	{
		ri.preDisplay(m_name.c_str(), m_type, m_mode, parameters());
	}

	inline virtual void doProcess(IDoRender &ri, const IArchiveCallback *cb)
	{
		ri.doDisplay(m_name.c_str(), m_type, m_mode, parameters());
	}

	inline virtual void postProcess(IDoRender &ri, const IArchiveCallback *cb)
	{
		ri.postDisplay(m_name.c_str(), m_type, m_mode, parameters());
	}

	/** @brief Assignment.
	 *
	 *  @param c CRManInterfaceCall to assign
	 *  @return A reference to this object.
	 */
	inline CRiDisplay &operator=(const CRiDisplay &c)
	{
		if ( this == &c )
			return *this;

		set(c.name(), c.type(), c.mode());

		CVarParamRManInterfaceCall::operator=(c);
		return *this;
	}
}; // CRiDisplay

///////////////////////////////////////////////////////////////////////////////
/** @brief The hidden surface algorithm.
 */
class CRiHider : public CVarParamRManInterfaceCall {
private:
	RtToken m_type; //!< Atomized type of the hidden surface algorithm.

public:
	/** @brief Gets name for the class.
	 *
	 *  @return The name of the class (can be used as atomized string)
	 */
	inline static const char *myClassName(void) { return "CRiHider"; }

	inline virtual const char *className() const { return CRiHider::myClassName(); }

	inline virtual bool isA(const char *atomizedClassName) const
	{
		return ( atomizedClassName == myClassName() );
	}

	inline virtual bool isKindOf(const char *atomizedClassName) const
	{
		if ( atomizedClassName == myClassName() )
			return true;
		return CVarParamRManInterfaceCall::isKindOf(atomizedClassName);
	}

	/** @brief Default constructor
	 *
	 *  @param aLineNo The line number to store, if aLineNo is initialized to -1 (a line number is not known)
	 *  @param aType Atomized type of the hidden surface algorithm.
	 */
	inline CRiHider(
		long aLineNo = -1,
		RtToken aType = COptions::defHiderType)
		: CVarParamRManInterfaceCall(aLineNo), m_type(aType)
	{
	}

	/** @brief Constructor
	 *
	 *  @param aLineNo The line number to store, if aLineNo is initialized to -1 (a line number is not known)
	 *  @param dict Dictonary with the current declarations.
	 *  @param p Counters (vertices, corners etc.) of the request.
	 *  @param curColorDescr Current color descriptor.
	 *  @param aType Atomized type of the hidden surface algorithm.
	 *  @param n Number of parameters (size of @a tokens, @a params).
	 *  @param tokens Tokens of the request.
	 *  @param params Parameter values of the request.
	 */
	inline CRiHider(
		long aLineNo, CDeclarationDictionary &decl, const CColorDescr &curColorDescr,
		RtToken aType,
		RtInt n, RtToken tokens[], RtPointer params[])
		: CVarParamRManInterfaceCall(aLineNo), m_type(aType)
	{
		CParameterClasses p;
		setParams(decl, p, curColorDescr, n, tokens, params);
	}

	/** @brief Constructor.
	 *
	 *  @param aLineNo The line number to store, if aLineNo is initialized to -1 (a line number is not known)
	 *  @param aType Atomized type of the hidden surface algorithm.
	 *  @param parameters Parsed parameter list.
	 */
	inline CRiHider(
		long aLineNo,
		RtToken aType,
		const CParameterList &theParameters
		)
		: CVarParamRManInterfaceCall(aLineNo, theParameters), m_type(aType)
	{
	}

	/** @brief Copy constructor.
	 *
	 *  @param c Object to copy.
	 */
	inline CRiHider(const CRiHider &c)
	{
		*this = c;
	}

	/** @brief Destructor.
	 */
	inline virtual ~CRiHider()
	{
	}

	inline virtual CRManInterfaceCall *duplicate() const
	{
		return new CRiHider(*this);
	}

	inline virtual EnumRequests interfaceIdx() const { return REQ_HIDER; }

	/** @brief Gets the type of the hidden surface algorithm.
	 *
	 *  @return Atomized type of the hidden surface algorithm.
	 */
	inline RtToken type() const
	{
		return m_type;
	}

	/** @brief Sets the type of the hidden surface algorithm.
	 *
	 *  @param aType Atomized type of the hidden surface algorithm.
	 */
	inline void type(RtToken aType)
	{
		m_type = aType;
	}

	inline virtual void preProcess(IDoRender &ri, const IArchiveCallback *cb)
	{
		ri.preHider(m_type, parameters());
	}

	inline virtual void doProcess(IDoRender &ri, const IArchiveCallback *cb)
	{
		ri.doHider(m_type, parameters());
	}

	inline virtual void postProcess(IDoRender &ri, const IArchiveCallback *cb)
	{
		ri.postHider(m_type, parameters());
	}

	/** @brief Assignment.
	 *
	 *  @param c CRManInterfaceCall to assign
	 *  @return A reference to this object.
	 */
	inline CRiHider &operator=(const CRiHider &c)
	{
		if ( this == &c )
			return *this;

		type(c.type());

		CVarParamRManInterfaceCall::operator=(c);
		return *this;
	}
}; // CRiHider

///////////////////////////////////////////////////////////////////////////////
/** @brief the samples per color and color transformation to/from RGB.
 */
class CRiColorSamples : public CRManInterfaceCall {
private:
	CColorDescr m_colorDecr; //!< Stored color descriptor.

public:
	/** @brief Gets name for the class.
	 *
	 *  @return The name of the class (can be used as atomized string)
	 */
	inline static const char *myClassName(void) { return "CRiColorSamples"; }

	inline virtual const char *className() const { return CRiColorSamples::myClassName(); }

	inline virtual bool isA(const char *atomizedClassName) const
	{
		return ( atomizedClassName == myClassName() );
	}

	inline virtual bool isKindOf(const char *atomizedClassName) const
	{
		if ( atomizedClassName == myClassName() )
			return true;
		return CRManInterfaceCall::isKindOf(atomizedClassName);
	}

	/** @brief Default constructor
	 *
	 *  Per Default no color transformation is done (transformation is the identity).
	 *
	 *  @param aLineNo The line number to store, if aLineNo is initialized to -1 (a line number is not known)
	 */
	inline CRiColorSamples(long aLineNo=-1)
		: CRManInterfaceCall(aLineNo)
	{
	}

	/** @brief Constructor
	 *
	 *  @param aLineNo The line number to store, if aLineNo is initialized to -1 (a line number is not known)
	 *  @param N Number of color samples (>0)
	 *  @param nRGB Transformation matrix from a color to a RGB value
	 *  @param RGBn Transformation matrix from a RGB value to a color
	 */
	inline CRiColorSamples(long aLineNo, RtInt N, RtFloat *nRGB, RtFloat *RGBn)
		: CRManInterfaceCall(aLineNo), m_colorDecr(N, nRGB, RGBn)
	{
	}

	/** @brief Copy constructor.
	 *
	 *  @param c Object to copy.
	 */
	inline CRiColorSamples(const CRiColorSamples &c)
	{
		*this = c;
	}

	/** @brief Destructor.
	 */
	inline virtual ~CRiColorSamples()
	{
	}

	inline virtual CRManInterfaceCall *duplicate() const
	{
		return new CRiColorSamples(*this);
	}

	inline virtual EnumRequests interfaceIdx() const { return REQ_COLOR_SAMPLES; }

	/** @brief Gets the number color samples.
	 *
	 * @return The number of color samples.
	 */
	inline RtInt colorSamples() const
	{
		return m_colorDecr.colorSamples();
	}

	/** @brief Gets the number color samples and transformation matrices.
	 *
	 * @see CColorDescr::getColorSamples()
	 *
	 * @retval nColorSamples The number of color samples.
	 * @retval nRGB Matrix color to RGB.
	 * @retval RGBn Matrix RGB to color.
	 */
	inline void get(RtInt nColorSamples, std::vector<RtFloat> &nRGB, std::vector<RtFloat> &RGBn) const
	{
		m_colorDecr.getColorSamples(nColorSamples, nRGB, RGBn);
	}

	/** @brief Sets the number color samples and transformation matrices.
	 *
	 * @see CColorDescr::colorSamples()
	 *
	 * @param nColorSamples The number of color samples (>0).
	 * @param nRGB Matrix color to RGB (should not be 0).
	 * @param RGBn Matrix RGB to color (should not be 0).
	 */
	inline RtVoid set(RtInt nColorSamples, RtFloat *nRGB, RtFloat *RGBn)
	{
		m_colorDecr.colorSamples(nColorSamples, nRGB, RGBn);
	}

	/** @brief Gets the color descriptor, read-only.
	 *
	 *  @return The read-only color descriptor.
	 */
	inline const CColorDescr &colorDescr() const
	{
		return m_colorDecr;
	}

	/** @brief Gets the color descriptor.
	 *
	 *  @return The color descriptor.
	 */
	inline CColorDescr &colorDescr()
	{
		return m_colorDecr;
	}

	/** @brief Sets the color descriptor.
	 *
	 *  @param aColorDescr A color descriptor.
	 */
	inline void colorDescr(const CColorDescr &aColorDescr)
	{
		m_colorDecr = aColorDescr;
	}

	inline virtual void preProcess(IDoRender &ri, const IArchiveCallback *cb)
	{
		std::vector<RtFloat> &nRGB = m_colorDecr.nRGBMatrix();
		std::vector<RtFloat> &RGBn = m_colorDecr.RGBnMatrix();

		ri.preColorSamples(
			colorSamples(),
			nRGB.empty() ? 0 : &nRGB[0],
			RGBn.empty() ? 0 : &RGBn[0]);
	}

	inline virtual void doProcess(IDoRender &ri, const IArchiveCallback *cb)
	{
		std::vector<RtFloat> &nRGB = m_colorDecr.nRGBMatrix();
		std::vector<RtFloat> &RGBn = m_colorDecr.RGBnMatrix();

		ri.doColorSamples(
			colorSamples(),
			nRGB.empty() ? 0 : &nRGB[0],
			RGBn.empty() ? 0 : &RGBn[0]);
	}

	inline virtual void postProcess(IDoRender &ri, const IArchiveCallback *cb)
	{
		std::vector<RtFloat> &nRGB = m_colorDecr.nRGBMatrix();
		std::vector<RtFloat> &RGBn = m_colorDecr.RGBnMatrix();

		ri.postColorSamples(
			colorSamples(),
			nRGB.empty() ? 0 : &nRGB[0],
			RGBn.empty() ? 0 : &RGBn[0]);
	}

	/** @brief Assignment.
	 *
	 *  @param c CRManInterfaceCall to assign
	 *  @return A reference to this object.
	 */
	inline CRiColorSamples &operator=(const CRiColorSamples &c)
	{
		if ( this == &c )
			return *this;

		colorDescr(c.colorDescr());

		CRManInterfaceCall::operator=(c);
		return *this;
	}
}; // CRiColorSamples

///////////////////////////////////////////////////////////////////////////////
/** @brief Sets the relative detail scale.
 */
class CRiRelativeDetail : public CRManInterfaceCall {
private:
	RtFloat m_relativedetail; //!< The relative detail scale.

public:
	/** @brief Gets name for the class.
	 *
	 *  @return The name of the class (can be used as atomized string)
	 */
	inline static const char *myClassName(void) { return "CRiRelativeDetail"; }

	inline virtual const char *className() const { return CRiRelativeDetail::myClassName(); }

	inline virtual bool isA(const char *atomizedClassName) const
	{
		return ( atomizedClassName == myClassName() );
	}

	inline virtual bool isKindOf(const char *atomizedClassName) const
	{
		if ( atomizedClassName == myClassName() )
			return true;
		return CRManInterfaceCall::isKindOf(atomizedClassName);
	}

	/** @brief Default constructor.
	 *
	 *  @param aLineNo The line number to store, if aLineNo is initialized to -1 (a line number is not known)
	 *  @param aRelativedetail A relative detail scale.
	 */
	inline CRiRelativeDetail(
		long aLineNo = -1,
		RtFloat aRelativedetail = COptions::defRelativeDetail)
		: CRManInterfaceCall(aLineNo), m_relativedetail(aRelativedetail)
	{
	}

	/** @brief Copy constructor.
	 *
	 *  @param c Object to copy.
	 */
	inline CRiRelativeDetail(const CRiRelativeDetail &c)
	{
		*this = c;
	}

	/** @brief Destructor.
	 */
	inline virtual ~CRiRelativeDetail()
	{
	}

	inline virtual CRManInterfaceCall *duplicate() const
	{
		return new CRiRelativeDetail(*this);
	}

	inline EnumRequests interfaceIdx() const { return REQ_RELATIVE_DETAIL; }

	/** @brief Gets the relative detail scale.
	 *
	 *  @return The relative detail scale.
	 */
	inline virtual RtFloat relativeDetail() const
	{
		return m_relativedetail;
	}

	/** @brief Sets the relative detail scale.
	 *
	 *  @return The relative detail scale.
	 */
	inline void relativeDetail(RtFloat aRelativedetail)
	{
		m_relativedetail = aRelativedetail;
	}

	inline virtual void preProcess(IDoRender &ri, const IArchiveCallback *cb)
	{
		ri.preRelativeDetail(m_relativedetail);
	}

	inline virtual void doProcess(IDoRender &ri, const IArchiveCallback *cb)
	{
		ri.doRelativeDetail(m_relativedetail);
	}

	inline virtual void postProcess(IDoRender &ri, const IArchiveCallback *cb)
	{
		ri.postRelativeDetail(m_relativedetail);
	}

	/** @brief Assignment.
	 *
	 *  @param c CRManInterfaceCall to assign
	 *  @return A reference to this object.
	 */
	inline CRiRelativeDetail &operator=(const CRiRelativeDetail &c) {
		if ( this == &c )
			return *this;

		relativeDetail(c.relativeDetail());

		CRManInterfaceCall::operator=(c);
		return *this;
	}
}; // CRiRelativeDetail

///////////////////////////////////////////////////////////////////////////////
/** @brief User defined option.
 */
class CRiOption : public CVarParamRManInterfaceCall {
private:
	RtToken m_name; //!< Name of the option as atomized string.

public:
	/** @brief Gets name for the class.
	 *
	 *  @return The name of the class (can be used as atomized string)
	 */
	inline static const char *myClassName(void) { return "CRiOption"; }

	inline virtual const char *className() const { return CRiOption::myClassName(); }

	inline virtual bool isA(const char *atomizedClassName) const
	{
		return ( atomizedClassName == myClassName() );
	}

	inline virtual bool isKindOf(const char *atomizedClassName) const
	{
		if ( atomizedClassName == myClassName() )
			return true;
		return CVarParamRManInterfaceCall::isKindOf(atomizedClassName);
	}

	/** @brief Default constructor.
	 *
	 *  @param aLineNo The line number to store, if aLineNo is initialized to -1 (a line number is not known)
	 *  @param aName Name of the option as atomized string.
	 */
	inline CRiOption(
		long aLineNo = -1,
		RtToken aName = RI_NULL)
		: CVarParamRManInterfaceCall(aLineNo), m_name(aName)
	{
	}

	/** @brief Constructor.
	 *
	 *  @param aLineNo The line number to store, if aLineNo is initialized to -1 (a line number is not known)
	 *  @param dict Dictonary with the current declarations.
	 *  @param p Counters (vertices, corners etc.) of the request.
	 *  @param curColorDescr Current color descriptor.
	 *  @param aName Name of the option as atomized string.
	 *  @param n Number of parameters (size of @a tokens, @a params).
	 *  @param tokens Tokens of the request.
	 *  @param params Parameter values of the request.
	 */
	inline CRiOption(
		long aLineNo, CDeclarationDictionary &decl, const CColorDescr &curColorDescr,
		RtToken name,
		RtInt n, RtToken tokens[], RtPointer params[])
		: CVarParamRManInterfaceCall(aLineNo), m_name(name)
	{
		CParameterClasses p;
		setParams(decl, p, curColorDescr, n, tokens, params);
	}

	/** @brief Constructor.
	 *
	 *  @param aLineNo The line number to store, if aLineNo is initialized to -1 (a line number is not known)
	 *  @param aName Name of the option as atomized string.
	 *  @param parameters Parsed parameter list.
	 */
	inline CRiOption(
		long aLineNo,
		RtToken aName,
		const CParameterList &theParameters
		)
		: CVarParamRManInterfaceCall(aLineNo, theParameters), m_name(aName)
	{
	}


	/** @brief Copy constructor.
	 *
	 *  @param c Object to copy.
	 */
	inline CRiOption(const CRiOption &c)
	{
		*this = c;
	}

	/** @brief Destructor.
	 */
	inline virtual ~CRiOption()
	{
	}

	inline virtual CRManInterfaceCall *duplicate() const
	{
		return new CRiOption(*this);
	}

	inline virtual EnumRequests interfaceIdx() const { return REQ_OPTION; }

	/** @brief Gets the name of the option as atomized string.
	 *
	 *  @return The name of the option as atomized string.
	 */
	inline RtToken name() const
	{
		return m_name;
	}

	/** @brief Sets the name of the option as atomized string.
	 *
	 *  @param aName The name of the option as atomized string.
	 */
	inline void name(RtToken aName)
	{
		m_name = aName;
	}

	inline virtual void preProcess(IDoRender &ri, const IArchiveCallback *cb)
	{
		ri.preOption(m_name, parameters());
	}

	inline virtual void doProcess(IDoRender &ri, const IArchiveCallback *cb)
	{
		ri.doOption(m_name, parameters());
	}

	inline virtual void postProcess(IDoRender &ri, const IArchiveCallback *cb)
	{
		ri.postOption(m_name, parameters());
	}

	/** @brief Assignment.
	 *
	 *  @param c CRManInterfaceCall to assign
	 *  @return A reference to this object.
	 */
	inline CRiOption &operator=(const CRiOption &c)
	{
		if ( this == &c )
			return *this;

		name(c.name());

		CVarParamRManInterfaceCall::operator=(c);
		return *this;
	}
}; // CRiOption

}

#endif // // _RICPP_RENDERSTATE_RIMACROOPTIONS_H
