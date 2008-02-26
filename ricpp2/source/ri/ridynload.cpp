// Adapter ri - ricpp Adapter for dynamic load procedurals (CRiCPPBridge Instance from procedural), empty begin, end

#define RI_EXPORTS
#include "ricpp/ri/ri.h"

#ifndef _RICPP_RICPP_RICPP_H
#include "ricpp/ricpp/ricpp.h"
#endif // _RICPP_RICPP_RICPP_H

#ifndef _RICPP_RI_RIINTERNAL_H
#include "ricpp/ri/riinternal.h"
#endif // _RICPP_RI_RIINTERNAL_H

using namespace RiCPP;


extern "C" {

// ----------------------------------------------------------------------------
RICPP_INTERN(RtVoid) RiBegin(RtToken name)
{
	// ERROR there is no begin/end in daynmic load procedurals (only one context)
}

RICPP_INTERN(RtVoid) RiEnd(void)
{
	// ERROR there is no begin/end in daynmic load procedurals (only one context)
}


// ----------------------------------------------------------------------------
RICPP_INTERN(RtContextHandle) RiCPPBegin(RtToken name, ...)
{
	// ERROR there is no begin/end in daynmic load procedurals (only one context)
	return 0;
}


RICPP_INTERN(RtContextHandle)
	RiCPPBeginV(RtToken name, int n, RtToken tokens[], RtPointer params[])
{
	// ERROR there is no begin/end in daynmic load procedurals (only one context)
	return 0;
}

RICPP_INTERN(RtVoid)
	RiCPPEnd (void)
{
	// ERROR there is no begin/end in daynmic load procedurals (only one context)
}

// ----------------------------------------------------------------------------
RICPP_INTERN(RtContextHandle) RiGetContext(void)
{
	// ERROR no context switching allowed in dynamic load (only one context)
	return illContextHandle;
}

RICPP_INTERN(RtVoid) RiContext(RtContextHandle h)
{
	// ERROR no context switching allowed in dynamic load (only one context)
}

}
