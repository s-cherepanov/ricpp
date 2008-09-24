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

#ifndef _RICPP_RICONTEXT_RIMACROATTRIBUTES_H
#include "ricpp/ricontext/rimacroattributes.h"
#endif _RICPP_RICONTEXT_RIMACROATTRIBUTES_H

#ifndef _RICPP_RICONTEXT_BLEND_H
#include "ricpp/ricontext/blend.h"
#endif // _RICPP_RICONTEXT_BLEND_H

#ifndef _RICPP_RICONTEXT_SUBDIVISION_H
#include "ricpp/ricontext/subdivision.h"
#endif // _RICPP_RICONTEXT_SUBDIVISION_H

namespace RiCPP {

	class CUnitCircle {
		IndexType m_tess;
		RtFloat m_thetaMax;
		RtFloat m_thetaMin;
		std::vector<RtFloat> m_circleData;
		
		void calc();
	public:
		inline CUnitCircle(IndexType theTess, RtFloat theThetaMax, RtFloat theThetaMin)
			: m_tess(theTess), m_thetaMax(theThetaMax), m_thetaMin(theThetaMin)
		{
			calc();
		}

		inline const RtFloat &operator[](size_t index) const
		{
			return m_circleData[index];
		}

		inline IndexType tess()     const { return m_tess; }
		inline RtFloat   thetaMax() const { return m_thetaMax; }
		inline RtFloat   thetaMin() const { return m_thetaMin; }
	}; // CUnitCircle

	// =========================================================================
	
	class CTesselator {
		std::list<CSurface *> m_surfaces;
		IndexType m_tessU, m_tessV;
		bool m_frontFaceCW;
		bool m_flipNormals;
		bool m_useStrips;
		bool m_useTriangles;

		void detachPtr();
	protected:
		CVarParamRManInterfaceCall *m_objPtr;
		virtual const CVarParamRManInterfaceCall &obj() const = 0;

	public:
		inline CTesselator()
		: m_tessU(1), m_tessV(1), m_frontFaceCW(true), m_flipNormals(false), m_useStrips(false), m_useTriangles(true), m_objPtr(0)
		{
		}
		virtual ~CTesselator();

		bool releaseSurface(CSurface *surf);
		CSurface *createSurface();
		
		inline IndexType tessU() const { return m_tessU; }
		inline void tessU(IndexType aTessU) { m_tessU = aTessU; }
		inline IndexType tessV() const { return m_tessV; }
		inline void tessV(IndexType aTessV) { m_tessV = aTessV; }
		
		inline void tesselation(IndexType aTessU, IndexType aTessV)
		{
			tessU(aTessU);
			tessV(aTessV);
		}

		inline void tesselation(IndexType &aTessU, IndexType &aTessV) const
		{
			aTessU = tessU();
			aTessV = tessV();
		}
		
		inline bool frontFaceCW() const { return m_frontFaceCW; }
		inline void frontFaceCW(bool anIsLeftHanded) { m_frontFaceCW = anIsLeftHanded; }
		
		inline bool flipNormals() const { return m_flipNormals; }
		inline void flipNormals(bool aFlipNormals) { m_flipNormals = aFlipNormals; }

		inline bool useStrips() const { return m_useStrips; }
		inline void useStrips(bool aUseStrips) { m_useStrips = aUseStrips; }

		inline bool useTriangles() const { return m_useTriangles; }
		inline void useTriangles(bool aUseTriangles) { m_useTriangles = aUseTriangles; }

		virtual CSurface *tesselate(const CDeclaration &posDecl, const CDeclaration &normDecl) = 0;
		
		void detach();
		void attach(CVarParamRManInterfaceCall *anObjPtr);
	}; // CTesselator
	
	// =========================================================================
	
