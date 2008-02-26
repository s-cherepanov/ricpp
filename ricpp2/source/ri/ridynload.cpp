// Adapter ri - ricpp Adapter for dynamic load procedurals (CRiCPPBridge Instance from procedural), empty begin, end
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
RICPP_EXTERN(RtVoid) RiBegin(RtToken name)
{
	// ERROR there is no begin/end in daynmic load procedurals (only one context)
}

RICPP_EXTERN(RtVoid) RiEnd(void)
{
	// ERROR there is no begin/end in daynmic load procedurals (only one context)
}


// ----------------------------------------------------------------------------
RICPP_EXTERN(RtContextHandle) RiCPPBegin(RtToken name, ...)
{
	// ERROR there is no begin/end in daynmic load procedurals (only one context)
	return 0;
}


RICPP_EXTERN(RtContextHandle)
	RiCPPBeginV(RtToken name, int n, RtToken tokens[], RtPointer params[])
{
	// ERROR there is no begin/end in daynmic load procedurals (only one context)
	return 0;
}

RICPP_EXTERN(RtVoid)
	RiCPPEnd (void)
{
	// ERROR there is no begin/end in daynmic load procedurals (only one context)
}

// ----------------------------------------------------------------------------
RICPP_EXTERN(RtContextHandle) RiGetContext(void)
{
	// ERROR no context switching allowed in dynamic load (only one context)
	return illContextHandle;
}

RICPP_EXTERN(RtVoid) RiContext(RtContextHandle h)
{
	// ERROR no context switching allowed in dynamic load (only one context)
}

}
