#ifndef _RICPP_RENDERCONTEXT_PARAMETRIC_H
#define _RICPP_RENDERCONTEXT_PARAMETRIC_H

// RICPP - RenderMan Interface CPP Language Binding
//         RenderMan is (R) by Pixar
//         Windows is (R) by Microsoft
//
// Copyright © 2001 - 2003, Andreas Pidde
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

/*! \file parametric.h
 *  \brief Declaration of the classes for parametric meshes.
 *  \author Andreas Pidde (andreas@pidde.de)
 */

#include "surfaces.h"


//! Basis values for bicubic spline patches of given parametric intervals
/*! The basis values are stored for caching in a TUVVectorCache instance. Since the coefficients won't change for
 *  different geometries. Multiplied with the geometry vectors, the bicubic patches result.
 */
class TUVVector {
protected:
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
	inline TUVVector() : m_tessU(0), m_tessV(0) {
		memset(&m_uBasis[0], 0, sizeof(m_uBasis));
		memset(&m_vBasis[0], 0, sizeof(m_vBasis));
	}

	//! Constructor, assings the parameters to the members 
	inline TUVVector(IndexType tessu, IndexType tessv, const RtBasis uBasis, const RtBasis vBasis) {
		reset(tessu, tessv, uBasis, vBasis);
	}

	//! Copy constructor, assings the members of another instance to the members of *this
	inline TUVVector(const TUVVector &v) { *this = v; }

	//! Destructor
	inline virtual ~TUVVector() {}

	//! Duplication
	/*! \return A clone of *this
	 */
	inline virtual TUVVector &duplicate() const {
		TUVVector *retVal = new TUVVector(*this);
		if ( !retVal ) {
			throw TSystemException(__FILE__, __LINE__);
		}
		return *retVal;
	}

	//! Assigns the members of an instance uvVector to the members of *this
	inline TUVVector &operator=(const TUVVector &uvVector) {
		if ( &uvVector == this )
			return *this;
		reset(uvVector.m_tessU, uvVector.m_tessV, uvVector.m_uBasis, uvVector.m_vBasis);
		return *this;
	}

	//! Resets the members of *this by the values of the parameters
	void reset(IndexType tessu, IndexType tessv, const RtBasis uBasis, const RtBasis vBasis);

	//! Returns true if the basis matrices equals to the appropriate parameters
	inline bool hasBasis(const RtBasis uBasis, const RtBasis vBasis) const {
		if ( memcmp(m_uBasis, uBasis, sizeof(RtBasis)) != 0 )
			return false;
		if ( memcmp(m_vBasis, vBasis, sizeof(RtBasis)) != 0 )
			return false;
		return true;
	}

	//! Number of intervals in parametric direction u
	inline virtual IndexType nu() const {return m_tessU+1;}

	//! Number of intervals in parametric direction v
	inline virtual IndexType nv() const {return m_tessV+1;}

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

	//! Blends RenderMan parameters (e.g. RI_P) by the basis values
	void bicubicBlend(
		const IndexType (& idx)[16],
		const TParameter &source,
		TNamedVector<RtFloat> &dest,
		bool clearAndReserve) const;

	//! Blends RenderMan parameters (e.g. RI_P) by the basis values
	void bicubicBlend(
		const IndexType (& idx)[16],
		const char *name,
		IndexType components,
		IndexType cardinality,
		const RtFloat *source,
		TNamedVector<RtFloat> &dest,
		bool clearAndReserve) const;

	//! Returns the normals for a patch
	void bicubicBlendNormals(
		const IndexType (& idx)[16],
		const char *name,
		IndexType components,
		IndexType cardinality,
		const RtFloat *source,
		TNamedVector<RtFloat> &dest,
		bool clearAndReserve) const;
}; // class TUVVector


//! Caches instances of TUVVector
class TUVVectorCache {
	std::deque<TUVVector> m_cache; //!< Cached TUVVector instances
	IndexType m_lastIndex;         //!< Index of the instance matched previously (valid only if m_lastIndex < m_cache.size() - so init with 0 is ok)
public:
	//! Standard constructor, clears all members
	inline TUVVectorCache() : m_lastIndex(0) {}
	//! Returns the TUVVector for given basis matrices and tesselations, creates and caches a TUVVector instance if not found in m_cache
	TUVVector &findUVVector(IndexType tessu, IndexType tessv, const RtBasis &uBasis, const RtBasis &vBasis);
}; // TUVVectorCache


////////////////////////////////////////////////////////////////////////////////
/*! TBSplineBasis is a class to calculate a bspline basis and its first derivate
 *  using the Cox-de Boor recurrence formular.
 */
class TBSplineBasis {
public:
	RtInt ncpts;                //!< Number of control points
	RtInt order;                //!< Order of the polynom (degree = k-1), 0 < k <= n
	std::vector<RtFloat> knots; //!< Knot vector, size = n+k, knots[i] <= knots[i+1]
	RtFloat tmin,               //!< Minimum value for parameter t, tmin >= x[order-1] && tmin < tmax
	        tmax;               //!< Maximum value for parameter t, tmax <= x[ncpts] && tmax > tmin
	RtInt tess;                 //!< Tesselation of the parameter

	// Results
	RtInt segments;             //!< Number of segments of the bspline

	std::vector<RtFloat> tVals; //!< Parameters used to teselate the spline
	std::vector<RtInt> valCnts; //!< Number of parameters per segment

