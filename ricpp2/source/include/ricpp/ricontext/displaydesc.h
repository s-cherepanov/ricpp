#ifndef _RICPP_RICONTEXT_DISPLAYDESC_H
#define _RICPP_RICONTEXT_DISPLAYDESC_H

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

/** @file displaydesc.h
 *  @author Andreas Pidde (andreas@pidde.de)
 *  @brief Declaration of the CDisplayDescr display descriptor
 *         (stores contents of a RiDisplay call).
 */

#ifndef _RICPP_RICONTEXT_PARAMETER_H
#include "ricpp/ricontext/parameter.h"
#endif // _RICPP_RICONTEXT_PARAMETER_H

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
		CDisplayChannelDescr(const CDisplayChannelDescr &dd, CDeclarationDictionary &newDict);
		CDisplayChannelDescr &operator=(const CDisplayChannelDescr &dcd);
		CDisplayChannelDescr &assignRemap(const CDisplayChannelDescr &dcd, CDeclarationDictionary &newDict);
		bool operator==(const CDisplayChannelDescr &dcd) const;
		bool operator==(const char *name) const;
		bool operator==(const std::string name) const;

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
		RtToken m_type;      ///< Type of the display, e.g. TRi::RI_FILE, TRi::RI_FRAMEBUFFER, default is TRi::RI_NULL
		RtString m_mode;      ///< Mode of the output, e.g. TRi::RI_RGBA etc., default si TRi::RI_NULL

		RtInt m_origin[2];   ///< Origin (offset from upper left), default is (0, 0)

		RtInt   m_width,       ///< Width, initialized with -1: Size of device
		m_height;      ///< Height, initialized with -1: Size of device
		RtFloat m_pixelAspectRatio; ///< The pixel aspect ration, initialized with -1: Ratio of device pixel

		std::list<CDisplayChannelDescr> m_channels;
		CStringList m_channelNames;

	public:
		CDisplayDescr(); ///< Standard Constructor, fills in default values

		//! Copy constructor for deep copy
		/*! @param dd Display description to copy
		 */
		CDisplayDescr(const CDisplayDescr &dd);
		CDisplayDescr(const CDisplayDescr &dd, CDeclarationDictionary &newDict);

		//! Assignment, deep copy.
		/*! @param dd Display description to copy
		 */
		CDisplayDescr &operator=(const CDisplayDescr &dd);
		CDisplayDescr &assignRemap(const CDisplayDescr &dd, CDeclarationDictionary &newDict);

		//! Take parameters of TRi::displayV() to fill the member variables.
		/*! @param dict Current declarations
		 *  @param colorDescr Current color descriptor.
		 *  @param channels Descriptors (declarations) of the display channels.
		 *  @param aName Name of the display, @see m_name
		 *  @param aType The display type, @see m_type
		 *  @param aMode The display mode, @see m_mode
		 *  @param n Number of token/parameter pairs in @a tokens and @a params.
		 *  @param tokens Tokens of the token/parameter pairs.
		 *  @param params Pointer to parameters of the token/parameter pairs.
		 */
		void displayV(CDeclarationDictionary &dict, const CColorDescr &colorDescr, const TypeDisplayChannels &channels, RtToken aName, RtToken aType, RtString aMode, RtInt n, RtToken tokens[], RtPointer params[]);
		void display(const CDisplayDescr::TypeDisplayChannels &channels, RtToken aName, RtToken aType, RtString aMode);
		void display(const TypeDisplayChannels &channels, RtToken aName, RtToken aType, RtString aMode, const CParameterList &params);

		bool isPrimary() const;
		inline RtToken type() const
		{
			return m_type;
		}

		inline RtString mode() const
		{
			return m_mode;
		}

		inline void getOrigin(RtInt &x0, RtInt &y0) const
		{
			x0 = m_origin[0];
			y0 = m_origin[1];
		}
		inline const CStringList &channelNames() const
		{
			return m_channelNames;
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
	}; // CDisplayDescr

	/*! @brief Class for Viewport Data
	 */
	class CViewPort {
		RtInt   m_originX,     ///< X-origin, initialized with 0.
				m_originY;     ///< Y-origin, initialized with 0.
		RtInt   m_width,       ///< Width, initialized with -1: Size of device
				m_height;      ///< Height, initialized with -1: Size of device
		RtFloat m_pixelAspectRatio; ///< The pixel aspect ration, initialized with -1: Ratio of device pixel

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
		 *  @return Aspect ratio of the viewport: (m_iWidth*m_fPixelAspect)/(RtFloat)m_iHeight
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
