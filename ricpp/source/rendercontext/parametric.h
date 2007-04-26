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


////////////////////////////////////////////////////////////////////////////////
//! Basis values for bicubic spline patches of given parametric intervals
/*! The basis values are stored for caching in a TUVVectorCache instance. Since
 *  the coefficients won't change for different geometries. Multiplied with the
 *  geometry vectors, the bicubic patches result.
 */
class TUVVector {
protected:
	RtBasis m_uBasis;                 //!< Basis matrix used for the parametric direction u
	RtBasis m_vBasis;                 //!< Basis matrix used for the parametric direction v
	IndexType m_tessU;                //!< Number of segments for tesselation in parametric direction u
	IndexType m_tessV;                //!< Number of segments for tesselation in parametric direction v
public:
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
}; // class TUVVector


////////////////////////////////////////////////////////////////////////////////
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
	RtInt m_ncpts;                //!< Number of control points (control points are not stored in instances of this class)
	RtInt m_order;                //!< Order of the polynom (degree = k-1), 0 < k <= m_ncpts
	std::vector<RtFloat> m_knots; //!< Knot vector, size = m_ncpts+m_order, knots[i] <= knots[i+1]
	RtFloat m_tmin,               //!< Minimum value for parameter t, tmin >= x[order-1] && tmin < tmax
	        m_tmax;               //!< Maximum value for parameter t, tmax <= x[ncpts] && tmax > tmin
	RtInt m_tess;                 //!< Tesselation of the parameter, i.e. the number of intervals for the parameter range

	// Results
	RtInt m_segments;                  //!< Number of segments of the bspline: 1 + m_ncpts - m_order
	std::vector<RtInt> m_valOffs;      //!< Offset (in m_tVals) per segment, size of vector = m_segments
	std::vector<RtInt> m_valCnts;      //!< Number of parameters (in m_tVals) per segment, size of vector = m_segments, max(m_valCnts[i]) = m_tess+1
	std::vector<RtFloat> m_tVals;      //!< Parameters used to tesselate the spline, parameter range for a segment s is m_tVals[m_valOffs[s]] to m_tVals[m_valOffs[s]+m_valCnts[s]] (excluding), interval can be empty
	std::vector<RtFloat> m_basis;      //!< bspline basis for parameters m_tVals (number of values per parameter = order, only the influencing basis values are stored)
	std::vector<RtFloat> m_basisDeriv; //!< bspline basis first derivate for parameters m_tVals (number of values per parameter = order, only the influencing basis values are stored)

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

	//! Test if the member variables are valid for bspline-calculation
	bool unvalidParams();

	//! Calc all basis values, using Cox-de Boor recurrence function
	void calc();

public:
	//! Standard constructor, simpliy clears the members
	inline TBSplineBasis() {
		m_ncpts = 0;
		m_order = 0;
		m_tmin = (RtFloat)0;
		m_tmax = (RtFloat)0;
		m_tess = 0;
		m_segments = 0;
	}

	//! Constructor to initialize the members
	TBSplineBasis(RtInt ncpts, RtInt order, const std::vector<RtFloat> &knots,
		          RtFloat tmin, RtFloat tmax, RtInt tess);

	//! copy constructor
	inline TBSplineBasis(const TBSplineBasis &sp) {
		*this = sp;
	}

	//! Clone *this
	/*! \return A clone of *this 
	 */
	inline TBSplineBasis &duplicate() const {
		TBSplineBasis *cp = new TBSplineBasis(*this);
		if ( cp == NULL ) {
			throw TSystemException(__FILE__, __LINE__);
		}
		return *cp;
	}

	//! Assignment
	TBSplineBasis &operator=(const TBSplineBasis &sp);

	//! Recalculate the basis
	void reset(RtInt ncpts, RtInt order, const std::vector<RtFloat> &knots,
		       RtFloat tmin, RtFloat tmax, RtInt tess);
	//! Recalculate the basis
	void reset(RtInt ncpts, RtInt order, const std::vector<RtFloat> &knots, RtInt knotOffs,
		       RtFloat tmin, RtFloat tmax, RtInt tess);
}; // TBSplineBasis


class TUVPatch : public TPrimFace {
protected:
	IndexType m_tessU, //!< Tesselation in u direction, number of vertices in u direction is m_tessU+1
	          m_tessV; //!< Tesselation in v direction, number of vertices in v direction is m_tessV+1