	std::vector<RtFloat> basis;      //! bspline basis for parameters in tVals (order values per parameter)
	std::vector<RtFloat> basisDeriv; //! bspline basis first derivate for parameters in tVals (order values per parameter)

protected:
	/*! Calculates the bspline basis a parameter value t in a span
	 *  and returns the basis in N[span-order...span] and the first derivate in Nd[span-order...span].
	 *  Algorithm based on the Cox-de Boor recurrence formular.
	 *
	 *  assert( !(k > n) );
	 *  assert( !(x.size() < (std::vector<RtFloat>::size_type)size+1) );
	 *  assert( !(N.size() < (std::vector<RtFloat>::size_type)size) );
	 *  assert( !(Nd.size() < (std::vector<RtFloat>::size_type)size) );
	 *  assert( !(t > tmax) );
	 */
	void bsplineBasisDerivate(RtFloat t, RtInt span,
		std::vector<RtFloat> &N, std::vector<RtFloat> &Nd);

	// ! test if the member variables are valid for bspline-calculation
	bool unvalidParams();

	// ! calc all basis values
	void calc();

public:
	//! Standard constructor, simpliy clears the members
	inline TBSplineBasis() {
		ncpts = 0;
		order = 0;
		tmin = (RtFloat)0;
		tmax = (RtFloat)0;
		tess = 0;
		segments = 0;
	}

	TBSplineBasis(RtInt n, RtInt k, const std::vector<RtFloat> &x, RtFloat tmini, RtFloat tmaxi, RtInt tesselation);

	inline TBSplineBasis(const TBSplineBasis &sp) {
		*this = sp;
	}

	inline TBSplineBasis &duplicate() const {
		TBSplineBasis *cp = new TBSplineBasis(*this);
		if ( cp == NULL ) {
			throw TSystemException(__FILE__, __LINE__);
		}
		return *cp;
	}

	TBSplineBasis &operator=(const TBSplineBasis &sp);

	reset(RtInt n, RtInt k, const std::vector<RtFloat> &x, RtFloat tmini, RtFloat tmaxi, RtInt tesselation);
}; // TBSplineBasis

class TNuBasis {
	RtInt m_n;      //!< Number of control points
	RtInt m_order;  //!< Order of the polygon
	std::vector<RtFloat> m_values; //!< Value for each parameter

public:
	inline TNuBasis(RtInt p_n, RtInt p_order) : m_n(p_n), m_order(p_order) {
		if ( m_order > p_n )
			m_order = p_n;
	}

	static void Cox_DeBoor_derivate(RtInt i, RtInt k, RtInt deg, RtFloat t, RtFloat tmax, RtInt n, const std::vector<RtFloat> &knotvec, std::vector<RtFloat> &N);
	static void Cox_DeBoor(RtInt i, RtInt k, RtInt deg, RtFloat t, RtFloat tmax, RtInt n, const std::vector<RtFloat> &knotvec, std::vector<RtFloat> &N);

	static RtFloat derivative2(RtInt i, RtInt k, RtInt deg, RtInt degMax, RtFloat t, RtFloat tmax, const std::vector<RtFloat> &knotvec);
	static RtFloat derivative1(RtInt i, RtInt deg, RtFloat t, RtFloat tmax, const std::vector<RtFloat> &knotvec);
	static RtFloat derivative(RtInt i, RtInt deg, RtFloat t, RtFloat tmax, const std::vector<RtFloat> &knotvec);

	inline RtInt degree() const { return m_order-1; }
	inline RtInt order() const { return m_order; }
	inline RtInt n() const { return m_n; }

	void calc(IndexType tess, RtFloat tmin, RtFloat tmax, const std::vector<RtFloat> &knotvec, bool isDerivate);

	inline const RtFloat &operator[] (IndexType idx) const {
		assert(idx < m_values.size());
		return m_values[idx];
	}

	inline IndexType size() const { return (IndexType)m_values.size(); }
}; // TNuBasis


class TUVPatch : public TPrimFace {
protected:
	IndexType m_tessU, //!< Tesselation in u direction, number of vertices in u direction is m_tessU+1
	          m_tessV; //!< Tesselation in v direction, number of vertices in v direction is m_tessV+1

	inline virtual void fillCornerIdx(IndexType *idx, RtInt nu=2, RtInt nv=2, RtInt upatch=0, RtInt vpatch=0) const {
		idx[0] = (vpatch % nv)       * nu + (upatch % nu);
		idx[1] = (vpatch % nv)       * nu + ((upatch + 1) % nu);
		idx[2] = ((vpatch + 1) % nv) * nu + (upatch % nu);
		idx[3] = ((vpatch + 1) % nv) * nu + ((upatch + 1) % nu);
	}
public:
	inline TUVPatch() : m_tessU(0), m_tessV(0) {}
	inline TUVPatch(const TUVPatch &uvPatch) { *this = uvPatch; }

	//! Duplication
	/*! \return A clone of *this
	 */
	inline virtual TPrimFace &duplicate() const {
		TPrimFace *retVal = new TUVPatch(*this);
		if ( !retVal ) {
			throw TSystemException(__FILE__, __LINE__);
		}
		return *retVal;
	}

	virtual const RtFloat *getTextureST(TNamedVector<RtFloat> &setST, IndexType &size, RtFloat s1, RtFloat t1, RtFloat s2, RtFloat t2, RtFloat s3, RtFloat t3, RtFloat s4, RtFloat t4);
	void resizeVerticesBy(RtFloat r);

	TUVPatch &operator=(const TUVPatch &uvPatch);

	virtual void insertVertexData(const TParameterMap &pm, RtInt nu=2, RtInt nv=2, RtInt upatch=0, RtInt vpatch=0);
	virtual void insertNormalData();
	virtual void insertVertexIndexData();

	inline virtual IndexType nu() const {return m_tessU+1;}
	inline virtual IndexType nv() const {return m_tessV+1;}

	inline IndexType tessU() const { return m_tessU; }
	inline IndexType tessV() const { return m_tessV; }
	inline IndexType tessU(IndexType u) { IndexType pu = m_tessU; m_tessU = u; return pu; }
	inline IndexType tessV(IndexType v) { IndexType pv = m_tessV; m_tessV = v; return pv; }
}; // class TUVPatch


