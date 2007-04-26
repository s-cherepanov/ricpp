#include "rendererloader/rendererloader.h"
#include "gendynlib/win32dynlib.h"
#include "baserenderer/rirenderer.h"

using namespace RiCPP;

extern "C" {
typedef IRiRenderer *(CDECL *TNewRendererFunc)();
typedef void (CDECL *TDeleteRendererFunc)(IRiRenderer *ri);
typedef unsigned long (CDECL *TMajorInterfaceVerFunc)();
typedef unsigned long (CDECL *TMinorInterfaceVerFunc)();
typedef const char *(CDECL *TRendererTypeFunc)();
typedef const char *(CDECL *TRendererNameFunc)();
}

bool CRendererLoader::CRendererLib::validDLL() {
	if ( !m_lib )
		return false;
	if ( strcmp(m_lib->className(), CWin32DynLib::staticClassName()) != 0 )
		return false;
	return true;
}

IRiRenderer *CRendererLoader::CRendererLib::newRenderer() {
	if ( majorInterfaceVer() > IRiRenderer::majorVersion ) {
		return NULL;
	}
	IRiRenderer *ri = ((TNewRendererFunc)((CWin32LibFunc *)m_newRenderer)->funcPtr())();
	return ri;
}
void CRendererLoader::CRendererLib::deleteRenderer(IRiRenderer *ri) {
	((TDeleteRendererFunc)((CWin32LibFunc *)m_deleteRenderer)->funcPtr())(ri);
}

unsigned long CRendererLoader::CRendererLib::majorInterfaceVer() {
	return ((TMajorInterfaceVerFunc)((CWin32LibFunc *)m_majorInterfaceVer)->funcPtr())();
}

unsigned long CRendererLoader::CRendererLib::minorInterfaceVer() {
	return ((TMinorInterfaceVerFunc)((CWin32LibFunc *)m_minorInterfaceVer)->funcPtr())();
}

const char *CRendererLoader::CRendererLib::rendererType() {
	return ((TRendererTypeFunc)((CWin32LibFunc *)m_rendererType)->funcPtr())();
}

const char *CRendererLoader::CRendererLib::rendererName() {
	return ((TRendererNameFunc)((CWin32LibFunc *)m_rendererName)->funcPtr())();
}
