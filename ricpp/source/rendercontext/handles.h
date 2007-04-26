#ifndef _RICPP_RENDERCONTEXT_HANDLES_H
#define _RICPP_RENDERCONTEXT_HANDLES_H

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

/*! \file handles.h
 *  \brief Slots for light and object handles for the render contexts of RiCPP.
 *
 *         E.g. the parser connect a light handle number of a Rib file with it's
 *         light handle used by the renderer using an instance of TLightHandle together
 *         with a TRiLightSource.
 *  \author Andreas Pidde (andreas@pidde.de)
 */

#include "rendererloader/ricpp.h"

//! Store an object handle
class TObjectHandle {
public:
	long m_magic; //!< For debugging purposes
	RtObjectHandle m_handle; //! The object handle
	inline TObjectHandle() : m_magic(12345), m_handle((RtObjectHandle)-1) {} //!< Initialize with an empty handle
	inline TObjectHandle(RtObjectHandle handle) : m_magic(12345), m_handle(handle) {} //!< Initialize with a handle
}; // TObjectHandle

//! Store a light handle
class TLightHandle {
public:
	long m_magic; //!< For debugging purposes
	RtLightHandle m_handle; //! The light handle
	inline TLightHandle() : m_magic(12345), m_handle((RtLightHandle)-1) {} //!< Initialize with an empty handle
	inline TLightHandle(RtLightHandle handle) : m_magic(12345), m_handle(handle) {} //!< Initialize with a handle
}; // TLightHandle

//! Store a context handle
class TContextHandle {
public:
	long m_magic; //!< For debugging purposes
	RtContextHandle m_handle; //! The context handle
	inline TContextHandle() : m_magic(12345), m_handle((RtContextHandle)-1) {} //!< Initialize with an empty handle
	inline TContextHandle(RtContextHandle handle) : m_magic(12345), m_handle(handle) {} //!< Initialize with a handle
}; // TContextHandle

#endif // _RICPP_RENDERCONTEXT_HANDLES_H
