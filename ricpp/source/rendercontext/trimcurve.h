#ifndef _RICPP_RENDERCONTEXT_TRIMCURVE_H
#define _RICPP_RENDERCONTEXT_TRIMCURVE_H

// RICPP - RenderMan Interface CPP Language Binding
//         RenderMan is (R) by Pixar
//         Windows is (R) by Microsoft
//
// Copyright © 2001 - 2002, Andreas Pidde
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

/*! \file trimcurve.h
 *  \brief Declaration of the container for a trim curve for NURBS, part of the attribute stack
 *  \author Andreas Pidde (andreas@pidde.de)
 */

#include "rendererloader/ricpp.h"
#include "rendererloader/sysexcept.h"

#include <vector>

//! TrimCurve part of the attribute stack, contains one trim curve
class TTrimCurveData {
public:
	RtInt m_nLoops; //!< Number of closed loops

	int m_total;	//!< Total number of curves
	int m_npoints;	//!< Total number of control points
	int m_nknots;	//!< Total number of knots

	
	std::vector<RtInt>   m_nCurves; //!< Number if loops (curves with holes)
	std::vector<RtInt>   m_order;   //!< Order of the curves
	std::vector<RtFloat> m_knots;   //!< Knot vectors of the curves
	std::vector<RtFloat> m_min;     //!< Minimum parametric values of the curves
	std::vector<RtFloat> m_max;     //!< Maximum parametric values of the curves
	std::vector<RtInt>   m_n;       //!< Number of coordinates
	std::vector<RtFloat> m_u;       //!< u coordinates of the curves
	std::vector<RtFloat> m_v;       //!< v coordinates of the curves
	std::vector<RtFloat> m_w;       //!< w coordinates of the curves
	std::vector<RtFloat> m_points;  //!< Points filled with (u[0], v[0], w[0], ... )

	/////////////////////////////////////////////////////////////////////////////////////////
	// Member functions

	//! Standard constructor, initializes with 0 loops
	inline TTrimCurveData() {
		m_nLoops = 0;
	}

	//! Constructor, fills instances with the values of a trim curve interface call
	/*! \param nloops   Number if loops (curves with holes)
	 *  \param ncurves  Curves of the loops
	 *  \param order    Order of the curves
	 *  \param knot     Knot vectors of the curves
	 *  \param amin     Minimum parametric values of the curves
	 *  \param amax     Maximum parametric values of the curves
	 *  \param n        Number of the homogene u,v,w coordinates
	 *  \param u        u coordinates of the curves
	 *  \param v        v coordinates of the curves
	 *  \param w        w coordinates of the curves
	 */
	inline TTrimCurveData(
		RtInt nloops, const RtInt *ncurves,
		const RtInt *order, const RtFloat *knot,
		const RtFloat *amin, const RtFloat *amax,
		const RtInt *n, const RtFloat *u, const RtFloat *v, const RtFloat *w)
	{
		m_nLoops = 0;
		trimCurve(nloops, ncurves, order, knot, amin, amax, n, u, v, w);
	}

	//! Copy constructor
	/*! \param curve The curve to assign from.
	 */
	TTrimCurveData(const TTrimCurveData &curve) {
		*this = curve;
	}

	//! Destructor
	~TTrimCurveData() {
		releaseAll();
	}

	//! Duplicates *this
	/*! \return A new TTrimCurveData instance with the values of this
	 *  \throws TSystemException If there is not enough memory for a new instance
	 */
	inline TTrimCurveData &duplicate() const {
		TTrimCurveData *curve = new TTrimCurveData(*this);
		if ( !curve )
			throw TSystemException(__FILE__, __LINE__);
		return *curve;
	}

	//! Assignment of another curve
	/*! \param curve The curve to assign
	 *  \return this
	 */
	inline TTrimCurveData &operator=(const TTrimCurveData &curve) {
		if ( this == &curve )
			return *this;
		if ( curve.m_nLoops > 0 )
			trimCurve(
				curve.m_nLoops,
				curve.m_nLoops == 0 || curve.m_nCurves.empty() ? NULL : &curve.m_nCurves[0],
				curve.m_nLoops == 0 || curve.m_order.empty() ? NULL : &curve.m_order[0],
				curve.m_nLoops == 0 || curve.m_knots.empty() ? NULL : &curve.m_knots[0],
				curve.m_nLoops == 0 || curve.m_min.empty() ? NULL : &curve.m_min[0],
				curve.m_nLoops == 0 || curve.m_max.empty() ? NULL : &curve.m_max[0],
				curve.m_nLoops == 0 || curve.m_n.empty() ? NULL : &curve.m_n[0],
				curve.m_nLoops == 0 || curve.m_u.empty() ? NULL : &curve.m_u[0],
				curve.m_nLoops == 0 || curve.m_v.empty() ? NULL : &curve.m_v[0],
				curve.m_nLoops == 0 || curve.m_w.empty() ? NULL : &curve.m_w[0]
			);
		else {
			releaseAll();
		}
		return *this;
	}

	inline bool compareWith(const TTrimCurveData &curve) {
		if ( m_nLoops != curve.m_nLoops ) return false;
		if ( m_nCurves != curve.m_nCurves ) return false;
		if ( m_order != curve.m_order )  return false;
		if ( m_knots != curve.m_knots )  return false;
		if ( m_min != curve.m_min )  return false;
		if ( m_max != curve.m_max )  return false;
		if ( m_n != curve.m_n )  return false;
		if ( m_u != curve.m_u )  return false;
		if ( m_v != curve.m_v )  return false;
		if ( m_w != curve.m_w )  return false;
		if ( m_points != curve.m_points )  return false;
		return true;
	}

	//! Empties instance
	inline void releaseAll() {
		m_nLoops = 0;
	}

	//! Fills instances with the values of a trim curve interface call.
	/*! \param nloops   Number if loops (curves with holes)
	 *  \param ncurves  Curves of the loops
	 *  \param order    Order of the curves
	 *  \param knot     Knot vectors of the curves
	 *  \param amin     Minimum parameter of a curve
	 *  \param amax     Maximum parameter of a curve
	 *  \param n        Number of the homogene u,v,w coordinates
	 *  \param u        u coordinates of the curves
	 *  \param v        v coordinates of the curves
	 *  \param w        w coordinates of the curves
	 */
	void trimCurve(RtInt nloops, const RtInt *ncurves, const RtInt *order, const RtFloat *knot, const RtFloat *amin, const RtFloat *amax,
				   const RtInt *n, const RtFloat *u, const RtFloat *v, const RtFloat *w);
};

#endif // _RICPP_RENDERCONTEXT_TRIMCURVE_H
