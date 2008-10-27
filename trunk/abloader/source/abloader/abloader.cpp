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

#pragma GCC visibility push(hidden)

#include "abloader/abloader/abloader.h"

// #define _TRACE
#ifndef _ABLOADER_TOOLS_TRACE
#include "abloader/tools/trace.h"
#endif // _ABLOADER_TOOLS_TRACE

#ifndef _RICPP_RICONTEXT_RIMACRO_H
#include "ricpp/ricontext/rimacro.h"
#endif // _RICPP_RICONTEXT_RIMACRO_H

#include <Shader/Fresnel.h>
#include <Shader/Phong.h>
#include <Shader/Reflect.h>
#include <Core/Vector3.h>

#pragma GCC visibility pop

using namespace RiCPP;
using namespace AB;

static const bool _USESTRIPS = false;
static const HierarchyType _DEF_HIERARCHY = BVH; // BVH SAHBVH
static const RenderMode _DEF_RENDERMODE = RAY_TRACING;
static bool _DEF_DEFAULT_CAMERA = true;

// -----------------------------------------------------------------------------

const char *CABLoaderCreator::myName() { return ABLOADERCREATOR_NAME; }
const char *CABLoaderCreator::myType() { return CContextCreator::myType(); }
unsigned long CABLoaderCreator::myMajorVersion() { return CContextCreator::myMajorVersion(); }
unsigned long CABLoaderCreator::myMinorVersion() { return 1; }
unsigned long CABLoaderCreator::myRevision() { return 1; }
void CABLoaderCreator::startup()
{
	trace("<> CABLoaderCreator::startup()");
}

void CABLoaderCreator::shutdown()
{
	trace("<> CABLoaderCreator::shutdown()");
}

const char *CABLoader::myName() { return ABLOADER_NAME; }
const char *CABLoader::myType() { return IRiContext::myType(); }
unsigned long CABLoader::myMajorVersion() { return IRiContext::myMajorVersion(); }
unsigned long CABLoader::myMinorVersion() { return 1; }
unsigned long CABLoader::myRevision() { return 1; }
RtToken CABLoader::myRendererType() { return RI_REALISTIC; }

CABLoader::CABLoader()
{
	trace("<> CABLoader::CABLoader()");
	useStrips(_USESTRIPS);

	m_hierarchyType = _DEF_HIERARCHY;
	m_renderMode = _DEF_RENDERMODE;
	m_defaultCamera = _DEF_DEFAULT_CAMERA;

	// Sync
	RI_RENDER = 0;

	// Control
	RI_ABLOADER = 0;

	RI_SET_MOUSE_POSITION = 0;
	RI_QUAL_MOUSE_MOVED_TO = 0;

	RI_MOUSE_MOVED_TO = 0;
	RI_QUAL_SET_MOUSE_POSITION = 0;
	
	RI_HIERARCHY_TYPE = 0;
	RI_QUAL_HIERARCHY_TYPE = 0;
	RI_BVH = 0;
	RI_SAHBVH = 0;
	RI_BIH = 0;
	RI_HIERARCHY_PLUGIN = 0;
	RI_NO_HIERARCHY = 0;
	
	RI_RENDER_MODE = 0;
	RI_QUAL_RENDER_MODE = 0;
	RI_RAY_TRACING = 0;
	RI_OPENGL = 0;
	
	RI_SET_CAMERA = 0;
	RI_QUAL_SET_CAMERA =0;

	RI_DEFAULT = 0;
	RI_NONE = 0;

	m_ab = 0;
}

CABLoader::~CABLoader()
{
	trace("<> CABLoader::~CABLoader()");
}

bool CABLoader::valid() const
{
	return m_ab != 0;
}

void CABLoader::testScene()
{
	double dabp[9] = {
		-1,  -1, 0,
		1,  -1, 0,
		0,   1, 0
	};
	double dabn[9] = {
		0, 0, -1,
		0, 0, -1,
		0, 0, -1
	};
	
	AB::Vector3 abp[3], abn[3];
	for ( int v = 0; v < 3; ++v ) {
		for ( int c = 0; c < 3; ++c ) {
			abp[v][c] = dabp[v*3+c];
			abn[v][c] = dabn[v*3+c];
		}
	}
	m_ab->addTriangle(abp[0], abp[1], abp[2], abn[0], abn[1], abn[2]);
}

