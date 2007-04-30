#ifndef _RICPP_GENDYNLIB_DYNLIB_H
#define _RICPP_GENDYNLIB_DYNLIB_H

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

#ifndef _RICPP_TOOLS_STRINGLIST_H
#include "tools/stringlist.h"
#endif // _RICPP_TOOLS_STRINGLIST_H

#include <string>

namespace RiCPP {

	class CLibFunc {
	public:
		virtual ~CLibFunc();
		virtual bool valid()=0;
	}; // CLibFunc

	class CDynLib {
		unsigned long m_useCount;

	protected:
		std::string m_libname;
		std::string m_libpath;
		CStringList m_searchpath;

		virtual bool doLoad() = 0;
		virtual bool doUnload() = 0;

	public:
		CDynLib(const char *libname, const char *searchpath);
		virtual ~CDynLib();

		const char *libname() const;
		const char *libpath();
		bool load();
		bool unload();
		unsigned long useCount() const;

		virtual const char *className() const = 0;
		virtual bool isLoaded() = 0;
		virtual bool valid() const;
		virtual const char *findLib() = 0;
		virtual CLibFunc *getFunc(const char *name) const = 0;
		virtual void deleteFunc(CLibFunc *f) const;
	}; // CDynLib


	class CDynLibFactory {
		public:
			static CDynLib *newDynLib(const char *libname, const char *searchpath);
			static void deleteDynLib(CDynLib *lib);
	}; // CDynLibFactory

} // namespace RiCPP

#endif // _RICPP_GENDYNLIB_DYNLIB_H