	TNamedVector<RtFloat> *m_Pdu;       //!< Partial derivatives with respect to u for all vertices
	bool m_freePdu;                     //!< Free m_Pdu at destruction
	TNamedVector<RtFloat> *m_Pdv;       //!< Partial derivatives with respect to v for all vertices
	bool m_freePdv;                     //!< Free m_Pdv at destruction
	TNamedVector<RtFloat> *m_st;        //!< transformed st parameters (by m_stRect[8])
	bool m_freeST;                      //!< Free m_st at destruction
	bool m_transformS;                  //!< Allowed to transform S texture coordinates
	bool m_transformT;                  //!< Allowed to transform T texture coordinates
	enum ESTIdx { IDX_S1=0, IDX_T1, IDX_S2, IDX_T2, IDX_S3, IDX_T3, IDX_S4, IDX_T4 }; //!< Indices for m_stRect
	RtFloat m_stRect[8];                //!< Texture transform bei bilinear interpolation, corners: s1 t1 s2 t2 s3 t3 s4 t4

	inline virtual void fillCornerIdx(IndexType *idx, RtInt nu=2, RtInt nv=2, RtInt upatch=0, RtInt vpatch=0) const {
		idx[0] = (vpatch % nv)       * nu + (upatch % nu);
		idx[1] = (vpatch % nv)       * nu + ((upatch + 1) % nu);
		idx[2] = ((vpatch + 1) % nv) * nu + (upatch % nu);
		idx[3] = ((vpatch + 1) % nv) * nu + ((upatch + 1) % nu);
	}

	inline void freePd() {
		if ( m_freePdu && m_Pdu ) delete m_Pdu;
		m_Pdu = NULL;
		m_freePdu = false;
		if ( m_freePdv && m_Pdv ) delete m_Pdv;
		m_Pdv = NULL;
		m_freePdv = false;
		if ( m_freeST && m_st ) delete m_st;
		m_st = NULL;
		m_freeST = false;
	}

public:
	inline TUVPatch() : m_tessU(0), m_tessV(0) {
		m_Pdu = NULL;
		m_freePdu = false;
		m_Pdv = NULL;
		m_freePdv = false;
		m_st = NULL;
		m_freeST = false;
		m_transformS = false;
		m_transformT = false;
		m_stRect[IDX_S1] = 0;
		m_stRect[IDX_T1] = 0;
		m_stRect[IDX_S2] = 1;
		m_stRect[IDX_T2] = 0;
		m_stRect[IDX_S3] = 0;
		m_stRect[IDX_T3] = 1;
		m_stRect[IDX_S4] = 1;
		m_stRect[IDX_T4] = 1;
	}
	inline TUVPatch(const TUVPatch &uvPatch) { *this = uvPatch; }

	inline ~TUVPatch() {
		freePd();
	}

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

	virtual void transformTextureST();
	virtual const RtFloat *getTextureST(IndexType &size, RtFloat s1, RtFloat t1, RtFloat s2, RtFloat t2, RtFloat s3, RtFloat t3, RtFloat s4, RtFloat t4);
	void resizeVerticesBy(RtFloat r);

	TUVPatch &operator=(const TUVPatch &uvPatch);

	virtual void insertVertexData(const TParameterMap &pm, RtInt nu=2, RtInt nv=2, RtInt upatch=0, RtInt vpatch=0);
	virtual void insertVertexIndexData();

	inline virtual IndexType nu() const {return m_tessU+1;}
	inline virtual IndexType nv() const {return m_tessV+1;}

	inline IndexType tessU() const { return m_tessU; }
	inline IndexType tessV() const { return m_tessV; }
	inline IndexType tessU(IndexType u) { IndexType pu = m_tessU; m_tessU = u; return pu; }
	inline IndexType tessV(IndexType v) { IndexType pv = m_tessV; m_tessV = v; return pv; }
}; // class TUVPatch


