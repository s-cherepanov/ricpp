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

/** @file trianglerenderer.cpp
 *  @author Andreas Pidde (andreas@pidde.de)
 *  @brief Base class for renderers that use triangle primitives.
 */

#include "ricpp/baserenderer/trianglerenderer.h"

using namespace RiCPP;

static const RtInt _TESSELATION = 16;
static const bool _USESTRIPS = false;

CTriangleRenderer::CTriangleRenderer()
{
	m_tessU = _TESSELATION;
	m_tessV = _TESSELATION;
	m_useStrips = _USESTRIPS;
	m_cacheGrids = true;
}

void CTriangleRenderer::getPosAndNormals(const CFace &f, const CMatrix3D &trans, std::vector<RtFloat> &p, std::vector<RtFloat> &n)
{
	n.clear();
	p.clear();
	
	// Positions
	const TemplPrimVar<RtFloat> *pp = f.floats(RI_P);
	if ( !pp )
		return;
	
	p = pp->values();
	
	if ( p.empty() )
		return;
	
	assert(p.size() == pp->values().size());
	
	trans.transformPoints((RtInt)p.size()/3, (RtPoint *)&p[0]);
	
	// Normals
	const TemplPrimVar<RtFloat> *np = f.floats(RI_N);
	if ( np && np->declarationPtr() ) {
		if ( np->values().size() == p.size() || np->values().size() == 3 ) {
			
			/** @todo Mathematics of the normal transformation with C3DMatrix needs to be checked, seems to be faulty
			 */
			
			// n = np->value();
			// trans.transformNormals(n.size()/3, (RtPoint *)&n[0]);
			
			n = pp->values();

			if ( np->values().size() == p.size() ) {
				for ( unsigned int i = 0; i < n.size()-2; i+=3 ) {
					// if ( flipNormals() ) {
					//	n[i]   -= np->values()[i];
					//	n[i+1] -= np->values()[i+1];
					//	n[i+2] -= np->values()[i+2];
					// } else {
						n[i]   += np->values()[i];
						n[i+1] += np->values()[i+1];
						n[i+2] += np->values()[i+2];
					// }
				}
			} else {
				for ( unsigned int i = 0; i < n.size()-2; i+=3 ) {
					// if ( flipNormals() ) {
					//	n[i]   -= np->values()[0];
					//	n[i+1] -= np->values()[1];
					//	n[i+2] -= np->values()[2];
					// } else {
						n[i]   += np->values()[0];
						n[i+1] += np->values()[1];
						n[i+2] += np->values()[2];
					// }
				}
			}
			
			trans.transformPoints((RtInt)n.size()/3, (RtPoint *)&n[0]);
			
			for ( unsigned int i = 0; i < n.size()-2; i+=3 ) {
				n[i]   -= p[i];
				n[i+1] -= p[i+1];
				n[i+2] -= p[i+2];
				normalize(n[i], n[i+1], n[i+2]);
			}
		} else {
			/** @todo Normal size not recognized
			 */
		}
	} else {
		/** @todo Normals not found
		 */
	}
}


bool CTriangleRenderer::startHandling(CVarParamRManInterfaceCall &obj)
{
	if ( obj.tesselator() ) {
		triangulate(*obj.tesselator());
		return true;
	}
	return false;
}

RtVoid CTriangleRenderer::endHandling(CVarParamRManInterfaceCall &obj, CTesselator *triObj)
{
	if ( !triObj )
		return;
	
	triangulate(*triObj);

	if ( m_cacheGrids ) {
		obj.attach(triObj);
	} else {
		delete triObj;
	}
}



RtVoid CTriangleRenderer::triangulate(CTesselator &triObj)
{
	const CDeclaration *pdecl = renderState()->declFind(RI_P);
	if ( !pdecl || !pdecl->isFloat3Decl() ) {
		/// @todo Errorhandling, bad position declaration
		return;
	}
	const CDeclaration *ndecl = renderState()->declFind(RI_N);
	if ( !ndecl || !ndecl->isFloat3Decl() ) {
		/// @todo Errorhandling, bad normal declaration
		return;
	}
	
	triObj.useStrips(m_useStrips);
	triObj.tesselation(m_tessU, m_tessV);
	triObj.flipNormals(flipNormals());

	hideSurface(triObj.tesselate(*pdecl, *ndecl));
}

RtVoid CTriangleRenderer::triangulate(CRiPolygon &obj)
{
	if ( startHandling(obj) )
		return;
	CPolygonTesselator *t = new CPolygonTesselator(obj);
	endHandling(obj, t);
}

RtVoid CTriangleRenderer::triangulate(CRiPointsPolygons &obj)
{
	if ( startHandling(obj) )
		return;
	CPointsPolygonsTesselator *t = new CPointsPolygonsTesselator(obj);
	endHandling(obj, t);
}