template<class _T> inline _T &findUV(std::deque<_T> &uvMeshDeque, IndexType tessU, IndexType tessV, IndexType &idx, bool &isNew, IndexType numMesh=1) {
	isNew = false;

	if ( idx < (IndexType)uvMeshDeque.size() ) {
		_T &meshRef = uvMeshDeque[idx];
		if ( meshRef.tessU() == tessU && meshRef.tessV() == tessV )
			return meshRef;
	}

	typename std::deque<_T>::const_iterator i;
	IndexType cnt = 0, j;
	for ( i = uvMeshDeque.begin(); i != uvMeshDeque.end(); cnt += numMesh ) {
		if ( (*i).tessU() == tessU && (*i).tessV() == tessV ) {
			idx = cnt;
			return uvMeshDeque[cnt];
		}
		for ( j = 0; j< numMesh; j++, i++ );
	}

	isNew = true;
	idx = uvMeshDeque.size();
	IndexType savIdx=idx;
	uvMeshDeque.resize(uvMeshDeque.size()+numMesh);
	for ( ; savIdx < uvMeshDeque.size(); ++savIdx ) {
		_T &mesh = uvMeshDeque[savIdx];
		mesh.tessU(tessU);
		mesh.tessV(tessV);
	}

	_T &retVal = uvMeshDeque[idx];
	return retVal;
}


template<class _T> inline _T &findBicubicUV(std::deque<_T> &uvMeshDeque, IndexType tessU, IndexType tessV, const RtBasis &uBasis, const RtBasis &vBasis, IndexType &idx, bool &isNew) {
	isNew = false;

	if ( idx < (IndexType)uvMeshDeque.size() ) {
		_T &meshRef = uvMeshDeque[idx];
		if ( meshRef.tessU() == tessU && meshRef.tessV() == tessV && meshRef.hasBasis(uBasis, vBasis)  )
			return meshRef;
	}

	typename std::deque<_T>::const_iterator i;
	IndexType cnt = 0;
	for ( i = uvMeshDeque.begin(); i != uvMeshDeque.end(); i++, cnt++ ) {
		if ( (*i).tessU() == tessU && (*i).tessV() == tessV && (*i).hasBasis(uBasis, vBasis) ) {
			idx = cnt;
			return uvMeshDeque[cnt];
		}
	}

	isNew = true;
	idx = cnt;
	uvMeshDeque.resize(uvMeshDeque.size()+1);
	_T &retVal = uvMeshDeque.back();
	retVal.reset(tessU, tessV, uBasis, vBasis);
	return retVal;
}


class TQuadric : public TUVPatch {
public:
	inline TQuadric() {}
	inline TQuadric(const TQuadric &quadric) {
		*this = quadric;
	}

	//! Duplication
	/*! \return A clone of *this
	 */
	inline virtual TPrimFace &duplicate() const {
		TPrimFace *retVal = new TQuadric(*this);
		if ( !retVal ) {
			throw TSystemException(__FILE__, __LINE__);
		}
		return *retVal;
	}
	inline TQuadric &operator=(const TQuadric &quadric) {
		TUVPatch::operator=(quadric);
		return *this;
	}
}; // class TQuadric


class TSphereMesh : public TQuadric {
public:
	inline TSphereMesh() {}
	inline TSphereMesh(const TSphereMesh &sphereMesh) {
		*this = sphereMesh;
	}
	//! Duplication
	/*! \return A clone of *this
	 */
	inline virtual TPrimFace &duplicate() const {
		TPrimFace *retVal = new TSphereMesh(*this);
		if ( !retVal ) {
			throw TSystemException(__FILE__, __LINE__);
		}
		return *retVal;
	}
	//! Creates the grid points and normals of a sphere
	/*! \param tessU Number in tesselations in u directions.
	 *  \param tessV Number in tesselations in v directions.
	 *  \param radius Radius of the sphere
	 *  \param zmin Minimum in z 
	 *  \param zmax Maximum in z
	 *  \param thetamax Rotation about z
	 */
	void createPN(IndexType tessU, IndexType tessV, RtFloat radius, RtFloat zmin, RtFloat zmax, RtFloat thetamax);
	inline TSphereMesh &operator=(const TSphereMesh &sphereMesh) {
		TQuadric::operator=(sphereMesh);
		return *this;
	}
}; // class TSphereMesh


class TCylinderMesh : public TQuadric {
public:
	inline TCylinderMesh() {}
	inline TCylinderMesh(const TCylinderMesh &cylinderMesh) {
		*this = cylinderMesh;
	}
	//! Duplication
	/*! \return A clone of *this
	 */
	inline virtual TPrimFace &duplicate() const {
		TPrimFace *retVal = new TCylinderMesh(*this);
		if ( !retVal ) {
			throw TSystemException(__FILE__, __LINE__);
		}
		return *retVal;
	}
	//! Creates the grid points and normals of a cylinder.
	/*! \param tessU Number in tesselations in u directions.
	 *  \param tessV Number in tesselations in v directions.
	 *  \param radius Radius of the sphere
	 *  \param zmin Minimum in z 
	 *  \param zmax Maximum in z
	 *  \param thetamax Rotation about z
	 */
	void createPN(IndexType tessU, IndexType tessV, RtFloat radius, RtFloat zmin, RtFloat zmax, RtFloat thetamax);
	inline TCylinderMesh &operator=(const TCylinderMesh &cylinderMesh) {
		TQuadric::operator=(cylinderMesh);
		return *this;
	}
}; // class TCylinderMesh