	class CBasePolygonTesselator : public CTesselator {
	private:
		void triangles(IndexType nVerts, IndexType offs, std::vector<IndexType> &stripIdx) const;
		void strip(IndexType nVerts, IndexType offs, std::vector<IndexType> &stripIdx) const;
		void addNormalsToPos(const CDeclaration &normDecl, const RtNormal &aNormal, const std::vector<RtFloat> &pos, CFace &f);
	protected:
		void buildIndices(IndexType nVerts, IndexType offs, CFace &f);
		void insertParams(IndexType faceIdx, const CParameterList &plist, const std::vector<RtInt> &verts, IndexType nverts, IndexType vertsOffs, CFace &f);
		bool addNormals(const CDeclaration &normDecl, CFace &f);
		bool addNormals(const CDeclaration &normDecl, const RtNormal &aNormal, CFace &f);
	}; // CBasePolygonTesselator
	
	class CPolygonTesselator : public CBasePolygonTesselator {
		CRiPolygon *m_obj;
	protected:
		inline virtual const CVarParamRManInterfaceCall &obj() const
		{
			assert(m_obj);
			return *m_obj;
		}
	public:
		inline CPolygonTesselator(CRiPolygon &obj) : m_obj(&obj)  {}
		virtual CSurface *tesselate(const CDeclaration &posDecl, const CDeclaration &normDecl);
	}; // CPolygonTesselator

	class CPointsPolygonsTesselator : public CBasePolygonTesselator {
		CRiPointsPolygons *m_obj;
	protected:
		inline virtual const CVarParamRManInterfaceCall &obj() const
		{
			assert(m_obj);
			return *m_obj;
		}
	public:
		inline CPointsPolygonsTesselator(CRiPointsPolygons &obj) : m_obj(&obj)  {}
		virtual CSurface *tesselate(const CDeclaration &posDecl, const CDeclaration &normDecl);
	}; // CPointsPolygonsTesselator

	class CGeneralPolygonTesselator : public CBasePolygonTesselator {
		CRiGeneralPolygon *m_obj;
		const CTriangulatedPolygon *m_tpPtr;
	protected:
		inline virtual const CVarParamRManInterfaceCall &obj() const
		{
			assert(m_obj);
			return *m_obj;
		}
	public:
		inline CGeneralPolygonTesselator(CRiGeneralPolygon &obj, const IPolygonTriangulationStrategy &strategy) : m_obj(&obj)
		{
			m_tpPtr = m_obj->triangulate(strategy);
		}
		virtual CSurface *tesselate(const CDeclaration &posDecl, const CDeclaration &normDecl);
	}; // CGeneralPolygonTesselator

	class CPointsGeneralPolygonsTesselator : public CBasePolygonTesselator {
		CRiPointsGeneralPolygons *m_obj;
		const std::vector<CTriangulatedPolygon> *m_tpPtr;
	protected:
		inline virtual const CVarParamRManInterfaceCall &obj() const
		{
			assert(m_obj);
			return *m_obj;
		}
	public:
		inline CPointsGeneralPolygonsTesselator(CRiPointsGeneralPolygons &obj, const IPolygonTriangulationStrategy &strategy) : m_obj(&obj)
		{
			m_tpPtr = &m_obj->triangulate(strategy);
		}
		virtual CSurface *tesselate(const CDeclaration &posDecl, const CDeclaration &normDecl);
	}; // CPointsGeneralPolygonsTesselator


	// =========================================================================

	class CParametricTesselator : public CTesselator {
	protected:
		struct SParametricVars {
			IndexType nVars;
			RtFloat deltaU, deltaV;
			RtFloat flipNormal;
			std::vector<RtFloat> *positions, *normals;
			
			inline SParametricVars(const CDeclaration &posDecl, const CDeclaration &normDecl,
								   IndexType aTessU, IndexType aTessV, bool aFlipNormal, CFace &aFace)
			{
				initVars(posDecl, normDecl, aTessU, aTessV, aFlipNormal, aFace);
			}
			
			void initVars(IndexType aTessU, IndexType aTessV, bool aFlipNormal, CFace &aFace);
			void initVars(const CDeclaration &posDecl, const CDeclaration &normDecl,
						  IndexType aTessU, IndexType aTessV, bool aFlipNormal, CFace &aFace);
		}; // SParametricVars

