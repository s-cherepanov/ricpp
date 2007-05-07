#ifndef _RICPP_BASERENDERER_CONTEXTCREATOR_H
#define _RICPP_BASERENDERER_CONTEXTCREATOR_H

// RICPP - RenderMan(R) Interface CPP Language Binding
//
//     RenderMan(R) is a registered trademark of Pixar
// The RenderMan(R) Interface Procedures and Protocol are:
//         Copyright 1988, 1989, 2000, 2005 Pixar
//                 All rights Reservered
//
// Copyright (c) of RiCPP 2007, Andreas Pidde
// Contact: andreas@pidde.de
//
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public
// License as published by the Free Software Foundation; either
// version 2 of the License, or (at your option) any later version.
//  
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Public License for more details.
//
// You should have received a copy of the GNU General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

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

	// Don't throw
	virtual RtVoid abort(void);

	// Don't throw
	virtual RtVoid end(void);

	// Don't throw
	virtual unsigned int majorVersion(void);
}; // CContextCreator

} // namespace RiCPP

#endif // _RICPP_BASERENDERER_CONTEXTCREATOR_H
