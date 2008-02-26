// Adapter ri - ricpp Adapter for programs (CRiCPPBridge Instance), begin, end

#define RI_EXPORTS
#include "ricpp/ri/ri.h"

#ifndef _RICPP_RICPP_RICPPBRIDGE_H
#include "ricpp/ricppbridge/ricppbridge.h"
#endif // _RICPP_RICPP_RICPPBRIDGE_H

#ifndef _RICPP_RI_RIINTERNAL_H
#include "ricpp/ri/riinternal.h"
#endif // _RICPP_RI_RIINTERNAL_H

using namespace RiCPP;

static CRiCPPBridge ri; // The bridge to the rendering context

extern "C" {

// ----------------------------------------------------------------------------
RICPP_INTERN(RtVoid) RiBegin(RtToken name)
{
	// RiCPPBeginV() is defined in riprog.cpp for programs (sets _ricppRoot())
	// RiCPPBeginV() is defined as an error in ridynload.cpp
	RtContextHandle r = RiCPPBeginV(name, 0, 0, 0);
}

RICPP_INTERN(RtVoid) RiEnd(void)
{
	// RiCPPEnd() is defined in riprog.cpp for programs (clears _ricppRoot())
	// RiCPPEnd() is defined as an error in ridynload.cpp
	RiCPPEnd();
}


// ----------------------------------------------------------------------------
RICPP_INTERN(RtContextHandle) RiCPPBegin(RtToken name, ...)
{
	GETARGS(name)
	return RiCPPBeginV(name, n, tokens, params);
}

RICPP_INTERN(RtContextHandle)
RiCPPBeginV(RtToken name, int n, RtToken tokens[], RtPointer params[]) {
	if ( _ricppRoot() != 0 ) {
		// ERROR
		return illContextHandle;
	}
	_ricppRoot(&ri);
	return _ricppRoot()->beginV(name, n, tokens, params);
}

RICPP_INTERN(RtVoid)
RiCPPEnd (void) {
	if ( !_ricppRoot() ) {
		// ERROR
		return;
	}
	_ricppRoot()->end();
	_ricppRoot(0);
}

// ----------------------------------------------------------------------------
RICPP_INTERN(RtContextHandle) RiGetContext(void)
{
	if ( _ricppRoot() != &ri ) {
		return illContextHandle;
	}
	return ri.getContext();
}

RICPP_INTERN(RtVoid) RiContext(RtContextHandle h)
{
	if ( _ricppRoot() != &ri ) {
		// Error
		return;
	}
	ri.context(h);
}


// ----------------------------------------------------------------------------
// Other external functions

// ErrorHandler

// Declare

/// Options

} // extern "C"