		void insertBilinearParams(IndexType faceIndex,
								  const IndexType (&cornerIdx)[4], const IndexType (&faceCornerIdx)[4],
								  CFace &f);
		
		void getStdCornerIdx(IndexType offset, IndexType (&idx)[4]) const;
		void getCornerIdx(IndexType upatch, IndexType vpatch, IndexType nu, IndexType nv, IndexType (&idx)[4]) const;
		void getFaceCornerIdx(IndexType faceIndex, IndexType (&idx)[4]) const;
		void getFaceCornerIdx(IndexType upatch, IndexType vpatch, IndexType nu, IndexType nv, IndexType (&idx)[4]) const;

		void buildIndices(CFace &f);
	}; // CParametricTesselator

	// -------------------------------------------------------------------------

	class CQuadricTesselator : public CParametricTesselator {
	private:
		void insertParams(CFace &f);
	protected:
		virtual void buildPN(const CDeclaration &posDecl, const CDeclaration &normDecl, CFace &f) = 0;
		void buildConePN(RtFloat height, RtFloat radius, RtFloat thetamax, RtFloat displacement, const CDeclaration &posDecl, const CDeclaration &normDecl, const SParametricVars &var, CFace &f);
		void buildHyperboloidPN(RtPoint point1, RtPoint point2, RtFloat thetamax, const CDeclaration &posDecl, const CDeclaration &normDecl, const SParametricVars &var, CFace &f);
	public:
		virtual CSurface *tesselate(const CDeclaration &posDecl, const CDeclaration &normDecl);
	}; // CQuadricTesselator
	
	class CConeTesselator : public CQuadricTesselator {
		CRiCone *m_obj;
	protected:
		virtual void buildPN(const CDeclaration &posDecl, const CDeclaration &normDecl, CFace &f);
		inline virtual const CVarParamRManInterfaceCall &obj() const
		{
			assert(m_obj);
			return *m_obj;
		}
	public:
		inline CConeTesselator(CRiCone &obj) : m_obj(&obj) {}
	}; // CConeTesselator
	
	class CCylinderTesselator : public CQuadricTesselator {
		CRiCylinder *m_obj;
	protected:
		virtual void buildPN(const CDeclaration &posDecl, const CDeclaration &normDecl, CFace &f);
		inline virtual const CVarParamRManInterfaceCall &obj() const
		{
			assert(m_obj);
			return *m_obj;
		}
	public:
		inline CCylinderTesselator(CRiCylinder &obj) : m_obj(&obj) {}
	}; // CCylinderTesselator
	
	class CDiskTesselator : public CQuadricTesselator {
		CRiDisk *m_obj;
	protected:
		virtual void buildPN(const CDeclaration &posDecl, const CDeclaration &normDecl, CFace &f);
		inline virtual const CVarParamRManInterfaceCall &obj() const
		{
			assert(m_obj);
			return *m_obj;
		}
	public:
		inline CDiskTesselator(CRiDisk &obj) : m_obj(&obj) {}
	}; // CDiskTesselator

	class CHyperboloidTesselator : public CQuadricTesselator {
		CRiHyperboloid *m_obj;
	protected:
		virtual void buildPN(const CDeclaration &posDecl, const CDeclaration &normDecl, CFace &f);
		inline virtual const CVarParamRManInterfaceCall &obj() const
		{
			assert(m_obj);
			return *m_obj;
		}
	public:
		inline CHyperboloidTesselator(CRiHyperboloid &obj) : m_obj(&obj) {}
	}; // CHyperboloidTesselator

	class CParaboloidTesselator : public CQuadricTesselator {
		CRiParaboloid *m_obj;
	protected:
		virtual void buildPN(const CDeclaration &posDecl, const CDeclaration &normDecl, CFace &f);
		inline virtual const CVarParamRManInterfaceCall &obj() const
		{
			assert(m_obj);
			return *m_obj;
		}
	public:
		inline CParaboloidTesselator(CRiParaboloid &obj) : m_obj(&obj) {}
	}; // CParaboloidTesselator

