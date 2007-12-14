#ifndef _RICPP_RIBPARSER_RIBMISC_H
#define _RICPP_RIBPARSER_RIBMISC_H

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

/** @file ribmisc.h
 *  @author Andreas Pidde (andreas@pidde.de)
 *  @brief Handles RIB requests that cannot be sorted to the other groups (incl. Version).
 *
 *  The objects are used only by the implementation of the rib parser.
 */

#ifndef _RICPP_RIBPARSER_RIBPARSER_H
#include "ricpp/ribparser/ribparser.h"
#endif // _RICPP_RIBPARSER_RIBPARSER_H

namespace RiCPP {
	/** @brief Handles RIB request ErrorHandler handlername
	 */
	class CErrorHandlerRibRequest : public CRibRequest {
	public:
		virtual void operator()(IRibParserState &parser, CRibRequestData &request) const;
		inline virtual EnumRequests interfaceIdx() const { return REQ_ERROR_HANDLER; }
	}; // CErrorHandlerRibRequest

	/** @brief Handles RIB request Declare name declaration
	 */
	class CDeclareRibRequest : public CRibRequest {
	public:
		virtual void operator()(IRibParserState &parser, CRibRequestData &request) const;
		inline virtual EnumRequests interfaceIdx() const { return REQ_DECLARE; }
	}; // CDeclareRibRequest

	/** @brief Handles RIB request ReadArchive filename
	 */
	class CReadArchiveRibRequest : public CRibRequest {
	public:
		virtual void operator()(IRibParserState &parser, CRibRequestData &request) const;
		inline virtual EnumRequests interfaceIdx() const { return REQ_READ_ARCHIVE; }
	}; // CReadArchiveRibRequest

	/** @brief Handles RIB request Version versionnumber
	 */
	class CVersionRibRequest : public CRibRequest {
	public:
		virtual void operator()(IRibParserState &parser, CRibRequestData &request) const;
		inline virtual EnumRequests interfaceIdx() const { return REQ_VERSION; }
	}; // CVersionRibRequest
}

#endif // _RICPP_RIBPARSER_RIBMISC_H