class TParaboloidMesh : public TQuadric {
public:
	inline TParaboloidMesh() {}
	inline TParaboloidMesh(const TParaboloidMesh &paraboloidMesh) {
		*this = paraboloidMesh;
	}
	//! Duplication
	/*! \return A clone of *this
	 */
	inline virtual TPrimFace &duplicate() const {
		TPrimFace *retVal = new TParaboloidMesh(*this);
		if ( !retVal ) {
			throw TSystemException(__FILE__, __LINE__);
		}
		return *retVal;
	}
	//! Creates the grid points and normals of a paraboloid.
	/*! \param tessU Number in tesselations in u directions.
	 *  \param tessV Number in tesselations in v directions.
	 *  \param rmax Radius of the paraboloid
	 *  \param zmin Minimum in z 
	 *  \param zmax Maximum in z
	 *  \param thetamax Rotation about z
	 */
	void createPN(IndexType tessU, IndexType tessV, RtFloat rmax, RtFloat zmin, RtFloat zmax, RtFloat thetamax);
	inline TParaboloidMesh &operator=(const TParaboloidMesh &paraboloidMesh) {
		TQuadric::operator=(paraboloidMesh);
		return *this;
	}
}; // class TParaboloidMesh


class TConeMesh : public TQuadric {
public:
	inline TConeMesh() {}
	inline TConeMesh(const TConeMesh &coneMesh) {
		*this = coneMesh;
	}
	//! Duplication
	/*! \return A clone of *this
	 */
	inline virtual TPrimFace &duplicate() const {
		TPrimFace *retVal = new TConeMesh(*this);
		if ( !retVal ) {
			throw TSystemException(__FILE__, __LINE__);
		}
		return *retVal;
	}
	//! Creates the grid points and normals of a cone.
	/*! \param tessU Number in tesselations in u directions.
	 *  \param tessV Number in tesselations in v directions.
	 *  \param height Radius of the cone
	 *  \param radius Radius of the cone
	 *  \param thetamax Rotation about z
	 *  \param displace Displacement in z direction
	 */
	void createPN(IndexType tessU, IndexType tessV, RtFloat height, RtFloat radius, RtFloat thetamax, RtFloat displacement);
	inline TConeMesh &operator=(const TConeMesh &coneMesh) {
		TQuadric::operator=(coneMesh);
		return *this;
	}
}; // class TConeMesh


class TTorusMesh : public TQuadric {
public:
	inline TTorusMesh() {}
	inline TTorusMesh(const TTorusMesh &torusMesh) {
		*this = torusMesh;
	}
	//! Duplication
	/*! \return A clone of *this
	 */
	inline virtual TPrimFace &duplicate() const {
		TPrimFace *retVal = new TTorusMesh(*this);
		if ( !retVal ) {
			throw TSystemException(__FILE__, __LINE__);
		}
		return *retVal;
	}
	//! Creates the grid points and normals of a torus.
	/*! \param tessU Number in tesselations in u directions.
	 *  \param tessV Number in tesselations in v directions.
	 *  \param majorrad Major radius of torus
	 *  \param minorrad Minor radius of the torus
	 *  \param phimin Minimum radius in parametric v
	 *  \param phimax Maximum radius in parametric v
	 *  \param thetamax Rotation in parametric u
	 */
	void createPN(IndexType tessU, IndexType tessV, RtFloat majorrad, RtFloat minorrad, RtFloat phimin, RtFloat phimax, RtFloat thetamax);
	inline TTorusMesh &operator=(const TTorusMesh &torusMesh) {
		TQuadric::operator=(torusMesh);
		return *this;
	}
}; // class TTorusMesh


class THyperboloidMesh : public TQuadric {
public:
	inline THyperboloidMesh() {}
	inline THyperboloidMesh(const THyperboloidMesh &hyperboloidMesh) {
		*this = hyperboloidMesh;
	}
	//! Duplication
	/*! \return A clone of *this
	 */
	inline virtual TPrimFace &duplicate() const {
		TPrimFace *retVal = new THyperboloidMesh(*this);
		if ( !retVal ) {
			throw TSystemException(__FILE__, __LINE__);
		}
		return *retVal;
	}
	//! Creates the grid points and normals of a hyperboloid.
	/*! \param tessU Number in tesselations in u directions.
	 *  \param tessV Number in tesselations in v directions.
	 *  \param point1 Startpoint of the line to rotate
	 *  \param point2 Endpoint of the line to rotate
	 *  \param thetamax Rotation about z
	 */
	void createPN(IndexType tessU, IndexType tessV, RtPoint point1, RtPoint point2, RtFloat thetamax);
	inline THyperboloidMesh &operator=(const THyperboloidMesh &hyperboloidMesh) {
		TQuadric::operator=(hyperboloidMesh);
		return *this;
	}
}; // class THyperboloidMesh


class TBilinearMesh : public TUVPatch {
public:
	inline TBilinearMesh() {}
	inline TBilinearMesh(const TBilinearMesh &patchMesh) {
		*this = patchMesh;
	}
	//! Duplication
	/*! \return A clone of *this
	 */
	inline virtual TPrimFace &duplicate() const {
		TPrimFace *retVal = new TBilinearMesh(*this);
		if ( !retVal ) {
			throw TSystemException(__FILE__, __LINE__);
		}
		return *retVal;
	}
	inline TBilinearMesh &operator=(const TBilinearMesh &patchMesh) {
		TUVPatch::operator=(patchMesh);
		return *this;
	}

	virtual void insertVertexData(const TParameterMap &pm, RtInt nu=2, RtInt nv=2, RtInt upatch=0, RtInt vpatch=0);
}; // class TBilinearMesh


