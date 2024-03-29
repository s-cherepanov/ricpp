// RICPP - RenderMan Interface CPP Language Binding
//         RenderMan is (R) by Pixar
//         Windows is (R) by Microsoft
//
// Copyright � 2001 - 2002, Andreas Pidde
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

/*! \file stdafx.h
 *  \brief File for precompiled headers
 *  \author Andreas Pidde (andreas@pidde.de)
 */

#include <windows.h>

// Warnings disable:
// C4097 Synonym for class name,
// C4100 unreferenced formal parameter,
// C4201 non standard nameless struct/union
// C4706 assignment in conditional expression
// #pragma warning ( disable : 4097 4100 4201 4706 )

#ifdef WIN32
#ifndef __GNUWIN32__
// Warnings disable for STL
#pragma warning(disable:4786)
// disable throw() warning
#pragma warning(disable:4290)
#endif //
#endif // WIN32

