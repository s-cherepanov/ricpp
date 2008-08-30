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

/** @file baserenderer.cpp
 *  @author Andreas Pidde (andreas@pidde.de)
 *  @brief Implementation of the backend base class of a renderer context.
 *  @todo Orientation, basis for motion blur and DOF
 */

#include "ricpp/baserenderer/baserenderer.h"

#ifndef _RICPP_RICONTEXT_RIMACRO_H
#include "ricpp/ricontext/rimacro.h"
#endif // _RICPP_RICONTEXT_RIMACRO_H

#ifndef _RICPP_RIBPARSER_RIBPARSER_H
#include "ricpp/ribparser/ribparser.h"
#endif // _RICPP_RIBPARSER_RIBPARSER_H

#ifdef _DEBUG
#define _TRACE_ARCHIVE
#define _TRACE
#ifndef _RICPP_TOOLS_TRACE_H
#include "ricpp/tools/trace.h"
#endif // _RICPP_TOOLS_TRACE_H
#endif

using namespace RiCPP;

#define RICPP_PREAMBLE(AREQ) \
	EnumRequests req = AREQ; \
	try { \
		if ( !preCheck(req) ) \
			return;

#define RICPP_PREAMBLE_RET(AREQ, ERR_RETVAL) \
	EnumRequests req = AREQ; \
	try { \
		if ( !preCheck(req) ) \
			return ERR_RETVAL;
			
#define RICPP_PROCESS(AREQ) \
{ \
	CRManInterfaceCall *r = macroFactory().AREQ; \
	if ( !r ) \
		throw ExceptRiCPPError( \
			RIE_NOMEM, RIE_SEVERE, \
			renderState()->printLineNo(__LINE__), \
			renderState()->printName(__FILE__), \
			"'%s': Unable to create request object.", CRequestInfo::requestName(req)); \
	processRequest(r); \
}

#define RICPP_PROCESS_IMMEDIATE(AREQ) \
{ \
	CRManInterfaceCall *r = macroFactory().AREQ; \
	if ( !r ) \
		throw ExceptRiCPPError( \
			RIE_NOMEM, RIE_SEVERE, \
			renderState()->printLineNo(__LINE__), \
			renderState()->printName(__FILE__), \
			"'%s': Unable to create request object.", CRequestInfo::requestName(req)); \
	processRequest(r, true); \
}


#define RICPP_POSTAMBLE \
	} catch ( ExceptRiCPPError &e2 ) { \
		ricppErrHandler().handleError(e2); \
		return; \
	} catch ( std::exception &e1 ) { \
		ricppErrHandler().handleError( \
			RIE_SYSTEM, RIE_SEVERE, \
			renderState()->printLineNo(__LINE__), \
			renderState()->printName(__FILE__), \
			"Unknown error at '%s': %s", CRequestInfo::requestName(req), e1.what()); \
			return; \
	} catch ( ... ) { \
		ricppErrHandler().handleError( \
			RIE_SYSTEM, RIE_SEVERE, \
			renderState()->printLineNo(__LINE__), \
			renderState()->printName(__FILE__), \
			"Unknown error at '%s'",  CRequestInfo::requestName(req)); \
			return; \
	}
		
#define RICPP_POSTAMBLE_RET(ERR_RETVAL) \
	} catch ( ExceptRiCPPError &e2 ) { \
		ricppErrHandler().handleError(e2); \
		return ERR_RETVAL; \
	} catch ( std::exception &e1 ) { \
		ricppErrHandler().handleError( \
			RIE_SYSTEM, RIE_SEVERE, \
			renderState()->printLineNo(__LINE__), \
			renderState()->printName(__FILE__), \
			"Unknown error at '%s': %s", CRequestInfo::requestName(req), e1.what()); \
			return ERR_RETVAL; \
	} catch ( ... ) { \
		ricppErrHandler().handleError( \
			RIE_SYSTEM, RIE_SEVERE, \
			renderState()->printLineNo(__LINE__), \
			renderState()->printName(__FILE__), \
			"Unknown error at '%s'",  CRequestInfo::requestName(req)); \
			return ERR_RETVAL; \
	}

#define RICPP_UNREC_TOKENS(NPARAMSSAV) { \
\
	if ( n != static_cast<RtInt>(NPARAMSSAV) ) { \
		ricppErrHandler().handleError( \
			RIE_BADTOKEN, RIE_WARNING, \
			renderState()->printLineNo(__LINE__), \
			renderState()->printName(__FILE__), \
			"Unrecognized tokens in '%s'",  CRequestInfo::requestName(req)); \
	} \
}

void CBaseRenderer::teapot()
{
	// Newell (Utah) Teapot
	RtToken ri_p[1] = { RI_P };
	RtPointer patch[1];
	
	attributeBegin();
	
    basis(RiBezierBasis, RI_BEZIERSTEP, RiBezierBasis, RI_BEZIERSTEP);
	
	RtFloat patch_1[] = {
		1.5F,       0.0F,      0.0F,      1.5F,       0.828427F, 0.0F,      0.828427F,  1.5F,      0.0F,      0.0F,       1.5F,      0.0F,
		-0.828427F,  1.5F,      0.0F,     -1.5F,       0.828427F, 0.0F,     -1.5F,       0.0F,      0.0F,     -1.5F,      -0.828427F, 0.0F,
		-0.828427F, -1.5F,      0.0F,      0.0F,      -1.5F,      0.0F,      0.828427F, -1.5F,      0.0F,      1.5F,      -0.828427F, 0.0F,
		1.5F,       0.0F,      0.0F,      1.5F,       0.0F,      0.075F,    1.5F,       0.828427F, 0.075F,    0.828427F,  1.5F,      0.075F,
		
		0.0F,       1.5F,      0.075F,   -0.828427F,  1.5F,      0.075F,   -1.5F,       0.828427F, 0.075F,   -1.5F,       0.0F,      0.075F,
		-1.5F,      -0.828427F, 0.075F,   -0.828427F, -1.5F,      0.075F,    0.0F,      -1.5F,      0.075F,    0.828427F, -1.5F,      0.075F,
		1.5F,      -0.828427F, 0.075F,    1.5F,       0.0F,      0.075F,    2.0F,       0.0F,      0.3F,      2.0F,       1.10457F,  0.3F,
		1.10457F,   2.0F,      0.3F,      0.0F,       2.0F,      0.3F,     -1.10457F,   2.0F,      0.3F,     -2.0F,       1.10457F,  0.3F,
		
		-2.0F,       0.0F,      0.3F,     -2.0F,      -1.10457F,  0.3F,     -1.10457F,  -2.0F,      0.3F,      0.0F,      -2.0F,      0.3F,
		1.10457F,  -2.0F,      0.3F,      2.0F,      -1.10457F,  0.3F,      2.0F,       0.0F,      0.3F,      2.0F,       0.0F,      0.75F,
		2.0F,       1.10457F,  0.75F,     1.10457F,   2.0F,      0.75F,     0.0F,       2.0F,      0.75F,    -1.10457F,   2.0F,      0.75F,
		-2.0F,       1.10457F,  0.75F,    -2.0F,       0.0F,      0.75F,    -2.0F,      -1.10457F,  0.75F,    -1.10457F,  -2.0F,      0.75F,
		
		0.0F,      -2.0F,      0.75F,     1.10457F,  -2.0F,      0.75F,     2.0F,      -1.10457F,  0.75F,     2.0F,       0.0F,      0.75F,
		2.0F,       0.0F,      1.2F,      2.0F,       1.10457F,  1.2F,      1.10457F,   2.0F,      1.2F,      0.0F,       2.0F,      1.2F,
		-1.10457F,   2.0F,      1.2F,     -2.0F,       1.10457F,  1.2F,     -2.0F,       0.0F,      1.2F,     -2.0F,      -1.10457F,  1.2F,
		-1.10457F,  -2.0F,      1.2F,      0.0F,      -2.0F,      1.2F,      1.10457F,  -2.0F,      1.2F,      2.0F,      -1.10457F,  1.2F,
		
		2.0F,       0.0F,      1.2F,      1.75F,      0.0F,      1.725F,    1.75F,      0.966498F, 1.725F,    0.966498F,  1.75F,     1.725F,
		0.0F,       1.75F,     1.725F,   -0.966498F,  1.75F,     1.725F,   -1.75F,      0.966498F, 1.725F,   -1.75F,      0.0F,      1.725F,
		-1.75F,     -0.966498F, 1.725F,   -0.966498F, -1.75F,     1.725F,    0.0F,      -1.75F,     1.725F,    0.966498F, -1.75F,     1.725F,
		1.75F,     -0.966498F, 1.725F,    1.75F,      0.0F,      1.725F,    1.5F,       0.0F,      2.25F,     1.5F,       0.828427F, 2.25F,
		
		0.828427F,  1.5F,      2.25F,     0.0F,       1.5F,      2.25F,    -0.828427F,  1.5F,      2.25F,    -1.5F,       0.828427F, 2.25F,
		-1.5F,       0.0F,      2.25F,    -1.5F,      -0.828427F, 2.25F,    -0.828427F, -1.5F,      2.25F,     0.0F,      -1.5F,      2.25F,
		0.828427F, -1.5F,      2.25F,     1.5F,      -0.828427F, 2.25F,     1.5F,       0.0F,      2.25F,     1.4375F,    0.0F,      2.38125F,
		1.4375F,    0.793909F, 2.38125F,  0.793909F,  1.4375F,   2.38125F,  0.0F,       1.4375F,   2.38125F, -0.793909F,  1.4375F,   2.38125F,
		
		-1.4375F,    0.793909F, 2.38125F, -1.4375F,    0.0F,      2.38125F, -1.4375F,   -0.793909F, 2.38125F, -0.793909F, -1.4375F,   2.38125F,
		0.0F,      -1.4375F,   2.38125F,  0.793909F, -1.4375F,   2.38125F,  1.4375F,   -0.793909F, 2.38125F,  1.4375F,    0.0F,      2.38125F,
		1.3375F,    0.0F,      2.38125F,  1.3375F,    0.738681F, 2.38125F,  0.738681F,  1.3375F,   2.38125F,  0.0F,       1.3375F,   2.38125F,
		-0.738681F,  1.3375F,   2.38125F, -1.3375F,    0.738681F, 2.38125F, -1.3375F,    0.0F,      2.38125F, -1.3375F,   -0.738681F, 2.38125F,
		
		-0.738681F, -1.3375F,   2.38125F,  0.0F,      -1.3375F,   2.38125F,  0.738681F, -1.3375F,   2.38125F,  1.3375F,   -0.738681F, 2.38125F,
		1.3375F,    0.0F,      2.38125F,  1.4F,       0.0F,      2.25F,     1.4F,       0.773198F, 2.25F,     0.773198F,  1.4F,      2.25F,
		0.0F,       1.4F,      2.25F,    -0.773198F,  1.4F,      2.25F,    -1.4F,       0.773198F, 2.25F,    -1.4F,       0.0F,      2.25F,
		-1.4F,      -0.773198F, 2.25F,    -0.773198F, -1.4F,      2.25F,     0.0F,      -1.4F,      2.25F,     0.773198F, -1.4F,      2.25F,
		
		1.4F,      -0.773198F, 2.25F,     1.4F,       0.0F,      2.25F};
	patch[0] = (RtPointer)&patch_1[0];
	patchMeshV(RI_BICUBIC, 13, RI_NONPERIODIC, 10, RI_NONPERIODIC, 1, ri_p, patch); 
	
	RtFloat patch_2[] = {
		1.3F, 0.0F, 2.25F, 1.3F, 0.71797F, 2.25F, 0.71797F, 1.3F, 2.25F, 0.0F, 1.3F, 2.25F,
		-0.71797F, 1.3F, 2.25F, -1.3F, 0.71797F, 2.25F, -1.3F, 0.0F, 2.25F, -1.3F, -0.71797F, 2.25F,
		-0.71797F, -1.3F, 2.25F, 0.0F, -1.3F, 2.25F, 0.71797F, -1.3F, 2.25F, 1.3F, -0.71797F, 2.25F,
		1.3F, 0.0F, 2.25F, 1.3F, 0.0F, 2.4F, 1.3F, 0.71797F, 2.4F, 0.71797F, 1.3F, 2.4F,
		0.0F, 1.3F, 2.4F, -0.71797F, 1.3F, 2.4F, -1.3F, 0.71797F, 2.4F, -1.3F, 0.0F, 2.4F,
		-1.3F, -0.71797F, 2.4F, -0.71797F, -1.3F, 2.4F, 0.0F, -1.3F, 2.4F, 0.71797F, -1.3F, 2.4F,
		1.3F, -0.71797F, 2.4F, 1.3F, 0.0F, 2.4F, 0.4F, 0.0F, 2.4F, 0.4F, 0.220914F, 2.4F,
		0.220914F, 0.4F, 2.4F, 0.0F, 0.4F, 2.4F, -0.220914F, 0.4F, 2.4F, -0.4F, 0.220914F, 2.4F,
		-0.4F, 0.0F, 2.4F, -0.4F, -0.220914F, 2.4F, -0.220914F, -0.4F, 2.4F, 0.0F, -0.4F, 2.4F,
		0.220914F, -0.4F, 2.4F, 0.4F, -0.220914F, 2.4F, 0.4F, 0.0F, 2.4F, 0.2F, 0.0F, 2.55F,
		0.2F, 0.110457F, 2.55F, 0.110457F, 0.2F, 2.55F, 0.0F, 0.2F, 2.55F, -0.110457F, 0.2F, 2.55F,
		-0.2F, 0.110457F, 2.55F, -0.2F, 0.0F, 2.55F, -0.2F, -0.110457F, 2.55F, -0.110457F, -0.2F, 2.55F,
		0.0F, -0.2F, 2.55F, 0.110457F, -0.2F, 2.55F, 0.2F, -0.110457F, 2.55F, 0.2F, 0.0F, 2.55F,
		0.0F, 0.0F, 2.7F, 0.0F, 0.0F, 2.7F, 0.0F, 0.0F, 2.7F, 0.0F, 0.0F, 2.7F,
		0.0F, 0.0F, 2.7F, 0.0F, 0.0F, 2.7F, 0.0F, 0.0F, 2.7F, 0.0F, 0.0F, 2.7F,
		0.0F, 0.0F, 2.7F, 0.0F, 0.0F, 2.7F, 0.0F, 0.0F, 2.7F, 0.0F, 0.0F, 2.7F,
		0.0F, 0.0F, 2.7F, 0.8F, 0.0F, 3.0F, 0.8F, 0.441828F, 3.0F, 0.441828F, 0.8F, 3.0F,
		0.0F, 0.8F, 3.0F, -0.441828F, 0.8F, 3.0F, -0.8F, 0.441828F, 3.0F, -0.8F, 0.0F, 3.0F,
		-0.8F, -0.441828F, 3.0F, -0.441828F, -0.8F, 3.0F, 0.0F, -0.8F, 3.0F, 0.441828F, -0.8F, 3.0F,
		0.8F, -0.441828F, 3.0F, 0.8F, 0.0F, 3.0F, 0.0F, 0.0F, 3.0F, 0.0F, 0.0F, 3.0F,
		0.0F, 0.0F, 3.0F, 0.0F, 0.0F, 3.0F, 0.0F, 0.0F, 3.0F, 0.0F, 0.0F, 3.0F,
		0.0F, 0.0F, 3.0F, 0.0F, 0.0F, 3.0F, 0.0F, 0.0F, 3.0F, 0.0F, 0.0F, 3.0F,
		0.0F, 0.0F, 3.0F, 0.0F, 0.0F, 3.0F, 0.0F, 0.0F, 3.0F
	};
	patch[0] = (RtPointer)&patch_2[0];
	patchMeshV(RI_BICUBIC, 13, RI_NONPERIODIC,  7, RI_NONPERIODIC, 1, ri_p, patch); 
	
	RtFloat patch_3[] = {
		-2.0F, 0.0F, 0.75F, -2.0F, 0.3F, 0.75F, -1.9F, 0.3F, 0.45F, -1.9F, 0.0F, 0.45F,
		-2.5F, 0.0F, 0.975F, -2.5F, 0.3F, 0.975F, -2.65F, 0.3F, 0.7875F, -2.65F, 0.0F, 0.7875F,
		-2.7F, 0.0F, 1.425F, -2.7F, 0.3F, 1.425F, -3.0F, 0.3F, 1.2F, -3.0F, 0.0F, 1.2F,
		-2.7F, 0.0F, 1.65F, -2.7F, 0.3F, 1.65F, -3.0F, 0.3F, 1.65F, -3.0F, 0.0F, 1.65F,
		-2.7F, 0.0F, 1.875F, -2.7F, 0.3F, 1.875F, -3.0F, 0.3F, 2.1F, -3.0F, 0.0F, 2.1F,
		-2.3F, 0.0F, 1.875F, -2.3F, 0.3F, 1.875F, -2.5F, 0.3F, 2.1F, -2.5F, 0.0F, 2.1F,
		-1.6F, 0.0F, 1.875F, -1.6F, 0.3F, 1.875F, -1.5F, 0.3F, 2.1F, -1.5F, 0.0F, 2.1F
	};
	patch[0] = (RtPointer)&patch_3[0];
	patchMeshV(RI_BICUBIC,  4, RI_NONPERIODIC,  7, RI_NONPERIODIC, 1, ri_p, patch); 
	
	RtFloat patch_4[] = {
		2.8F, 0.0F, 2.25F, 2.8F, 0.15F, 2.25F, 3.2F, 0.15F, 2.25F, 3.2F, 0.0F, 2.25F,
		2.9F, 0.0F, 2.325F, 2.9F, 0.25F, 2.325F, 3.45F, 0.15F, 2.3625F, 3.45F, 0.0F, 2.3625F,
		2.8F, 0.0F, 2.325F, 2.8F, 0.25F, 2.325F, 3.525F, 0.25F, 2.34375F, 3.525F, 0.0F, 2.34375F,
		2.7F, 0.0F, 2.25F, 2.7F, 0.25F, 2.25F, 3.3F, 0.25F, 2.25F, 3.3F, 0.0F, 2.25F,
		2.3F, 0.0F, 1.95F, 2.3F, 0.25F, 1.95F, 2.4F, 0.25F, 1.875F, 2.4F, 0.0F, 1.875F,
		2.6F, 0.0F, 1.275F, 2.6F, 0.66F, 1.275F, 3.1F, 0.66F, 0.675F, 3.1F, 0.0F, 0.675F,
		1.7F, 0.0F, 1.275F, 1.7F, 0.66F, 1.275F, 1.7F, 0.66F, 0.45F, 1.7F, 0.0F, 0.45F
	};
	patch[0] = (RtPointer)&patch_4[0];
	patchMeshV(RI_BICUBIC,  4, RI_NONPERIODIC,  7, RI_NONPERIODIC, 1, ri_p, patch); 
	
	RtFloat patch_5[] = {
		-1.9F, 0.0F, 0.45F, -1.9F, -0.3F, 0.45F, -2.0F, -0.3F, 0.75F, -2.0F, 0.0F, 0.75F,
		-2.65F, 0.0F, 0.7875F, -2.65F, -0.3F, 0.7875F, -2.5F, -0.3F, 0.975F, -2.5F, 0.0F, 0.975F,
		-3.0F, 0.0F, 1.2F, -3.0F, -0.3F, 1.2F, -2.7F, -0.3F, 1.425F, -2.7F, 0.0F, 1.425F,
		-3.0F, 0.0F, 1.65F, -3.0F, -0.3F, 1.65F, -2.7F, -0.3F, 1.65F, -2.7F, 0.0F, 1.65F,
		-3.0F, 0.0F, 2.1F, -3.0F, -0.3F, 2.1F, -2.7F, -0.3F, 1.875F, -2.7F, 0.0F, 1.875F,
		-2.5F, 0.0F, 2.1F, -2.5F, -0.3F, 2.1F, -2.3F, -0.3F, 1.875F, -2.3F, 0.0F, 1.875F,
		-1.5F, 0.0F, 2.1F, -1.5F, -0.3F, 2.1F, -1.6F, -0.3F, 1.875F, -1.6F, 0.0F, 1.875F
	};
	patch[0] = (RtPointer)&patch_5[0];
	patchMeshV(RI_BICUBIC,  4, RI_NONPERIODIC,  7, RI_NONPERIODIC, 1, ri_p, patch); 
	
	RtFloat patch_6[] = {
		3.2F, 0.0F, 2.25F, 3.2F, -0.15F, 2.25F, 2.8F, -0.15F, 2.25F, 2.8F, 0.0F, 2.25F,
		3.45F, 0.0F, 2.3625F, 3.45F, -0.15F, 2.3625F, 2.9F, -0.25F, 2.325F, 2.9F, 0.0F, 2.325F,
		3.525F, 0.0F, 2.34375F, 3.525F, -0.25F, 2.34375F, 2.8F, -0.25F, 2.325F, 2.8F, 0.0F, 2.325F,
		3.3F, 0.0F, 2.25F, 3.3F, -0.25F, 2.25F, 2.7F, -0.25F, 2.25F, 2.7F, 0.0F, 2.25F,
		2.4F, 0.0F, 1.875F, 2.4F, -0.25F, 1.875F, 2.3F, -0.25F, 1.95F, 2.3F, 0.0F, 1.95F,
		3.1F, 0.0F, 0.675F, 3.1F, -0.66F, 0.675F, 2.6F, -0.66F, 1.275F, 2.6F, 0.0F, 1.275F,
		1.7F, 0.0F, 0.45F, 1.7F, -0.66F, 0.45F, 1.7F, -0.66F, 1.275F, 1.7F, 0.0F, 1.275F
	};
	patch[0] = (RtPointer)&patch_6[0];
	patchMeshV(RI_BICUBIC,  4, RI_NONPERIODIC,  7, RI_NONPERIODIC, 1, ri_p, patch);
	
	attributeEnd();
}

