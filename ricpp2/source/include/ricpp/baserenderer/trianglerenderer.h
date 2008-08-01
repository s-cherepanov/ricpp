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

namespace RiCPP {
	
	/** @brief Base class to triangulate primitives.
	 */
	class CTriangleRenderer : public CBaseRenderer {
	protected:
		typedef CBaseRenderer TypeParent;

	private:
		CEarClipper m_earClipper; ///< Triangulation strategy
		RtInt m_tessX;
		RtInt m_tessY;
		
	protected:
		inline virtual const IPolygonTriangulationStrategy &polygonTriangulationStrategy() const { return m_earClipper; }
		
	public:
		CTriangleRenderer();

		virtual RtVoid triangulate(CRiPolygon &obj);
		virtual RtVoid triangulate(CRiPointsPolygons &obj);
		virtual RtVoid triangulate(CRiGeneralPolygon &obj);
		virtual RtVoid triangulate(CRiPointsGeneralPolygons &obj);
		virtual RtVoid triangulate(CRiParaboloid &obj);
	}; // CTriangleRenderer

}

#endif // _RICPP_BASERENDERER_TRIANGLERENDERER_H
