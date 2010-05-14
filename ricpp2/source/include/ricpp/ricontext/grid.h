#ifndef _RICPP_RICONTEXT_GRID_H
#define _RICPP_RICONTEXT_GRID_H

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

/** @file grid.h
 *  @author Andreas Pidde (andreas@pidde.de)
 *  @brief Grid of parametric data for NURBS trimming.
 */

#ifndef _RICPP_RICPP_TYPES_H
#include "ricpp/ricpp/types.h"
#endif /* _RICPP_RICPP_TYPES_H */

namespace RiCPP {
	
	class CGridCoordinate {
		RtFloat m_coord;
	public:
		inline CGridCoordinate() { m_coord = (RtFloat)0; }
		inline void set(RtFloat val) { m_coord = val; }
		inline RtFloat get() const { return m_coord; }
	}; // CGridCoordinate
	
	class CParametricAxis {
		std::vector<CGridCoordinate> m_coordinates;
	public:
		inline CParametricAxis() {}
		void init(RtInt n, RtFloat knot[], RtFloat minKnot, RtFloat maxKnot, IndexType tess);
	}; // CParametricAxis
	
	class CParametricGrid {
		enum EnumAxisIndex {
			uAxisIdx = 0,
			vAxisIdx
		};
		CParametricAxis m_axises[2]; //!< u, v axis
	public:
		inline CParametricGrid() {}
		CParametricGrid(RtInt nu, RtFloat uknot[], RtFloat umin, RtFloat umax, IndexType utess, RtInt nv, RtFloat vknot[], RtFloat vmin, RtFloat vmax, IndexType vtess);
		inline const CParametricAxis &uAxis() const { return m_axises[uAxisIdx]; }
		inline const CParametricAxis &vAxis() const { return m_axises[vAxisIdx]; }
	}; // CParametricGrid
	
	class CTrimLoop {
	public:
	}; // CTrimLoop
	
	class CTrimFragment {
	public:
	}; // CTrimFragment
	
} // namespace RiCPP

#endif // _RICPP_RICONTEXT_GRID_H
