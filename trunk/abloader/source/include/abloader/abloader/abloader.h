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

#ifndef _RICPP_BASERENDERER_TRIANGLERENDERER_H
#include "ricpp/baserenderer/trianglerenderer.h"
#endif // _RICPP_BASERENDERER_TRIANGLERENDERER_H

#ifndef _RICPP_RICONTEXT_CONTEXTCREATOR_H
#include "ricpp/ricontext/contextcreator.h"
#endif // _RICPP_RICONTEXT_CONTEXTCREATOR_H

#include "API/Augenblick.h"

namespace RiCPP {
	
#define ABLOADER_NAME "abloader"
#define ABLOADERCREATOR_NAME "abloadercreator"
	
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

		bool valid() const;
		RtVoid setABOptions();
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
		
		inline virtual const char *name() const { return myName(); }
		inline virtual const char *type() const { return myType(); }
		inline virtual unsigned long majorVersion() const { return myMajorVersion(); }
		inline virtual unsigned long minorVersion() const { return myMinorVersion(); }
		inline virtual unsigned long revision() const { return myRevision(); }
		
		inline virtual RtToken rendererType() const { return myRendererType(); }
		
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
		
		inline virtual const char *name() const { return myName(); }
		inline virtual const char *type() const { return myType(); }
		inline virtual unsigned long majorVersion() const { return myMajorVersion(); }
		inline virtual unsigned long minorVersion() const { return myMinorVersion(); }
		inline virtual unsigned long revision() const { return myRevision(); }
		
		virtual void startup();
		virtual void shutdown();
		
		inline virtual unsigned long majorContextVersion() const { return CABLoader::myMajorVersion(); }
		inline virtual unsigned long minorContextVersion() const { return CABLoader::myMinorVersion(); }
		inline virtual unsigned long contextRevision() const { return CABLoader::myRevision(); }
		inline virtual RtToken contextName() const { return CABLoader::myName(); }
		inline virtual RtToken contextType() const { return CABLoader::myType(); }
		inline virtual RtToken rendererType() const { return CABLoader::myRendererType(); }
	}; // CABLoaderCreator
	
} // namespace RiCPP

#endif // _ABLOADER_ABLOADER_ABLOADER_H
