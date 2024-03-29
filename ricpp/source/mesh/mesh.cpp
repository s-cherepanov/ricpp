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

/*! \file mesh.cpp
 *  \brief RenderMan output as mesh
 *  \author Andreas Pidde (andreas@pidde.de)
 */

#include "stdafx.h"
#include "mesh.h"

#include "libmesh/libmesh.h"

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

// #include "../../../context3d/logfile.h"
// TLogfile logfile("c:\\temp\\glrenderer.log", "TGLRenderer");

int WINAPI DllMain(HINSTANCE hInstance, DWORD fdwReason, PVOID pvReserved) {
	// setlocale( LC_ALL, "C" );
	/*
	if ( fdwReason == DLL_PROCESS_DETACH ) {
		logfile.ReportLogEnd("TGLRenderer");
	} else if ( fdwReason == DLL_PROCESS_ATTACH ) {
		logfile.ReportLogStart("TGLRenderer");
	}
	*/
	return TRUE;
}

extern "C" __declspec ( dllexport ) TRi * CDECL GetRenderer() {
	TMesh *renderer = new TMesh;
	return renderer;
}

extern "C" __declspec ( dllexport ) void CDECL DeleteRenderer(TRi *renderer) {
	if ( renderer )
		delete (TMesh *)renderer;
}