bool CBaseRenderer::isOpaque(const std::vector<RtFloat> &opacity) const
{
	for ( std::vector<RtFloat>::const_iterator i = opacity.begin(); i != opacity.end(); i++ ) {
		if ( (*i) < 1.0 )
			return false;
	}
	return false;
}

RtFloat CBaseRenderer::opacityToAlpha(IndexType nSamples, const RtFloat *opacity) const
{
	if ( nSamples <= 0 )
		return (RtFloat)1.0;
	
	assert(opacity != 0);
	
	RtFloat alpha = 0;
	for ( IndexType i=0; i<nSamples; ++i) {
		alpha += (opacity[i])/nSamples;
	}

	return clamp<RtFloat>(alpha, (RtFloat)0.0, (RtFloat)1.0);
}

RtFloat CBaseRenderer::opacityToAlpha(const std::vector<RtFloat> &opacity) const 
{
	if ( opacity.size() <= 0 ) {
		return 1.0;
	}
	return opacityToAlpha(static_cast<IndexType>(opacity.size()),  &opacity[0]);
}

void CBaseRenderer::getColor3f(const CColorDescr &c, RtFloat gain, RtFloat gamma, const RtFloat *fromC, RtFloat toC[3]) const
{
	assert(fromC != 0);
	c.nToRGB(fromC, toC);
	toC[0] = pow(toC[0]*gain, (RtFloat)1.0/gamma);
	toC[1] = pow(toC[1]*gain, (RtFloat)1.0/gamma);
	toC[2] = pow(toC[2]*gain, (RtFloat)1.0/gamma);
}

void CBaseRenderer::getColor3f(const CColorDescr &c, RtFloat gain, RtFloat gamma, const std::vector<RtFloat> &fromC, RtFloat toC[3]) const
{
	if ( fromC.size() <= 0 || (RtInt)fromC.size() != c.colorSamples() ) {
		toC[0] = 0;
		toC[1] = 0;
		toC[2] = 0;
		return;
	}
	getColor3f(c, gain, gamma, &fromC[0], toC);
}

void CBaseRenderer::getCs(const CColorDescr &c, RtFloat gain, RtFloat gamma, const std::vector<RtFloat> &cs, std::vector<RtFloat> &retVal) const
{
	retVal.clear();
	IndexType size = static_cast<IndexType>(cs.size() / c.colorSamples());
	if ( size <= 0 )
		return;
	
	retVal.resize(size*3);
	
	for ( IndexType i = 0; i < size; ++i ) {
		getColor3f(c, gain, gamma, &cs[i*c.colorSamples()], &retVal[i*3]);
	}
}

void CBaseRenderer::getAlpha(const CColorDescr &c, const std::vector<RtFloat> &os, std::vector<RtFloat> &retVal) const
{
	retVal.clear();
	IndexType size = static_cast<IndexType>(os.size() / c.colorSamples());
	if ( size <= 0 )
		return;
	
	retVal.resize(size);
	
	for ( IndexType i = 0; i < size; ++i ) {
		retVal[i] = opacityToAlpha(c.colorSamples(), &os[i*c.colorSamples()]); 
	}
}

CBaseRenderer::CBaseRenderer() :
	m_renderState(0),
	m_parserCallback(0)
{
	m_renderState = 0;
	m_modeStack = 0;
	m_optionsFactory = 0;
	m_attributesFactory = 0;
	m_transformationFactory = 0;
	m_filterFuncFactory = 0;
	m_macroFactory = 0;
	m_attributesResourceFactory = 0;
	initDelayed();
}

CBaseRenderer::~CBaseRenderer()
{
	if ( m_renderState ) {
		delete m_renderState;
	}
	m_renderState = 0;

	if ( m_optionsFactory ) {
		deleteOptionsFactory(m_optionsFactory);
	}
	m_optionsFactory = 0;

	if ( m_attributesFactory ) {
		deleteAttributesFactory(m_attributesFactory);
	}
	m_attributesFactory = 0;

	if ( m_transformationFactory ) {
		deleteTransformationFactory(m_transformationFactory);
	}
	m_transformationFactory = 0;

	if ( m_filterFuncFactory ) {
		deleteFilterFuncFactory(m_filterFuncFactory);
	}
	m_filterFuncFactory = 0;

	if ( m_modeStack ) {
		deleteModeStack(m_modeStack);
	}
	m_modeStack = 0;

	if ( m_macroFactory ) {
		deleteMacroFactory(m_macroFactory);
	}
	m_macroFactory = 0;

	if ( m_attributesResourceFactory )
	{
		deleteAttributesResourceFactory(m_attributesResourceFactory);
	}
	m_attributesResourceFactory = 0;
}


RtVoid CBaseRenderer::restart(void)
{
	initDelayed();
	if ( !renderState() )
		return;
	
	defaultDeclarations();
	renderState()->restart();
}


void CBaseRenderer::hideSurface(const CSurface *s)
{
	if ( !s )
		return;
	CSurface::const_iterator iter;
	for ( iter = s->begin(); iter != s->end(); iter++ ) {
		hide(*iter);
	}
}


const CAttributes &CBaseRenderer::attributes() const
{
	if ( m_replayDelayedMode ) {
		assert (m_attributes != 0);
		if ( m_attributes != 0)
			return *m_attributes;
	}
	
	return renderState()->attributes();
}


CAttributes &CBaseRenderer::attributes()
{
	return const_cast<CAttributes &>(((const CBaseRenderer *)this)->attributes());
}


const CTransformation &CBaseRenderer::transformation() const
{
	if ( m_replayDelayedMode ) {
		assert (m_transformation != 0);
		if ( m_transformation != 0)
			return *m_transformation;
	}
	
	return renderState()->curTransform();
}


CTransformation &CBaseRenderer::transformation()
{
	return const_cast<CTransformation &>(((const CBaseRenderer *)this)->transformation());
}


bool CBaseRenderer::delayRequest(CRManInterfaceCall &obj)
{
	if ( !replayMode() ) {
		obj.deferedDeletion(true);
		renderState()->rememberState();
		m_delayedRequests.push_back(CDelayedRequest(&obj, renderState()->rememberedAttributes(), renderState()->rememberedTransformation()));
		return true;
	}
	return false;
}

void CBaseRenderer::initDelayed()
{
	m_attributes = 0;
	m_transformation = 0;
	m_replayDelayedMode = false;
	m_delayedRequests.clear();
}

void CBaseRenderer::replayDelayed()
{
	m_replayDelayedMode = true;
	
	std::list<CDelayedRequest>::iterator i = m_delayedRequests.begin();
	for ( ; i != m_delayedRequests.end(); ++i ) {
		if ( (*i).request() ) {
			m_attributes = (*i).attributes();
			m_transformation = (*i).transformation();
			try {
				
				(*i).request()->doProcess(*this, 0);

			} catch ( ExceptRiCPPError &e2 ) {
				ricppErrHandler().handleError(e2);
			} catch ( std::exception &e1 ) {
				ricppErrHandler().handleError(RIE_SYSTEM, RIE_SEVERE, renderState()->printLineNo(__LINE__), renderState()->printName(__FILE__), "Error at 'replayDelayed(%s)': %s", noNullStr((*i).request()->requestName()), e1.what());
			} catch ( ... ) {
				ricppErrHandler().handleError(RIE_SYSTEM, RIE_SEVERE, renderState()->printLineNo(__LINE__), renderState()->printName(__FILE__), "Unknown error at 'replayDelayed(%s)'", noNullStr((*i).request()->requestName()));
			}
		}
	}
	
	m_attributes = 0;
	m_transformation = 0;
	m_replayDelayedMode = false;
	m_delayedRequests.clear();
}


void CBaseRenderer::clearDelayed()
{	
	initDelayed();
}

CMatrix3D CBaseRenderer::toCamera() const
{
	CMatrix3D m;
	
	assert(renderState()->worldToCamera() != 0);
	if ( renderState()->worldToCamera() )
		m.concatTransform(renderState()->worldToCamera()->getCTM());

	m.concatTransform(transformation().getCTM());

	return m;
}

CMatrix3D CBaseRenderer::toScreen() const
{
	CMatrix3D m;
	
	assert(renderState()->cameraToScreen() != 0);
	if ( renderState()->cameraToScreen() )
		m.concatTransform(renderState()->cameraToScreen()->getCTM());

	/** @todo Need to cache world to raster CTM in renderState() (fixed in world block)!
	 */
	assert(renderState()->worldToCamera() != 0);
	if ( renderState()->worldToCamera() )
		m.concatTransform(renderState()->worldToCamera()->getCTM());
	
	m.concatTransform(transformation().getCTM());
	return m;
}

CMatrix3D CBaseRenderer::toNDC() const
{
	CMatrix3D m;
	
	/** @todo Need to cache world to raster CTM in renderState() (fixed in world block)!
	 */	
	assert(renderState()->screenToNDC() != 0);
	if ( renderState()->screenToNDC() )
		m.concatTransform(renderState()->screenToNDC()->getCTM());

	assert(renderState()->cameraToScreen() != 0);
	if ( renderState()->cameraToScreen() )
		m.concatTransform(renderState()->cameraToScreen()->getCTM());
	
	assert(renderState()->worldToCamera() != 0);
	if ( renderState()->worldToCamera() )
		m.concatTransform(renderState()->worldToCamera()->getCTM());	

	m.concatTransform(transformation().getCTM());
	return m;
}

CMatrix3D CBaseRenderer::toRaster() const
{
	CMatrix3D m;
	
	/** @todo Need to cache world to raster CTM in renderState() (fixed in world block)!
	 */

	assert(renderState()->NDCToRaster() != 0);
	if ( renderState()->NDCToRaster() )
		m.concatTransform(renderState()->NDCToRaster()->getCTM());
	
	assert(renderState()->screenToNDC() != 0);
	if ( renderState()->screenToNDC() )
		m.concatTransform(renderState()->screenToNDC()->getCTM());

	assert(renderState()->cameraToScreen() != 0);
	if ( renderState()->cameraToScreen() )
		m.concatTransform(renderState()->cameraToScreen()->getCTM());

	assert(renderState()->worldToCamera() != 0);
	if ( renderState()->worldToCamera() )
		m.concatTransform(renderState()->worldToCamera()->getCTM());

	m.concatTransform(transformation().getCTM());

	return m;
}

CRManInterfaceFactory *CBaseRenderer::getNewMacroFactory()
{
	return new CRManInterfaceFactory;
}
void CBaseRenderer::deleteMacroFactory(CRManInterfaceFactory *ptr)
{
	if ( ptr )
		delete ptr;
}
CRManInterfaceFactory &CBaseRenderer::macroFactory()
{
	return *m_macroFactory;
}


void CBaseRenderer::registerRibParserCallback(IRibParserCallback &cb)
{
	m_parserCallback = &cb;
}


void CBaseRenderer::registerResources()
{
	if ( !m_attributesResourceFactory ) {
		m_attributesResourceFactory = getNewAttributesResourceFactory();
		if ( !m_attributesResourceFactory ) {
			throw ExceptRiCPPError(RIE_NOMEM, RIE_SEVERE, __LINE__, __FILE__, "Cannot create a resource factory for attribue resources.");
		}
	}

	renderState()->registerResourceFactory(m_attributesResourceFactory);
}


CAttributesResourceFactory *CBaseRenderer::getNewAttributesResourceFactory()
{
	return new CAttributesResourceFactory;
}