	class CSphereTesselator : public CQuadricTesselator {
		CRiSphere *m_obj;
	protected:
		virtual void buildPN(const CDeclaration &posDecl, const CDeclaration &normDecl, CFace &f);
		inline virtual const CVarParamRManInterfaceCall &obj() const
		{
			assert(m_obj);
			return *m_obj;
		}
	public:
		inline CSphereTesselator(CRiSphere &obj) : m_obj(&obj) {}
	}; // CSphereTesselator

	class CTorusTesselator : public CQuadricTesselator {
		CRiTorus *m_obj;
	protected:
		virtual void buildPN(const CDeclaration &posDecl, const CDeclaration &normDecl, CFace &f);
		inline virtual const CVarParamRManInterfaceCall &obj() const
		{
			assert(m_obj);
			return *m_obj;
		}
	public:
		inline CTorusTesselator(CRiTorus &obj) : m_obj(&obj) {}
	}; // CTorusTesselator
	
	// -------------------------------------------------------------------------
		
	class CRootPatchTesselator : public CParametricTesselator {
	private:
		CRiBasis m_basis;
		CBicubicVectors *m_basisVectors;
		inline CRootPatchTesselator() {}
	protected:
		inline CRootPatchTesselator(const CRiBasis &aBasis) : m_basis(aBasis), m_basisVectors(0) {}
		void insertBicubicParams(IndexType faceIndex,
								 const IndexType (&cornerIdx)[4], const IndexType (&faceCornerIdx)[4],
								 const IndexType (&controlIdx)[16], const IndexType (&faceControlIdx)[16],
								 CFace &f);
		void buildBilinearPN(const CDeclaration &posDecl, const CDeclaration &normDecl,
									 IndexType faceIndex,
									 const IndexType (&cornerIdx)[4], const IndexType (&faceCornerIdx)[4],
									 CFace &f);
		void buildBicubicPN(const CDeclaration &posDecl, const CDeclaration &normDecl,
									IndexType faceIndex,
									const IndexType (&cornerIdx)[4], const IndexType (&faceCornerIdx)[4],
									const IndexType (&controlIdx)[16], const IndexType (&faceControlIdx)[16],
									CFace &f);
		
		void getStdControlIdx(IndexType offset, IndexType (&idx)[16]) const;
		void getControlIdx(IndexType upatch, IndexType vpatch, IndexType nu, IndexType nv, IndexType ustep, IndexType vstep, IndexType (&idx)[16]) const;
		void getFaceControlIdx(IndexType upatch, IndexType vpatch, IndexType nu, IndexType nv, IndexType (&idx)[16]) const;

		inline const CRiBasis &basis() const { return m_basis; }
		inline CRiBasis &basis() { return m_basis; }
		
		inline const CBicubicVectors &basisVectors() const { assert(m_basisVectors); return *m_basisVectors; }
		inline CBicubicVectors &basisVectors() { assert(m_basisVectors); return *m_basisVectors; }
		inline bool newBasisVectors()
		{
			if ( !m_basisVectors )
				m_basisVectors = new CBicubicVectors;
			return m_basisVectors != 0;
		}

		inline void deleteBasisVectors()
		{
			delete m_basisVectors;
			m_basisVectors = 0;
		}
	}; // CRootPatchTesselator
	
	class CPatchTesselator : public CRootPatchTesselator {
		CRiPatch *m_obj;
		
	protected:
		inline virtual const CVarParamRManInterfaceCall &obj() const
		{
			assert(m_obj);
			return *m_obj;
		}

	public:
		inline CPatchTesselator(CRiPatch &obj, const CRiBasis &aBasis) : CRootPatchTesselator(aBasis), m_obj(&obj) {}
		virtual CSurface *tesselate(const CDeclaration &posDecl, const CDeclaration &normDecl);
	}; // CPatchTesselator

	class CPatchMeshTesselator : public CRootPatchTesselator {
		CRiPatchMesh *m_obj;
		
	protected:
		inline virtual const CVarParamRManInterfaceCall &obj() const
		{
			assert(m_obj);
			return *m_obj;
		}

