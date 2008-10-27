#ifndef _RICPP_RENDERCONTEXT_PATHSTACK_H
#define _RICPP_RENDERCONTEXT_PATHSTACK_H

// RICPP - RenderMan Interface CPP Language Binding
//         RenderMan is (R) by Pixar
//         Windows is (R) by Microsoft
//
// Copyright © 2003, Andreas Pidde
//
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

/*! \file pathstack.h
 *  \brief Declaration of TPathStack, handles a stack of directory names to handle relative
 *         pathes
 *  \author Andreas Pidde (andreas@pidde.de)
 */
#include <string>
#include <vector>

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//! Stack of file pathes
/*! Stack of current pathes, used for local pathes relative
 * to the last loaded RIB file (readArchive)
 * pathes in RenderMan interface uses / instead of \\ as
 * path seperator for compatibility with other file systems
 */
class TPathStack {
	//! Vector of file pathes
	std::vector<std::string> m_pathes;
public:
	//! Empty standard constructor
	inline TPathStack() {}

	//! Pushes a new relative path.
	/*! \param path The path to push on the stack, the path will be expanded.
	 */
	inline void push(const char *path) {
		std::string newPath = path;
		expandDirectory(newPath);
		m_pathes.push_back(newPath);
	}

	//! Pops a path.
	/*! \return true if a path could be poped, false if the stack was empty.
	 */
	inline bool pop() { 
		if ( m_pathes.empty() )
			return false;
		m_pathes.pop_back();
		return true;
	}

	//! Clear the pathes.
	inline void clear() {
		m_pathes.clear();
	}

	void expandPath(std::string &path) const;
	void expandDirectory(std::string &path) const;
	void currentPath(std::string &path) const;
};

#endif // _RICPP_RENDERCONTEXT_PATHSTACK_H

