// ribwriterdll.cpp : Defines the entry point for the DLL application.
//

#include "ribwriter/ribwriter.h"

using namespace RiCPP;
// Symbolic name for visibility("default") attribute.
#define EXPORT __attribute__((visibility("default")))

extern "C" {

EXPORT
CContextCreator *newContextCreator() {
	return new CRibWriterCreator;
}

EXPORT
void deleteContextCreator(CContextCreator *cc) {
	if ( cc )
		delete cc;
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