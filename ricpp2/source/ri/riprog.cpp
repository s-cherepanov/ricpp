// Adapter ri - ricpp Adapter for programs (CRiCPPBridge Instance), begin, end
#define RICPP_CREATEDLL
#include "ricpp/ricppbridge/ricppbridge.h"
#include "ricpp/ri/ri.h"

using namespace RiCPP;

extern IRiRoot *_ricppRoot();
extern void _ricppRoot(IRiRoot *aRoot);

static CRiCPPBridge ri; // The bridge to the rendering context

extern "C" {

// ----------------------------------------------------------------------------
RICPP_EXTERN(RtContextHandle)
RiCPPBeginV(RtToken name, int n, RtToken tokens[], RtPointer params[]) {
	if ( _ricppRoot() != 0 ) {
		// ERROR
		return illContextHandle;
	}
	_ricppRoot(&ri);
	return _ricppRoot()->beginV(name, n, tokens, params);
}

RICPP_EXTERN(RtVoid)
RiCPPEnd (void) {
	if ( !_ricppRoot() ) {
		// ERROR
		return;
	}
	_ricppRoot()->end();
	_ricppRoot(0);
}

// ----------------------------------------------------------------------------
RICPP_EXTERN(RtContextHandle) RiGetContext(void)
{
	if ( _ricppRoot() != &ri ) {
		return illContextHandle;
	}
	return ri.getContext();
}

RICPP_EXTERN(RtVoid) RiContext(RtContextHandle h)
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
