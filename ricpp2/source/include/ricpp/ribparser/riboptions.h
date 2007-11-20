#ifndef _RICPP_RIBPARSER_RIBOPTIONS_H
#define _RICPP_RIBPARSER_RIBOPTIONS_H

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

/** @file riboptions.h
 *  @author Andreas Pidde (andreas@pidde.de)
 *  @brief Handles RIB requests for options (Shutter, ScreenWindow, Format, ...).
 *
 *  The objects are used only by the implementation of the rib options.
 */

#ifndef _RICPP_RIBPARSER_RIBPARSER_H
#include "ricpp/ribparser/ribparser.h"
#endif // _RICPP_RIBPARSER_RIBPARSER_H

namespace RiCPP {

	/** @brief Handles RIB request Format xres yres pixelaspectratio.
	 */
	class CFormatRibRequest : public CRibRequest {
	public:
		virtual void operator()(IRibParserState &parser, CRibRequestData &request) const;
		inline virtual EnumRequests interfaceIdx() const { return REQ_FORMAT; }
	}; // CFormatRibRequest

	/** @brief Handles RIB request FrameAspectRatio ratio.
	 */
	class CFrameAspectRatioRibRequest : public CRibRequest {
	public:
		virtual void operator()(IRibParserState &parser, CRibRequestData &request) const;
		inline virtual EnumRequests interfaceIdx() const { return REQ_FRAME_ASPECT_RATIO; }
	}; // CFrameAspectRatioRibRequest

	/** @brief Handles RIB request ScreenWindow left right bottom top.
	 */
	class CScreenWindowRibRequest : public CRibRequest {
	public:
		virtual void operator()(IRibParserState &parser, CRibRequestData &request) const;
		inline virtual EnumRequests interfaceIdx() const { return REQ_SCREEN_WINDOW; }
	}; // CScreenWindowRibRequest

	/** @brief Handles RIB request CropWindow xmin xmax ymin ymax.
	 */
	class CCropWindowRibRequest : public CRibRequest {
	public:
		virtual void operator()(IRibParserState &parser, CRibRequestData &request) const;
		inline virtual EnumRequests interfaceIdx() const { return REQ_CROP_WINDOW; }
	}; // CCropWindowRibRequest

	/** @brief Handles RIB request Projection type ...
	 */
	class CProjectionRibRequest : public CRibRequest {
	public:
		virtual void operator()(IRibParserState &parser, CRibRequestData &request) const;
		inline virtual EnumRequests interfaceIdx() const { return REQ_PROJECTION; }
	}; // CProjectionRibRequest
}

#endif // _RICPP_RIBPARSER_RIBOPTIONS_H