	public:
		inline CPatchMeshTesselator(CRiPatchMesh &obj, const CRiBasis &aBasis) : CRootPatchTesselator(aBasis), m_obj(&obj) {}
		virtual CSurface *tesselate(const CDeclaration &posDecl, const CDeclaration &normDecl);
	}; // CPatchMeshTesselator
	
	// -------------------------------------------------------------------------
	
	class CNuPatchTesselator : public CParametricTesselator {
		CRiNuPatch *m_obj;
		CUVBSplineBasis *m_basis;
	
		// State variables, filled by fillIdx
		RtInt m_useg, m_vseg;
		std::vector<IndexType> m_cornerIdx, m_faceCornerIdx, m_vertexIdx, m_faceVertexIdx;
		std::vector<IndexType> m_idIdx;

		void fillIdx(RtInt usegment, RtInt vsegment);
		inline RtInt faceIndex() const { return m_vseg * uBasis().numSegments() + m_useg; }

		inline const CUVBSplineBasis &basis() const { assert(m_basis); return *m_basis; }
		inline CUVBSplineBasis &basis() { assert(m_basis); return *m_basis; }
		inline const CBSplineBasis &uBasis() const { return basis().uBasis(); }
		inline CBSplineBasis &uBasis() { return basis().uBasis(); }		
		inline const CBSplineBasis &vBasis() const { return basis().vBasis(); }
		inline CBSplineBasis &vBasis() { return basis().vBasis(); }

		void insertNuParams(CFace &f);
		void buildNuPN(const CDeclaration &posDecl, const CDeclaration &normDecl, CFace &f);
		bool extractPFromPz(const CParameter *pz, std::vector<RtFloat> &results) const;

	protected:
		inline virtual const CVarParamRManInterfaceCall &obj() const
		{
			assert(m_obj);
			return *m_obj;
		}

	public:
		inline CNuPatchTesselator() : m_useg(0), m_vseg(0), m_obj(0), m_basis(0) { }
		inline CNuPatchTesselator(CRiNuPatch &obj) : m_obj(&obj), m_useg(0), m_vseg(0), m_basis(0) { }
		virtual CSurface *tesselate(const CDeclaration &posDecl, const CDeclaration &normDecl);
	}; // CNuPatchTesselator

	// -------------------------------------------------------------------------
	
	class CSubdivisionHierarchyTesselator  : public CTesselator {
	private:
		std::list<CSubdivisionIndices> m_indices;    //!< Contains topology
		CRiHierarchicalSubdivisionMesh m_obj;        //!< CRiHierarchicalSubdivisionMesh handles non-hierarchical subdivisions as well
		const CSubdivisionStrategies &m_strategies;	 //!< Strategies for subdivision ("catmull-clark")
		
		void subdivide(const CSubdivisionStrategy &strategy, IndexType depth);
		void insertParams(const CSubdivisionStrategy &strategy, const std::list<CSubdivisionIndices>::const_iterator &curIndices, CFace &aFace);
		void extractFaces(const CSubdivisionStrategy &strategy, const std::list<CSubdivisionIndices>::iterator &curIndices, long faceNum, const CFace &varyingData, const CDeclaration &posDecl, const CDeclaration &normDecl, std::vector<IndexType> &origIndices, CFace &f);
		
	protected:
		inline virtual const CVarParamRManInterfaceCall &obj() const
		{
			return m_obj;
		}
		
	public:
		inline CSubdivisionHierarchyTesselator(CRiHierarchicalSubdivisionMesh &anObj, const CSubdivisionStrategies &theStrategies)
		: m_obj(anObj), m_strategies(theStrategies)
		{
		}
		inline CSubdivisionHierarchyTesselator(CRiSubdivisionMesh &anObj, const CSubdivisionStrategies &theStrategies)
		: m_obj(anObj), m_strategies(theStrategies)
		{
		}
		
		
		virtual CSurface *tesselate(const CDeclaration &posDecl, const CDeclaration &normDecl);
	};
}

#endif // _RICPP_RICONTEXT_TRIANGULATION_H
