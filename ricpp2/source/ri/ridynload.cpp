// Adapter ri - ricpp Adapter for dynamic load procedurals (CRiCPPBridge Instance from procedural), empty begin, end

#define RICPP_EXPORTS
#include "ricpp/ri/ri.h"

#ifndef _RICPP_RICPP_RIROOT_H
#include "ricpp/ricpp/riroot.h"
#endif // _RICPP_RICPP_RIROOT_H

#ifndef _RICPP_RI_RIINTERNAL_H
#include "ricpp/ri/riinternal.h"
#endif // _RICPP_RI_RIINTERNAL_H

using namespace RiCPP;

extern "C" {

// ----------------------------------------------------------------------------
/** @brief Implementation of the Setting of the renderer.
 *
 * The entry point for procedurals is postponed to the user who will create the dll.
 @verbatim
  RICPP_INTERN(RtPointer) ConvertParameters(char *c) {}
  RICPP_INTERN(RtVoid) Subdivide(RtPointer data, RtFloat detail) {}
  RICPP_INTERN(RtVoid) Free(RtPointer data) {}
 @endverbatim
 *
 * The dynlib must then be compiled as C++ - At least must be linked with the C++ stdlibs.
 *
 * @param ri the renderer frontend
 */
RICPP_INTERN(RtVoid) SetRenderer(IRi &ri)
{
	RiCPPRoot(&ri);
}

// ----------------------------------------------------------------------------
RICPP_INTERN(RtVoid) RiBegin(RtToken name)
{
	// ERROR there is no begin/end in dynamic load procedurals (only one context)
}

RICPP_INTERN(RtVoid) RiEnd(void)
{
	// ERROR there is no begin/end in dynamic load procedurals (only one context)
}


// ----------------------------------------------------------------------------
RICPP_INTERN(RtContextHandle) RiCPPBegin(RtToken name, ...)
{
	// ERROR there is no begin/end in dynamic load procedurals (only one context)
	return 0;
}


RICPP_INTERN(RtContextHandle)
	RiCPPBeginV(RtToken name, int n, RtToken tokens[], RtPointer params[])
{
	// ERROR there is no begin/end in dynamic load procedurals (only one context)
	return 0;
}

RICPP_INTERN(RtVoid)
	RiCPPEnd (void)
{
	// ERROR there is no begin/end in dynamic load procedurals (only one context)
}

RICPP_INTERN(RtContextHandle) RiGetContext(void)
{
	// ERROR only one context in dynamic load procedurals
	return illContextHandle;
}

RICPP_INTERN(RtVoid) RiContext(RtContextHandle handle)
{
	// ERROR only one context in dynamic load procedurals
}


} /* extern "C" */