template<class _T> inline _T &findCurve(std::deque<_T> &curveDeque, IndexType tess, IndexType &idx, bool &isNew, IndexType numCurves=1) {
	isNew = false;

	if ( idx < (IndexType)curveDeque.size() ) {
		_T &curveRef = curveDeque[idx];
		if ( curveRef.tess() == tess )
			return curveRef;
	}

	typename std::deque<_T>::const_iterator i;
	IndexType cnt = 0, j;
	for ( i = curveDeque.begin(); i != curveDeque.end(); cnt += numCurves ) {
		if ( (*i).tess() == tess ) {
			idx = cnt;
			return curveDeque[cnt];
		}
		for ( j = 0; j< numCurves; j++, i++ );
	}

	isNew = true;
	idx = curveDeque.size();
	IndexType savIdx=idx;
	curveDeque.resize(curveDeque.size()+numCurves);
	for ( ; savIdx < curveDeque.size(); ++savIdx ) {
		_T &mesh = curveDeque[savIdx];
		mesh.tess(tess);
	}

	_T &retVal = curveDeque[idx];
	return retVal;
}

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
		if ( &quadric == this )
			return *this;
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
		if ( &sphereMesh == this )
			return *this;
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
		if ( &cylinderMesh == this )
			return *this;
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
		if ( &paraboloidMesh == this )
			return *this;
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
		if ( &coneMesh == this )
			return *this;
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
		if ( &torusMesh == this )
			return *this;
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
		if ( &hyperboloidMesh == this )
			return *this;
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
		if ( &patchMesh == this )
			return *this;
		TUVPatch::operator=(patchMesh);
		return *this;
	}

	virtual void insertVertexData(const TParameterMap &pm, RtInt nu=2, RtInt nv=2, RtInt upatch=0, RtInt vpatch=0);
}; // class TBilinearMesh


class TBicubicMesh : public TUVPatch {
protected:
	RtBasis m_uBasis;
	RtBasis m_vBasis;

	//! Blends RenderMan parameters (e.g. RI_P) by the basis values
	void bicubicBlend(
		const IndexType *idx,
		const TParameter &source,
		TNamedVector<RtFloat> &dest,
		const TUVVector &uvvect,
		bool clearAndReserve);

	//! Blends RenderMan parameters (e.g. RI_P) by the basis values
	void bicubicBlend(
		const IndexType *idx,
		const char *name,
		IndexType components,
		IndexType cardinality,
		const RtFloat *source,
		TNamedVector<RtFloat> &dest,
		const TUVVector &uvvect,
		bool clearAndReserve);

	void TBicubicMesh::bicubicBlendNormals(
		const IndexType *idx,
		const TParameter &source,
		TNamedVector<RtFloat> &dest,
		const TUVVector &uvVect,
		bool clearAndReserve);

	void TBicubicMesh::bicubicBlendNormals(
		const IndexType *idx,
		const char *name,
		IndexType components,
		IndexType cardinality,
		const RtFloat *source,
		TNamedVector<RtFloat> &dest,
		const TUVVector &uvvect,
		bool clearAndReserve);
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
		if ( &patchMesh == this )
			return *this;
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


//! A simple container for concatenated trim segments that forms a loop
class TSimpleTrimLoop {
public:
	std::vector<RtFloat> m_points; //!< Container with the curve Points, explicitly closed m_points[0] == m_points[m_points.size()-1]
}; // TSimpleTrimLoop


/*! TCurveSegment is a container for one segment of a NURBS trim curve loop \see TTrimCurve
 */
class TNuCurveSegment {
public:
	RtInt m_tess;                  //!< Tesselation of the curve segment (number of line segments)
	std::vector<RtFloat> m_points; //!< The points of the tesselated curve segment, result of TNuCurveSegment::nuBlend()
public:
	//! Constructor, simply clears the tesselaton m_tess=0
	inline TNuCurveSegment() : m_tess(0) {};
	//! \return The number of linear segments (m_tess)
	inline RtInt tess() const { return m_tess; }
	//! Sets the new number of linear segments (m_tess)
	/*! \param t The new number of linear segments (m_tess)
	 *   \return The previous number of linear segments (m_tess)
	 */
	inline RtInt tess(RtInt t) { RtInt old = m_tess; m_tess = t; return old; }
	//! Non uniform blending
	/*! \param source The vector with the control points
	 *  \param offs The offset of the lopp to blend
	 *  \param seg to the curve segment to blend
	 *  \param basis The B-Spline (NURBS) basis of the curve
	 *  \param clearAndReserve true clear TNuCurveSegment::m_points before starting to blend, false To append values
	 *  \return The index of the first point of the curve in TNuCurveSegment:::m_points
	 */
	IndexType nuBlend(
		const std::vector<RtFloat> &source,
		RtInt offs,
		RtInt seg,
		const TBSplineBasis &basis,
		bool clearAndReserve);
}; // class TNuCurveSegment


/*! A vertex of the trim curve.
 *  It is a vertex obtained by tesselation of the curve and/or by intersection with a TParameterAxisCell
 */
enum ETrimVertexType { trimVertex=1, uaxisCoord=2, vaxisCoord=4};
class TTrimVertex {
public:
	IndexType
		m_ui,   //!< Index of the TParameterAxisCell 
		m_vi,   //!< Index of the TParameterAxisCell
		m_idx,  //!< Index of the TParameterCell (calculated using m_ui and m_vi)
		m_vidx, //!< Index of the vertex
		m_lidx; //!< Index of the loop
	RtFloat
		m_u,    //!<  u coordinate of the vertex
		m_v;    //!<  v coordinate of the vertex
	unsigned int m_vtype; //!< Vertex type, bitfield of ETrimVertexType, 0 for uninitialized

