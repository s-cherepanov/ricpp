#ifndef _RICPP_RICONTEXT_RIMACRO_H
#define _RICPP_RICONTEXT_RIMACRO_H

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

/** @file rimacro.h
 *  @brief Concentrates the declarations of the macro classes to record interface calls.
 *
 *  There is a class for each interface call, the classes can be used to store interface
 *  calls in memory and 'replay' them later. CRiMacro is the class to store a list of macros.
 *  The renderer base class TIntermediateRenderer manages the construction of macros while
 *  loading a RIB file.
 *
 *  @author Andreas Pidde (andreas@pidde.de)
 */

#ifndef _RICPP_RICONTEXT_RIMACROFACTORY_H
#include "ricpp/ricontext/rimacrofactory.h"
#endif // _RICPP_RICONTEXT_RIMACROFACTORY_H

#endif // _RICPP_RICONTEXT_RIMACRO_H
