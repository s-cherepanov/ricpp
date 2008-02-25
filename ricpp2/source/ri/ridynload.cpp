// Adapter ri - ricpp Adapter for dynamic load procedurals (CRiCPPBridge Instance from procedural), empty begin, end

#include "ricpp/ri/ri.h"
#include "ricpp/ricpp/ricpp.h"

using namespace RiCPP;

extern IRiRoot *_ricppRoot();
extern void _ricppRoot(IRiRoot *aRoot);

#ifdef RICPP_EXTERN
#undef RICPP_EXTERN
#endif
#define RICPP_EXTERN(atype) atype

extern "C" {

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