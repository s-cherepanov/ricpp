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
 *  \brief Implementation of TPathStack, handles a stack of directory names to handle relative
 *         pathes
 *  \author Andreas Pidde (andreas@pidde.de)
 */

#include "stdafx.h"
#include "pathstack.h"
#ifdef WIN32
#  include <direct.h>
#endif // WIN32

////////////////////////////////////////////////////////////////////////////////
//! Returns the last (current) working directory used.
/*! The working directory is the directory of the last loaded RIB file or
 *  the systems CWD if there is no RIB file. \\ are translated to /.
 *  \retval path The last working directory used.
 */
void TPathStack::currentPath(std::string &path) const
{
	if ( m_pathes.empty() ) {
		// There are no RIB files loaded, so take the system
		// current working directory
		char buffer[_MAX_PATH+1];
		buffer[_MAX_PATH] = '\0';
		char *cwd = _getcwd(buffer, _MAX_PATH);

		// ? no CWD - take local path
		if ( !cwd ) {
			path = "";
			return;
		}

		// translate \ to /
		while ( *cwd ) {
			if ( *cwd == '\\' )
				*cwd = '/';
			++cwd;
		}
		path = buffer;
		return;
	}

	// return the path of the last loaded RIB file
	path = m_pathes.back();
}


////////////////////////////////////////////////////////////////////////////////
//!  Expand path to a complete path with respect to currentPath().
/*! \path The path to expand.
 */
void TPathStack::expandPath(std::string &path) const {
	// if absolute path, don't change it
	if ( (path[1] == ':' && path[2] == '/') || (path[0]=='/' && path[1]=='/') )
		return;

	if ( (path[1] == ':' && path[2] == '\\') || (path[0]=='\\' && path[1]=='\\') )
		return;

	// Get current working path
	std::string curPath;
	currentPath(curPath);

	// No path do nothing
	if ( curPath.empty() )
		return;

	// if relative path, add the last used path
	if ( path[0] != '/' && path[0] != '\\' ) {
		curPath.append("/");
		curPath.append(path);
		path    = curPath;
	} else {
		if ( (curPath[1] == ':' && curPath[2] == '/') || (curPath[1] == ':' && curPath[2] == '\\')  ) {
			// ... add drive, like C:, D: and so on
			char drive[3];
			drive[0] = curPath[0];
			drive[1] = curPath[1];
			drive[2] = '\0';
			path = drive + path;
		} else if ( (curPath[0]=='/' && curPath[1]=='/') && (curPath[0]=='\\' && curPath[1]=='\\') ) {
			// ... add UNC name: //computername
			std::string unc = "//";
			unsigned int i=2;
			while ( i < curPath.length() && (curPath[i] != '/' && curPath[i] != '\\') ) {
				unc = curPath[i];
				++i;
			}
			path = unc + path;
		}
	}
}

////////////////////////////////////////////////////////////////////////////////
//! Expand the directory name of a path.
/*! Same as expandPath(), but remove the filename.
 * \path The path to expand.
 */
void TPathStack::expandDirectory(std::string &path) const {
	expandPath(path);
	int pos = (int)path.find_last_of('\\');
	int pos2 = (int)path.find_last_of('/');
	if ( pos < pos2 ) {
		pos = pos2;
	}
	if ( pos < (int)path.length() ) {
		path.erase(pos, (int)path.length()-pos);
	}
	path.resize(pos);
}
