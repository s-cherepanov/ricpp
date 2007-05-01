#ifndef _RICPP_BASERENDERER_CONTEXTCREATOR_H
#define _RICPP_BASERENDERER_CONTEXTCREATOR_H

#ifndef _RICPP_RICPP_RENDERERERROR_H
#include "ricpp/renderererror.h"
#endif // _RICPP_RICPP_RENDERERERROR_H

#ifndef _RICPP_BASERENDERER_RICONTEXT_H
#include "baserenderer/ricontext.h"
#endif // _RICPP_BASERENDERER_RICONTEXT_H

#include <list>

namespace RiCPP {

// Context creation and switching
class CContextCreator {
	std::list<IRiContext *> m_contextList;
	IRiContext *m_curContext;
protected:
	// Factory method
	virtual IRiContext *getNewContext();

	virtual void deleteContext();
public:
	CContextCreator();
	~CContextCreator();

	virtual IRiContext *getContext();

	// Used also to notify the context switching, e.g. for OpenGL
	virtual RtVoid context(IRiContext *context);

	virtual RtVoid begin(RtString name);
	virtual RtVoid abort(void);
	virtual RtVoid end(void);
}; // CContextCreator

} // namespace RiCPP

#endif // _RICPP_BASERENDERER_CONTEXTCREATOR_H