class TBicubicMesh : public TUVPatch {
protected:
	RtBasis m_uBasis;
	RtBasis m_vBasis;
public:
	inline TBicubicMesh() {}
	inline TBicubicMesh(const TBicubicMesh &patchMesh) {
		*this = patchMesh;
	}
	//! Duplication
	/*! \return A clone of *this
	 */
	inline virtual TPrimFace &duplicate() const {
		TPrimFace *retVal = new TBicubicMesh(*this);
		if ( !retVal ) {
			throw TSystemException(__FILE__, __LINE__);
		}
		return *retVal;
	}
	inline TBicubicMesh &operator=(const TBicubicMesh &patchMesh) {
		memcpy(m_uBasis, patchMesh.m_uBasis, sizeof(RtBasis));
		memcpy(m_vBasis, patchMesh.m_vBasis, sizeof(RtBasis));
		TUVPatch::operator=(patchMesh);
		return *this;
	}
	inline void basis(const RtBasis &ubasis, const RtBasis &vbasis) {
		memcpy(m_uBasis, ubasis, sizeof(RtBasis));
		memcpy(m_vBasis, vbasis, sizeof(RtBasis));
	}
	inline bool hasBasis(const RtBasis &ubasis, const RtBasis &vbasis) const {
		if ( memcmp(m_uBasis, ubasis, sizeof(RtBasis)) != 0 )
			return false;
		if ( memcmp(m_vBasis, vbasis, sizeof(RtBasis)) != 0 )
			return false;
		return true;
	}
	// inline const RtBasis &ubasis() const { return m_uBasis; }
	// inline const RtBasis &vbasis() const { return m_vBasis; }
	virtual void insertVertexDataUV(
		const TParameterMap &pm, const TUVVector &uv,
		RtInt nu=4, RtInt nv=4,
		RtInt nu_bilin=2, RtInt nv_bilin=2,
		RtInt upatch=0, RtInt vpatch=0,
		RtInt ustep=3, RtInt vstep=3);

	inline virtual void insertVertexData(
		const TParameterMap &pm,
		RtInt nu=4, RtInt nv=4,
		RtInt nu_bilin=2, RtInt nv_bilin=2,
		RtInt upatch=0, RtInt vpatch=0,
		RtInt ustep=3, RtInt vstep=3)
	{
		TUVVector uv(m_tessU, m_tessV, m_uBasis, m_uBasis);
		insertVertexDataUV(pm, uv, nu, nv, nu_bilin, nv_bilin, upatch, vpatch, ustep, vstep);
	}

	inline void reset(IndexType tessU, IndexType tessV, const RtBasis &uBasis, const RtBasis &vBasis) {
		TUVPatch::tessU(tessU);
		TUVPatch::tessV(tessV);
		basis(uBasis, vBasis);
	}
}; // class TBicubicMesh


class TNuMesh : public TUVPatch {
public:
	inline TNuMesh() {}
	inline TNuMesh(const TNuMesh &patchMesh) {
		*this = patchMesh;
	}
	//! Duplication
	/*! \return A clone of *this
	 */
	inline virtual TPrimFace &duplicate() const {
		TPrimFace *retVal = new TNuMesh(*this);
		if ( !retVal ) {
			throw TSystemException(__FILE__, __LINE__);
		}
		return *retVal;
	}
	inline TNuMesh &operator=(const TNuMesh &patchMesh) {
		TUVPatch::operator=(patchMesh);
		return *this;
	}

	void nuBlend(
		const TParameter &source,
		TNamedVector<RtFloat> &dest,
		RtInt pnu,
		RtInt pnv,
		const TNuBasis &uBasis,
		const TNuBasis &vBasis
		) const;

	void nuBlend(
		const char *name,
		IndexType components,
		IndexType cardinality,
		const RtFloat *source,
		TNamedVector<RtFloat> &dest,
		RtInt pnu,
		RtInt pnv,
		const TNuBasis &uBasis,
		const TNuBasis &vBasis,
		bool clearAndReserve) const;

	void nuBlendNormals(
		const TParameter &source,
		TNamedVector<RtFloat> &dest,
		RtInt pnu,
		RtInt pnv,
		const TNuBasis &uBasis,
		const TNuBasis &vBasis,
		const TNuBasis &duBasis,
		const TNuBasis &dvBasis
		) const;

	void nuBlendNormals(
		const char *name,
		IndexType components,
		IndexType cardinality,
		const RtFloat *source,
		TNamedVector<RtFloat> &dest,
		RtInt pnu,
		RtInt pnv,
		const TNuBasis &uBasis,
		const TNuBasis &vBasis,
		const TNuBasis &duBasis,
		const TNuBasis &dvBasis,
		bool clearAndReserve) const;

	virtual void insertVertexData(const TParameterMap &pm,
		RtInt nu, RtInt nv, IndexType tessU, IndexType tessV,
		const TNuBasis &uBasis, const TNuBasis &vBasis,
		const TNuBasis &duBasis, const TNuBasis &dvBasis
		);
}; // class TNuMesh


class TUnitCircleCache {
public:
	void getUnitCircle(std::vector<RtFloat> &circledata, IndexType tess, RtFloat thetamin, RtFloat thetamax);
	void getUnitCircle(std::vector<RtFloat> &circledata, IndexType tess, RtFloat thetamax);
}; // class TUnitCircle

class TUnitSphereCache  {
	IndexType m_lastIndex;
	std::deque<TSphereMesh> m_meshData;     //!< Meshes for various tesselations.
public:
	inline TUnitSphereCache() : m_lastIndex(0) {}
	const TSphereMesh &getUnitSphereRef(IndexType tessU, IndexType tessV);
	void getUnitSphere(TSphereMesh &sphere, IndexType tessU, IndexType tessV);
}; // class TUnitSphere