void CABLoader::defaultDeclarations()
{
	TypeParent::defaultDeclarations();
	
	// Additional tokens and declarations
	
	RI_RENDER = renderState()->tokFindCreate("render"); // Synchronize
	RI_ABLOADER = renderState()->tokFindCreate("abloader"); // Option and control classes
	
	RI_SET_MOUSE_POSITION = renderState()->tokFindCreate("setMousePosition");
	RI_QUAL_SET_MOUSE_POSITION = renderState()->declare("Control:abloader:setMousePosition", "constant integer[2]", true);

	RI_MOUSE_MOVED_TO = renderState()->tokFindCreate("mouseMovedTo");
	RI_QUAL_MOUSE_MOVED_TO = renderState()->declare("Control:abloader:mouseMovedTo", "constant integer[2]", true);

	RI_HIERARCHY_TYPE = renderState()->tokFindCreate("hierarchyType"); // option
	RI_QUAL_HIERARCHY_TYPE = renderState()->declare("Option:abloader:hierarchyType", "constant string", true);
	RI_BVH = renderState()->tokFindCreate("bvh");
	RI_SAHBVH = renderState()->tokFindCreate("sahbvh");
	RI_BIH = renderState()->tokFindCreate("bih");
	RI_HIERARCHY_PLUGIN = renderState()->tokFindCreate("hierarchyPlugin");
	RI_NO_HIERARCHY= renderState()->tokFindCreate("noHierarchy");
	
	RI_RENDER_MODE = renderState()->tokFindCreate("renderMode"); // option
	RI_QUAL_RENDER_MODE = renderState()->declare("Option:abloader:renderMode", "constant string", true);
	RI_RAY_TRACING = renderState()->tokFindCreate("ray-tracing");
	
	RI_SET_CAMERA = renderState()->tokFindCreate("setCamera"); // option
	RI_QUAL_SET_CAMERA = renderState()->declare("Option:abloader:setCamera", "constant string", true);
	
	RI_DEFAULT = renderState()->tokFindCreate("default");
	RI_NONE = renderState()->tokFindCreate("none");
}


RtVoid CABLoader::setABOptions()
{
	m_hierarchyType = _DEF_HIERARCHY;
	m_renderMode    = _DEF_RENDERMODE;
	m_defaultCamera = _DEF_DEFAULT_CAMERA;
	
	const CParameter *aHierarchyType = renderState()->options().get(RI_ABLOADER, RI_HIERARCHY_TYPE);
	if ( aHierarchyType ) {
		std::string strVal;
		aHierarchyType->get(0, strVal);
		if ( strVal == std::string(RI_BVH) )
			m_hierarchyType = BVH;
		else if ( strVal == std::string(RI_SAHBVH) )
			m_hierarchyType = SAHBVH;
		else if ( strVal == std::string(RI_BIH) )
			m_hierarchyType = BIH;
		else if ( strVal == std::string(RI_HIERARCHY_PLUGIN) )
			m_hierarchyType = HIERARCHY_PLUGIN;
		else if ( strVal == std::string(RI_NONE) )
			m_hierarchyType = NO_HIERARCHY;
		else if ( strVal == std::string(RI_NO_HIERARCHY) )
			m_hierarchyType = NO_HIERARCHY;
		
		m_ab->setHierarchyType(m_hierarchyType);
	}
	
	const CParameter *aRenderMode = renderState()->options().get(RI_ABLOADER, RI_RENDER_MODE);
	if ( aRenderMode ) {
		std::string strVal;
		aRenderMode->get(0, strVal);
		if ( strVal == std::string(RI_RAY_TRACING) )
			m_renderMode = RAY_TRACING;
		else if ( strVal == std::string(RI_OPENGL) )
			m_renderMode = OPENGL;
		else if ( strVal == std::string(RI_NONE) )
			m_renderMode = NONE;
		
		// m_ab->setRenderMode(m_renderMode);
	}
	m_ab->setRenderMode(m_renderMode);
	// AB hangs, if rendermode is not set!

	const CParameter *aDefaultCamera = renderState()->options().get(RI_ABLOADER, RI_SET_CAMERA);
	if ( aDefaultCamera ) {
		std::string strVal;
		aDefaultCamera->get(0, strVal);
		if ( strVal == std::string(RI_NONE) ) {
			m_defaultCamera = false;
		}
	}
	
	if ( m_defaultCamera ) {
		m_ab->applyDefaultView();
		// m_ab->setSize(renderState()->options().xResolution(), renderState()->options().yResolution());
		// m_ab->setAspectRatio(renderState()->options().frameAspectRatio());
		Vector3 cameraPosition(0,0,0);
		Vector3 cameraDirection(0,0,1);
		m_ab->setCameraPosition(cameraPosition);
		m_ab->setCameraDirection(cameraDirection);
		// m_ab->setIgnoreRTDisplay(true);
		// m_ab->requestNewRendering();
	}
}


