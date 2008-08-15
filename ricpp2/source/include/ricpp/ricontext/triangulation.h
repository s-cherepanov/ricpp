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

#ifndef _RICPP_RICONTEXT_RIMACROPRIMS_H
#include "ricpp/ricontext/rimacroprims.h"
#endif _RICPP_RICONTEXT_RIMACROPRIMS_H

namespace RiCPP {

	inline RtFloat getDelta(RtFloat minVal, RtFloat maxVal, RtFloat tess)
	{
		assert(minVal <= maxVal);
		return (maxVal-minVal) / tess;
		// Can be less eps, even zero... (e.g. cone height 0)
	}	
		
	inline RtFloat getDeltaNotZero(RtFloat minVal, RtFloat maxVal, RtFloat tess)
	{
		assert(minVal <= maxVal);
		RtFloat delta = (maxVal-minVal) / tess;
		if ( delta < eps<RtFloat>() )
			return eps<RtFloat>();
		return delta;
	}	

	class CTesselator {
		std::list<CSurface *> m_surfaces;
	protected:
		virtual const CVarParamRManInterfaceCall &obj() const = 0;
	public:
		CTesselator();
		virtual ~CTesselator();

		bool releaseSurface(CSurface *surf);
		CSurface *createSurface();
	}; // CTesselator
	
	class CTriangulator : public CTesselator {
	public:
	}; // CTriangulator
	
	// =========================================================================
	
	class CBasePolygonTriangulator : public CTriangulator {
	protected:
		void triangleStrip(std::vector<IndexType> &strip, IndexType nVerts, IndexType offs) const;
		void insertParams(CFace &f, IndexType faceIdx, const CParameterList &plist, const std::vector<RtInt> &verts, IndexType nverts, IndexType vertsOffs);
	}; // CBasePolygonTriangulator
	
	class CPolygonTriangulator : public CBasePolygonTriangulator {
		CRiPolygon m_obj;
	protected:
		inline virtual const CVarParamRManInterfaceCall &obj() const { return m_obj; }
	public:
		inline CPolygonTriangulator(const CRiPolygon &obj) : m_obj(obj)  {}
		CSurface *triangulate();
	}; // CPolygonTriangulator

	class CPointsPolygonsTriangulator : public CBasePolygonTriangulator {
		CRiPointsPolygons m_obj;
	protected:
		inline virtual const CVarParamRManInterfaceCall &obj() const { return m_obj; }
	public:
		inline CPointsPolygonsTriangulator(const CRiPointsPolygons &obj) : m_obj(obj)  {}
		CSurface *triangulate();
	}; // CPointsPolygonsTriangulator

	class CGeneralPolygonTriangulator : public CBasePolygonTriangulator {
		CRiGeneralPolygon m_obj;
		const CTriangulatedPolygon *m_tpPtr;
	protected:
		inline virtual const CVarParamRManInterfaceCall &obj() const { return m_obj; }
	public:
		inline CGeneralPolygonTriangulator(const CRiGeneralPolygon &obj, const IPolygonTriangulationStrategy &strategy) : m_obj(obj)
		{
			m_tpPtr = m_obj.triangulate(strategy);
		}
		CSurface *triangulate();
	}; // CGeneralPolygonTriangulator

	class CPointsGeneralPolygonsTriangulator : public CBasePolygonTriangulator {
		CRiPointsGeneralPolygons m_obj;
		const std::vector<CTriangulatedPolygon> *m_tpPtr;
	protected:
		inline virtual const CVarParamRManInterfaceCall &obj() const { return m_obj; }
	public:
		inline CPointsGeneralPolygonsTriangulator(const CRiPointsGeneralPolygons &obj, const IPolygonTriangulationStrategy &strategy) : m_obj(obj)
		{
			m_tpPtr = &m_obj.triangulate(strategy);
		}
		CSurface *triangulate();
	}; // CPointsGeneralPolygonsTriangulator


	// =========================================================================