void CBaseRenderer::initRenderState()
// throw ExceptRiCPPError
{
	if ( m_renderState != 0 ) {
		throw ExceptRiCPPError(RIE_BUG, RIE_SEVERE, __LINE__, __FILE__, "Render state already initialized in initRenderState()");
	}

	if ( !m_modeStack ) {
		m_modeStack = getNewModeStack();

		if ( !m_modeStack ) {
			throw ExceptRiCPPError(RIE_NOMEM, RIE_SEVERE, __LINE__, __FILE__, "Cannot create a mode stack");
		}
	}

	if ( !m_optionsFactory ) {
			m_optionsFactory = getNewOptionsFactory();

		if ( !m_optionsFactory ) {
			throw ExceptRiCPPError(RIE_NOMEM, RIE_SEVERE, __LINE__, __FILE__, "Cannot create an options factory");
		}
	}

	if ( !m_attributesFactory ) {
			m_attributesFactory = getNewAttributesFactory();

		if ( !m_attributesFactory ) {
			throw ExceptRiCPPError(RIE_NOMEM, RIE_SEVERE, __LINE__, __FILE__, "Cannot create an attributes factory");
		}
	}

	if ( !m_transformationFactory ) {
			m_transformationFactory = getNewTransformationFactory();

		if ( !m_transformationFactory ) {
			throw ExceptRiCPPError(RIE_NOMEM, RIE_SEVERE, __LINE__, __FILE__, "Cannot create a transformation factory");
		}
	}

	if ( !m_filterFuncFactory ) {
		m_filterFuncFactory = getNewFilterFuncFactory();

		if ( !m_filterFuncFactory ) {
			throw ExceptRiCPPError(RIE_NOMEM, RIE_SEVERE, __LINE__, __FILE__, "Cannot create an pixel filter factory");
		}
	}

	if ( !m_macroFactory ) {
		m_macroFactory = getNewMacroFactory();

		if ( !m_macroFactory ) {
			throw ExceptRiCPPError(RIE_NOMEM, RIE_SEVERE, __LINE__, __FILE__, "Cannot create an macro factory");
		}
	}

	m_renderState = new CRenderState(*m_modeStack, *m_optionsFactory, *m_attributesFactory, *m_transformationFactory, *m_filterFuncFactory, *m_macroFactory);
	if ( !m_renderState ) {
		throw ExceptRiCPPError(RIE_NOMEM, RIE_SEVERE, __LINE__, __FILE__, "Cannot create a render state");
	}

	registerResources();
}


bool CBaseRenderer::preCheck(EnumRequests req)
{
	if ( !renderState() ) {
		throw ExceptRiCPPError(RIE_BUG, RIE_SEVERE, renderState()->printLineNo(__LINE__), renderState()->printName(__FILE__), "State not initialized in request %s", CRequestInfo::requestName(req));
	}

	if ( !renderState()->validRequest(req) ) {
		throw ExceptRiCPPError(RIE_ILLSTATE, RIE_ERROR, renderState()->printLineNo(__LINE__), renderState()->printName(__FILE__), "request %s not in valid block", CRequestInfo::requestName(req));
	}

	if ( !renderState()->hasOptions() ) {
		throw ExceptRiCPPError(RIE_BUG, RIE_SEVERE, renderState()->printLineNo(__LINE__), renderState()->printName(__FILE__), "%s() - options not available.", CRequestInfo::requestName(req));
	}

	if ( !renderState()->hasAttributes() ) {
		throw ExceptRiCPPError(RIE_BUG, RIE_SEVERE, renderState()->printLineNo(__LINE__), renderState()->printName(__FILE__), "%s() - attributes not available.", CRequestInfo::requestName(req));
	}

	if ( !renderState()->hasTransform() ) {
		throw ExceptRiCPPError(RIE_BUG, RIE_SEVERE, renderState()->printLineNo(__LINE__), renderState()->printName(__FILE__), "%s() - transformations not available.", CRequestInfo::requestName(req));
	}

	if ( !hasMacroFactory() ) {
		throw ExceptRiCPPError(RIE_BUG, RIE_SEVERE, renderState()->printLineNo(__LINE__), renderState()->printName(__FILE__), "%s() - macro factory not available.", CRequestInfo::requestName(req));
	}

	return !renderState()->reject();
}


void CBaseRenderer::defaultDeclarations()
{
	renderState()->defaultDeclarations();
}

void CBaseRenderer::recordRequest(CRManInterfaceCall *aRequest)
{
	if ( !aRequest )
		return;
	aRequest->recorded(true);
	renderState()->curMacro()->add(aRequest);
}

void CBaseRenderer::processRequest(CRManInterfaceCall *aRequest, bool immediately)
{
	if ( !aRequest )
		return;

	ExceptRiCPPError ePre, eDo, ePost;
	
	bool recorded = false;
	
	try {
		
		aRequest->preProcess(*this, 0);
		
	} catch ( ExceptRiCPPError &e2 ) {
		ePre = e2;
	} catch ( std::exception &e1 ) {
		ePre.set(RIE_SYSTEM, RIE_SEVERE, renderState()->printLineNo(__LINE__), renderState()->printName(__FILE__), "Error at preprocessing, 'processRequest(%s)': %s", noNullStr(aRequest->requestName()), e1.what());
	} catch ( ... ) {
		ePre.set(RIE_SYSTEM, RIE_SEVERE, renderState()->printLineNo(__LINE__), renderState()->printName(__FILE__), "Unknown error at preprocessing, 'processRequest(%s)'", noNullStr(aRequest->requestName()));
	}
	
	if ( !ePre.isSevere() ) {
		
		if ( !immediately && renderState()->curMacro() ) {
			recordRequest(aRequest);
			recorded = true;
		}
			
		if ( immediately || (!renderState()->recordMode() && renderState()->executeConditionial()) ) {
			try {
				
				aRequest->doProcess(*this, 0);
				
			} catch ( ExceptRiCPPError &e2 ) {
				eDo = e2;
			} catch ( std::exception &e1 ) {
				eDo.set(RIE_SYSTEM, RIE_SEVERE, renderState()->printLineNo(__LINE__), renderState()->printName(__FILE__), "Error at do processing, 'processRequest(%s)': %s", noNullStr(aRequest->requestName()), e1.what());
			} catch ( ... ) {
				eDo.set(RIE_SYSTEM, RIE_SEVERE, renderState()->printLineNo(__LINE__), renderState()->printName(__FILE__), "Unknown error at do processing, 'processRequest(%s)'", noNullStr(aRequest->requestName()));
			}
		}

		if ( !eDo.isSevere() ) {
			try {
				
				aRequest->postProcess(*this, 0);
				
			} catch ( ExceptRiCPPError &e2 ) {
				ePost = e2;
			} catch ( std::exception &e1 ) {
				ePost.set(RIE_SYSTEM, RIE_SEVERE, renderState()->printLineNo(__LINE__), renderState()->printName(__FILE__), "Error at postprocessing, 'processRequest(%s)': %s", noNullStr(aRequest->requestName()), e1.what());
			} catch ( ... ) {
				ePost.set(RIE_SYSTEM, RIE_SEVERE, renderState()->printLineNo(__LINE__), renderState()->printName(__FILE__), "Unknown error at postprocessing, 'processRequest(%s)'", noNullStr(aRequest->requestName()));
			}
		}
	}
	
	if ( !recorded && aRequest->deferedDeletion() )
		renderState()->deferRequest(aRequest);

	if ( !recorded && !aRequest->deferedDeletion() )
		macroFactory().deleteRequest(aRequest);

	if ( ePre.code() != RIE_NOERROR )
		ricppErrHandler().handleError(ePre);
	if ( eDo.code() != RIE_NOERROR )
		ricppErrHandler().handleError(eDo);
	if ( ePost.code() != RIE_NOERROR )
		ricppErrHandler().handleError(ePost);
}


void CBaseRenderer::replayRequest(CRManInterfaceCall &aRequest, const IArchiveCallback *cb)
{
	ExceptRiCPPError ePre, eDo, ePost;

	try {
		if ( !preCheck(aRequest.interfaceIdx()) ) {
			return;
		}
	} catch ( ExceptRiCPPError &e2 ) {
		ricppErrHandler().handleError(e2);
	} catch ( std::exception &e1 ) {
		ricppErrHandler().handleError(RIE_SYSTEM, RIE_SEVERE, renderState()->printLineNo(__LINE__), renderState()->printName(__FILE__), "Error at 'replayRequest(%s)': %s", noNullStr(aRequest.requestName()), e1.what());
	} catch ( ... ) {
		ricppErrHandler().handleError(RIE_SYSTEM, RIE_SEVERE, renderState()->printLineNo(__LINE__), renderState()->printName(__FILE__), "Unknown error at 'replayRequest(%s)'", noNullStr(aRequest.requestName()));
	}

	try {
		aRequest.preProcess(*this, cb);
	} catch ( ExceptRiCPPError &e2 ) {
		ePre = e2;
	} catch ( std::exception &e1 ) {
		ePre.set(RIE_SYSTEM, RIE_SEVERE, renderState()->printLineNo(__LINE__), renderState()->printName(__FILE__), "Error at preprocessing, 'replayRequest(%s)': %s", noNullStr(aRequest.requestName()), e1.what());
	} catch ( ... ) {
		ePre.set(RIE_SYSTEM, RIE_SEVERE, renderState()->printLineNo(__LINE__), renderState()->printName(__FILE__), "Unknown error at preprocessing, 'replayRequest(%s)'", noNullStr(aRequest.requestName()));
	}
	
	if ( !ePre.isSevere() ) {
		
		if ( !renderState()->recordMode() && renderState()->executeConditionial() ) {
			try {
				aRequest.doProcess(*this, cb);
			} catch ( ExceptRiCPPError &e2 ) {
				eDo = e2;
			} catch ( std::exception &e1 ) {
				eDo.set(RIE_SYSTEM, RIE_SEVERE, renderState()->printLineNo(__LINE__), renderState()->printName(__FILE__), "Error at do processing, 'replayRequest(%s)': %s", noNullStr(aRequest.requestName()), e1.what());
			} catch ( ... ) {
				eDo.set(RIE_SYSTEM, RIE_SEVERE, renderState()->printLineNo(__LINE__), renderState()->printName(__FILE__), "Unknown error at do processing, 'replayRequest(%s)'", noNullStr(aRequest.requestName()));
			}
		}
	
		try {
			aRequest.postProcess(*this, cb);
		} catch ( ExceptRiCPPError &e2 ) {
			ePost = e2;
		} catch ( std::exception &e1 ) {
			ePost.set(RIE_SYSTEM, RIE_SEVERE, renderState()->printLineNo(__LINE__), renderState()->printName(__FILE__), "Error at postprocessing, 'replayRequest(%s)': %s", noNullStr(aRequest.requestName()), e1.what());
		} catch ( ... ) {
			ePost.set(RIE_SYSTEM, RIE_SEVERE, renderState()->printLineNo(__LINE__), renderState()->printName(__FILE__), "Unknown error at postprocessing, 'replayRequest(%s)'", noNullStr(aRequest.requestName()));
		}
	}
	
	if ( ePre.code() != RIE_NOERROR )
		ricppErrHandler().handleError(ePre);
	if ( eDo.code() != RIE_NOERROR )
		ricppErrHandler().handleError(eDo);
	if ( ePost.code() != RIE_NOERROR )
		ricppErrHandler().handleError(ePost);
}

void CBaseRenderer::processArchiveInstance(RtArchiveHandle handle, const IArchiveCallback *callback, const CParameterList &params)
{
#ifdef _TRACE_ARCHIVE
	trace("*** CBaseRenderer::processArchiveInstance()");
#endif
	CRiArchiveMacro *m = renderState()->findArchiveInstance(handle);
	if ( m ) {
		if ( m->isClosed() ) {
			CRiMacro *msav = renderState()->curReplay();
			renderState()->curReplay(m);
			renderState()->moveArchiveBegin();
			try {
				m->replay(*this, callback);
				renderState()->curReplay(msav);
			} catch(...) {
				renderState()->curReplay(msav);
				renderState()->moveArchiveEnd();
				throw;
			}
			renderState()->moveArchiveEnd();
		} else {
			throw ExceptRiCPPError(RIE_BADHANDLE, RIE_SEVERE, renderState()->printLineNo(__LINE__), renderState()->printName(__FILE__), "Archive instance %s used before its ArchiveEnd() (self inclusion, recursion doesn't work).", handle);
		}
	} else {
		throw ExceptRiCPPError(RIE_BADHANDLE, RIE_SEVERE, renderState()->printLineNo(__LINE__), renderState()->printName(__FILE__), "Archive instance %s not found.", handle);
	}
}

void CBaseRenderer::readArchiveFromStream(RtString name, IRibParserCallback &parserCallback, const IArchiveCallback *callback, const CParameterList &params)
{
#ifdef _TRACE_ARCHIVE
	trace("*** CBaseRenderer::readArchiveFromStream()");
#endif

	CParameterList p = params;
	CUri sav(renderState()->baseUri());
	std::string oldArchiveName = renderState()->archiveName();
	long oldLineNo = renderState()->lineNo();

	CRibParser parser(parserCallback, *renderState(), renderState()->baseUri());
	renderState()->moveArchiveBegin();
	try {
		std::string filename;
		if ( notEmptyStr(name) ) {
			filename = name;
			renderState()->varSubst(filename, '$');
		}
		if ( parser.canParse(name) ) {
			renderState()->baseUri() = parser.absUri();
			bool savCache = renderState()->cacheFileArchives();
			if ( savCache ) {
				renderState()->archiveFileBegin(name, macroFactory());
			}
			renderState()->archiveName(name);
			renderState()->lineNo(1);
			parser.parse(callback, params);
			if ( savCache ) {
				renderState()->archiveFileEnd();
			}
			renderState()->archiveName(oldArchiveName.c_str());
			renderState()->lineNo(oldLineNo);
			renderState()->baseUri() = sav;
			parser.close();
			renderState()->curParamList() = p;
		} else {
			throw ExceptRiCPPError(RIE_SYSTEM, RIE_ERROR,
				renderState()->printLineNo(__LINE__),
				renderState()->printName(__FILE__),
				"Cannot open archive: %s", name);
		}
	} catch (ExceptRiCPPError &e1) {
		renderState()->baseUri() = sav;
		renderState()->archiveName(oldArchiveName.c_str());
		renderState()->lineNo(oldLineNo);
		parser.close();
		renderState()->curParamList() = p;
		renderState()->moveArchiveEnd();
		throw e1;
	} catch (std::exception &e2) {
		renderState()->baseUri() = sav;
		renderState()->archiveName(oldArchiveName.c_str());
		renderState()->lineNo(oldLineNo);
		parser.close();
		renderState()->curParamList() = p;
		renderState()->moveArchiveEnd();
		throw ExceptRiCPPError(RIE_SYSTEM, RIE_SEVERE,
			renderState()->printLineNo(__LINE__),
			renderState()->printName(__FILE__),
			"While parsing name: %s", name, e2.what());
	} catch(...) {
		renderState()->baseUri() = sav;
		renderState()->archiveName(oldArchiveName.c_str());
		renderState()->lineNo(oldLineNo);
		parser.close();
		renderState()->curParamList() = p;
		renderState()->moveArchiveEnd();
		throw ExceptRiCPPError(RIE_SYSTEM, RIE_SEVERE,
			renderState()->printLineNo(__LINE__),
			renderState()->printName(__FILE__),
			"Unknown error while parsing: %s", name);
	}
	renderState()->moveArchiveEnd();
}

void CBaseRenderer::processReadArchive(RtString name, const IArchiveCallback *callback, const CParameterList &params)
{
#ifdef _TRACE_ARCHIVE
	trace("-> CBaseRenderer::processReadArchive()");
#endif
	
	if ( !emptyStr(name) ) {
		// 1. Look for archive in stored archives
#ifdef _TRACE_ARCHIVE
		trace("** call get handle");
#endif
		RtArchiveHandle handle = renderState()->storedArchiveName(name);
#ifdef _TRACE_ARCHIVE
		trace("** got handle");
#endif
		if ( handle ) {
#ifdef _TRACE_ARCHIVE
			trace("** call processArchiveInstance");
#endif
			processArchiveInstance(handle, callback, params);
#ifdef _TRACE_ARCHIVE
			trace("<- CBaseRenderer::processReadArchive(), regular read from file");
#endif
			return;
		}
	}

	if ( !m_parserCallback ) {
#ifdef _TRACE_ARCHIVE
		trace("<- CBaseRenderer::processReadArchive(), throw !m_parserCallback");
#endif
		throw ExceptRiCPPError(RIE_BUG, RIE_SEVERE,
			renderState()->printLineNo(__LINE__),
			renderState()->printName(__FILE__),
			"Parser callbacks not defined for archive: %s", name);
	}

 	// 2. Read archive from stream (name == RI_NULL for stdin)
	readArchiveFromStream(name, *m_parserCallback, callback, params);
#ifdef _TRACE_ARCHIVE
	trace("<- CBaseRenderer::processReadArchive(), regular read from stdin");
#endif
}

// ----------------------------------------------------------------------------

bool CBaseRenderer::init(const CDeclarationDictionary &theDeclDict, const COptions &theOptions, const COptionsBase &theControls)
{
	// Render state is initialized here, there is no mode so it must be not valid.
	// This init() is only called through the framework by the frontend after creation of this backend.
	if ( renderState() ) {
		ricppErrHandler().handleError(RIE_NESTING, RIE_SEVERE, "State already initialized in begin, begin called twice. That can an implementation error.");
		return false;
	}

	try {
		// Create a new state object
		initRenderState();

		// Copy standard declarations
		renderState()->dict().assignRemap(theDeclDict);

		// Copy options
		renderState()->options().assignRemap(theOptions, renderState()->dict());

		// Copy controls
		renderState()->controls().assignRemap(theControls, renderState()->dict());

		// Set renderers standard declarations
		defaultDeclarations();

		renderState()->dict().clearRemap();

		return true;
	} catch ( ExceptRiCPPError &e2 ) {
		ricppErrHandler().handleError(e2);
	} catch ( std::exception &e1 ) {
		ricppErrHandler().handleError(
			RIE_SYSTEM, RIE_SEVERE,
			__LINE__, __FILE__,
			"Unknown error at '%s': %s",  "init()", e1.what());
	} catch ( ... ) {
		ricppErrHandler().handleError(
			RIE_SYSTEM, RIE_SEVERE,
			__LINE__, __FILE__,
			"Unknown error at '%s'",  "init()");
	}
	return false;
}

