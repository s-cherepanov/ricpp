// ribwriterdll.cpp : Defines the entry point for the DLL application.
//

#include "ribwriter/ribwriter.h"
#include <windows.h>

using namespace RiCPP;

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
					 )
{
    return TRUE;
}

extern "C" __declspec ( dllexport ) IRiRenderer * CDECL newRenderer() {
	return new CRibWriter;
}

extern "C" __declspec ( dllexport ) void CDECL deleteRenderer(IRiRenderer *ri) {
	if ( ri )
		delete ri;
}

extern "C" __declspec ( dllexport ) unsigned long CDECL majorInterfaceVer() {
	return CRibWriter::majorVersion;
}

extern "C" __declspec ( dllexport ) unsigned long CDECL minorInterfaceVer() {
	return CRibWriter::minorVersion;
}

extern "C" __declspec ( dllexport ) RtToken CDECL rendererType() {
	return CRibWriter::myRendererType();
}

extern "C" __declspec ( dllexport ) RtToken CDECL rendererName() {
	return CRibWriter::myRendererName();
}