	class CParametricTriangulator : public CTriangulator {
	protected:
		void insertBilinearParams(IndexType faceIndex,
								  const IndexType (&cornerIdx)[4], const IndexType (&faceCornerIdx)[4],
								  RtInt tessU, RtInt tessV,
								  CFace &f);
		void insertBicubicParams(IndexType faceIndex,
								 const IndexType (&cornerIdx)[4], const IndexType (&faceCornerIdx)[4],
								 const IndexType (&controlIdx)[16], const IndexType (&faceControlIdx)[16],
								 RtInt tessU, RtInt tessV,
								 CFace &f);
	public:
		virtual CSurface *triangulate(const CDeclaration &posDecl, const CDeclaration &normDecl, RtInt tessU, RtInt tessV, bool equalOrientations, bool useStrips) = 0;
	}; // CParametricTriangulator

	// -------------------------------------------------------------------------

	class CQuadricTriangulator : public CParametricTriangulator {
	private:
		void insertParams(RtInt tessU, RtInt tessV, CFace &f);
	protected:
		virtual void buildPN(const CDeclaration &pointDecl, const CDeclaration &normDecl, RtInt tessU, RtInt tessV, bool equalOrientations, CFace &f) = 0;
	public:
		virtual CSurface *triangulate(const CDeclaration &posDecl, const CDeclaration &normDecl, RtInt tessU, RtInt tessV, bool equalOrientations, bool useStrips);
	}; // CQuadricTriangulator
	
	class CConeTriangulator : public CQuadricTriangulator {
		CRiCone m_obj;
	protected:
		virtual void buildPN(const CDeclaration &pointDecl, const CDeclaration &normDecl, RtInt tessU, RtInt tessV, bool equalOrientations, CFace &f);
		inline virtual const CVarParamRManInterfaceCall &obj() const { return m_obj; }
	public:
		inline CConeTriangulator(const CRiCone &obj) : m_obj(obj) {}
	}; // CConeTriangulator
	
	class CCylinderTriangulator : public CQuadricTriangulator {
		CRiCylinder m_obj;
	protected:
		virtual void buildPN(const CDeclaration &pointDecl, const CDeclaration &normDecl, RtInt tessU, RtInt tessV, bool equalOrientations, CFace &f);
		inline virtual const CVarParamRManInterfaceCall &obj() const { return m_obj; }
	public:
		inline CCylinderTriangulator(const CRiCylinder &obj) : m_obj(obj) {}
	}; // CCylinderTriangulator
	
	class CDiskTriangulator : public CQuadricTriangulator {
		CRiDisk m_obj;
	protected:
		virtual void buildPN(const CDeclaration &pointDecl, const CDeclaration &normDecl, RtInt tessU, RtInt tessV, bool equalOrientations, CFace &f);
		inline virtual const CVarParamRManInterfaceCall &obj() const { return m_obj; }
	public:
		inline CDiskTriangulator(const CRiDisk &obj) : m_obj(obj) {}
	}; // CDiskTriangulator

	class CHyperboloidTriangulator : public CQuadricTriangulator {
		CRiHyperboloid m_obj;
	protected:
		virtual void buildPN(const CDeclaration &pointDecl, const CDeclaration &normDecl, RtInt tessU, RtInt tessV, bool equalOrientations, CFace &f);
		inline virtual const CVarParamRManInterfaceCall &obj() const { return m_obj; }
	public:
		inline CHyperboloidTriangulator(const CRiHyperboloid &obj) : m_obj(obj) {}
	}; // CHyperboloidTriangulator

	class CParaboloidTriangulator : public CQuadricTriangulator {
		CRiParaboloid m_obj;
	protected:
		virtual void buildPN(const CDeclaration &pointDecl, const CDeclaration &normDecl, RtInt tessU, RtInt tessV, bool equalOrientations, CFace &f);
		inline virtual const CVarParamRManInterfaceCall &obj() const { return m_obj; }
	public:
		inline CParaboloidTriangulator(const CRiParaboloid &obj) : m_obj(obj) {}
	}; // CParaboloidTriangulator

