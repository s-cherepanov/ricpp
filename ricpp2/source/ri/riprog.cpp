// Adapter ri - ricpp Adapter for programs (CRiCPPBridge Instance), begin, end

#define RI_EXPORTS
#include "ricpp/ri/ri.h"

#ifndef _RICPP_RICPP_RICPPBRIDGE_H
#include "ricpp/ricppbridge/ricppbridge.h"
#endif // _RICPP_RICPP_RICPPBRIDGE_H

#ifndef _RICPP_RI_RIINTERNAL_H
#include "ricpp/ri/riinternal.h"
#endif // _RICPP_RI_RIINTERNAL_H

namespace RiCPP {

	static CRiCPPBridge ri; // The bridge to the rendering context

	void SetRoot()
	{
		if ( RiCPPRoot() == 0 ) {
			RiCPPRoot(&ri);
		}
	}

}

using namespace RiCPP;

#ifdef _WIN32

#ifdef _MANAGED
#pragma managed(push, off)
#endif

/** @fn BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
 *  @brief Entry point for the DLL.
 *  @param hModule
 *  @param ul_reason_for_call
 *  @param lpReserved
 *  @return TRUE
 */
BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD   ul_reason_for_call,
                       LPVOID  lpReserved
					 )
{
	// If there is something alike at Apple: SetRoot() can be done here
    return TRUE;
}

#ifdef _MANAGED
#pragma managed(pop)
#endif

#endif // WIN32

extern "C" {

// ----------------------------------------------------------------------------
RICPP_INTERN(RtVoid) RiBegin(RtToken name)
{
	SetRoot();
	RiCPPInternalBeginV(name, 0, 0, 0);
}

RICPP_INTERN(RtVoid) RiEnd(void)
{
	SetRoot();
	RiCPPInternalEnd();
}


// ----------------------------------------------------------------------------
RICPP_INTERN(RtContextHandle) RiCPPBegin(RtToken name, ...)
{
	SetRoot();
	GETARGS(name)
	return RiCPPInternalBeginV(name, n, tokens, params);
}

RICPP_INTERN(RtContextHandle) RiCPPBeginV(RtToken name, int n, RtToken tokens[], RtPointer params[])
{
	SetRoot();
	return RiCPPInternalBeginV(name, n, tokens, params);
}

RICPP_INTERN(RtVoid) RiCPPEnd (void)
{
	SetRoot();
	RiCPPInternalEnd();
}

	
RICPP_INTERN(RtContextHandle) RiGetContext(void)
{
	RtContextHandle h = illContextHandle;
	SetRoot();
	PREAMBLE_RET(h)
		h = RiCPPRoot()->getContext();
	POSTAMBLE_RET(h)
}

RICPP_INTERN(RtVoid) RiContext(RtContextHandle handle)
{
	SetRoot();
	PREAMBLE
	RiCPPRoot()->context(handle);
	POSTAMBLE
}

// ----------------------------------------------------------------------------

RICPP_INTERN(RtVoid) RiErrorHandler(RtErrorHandler handler)
{
	SetRoot();
	RiCPPInternalErrorHandler(handler);
}

// ----------------------------------------------------------------------------

RICPP_INTERN(RtVoid) RiControl (char *name, ...)
{
	SetRoot();
	GETARGS(name)
	RiCPPInternalControlV(name, n, tokens, params);
}

RICPP_INTERN(RtVoid) RiControlV (char *name, RtInt n, RtToken tokens[], RtPointer params[])
{
	SetRoot();
	RiCPPInternalControlV(name, n, tokens, params);
}

} // extern "C"
