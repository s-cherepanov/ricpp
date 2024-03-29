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

#ifndef _RICPP_RICPP_RICPPERROR_H
#include "ricpp/ricpp/ricpperror.h"
#endif // _RICPP_RICPP_RICPPERROR_H

#ifndef _RICPP_RICPPBASE_RICPPDECLS_H
#include "ricpp/ribase/ricppdecls.h"
#endif // _RICPP_RICPPBASE_RICPPDECLS_H

#include <iostream>

using namespace RiCPP;

static CAbortErrorHandler abortErrorHandlerFunc;
const CAbortErrorHandler &CAbortErrorHandler::func() { return abortErrorHandlerFunc; }
RtToken CAbortErrorHandler::myName() {return RI_ABORT; }
RtVoid CAbortErrorHandler::operator()(IRi &ri, RtInt code, RtInt severity, RtString message) const
{
	RiLastError = code;
	std::cerr << "# *** Code " <<
		"[" << code << "] " << CRiCPPErrMsg::errorMessage(code) <<
		", severity [" << severity << "] " << CRiCPPErrMsg::errorSeverity(severity) << ", '" <<
		noNullStr(message) << "'" << std::endl;
	if ( severity == RIE_SEVERE ) {
		std::cerr << "# abort renderer." << std::endl;
		ri.synchronize(RI_ABORT);
		exit(code);
	}
}

static CPrintErrorHandler printErrorHandlerFunc;
const CPrintErrorHandler &CPrintErrorHandler::func() { return printErrorHandlerFunc; }
RtToken CPrintErrorHandler::myName() {return RI_PRINT; }
RtVoid CPrintErrorHandler::operator()(IRi &ri, RtInt code, RtInt severity, RtString message) const
{
	const IRi *pri = &ri;
	pri = pri+0;
	RiLastError = code;
	std::cerr << "# *** Code " <<
		"[" << code << "] " << CRiCPPErrMsg::errorMessage(code) <<
		", severity [" << severity << "] " << CRiCPPErrMsg::errorSeverity(severity) << ", '" <<
		noNullStr(message) << "'" << std::endl;
}

static CIgnoreErrorHandler ignoreErrorHandlerFunc;
const CIgnoreErrorHandler &CIgnoreErrorHandler::func() { return ignoreErrorHandlerFunc; }
RtToken CIgnoreErrorHandler::myName() {return RI_IGNORE; }
RtVoid CIgnoreErrorHandler::operator()(IRi &ri, RtInt code, RtInt severity, RtString message) const
{
	const IRi *pri = &ri;
	pri = pri+0;
	code = code+0;
	severity = severity+0;
	message = message+0;
	RiLastError = code;
}

//
// CErrorHandlerFactory
//

const IErrorHandler *CErrorHandlerFactory::newFunc(RtToken name)
{
	if ( emptyStr(name) )
		return 0;

	if ( CAbortErrorHandler::myName() == name )
		return new CAbortErrorHandler;

	if ( CPrintErrorHandler::myName() == name )
		return new CPrintErrorHandler;

	if ( CIgnoreErrorHandler::myName() == name )
		return new CIgnoreErrorHandler;

	return 0;
}

void CErrorHandlerFactory::deleteFunc(IErrorHandler *f)
{
	if ( !f )
		return;
	delete f;
}

const IErrorHandler *CErrorHandlerFactory::singleton(RtToken name) const
{
	if ( emptyStr(name) )
		return 0;

	if ( CAbortErrorHandler::myName() == name )
		return &CAbortErrorHandler::func();

	if ( CPrintErrorHandler::myName() == name )
		return &CPrintErrorHandler::func();

	if ( CIgnoreErrorHandler::myName() == name )
		return &CIgnoreErrorHandler::func();

	return 0;
}