	class CSphereTriangulator : public CQuadricTriangulator {
		CRiSphere m_obj;
	protected:
		virtual void buildPN(const CDeclaration &pointDecl, const CDeclaration &normDecl, RtInt tessU, RtInt tessV, bool equalOrientations, CFace &f);
		inline virtual const CVarParamRManInterfaceCall &obj() const { return m_obj; }
	public:
		inline CSphereTriangulator(const CRiSphere &obj) : m_obj(obj) {}
	}; // CSphereTriangulator

	class CTorusTriangulator : public CQuadricTriangulator {
		CRiTorus m_obj;
	protected:
		virtual void buildPN(const CDeclaration &pointDecl, const CDeclaration &normDecl, RtInt tessU, RtInt tessV, bool equalOrientations, CFace &f);
		inline virtual const CVarParamRManInterfaceCall &obj() const { return m_obj; }
	public:
		inline CTorusTriangulator(const CRiTorus &obj) : m_obj(obj) {}
	}; // CTorusTriangulator
	
	// -------------------------------------------------------------------------
	
	////////////////////////////////////////////////////////////////////////////////
	//! Basis values for bicubic spline patches of given parametric intervals
	class CUVVector {
	private:
		RtBasis m_uBasis;                 //!< Basis matrix used for the parametric direction u
		RtBasis m_vBasis;                 //!< Basis matrix used for the parametric direction v
		IndexType m_tessU;                //!< Number of segments for tesselation in parametric direction u
		IndexType m_tessV;                //!< Number of segments for tesselation in parametric direction v
		std::vector<RtFloat> m_uVector;   //!< Base values for direction u, one value per parameter value (m_tessU+1 values)
		std::vector<RtFloat> m_vVector;   //!< Base values for direction v, one value per parameter value (m_tessV+1 values)
		std::vector<RtFloat> m_duVector;  //!< First derivates of base values for direction u, one value per parameter value (m_tessU+1 values)
		std::vector<RtFloat> m_dvVector;  //!< First derivates of base values for direction v, one value per parameter value (m_tessV+1 values)
	public:
		//! Standard constructor, just clears the members
		CUVVector();
		
		//! Constructor, assings the parameters to the members 
		inline CUVVector(IndexType aTessU, IndexType aTessV, const RtBasis aUBasis, const RtBasis aVBasis)
		{
			reset(aTessU, aTessV, aUBasis, aVBasis);
		}
		
		//! Copy constructor, assings the members of another instance to the members of *this
		inline CUVVector(const CUVVector &v) { *this = v; }
		
		//! Duplication
		/*! @return A clone of *this
		 */
		inline CUVVector *duplicate() const
		{
			return new CUVVector(*this);
		}
		
		//! Assigns the members of an instance uvVector to the members of *this
		inline CUVVector &operator=(const CUVVector &uvVector)
		{
			if ( &uvVector == this )
				return *this;
			reset(uvVector.m_tessU, uvVector.m_tessV, uvVector.m_uBasis, uvVector.m_vBasis);
			return *this;
		}
		
		//! Resets the members of *this by the values of the parameters
		void reset(IndexType aTessU, IndexType aTessV, const RtBasis aUBasis, const RtBasis aVBasis);
		
		//! Returns true if the basis matrices equals to the appropriate parameters
		bool hasBasis(const RtBasis aUBasis, const RtBasis aVBasis) const;
		
		//! Basis matrix for parametric u
		inline const RtBasis &uBasis() const { return m_uBasis; }
		//! Basis matrix for parametric v
		inline const RtBasis &vBasis() const { return m_vBasis;	}
		
		//! Number of intervals in parametric direction u
		inline IndexType nu() const {return m_tessU+1;}
		
		//! Number of intervals in parametric direction v
		inline IndexType nv() const {return m_tessV+1;}
		
		//! Tesselation of the patch in parametric direction u
		inline IndexType tessU() const { return m_tessU; }
		
		//! Tesselation of the patch in parametric direction v
		inline IndexType tessV() const { return m_tessV; }
		
		//! Basis values in direction u
		inline const std::vector<RtFloat> &uVector() const { return m_uVector; }
		
		//! Basis values in direction v
		inline const std::vector<RtFloat> &vVector() const { return m_vVector; }
		
