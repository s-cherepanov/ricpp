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

/** @file ricpp.cpp
 *  @author Andreas Pidde (andreas@pidde.de)
 *  @brief Strings for the RenderMan(R) C++ Binding
 */

#include "ricpp/ricpp/ricpp.h"

using namespace RiCPP;

// Error messages RIE_ as clear text

RtString CRiCPPErrMsg::errorMessage(RtInt aCode)
{
	RtString riErrorMessages[] = {
		"RIE_NOERROR",
		"RIE_NOMEM: Out of memory",
		"RIE_SYSTEM: Miscellaneous system error",
		"RIE_NOFILE: File nonexistant",
		"RIE_BADFILE: Bad file format",
		"RIE_VERSION: File version mismatch",
		"RIE_DISKFULL: Target disk is full",
		RI_UNKNOWN,
		RI_UNKNOWN,
		RI_UNKNOWN,
		RI_UNKNOWN,
		"RIE_INCAPABLE, RIE_OPTIONAL: Optional RI feature",
		"RIE_UNIMPLEMENT: Unimplemented feature",
		"RIE_LIMIT: Arbitrary program limit",
		"RIE_BUG: Probably a bug in renderer",
		RI_UNKNOWN,
		RI_UNKNOWN,
		RI_UNKNOWN,
		RI_UNKNOWN,
		RI_UNKNOWN,
		RI_UNKNOWN,
		RI_UNKNOWN,
		RI_UNKNOWN,
		"RIE_NOTSTARTED: RiBegin not called",
		"RIE_NESTING: Bad begin-end nesting",
		"RIE_NOTOPTIONS: Invalid state for options",
		"RIE_NOTATTRIBS: Invalid state for attributes",
		"RIE_NOTPRIMS: Invalid state for primitives",
		"RIE_ILLSTATE: Other invalid state",
		"RIE_BADMOTION: Badly formed motion block",
		"RIE_BADSOLID: Badly formed solid block",
		RI_UNKNOWN,
		RI_UNKNOWN,
		RI_UNKNOWN,
		RI_UNKNOWN,
		RI_UNKNOWN,
		RI_UNKNOWN,
		RI_UNKNOWN,
		RI_UNKNOWN,
		RI_UNKNOWN,
		RI_UNKNOWN,
		"RIE_BADTOKEN: Invalid token for request",
		"RIE_RANGE: Parameter out of range",
		"RIE_CONSISTENCY: Parameters inconsistent",
		"RIE_BADHANDLE: Bad object, light or context handle",
		"RIE_NOSHADER: Can't load requested shader",
		"RIE_MISSINGDATA: Required parameters not provided",
		"RIE_SYNTAX: Declare type syntax error",
		RI_UNKNOWN,
		RI_UNKNOWN,
		RI_UNKNOWN,
		RI_UNKNOWN,
		RI_UNKNOWN,
		RI_UNKNOWN,
		RI_UNKNOWN,
		RI_UNKNOWN,
		RI_UNKNOWN,
		RI_UNKNOWN,
		RI_UNKNOWN,
		RI_UNKNOWN,
		RI_UNKNOWN,
		"RIE_MATH: Zerodivide, noninvert matrix, etc.",
		RI_NULL
	};

	if ( aCode < 0 || aCode > RIE_LASTERROR )
		return RI_UNKNOWN;
	return riErrorMessages[(int)aCode];
}

RtString CRiCPPErrMsg::errorSeverity(RtInt aSeverity)
{
	RtString riErrorSeverity[] = {
		"RIE_INFO: Info",
		"RIE_WARNING: Warning",
		"RIE_ERROR: Error",
		"RIE_SEVERE: Severe",
		RI_NULL
	};
	if ( aSeverity < 0 || aSeverity > RIE_LASTSEVERITY )
		return RI_UNKNOWN;
	return riErrorSeverity[(int)aSeverity];
}
