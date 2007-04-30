// ribwriterdll.cpp : Defines the entry point for the DLL application.
//

#include "ribwriter/ribwriter.h"

using namespace RiCPP;
// Symbolic name for visibility("default") attribute.
#define EXPORT __attribute__((visibility("default")))

extern "C" {

EXPORT
IRiRenderer *newRenderer() {
	return new CRibWriter;
}

EXPORT
void deleteRenderer(IRiRenderer *ri) {
	if ( ri )
		delete ri;
}

EXPORT
unsigned long majorInterfaceVer() {
	return CRibWriter::majorVersion;
}

EXPORT
unsigned long minorInterfaceVer() {
	return CRibWriter::minorVersion;
}

EXPORT
RtToken rendererType() {
	return CRibWriter::myRendererType();
}

EXPORT
RtToken rendererName() {
	return CRibWriter::myRendererName();
}

}