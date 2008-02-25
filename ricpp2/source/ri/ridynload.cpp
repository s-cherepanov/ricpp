// Adapter ri - ricpp Adapter for dynamic load procedurals (CRiCPPBridge Instance from procedural), empty begin, end

#include "ricpp/ri/ri.h"
#include "ricpp/ricpp/ricpp.h"

using namespace RiCPP;

extern IRiRoot *riRoot;

#ifdef RICPP_EXTERN
#undef RICPP_EXTERN
#endif
#define RICPP_EXTERN(atype) extern "C" atype
