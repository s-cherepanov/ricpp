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
#include "ricpp/tools/stringlist.h"
#include "ricpp/tools/filepath.h"

#include <iostream>

using namespace RiCPP;

/**
 *  Interface test program
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
	ri.end();
	ri.begin("ribwriter \"$HOME/test.rib\"");
	ri.end();

	ri.errorHandler(ri.errorAbort());

	ri.option("searchpath", "renderer", ".;$PROGDIR;$PATH", RI_NULL);
	ri.begin("ribwriter");
	ri.end();
    std::cout << "Good bye, World!" << std::endl;

    return 0;
}