	inline TTrimVertex() : m_vidx(0), m_lidx(0), m_vtype(0) {}
	inline TTrimVertex(IndexType ui, IndexType vi, IndexType idx, RtFloat u, RtFloat v, unsigned int vtype=trimVertex)
		: m_ui(ui), m_vi(vi), m_idx(idx), m_u(u), m_v(v), m_vtype(vtype), m_vidx(0), m_lidx(0) {
	}

	inline TTrimVertex(const TTrimVertex &vert) {
		*this = vert;
	}
	inline TTrimVertex &operator=(const TTrimVertex &vert) {
		if ( &vert == this )
			return *this;
		m_ui = vert.m_ui;
		m_vi = vert.m_vi;
		m_idx = vert.m_idx;
		m_vidx = vert.m_vidx;
		m_lidx = vert.m_lidx;
		m_u = vert.m_u;
		m_v = vert.m_v;
		m_vtype = vert.m_vtype;
		return *this;
	}
	inline bool onAxis() {
		return (m_vtype & (uaxisCoord | vaxisCoord)) != 0;
	}
}; // TTrimVertex


/*! A trim loop as a list of TTrimVertex, formed by some trim curve segments, explicitly closed
 */
class TTrimLoop {
public:
	std::list<TTrimVertex> m_trimVertices;
	inline void buildIndices(IndexType loopIdx) {
		IndexType vidx = 0;
		std::list<TTrimVertex>::iterator i = m_trimVertices.begin();
		for ( ; i != m_trimVertices.end(); i++, ++vidx ) {
			(*i).m_vidx = vidx;
			(*i).m_lidx = loopIdx;
		}
	}
}; // TTrimLoop


/*! A value of one grid coordiante axis (TParameterGrid)
 *  The coordinates are either segment borders or borders
 *  of the facets of the tesselatioon of the patch
 */
class TParameterAxisCell {
public:
	RtFloat m_coord; //!< Parametric coordinate of an axis in TParameterGrid, may be riInfinity or -riInfinity
	bool m_isBorder; //!< true if the coordinate is a segment (face) border
	inline TParameterAxisCell() : m_coord(0.0), m_isBorder(false) {}
	inline TParameterAxisCell(const TParameterAxisCell &pa) { *this = pa; }
	inline TParameterAxisCell &operator=(const TParameterAxisCell &ac) {
		if ( &ac == this )
			return *this;
		m_coord = ac.m_coord;
		m_isBorder = ac.m_isBorder;
		return *this;
	}
}; // TParameterAxisCell


/*! Cell for a TParameterGrid, facet size: left <= content < right, top <= content < bottom
 */
enum ETrimEdgeType { noEdge = 0, inCell=1, onUAxis=2, onVAxis=3, onCorner=3};
class TParameterCell {
protected:
	class TTrimThread {
	public:
		bool m_isClosed;
		enum ETrimEdgeType m_edgeType;
		std::list<const TTrimVertex *> m_thread;
		inline TTrimThread() : m_isClosed(false), m_edgeType(noEdge) {}
		inline TTrimThread(const TTrimThread &sc) { *this = sc; }
		inline TTrimThread &operator=(const TTrimThread &tt) {
			if ( &tt == this )
				return *this;
			m_isClosed = tt.m_isClosed;
			m_edgeType = tt.m_edgeType;
			m_thread = tt.m_thread;
			return *this;
		}
	};

