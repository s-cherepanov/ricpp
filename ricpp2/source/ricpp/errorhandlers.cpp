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

/** @file errorhandlers.cpp
 *  @author Andreas Pidde (andreas@pidde.de)
 *  @brief The standard error handlers of the interface
 */

#include "ricpp/ricpp/errorhandlers.h"
#include <iostream>

using namespace RiCPP;

RtVoid CAbortErrorHandler::operator()(RtInt code, RtInt severity, RtString msg) const
{
	std::cout << "*** Code " <<
		"[" << code << "] " << CRiCPPErrMsg::errorMessage(code) <<
		", severity [" << severity << "] " << CRiCPPErrMsg::errorSeverity(severity) << ", '" <<
		msg << "'" << std::endl;
	if ( severity == RIE_SEVERE ) {
		std::cout << "exit." << std::endl;
		exit(code);
	}
}

RtVoid CPrintErrorHandler::operator()(RtInt code, RtInt severity, RtString msg) const
{
	std::cout << "*** Code " <<
		"[" << code << "] " << CRiCPPErrMsg::errorMessage(code) <<
		", severity [" << severity << "] " << CRiCPPErrMsg::errorSeverity(severity) << ", '" <<
		msg << "'" << std::endl;
}

RtVoid CIgnoreErrorHandler::operator()(RtInt code, RtInt severity, RtString msg) const
{
	code = code;
	severity = severity;
	msg = msg;
}
