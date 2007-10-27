#ifndef _RICPP_RICONTEXT_DISPLAYDESC_H
#define _RICPP_RICONTEXT_DISPLAYDESC_H

/*! \file displaydesc.h
 *  \brief Declaration of the CDisplayDescr display descriptor
 *         (stores contents of a RiDisplay call)
 *  \author Andreas Pidde (andreas@pidde.de)
 */

#ifndef _RICPP_RICONTEXT_PARAMETER_H
#include "ricpp/ricontext/parameter.h"
#endif // _RICPP_RICONTEXT_PARAMETER_H

#ifndef _RICPP_DECLARATION_DECLDICT_H
#include "ricpp/declaration/decldict.h"
#endif // _RICPP_DECLARATION_DECLDICT_H

#ifndef _RICPP_TOOLS_STRINGLIST_H
#include "ricpp/tools/stringlist.h"
#endif // _RICPP_TOOLS_STRINGLIST_H

namespace RiCPP {

	class CDisplayChannelDescr : public CNamedParameterList {
		CDeclaration *m_channel;

	public:
		CDisplayChannelDescr();
		virtual ~CDisplayChannelDescr();
		CDisplayChannelDescr(const CDisplayChannelDescr &dcd);
		CDisplayChannelDescr &operator=(const CDisplayChannelDescr &dcd);
		bool operator==(const CDisplayChannelDescr &dcd) const;
		bool operator==(const char *name) const;
		bool CDisplayChannelDescr::operator==(std::string name) const;

		RtVoid displayChannelV(CDeclarationDictionary &dict, const CColorDescr &colorDescr, RtString aChannel, RtInt n, RtToken tokens[], RtPointer params[]);
		RtVoid displayChannel(CDeclarationDictionary &dict, const CColorDescr &colorDescr, RtString aChannel, const CParameterList &params);

		inline const CDeclaration *channel() const
		{
			return m_channel;
		}

	}; // CDisplayChannelDescr


	////////////////////////////////////////////////////////////////////////////////
	//! A display descriptor to store and access standard TRi::display() parameters.
	/*! The parameters are stored, because there can be multiple displays (e.g.
	 *  a file with depth coordinates only and a framebuffer showing RGBA values.
	 */
	class CDisplayDescr : public CNamedParameterList {
	public:
		typedef std::list<CDisplayChannelDescr> TypeDisplayChannels;

	private:
		RtToken m_type;      //!< Type of the display, e.g. TRi::RI_FILE, TRi::RI_FRAMEBUFFER, default is TRi::RI_NULL
		RtString m_mode;      //!< Mode of the output, e.g. TRi::RI_RGBA etc., default si TRi::RI_NULL

		RtInt m_origin[2];   //!< Origin (offset from upper left), default is (0, 0)

		std::list<CDisplayChannelDescr> m_channels;
		CStringList m_channelNames;

	public:
		CDisplayDescr(); //!< Standard Constructor, fills in default values

		//! Copy constructor for deep copy
		/*! \param tdd Display description to copy
		 */
		CDisplayDescr(const CDisplayDescr &dd);

		//! Assignment, deep copy.
		/*! \param dd Display description to copy
		 */
		CDisplayDescr &operator=(const CDisplayDescr &dd);

		//! Take parameters of TRi::displayV() to fill the member variables.
		/*! \param dict Current declarations
		 *  \param curColorSize  Number of color components.
		 *  \param aName Name of the display, \sa m_name
		 *  \param aType The display type, \sa m_type
		 *  \param aMode The display mode, \sa m_mode
		 */
		void displayV(CDeclarationDictionary &dict, const CColorDescr &colorDescr, const TypeDisplayChannels &channels, RtToken aName, RtToken aType, RtString aMode, RtInt n, RtToken tokens[], RtPointer params[]);
		void display(const CDisplayDescr::TypeDisplayChannels &channels, RtToken aName, RtToken aType, RtString aMode);
		void display(const TypeDisplayChannels &channels, RtToken aName, RtToken aType, RtString aMode, const CParameterList &params);

		inline RtToken type() const
		{
			return m_type;
		}

		inline RtString mode() const
		{
			return m_mode;
		}

		inline void origin(RtInt &x0, RtInt &y0) const
		{
			x0 = m_origin[0];
			y0 = m_origin[1];
		}
		inline const CStringList &channelNames() const
		{
			return m_channelNames;
		}
	}; // CDisplayDescr

	/*! @brief Class for Viewport Data
	 */
	class CViewPort {
		RtInt   m_originX,     //!< X-origin, initialized with 0.
				m_originY;     //!< Y-origin, initialized with 0.
		RtInt   m_width,       //!< Width, initialized with -1: Size of device
				m_height;      //!< Height, initialized with -1: Size of device
		RtFloat m_pixelAspectRatio; //!< The pixel aspect ration, initialized with -1: Ratio of device pixel

	public:
		//! Constructor to set the initial values of the members
		inline CViewPort()
		{
			initialize();
		}

		inline CViewPort(const CViewPort &vp)
		{
			*this = vp;
		}

		inline CViewPort &operator=(const CViewPort &vp)
		{
			if ( this == &vp )
				return *this;
			m_originX = vp.m_originX;
			m_originY = vp.m_originY;
			m_width = vp.m_width;
			m_height = vp.m_height;
			m_pixelAspectRatio = vp.m_pixelAspectRatio;
			return *this;
		}

		//! Sets the initial values of the members
		inline void initialize()
		{
			m_originX = 0;
			m_originY = 0;
			m_width = -1;
			m_height = -1;
			m_pixelAspectRatio = (RtFloat)-1.0;
		}

		//! The aspect ratio of the viewport
		/*! Calculates the physical aspect ratio of the viewport, works only if
		 *  m_iHeight and m_iWidth are set with values > 0.0.
		 *  \return Aspect ratio of the viewport: (m_iWidth*m_fPixelAspect)/(RtFloat)m_iHeight
		 */
		inline RtFloat viewPortAspectRatio() const
		{
			return (RtFloat)( m_height == 0 ? 0.0 : (m_width*m_pixelAspectRatio)/(RtFloat)m_height );
		}

		inline void origin(RtInt x, RtInt y)
		{
			m_originX = x;
			m_originY = y;
		}

		inline RtInt originX() const
		{
			return m_originX;
		}

		inline RtInt originY() const
		{
			return m_originY;
		}


		inline void size(RtInt wid, RtInt ht)
		{
			m_width = wid;
			m_height = ht;
		}

		inline RtInt width() const
		{
			return m_width;
		}

		inline RtInt height() const
		{
			return m_height;
		}

		inline void pixelAspectRatio(RtFloat ratio)
		{
			m_pixelAspectRatio = ratio;
		}

		inline RtFloat pixelAspectRatio() const
		{
			return m_pixelAspectRatio;
		}
	}; // CViewPort

}

#endif // _RICPP_RICONTEXT_DISPLAYDESC_H
