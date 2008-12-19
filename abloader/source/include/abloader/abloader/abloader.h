#ifndef _ABLOADER_ABLOADER_ABLOADER_H
#define _ABLOADER_ABLOADER_ABLOADER_H

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

/** @file abloader.h
 *  @author Andreas Pidde (andreas@pidde.de)
 *  @brief Declaration of a context for Augenblick
 */

#include "API/Augenblick.h"

/* The classes below are not exported */
#pragma GCC visibility push(hidden)

#include <ricpp/RiCPPTriangle.h>

namespace RiCPP {
		
	/** @brief Rendering context to render using Augenblick
	 */
	class CABLoader : public CTriangleRenderer {
		
		AB::Augenblick *m_ab;
		
		RtToken RI_RENDER;
		RtToken RI_ABLOADER;

		RtToken RI_SET_MOUSE_POSITION;
		RtToken RI_QUAL_SET_MOUSE_POSITION;

		RtToken RI_MOUSE_MOVED_TO;
		RtToken RI_QUAL_MOUSE_MOVED_TO;

		RtToken RI_HIERARCHY_TYPE;
		RtToken RI_QUAL_HIERARCHY_TYPE;
		RtToken RI_BVH;
		RtToken RI_SAHBVH;
		RtToken RI_BIH;
		RtToken RI_HIERARCHY_PLUGIN;
		RtToken RI_NO_HIERARCHY;

		RtToken RI_RENDER_MODE;
		RtToken RI_QUAL_RENDER_MODE;
		RtToken RI_RAY_TRACING;
		RtToken RI_OPENGL;
	
		RtToken RI_SET_CAMERA;
		RtToken RI_QUAL_SET_CAMERA;
		
		RtToken RI_DEFAULT;
		RtToken RI_NONE;

		AB::HierarchyType m_hierarchyType;
		AB::RenderMode m_renderMode;
		bool m_defaultCamera;
		unsigned int m_geometryCounter;

		bool valid() const;
		void setABHierarchy();
		void setABOptions();
		void testScene();
	
	protected:
		typedef CTriangleRenderer TypeParent;
		
		virtual void defaultDeclarations();

		using TypeParent::hide;
		virtual void hide(const CFace &f);
		
	public:
		CABLoader();
		virtual ~CABLoader();
		
		static const char *myName();
		static const char *myType();
		static unsigned long myMajorVersion();
		static unsigned long myMinorVersion();
		static unsigned long myRevision();
		
		static RtToken myRendererType();
		
		virtual const char *name() const;
		virtual const char *type() const;
		virtual unsigned long majorVersion() const;
		virtual unsigned long minorVersion() const;
		virtual unsigned long revision() const;
		
		virtual RtToken rendererType() const;
		
		using TypeParent::doProcess;
		using TypeParent::preProcess;
		using TypeParent::postProcess;
		
		virtual RtVoid doProcess(CRiSynchronize &obj);
		virtual RtVoid doProcess(CRiControl &obj);
		
		virtual RtVoid doProcess(CRiBegin &obj);
		virtual RtVoid doProcess(CRiEnd &obj);
		
		virtual RtVoid doProcess(CRiWorldBegin &obj);
		virtual RtVoid doProcess(CRiWorldEnd &obj);
	}; // CABLoader
	
	/** @brief Creator for the CABLoader rendering context.
	 */
	class CABLoaderCreator : public CContextCreator {
	protected:
		inline virtual IRiContext *getNewContext() { return new CABLoader; }
		
	public:
		static const char *myName();
		static const char *myType();
		static unsigned long myMajorVersion();
		static unsigned long myMinorVersion();
		static unsigned long myRevision();
		
		virtual const char *name() const;
		virtual const char *type() const;
		virtual unsigned long majorVersion() const;
		virtual unsigned long minorVersion() const;
		virtual unsigned long revision() const;
		
		virtual void startup();
		virtual void shutdown();
		
		virtual unsigned long majorContextVersion() const;
		virtual unsigned long minorContextVersion() const;
		virtual unsigned long contextRevision() const;
		virtual RtToken contextName() const;
		virtual RtToken contextType() const;
		virtual RtToken rendererType() const;
	}; // CABLoaderCreator
	
} // namespace RiCPP

#pragma GCC visibility pop

#endif // _ABLOADER_ABLOADER_ABLOADER_H