RtVoid CTriangleRenderer::triangulate(CRiGeneralPolygon &obj)
{
	if ( startHandling(obj) )
		return;
	CGeneralPolygonTesselator *t = new CGeneralPolygonTesselator(obj, polygonTriangulationStrategy());
	endHandling(obj, t);
}

RtVoid CTriangleRenderer::triangulate(CRiPointsGeneralPolygons &obj)
{
	if ( startHandling(obj) )
		return;
	CPointsGeneralPolygonsTesselator *t = new CPointsGeneralPolygonsTesselator(obj, polygonTriangulationStrategy());
	endHandling(obj, t);
}

RtVoid CTriangleRenderer::triangulate(CRiCone &obj)
{
	if ( startHandling(obj) )
		return;
	CConeTesselator *t = new CConeTesselator(obj);
	endHandling(obj, t);
}

RtVoid CTriangleRenderer::triangulate(CRiCylinder &obj)
{
	if ( startHandling(obj) )
		return;
	CCylinderTesselator *t = new CCylinderTesselator(obj);
	endHandling(obj, t);
}

RtVoid CTriangleRenderer::triangulate(CRiDisk &obj)
{
	if ( startHandling(obj) )
		return;
	CDiskTesselator *t = new CDiskTesselator(obj);
	endHandling(obj, t);
}

RtVoid CTriangleRenderer::triangulate(CRiHyperboloid &obj)
{
	if ( startHandling(obj) )
		return;
	CHyperboloidTesselator *t = new CHyperboloidTesselator(obj);
	endHandling(obj, t);
}

RtVoid CTriangleRenderer::triangulate(CRiParaboloid &obj)
{
	if ( startHandling(obj) )
		return;
	CParaboloidTesselator *t = new CParaboloidTesselator(obj);
	endHandling(obj, t);
}

RtVoid CTriangleRenderer::triangulate(CRiSphere &obj)
{
	if ( startHandling(obj) )
		return;
	CSphereTesselator *t = new CSphereTesselator(obj);
	endHandling(obj, t);
}

RtVoid CTriangleRenderer::triangulate(CRiTorus &obj)
{
	if ( startHandling(obj) )
		return;
	CTorusTesselator *t = new CTorusTesselator(obj);
	endHandling(obj, t);
}

RtVoid CTriangleRenderer::triangulate(CRiPatch &obj)
{
	if ( startHandling(obj) )
		return;
	
	CRiBasis basis(renderState()->lineNo(), attributes().uBasis(), attributes().uStep(), attributes().vBasis(), attributes().vStep());
	CPatchTesselator *t = new CPatchTesselator(obj, basis);
	endHandling(obj, t);
}

RtVoid CTriangleRenderer::triangulate(CRiPatchMesh &obj)
{
	if ( startHandling(obj) )
		return;
	
	CRiBasis basis(renderState()->lineNo(), attributes().uBasis(), attributes().uStep(), attributes().vBasis(), attributes().vStep());
	CPatchMeshTesselator *t = new CPatchMeshTesselator(obj, basis);
	endHandling(obj, t);
}

RtVoid CTriangleRenderer::triangulate(CRiNuPatch &obj)
{
	if ( startHandling(obj) )
		return;
	
	CNuPatchTesselator *t = new CNuPatchTesselator(obj);
	endHandling(obj, t);
}

RtVoid CTriangleRenderer::doProcess(CRiPolygon &obj)
{
	triangulate(obj);
}


RtVoid CTriangleRenderer::doProcess(CRiGeneralPolygon &obj)
{
	triangulate(obj);
}


RtVoid CTriangleRenderer::doProcess(CRiPointsPolygons &obj)
{
	triangulate(obj);
}

RtVoid CTriangleRenderer::doProcess(CRiPointsGeneralPolygons &obj)
{
	triangulate(obj);
}

RtVoid CTriangleRenderer::doProcess(CRiCone &obj)
{
	triangulate(obj);
}

RtVoid CTriangleRenderer::doProcess(CRiCylinder &obj)
{
	triangulate(obj);
}

RtVoid CTriangleRenderer::doProcess(CRiDisk &obj)
{
	triangulate(obj);
}

RtVoid CTriangleRenderer::doProcess(CRiHyperboloid &obj)
{
	triangulate(obj);
}

RtVoid CTriangleRenderer::doProcess(CRiParaboloid &obj)
{
	triangulate(obj);
}

RtVoid CTriangleRenderer::doProcess(CRiSphere &obj)
{
	triangulate(obj);
}

RtVoid CTriangleRenderer::doProcess(CRiTorus &obj)
{
	triangulate(obj);
}

RtVoid CTriangleRenderer::doProcess(CRiPatch &obj)
{
	triangulate(obj);
}

RtVoid CTriangleRenderer::doProcess(CRiPatchMesh &obj)
{
	triangulate(obj);
}

RtVoid CTriangleRenderer::doProcess(CRiNuPatch &obj)
{
	triangulate(obj);
}
