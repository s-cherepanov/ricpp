#ifndef _RICPP_RICONTEXT_BLEND_H
#define _RICPP_RICONTEXT_BLEND_H

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

/** @file blend.h
 *  @author Andreas Pidde (andreas@pidde.de)
 *  @brief Bilinear and bicubic blending (later via basis vectors for bicubic spline patches of given parametric intervals).
 */

#ifndef _RICPP_RICPP_TYPES_H
#include "ricpp/ricpp/types.h"
#endif // _RICPP_RICPP_TYPES_H

namespace RiCPP {

/** @brief Bilinear blending
 */
class CBilinearBlend {
private:
	IndexType m_tessU;                //!< Number of segments for tesselation in parametric direction u
	IndexType m_tessV;                //!< Number of segments for tesselation in parametric direction v
	
public:
	//! Standard constructor, just clears the members
	CBilinearBlend();
	//! Standard constructor, assigns tesselation
	CBilinearBlend(IndexType aTessU, IndexType aTessV);
	//! do the blending
	void bilinearBlend(IndexType elemSize, const IndexType (&cornerIdx)[4], const std::vector<RtFloat> &vals, std::vector<RtFloat> &retvals);
	
	//! Tesselation of the patch in parametric direction u
	inline IndexType tessU() const { return m_tessU; }
	inline void tessU(IndexType aTessU) { m_tessU = aTessU; }
	
	//! Tesselation of the patch in parametric direction v
	inline IndexType tessV() const { return m_tessV; }
	inline void tessV(IndexType aTessV) { m_tessV = aTessV; }
}; // CBicubicBlend
		
////////////////////////////////////////////////////////////////////////////////
//! Basis vectors for bicubic spline patches of given parametric intervals
class CBicubicVectors {
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
	CBicubicVectors();
	
	//! Constructor, assings the parameters to the members 
	inline CBicubicVectors(IndexType aTessU, IndexType aTessV, const RtBasis aUBasis, const RtBasis aVBasis)
	{
		reset(aTessU, aTessV, aUBasis, aVBasis);
	}
	
	//! Copy constructor, assings the members of another instance to the members of *this
	inline CBicubicVectors(const CBicubicVectors &v) { *this = v; }
	
	//! Duplication
	/*! @return A clone of *this
	 */
	inline CBicubicVectors *duplicate() const
	{
		return new CBicubicVectors(*this);
	}
	
	//! Assigns the members of an instance uvVector to the members of *this
	inline CBicubicVectors &operator=(const CBicubicVectors &uvVector)
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
					  std::vector<RtFloat> &retvals) const;
	
	void bicubicBlendWithNormals(IndexType elemSize,
								 const IndexType (&controlIdx)[16],
								 const std::vector<RtFloat> &vals,
								 bool flipNormal,
								 std::vector<RtFloat> &retvals,
								 std::vector<RtFloat> &normals) const;
}; // class CBicubicVectors

}

#endif // _RICPP_RICONTEXT_BLEND_H
