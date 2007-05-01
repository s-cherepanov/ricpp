#include "rendererloader/rendererloader.h"
#include "gendynlib/macdynlib.h"
#include "baserenderer/contextcreator.h"

using namespace RiCPP;

extern "C" {
typedef CContextCreator *(*TNewContextCreatorFunc)();
typedef void (*TDeleteContextCreatorFunc)(CContextCreator *);
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

CContextCreator *CRendererLoader::CRendererLib::newContextCreator() {
	if ( majorInterfaceVer() > IRiRenderer::majorVersion ) {
		return NULL;
	}
	IRiRenderer *ri = ((TNewContextCreatorFunc)((CMacLibFunc *)m_newContextCreator)->funcPtr())();
	return ri;
}
void CRendererLoader::CRendererLib::deleteContextCreator(CContextCreator *cc) {
	((TDeleteContextCreatorFunc)((CMacLibFunc *)m_deleteContextCreator)->funcPtr())(cc);
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