		//! Derivates of basis values in direction u
		inline const std::vector<RtFloat> &duVector() const { return m_duVector; }
		
		//! Derivates of basis values in direction v
		inline const std::vector<RtFloat> &dvVector() const { return m_dvVector; }

		void bicubicBlend(IndexType elemSize,
						  const IndexType (&controlIdx)[16],
						  const std::vector<RtFloat> &vals,
						  std::vector<RtFloat> &retvals);
		
		void bicubicBlendWithNormals(IndexType elemSize,
									 const IndexType (&controlIdx)[16],
									 const std::vector<RtFloat> &vals,
									 bool flipNormal,
									 std::vector<RtFloat> &retvals,
									 std::vector<RtFloat> &normals);
	}; // class CUVVector

	
	class CRootPatchTriangulator : public CParametricTriangulator {
	private:
		CRiBasis m_basis;
		CUVVector m_uvVector;
		inline CRootPatchTriangulator() {}
		void getFaceIdx(IndexType upatch, IndexType vpatch, IndexType nu, IndexType nv, IndexType patchsize, IndexType *idx) const;
	protected:
		inline CRootPatchTriangulator(const CRiBasis &aBasis) : m_basis(aBasis) {}
		virtual void buildBilinearPN(const CDeclaration &pointDecl, const CDeclaration &normDecl,
									 RtInt tessU, RtInt tessV,
									 bool equalOrientations,
									 IndexType faceIndex,
									 const IndexType (&cornerIdx)[4], const IndexType (&faceCornerIdx)[4],
									 CFace &f);
		virtual void buildBicubicPN(const CDeclaration &pointDecl, const CDeclaration &normDecl,
									RtInt tessU, RtInt tessV,
									bool equalOrientations,
									IndexType faceIndex,
									const IndexType (&cornerIdx)[4], const IndexType (&faceCornerIdx)[4],
									const IndexType (&controlIdx)[16], const IndexType (&faceControlIdx)[16],
									CFace &f);
		
		inline const CRiBasis &basis() const { return m_basis; }
		inline CRiBasis &basis() { return m_basis; }

		
		void getCornerIdx(IndexType upatch, IndexType vpatch, IndexType nu, IndexType nv, IndexType (&idx)[4]) const;
		void getFaceCornerIdx(IndexType upatch, IndexType vpatch, IndexType nu, IndexType nv, IndexType (&idx)[4]) const;
		void getControlIdx(IndexType upatch, IndexType vpatch, IndexType nu, IndexType nv, IndexType ustep, IndexType vstep, IndexType (&idx)[16]) const;
		void getFaceControlIdx(IndexType upatch, IndexType vpatch, IndexType nu, IndexType nv, IndexType (&idx)[16]) const;
	}; // CRootPatchTriangulator
	
	class CPatchTriangulator : public CRootPatchTriangulator {
		CRiPatch m_obj;
	protected:
		inline virtual const CVarParamRManInterfaceCall &obj() const { return m_obj; }
	public:
		inline CPatchTriangulator(const CRiPatch &obj, const CRiBasis &aBasis) : CRootPatchTriangulator(aBasis), m_obj(obj) {}
		CSurface *triangulate(const CDeclaration &posDecl, const CDeclaration &normDecl, RtInt tessU, RtInt tessV, bool equalOrientations, bool useStrips);
	}; // CPatchTriangulator

	class CPatchMeshTriangulator : public CRootPatchTriangulator {
		CRiPatchMesh m_obj;
	protected:
		inline virtual const CVarParamRManInterfaceCall &obj() const { return m_obj; }
	public:
		inline CPatchMeshTriangulator(const CRiPatchMesh &obj, const CRiBasis &aBasis) : CRootPatchTriangulator(aBasis), m_obj(obj) {}
		CSurface *triangulate(const CDeclaration &posDecl, const CDeclaration &normDecl, RtInt tessU, RtInt tessV, bool equalOrientations, bool useStrips);
	}; // CPatchMeshTriangulator
	
}

#endif // _RICPP_RICONTEXT_TRIANGULATION_H