// ----------------------------------------------------------------------------

RtContextHandle CBaseRenderer::beginV(RtString name, RtInt n, RtToken tokens[], RtPointer params[])
// throw ExceptRiCPPError
{
	size_t nParamsSav = n;
	RICPP_PREAMBLE_RET(REQ_BEGIN, illContextHandle)

		// Indicates that begin has been called
		renderState()->contextBegin();
	
		renderState()->parseParameters(RI_BEGIN, name, CParameterClasses(), n, tokens, params);
	    nParamsSav = renderState()->curParamList().size();
		name = renderState()->tokFindCreate(name);
		RICPP_PROCESS_IMMEDIATE(newRiBegin(renderState()->lineNo(), name, renderState()->curParamList()));
	RICPP_POSTAMBLE_RET(illContextHandle)

	RICPP_UNREC_TOKENS(nParamsSav)

	// There is no handle here, the frontend creates the backend
	return 1; // indicates success
}


RtVoid CBaseRenderer::end(void)
// throw ExceptRiCPPError
{
	// Special handling, deletes the context even if it's an error condition
	EnumRequests req = REQ_END;

	if ( !renderState() ) {
		ricppErrHandler().handleError(RIE_ILLSTATE, RIE_SEVERE, __LINE__, __FILE__, "'%s': State not initialized, break.", CRequestInfo::requestName(req));
		return;
	}

	ExceptRiCPPError err;
	if ( renderState()->curMode() != MODE_BEGIN ) {
		// Let's end cleaning anyway.
		err.set(RIE_NESTING, RIE_WARNING, __LINE__, __FILE__, "'%s': Ended context not at begin-state", CRequestInfo::requestName(req));
	}

	try {
		RICPP_PROCESS_IMMEDIATE(newRiEnd(renderState()->lineNo()));
	} catch ( ExceptRiCPPError &e2 ) {
		err = e2;
	} catch ( std::exception &e1 ) {
		err.set(RIE_SYSTEM, RIE_SEVERE, __LINE__, __FILE__, "Unknown error at '%s': %s", CRequestInfo::requestName(req), e1.what());
	} catch ( ... ) {
		err.set(RIE_SYSTEM, RIE_SEVERE, __LINE__, __FILE__, "Unknown error at '%s'", CRequestInfo::requestName(req));
	}

	try {
		renderState()->contextEnd();
	} catch ( ExceptRiCPPError &e2 ) {
		err = e2;
	} catch ( std::exception &e1 ) {
		err.set(RIE_SYSTEM, RIE_SEVERE, __LINE__, __FILE__, "Unknown error at '%s': %s", CRequestInfo::requestName(req), e1.what());
	} catch ( ... ) {
		err.set(RIE_SYSTEM, RIE_SEVERE, __LINE__, __FILE__, "Unknown error at '%s'", CRequestInfo::requestName(req));
	}
	
	// Delete the state, even if there are errors
	if ( m_renderState ) {
		delete m_renderState;
	}
	m_renderState = 0;

	if ( err.isError() ) {
		ricppErrHandler().handleError(err);
	}
}

RtVoid CBaseRenderer::errorHandler(const IErrorHandler &handler)
{
	RICPP_PREAMBLE(REQ_ERROR_HANDLER)
		RICPP_PROCESS(newRiErrorHandler(renderState()->lineNo(), handler));
	RICPP_POSTAMBLE
}

RtVoid CBaseRenderer::doProcess(CRiSynchronize &obj)
{
	if ( obj.name() == RI_RESTART ) {
		restart();
	}
}

RtVoid CBaseRenderer::synchronize(RtString name)
{
	RICPP_PREAMBLE(REQ_SYSTEM)
		name = renderState()->tokFindCreate(name);
		RICPP_PROCESS(newRiSynchronize(renderState()->lineNo(), name));
	RICPP_POSTAMBLE
}

RtVoid CBaseRenderer::system(RtString cmd)
{
	RICPP_PREAMBLE(REQ_SYSTEM)
		RICPP_PROCESS(newRiSystem(renderState()->lineNo(), cmd));
	RICPP_POSTAMBLE
}

RtToken CBaseRenderer::declare(RtToken name, RtString declaration)
{
	RtToken token = RI_NULL;
	RICPP_PREAMBLE_RET(REQ_DECLARE, RI_NULL)
		if ( emptyStr(name) ) {
			throw ExceptRiCPPError(
				RIE_MISSINGDATA,
				RIE_ERROR,
				renderState()->printLineNo(__LINE__), renderState()->printName(__FILE__),
				"name is missing in declare(\"%s\", \"%s\")", markEmptyStr(name), markEmptyStr(declaration)
			);
		}
		// Always process the declaration only once where it occurs
		token = renderState()->declare(name, declaration, false);
		// Use the original name as token (maybe with blanks)
		RICPP_PROCESS(newRiDeclare(renderState()->lineNo(), name, declaration));
	RICPP_POSTAMBLE_RET(RI_NULL)

	return token;
}


RtVoid CBaseRenderer::preProcess(CRiFrameBegin &obj)
{
	renderState()->frameBegin(obj.frameNumber());
}

RtVoid CBaseRenderer::frameBegin(RtInt number)
// throw ExceptRiCPPError
{
	RICPP_PREAMBLE(REQ_FRAME_BEGIN)
		RICPP_PROCESS(newRiFrameBegin(renderState()->lineNo(), number));
	RICPP_POSTAMBLE
}


RtVoid CBaseRenderer::preProcess(CRiFrameEnd &obj)
{
	renderState()->frameEnd();
}

RtVoid CBaseRenderer::frameEnd(void)
// throw ExceptRiCPPError
{
	RICPP_PREAMBLE(REQ_FRAME_END)
		RICPP_PROCESS(newRiFrameEnd(renderState()->lineNo()));
	RICPP_POSTAMBLE
}

RtVoid CBaseRenderer::preProcess(CRiWorldBegin &obj)
{
	renderState()->worldBegin();
}

RtVoid CBaseRenderer::worldBegin(void)
// throw ExceptRiCPPError
{
	RICPP_PREAMBLE(REQ_WORLD_BEGIN)
		RICPP_PROCESS(newRiWorldBegin(renderState()->lineNo()));
	RICPP_POSTAMBLE
}

RtVoid CBaseRenderer::preProcess(CRiWorldEnd &obj)
{
	renderState()->worldEnd();
}

RtVoid CBaseRenderer::worldEnd(void)
// throw ExceptRiCPPError
{
	RICPP_PREAMBLE(REQ_WORLD_END)
		RICPP_PROCESS(newRiWorldEnd(renderState()->lineNo()));
	    renderState()->deleteDeferedRequests();
	RICPP_POSTAMBLE
}


RtVoid CBaseRenderer::preProcess(CRiAttributeBegin &obj)
{
	renderState()->attributeBegin();
}


RtVoid CBaseRenderer::attributeBegin(void)
// throw ExceptRiCPPError
{
	RICPP_PREAMBLE(REQ_ATTRIBUTE_BEGIN)
		RICPP_PROCESS(newRiAttributeBegin(renderState()->lineNo()));
	RICPP_POSTAMBLE
}


RtVoid CBaseRenderer::preProcess(CRiAttributeEnd &obj)
{
	renderState()->attributeEnd();
}

RtVoid CBaseRenderer::attributeEnd(void)
// throw ExceptRiCPPError
{
	RICPP_PREAMBLE(REQ_ATTRIBUTE_END)
		RICPP_PROCESS(newRiAttributeEnd(renderState()->lineNo()));
	RICPP_POSTAMBLE
}


RtVoid CBaseRenderer::preProcess(CRiTransformBegin &obj)
{
	renderState()->transformBegin();
}

RtVoid CBaseRenderer::transformBegin(void)
// throw ExceptRiCPPError
{
	RICPP_PREAMBLE(REQ_TRANSFORM_BEGIN)
		RICPP_PROCESS(newRiTransformBegin(renderState()->lineNo()));
	RICPP_POSTAMBLE
}


RtVoid CBaseRenderer::preProcess(CRiTransformEnd &obj)
{
	renderState()->transformEnd();
}

RtVoid CBaseRenderer::transformEnd(void)
// throw ExceptRiCPPError
{
	RICPP_PREAMBLE(REQ_TRANSFORM_END)
		RICPP_PROCESS(newRiTransformEnd(renderState()->lineNo()));
	RICPP_POSTAMBLE
}


RtVoid CBaseRenderer::preProcess(CRiSolidBegin &obj)
{
	renderState()->solidBegin(obj.type());
}

RtVoid CBaseRenderer::solidBegin(RtToken type)
{
	RICPP_PREAMBLE(REQ_SOLID_BEGIN)

		RtToken typeTok = RI_PRIMITIVE;
		type = renderState()->tokFindCreate(type);
		if ( type == RI_INTERSECTION || type == RI_UNION || type == RI_DIFFERENCE ) {
			typeTok = type;
		}
		
		RICPP_PROCESS(newRiSolidBegin(renderState()->lineNo(), typeTok));
		
		if ( type != typeTok ) {
			throw ExceptRiCPPError(
				RIE_BADSOLID, RIE_ERROR,
				renderState()->printLineNo(__LINE__),
				renderState()->printName(__FILE__),
				"Unknown solid operation '%s' at '%s' (took RI_PRIMITIVE instead)", noNullStr(type), CRequestInfo::requestName(req));
		}

	RICPP_POSTAMBLE
}


RtVoid CBaseRenderer::preProcess(CRiSolidEnd &obj)
{
	renderState()->solidEnd();
}

RtVoid CBaseRenderer::solidEnd(void)
{
	RICPP_PREAMBLE(REQ_SOLID_END)
		RICPP_PROCESS(newRiSolidEnd(renderState()->lineNo()));
	RICPP_POSTAMBLE
}


RtObjectHandle CBaseRenderer::objectBegin(RtString name)
{
	RtObjectHandle handle = illObjectHandle;
	RICPP_PREAMBLE_RET(REQ_OBJECT_BEGIN, illObjectHandle)
		name = renderState()->tokFindCreate(name);
		handle = renderState()->objectBegin(name, macroFactory());
		RICPP_PROCESS_IMMEDIATE(newRiObjectBegin(renderState()->lineNo(), handle, name));
	RICPP_POSTAMBLE_RET(illObjectHandle)
	return handle;
}

RtVoid CBaseRenderer::objectEnd(void)
{
	RICPP_PREAMBLE(REQ_OBJECT_END)
		RICPP_PROCESS_IMMEDIATE(newRiObjectEnd(renderState()->lineNo()));
		renderState()->objectEnd();
	RICPP_POSTAMBLE
}

RtVoid CBaseRenderer::doProcess(CRiObjectInstance &obj)
{
	CRiObjectMacro *m = renderState()->objectInstance(obj.handle());
	if ( m ) {
		if ( m->isClosed() ) {
			CRiMacro *msav = renderState()->curReplay();
			renderState()->curReplay(m);
			renderState()->moveArchiveBegin();
			try {
				m->replay(*this, 0);
				renderState()->curReplay(msav);
			} catch (...) {
				renderState()->curReplay(msav);
				renderState()->moveArchiveEnd();
				throw;
			}
			renderState()->moveArchiveEnd();
		} else {
			throw ExceptRiCPPError(
				RIE_BADHANDLE, RIE_SEVERE,
				renderState()->printLineNo(__LINE__),
				renderState()->printName(__FILE__),
				"Object instance used before its doObjectInstance().");
		}
	} else {
		throw ExceptRiCPPError(
			RIE_BADHANDLE, RIE_SEVERE,
			renderState()->printLineNo(__LINE__),
			renderState()->printName(__FILE__),
			"Object instance not found.");
	}
}

RtVoid CBaseRenderer::objectInstance(RtObjectHandle handle)
{
	RICPP_PREAMBLE(REQ_OBJECT_INSTANCE)
		RICPP_PROCESS(newRiObjectInstance(renderState()->lineNo(), handle));
	RICPP_POSTAMBLE
}

RtVoid CBaseRenderer::preProcess(CRiMotionBegin &obj)
{
	/** @todo Make the motionBegin using a vector
	 */
	renderState()->motionBegin(obj.num(), &obj.times()[0]);
}

RtVoid CBaseRenderer::motionBeginV(RtInt N, RtFloat times[])
{
	if ( N < 0 || times == 0 )
		N=0;
	RICPP_PREAMBLE(REQ_MOTION_BEGIN)
		RICPP_PROCESS(newRiMotionBegin(renderState()->lineNo(), N, times));
	RICPP_POSTAMBLE

	if ( N == 0 ) {
		ricppErrHandler().handleError(
			RIE_BADTOKEN, RIE_WARNING,
			renderState()->printLineNo(__LINE__),
			renderState()->printName(__FILE__),
			"N less or equal 0 or empty times[] array in '%s'",  CRequestInfo::requestName(req));
	}
}

RtVoid CBaseRenderer::preProcess(CRiMotionEnd &obj)
{
	renderState()->motionEnd();
}

RtVoid CBaseRenderer::motionEnd(void)
{
	RICPP_PREAMBLE(REQ_MOTION_END)
		RICPP_PROCESS(newRiMotionEnd(renderState()->lineNo()));
	RICPP_POSTAMBLE
}


RtVoid CBaseRenderer::preProcess(CRiResourceBegin &obj)
{
	renderState()->resourceBegin();
}

RtVoid CBaseRenderer::resourceBegin(void)
// throw ExceptRiCPPError
{
	RICPP_PREAMBLE(REQ_RESOURCE_BEGIN)
		RICPP_PROCESS(newRiResourceBegin(renderState()->lineNo()));
	RICPP_POSTAMBLE
}


RtVoid CBaseRenderer::preProcess(CRiResourceEnd &obj)
{
	renderState()->resourceEnd();
}

RtVoid CBaseRenderer::resourceEnd(void)
// throw ExceptRiCPPError
{
	RICPP_PREAMBLE(REQ_RESOURCE_END)
		RICPP_PROCESS(newRiResourceEnd(renderState()->lineNo()));
	RICPP_POSTAMBLE
}

RtVoid CBaseRenderer::preProcess(CRiResource &obj)
{
	renderState()->resource(*this, obj.handle(), obj.type(), obj.parameters());
}

RtVoid CBaseRenderer::resourceV(RtToken handle, RtToken type, RtInt n, RtToken tokens[], RtPointer params[])
{
	size_t nParamsSav = n;
	RICPP_PREAMBLE(REQ_RESOURCE)
		handle = renderState()->tokFindCreate(handle);
		type = renderState()->tokFindCreate(type);
		renderState()->parseParameters(RI_RESOURCE, type, CParameterClasses(), n, tokens, params);
	    nParamsSav = renderState()->curParamList().size();
		RICPP_PROCESS(newRiResource(renderState()->lineNo(), handle, type, renderState()->curParamList()));
	RICPP_POSTAMBLE
	RICPP_UNREC_TOKENS(nParamsSav)
}

RtArchiveHandle CBaseRenderer::archiveBeginV(RtToken name, RtInt n, RtToken tokens[], RtPointer params[]) 
{
	size_t nParamsSav = n;
	RtArchiveHandle handle = illArchiveHandle;
	RICPP_PREAMBLE_RET(REQ_ARCHIVE_BEGIN, illArchiveHandle)
		name = renderState()->tokFindCreate(name);
		handle = renderState()->archiveBegin(name, macroFactory());
		renderState()->parseParameters(CParameterClasses(), n, tokens, params);
		nParamsSav = renderState()->curParamList().size();
		RICPP_PROCESS_IMMEDIATE(newRiArchiveBegin(renderState()->lineNo(), handle, name, renderState()->curParamList()));
	RICPP_POSTAMBLE_RET(illArchiveHandle)
	RICPP_UNREC_TOKENS(nParamsSav)
	return handle;
}

RtVoid CBaseRenderer::archiveEnd(void)
{
	RICPP_PREAMBLE(REQ_ARCHIVE_END)
		RICPP_PROCESS_IMMEDIATE(newRiArchiveEnd(renderState()->lineNo()));
		renderState()->archiveEnd();
	RICPP_POSTAMBLE
}



RtVoid CBaseRenderer::preProcess(CRiFormat &obj)
{
	renderState()->options().format(obj.xRes(), obj.yRes(), obj.aspect());
}

RtVoid CBaseRenderer::format(RtInt xres, RtInt yres, RtFloat aspect)
{
	RICPP_PREAMBLE(REQ_FORMAT)
		RICPP_PROCESS(newRiFormat(renderState()->lineNo(), xres, yres, aspect));
	RICPP_POSTAMBLE
}


RtVoid CBaseRenderer::preProcess(CRiFrameAspectRatio &obj)
{
	renderState()->options().frameAspectRatio(obj.aspect());
}

