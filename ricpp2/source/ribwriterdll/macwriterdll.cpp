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

// ribwriterdll.cpp : Defines the entry point for the DLL application.
//

#include "ribwriter/ribwriter.h"

using namespace RiCPP;
// Symbolic name for visibility("default") attribute.
#define EXPORT __attribute__((visibility("default")))

extern "C" {

EXPORT
CContextCreator *newContextCreator() {
	return new CRibWriterCreator;
}

EXPORT
void deleteContextCreator(CContextCreator *cc) {
	if ( cc )
		delete cc;
}

EXPORT
unsigned long majorInterfaceVer() {
	return CRibWriter::majorVersion;
}

EXPORT
unsigned long minorInterfaceVer() {
	return CRibWriter::ribWriterMinorVersion;
}

EXPORT
RtToken rendererType() {
	return CRibWriter::myRendererType();
}

EXPORT
RtToken rendererName() {
	return CRibWriter::myRendererName();
}

}