	class TTrimmedCell {
	public:
		std::list<TTrimThread> m_threads;
		inline TTrimmedCell() {}
		inline TTrimmedCell(const TTrimmedCell &tc) { *this = tc; }
		inline TTrimmedCell &operator=(const TTrimmedCell &tc) {
			if ( &tc == this )
				return *this;
			m_threads = tc.m_threads;
			return *this;
		}
	};

	TTrimmedCell *m_trim;

public:
	TParameterAxisCell *m_left, *m_right, *m_top, *m_bottom;
	bool m_inner;
	inline TParameterCell() : m_left(NULL), m_right(NULL), m_top(NULL), m_bottom(NULL), m_trim(NULL), m_inner(true) {}
	inline TParameterCell(const TParameterCell &pc) : m_left(NULL), m_right(NULL), m_top(NULL), m_bottom(NULL), m_trim(NULL), m_inner(true) { *this = pc; }
	inline TParameterCell &operator=(const TParameterCell &pc) {
		if ( &pc== this )
			return *this;
		m_left = pc.m_left;
		m_right = pc.m_right;
		m_top = pc.m_top;
		m_bottom = pc.m_bottom;
		m_inner = pc.m_inner;
		if ( m_trim ) {
			delete m_trim;
		}
		if ( pc.m_trim ) {
			m_trim = new TTrimmedCell(*pc.m_trim);
		}
		return *this;
	}
	bool insertTrim(std::list<TTrimVertex>::const_iterator &vertexIter, const std::list<TTrimVertex> &vertices, IndexType startVertexIdx);
	void cut();
	void triangulate();
	inline bool isFragmented() { return m_trim != NULL; }
	inline bool isValid() { return m_left && m_right && m_top && m_bottom; }
}; // TParameterCell


/*! A quadrilateral NURBS patch with trim curves 
 */
class TParameterGrid {
public:
	std::vector<TParameterAxisCell> m_uaxis, m_vaxis;
	IndexType m_width, m_height;
	std::vector<TParameterCell> m_cells;
	std::vector<TTrimLoop> m_trimLoops;
private:
	void fillaxis(std::vector<TParameterAxisCell> &axis,
		const std::vector<RtFloat> &knots, RtFloat mini, RtFloat maxi, RtInt ncpts, RtInt order, IndexType tess);
	void addIntersections(const TTrimVertex &from, const TTrimVertex &to);
public:
	inline TParameterGrid() {m_width = m_height = 0;}
	TParameterGrid(
		const std::vector<RtFloat> &uknots, RtFloat umin, RtFloat umax, RtInt uncpts, RtInt uorder, IndexType utess,
		const std::vector<RtFloat> &vknots, RtFloat vmin, RtFloat vmax, RtInt vncpts, RtInt vorder, IndexType vtess);
	void reset(
		const std::vector<RtFloat> &uknots, RtFloat umin, RtFloat umax, RtInt uncpts, RtInt uorder, IndexType utess,
		const std::vector<RtFloat> &vknots, RtFloat vmin, RtFloat vmax, RtInt vncpts, RtInt vorder, IndexType vtess);
	inline IndexType findIdx(RtFloat param, IndexType lidx, IndexType ridx, const std::vector<TParameterAxisCell> &axis) {
		if ( lidx>=ridx )
			return ridx;
		IndexType idx = (lidx+ridx)/2;
		if ( param < axis[idx].m_coord )
			return findIdx(param, lidx, idx-1, axis);
		if ( param >= axis[idx+1].m_coord )
			return findIdx(param, idx+1, ridx, axis);
		return idx;
	}
	inline IndexType findIdx(RtFloat param, const std::vector<TParameterAxisCell> &axis) {
		assert(axis.size() > 0);
		if ( axis.size() <= 0 )
			return 0;

		return findIdx(param, 0, (IndexType)(axis.size()-1), axis);
	}
	inline void initpath(RtInt nLoops) { m_trimLoops.clear(); m_trimLoops.reserve(nLoops); }
	inline void newpath() { m_trimLoops.resize(m_trimLoops.size()+1); }
	void addCurveSegment(const TNuCurveSegment &cs);
	void closepath();
	void split();
	void clear();
	inline bool hasTrimLoops() { return !m_trimLoops.empty(); }
}; // TParameterGrid


//! A NURBS trim curve, can contain of several loops (closed curves), one loop can be formed of many segments
class TTrimCurve : public TRiTrimCurve {
public:
	IndexType m_lastIndex;                    //!< Index of the current start in m_curveSegs 