void CABLoader::hide(const CFace &f)
{
	trace("-> CABLoader::hide(const CFace &)");
	
	if ( !valid() ) {
		trace("<- CABLoader::hide(const CFace &), not valid");
		return;
	}

	std::vector<RtFloat> p;
	std::vector<RtFloat> n;

	getPosAndNormals(f, toCamera(), p, n);
	if ( p.empty() || p.size() != n.size() ) {
		trace("<- CABLoader::hide(const CFace &), no geometry");
		return;
	}

	Vector3 abp[3], abn[3];
	unsigned int shaderId=0;
	IndexType sizeCnt = 0;
	switch ( f.faceType() ) {
		case FACETYPE_TRIANGLES: {
			for ( std::vector<IndexType>::const_iterator siter = f.sizes().begin(); siter != f.sizes().end(); siter++ ) {
				for ( IndexType idx = 0; idx < *siter-2; ) {
					for ( int i = 0; i < 3; ++i, ++idx ) {						
						abp[i][0] = -1.0 * p[f.indices()[sizeCnt+idx]*3+0];
						abp[i][1] = p[f.indices()[sizeCnt+idx]*3+1];
						abp[i][2] = p[f.indices()[sizeCnt+idx]*3+2];

						if ( n.size() == p.size() ) {
							abn[i][0] = -1.0 * n[f.indices()[sizeCnt+idx]*3+0];
							abn[i][1] = -1.0 * n[f.indices()[sizeCnt+idx]*3+1];
							abn[i][2] = -1.0 * n[f.indices()[sizeCnt+idx]*3+2];
						} else if ( n.size() == 3 ) {
							abn[i][0] = -1.0 * n[0];
							abn[i][1] = -1.0 * n[1];
							abn[i][2] = -1.0 * n[2];
						} else {
							abn[i][0] = 0;
							abn[i][1] = 0;
							abn[i][2] = 0;
						}
					}
					m_ab->addTriangle(abp[0], abp[1], abp[2], abn[0], abn[1], abn[2], shaderId);
				}
				sizeCnt += (*siter);
			}
		}
		break;
			
		default:
			break;
	}

	trace("<- CABLoader::hide(const CFace &)");
}

RtVoid CABLoader::doProcess(CRiSynchronize &obj)
{
	TypeParent::doProcess(obj);
	
	if ( !valid() )
		return;

	if ( obj.name() == RI_RENDER ) {
		m_ab->requestNewRendering();
	}
}

RtVoid CABLoader::doProcess(CRiControl &obj)
{
	TypeParent::doProcess(obj);
	
	if ( !valid() )
		return;

	if ( obj.name() == RI_ABLOADER ) {
		CParameterList::const_iterator i;
		for ( i = obj.parameters().begin(); i != obj.parameters().end(); i++ ) {
			if ( (*i).matches(QUALIFIER_CONTROL, RI_ABLOADER, RI_SET_MOUSE_POSITION) ) {
				const std::vector<RtInt> mpos = (*i).ints();
				if ( mpos.size() == 2 ) {
					m_ab->setMousePosition(mpos[0], mpos[1]);
				}
			} else if ( (*i).matches(QUALIFIER_CONTROL, RI_ABLOADER, RI_MOUSE_MOVED_TO) ) {
				const std::vector<RtInt> mmov = (*i).ints();
				if ( mmov.size() == 2 ) {
					m_ab->mouseMovedTo(mmov[0], mmov[1]);
				}
			} 
		}
	}
}

RtVoid CABLoader::doProcess(CRiBegin &obj)
{
	trace("-> CABLoader::doProcess(CRiBegin &)");

	TypeParent::doProcess(obj);

	m_ab = Augenblick::Instance();
#ifndef _MSC_VER
	// m_ab->pluginManager()->loadAllRegisteredPlugins();
#endif
	
	if ( !m_ab ) {
		std::cout << "Augenblick not instanciated.";
		trace("<- CABLoader::doProcess(CRiBegin &)");
		return;
	}

	m_ab->internalCheck();
	m_ab->setThreadCount(1);
	trace("<- CABLoader::doProcess(CRiBegin &)");
}

RtVoid CABLoader::doProcess(CRiEnd &obj)
{
	trace("-> CABLoader::doProcess(CRiEnd &)");
	TypeParent::doProcess(obj);	
	trace("<- CABLoader::doProcess(CRiEnd &)");
}


RtVoid CABLoader::doProcess(CRiWorldBegin &obj)
{
	trace("-> CABLoader::doProcess(CRiWorldBegin &)");
	TypeParent::doProcess(obj);
	
	if ( !valid() )
		return;
	
	// testScene();

	trace("<- CABLoader::doProcess(CRiWorldBegin &)");
}

RtVoid CABLoader::doProcess(CRiWorldEnd &obj)
{
	TypeParent::doProcess(obj);
	
	trace("-> CABLoader::doProcess(CRiWorldEnd &)");
	if ( !valid() ) {
		trace("<- CABLoader::doProcess(CRiWorldEnd &) - not valid");
		return;
	}
	
	setABOptions();
	trace("<- CABLoader::doProcess(CRiWorldEnd &)");
}
