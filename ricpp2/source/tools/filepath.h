#ifndef _RICPP_TOOLS_FILEPATH_H
#define _RICPP_TOOLS_FILEPATH_H

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
#include <string>

namespace RiCPP {

	class CFilepathConverter {
	public:
		inline static char internalPathSeperator() {return '/';}
		inline static char internalPathlistSeperator() {return ';';}
		static char nativePathSeperator();
		static char nativePathlistSeperator();
		static std::string &convertToInternal(std::string &var);
		static std::string &convertToNative(std::string &var);
	}; // CFilepathConverter

	class CFilepath {
		std::string m_filepath;
		std::string m_nativepath;
		std::string m_fullpath;
		void convertToNative();
	public:
		inline CFilepath():m_filepath("") { convertToNative(); }
		inline CFilepath(const char *aFilepath):m_filepath(aFilepath ? aFilepath : "") { convertToNative(); }
		inline const char *filepath() const { return m_filepath.c_str(); }
		inline const char *nativepath() const { return m_nativepath.c_str(); }
		inline const char *fullpath() const { return m_fullpath.c_str(); }
		bool isAbsolute() const;
		inline bool isRelative() const { return !isAbsolute(); }
	}; // CFilepath
}; // namespace RiCPP

#endif // _RICPP_TOOLS_FILEPATH_H