	IndexType m_segments;                     //!< Number of segments, a segment can be empty
	IndexType m_tess;                         //!< Tesselation of the trim curve
	std::vector<TBSplineBasis> m_basis;       //!< Basis for each curve
	std::deque<TNuCurveSegment> m_curveSegs;  //!< All curve segments of all loops

	std::vector<IndexType> m_curveSegOffs;    //!< Offset to m_curveSegs for each curve
	std::vector<IndexType> m_curveSegSize;    //!< Number of segments for curve
	std::vector<IndexType> m_curveSegPoints;  //!< Sum of the tesselated points for each curve

	std::vector<TSimpleTrimLoop> m_trimLoops; //!< Concatenated TrimLoops

protected:
	//! Cacluates the B-spline basis of the curves
	bool calcBasis(IndexType tess);

public:
	inline static const char *myClassName(void) { return "TTrimCurve"; }
	inline virtual const char *className() const { return TTrimCurve::myClassName(); }

	inline TTrimCurve(
		long aLineNo,
		RtInt nloops, RtInt *ncurves, RtInt *order, RtFloat *knot, RtFloat *amin, RtFloat *amax,
		RtInt *n, RtFloat *u, RtFloat *v, RtFloat *w)
	: TRiTrimCurve(aLineNo, nloops, ncurves, order, knot, amin, amax, n, u, v, w),
	  m_lastIndex(0),
	  m_segments(0),
	  m_tess(0)
	{
	}

	inline TTrimCurve(
		long aLineNo, const TTrimCurveData &trimCurve
	)
	: TRiTrimCurve(aLineNo, trimCurve),
	  m_lastIndex(0),
	  m_segments(0),
	  m_tess(0)
	{
	}

	IndexType tesselate(IndexType tess);
	inline virtual void replay(TRi &ri, TRiCallbackInterface &ricb, TDrawPrim *dp) {
		TRiTrimCurve::replay(ri);
	}

	bool isTrim(const TTrimCurveData &trimCurve, IndexType tess);
	void insertIntoGrid(TParameterGrid &grid) const;
}; // class TTrimCurve


//! One segment of a NUPatch 
class TNuMesh : public TUVPatch {
	TNamedVector<RtFloat> m_u;         //!< u parameters (default texture coordinates s)
	TNamedVector<RtFloat> m_v;         //!< v parameters (default texture coordinates t)
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
		if ( &patchMesh == this )
			return *this;
		TUVPatch::operator=(patchMesh);
		m_u = patchMesh.m_u;
		m_v = patchMesh.m_v;
		return *this;
	}

	void nuBlend(
		const TParameter &source,
		TNamedVector<RtFloat> &dest,
		RtInt useg,
		RtInt vseg,
		const TBSplineBasis &uBasis,
		const TBSplineBasis &vBasis,
		const std::vector<IndexType> &idx
		) const;

	void nuBlend(
		const char *name,
		IndexType components,
		IndexType cardinality,
		const RtFloat *source,
		TNamedVector<RtFloat> &dest,
		RtInt useg,
		RtInt vseg,
		const TBSplineBasis &uBasis,
		const TBSplineBasis &vBasis,
		const std::vector<IndexType> &idx,
		bool clearAndReserve) const;

	void nuBlendNormals(
		const TParameter &source,
		TNamedVector<RtFloat> &dest,
		RtInt useg,
		RtInt vseg,
		const TBSplineBasis &uBasis,
		const TBSplineBasis &vBasis,
		const std::vector<IndexType> &idx
		);

	void nuBlendNormals(
		const char *name,
		IndexType components,
		IndexType cardinality,
		const RtFloat *source,
		TNamedVector<RtFloat> &dest,
		RtInt useg,
		RtInt vseg,
		const TBSplineBasis &uBasis,
		const TBSplineBasis &vBasis,
		const std::vector<IndexType> &idx,
		bool clearAndReserve);

