#include "rendererloader/rendererloader.h"
#include "gendynlib/macdynlib.h"
#include "baserenderer/rirenderer.h"

using namespace RiCPP;

extern "C" {
typedef IRiRenderer *(*TNewRendererFunc)();
typedef void (*TDeleteRendererFunc)(IRiRenderer *ri);
typedef unsigned long (*TMajorInterfaceVerFunc)();
typedef unsigned long (*TMinorInterfaceVerFunc)();
typedef const char *(*TRendererTypeFunc)();
typedef const char *(*TRendererNameFunc)();
}

bool CRendererLoader::CRendererLib::validDLL() {
	if ( !m_lib )
		return false;
	if ( strcmp(m_lib->className(), CMacDynLib::staticClassName()) != 0 )
		return false;
	return true;
}

IRiRenderer *CRendererLoader::CRendererLib::newRenderer() {
	if ( majorInterfaceVer() > IRiRenderer::majorVersion ) {
		return NULL;
	}
	IRiRenderer *ri = ((TNewRendererFunc)((CMacLibFunc *)m_newRenderer)->funcPtr())();
	return ri;
}
void CRendererLoader::CRendererLib::deleteRenderer(IRiRenderer *ri) {
	((TDeleteRendererFunc)((CMacLibFunc *)m_deleteRenderer)->funcPtr())(ri);
}

unsigned long CRendererLoader::CRendererLib::majorInterfaceVer() {
	return ((TMajorInterfaceVerFunc)((CMacLibFunc *)m_majorInterfaceVer)->funcPtr())();
}

unsigned long CRendererLoader::CRendererLib::minorInterfaceVer() {
	return ((TMinorInterfaceVerFunc)((CMacLibFunc *)m_minorInterfaceVer)->funcPtr())();
}

const char *CRendererLoader::CRendererLib::rendererType() {
	return ((TRendererTypeFunc)((CMacLibFunc *)m_rendererType)->funcPtr())();
}

const char *CRendererLoader::CRendererLib::rendererName() {
	return ((TRendererNameFunc)((CMacLibFunc *)m_rendererName)->funcPtr())();
}