RtVoid CBaseRenderer::frameAspectRatio(RtFloat aspect)
{
	RICPP_PREAMBLE(REQ_FRAME_ASPECT_RATIO)
		if ( nearlyZero(aspect) ) {
			throw ExceptRiCPPError( \
								   RIE_MATH, RIE_ERROR, \
								   renderState()->printLineNo(__LINE__), \
								   renderState()->printName(__FILE__), \
								   "'%s': aspect should not be 0.", CRequestInfo::requestName(req)); \
		}
		RICPP_PROCESS(newRiFrameAspectRatio(renderState()->lineNo(), aspect));
	RICPP_POSTAMBLE
}


RtVoid CBaseRenderer::preProcess(CRiScreenWindow &obj)
{
	renderState()->options().screenWindow(obj.left(), obj.right(), obj.bottom(), obj.top());
}

RtVoid CBaseRenderer::screenWindow(RtFloat left, RtFloat right, RtFloat bot, RtFloat top)
{
	RICPP_PREAMBLE(REQ_SCREEN_WINDOW)
		RICPP_PROCESS(newRiScreenWindow(renderState()->lineNo(), left, right, bot, top));
	RICPP_POSTAMBLE
}


RtVoid CBaseRenderer::preProcess(CRiCropWindow &obj)
{
	renderState()->options().cropWindow(obj.xMin(), obj.xMax(), obj.yMin(), obj.yMax());
}

RtVoid CBaseRenderer::cropWindow(RtFloat xmin, RtFloat xmax, RtFloat ymin, RtFloat ymax)
{
	RICPP_PREAMBLE(REQ_CROP_WINDOW)
		if ( xmin > xmax ) std::swap(xmin, xmax);
		if ( ymin > ymax ) std::swap(ymin, ymax);
		RICPP_PROCESS(newRiCropWindow(renderState()->lineNo(), xmin, xmax, ymin, ymax));
	RICPP_POSTAMBLE
}


RtVoid CBaseRenderer::preProcess(CRiProjection &obj)
{
	// Sets the state (can throw)
	renderState()->projection(obj.name(), obj.parameters());	
	// Here Camera coord space
}


RtVoid CBaseRenderer::projectionV(RtToken name, RtInt n, RtToken tokens[], RtPointer params[])
{
	size_t nParamsSav = n;
	RICPP_PREAMBLE(REQ_PROJECTION)
		name = renderState()->tokFindCreate(name);
		renderState()->parseParameters(RI_PROJECTION, name, CParameterClasses(), n, tokens, params);
		nParamsSav = renderState()->curParamList().size();
		RICPP_PROCESS(newRiProjection(renderState()->lineNo(), name, renderState()->curParamList()));
	RICPP_POSTAMBLE
	RICPP_UNREC_TOKENS(nParamsSav)
}


RtVoid CBaseRenderer::preProcess(CRiClipping &obj)
{
	renderState()->options().clipping(obj.hither(), obj.yon());
}

RtVoid CBaseRenderer::clipping(RtFloat hither, RtFloat yon)
{
	if ( hither > yon ) {
		std::swap(hither, yon);
	}
	RICPP_PREAMBLE(REQ_CLIPPING)
		RICPP_PROCESS(newRiClipping(renderState()->lineNo(), hither, yon));
	RICPP_POSTAMBLE
}


RtVoid CBaseRenderer::preProcess(CRiClippingPlane &obj)
{
	renderState()->options().clippingPlane(obj.x(), obj.y(), obj.z(), obj.nx(), obj.ny(), obj.nz());
}

RtVoid CBaseRenderer::clippingPlane(RtFloat x, RtFloat y, RtFloat z, RtFloat nx, RtFloat ny, RtFloat nz)
{
	RICPP_PREAMBLE(REQ_CLIPPING_PLANE)
		RICPP_PROCESS(newRiClippingPlane(renderState()->lineNo(), x, y, z, nx, ny, nz));
	RICPP_POSTAMBLE
}


RtVoid CBaseRenderer::preProcess(CRiDepthOfField &obj)
{
	renderState()->options().depthOfField(obj.fStop(), obj.focalLength(), obj.focalDistance());
}

RtVoid CBaseRenderer::depthOfField(RtFloat fstop, RtFloat focallength, RtFloat focaldistance)
{
	RICPP_PREAMBLE(REQ_DEPTH_OF_FIELD)
		RICPP_PROCESS(newRiDepthOfField(renderState()->lineNo(), fstop, focallength, focaldistance));
	RICPP_POSTAMBLE
}


RtVoid CBaseRenderer::preProcess(CRiShutter &obj)
{
	renderState()->options().shutter(obj.sMin(), obj.sMax());
}

RtVoid CBaseRenderer::shutter(RtFloat smin, RtFloat smax)
{
	RICPP_PREAMBLE(REQ_SHUTTER)
		RICPP_PROCESS(newRiShutter(renderState()->lineNo(), smin, smax));
	RICPP_POSTAMBLE
}


RtVoid CBaseRenderer::preProcess(CRiPixelVariance &obj)
{
	renderState()->options().pixelVariance(obj.variation());
}

RtVoid CBaseRenderer::pixelVariance(RtFloat variation)
{
	RICPP_PREAMBLE(REQ_PIXEL_VARIANCE)
		RICPP_PROCESS(newRiPixelVariance(renderState()->lineNo(), variation));
	RICPP_POSTAMBLE
}


RtVoid CBaseRenderer::preProcess(CRiPixelSamples &obj)
{
	renderState()->options().pixelSamples(obj.xSamples(), obj.ySamples());
}

RtVoid CBaseRenderer::pixelSamples(RtFloat xsamples, RtFloat ysamples)
{
	RICPP_PREAMBLE(REQ_PIXEL_SAMPLES)
		RICPP_PROCESS(newRiPixelSamples(renderState()->lineNo(), xsamples, ysamples));
	RICPP_POSTAMBLE
}


RtVoid CBaseRenderer::preProcess(CRiPixelFilter &obj)
{
	renderState()->options().pixelFilter(obj.filterFunc(), obj.xWidth(), obj.yWidth());
}

RtVoid CBaseRenderer::pixelFilter(const IFilterFunc &function, RtFloat xwidth, RtFloat ywidth)
{
	RICPP_PREAMBLE(REQ_PIXEL_FILTER)
		RICPP_PROCESS(newRiPixelFilter(renderState()->lineNo(), function, xwidth, ywidth));
	RICPP_POSTAMBLE
}


RtVoid CBaseRenderer::preProcess(CRiExposure &obj)
{
	renderState()->options().exposure(obj.gain(), obj.gamma());
}

RtVoid CBaseRenderer::exposure(RtFloat gain, RtFloat gamma)
{
	RICPP_PREAMBLE(REQ_EXPOSURE)
		RICPP_PROCESS(newRiExposure(renderState()->lineNo(), gain, gamma));
	RICPP_POSTAMBLE
}


RtVoid CBaseRenderer::preProcess(CRiImager &obj)
{
	renderState()->options().imager(obj.name(), obj.parameters());
}

RtVoid CBaseRenderer::imagerV(RtString name, RtInt n, RtToken tokens[], RtPointer params[])
{
	size_t nParamsSav = n;
	RICPP_PREAMBLE(REQ_IMAGER)
		name = renderState()->tokFindCreate(name);
		renderState()->parseParameters(RI_IMAGER, name, CParameterClasses(), n, tokens, params);
		nParamsSav = renderState()->curParamList().size();
		RICPP_PROCESS(newRiImager(renderState()->lineNo(), name, renderState()->curParamList()));
	RICPP_POSTAMBLE
	RICPP_UNREC_TOKENS(nParamsSav)
}


RtVoid CBaseRenderer::preProcess(CRiQuantize &obj)
{
	renderState()->options().quantize(obj.type(), obj.one(), obj.qMin(), obj.qMax(), obj.ditherAmplitude());
}

RtVoid CBaseRenderer::quantize(RtToken type, RtInt one, RtInt qmin, RtInt qmax, RtFloat ampl)
{
	RICPP_PREAMBLE(REQ_QUANTIZE)
		type = renderState()->tokFindCreate(type);
		RICPP_PROCESS(newRiQuantize(renderState()->lineNo(), type, one, qmin, qmax, ampl));
	RICPP_POSTAMBLE
}


RtVoid CBaseRenderer::preProcess(CRiDisplayChannel &obj)
{
	renderState()->options().displayChannel(renderState()->dict(), renderState()->options().colorDescr(), obj.channel(), obj.parameters());
}

RtVoid CBaseRenderer::displayChannelV(RtToken channel, RtInt n, RtToken tokens[], RtPointer params[])
{
	size_t nParamsSav = n;
	RICPP_PREAMBLE(REQ_DISPLAY_CHANNEL)
		channel = renderState()->tokFindCreate(channel);
		renderState()->parseParameters(CParameterClasses(), n, tokens, params);
		nParamsSav = renderState()->curParamList().size();
		RICPP_PROCESS(newRiDisplayChannel(renderState()->lineNo(), channel, renderState()->curParamList()));
	RICPP_POSTAMBLE
	RICPP_UNREC_TOKENS(nParamsSav)
}

RtVoid CBaseRenderer::preProcess(CRiDisplay &obj)
{
	renderState()->options().display(obj.name(), obj.type(), obj.mode(), obj.parameters());
}

RtVoid CBaseRenderer::displayV(RtString name, RtToken type, RtString mode, RtInt n, RtToken tokens[], RtPointer params[])
{
	size_t nParamsSav = n;
	RICPP_PREAMBLE(REQ_DISPLAY)
		type = renderState()->tokFindCreate(type);
		mode = renderState()->tokFindCreate(mode);
		renderState()->parseParameters(RI_DISPLAY, type, CParameterClasses(), n, tokens, params);
		nParamsSav = renderState()->curParamList().size();
		RICPP_PROCESS(newRiDisplay(renderState()->lineNo(), name, type, mode, renderState()->curParamList()));
	RICPP_POSTAMBLE
	RICPP_UNREC_TOKENS(nParamsSav)
}


RtVoid CBaseRenderer::preProcess(CRiHider &obj)
{
	renderState()->options().hider(obj.type(), obj.parameters());
}

RtVoid CBaseRenderer::hiderV(RtToken type, RtInt n, RtToken tokens[], RtPointer params[])
{
	size_t nParamsSav = n;
	RICPP_PREAMBLE(REQ_HIDER)
		type = renderState()->tokFindCreate(type);
		renderState()->parseParameters(RI_HIDER, type, CParameterClasses(), n, tokens, params);
		nParamsSav = renderState()->curParamList().size();
		RICPP_PROCESS(newRiHider(renderState()->lineNo(), type, renderState()->curParamList()));
	RICPP_POSTAMBLE
	RICPP_UNREC_TOKENS(nParamsSav)
}


RtVoid CBaseRenderer::preProcess(CRiColorSamples &obj)
{
	renderState()->options().colorSamples(obj.nSamples(), obj.nRGBPtr(), obj.RGBnPtr());
}

RtVoid CBaseRenderer::colorSamples(RtInt N, RtFloat nRGB[], RtFloat RGBn[])
{
	RICPP_PREAMBLE(REQ_COLOR_SAMPLES)
		RICPP_PROCESS(newRiColorSamples(renderState()->lineNo(), N, nRGB, RGBn));
	RICPP_POSTAMBLE

	if ( N <= 0 || nRGB == 0 || RGBn == 0 ) {
		ricppErrHandler().handleError(
			RIE_RANGE, RIE_WARNING,
			"Illegal values for the parameters of colorSamples(), using identity mapping."
		);
	}
}


RtVoid CBaseRenderer::preProcess(CRiRelativeDetail &obj)
{
	renderState()->options().relativeDetail(obj.relativeDetail());
}

RtVoid CBaseRenderer::relativeDetail(RtFloat relativedetail)
{
	RICPP_PREAMBLE(REQ_RELATIVE_DETAIL)
		RICPP_PROCESS(newRiRelativeDetail(renderState()->lineNo(), relativedetail));
	RICPP_POSTAMBLE
}


RtVoid CBaseRenderer::preProcess(CRiOption &obj)
{
	renderState()->option(obj.name(), obj.parameters());
}

RtVoid CBaseRenderer::optionV(RtToken name, RtInt n, RtToken tokens[], RtPointer params[])
{
	size_t nParamsSav = n;
	RICPP_PREAMBLE(REQ_OPTION)
		name = renderState()->tokFindCreate(name);
		renderState()->parseParameters(RI_OPTION, name, CParameterClasses(), n, tokens, params);
		nParamsSav = renderState()->curParamList().size();
		RICPP_PROCESS(newRiOption(renderState()->lineNo(), name, renderState()->curParamList()));
	RICPP_POSTAMBLE
	RICPP_UNREC_TOKENS(nParamsSav)
}


RtVoid CBaseRenderer::preProcess(CRiControl &obj)
{
	renderState()->control(obj.name(), obj.parameters());
}

RtVoid CBaseRenderer::controlV(RtToken name, RtInt n, RtToken tokens[], RtPointer params[])
{
	size_t nParamsSav = n;
	RICPP_PREAMBLE(REQ_CONTROL)
		name = renderState()->tokFindCreate(name);
		renderState()->parseParameters(RI_CONTROL, name, CParameterClasses(), n, tokens, params);
		nParamsSav = renderState()->curParamList().size();
		RICPP_PROCESS_IMMEDIATE(newRiControl(renderState()->lineNo(), name, renderState()->curParamList()));
	RICPP_POSTAMBLE
	RICPP_UNREC_TOKENS(nParamsSav)
}


RtVoid CBaseRenderer::preProcess(CRiLightSource &obj)
{
	CHandle *handle = renderState()->lightSourceHandle(obj.handle());
	if ( !handle ) {
		RtLightHandle l = renderState()->renewLightHandle(obj.name(), obj.handleName(),renderState()->curParamList());
		if ( l == illLightHandle ) {
			throw ExceptRiCPPError(
				RIE_BADHANDLE,
				RIE_SEVERE,
				renderState()->printLineNo(__LINE__),
				renderState()->printName(__FILE__),
				"Handle not created for LightSource \"%s\"",
				noNullStr(obj.name()));
		}
	}
}

	
RtVoid CBaseRenderer::doProcess(CRiLightSource &obj)
{
	if ( !renderState()->newLightSource(obj.handle(), false, obj.name(), obj.parameters()) ) {
		throw ExceptRiCPPError(
			RIE_NOMEM,
			RIE_SEVERE,
			renderState()->printLineNo(__LINE__),
			renderState()->printName(__FILE__),
			"LightSource of \"%s\"",
			noNullStr(obj.name()));
	}
}

RtLightHandle CBaseRenderer::lightSourceV(RtString name, RtInt n, RtToken tokens[], RtPointer params[])
{
	size_t nParamsSav = n;
	RtLightHandle h = illLightHandle;

	RICPP_PREAMBLE_RET(REQ_LIGHT_SOURCE, illLightHandle)
		name = renderState()->tokFindCreate(name);
		renderState()->parseParameters(RI_LIGHT_SOURCE, name, CParameterClasses(), n, tokens, params);
		nParamsSav = renderState()->curParamList().size();
		h = renderState()->newLightHandle(name, renderState()->curParamList());
		RICPP_PROCESS(newRiLightSource(renderState()->lineNo(), h, name, renderState()->curParamList()));
	RICPP_POSTAMBLE_RET(illLightHandle)
	RICPP_UNREC_TOKENS(nParamsSav)

	return h;
}


RtVoid CBaseRenderer::preProcess(CRiAreaLightSource &obj)
{
	if ( obj.name() != RI_NULL ) {
		// Test the handle
		CHandle *handle = renderState()->lightSourceHandle(obj.handle());
		if ( !handle ) {
			RtLightHandle l = renderState()->renewLightHandle(obj.name(), obj.handleName(), renderState()->curParamList());
			if ( l == illLightHandle ) {
				throw ExceptRiCPPError(
					RIE_BADHANDLE,
					RIE_SEVERE,
					renderState()->printLineNo(__LINE__),
					renderState()->printName(__FILE__),
					"Handle not created for AreaLightSource \"%s\"",
					noNullStr(obj.name()));
			}
		}
	} else {
		// AreaLightSource ended
		renderState()->endAreaLightSource();
	}
}

RtVoid CBaseRenderer::doProcess(CRiAreaLightSource &obj)
{
	// name == RI_NULL : Area light source was closed
	if ( obj.name() != RI_NULL ) {
		// Create an area light source
		if ( !renderState()->newLightSource(obj.handle(), true, obj.name(), obj.parameters()) ) {
			throw ExceptRiCPPError(
				RIE_NOMEM,
				RIE_SEVERE,
				renderState()->printLineNo(__LINE__),
				renderState()->printName(__FILE__),
				"AreaLightSource of \"%s\"",
				noNullStr(obj.name()));
		}
		renderState()->startAreaLightSource(obj.handle());
	}
}
	
