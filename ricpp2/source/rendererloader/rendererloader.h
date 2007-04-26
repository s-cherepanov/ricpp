#ifndef _RICPP_RENDERERLOADER_RENDERERLOADER_H
#define _RICPP_RENDERERLOADER_RENDERERLOADER_H

// RICPP - RenderMan(R) Interface CPP Language Binding
//
//     RenderMan(R) is a registered trademark of Pixar
// The RenderMan(R) Interface Procedures and Protocol are:
//         Copyright 1988, 1989, 200,, 2005 Pixar
//                 All rights Reservered
//
// Copyright © of RiCPP 2007, Andreas Pidde
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

#ifndef _RICPP_RICPP_RENDERERCREATOR_H
#include "ricpp/renderercreator.h"
#endif // _RICPP_RICPP_RENDERERCREATOR_H

#ifndef _RICPP_GENDYNLIB_DYNLIB_H
#include "gendynlib/dynlib.h"
#endif // _RICPP_GENDYNLIB_DYNLIB_H

#ifndef _RICPP_TOOLS_OBJPTRREGISTRY_H
#include "tools/objptrregistry.h"
#endif // _RICPP_TOOLS_OBJPTRREGISTRY_H

namespace RiCPP {

class CRendererLoader : public IRendererCreator
{
private:
	IRiRenderer *m_ribWriter;
	std::string m_searchpath;

	class CRendererLib {
		CDynLib *m_lib;
		IRiRenderer *m_renderer;

		CLibFunc *m_newRenderer;
		CLibFunc *m_deleteRenderer;
		CLibFunc *m_majorInterfaceVer;
		CLibFunc *m_minorInterfaceVer;
		CLibFunc *m_rendererType;
		CLibFunc *m_rendererName;

		IRiRenderer *newRenderer();
		void deleteRenderer(IRiRenderer *);
		bool validDLL();
	public:
		CRendererLib(CDynLib *);
		~CRendererLib();

		bool valid();
		IRiRenderer *getRenderer();

		unsigned long majorInterfaceVer();
		unsigned long minorInterfaceVer();
		const char *rendererType();
		const char *rendererName();
		// Renderer is deleted in destructor with deleteRenderer(IRiRenderer *)
	};
	CObjPtrRegistry<std::string, class CRendererLib *> m_libs;

protected:
	virtual IRiRenderer *getRibWriter();

public:
	CRendererLoader();
	virtual ~CRendererLoader();

	virtual IRiRenderer *beginRenderer(RtString name);
	virtual RtVoid endRenderer(IRiRenderer *renderer);
	virtual RtVoid abortRenderer(IRiRenderer *renderer);
	virtual RtVoid doOptionV(RtString name, RtInt n, RtToken tokens[], RtPointer params[]);
}; // CRendererLoader

} // namespace RiCPP;

#endif // _RICPP_RENDERERLOADER_RENDERERLOADER_H
