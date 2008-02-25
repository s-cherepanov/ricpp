// Adapter ri - ricpp Adapter for programs (CRiCPPBridge Instance), begin, end

#include "ricpp/ricppbridge/ricppbridge.h"
#include "ricpp/ri/ri.h"

using namespace RiCPP;

extern IRiRoot *riRoot;
CRiCPPBridge ri;    ///< The bridge to the rendering context

#ifdef RICPP_EXTERN
#undef RICPP_EXTERN
#endif
#define RICPP_EXTERN(atype) atype

extern "C" {

RICPP_EXTERN(RtContextHandle)
RiCPPBeginV(RtToken name, int n, RtToken tokens[], RtPointer params[]) {
	if ( riRoot != 0 ) {
		// ERROR
		return illContextHandle;
	}
	riRoot = &ri;
	return riRoot->beginV(name, n, tokens, params);
}

RICPP_EXTERN(RtVoid)
RiEnd (void) {
	if ( !riRoot ) {
		// ERROR
		return;
	}
	riRoot->end();
	riRoot = 0;
}



// Other external functions

// ErrorHandler

// Declare

/// Options

} // extern "C"
