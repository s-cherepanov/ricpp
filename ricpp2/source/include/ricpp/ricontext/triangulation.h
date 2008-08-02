#ifndef _RICPP_RICONTEXT_TRIANGULATION_H
#define _RICPP_RICONTEXT_TRIANGULATION_H

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

/** @file triangulation.h
 *  @author Andreas Pidde (andreas@pidde.de)
 *  @brief Triangulation of the surface primitives.
 */

#ifndef _RICPP_RICONTEXT_SURFACE_H
#include "ricpp/ricontext/surface.h"
#endif _RICPP_RICONTEXT_SURFACE_H

#ifndef _RICPP_RICONTEXT_RIMACROPRIMS_H
#include "ricpp/ricontext/rimacroprims.h"
#endif _RICPP_RICONTEXT_RIMACROPRIMS_H

namespace RiCPP {

	class CTesselator {
		std::list<CSurface *> m_surfaces;
	public:
		CTesselator();
		virtual ~CTesselator();

		bool releaseSurface(CSurface *surf);
		CSurface *createSurface();
	};
	
	class CTriangulator : public CTesselator {
	public:
	};
	
	// =========================================================================
	
	class CPolygonTriangulator : public CTriangulator {
	protected:
		void triangleStrip(std::vector<IndexType> &strip, IndexType nVerts, IndexType offs) const;
		void insertParameters(CFace &f, IndexType faceIdx, const CParameterList &plist, const std::vector<RtInt> &verts, IndexType nverts, IndexType vertsOffs) const;
	};
	
	class CConvexPolygonTriangulator : public CPolygonTriangulator {
	public:
		CSurface *triangulate(CRiPolygon &obj);
	};

	class CConvexPointsPolygonsTriangulator : public CPolygonTriangulator {
	public:
		CSurface *triangulate(CRiPointsPolygons &obj);
	};

	class CGeneralPolygonTriangulator : public CPolygonTriangulator {
	public:
		CSurface *triangulate(CRiGeneralPolygon &obj, const IPolygonTriangulationStrategy &strategy);
	};

	class CPointsGeneralPolygonsTriangulator : public CPolygonTriangulator {
	public:
		CSurface *triangulate(CRiPointsGeneralPolygons &obj, const IPolygonTriangulationStrategy &strategy);
	};


	// =========================================================================

	class CParametricTriangulator : public CTriangulator {
	protected:
	};

	class CQuadricTriangulator : public CParametricTriangulator {
	protected:
		struct SQuadricVars {
			IndexType nVars, realTessU, realTessV;
			RtFloat deltaU, deltaV;
			RtFloat flipNormal;
			std::vector<RtFloat> *positions, *normals;
		};
		void getUnitCircle(std::vector<RtFloat> &circledata, IndexType tess, RtFloat thetamax, RtFloat thetamin=0);
		virtual void buildPN(const CDeclaration &pointDecl, const CDeclaration &normDecl, RtInt tessU, RtInt tessV, bool equalOrientations, CFace &f) = 0;
		void initVars(const CDeclaration &pointDecl, const CDeclaration &normDecl, RtInt tessU, RtInt tessV, bool equalOrientations, CFace &f, SQuadricVars &retVals);
	public:
		CSurface *triangulate(const CDeclaration &posDecl, const CDeclaration &normDecl, RtInt tessU, RtInt tessV, bool equalOrientations);
	};
	
	class CParaboloidTriangulator : public CQuadricTriangulator {
		const CRiParaboloid *m_obj;
	protected:
		virtual void buildPN(const CDeclaration &pointDecl, const CDeclaration &normDecl, RtInt tessU, RtInt tessV, bool equalOrientations, CFace &f);
	public:
		inline CParaboloidTriangulator(const CRiParaboloid &obj) : m_obj(&obj) {}
	};

	class CSphereTriangulator : public CQuadricTriangulator {
		const CRiSphere *m_obj;
	protected:
		virtual void buildPN(const CDeclaration &pointDecl, const CDeclaration &normDecl, RtInt tessU, RtInt tessV, bool equalOrientations, CFace &f);
	public:
		inline CSphereTriangulator(const CRiSphere &obj) : m_obj(&obj) {}
	};
}

#endif // _RICPP_RICONTEXT_TRIANGULATION_H
