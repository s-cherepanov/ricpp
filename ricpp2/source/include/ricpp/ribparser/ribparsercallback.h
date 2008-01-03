#ifndef _RICPP_RIBPARSER_RIBPARSERCALLBACK_H
#define _RICPP_RIBPARSER_RIBPARSERCALLBACK_H

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

/** @file ribparsercallback.h
 *  @author Andreas Pidde (andreas@pidde.de)
 *  @brief Front end infos the rib parser needs to know.
 */

#ifndef _RICPP_RICPP_ERRORHANDLERS_H
#include "ricpp/ricpp/errorhandlers.h"
#endif // _RICPP_RICPP_ERRORHANDLERS_H

#ifndef _RICPP_RIBFILTER_RIBFILTERLIST_H
#include "ricpp/ribfilter/ribfilterlist.h"
#endif // _RICPP_RIBFILTER_RIBFILTERLIST_H

#ifndef _RICPP_STREAMS_BACKBUFFER_H
#include "ricpp/streams/backbuffer.h"
#endif // _RICPP_STREAMS_BACKBUFFER_H

namespace RiCPP {

	/** @brief Interface implemented by the front end to support the Rib parser CRibParser.
	 */
	class IRibParserCallback
	{
	public:
		/** @brief Gets the error handler of the front end.
		 *
		 *  Errors are typically not thrown while parsing. Instead the error handler of the
		 *  front end is called. Doing so, it is possible to continue parsing after an error
		 *  occured.
		 *
		 *  @return The error handler of the front end.
		 */
		virtual IRiCPPErrorHandler &ricppErrHandler() = 0;
		
		/** @brief Gets the protocol handler for stream back buffers.
		 *
		 *  Since the protocol handlers for stream back buffers are managed by the
		 *  front end, the parser needs to know this object to create the input stream.
		 *  The FILE stream buffer for example is a build in stream buffer for standard
		 *  io with optional zlib support.
		 *
		 *  @return The protocol handler for stream back buffers.
		 */
		virtual CBackBufferProtocolHandlers &protocolHandlers() = 0;
		
		/** @brief Gets the rib filter for the ri calls.
		 *
		 *  Gets the rib filter that is used for the parsed ri calls.
		 *  Users of RiCPP can provide own CRibFilter filters to manipulate
		 *  the read RIB stream. The last filter will call the interfaces
		 *  routines of the front end.
		 *
		 *  @return The rib filter for the ri calls.
		 */
		virtual IRiRoot &ribFilter() = 0;

		/** @brief Gets the frontend for RI-Calls (as used in procedurals)
		 *
		 *  @return Frontend for RI-calls
		 */
		virtual IRi &frontend() = 0;
	}; // IRibParserCallback

}

#endif // _RICPP_RIBPARSER_RIBPARSERCALLBACK_H
