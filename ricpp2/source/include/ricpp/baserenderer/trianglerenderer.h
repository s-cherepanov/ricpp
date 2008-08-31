#ifndef _RICPP_BASERENDERER_TRIANGLERENDERER_H
#define _RICPP_BASERENDERER_TRIANGLERENDERER_H

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

/** @file trianglerenderer.h
 *  @author Andreas Pidde (andreas@pidde.de)
 *  @brief Base class for renderers that use triangle primitives.
 */

#ifndef _RICPP_BASERENDERER_BASERENDERER_H
#include "ricpp/baserenderer/baserenderer.h"
#endif // _RICPP_BASERENDERER_BASERENDERER_H

#ifndef _RICPP_RICONTEXT_POLYGON_H
#include "ricpp/ricontext/polygon.h"
#endif // _RICPP_RICONTEXT_POLYGON_H

#ifndef _RICPP_RICONTEXT_TRIANGULATION_H
#include "ricpp/ricontext/triangulation.h"
#endif // _RICPP_RICONTEXT_TRIANGULATION_H

namespace RiCPP {
	
	/** @brief Base class to triangulate primitives.
	 */
	class CTriangleRenderer : public CBaseRenderer {
	protected:
		typedef CBaseRenderer TypeParent;

	private:
		CEarClipper m_earClipper; ///< Triangulation strategy
		RtInt m_tessU;
		RtInt m_tessV;
		bool m_useStrips;
		bool m_cacheGrids;
		
		bool startHandling(CVarParamRManInterfaceCall &obj);
		RtVoid endHandling(CVarParamRManInterfaceCall &obj, CTesselator *triObj);
		
	protected:
		inline virtual const IPolygonTriangulationStrategy &polygonTriangulationStrategy() const { return m_earClipper; }
		void getPosAndNormals(const CFace &f, const CMatrix3D &trans, std::vector<RtFloat> &p, std::vector<RtFloat> &n);
		
	public:
		CTriangleRenderer();
		
		inline bool useStrips() const { return m_useStrips; }
		inline void useStrips(bool aUseStrips) { m_useStrips = aUseStrips; }
		
		inline bool cacheGrids() const { return m_cacheGrids; }
		inline void cacheGrids(bool doCache) { m_cacheGrids = doCache; }

		using TypeParent::doProcess;
		using TypeParent::preProcess;
		using TypeParent::postProcess;
		
		virtual RtVoid triangulate(CTesselator &triObj);
		
		virtual RtVoid triangulate(CRiPolygon &obj);
		virtual RtVoid triangulate(CRiPointsPolygons &obj);
		virtual RtVoid triangulate(CRiGeneralPolygon &obj);
		virtual RtVoid triangulate(CRiPointsGeneralPolygons &obj);

		virtual RtVoid triangulate(CRiCone &obj);
		virtual RtVoid triangulate(CRiCylinder &obj);
		virtual RtVoid triangulate(CRiDisk &obj);
		virtual RtVoid triangulate(CRiHyperboloid &obj);
		virtual RtVoid triangulate(CRiParaboloid &obj);
		virtual RtVoid triangulate(CRiSphere &obj);
		virtual RtVoid triangulate(CRiTorus &obj);

		virtual RtVoid triangulate(CRiPatch &obj);
		virtual RtVoid triangulate(CRiPatchMesh &obj);
		virtual RtVoid triangulate(CRiNuPatch &obj);
		
		//

		virtual RtVoid doProcess(CRiPolygon &obj);
		virtual RtVoid doProcess(CRiGeneralPolygon &obj);
		virtual RtVoid doProcess(CRiPointsPolygons &obj);
		virtual RtVoid doProcess(CRiPointsGeneralPolygons &obj);
		
		virtual RtVoid doProcess(CRiCone &obj);
		virtual RtVoid doProcess(CRiCylinder &obj);
		virtual RtVoid doProcess(CRiDisk &obj);
		virtual RtVoid doProcess(CRiHyperboloid &obj);
		virtual RtVoid doProcess(CRiParaboloid &obj);
		virtual RtVoid doProcess(CRiSphere &obj);
		virtual RtVoid doProcess(CRiTorus &obj);
		
		virtual RtVoid doProcess(CRiPatch &obj);
		virtual RtVoid doProcess(CRiPatchMesh &obj);
		virtual RtVoid doProcess(CRiNuPatch &obj);
	}; // CTriangleRenderer

}

#endif // _RICPP_BASERENDERER_TRIANGLERENDERER_H
