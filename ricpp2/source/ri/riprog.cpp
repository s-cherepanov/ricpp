// Adapter ri - ricpp Adapter for programs (CRiCPPBridge Instance), begin, end

#define RICPP_EXPORTS
#include "ricpp/ri/ri.h"

#ifndef _RICPP_RICPP_RICPPBRIDGE_H
#include "ricpp/ricppbridge/ricppbridge.h"
#endif // _RICPP_RICPP_RICPPBRIDGE_H

#ifndef _RICPP_RI_RIINTERNAL_H
#include "ricpp/ri/riinternal.h"
#endif // _RICPP_RI_RIINTERNAL_H

#include <iostream>

namespace RiCPP {

	static CRiCPPBridge *ri=0; ///< The bridge to the rendering context

	void SetRoot()
	{
		if ( RiCPPRoot() == 0 ) {
			if ( ri == 0 )
				ri = new CRiCPPBridge;
			RiCPPRoot(ri);
		}
	}

	void UnsetRoot()
	{
		RiCPPRoot(0);
		if ( ri )
			delete ri;
		ri = 0;
	}
}

using namespace RiCPP;

#if defined _MSC_VER

#if defined _MANAGED
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
	if ( ul_reason_for_call == DLL_PROCESS_ATTACH ) {
		// std::cout << "INITIALIZER" << std::endl;
		SetRoot();
	} else if ( ul_reason_for_call == DLL_PROCESS_DETACH ) {
		// std::cout << "FINALIZER" << std::endl;
		UnsetRoot();
	}
    return TRUE;
}

#if defined _MANAGED
#pragma managed(pop)
#endif

#endif // _MSC_VER

#if defined __GNUC__

extern "C" {
	
	__attribute__((constructor)) 
	static void initializer() {
		// std::cout << "INITIALIZER" << std::endl;
		SetRoot();
	}
	
	
	__attribute__((destructor)) 
	static void finalizer() { 
		UnsetRoot();
		// std::cout << "FINALIZER" << std::endl;
	} 
	
}
#endif // __GNUC__

extern "C" {
	
// ----------------------------------------------------------------------------
RICPP_INTERN(RtVoid) RiBegin(RtToken name)
{
	RiCPPInternalBeginV(name, 0, 0, 0);
}

RICPP_INTERN(RtVoid) RiEnd(void)
{
	RiCPPInternalEnd();
}


// ----------------------------------------------------------------------------
RICPP_INTERN(RtContextHandle) RiCPPBegin(RtToken name, ...)
{
	RI_GETARGS(name)
	return RiCPPInternalBeginV(name, n, tokens, params);
}

RICPP_INTERN(RtContextHandle) RiCPPBeginV(RtToken name, int n, RtToken tokens[], RtPointer params[])
{
	return RiCPPInternalBeginV(name, n, tokens, params);
}

RICPP_INTERN(RtVoid) RiCPPEnd (void)
{
	RiCPPInternalEnd();
}

	
RICPP_INTERN(RtContextHandle) RiGetContext(void)
{
	RtContextHandle h = illContextHandle;
	RI_PREAMBLE_RET(h)
		h = RiCPPRoot()->getContext();
	RI_POSTAMBLE_RET(h)
}

RICPP_INTERN(RtVoid) RiContext(RtContextHandle handle)
{
	RI_PREAMBLE
	RiCPPRoot()->context(handle);
	RI_POSTAMBLE
}

} // extern "C"