class TSphere : public TRiSphere {
	std::deque<TSphereMesh> m_meshData;     //!< Meshes for various tesselations.
	IndexType m_lastIndex;
public:
	inline TSphere(
		long aLineNo, TDeclarationMap &declmap, RtInt colorComps,
		RtFloat radius, RtFloat zmin, RtFloat zmax, RtFloat thetamax,
		RtInt n, RtToken tokens[], RtPointer params[])
		: TRiSphere(
			aLineNo, declmap, colorComps,
			radius, zmin, zmax, thetamax,
			n, tokens, params),
		  m_lastIndex(0)
	{}

	//! Tesselation of the sphere.
	/*! \param tessU Number in tesselations in u directions.
	 *  \param tessV Number in tesselations in v directions.
	 */
	TSphereMesh &tesselate(IndexType tessU, IndexType tessV);

	inline virtual void replay(TRi &ri, TRiCallbackInterface &ricb, TDrawPrim *dp) {
		if ( dp ) {
			dp->drawGeometricPrim(tesselate((IndexType)ricb.curUTesselation(), (IndexType)ricb.curVTesselation()));
		} else {
			TRiSphere::replay(ri);
		}
	}
}; // class TSphere


class TCylinder : public TRiCylinder {
	std::deque<TCylinderMesh> m_meshData;     //!< Meshes for various tesselations.
	IndexType m_lastIndex;
public:
	inline TCylinder(
		long aLineNo, TDeclarationMap &declmap, RtInt colorComps,
		RtFloat radius, RtFloat zmin, RtFloat zmax, RtFloat thetamax,
		RtInt n, RtToken tokens[], RtPointer params[])
		: TRiCylinder(
			aLineNo, declmap, colorComps,
			radius, zmin, zmax, thetamax,
			n, tokens, params),
		  m_lastIndex(0)
	{}

	//! Tesselation of the torus.
	/*! \param tessU Number in tesselations in u directions.
	 *  \param tessV Number in tesselations in v directions.
	 */
	TCylinderMesh &tesselate(IndexType tessU, IndexType tessV);

	inline virtual void replay(TRi &ri, TRiCallbackInterface &ricb, TDrawPrim *dp) {
		if ( dp ) {
			dp->drawGeometricPrim(tesselate((IndexType)ricb.curUTesselation(), (IndexType)ricb.curVTesselation()));
		} else {
			TRiCylinder::replay(ri);
		}
	}
}; // class TCylider


class TParaboloid : public TRiParaboloid {
	std::deque<TParaboloidMesh> m_meshData;     //!< Meshes for various tesselations.
	IndexType m_lastIndex;
public:
	inline TParaboloid(
		long aLineNo, TDeclarationMap &declmap, RtInt colorComps,
		RtFloat rmax, RtFloat zmin, RtFloat zmax, RtFloat thetamax,
		RtInt n, RtToken tokens[], RtPointer params[])
		: TRiParaboloid(
			aLineNo, declmap, colorComps,
			rmax, zmin, zmax, thetamax,
			n, tokens, params),
		  m_lastIndex(0)
	{}

	//! Tesselation of the paraboloid.
	/*! \param tessU Number in tesselations in u directions.
	 *  \param tessV Number in tesselations in v directions.
	 */
	TParaboloidMesh &tesselate(IndexType tessU, IndexType tessV);

	inline virtual void replay(TRi &ri, TRiCallbackInterface &ricb, TDrawPrim *dp) {
		if ( dp ) {
			dp->drawGeometricPrim(tesselate((IndexType)ricb.curUTesselation(), (IndexType)ricb.curVTesselation()));
		} else {
			TRiParaboloid::replay(ri);
		}
	}
}; // class TParaboloid


class TCone : public TRiCone {
	std::deque<TConeMesh> m_meshData;     //!< Meshes for various tesselations.
	IndexType m_lastIndex;
public:
	inline TCone(
		long aLineNo, TDeclarationMap &declmap, RtInt colorComps,
		RtFloat height, RtFloat radius, RtFloat thetamax,
		RtInt n, RtToken tokens[], RtPointer params[])
		: TRiCone(
			aLineNo, declmap, colorComps,
			height, radius, thetamax,
			n, tokens, params),
		  m_lastIndex(0)
	{}

	//! Tesselation of the cone.
	/*! \param tessU Number in tesselations in u directions.
	 *  \param tessV Number in tesselations in v directions.
	 */
	TConeMesh &tesselate(IndexType tessU, IndexType tessV);

	inline virtual void replay(TRi &ri, TRiCallbackInterface &ricb, TDrawPrim *dp) {
		if ( dp ) {
			dp->drawGeometricPrim(tesselate((IndexType)ricb.curUTesselation(), (IndexType)ricb.curVTesselation()));
		} else {
			TRiCone::replay(ri);
		}
	}
}; // class TCone


class TDisk : public TRiDisk {
	std::deque<TConeMesh> m_meshData;     //!< Meshes for various tesselations, disk is a special form of a cone
	IndexType m_lastIndex;
public:
	inline TDisk(
		long aLineNo, TDeclarationMap &declmap, RtInt colorComps,
		RtFloat height, RtFloat radius, RtFloat thetamax,
		RtInt n, RtToken tokens[], RtPointer params[])
		: TRiDisk(
			aLineNo, declmap, colorComps,
			height, radius, thetamax,
			n, tokens, params),
		  m_lastIndex(0)
	{}

	//! Tesselation of the disk.
	/*! \param tessU Number in tesselations in u directions.
	 *  \param tessV Number in tesselations in v directions.
	 */
	TConeMesh &tesselate(IndexType tessU, IndexType tessV);

	inline virtual void replay(TRi &ri, TRiCallbackInterface &ricb, TDrawPrim *dp) {
		if ( dp ) {
			dp->drawGeometricPrim(tesselate((IndexType)ricb.curUTesselation(), (IndexType)ricb.curVTesselation()));
		} else {
			TRiDisk::replay(ri);
		}
	}
}; // class TDisk


