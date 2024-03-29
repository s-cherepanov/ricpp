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

/** @file liunxpath.cpp
 *  @author Andreas Pidde (andreas@pidde.de)
 *  @brief Linux implementation of the file path classes (same as Mac)
 */

#if !(defined _WIN32) && !(defined __APPLE__)

#include "ricpp/tools/filepath.h"

using namespace RiCPP;

/** @brief Dynamic libraries have the suffix ".so" at linux systems
 */
const char *CFilepathConverter::nativeDynlibSuffix() { return ".so"; }

#endif // !_WIN32 && !__APPLE__