RtLightHandle CBaseRenderer::areaLightSourceV(RtString name, RtInt n, RtToken tokens[], RtPointer params[])
{
	size_t nParamsSav = n;
	RtLightHandle h = illLightHandle;

	RICPP_PREAMBLE_RET(REQ_AREA_LIGHT_SOURCE, illLightHandle)
		name = renderState()->tokFindCreate(name);
		renderState()->parseParameters(RI_AREA_LIGHT_SOURCE, name, CParameterClasses(), n, tokens, params);
		nParamsSav = renderState()->curParamList().size();
		h = renderState()->newLightHandle(name, renderState()->curParamList());
		RICPP_PROCESS(newRiAreaLightSource(renderState()->lineNo(), h, name, renderState()->curParamList()));
	RICPP_POSTAMBLE_RET(illLightHandle)
	RICPP_UNREC_TOKENS(nParamsSav)
	return h;
}

RtVoid CBaseRenderer::preProcess(CRiIlluminate &obj)
{
	// Only a test if LightSource was declared
	CHandle *handle = renderState()->lightSourceHandle(obj.handle());
	if ( !handle ) {
		throw ExceptRiCPPError(
			RIE_BADHANDLE,
			RIE_SEVERE,
			renderState()->printLineNo(__LINE__),
			renderState()->printName(__FILE__),
			"Illuminate a light source (Handle: %s)",
			noNullStr(obj.handle()));
	}
	CLightSource *l = renderState()->lightSourceInstance(obj.handle());
	renderState()->attributes().illuminate(l, obj.onOff());
}


RtVoid CBaseRenderer::illuminate(RtLightHandle light, RtBoolean onoff)
{
	RICPP_PREAMBLE(REQ_ILLUMINATE)
		RICPP_PROCESS(newRiIlluminate(renderState()->lineNo(), light, onoff));
	RICPP_POSTAMBLE
}


RtVoid CBaseRenderer::preProcess(CRiAttribute &obj)
{
	renderState()->attribute(obj.name(), obj.parameters());
}


RtVoid CBaseRenderer::attributeV(RtToken name, RtInt n, RtToken tokens[], RtPointer params[])
{
	size_t nParamsSav = n;
	RICPP_PREAMBLE(REQ_ATTRIBUTE)
		name = renderState()->tokFindCreate(name);
		renderState()->parseParameters(RI_ATTRIBUTE, name, CParameterClasses(), n, tokens, params);
		nParamsSav = renderState()->curParamList().size();
		RICPP_PROCESS(newRiAttribute(renderState()->lineNo(), name, renderState()->curParamList()));
	RICPP_POSTAMBLE
	RICPP_UNREC_TOKENS(nParamsSav)
}


RtVoid CBaseRenderer::preProcess(CRiColor &obj)
{
	renderState()->attributes().color(obj.color());
}

RtVoid CBaseRenderer::color(RtColor Cs)
{
	RICPP_PREAMBLE(REQ_COLOR)
		RICPP_PROCESS(newRiColor(renderState()->lineNo(), renderState()->options().colorDescr().colorSamples(), Cs));
	RICPP_POSTAMBLE
}


RtVoid CBaseRenderer::preProcess(CRiOpacity &obj)
{
	renderState()->attributes().opacity(obj.opacity());
}

RtVoid CBaseRenderer::opacity(RtColor Os)
{
	RICPP_PREAMBLE(REQ_OPACITY)
		RICPP_PROCESS(newRiOpacity(renderState()->lineNo(), renderState()->options().colorDescr().colorSamples(), Os));
	RICPP_POSTAMBLE
}


RtVoid CBaseRenderer::preProcess(CRiSurface &obj)
{
	renderState()->attributes().surface(obj.name(), obj.parameters(), renderState()->curTransform());
}

RtVoid CBaseRenderer::surfaceV(RtString name, RtInt n, RtToken tokens[], RtPointer params[])
{
	size_t nParamsSav = n;
	RICPP_PREAMBLE(REQ_SURFACE)
		name = renderState()->tokFindCreate(name);
		renderState()->parseParameters(RI_SURFACE, name, CParameterClasses(), n, tokens, params);
		nParamsSav = renderState()->curParamList().size();
		RICPP_PROCESS(newRiSurface(renderState()->lineNo(), name, renderState()->curParamList()));
	RICPP_POSTAMBLE
	RICPP_UNREC_TOKENS(nParamsSav)
}


RtVoid CBaseRenderer::preProcess(CRiAtmosphere &obj)
{
	renderState()->attributes().atmosphere(obj.name(), obj.parameters(), renderState()->curTransform());
}

RtVoid CBaseRenderer::atmosphereV(RtString name, RtInt n, RtToken tokens[], RtPointer params[])
{
	size_t nParamsSav = n;
	RICPP_PREAMBLE(REQ_ATMOSPHERE)
		name = renderState()->tokFindCreate(name);
		renderState()->parseParameters(RI_ATMOSPHERE, name, CParameterClasses(), n, tokens, params);
		nParamsSav = renderState()->curParamList().size();
		RICPP_PROCESS(newRiAtmosphere(renderState()->lineNo(), name, renderState()->curParamList()));
	RICPP_POSTAMBLE
	RICPP_UNREC_TOKENS(nParamsSav)
}


RtVoid CBaseRenderer::preProcess(CRiInterior &obj)
{
	renderState()->attributes().interior(obj.name(), obj.parameters(), renderState()->curTransform());
}

RtVoid CBaseRenderer::interiorV(RtString name, RtInt n, RtToken tokens[], RtPointer params[])
{
	size_t nParamsSav = n;
	RICPP_PREAMBLE(REQ_INTERIOR)
		name = renderState()->tokFindCreate(name);
		renderState()->parseParameters(RI_INTERIOR, name, CParameterClasses(), n, tokens, params);
		nParamsSav = renderState()->curParamList().size();
		RICPP_PROCESS(newRiInterior(renderState()->lineNo(), name, renderState()->curParamList()));
	RICPP_POSTAMBLE
	RICPP_UNREC_TOKENS(nParamsSav)
}


RtVoid CBaseRenderer::preProcess(CRiExterior &obj)
{
	renderState()->attributes().exterior(obj.name(), obj.parameters(), renderState()->curTransform());
}

RtVoid CBaseRenderer::exteriorV(RtString name, RtInt n, RtToken tokens[], RtPointer params[])
{
	size_t nParamsSav = n;
	RICPP_PREAMBLE(REQ_EXTERIOR)
		name = renderState()->tokFindCreate(name);
		renderState()->parseParameters(RI_EXTERIOR, name, CParameterClasses(), n, tokens, params);
		nParamsSav = renderState()->curParamList().size();
		RICPP_PROCESS(newRiExterior(renderState()->lineNo(), name, renderState()->curParamList()));
	RICPP_POSTAMBLE
	RICPP_UNREC_TOKENS(nParamsSav)
}


RtVoid CBaseRenderer::preProcess(CRiDisplacement &obj)
{
	renderState()->attributes().displacement(obj.name(), obj.parameters(), renderState()->curTransform());
}

RtVoid CBaseRenderer::displacementV(RtString name, RtInt n, RtToken tokens[], RtPointer params[])
{
	size_t nParamsSav = n;
	RICPP_PREAMBLE(REQ_DISPLACEMENT)
		name = renderState()->tokFindCreate(name);
		renderState()->parseParameters(RI_DISPLACEMENT, name, CParameterClasses(), n, tokens, params);
		nParamsSav = renderState()->curParamList().size();
		RICPP_PROCESS(newRiDisplacement(renderState()->lineNo(), name, renderState()->curParamList()));
	RICPP_POSTAMBLE
	RICPP_UNREC_TOKENS(nParamsSav)
}

RtVoid CBaseRenderer::preProcess(CRiDeformation &obj)
{
	renderState()->attributes().deformation(obj.name(), obj.parameters(), renderState()->curTransform());
}

RtVoid CBaseRenderer::deformationV(RtString name, RtInt n, RtToken tokens[], RtPointer params[])
{
	size_t nParamsSav = n;
	RICPP_PREAMBLE(REQ_DEFORMATION)
		name = renderState()->tokFindCreate(name);
		renderState()->parseParameters(RI_DEFORMATION, name, CParameterClasses(), n, tokens, params);
		nParamsSav = renderState()->curParamList().size();
		RICPP_PROCESS(newRiDeformation(renderState()->lineNo(), name, renderState()->curParamList()));
	RICPP_POSTAMBLE
	RICPP_UNREC_TOKENS(nParamsSav)
}

RtVoid CBaseRenderer::preProcess(CRiTextureCoordinates &obj)
{
	renderState()->attributes().textureCoordinates(obj.s1(), obj.t1(), obj.s2(), obj.t2(), obj.s3(), obj.t3(), obj.s4(), obj.t4());
}

RtVoid CBaseRenderer::textureCoordinates(RtFloat s1, RtFloat t1, RtFloat s2, RtFloat t2, RtFloat s3, RtFloat t3, RtFloat s4, RtFloat t4)
{
	RICPP_PREAMBLE(REQ_TEXTURE_COORDINATES)
		RICPP_PROCESS(newRiTextureCoordinates(renderState()->lineNo(), s1, t1, s2, t2, s3, t3, s4, t4));
	RICPP_POSTAMBLE
}


RtVoid CBaseRenderer::preProcess(CRiShadingRate &obj)
{
	renderState()->attributes().shadingRate(obj.size());
}

RtVoid CBaseRenderer::shadingRate(RtFloat size)
{
	RICPP_PREAMBLE(REQ_SHADING_RATE)
		RICPP_PROCESS(newRiShadingRate(renderState()->lineNo(), size));
	RICPP_POSTAMBLE
}


RtVoid CBaseRenderer::preProcess(CRiShadingInterpolation &obj)
{
	renderState()->attributes().shadingInterpolation(obj.type());
}

RtVoid CBaseRenderer::shadingInterpolation(RtToken type)
{
	RICPP_PREAMBLE(REQ_SHADING_INTERPOLATION)
		type = renderState()->tokFindCreate(type);
		RICPP_PROCESS(newRiShadingInterpolation(renderState()->lineNo(), type));
	RICPP_POSTAMBLE
}


RtVoid CBaseRenderer::preProcess(CRiMatte &obj)
{
	renderState()->attributes().matte(obj.onOff());
}

RtVoid CBaseRenderer::matte(RtBoolean onoff)
{
	RICPP_PREAMBLE(REQ_MATTE)
		RICPP_PROCESS(newRiMatte(renderState()->lineNo(), onoff == RI_FALSE ? RI_FALSE : RI_TRUE));
	RICPP_POSTAMBLE
}


RtVoid CBaseRenderer::preProcess(CRiBound &obj)
{
	renderState()->attributes().bound(obj.bound());
}

RtVoid CBaseRenderer::bound(RtBound aBound)
{
	RICPP_PREAMBLE(REQ_BOUND)
		RICPP_PROCESS(newRiBound(renderState()->lineNo(), aBound));
	RICPP_POSTAMBLE
}


RtVoid CBaseRenderer::preProcess(CRiDetail &obj)
{
	renderState()->attributes().detail(obj.bound());
}

RtVoid CBaseRenderer::detail(RtBound aBound)
{
	RICPP_PREAMBLE(REQ_DETAIL)
		RICPP_PROCESS(newRiDetail(renderState()->lineNo(), aBound));
	RICPP_POSTAMBLE
}


RtVoid CBaseRenderer::preProcess(CRiDetailRange &obj)
{
	/** The first call of detailRange() within an attribute (world) block stores the attributes.
	 *  Subsequent calls restore these attribute state. An attributeEnd will pop the additional
	 *  attribute state.
	 */
	if ( !renderState()->attributes().detailRangeCalledInBlock() ) {
		renderState()->attributes().detailRange(obj.minVis(), obj.lowTran(), obj.upTran(), obj.maxVis());
		renderState()->storeAttributes();
	} else {
		renderState()->restoreAttributes();
		renderState()->attributes().detailRange(obj.minVis(), obj.lowTran(), obj.upTran(), obj.maxVis());
	}
}

RtVoid CBaseRenderer::detailRange(RtFloat minvis, RtFloat lowtran, RtFloat uptran, RtFloat maxvis)
{
	bool changed = false;
	if ( minvis > lowtran ) {
		changed = true;
		std::swap(minvis, lowtran);
	}
	if ( lowtran > uptran ) {
		changed = true;
		std::swap(lowtran, uptran);
		if ( minvis > lowtran ) {
			std::swap(minvis, lowtran);
		}
	}
	if ( uptran > maxvis ) {
		changed = true;
		std::swap(uptran, maxvis);
		if ( lowtran > uptran ) {
			std::swap(lowtran, uptran);
			if ( minvis > lowtran ) {
				std::swap(minvis, lowtran);
			}
		}
	}

	RICPP_PREAMBLE(REQ_DETAIL_RANGE)
		RICPP_PROCESS(newRiDetailRange(renderState()->lineNo(), minvis, lowtran, uptran, maxvis));
	RICPP_POSTAMBLE

	if ( changed ) {
		ricppErrHandler().handleError(
			RIE_BADTOKEN, RIE_ERROR,
			renderState()->printLineNo(__LINE__),
			renderState()->printName(__FILE__),
			"'%s': Detail range malformed, changed to (%f, %f, %f, %f)", 
			CRequestInfo::requestName(req), minvis, lowtran, uptran, maxvis);
	}
}


RtVoid CBaseRenderer::preProcess(CRiGeometricApproximation &obj)
{
	renderState()->attributes().geometricApproximation(obj.type(), obj.value());
}

RtVoid CBaseRenderer::geometricApproximation(RtToken type, RtFloat value)
{
	RICPP_PREAMBLE(REQ_GEOMETRIC_APPROXIMATION)
		type = renderState()->tokFindCreate(type);
		RICPP_PROCESS(newRiGeometricApproximation(renderState()->lineNo(), type, value));
	RICPP_POSTAMBLE
}


RtVoid CBaseRenderer::preProcess(CRiGeometricRepresentation &obj)
{
	renderState()->attributes().geometricRepresentation(obj.type());
}

RtVoid CBaseRenderer::geometricRepresentation(RtToken type)
{
	RICPP_PREAMBLE(REQ_GEOMETRIC_REPRESENTATION)
		type = renderState()->tokFindCreate(type);
		RICPP_PROCESS(newRiGeometricRepresentation(renderState()->lineNo(), type));
	RICPP_POSTAMBLE
}


RtVoid CBaseRenderer::preProcess(CRiOrientation &obj)
{
	renderState()->attributes().orientation(obj.orientation());
}

RtVoid CBaseRenderer::orientation(RtToken anOrientation)
{
	RtToken tokOrientation = RI_INSIDE;
	RICPP_PREAMBLE(REQ_ORIENTATION)
		anOrientation = renderState()->tokFindCreate(anOrientation);
		if ( anOrientation == RI_LH ||
		     anOrientation == RI_RH ||
		     anOrientation == RI_OUTSIDE )
		{
			tokOrientation = anOrientation;
		}
		RICPP_PROCESS(newRiOrientation(renderState()->lineNo(), tokOrientation));
	RICPP_POSTAMBLE

	if ( tokOrientation != anOrientation ) {
		ricppErrHandler().handleError(
			RIE_BADTOKEN, RIE_ERROR,
			renderState()->printLineNo(__LINE__),
			renderState()->printName(__FILE__),
			"Unrecognized tokens in '%s': %s, using %s instead.", 
			CRequestInfo::requestName(req), noNullStr(anOrientation), noNullStr(tokOrientation));
	}
}


RtVoid CBaseRenderer::preProcess(CRiReverseOrientation &obj)
{
	renderState()->attributes().reverseOrientation();
}

RtVoid CBaseRenderer::reverseOrientation(void)
{
	RICPP_PREAMBLE(REQ_REVERSE_ORIENTATION)
		RICPP_PROCESS(newRiReverseOrientation(renderState()->lineNo()));
	RICPP_POSTAMBLE
}


RtVoid CBaseRenderer::preProcess(CRiSides &obj)
{
	renderState()->attributes().sides(obj.nSides());
}

RtVoid CBaseRenderer::sides(RtInt nsides)
{
	RICPP_PREAMBLE(REQ_SIDES)
		RICPP_PROCESS(newRiSides(renderState()->lineNo(), nsides));
	RICPP_POSTAMBLE
}


RtVoid CBaseRenderer::preProcess(CRiBasis &obj)
{
	renderState()->attributes().basis(obj.uBasis(), obj.uStep(), obj.vBasis(), obj.vStep());
}

RtVoid CBaseRenderer::basis(RtBasis ubasis, RtInt ustep, RtBasis vbasis, RtInt vstep)
{
	RICPP_PREAMBLE(REQ_BASIS)
		RICPP_PROCESS(newRiBasis(renderState()->lineNo(), ubasis, ustep, vbasis, vstep));
	RICPP_POSTAMBLE
}


RtVoid CBaseRenderer::preProcess(CRiTrimCurve &obj)
{
	renderState()->attributes().trimCurve(obj.trimCurve());
}

RtVoid CBaseRenderer::trimCurve(RtInt nloops, RtInt ncurves[], RtInt order[], RtFloat knot[], RtFloat amin[], RtFloat amax[], RtInt n[], RtFloat u[], RtFloat v[], RtFloat w[])
{
	RICPP_PREAMBLE(REQ_TRIM_CURVE)
		RICPP_PROCESS(newRiTrimCurve(renderState()->lineNo(), nloops, ncurves, order, knot, amin, amax, n, u, v, w));
	RICPP_POSTAMBLE
}