class TTorus : public TRiTorus {
	std::deque<TTorusMesh> m_meshData;     //!< Meshes for various tesselations
	IndexType m_lastIndex;
public:
	inline TTorus(
		long aLineNo, TDeclarationMap &declmap, RtInt colorComps,
		RtFloat majorrad, RtFloat minorrad, RtFloat phimin, RtFloat phimax, RtFloat thetamax,
		RtInt n, RtToken tokens[], RtPointer params[])
		: TRiTorus(
			aLineNo, declmap, colorComps,
			majorrad, minorrad, phimin, phimax, thetamax,
			n, tokens, params),
		  m_lastIndex(0)
	{}

	//! Tesselation of the torus.
	/*! \param tessU Number in tesselations in u directions.
	 *  \param tessV Number in tesselations in v directions.
	 */
	TTorusMesh &tesselate(IndexType tessU, IndexType tessV);

	inline virtual void replay(TRi &ri, TRiCallbackInterface &ricb, TDrawPrim *dp) {
		if ( dp ) {
			dp->drawGeometricPrim(tesselate((IndexType)ricb.curUTesselation(), (IndexType)ricb.curVTesselation()));
		} else {
			TRiTorus::replay(ri);
		}
	}
}; // class TTorus


class THyperboloid : public TRiHyperboloid {
	std::deque<THyperboloidMesh> m_meshData;     //!< Meshes for various tesselations
	IndexType m_lastIndex;
public:
	inline THyperboloid(
		long aLineNo, TDeclarationMap &declmap, RtInt colorComps,
		RtPoint point1, RtPoint point2, RtFloat thetamax,
		RtInt n, RtToken tokens[], RtPointer params[])
		: TRiHyperboloid(
			aLineNo, declmap, colorComps,
			point1, point2, thetamax,
			n, tokens, params),
		  m_lastIndex(0)
	{}

	//! Tesselation of the hyperboloid.
	/*! \param tessU Number in tesselations in u directions.
	 *  \param tessV Number in tesselations in v directions.
	 */
	THyperboloidMesh &tesselate(IndexType tessU, IndexType tessV);
	inline virtual void replay(TRi &ri, TRiCallbackInterface &ricb, TDrawPrim *dp) {
		if ( dp ) {
			dp->drawGeometricPrim(tesselate((IndexType)ricb.curUTesselation(), (IndexType)ricb.curVTesselation()));
		} else {
			TRiHyperboloid::replay(ri);
		}
	}
}; // class THyperboloid


class TBilinearPatch : public TRiPatch {
	std::deque<TBilinearMesh> m_bilinearMeshes;     //!< Meshes for various tesselations and basis
	IndexType m_lastIndex;
public:
	inline TBilinearPatch(
		long aLineNo, TDeclarationMap &declmap, RtInt colorComps,
		RtInt n, RtToken tokens[], RtPointer params[])
		: TRiPatch(
			aLineNo, declmap, colorComps,
			TRi::RI_BILINEAR,
			n, tokens, params),
		  m_lastIndex(0)
	{
	}

	//! Tesselation of the patch.
	/*! \param tessU Number in tesselations in u directions.
	 *  \param tessV Number in tesselations in v directions.
	 */
	TBilinearMesh &tesselate(IndexType tessU, IndexType tessV);

	inline virtual void replay(TRi &ri, TRiCallbackInterface &ricb, TDrawPrim *dp) {
		if ( dp ) {
			dp->drawGeometricPrim(tesselate((IndexType)ricb.curUTesselation(), (IndexType)ricb.curVTesselation()));
		} else {
			TRiPatch::replay(ri);
		}
	}
}; // class TBilinearPatch


class TBicubicPatch : public TRiPatch {
	std::deque<TBicubicMesh> m_bicubicMeshes;     //!< Meshes for various tesselations and basis
	IndexType m_lastIndex;
public:
	inline TBicubicPatch(
		long aLineNo, TDeclarationMap &declmap, RtInt colorComps,
		RtInt n, RtToken tokens[], RtPointer params[])
		: TRiPatch(
			aLineNo, declmap, colorComps,
			TRi::RI_BICUBIC,
			n, tokens, params),
		  m_lastIndex(0)
	{
	}

	//! Tesselation of the patch.
	/*! \param tessU Number in tesselations in u directions.
	 *  \param tessV Number in tesselations in v directions.
	 *  \param ubasis Basis matrix for the paramteric u direction
	 *  \param vbasis Basis matrix for the paramteric v direction
	 */
	TBicubicMesh &tesselate(IndexType tessU, IndexType tessV, const RtBasis &ubasis, const RtBasis &vbasis);

	inline virtual void replay(TRi &ri, TRiCallbackInterface &ricb, TDrawPrim *dp) {
		if ( dp ) {
			dp->drawGeometricPrim(tesselate((IndexType)ricb.curUTesselation(), (IndexType)ricb.curVTesselation(), *ricb.curUBasis(), *ricb.curVBasis()));
		} else {
			TRiPatch::replay(ri);
		}
	}
}; // class TBicubicPatch


class TBilinearPatchMesh : public TRiPatchMesh {
	std::deque<TBilinearMesh> m_bilinearMeshes;     //!< Meshes for various tesselations and basis nu*nv
	IndexType m_lastIndex;
	IndexType m_upatches, m_vpatches;
public:
	inline TBilinearPatchMesh(
		long aLineNo, TDeclarationMap &declmap, RtInt colorComps,
		RtInt nu, RtToken uwrap, RtInt nv, RtToken vwrap,
		RtInt n, RtToken tokens[], RtPointer params[])
		: TRiPatchMesh(
			aLineNo, declmap, colorComps,
			1, 1,
			TRi::RI_BILINEAR,
			nu, uwrap, nv, vwrap,
			n, tokens, params),
		  m_lastIndex(0)
	{
		m_upatches = nu;
		if ( !strcmp(uwrap, TRi::RI_NONPERIODIC) )
			m_upatches--;
		m_vpatches = nv;
		if ( !strcmp(vwrap, TRi::RI_NONPERIODIC) )
			m_vpatches--;
	}

