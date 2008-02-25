// Adapter ri - ricpp Common for programs and dynamic load procedurals, external IRi

#include "ricpp/ri/ri.h"

#ifndef _RICPP_RICPP_RICPP_H
#include "ricpp/ricpp/ricpp.h"
#endif // _RICPP_RICPP_RICPPT_H

using namespace RiCPP;

IRiRoot *riRoot = 0; /// The RiCPP interface

#ifdef RICPP_EXTERN
#undef RICPP_EXTERN
#endif
#define RICPP_EXTERN(atype) atype

extern "C" {

RICPP_EXTERN(RtVoid)
	RiBegin (RtToken name)
{
	RtContextHandle r = RiCPPBeginV(name, 0, 0, 0);
}

} // extern "C"
