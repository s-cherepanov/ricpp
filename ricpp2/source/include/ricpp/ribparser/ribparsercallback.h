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
 *  @brief Front End infos the rib parser need to know
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

	class IRibParserCallback
	{
	public:
		inline virtual IRiCPPErrorHandler &ricppErrHandler() = 0;
		inline virtual CBackBufferProtocolHandlers &protocolHandlers() = 0;
		inline virtual IRiRoot &ribFilter() = 0;
	}; // IRibParserCallback

}

#endif // _RICPP_RIBPARSER_RIBPARSERCALLBACK_H