	//! Tesselation of the patch.
	/*! \param tessU Number in tesselations in u directions.
	 *  \param tessV Number in tesselations in v directions.
	 */
	IndexType tesselate(IndexType tessU, IndexType tessV);

	inline virtual void replay(TRi &ri, TRiCallbackInterface &ricb, TDrawPrim *dp) {
		if ( dp ) {
			IndexType tessU = (IndexType)ricb.curUTesselation();
			IndexType tessV = (IndexType)ricb.curVTesselation();
			IndexType meshIdx = tesselate(tessU, tessV);
			for ( IndexType v = 0; v < m_vpatches; ++v )
				for ( IndexType u = 0; u < m_upatches; ++u, ++meshIdx )
					dp->drawGeometricPrim(m_bilinearMeshes[meshIdx]);
		} else {
			TRiPatchMesh::replay(ri);
		}
	}
}; // class TBilinearPatchMesh


class TBicubicPatchMesh : public TRiPatchMesh {
	std::deque<TBicubicMesh> m_bicubicMeshes;     //!< Meshes for various tesselations and basis nu*nv
	IndexType m_lastIndex;
	IndexType m_nuBilin, m_nvBilin;
	IndexType m_upatches, m_vpatches;
	bool m_isuperiodic, m_isvperiodic;
public:
	inline TBicubicPatchMesh(
		long aLineNo, TDeclarationMap &declmap, RtInt colorComps,
		RtInt ustep, RtInt vstep, RtInt nu, RtToken uwrap, RtInt nv, RtToken vwrap,
		RtInt n, RtToken tokens[], RtPointer params[])
		: TRiPatchMesh(
			aLineNo, declmap, colorComps,
			ustep, vstep,
			TRi::RI_BICUBIC,
			nu, uwrap, nv, vwrap,
			n, tokens, params),
		  m_lastIndex(0)
	{
		m_isuperiodic = !strcmp(uwrap, TRi::RI_PERIODIC);
		m_isvperiodic = !strcmp(vwrap, TRi::RI_PERIODIC);

		m_upatches = m_isuperiodic ? nu/ustep : (nu-4)/ustep + 1;
		m_vpatches = m_isvperiodic ? nv/vstep : (nv-4)/vstep + 1;
		m_nuBilin = m_isuperiodic ? m_upatches : m_upatches+1;
		m_nvBilin = m_isvperiodic ? m_vpatches : m_vpatches+1;
	}

	//! Tesselation of the patch.
	/*! \param tessU Number in tesselations in u directions.
	 *  \param tessV Number in tesselations in v directions.
	 *  \param ubasis Basis matrix for the paramteric u direction
	 *  \param vbasis Basis matrix for the paramteric v direction
	 */
	IndexType tesselate(IndexType tessU, IndexType tessV, const RtBasis &uBasis, const RtBasis &vBasis);

	inline virtual void replay(TRi &ri, TRiCallbackInterface &ricb, TDrawPrim *dp) {
		if ( dp ) {
			IndexType tessU = (IndexType)ricb.curUTesselation();
			IndexType tessV = (IndexType)ricb.curVTesselation();
			const RtBasis *ubasis = ricb.curUBasis();
			const RtBasis *vbasis = ricb.curVBasis();
			if ( ubasis && vbasis ) {
				IndexType meshIdx = tesselate(tessU, tessV, *ubasis, *vbasis);
				for ( IndexType v = 0; v < m_vpatches; ++v )
					for ( IndexType u = 0; u < m_upatches; ++u, ++meshIdx )
						dp->drawGeometricPrim(m_bicubicMeshes[meshIdx]);
			}
		} else {
			TRiPatchMesh::replay(ri);
		}
	}
}; // class TBicubicPatchMesh



class TNuPatch : public TRiNuPatch {
	IndexType m_tessU, m_tessV;
	TNuBasis m_uBasis, m_vBasis;
	TNuBasis m_duBasis, m_dvBasis;
	TNuMesh m_mesh;
protected:
	bool calcBasis(IndexType tessU, IndexType tessV);
public:
	inline TNuPatch(
		long aLineNo, TDeclarationMap &declmap, RtInt colorComps,
		RtInt nu, RtInt uorder, RtFloat *uknot, RtFloat umin, RtFloat umax, RtInt nv, RtInt vorder, RtFloat *vknot, RtFloat vmin, RtFloat vmax,
		RtInt n, RtToken tokens[], RtPointer params[])
	: TRiNuPatch(aLineNo, declmap, colorComps,
	             nu, uorder, uknot, umin, umax,
				 nv, vorder, vknot, vmin, vmax,
				 n, tokens, params), 
	  m_tessU(0), m_tessV(0),
	  m_uBasis(nu, uorder), m_vBasis(nv, vorder),
	  m_duBasis(nu, uorder), m_dvBasis(nv, vorder)
	{
	}

	TNuMesh &tesselate(IndexType tessU, IndexType tessV);

	inline virtual void replay(TRi &ri, TRiCallbackInterface &ricb, TDrawPrim *dp) {
		if ( dp ) {
			IndexType tessU = (IndexType)ricb.curUTesselation();
			IndexType tessV = (IndexType)ricb.curVTesselation();
			TNuMesh &m = tesselate(tessU, tessV);
			dp->drawGeometricPrim(m);
			// TRiNuPatch::replay(ri);
		} else  {
			TRiNuPatch::replay(ri);
		}
	}
}; // class TNuPatch

#endif // _RICPP_RENDERCONTEXT_PARAMETRIC_H