	virtual void insertVertexDataNU(
		const TParameterMap &pm,
		RtInt useg, RtInt vseg,
		const TBSplineBasis &uBasis, const TBSplineBasis &vBasis,
		TParameterGrid &paramGrid
		);

	virtual void transformTextureST();
}; // class TNuMesh


//! Cache of tesselated unit circles (is sized to equiparam curves (either u or v) of TUnitSphere)
class TUnitCircleCache {
public:
	void getUnitCircle(std::vector<RtFloat> &circledata, IndexType tess, RtFloat thetamin, RtFloat thetamax);
	void getUnitCircle(std::vector<RtFloat> &circledata, IndexType tess, RtFloat thetamax);
}; // class TUnitCircle


//! Cache of tesselated unit spheres (is sized to a radius of a TSphere for faster calculation)
class TUnitSphereCache  {
	IndexType m_lastIndex;
	std::deque<TSphereMesh> m_meshData;     //!< Meshes for various tesselations.
public:
	inline TUnitSphereCache() : m_lastIndex(0) {}
	const TSphereMesh &getUnitSphereRef(IndexType tessU, IndexType tessV);
	void getUnitSphere(TSphereMesh &sphere, IndexType tessU, IndexType tessV);
}; // class TUnitSphere


//! Sphere primary for tesselation
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


//! Cylinder primary for tesselation
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


//! Paraboloid primary for tesselation
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


//! Cone primary for tesselation
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


//! Disk primary for tesselation
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


//! Torus primary for tesselation
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


//! Hyperbolid primary for tesselation
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


//! Single bilinear patch primary for tesselation
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


//! Single Bicubic patch primary for tesselation
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


//! Bilinear patch mesh primary for tesselation
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


//! Bicubic patch mesh primary for tesselation
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



////////////////////////////////////////////////////////////////////////////////
//! A NURBS patch
class TNuPatch : public TRiNuPatch {
	IndexType m_lastIndex;		//!< Start of the meshes in m_nuMeshes
	TTrimCurve *m_tessTrim;		//!< Currently used trim curve of the patch (copy)
	TParameterGrid m_paramGrid;	//!< Parameter grid to trim the patch

	TBSplineBasis m_uBasis, //!< B-Spline basis for direction u
	              m_vBasis; //!< B-Spline basis for direction v

	//! Meshes, there is one mesh per segment in u and v direction (u first).
	std::deque<TNuMesh> m_nuMeshes;

protected:
	//! Calculates the B-spline basis in u and v
	bool calcBasis(IndexType tessU, IndexType tessV);

public:
	//! Constructor inserts the values of a RiNuPatch call
	inline TNuPatch(
		long aLineNo, TDeclarationMap &declmap, RtInt colorComps,
		RtInt nu, RtInt uorder, RtFloat *uknot, RtFloat umin, RtFloat umax,
		RtInt nv, RtInt vorder, RtFloat *vknot, RtFloat vmin, RtFloat vmax,
		RtInt n, RtToken tokens[], RtPointer params[])
	: TRiNuPatch(aLineNo, declmap, colorComps,
		nu, uorder, uknot, umin, umax,
		nv, vorder, vknot, vmin, vmax,
		n, tokens, params), 
		m_lastIndex(0)
	{
		m_tessTrim = NULL;
	}

	//! Destructor clears the trim curve
	inline virtual ~TNuPatch() {
		if ( m_tessTrim ) delete m_tessTrim;
	}

	//! Tesselates th NURBS patch
	/*! \param tessU Number in tesselations in u directions.
	 *  \param tessV Number in tesselations in v directions.
	 *  \param newTrim to create a new trim curve
	 */
	IndexType tesselate(IndexType tessU, IndexType tessV, bool newTrim);

	//! Replays the content of the object in a render context
	/*! \param ri The render context
	 *  \param ricb Callback to query the state of the renderer
	 *  \param dp Optional drawing context for the meshes of the primary
	 */
	virtual void replay(TRi &ri, TRiCallbackInterface &ricb, TDrawPrim *dp);

}; // class TNuPatch

#endif // _RICPP_RENDERCONTEXT_PARAMETRIC_H
