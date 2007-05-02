#ifndef _RICPP_TOOLS_GETTEMP_H
#define _RICPP_TOOLS_GETTEMP_H

// RICPP - RenderMan Interface CPP Language Binding
//         RenderMan is (R) by Pixar
//         Windows is (R) by Microsoft
//
// Copyright © 2001 - 2002, Andreas Pidde
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

/*! \file gettemp.h
 *  \brief Function to get the path to the directory for temporary files
 *  \author Andrew Chapman 
 *          Andreas Pidde (andreas@pidde.de)
 */

#include <stdlib.h> //!< Included for getenv()

//! Get the path to the directory for temporary files by using the TEMP or TMP environment variable
/* \return The path to the directory for temporary files
 */
inline const char *getTemp() {
	const char *str = getenv("TEMP");
	if ( !(str || *str) )
		str = getenv("TMP");
#	ifdef WIN32
		return str ? str : "C:\\TEMP";
#	else
		return str ? str : "/usr/tmp";
#	endif
}

#endif // _RICPP_TOOLS_GETTEMP_H
