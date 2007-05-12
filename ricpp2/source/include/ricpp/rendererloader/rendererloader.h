#ifndef _RICPP_RENDERERLOADER_RENDERERLOADER_H
#define _RICPP_RENDERERLOADER_RENDERERLOADER_H

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

/** @file rendererloader.h
 *  @author Andreas Pidde (andreas@pidde.de)
 *  @brief Declares a class to provide a renderer creator by loading from dynamic library.
 *         A RIB writer is created by instanciating CRibWriter.
 */

#ifndef _RICPP_RENDERERLOADER_RENDERERCREATOR_H
#include "ricpp/rendererloader/renderercreator.h"
#endif // _RICPP_RENDERERLOADER_RENDERERCREATOR_H

#ifndef _RICPP_GENDYNLIB_DYNLIB_H
#include "ricpp/gendynlib/dynlib.h"
#endif // _RICPP_GENDYNLIB_DYNLIB_H

#ifndef _RICPP_TOOLS_OBJPTRREGISTRY_H
#include "ricpp/tools/objptrregistry.h"
#endif // _RICPP_TOOLS_OBJPTRREGISTRY_H

namespace RiCPP {

/** @brief Provides a renderer creator.
 *
 *  The creator is loaded a dynamic library or an allocated
 *  instanciating CRibWriter.
 */
class CRendererLoader : public IRendererCreator
{
private:
	CContextCreator *m_ribWriterCreator;
	std::string m_searchpath;

	/** Class to represent a dynamic library of a renderer creator
	 */
	class CRendererLib {
		CDynLib *m_lib;
		CContextCreator *m_contextCreator;

		CLibFunc *m_newContextCreator;
		CLibFunc *m_deleteContextCreator;
		CLibFunc *m_majorInterfaceVer;
		CLibFunc *m_minorInterfaceVer;
		CLibFunc *m_interfaceRevision;
		CLibFunc *m_rendererType;
		CLibFunc *m_rendererName;

		CContextCreator *newContextCreator(unsigned long majorVersion);
		void deleteContextCreator(CContextCreator *);
		bool validDLL();

	public:
		CRendererLib(CDynLib *);
		~CRendererLib();

		bool valid();
		CContextCreator *getContextCreator(unsigned long majorVersion);

		unsigned long majorInterfaceVer();
		unsigned long minorInterfaceVer();
		unsigned long interfaceRevision();
		const char *rendererType();
		const char *rendererName();
		// Renderer is deleted in destructor with deleteRendererCreator(IRiContext *)
	};
	TObjPtrRegistry<std::string, class CRendererLib *> m_libs;

protected:
	virtual CContextCreator *getRibWriterCreator();
	virtual const char *getRibWritername();
	virtual CContextCreator *loadContextCreator(const char *name);

public:
	CRendererLoader();
	virtual ~CRendererLoader();

	virtual CContextCreator *getContextCreator(RtString name);
	inline virtual void searchpath(RtString aSearchpath)
	{
		m_searchpath = aSearchpath ? m_searchpath : "";
	}
}; // CRendererLoader

} // namespace RiCPP;

#endif // _RICPP_RENDERERLOADER_RENDERERLOADER_H
