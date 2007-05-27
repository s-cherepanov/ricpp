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

/** @file main.cpp
 *  @author Andreas Pidde (andreas@pidde.de)
 *  @brief Used to test.
 */

#include "ricpp/ricppbridge/ricppbridge.h"
#include "ricpp/tools/filepath.h"

#include <iostream>

using namespace RiCPP;

/** @brief Interface test program
 */
int main (int argc, char * const argv[]) {
	CRiCPPBridge ri;
	std::cout << "Hello, World!" << std::endl;

	std::string str;
	std::cout << CEnv::tmpName() << ": " << CEnv::find(str, CEnv::tmpName()) << std::endl;
	std::cout << CEnv::homeName() << ": " << CEnv::find(str, CEnv::homeName()) << std::endl;
	std::cout << CEnv::pathName() << ": "  << CEnv::find(str, CEnv::pathName()) << std::endl;
	std::cout << CEnv::progDirName() << ": "  << CEnv::find(str, CEnv::progDirName()) << std::endl;

	ri.errorHandler(ri.errorPrint());

	ri.begin("test.rib");
	RtContextHandle ch1 = ri.getContext();
	ri.end();

	RtContextHandle ch2 = ri.begin("ribwriter", RI_FILE, "$HOME/test.rib", RI_NULL);
		ri.begin("test2.rib");
			ri.declare("surface:tile", "    constant    float "); // syntax error
			ri.declare("surface:tile:anarray", "    constant    float  [19] ");
			ri.declare("bla", "fdfw[19]ss"); // Error
			RtContextHandle ch3 = ri.getContext();
			ri.context(illContextHandle);
			ri.clipping(0, 1); /* Error */
			// Swapping contexts
			ri.context(ch2);
		ri.end();
		ri.context(ch3);
		ri.frameBegin(1);
		ri.frameEnd();
		ri.frameEnd(); /** Error **/
	ri.end();

	ri.frameEnd(); /* Error */
	ri.end(); /* Error */
	ri.context(ch1); /* Error */

	/* not existant */
	ri.begin("bla");
	ri.worldBegin();
	ri.worldEnd();
	ri.end();

	std::string name;
	name.reserve(200);
	for ( int i = 1; i < 100; ++i ) {
		ri.begin(RI_NULL);
			name = "name";
			for ( int j = 1; j < 100; ++j ) {
				ri.declare(name.c_str(), "    constant    string  [19] ");
				name += ".";
			}
			ri.frameBegin((RtInt)i);
				ri.worldBegin();
				ri.worldEnd();
			ri.frameEnd();
		ri.end();
	}
	name.clear();
	name.resize(0);
	ri.errorHandler(ri.errorAbort());

	/* wrong lib */
	ri.begin("user32");
	ri.worldBegin();
	ri.worldEnd();
	ri.end();

	for ( int i = 1; i < 1000; ++i ) {
		ri.begin(RI_NULL);
		ri.end();
	}

	ri.option("searchpath", "renderer", ".;$PROGDIR;$PATH", RI_NULL);
	ri.begin("ribwriter");
	ri.end();
    std::cout << "Good bye, World!" << std::endl;

    return 0;
}