RtVoid CBaseRenderer::preProcess(CRiIdentity &obj)
{
	RtFloat det = renderState()->curTransform().getCTM().determinant();
	renderState()->curTransform().identity();
	if ( det * renderState()->curTransform().getCTM().determinant() < 0 )
		renderState()->attributes().toggleOrientation();
}


RtVoid CBaseRenderer::identity(void)
{
	RICPP_PREAMBLE(REQ_IDENTITY)
		RICPP_PROCESS(newRiIdentity(renderState()->lineNo()));
	RICPP_POSTAMBLE
}


RtVoid CBaseRenderer::preProcess(CRiTransform &obj)
{
	RtFloat det = renderState()->curTransform().getCTM().determinant();
	renderState()->curTransform().transform(obj.transform());
	if ( det * renderState()->curTransform().getCTM().determinant() < 0 )
		renderState()->attributes().toggleOrientation();
}


RtVoid CBaseRenderer::transform(RtMatrix aTransform)
{
	RICPP_PREAMBLE(REQ_TRANSFORM)
		RICPP_PROCESS(newRiTransform(renderState()->lineNo(), aTransform));
	RICPP_POSTAMBLE
}


RtVoid CBaseRenderer::preProcess(CRiConcatTransform &obj)
{
	RtFloat det = renderState()->curTransform().getCTM().determinant();
	renderState()->curTransform().concatTransform(obj.transform());
	if ( det * renderState()->curTransform().getCTM().determinant() < 0 )
		renderState()->attributes().toggleOrientation();
}


RtVoid CBaseRenderer::concatTransform(RtMatrix aTransform)
{
	RICPP_PREAMBLE(REQ_CONCAT_TRANSFORM)
		RICPP_PROCESS(newRiConcatTransform(renderState()->lineNo(), aTransform));
	RICPP_POSTAMBLE
}


RtVoid CBaseRenderer::preProcess(CRiPerspective &obj)
{
	renderState()->curTransform().perspective(obj.fov());
}


RtVoid CBaseRenderer::perspective(RtFloat fov)
{
	RICPP_PREAMBLE(REQ_PERSPECTIVE)
		RICPP_PROCESS(newRiPerspective(renderState()->lineNo(), fov));
	RICPP_POSTAMBLE
}


RtVoid CBaseRenderer::preProcess(CRiTranslate &obj)
{
	renderState()->curTransform().translate(obj.dx(), obj.dy(), obj.dz());
}


RtVoid CBaseRenderer::translate(RtFloat dx, RtFloat dy, RtFloat dz)
{
	RICPP_PREAMBLE(REQ_TRANSLATE)
		RICPP_PROCESS(newRiTranslate(renderState()->lineNo(), dx, dy, dz));
	RICPP_POSTAMBLE
}


RtVoid CBaseRenderer::preProcess(CRiRotate &obj)
{
	renderState()->curTransform().rotate(obj.angle(), obj.dx(), obj.dy(), obj.dz());
}


RtVoid CBaseRenderer::rotate(RtFloat angle, RtFloat dx, RtFloat dy, RtFloat dz)
{
	RICPP_PREAMBLE(REQ_ROTATE)
		RICPP_PROCESS(newRiRotate(renderState()->lineNo(), angle, dx, dy, dz));
	RICPP_POSTAMBLE
}


RtVoid CBaseRenderer::preProcess(CRiScale &obj)
{
	RtFloat det = renderState()->curTransform().getCTM().determinant();
	renderState()->curTransform().scale(obj.dx(), obj.dy(), obj.dz());
	if ( det * renderState()->curTransform().getCTM().determinant() < 0 )
		renderState()->attributes().toggleOrientation();
}


RtVoid CBaseRenderer::scale(RtFloat dx, RtFloat dy, RtFloat dz)
{
	RICPP_PREAMBLE(REQ_SCALE)
		RICPP_PROCESS(newRiScale(renderState()->lineNo(), dx, dy, dz));
	RICPP_POSTAMBLE
}


RtVoid CBaseRenderer::preProcess(CRiSkew &obj)
{
	// RtFloat det = renderState()->curTransform().getCTM().determinant();
	renderState()->curTransform().skew(obj.angle(), obj.dx1(), obj.dy1(), obj.dz1(), obj.dx2(), obj.dy2(), obj.dz2());
	// if ( det * renderState()->curTransform().getCTM().determinant() < 0 )
	//	renderState()->attributes().toggleOrientation();
}


RtVoid CBaseRenderer::skew(RtFloat angle, RtFloat dx1, RtFloat dy1, RtFloat dz1, RtFloat dx2, RtFloat dy2, RtFloat dz2)
{
	RICPP_PREAMBLE(REQ_SKEW)
		RICPP_PROCESS(newRiSkew(renderState()->lineNo(), angle, dx1, dy1, dz1, dx2, dy2, dz2));
	RICPP_POSTAMBLE
}


RtVoid CBaseRenderer::preProcess(CRiScopedCoordinateSystem &obj)
{
	renderState()->scopedCoordinateSystem(obj.space());
}

RtVoid CBaseRenderer::scopedCoordinateSystem(RtToken space)
{
	RICPP_PREAMBLE(REQ_SCOPED_COORDINATE_SYSTEM)
		space = renderState()->tokFindCreate(space);
		RICPP_PROCESS(newRiScopedCoordinateSystem(renderState()->lineNo(), space));
	RICPP_POSTAMBLE
}


RtVoid CBaseRenderer::preProcess(CRiCoordinateSystem &obj)
{
	renderState()->coordinateSystem(obj.space());
}

RtVoid CBaseRenderer::coordinateSystem(RtToken space)
{
	RICPP_PREAMBLE(REQ_COORDINATE_SYSTEM)
		space = renderState()->tokFindCreate(space);
		RICPP_PROCESS(newRiCoordinateSystem(renderState()->lineNo(), space));
	RICPP_POSTAMBLE
}


RtVoid CBaseRenderer::preProcess(CRiCoordSysTransform &obj)
{
	RtFloat det = renderState()->curTransform().getCTM().determinant();
	renderState()->coordSysTransform(obj.space());
	if ( det * renderState()->curTransform().getCTM().determinant() < 0 )
		renderState()->attributes().toggleOrientation();
}

RtVoid CBaseRenderer::coordSysTransform(RtToken space)
{
	RICPP_PREAMBLE(REQ_COORD_SYS_TRANSFORM)
		space = renderState()->tokFindCreate(space);
		RICPP_PROCESS(newRiCoordSysTransform(renderState()->lineNo(), space));
	RICPP_POSTAMBLE
}


RtVoid CBaseRenderer::doProcess(CRiTransformPoints &obj)
{
	renderState()->transformPoints(obj.fromSpace(), obj.toSpace(), obj.nPoints(), obj.points());
}

RtPoint *CBaseRenderer::transformPoints(RtToken fromspace, RtToken tospace, RtInt npoints, RtPoint points[]) {
	RICPP_PREAMBLE_RET(REQ_TRANSFORM_POINTS, 0)
		fromspace = renderState()->tokFindCreate(fromspace);
		tospace = renderState()->tokFindCreate(tospace);
		RICPP_PROCESS(newRiTransformPoints(renderState()->lineNo(), fromspace, tospace, npoints, points));
	RICPP_POSTAMBLE_RET(0);
	return points ? &points[0] : 0;
}


RtVoid CBaseRenderer::polygonV(RtInt nvertices, RtInt n, RtToken tokens[], RtPointer params[])
{
	size_t nParamsSav = n;
	RICPP_PREAMBLE(REQ_POLYGON)
		renderState()->parseParameters(CPolygonClasses(nvertices), n, tokens, params);
		nParamsSav = renderState()->curParamList().size();
		RICPP_PROCESS(newRiPolygon(renderState()->lineNo(), nvertices, renderState()->curParamList()));
	RICPP_POSTAMBLE
	RICPP_UNREC_TOKENS(nParamsSav)
}


RtVoid CBaseRenderer::generalPolygonV(RtInt nloops, RtInt nverts[], RtInt n, RtToken tokens[], RtPointer params[])
{
	size_t nParamsSav = n;
	RICPP_PREAMBLE(REQ_GENERAL_POLYGON)
		renderState()->parseParameters(CGeneralPolygonClasses(nloops, nverts), n, tokens, params);
		nParamsSav = renderState()->curParamList().size();
		RICPP_PROCESS(newRiGeneralPolygon(renderState()->lineNo(), nloops, nverts, renderState()->curParamList()));
	RICPP_POSTAMBLE
	RICPP_UNREC_TOKENS(nParamsSav)
}


RtVoid CBaseRenderer::pointsPolygonsV(RtInt npolys, RtInt nverts[], RtInt verts[], RtInt n, RtToken tokens[], RtPointer params[])
{
	size_t nParamsSav = n;
	RICPP_PREAMBLE(REQ_POINTS_POLYGONS)
		renderState()->parseParameters(CPointsPolygonsClasses(npolys, nverts, verts), n, tokens, params);
		nParamsSav = renderState()->curParamList().size();
		RICPP_PROCESS(newRiPointsPolygons(renderState()->lineNo(), npolys, nverts, verts, renderState()->curParamList()));
	RICPP_POSTAMBLE
	RICPP_UNREC_TOKENS(nParamsSav)
}


RtVoid CBaseRenderer::pointsGeneralPolygonsV(RtInt npolys, RtInt nloops[], RtInt nverts[], RtInt verts[],  RtInt n, RtToken tokens[], RtPointer params[])
{
	size_t nParamsSav = n;
	RICPP_PREAMBLE(REQ_POINTS_GENERAL_POLYGONS)
		renderState()->parseParameters(CPointsGeneralPolygonsClasses(npolys, nloops, nverts, verts), n, tokens, params);
		nParamsSav = renderState()->curParamList().size();
		RICPP_PROCESS(newRiPointsGeneralPolygons(renderState()->lineNo(), npolys, nloops, nverts, verts, renderState()->curParamList()));
	RICPP_POSTAMBLE
	RICPP_UNREC_TOKENS(nParamsSav)
}


RtVoid CBaseRenderer::patchV(RtToken type, RtInt n, RtToken tokens[], RtPointer params[])
{
	size_t nParamsSav = n;
	RICPP_PREAMBLE(REQ_PATCH)
		type = renderState()->tokFindCreate(type);
		if ( type != RI_BILINEAR && type != RI_BICUBIC )
		{
			throw ExceptRiCPPError(RIE_BADTOKEN, RIE_ERROR, renderState()->printLineNo(__LINE__), renderState()->printName(__FILE__), "type neither RI_BILINEAR nor RI_BICUBIC at %s(), type == '%s'", CRequestInfo::requestName(req), type);
		}
		renderState()->parseParameters(CPatchClasses(type), n, tokens, params);
		nParamsSav = renderState()->curParamList().size();
		RICPP_PROCESS(newRiPatch(renderState()->lineNo(), type, renderState()->curParamList()));
	RICPP_POSTAMBLE
	RICPP_UNREC_TOKENS(nParamsSav)
}


RtVoid CBaseRenderer::patchMeshV(RtToken type, RtInt nu, RtToken uwrap, RtInt nv, RtToken vwrap, RtInt n, RtToken tokens[], RtPointer params[])
{
	size_t nParamsSav = n;
	RICPP_PREAMBLE(REQ_PATCH_MESH)
		type = renderState()->tokFindCreate(type);
		if ( type != RI_BILINEAR && type != RI_BICUBIC )
		{
			throw ExceptRiCPPError(RIE_BADTOKEN, RIE_ERROR, renderState()->printLineNo(__LINE__), renderState()->printName(__FILE__), "type neither RI_BILINEAR nor RI_BICUBIC at %s(), type == '%s'", CRequestInfo::requestName(req), type);
		}

		uwrap = renderState()->tokFindCreate(uwrap);
		if ( uwrap != RI_PERIODIC && uwrap != RI_NONPERIODIC )
		{
			throw ExceptRiCPPError(RIE_BADTOKEN, RIE_ERROR, renderState()->printLineNo(__LINE__), renderState()->printName(__FILE__), "uwrap neither RI_PERIODIC nor RI_NONPERIODIC at %s(), uwrap == '%s'", CRequestInfo::requestName(req), uwrap);
		}

		vwrap = renderState()->tokFindCreate(vwrap);
		if ( vwrap != RI_PERIODIC && vwrap != RI_NONPERIODIC )
		{
			throw ExceptRiCPPError(RIE_BADTOKEN, RIE_ERROR, renderState()->printLineNo(__LINE__), renderState()->printName(__FILE__), "vwrap neither RI_PERIODIC nor RI_NONPERIODIC at %s(), vwrap == '%s'", CRequestInfo::requestName(req), vwrap);
		}

		RtInt ustep, vstep;
		if ( type == RI_BILINEAR ) {
			ustep = vstep = 1;
		} else {
			RtBasis ubasis, vbasis;
			renderState()->attributes().getBasis(ubasis, ustep, vbasis, vstep);
		}

		renderState()->parseParameters(CPatchMeshClasses(type, nu, ustep, uwrap, nv, vstep, vwrap), n, tokens, params);
		nParamsSav = renderState()->curParamList().size();
		RICPP_PROCESS(newRiPatchMesh(renderState()->lineNo(), ustep, vstep, type, nu, uwrap, nv, vwrap, renderState()->curParamList()));
	RICPP_POSTAMBLE
	RICPP_UNREC_TOKENS(nParamsSav)
}


RtVoid CBaseRenderer::nuPatchV(RtInt nu, RtInt uorder, RtFloat uknot[], RtFloat umin, RtFloat umax, RtInt nv, RtInt vorder, RtFloat vknot[], RtFloat vmin, RtFloat vmax,  RtInt n, RtToken tokens[], RtPointer params[])
{
	size_t nParamsSav = n;
	RICPP_PREAMBLE(REQ_NU_PATCH)
		renderState()->parseParameters(CNuPatchClasses(nu, uorder, nv, vorder), n, tokens, params);
		nParamsSav = renderState()->curParamList().size();
		RICPP_PROCESS(newRiNuPatch(renderState()->lineNo(), nu, uorder, uknot, umin, umax, nv, vorder, vknot, vmin, vmax, renderState()->curParamList()));
	RICPP_POSTAMBLE
	RICPP_UNREC_TOKENS(nParamsSav)
}


RtVoid CBaseRenderer::subdivisionMeshV(RtToken scheme, RtInt nfaces, RtInt nvertices[], RtInt vertices[], RtInt ntags, RtToken tags[], RtInt nargs[], RtInt intargs[], RtFloat floatargs[],  RtInt n, RtToken tokens[], RtPointer params[])
{
	size_t nParamsSav = n;
	RICPP_PREAMBLE(REQ_SUBDIVISION_MESH)
		scheme = renderState()->tokFindCreate(scheme);
		renderState()->parseParameters(CSubdivisionMeshClasses(nfaces, nvertices, vertices), n, tokens, params);
		nParamsSav = renderState()->curParamList().size();
		RICPP_PROCESS(newRiSubdivisionMesh(renderState()->lineNo(), &renderState()->tokenMap(), scheme, nfaces, nvertices, vertices, ntags, tags, nargs, intargs, floatargs, renderState()->curParamList()));
	RICPP_POSTAMBLE
	RICPP_UNREC_TOKENS(nParamsSav)
}

RtVoid CBaseRenderer::hierarchicalSubdivisionMeshV(RtToken scheme, RtInt nfaces, RtInt nvertices[], RtInt vertices[], RtInt ntags, RtToken tags[], RtInt nargs[], RtInt intargs[], RtFloat floatargs[],  RtToken stringargs[],  RtInt n, RtToken tokens[], RtPointer params[])
{
	size_t nParamsSav = n;
	RICPP_PREAMBLE(REQ_HIERARCHICAL_SUBDIVISION_MESH)
		scheme = renderState()->tokFindCreate(scheme);
		renderState()->parseParameters(CSubdivisionMeshClasses(nfaces, nvertices, vertices), n, tokens, params);
		nParamsSav = renderState()->curParamList().size();
		RICPP_PROCESS(newRiHierarchicalSubdivisionMesh(renderState()->lineNo(), &renderState()->tokenMap(), scheme, nfaces, nvertices, vertices, ntags, tags, nargs, intargs, floatargs, stringargs, renderState()->curParamList()));
	RICPP_POSTAMBLE
	RICPP_UNREC_TOKENS(nParamsSav)
}

RtVoid CBaseRenderer::sphereV(RtFloat radius, RtFloat zmin, RtFloat zmax, RtFloat thetamax, RtInt n, RtToken tokens[], RtPointer params[])
{
	size_t nParamsSav = n;
	RICPP_PREAMBLE(REQ_SPHERE)
		renderState()->parseParameters(CQuadricClasses(), n, tokens, params);
		nParamsSav = renderState()->curParamList().size();
		RICPP_PROCESS(newRiSphere(renderState()->lineNo(), radius, zmin, zmax, thetamax, renderState()->curParamList()));
	RICPP_POSTAMBLE
	RICPP_UNREC_TOKENS(nParamsSav)
}

