#ifndef _RICPP_RIBASE_RICPPDECLS_H
#define _RICPP_RIBASE_RICPPDECLS_H

/*
// RICPP - RenderMan(R) Interface CPP Language Binding
//
//     RenderMan(R) is a registered trademark of Pixar
// The RenderMan(R) Interface Procedures and Protocol are:	/** @brief Defines a new display channel for display()
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
*/

/** @file ricppdecls.h
 *  @author Andreas Pidde (andreas@pidde.de)
 *  @brief RenderMan(R) interface declarations of RiLastError and functions that can be used as parameter
 */

#ifndef _RICPP_RIBASE_RICPPTYPES_H
#include "ricpp/ribase/ricpptypes.h"
#endif /* _RICPP_RIBASE_RICPPTYPES_H */

#ifdef __cplusplus
extern "C" {
#endif

#ifdef __cplusplus
/* namespace RiCPP { /* */
#endif

RICPP_EXTERN(RtInt) RiLastError;

RICPP_EXTERN(RtFloat)
    RiGaussianFilter (RtFloat x, RtFloat y, RtFloat xwidth, RtFloat ywidth),
    RiBoxFilter (RtFloat x, RtFloat y, RtFloat xwidth, RtFloat ywidth),
    RiTriangleFilter (RtFloat x, RtFloat y, RtFloat xwidth, RtFloat ywidth),
    RiCatmullRomFilter (RtFloat x, RtFloat y, RtFloat xwidth, RtFloat ywidth),
    RiSincFilter (RtFloat x, RtFloat y, RtFloat xwidth, RtFloat ywidth);

RICPP_EXTERN(RtVoid) 
	RiProcDelayedReadArchive (RtPointer data, RtFloat detail),
    RiProcRunProgram (RtPointer data, RtFloat detail),
    RiProcDynamicLoad (RtPointer data, RtFloat detail),
	RiProcFree(RtPointer);

RICPP_EXTERN(RtVoid)
    RiErrorIgnore (RtInt code, RtInt severity, RtString message),
    RiErrorPrint (RtInt code, RtInt severity, RtString message),
    RiErrorAbort (RtInt code, RtInt severity, RtString message);

// Added functions for convenience with gnu3DKit
	
RICPP_EXTERN(RtVoid)
	RiCPPMult3DPoint (const RtPoint point, const RtMatrix matrix, RtPoint result),
	RiCPPMult3DPoints (const RtPoint *points, RtInt count, const RtMatrix matrix, RtPoint *result),
	RiCPPIntersectLinePlane (const RtPoint *line, const RtPoint pnorm, const RtPoint ppoint,
							 RtPoint *psect),
	RiCPPMultiplyMatrix (const RtMatrix pre, const RtMatrix post, RtMatrix result);

RICPP_EXTERN(RtFloat)
	RiCPPDeterminant(RtMatrix matrix);
RICPP_EXTERN(RtBoolean)
	RiCPPInvertMatrix(const RtMatrix matrix, RtMatrix inverse);
	
	
#ifdef __cplusplus
} /* extern "C" */
#endif

#ifdef __cplusplus
/* } namespace RiCPP */
#endif

#endif /* _RICPP_RIBASE_RICPPDECLS_H */