RtVoid CBaseRenderer::coneV(RtFloat height, RtFloat radius, RtFloat thetamax, RtInt n, RtToken tokens[], RtPointer params[])
{
	size_t nParamsSav = n;
	RICPP_PREAMBLE(REQ_CONE)
		renderState()->parseParameters(CQuadricClasses(), n, tokens, params);
		nParamsSav = renderState()->curParamList().size();
		RICPP_PROCESS(newRiCone(renderState()->lineNo(), height, radius, thetamax, renderState()->curParamList()));
	RICPP_POSTAMBLE
	RICPP_UNREC_TOKENS(nParamsSav)
}

RtVoid CBaseRenderer::cylinderV(RtFloat radius, RtFloat zmin, RtFloat zmax, RtFloat thetamax, RtInt n, RtToken tokens[], RtPointer params[])
{
	size_t nParamsSav = n;
	RICPP_PREAMBLE(REQ_CYLINDER)
		renderState()->parseParameters(CQuadricClasses(), n, tokens, params);
		nParamsSav = renderState()->curParamList().size();
		RICPP_PROCESS(newRiCylinder(renderState()->lineNo(), radius, zmin, zmax, thetamax, renderState()->curParamList()));
	RICPP_POSTAMBLE
	RICPP_UNREC_TOKENS(nParamsSav)
}

RtVoid CBaseRenderer::hyperboloidV(RtPoint point1, RtPoint point2, RtFloat thetamax, RtInt n, RtToken tokens[], RtPointer params[])
{
	size_t nParamsSav = n;
	RICPP_PREAMBLE(REQ_HYPERBOLOID)
		renderState()->parseParameters(CQuadricClasses(), n, tokens, params);
		nParamsSav = renderState()->curParamList().size();
		RICPP_PROCESS(newRiHyperboloid(renderState()->lineNo(), point1, point2, thetamax, renderState()->curParamList()));
	RICPP_POSTAMBLE
	RICPP_UNREC_TOKENS(nParamsSav)
}

RtVoid CBaseRenderer::paraboloidV(RtFloat rmax, RtFloat zmin, RtFloat zmax, RtFloat thetamax, RtInt n, RtToken tokens[], RtPointer params[])
{
	size_t nParamsSav = n;
	RICPP_PREAMBLE(REQ_PARABOLOID)
		renderState()->parseParameters(CQuadricClasses(), n, tokens, params);
		nParamsSav = renderState()->curParamList().size();
		RICPP_PROCESS(newRiParaboloid(renderState()->lineNo(), rmax, zmin, zmax, thetamax, renderState()->curParamList()));
	RICPP_POSTAMBLE
	RICPP_UNREC_TOKENS(nParamsSav)
}

RtVoid CBaseRenderer::diskV(RtFloat height, RtFloat radius, RtFloat thetamax, RtInt n, RtToken tokens[], RtPointer params[])
{
	size_t nParamsSav = n;
	RICPP_PREAMBLE(REQ_DISK)
		renderState()->parseParameters(CQuadricClasses(), n, tokens, params);
		nParamsSav = renderState()->curParamList().size();
		RICPP_PROCESS(newRiDisk(renderState()->lineNo(), height, radius, thetamax, renderState()->curParamList()));
	RICPP_POSTAMBLE
	RICPP_UNREC_TOKENS(nParamsSav)
}

RtVoid CBaseRenderer::torusV(RtFloat majorrad, RtFloat minorrad, RtFloat phimin, RtFloat phimax, RtFloat thetamax, RtInt n, RtToken tokens[], RtPointer params[])
{
	size_t nParamsSav = n;
	RICPP_PREAMBLE(REQ_TORUS)
		renderState()->parseParameters(CQuadricClasses(), n, tokens, params);
		nParamsSav = renderState()->curParamList().size();
		RICPP_PROCESS(newRiTorus(renderState()->lineNo(), majorrad, minorrad, phimin, phimax, thetamax, renderState()->curParamList()));
	RICPP_POSTAMBLE
	RICPP_UNREC_TOKENS(nParamsSav)
}

RtVoid CBaseRenderer::pointsV(RtInt npts, RtInt n, RtToken tokens[], RtPointer params[])
{
	size_t nParamsSav = n;
	RICPP_PREAMBLE(REQ_POINTS)
		renderState()->parseParameters(CPointsClasses(npts), n, tokens, params);
		nParamsSav = renderState()->curParamList().size();
		RICPP_PROCESS(newRiPoints(renderState()->lineNo(), npts, renderState()->curParamList()));
	RICPP_POSTAMBLE
	RICPP_UNREC_TOKENS(nParamsSav)
}

RtVoid CBaseRenderer::curvesV(RtToken type, RtInt ncurves, RtInt nverts[], RtToken wrap, RtInt n, RtToken tokens[], RtPointer params[])
{
	size_t nParamsSav = n;
	RICPP_PREAMBLE(REQ_CURVES)
		type = renderState()->tokFindCreate(type);
		renderState()->parseParameters(CCurvesClasses(type, ncurves, nverts, wrap, renderState()->attributes().vStep()), n, tokens, params);
		nParamsSav = renderState()->curParamList().size();
		RICPP_PROCESS(newRiCurves(renderState()->lineNo(), renderState()->attributes().vStep(), type, ncurves, nverts, wrap, renderState()->curParamList()));
	RICPP_POSTAMBLE
	RICPP_UNREC_TOKENS(nParamsSav)
}

RtVoid CBaseRenderer::blobbyV(RtInt nleaf, RtInt ncode, RtInt code[], RtInt nflt, RtFloat flt[], RtInt nstr, RtString str[], RtInt n, RtToken tokens[], RtPointer params[])
{
	size_t nParamsSav = n;
	RICPP_PREAMBLE(REQ_BLOBBY)
		renderState()->parseParameters(CBlobbyClasses(nleaf), n, tokens, params);
		nParamsSav = renderState()->curParamList().size();
		RICPP_PROCESS(newRiBlobby(renderState()->lineNo(), nleaf, ncode, code, nflt, flt, nstr, str, renderState()->curParamList()));
	RICPP_POSTAMBLE
	RICPP_UNREC_TOKENS(nParamsSav)
}


RtVoid CBaseRenderer::preProcess(CRiProcedural &obj)
{
	if ( !m_parserCallback ) {
		throw ExceptRiCPPError(
			RIE_BUG,
			RIE_SEVERE,
			renderState()->printLineNo(__LINE__),
			renderState()->printName(__FILE__),
			"The frontend is unknown, while executing a procedural %s.", obj.subdivFunc()->name());
	}
}

RtVoid CBaseRenderer::doProcess(CRiProcedural &obj)
{
	assert(obj.subdivFunc() != 0);
	if ( obj.subdivFunc() ) {
		(*(obj.subdivFunc()))(m_parserCallback->frontend(), obj.data(), RI_INFINITY);
	} else {
		/** @todo Error, CBaseRenderer::doProcedural has no subdivfunc.
		 */
	}
	if ( obj.freeFunc() ) {
		(*(obj.freeFunc()))(obj.data());
	}
}

RtVoid CBaseRenderer::procedural(RtPointer data, RtBound bound, ISubdivFunc &subdivfunc, IFreeFunc *freefunc)
{
	RICPP_PREAMBLE(REQ_PROCEDURAL)
		RICPP_PROCESS(newRiProcedural(renderState()->lineNo(), data, bound, subdivfunc, freefunc));
	RICPP_POSTAMBLE
}

RtVoid CBaseRenderer::doProcess(CRiGeometry &obj)
{
	if ( obj.type() == RI_TEAPOT ) {
		teapot();
	}
}

RtVoid CBaseRenderer::geometryV(RtToken type, RtInt n, RtToken tokens[], RtPointer params[])
{
	size_t nParamsSav = n;
	RICPP_PREAMBLE(REQ_GEOMETRY)
		type = renderState()->tokFindCreate(type);
		renderState()->parseParameters(CParameterClasses(), n, tokens, params);
		nParamsSav = renderState()->curParamList().size();
		RICPP_PROCESS(newRiGeometry(renderState()->lineNo(), type, renderState()->curParamList()));
	RICPP_POSTAMBLE
	RICPP_UNREC_TOKENS(nParamsSav)
}

RtVoid CBaseRenderer::makeTextureV(RtString pic, RtString tex, RtToken swrap, RtToken twrap, const IFilterFunc &filterfunc, RtFloat swidth, RtFloat twidth, RtInt n, RtToken tokens[], RtPointer params[])
{
	size_t nParamsSav = n;
	RICPP_PREAMBLE(REQ_MAKE_TEXTURE)
		swrap = renderState()->tokFindCreate(swrap);
		twrap = renderState()->tokFindCreate(twrap);
		renderState()->parseParameters(RI_TEXTURE, RI_TEXTURE, CParameterClasses(), n, tokens, params);
		nParamsSav = renderState()->curParamList().size();
		RICPP_PROCESS(newRiMakeTexture(renderState()->lineNo(), pic, tex, swrap, twrap, filterfunc, swidth, twidth, renderState()->curParamList()));
	RICPP_POSTAMBLE
	RICPP_UNREC_TOKENS(nParamsSav)
}

RtVoid CBaseRenderer::makeBumpV(RtString pic, RtString tex, RtToken swrap, RtToken twrap, const IFilterFunc &filterfunc, RtFloat swidth, RtFloat twidth, RtInt n, RtToken tokens[], RtPointer params[])
{
	size_t nParamsSav = n;
	RICPP_PREAMBLE(REQ_MAKE_BUMP)
		swrap = renderState()->tokFindCreate(swrap);
		twrap = renderState()->tokFindCreate(twrap);
		renderState()->parseParameters(RI_TEXTURE, RI_BUMP, CParameterClasses(), n, tokens, params);
		nParamsSav = renderState()->curParamList().size();
		RICPP_PROCESS(newRiMakeBump(renderState()->lineNo(), pic, tex, swrap, twrap, filterfunc, swidth, twidth, renderState()->curParamList()));
	RICPP_POSTAMBLE
	RICPP_UNREC_TOKENS(nParamsSav)
}

RtVoid CBaseRenderer::makeLatLongEnvironmentV(RtString pic, RtString tex, const IFilterFunc &filterfunc, RtFloat swidth, RtFloat twidth, RtInt n, RtToken tokens[], RtPointer params[])
{
	size_t nParamsSav = n;
	RICPP_PREAMBLE(REQ_MAKE_LAT_LONG_ENVIRONMENT)
		renderState()->parseParameters(RI_TEXTURE, RI_LAT_LONG_ENVIRONMENT, CParameterClasses(), n, tokens, params);
		nParamsSav = renderState()->curParamList().size();
		RICPP_PROCESS(newRiMakeLatLongEnvironment(renderState()->lineNo(), pic, tex, filterfunc, swidth, twidth, renderState()->curParamList()));
	RICPP_POSTAMBLE
	RICPP_UNREC_TOKENS(nParamsSav)
}

RtVoid CBaseRenderer::makeCubeFaceEnvironmentV(RtString px, RtString nx, RtString py, RtString ny, RtString pz, RtString nz, RtString tex, RtFloat fov, const IFilterFunc &filterfunc, RtFloat swidth, RtFloat twidth, RtInt n, RtToken tokens[], RtPointer params[])
{
	size_t nParamsSav = n;
	RICPP_PREAMBLE(REQ_MAKE_CUBE_FACE_ENVIRONMENT)
		renderState()->parseParameters(RI_TEXTURE, RI_CUBE_FACE_ENVIRONMENT, CParameterClasses(), n, tokens, params);
		nParamsSav = renderState()->curParamList().size();
		RICPP_PROCESS(newRiMakeCubeFaceEnvironment(renderState()->lineNo(), px, nx, py, ny, pz, nz, tex, fov, filterfunc, swidth, twidth, renderState()->curParamList()));
	RICPP_POSTAMBLE
	RICPP_UNREC_TOKENS(nParamsSav)
}

RtVoid CBaseRenderer::makeShadowV(RtString pic, RtString tex, RtInt n, RtToken tokens[], RtPointer params[])
{
	size_t nParamsSav = n;
	RICPP_PREAMBLE(REQ_MAKE_SHADOW)
		renderState()->parseParameters(RI_TEXTURE, RI_SHADOW, CParameterClasses(), n, tokens, params);
		nParamsSav = renderState()->curParamList().size();
		RICPP_PROCESS(newRiMakeShadow(renderState()->lineNo(), pic, tex, renderState()->curParamList()));
	RICPP_POSTAMBLE
	RICPP_UNREC_TOKENS(nParamsSav)
}

RtVoid CBaseRenderer::makeBrickMapV(RtInt nNames, RtString ptcnames[], RtString bkmname, RtInt n, RtToken tokens[], RtPointer params[])
{
	size_t nParamsSav = n;
	RICPP_PREAMBLE(REQ_MAKE_BRICK_MAP)
		renderState()->parseParameters(RI_TEXTURE, RI_BRICK_MAP, CParameterClasses(), n, tokens, params);
		nParamsSav = renderState()->curParamList().size();
		RICPP_PROCESS(newRiMakeBrickMap(renderState()->lineNo(), nNames, ptcnames, bkmname, renderState()->curParamList()));
	RICPP_POSTAMBLE
	RICPP_UNREC_TOKENS(nParamsSav)
}

RtVoid CBaseRenderer::archiveRecordV(RtToken type, RtString line)
{
	RtToken typeTok = RI_COMMENT;
	RICPP_PREAMBLE(REQ_ARCHIVE_RECORD)
		type = renderState()->tokFindCreate(type);
		if ( type == RI_VERBATIM || type == RI_STRUCTURE )
			typeTok = type;
		RICPP_PROCESS(newRiArchiveRecord(renderState()->lineNo(), typeTok, line));
	RICPP_POSTAMBLE

	if ( typeTok != type ) {
		ricppErrHandler().handleError(
			RIE_BADTOKEN, RIE_ERROR,
			renderState()->printLineNo(__LINE__),
			renderState()->printName(__FILE__),
			"'%s': unknown comment type %s, taking %s", 
			CRequestInfo::requestName(req), noNullStr(type), noNullStr(typeTok));
	}
}

RtVoid CBaseRenderer::doProcess(CRiReadArchive &obj)
{
#ifdef _TRACE_ARCHIVE
	trace("-> CBaseRenderer::doProcess(CRiReadArchive)");
#endif
	processReadArchive(obj.name(), obj.callback(), obj.parameters());
#ifdef _TRACE_ARCHIVE
	trace("<- CBaseRenderer::doProcess(CRiReadArchive)");
#endif
}

RtVoid CBaseRenderer::readArchiveV(RtString name, const IArchiveCallback *callback, RtInt n, RtToken tokens[], RtPointer params[])
{
#ifdef _TRACE_ARCHIVE
	trace("-> CBaseRenderer::readArchiveV()");
#endif

	size_t nParamsSav = n;

	RICPP_PREAMBLE(REQ_READ_ARCHIVE)
		renderState()->parseParameters(CParameterClasses(), n, tokens, params);
		nParamsSav = renderState()->curParamList().size();
#ifdef _TRACE_ARCHIVE
		trace("** CBaseRenderer::readArchiveV() RICPP_PROCESS");
#endif
		RICPP_PROCESS(newRiReadArchive(renderState()->lineNo(), name, callback, renderState()->curParamList()));
	RICPP_POSTAMBLE
	RICPP_UNREC_TOKENS(nParamsSav)

#ifdef _TRACE_ARCHIVE
	trace("<- CBaseRenderer::readArchiveV()");
#endif
}


RtVoid CBaseRenderer::preProcess(CRiIfBegin &obj)
{
	renderState()->ifBegin(obj.exprStr());
}


RtVoid CBaseRenderer::ifBegin(RtString expr)
{
	RICPP_PREAMBLE(REQ_IF_BEGIN)
		RICPP_PROCESS(newRiIfBegin(renderState()->lineNo(), expr));
	RICPP_POSTAMBLE
}


RtVoid CBaseRenderer::preProcess(CRiElseIfBegin &obj)
{
	renderState()->elseIfBegin(obj.exprStr());
}


RtVoid CBaseRenderer::elseIfBegin(RtString expr)
{
	RICPP_PREAMBLE(REQ_ELSE_IF)
		RICPP_PROCESS(newRiElseIfBegin(renderState()->lineNo(), expr));
	RICPP_POSTAMBLE
}


RtVoid CBaseRenderer::preProcess(CRiElseBegin &obj)
{
	renderState()->elseBegin();
}


RtVoid CBaseRenderer::elseBegin(void)
{
	RICPP_PREAMBLE(REQ_ELSE)
		RICPP_PROCESS(newRiElseBegin(renderState()->lineNo()));
	RICPP_POSTAMBLE
}


RtVoid CBaseRenderer::preProcess(CRiIfEnd &obj)
{
	renderState()->ifEnd();
}

RtVoid CBaseRenderer::ifEnd(void)
{
	RICPP_PREAMBLE(REQ_IF_END)
		RICPP_PROCESS(newRiIfEnd(renderState()->lineNo()));
	RICPP_POSTAMBLE
}
