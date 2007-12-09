#ifndef _RICPP_RIBPARSER_RIBPRIMS_H
#define _RICPP_RIBPARSER_RIBPRIMS_H

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

/** @file riboptions.h
 *  @author Andreas Pidde (andreas@pidde.de)
 *  @brief Handles RIB requests for prims (Sphere, PatchMesh, Polygon, ...).
 *
 *  The objects are used only by the implementation of the rib parser.
 */

#ifndef _RICPP_RIBPARSER_RIBPARSER_H
#include "ricpp/ribparser/ribparser.h"
#endif // _RICPP_RIBPARSER_RIBPARSER_H

namespace RiCPP {

	/** @brief Handles RIB request Polygon ...
	 */
	class CPolygonRibRequest : public CRibRequest {
	public:
		virtual void operator()(IRibParserState &parser, CRibRequestData &request) const;
		inline virtual EnumRequests interfaceIdx() const { return REQ_POLYGON; }
	}; // CPolygonRibRequest

	/** @brief Handles RIB request GeneralPolygon [ nvertices ] <paramlist>
	 */
	class CGeneralPolygonRibRequest : public CRibRequest {
	public:
		virtual void operator()(IRibParserState &parser, CRibRequestData &request) const;
		inline virtual EnumRequests interfaceIdx() const { return REQ_GENERAL_POLYGON; }
	}; // CGeneralPolygonRibRequest

	/** @brief Handles RIB request PointsPolygons [ nvertices ] [ vertices ] ...
	 */
	class CPointsPolygonsRibRequest : public CRibRequest {
	public:
		virtual void operator()(IRibParserState &parser, CRibRequestData &request) const;
		inline virtual EnumRequests interfaceIdx() const { return REQ_POINTS_POLYGONS; }
	}; // CPolygonRibRequest

	/** @brief Handles RIB request PointsGeneralPolygons [ nloops ] [ nvertices ] [ vertices ] <paramlist>
	 */
	class CPointsGeneralPolygonsRibRequest : public CRibRequest {
	public:
		virtual void operator()(IRibParserState &parser, CRibRequestData &request) const;
		inline virtual EnumRequests interfaceIdx() const { return REQ_POINTS_GENERAL_POLYGONS; }
	}; // CGeneralPolygonRibRequest

	/** @brief Handles RIB request Patch type ...
	 */
	class CPatchRibRequest : public CRibRequest {
	public:
		virtual void operator()(IRibParserState &parser, CRibRequestData &request) const;
		inline virtual EnumRequests interfaceIdx() const { return REQ_PATCH; }
	}; // CPatchRibRequest

	/** @brief Handles RIB request PatchMesh type nu uwrap nv vwrap ...
	 */
	class CPatchMeshRibRequest : public CRibRequest {
	public:
		virtual void operator()(IRibParserState &parser, CRibRequestData &request) const;
		inline virtual EnumRequests interfaceIdx() const { return REQ_PATCH_MESH; }
	}; // CPatchMeshRibRequest

	/** @brief Handles RIB request NuPatch nu uorder [ uknot ] umin umax nv vorder [ vknot ] vmin vmax ...
	 */
	class CNuPatchRibRequest : public CRibRequest {
	public:
		virtual void operator()(IRibParserState &parser, CRibRequestData &request) const;
		inline virtual EnumRequests interfaceIdx() const { return REQ_NU_PATCH; }
	}; // CNuPatchRibRequest

	/** @brief Handles RIB request SubdivisionMesh scheme [ nvertices ] [ vertices ] [ tags ] [ nargs ] [ intargs ] [ floatargs ] ...
	 */
	class CSubdivisionMeshRibRequest : public CRibRequest {
	public:
		virtual void operator()(IRibParserState &parser, CRibRequestData &request) const;
		inline virtual EnumRequests interfaceIdx() const { return REQ_SUBDIVISION_MESH; }
	}; // CSubdivisionMeshRibRequest

	/** @brief Handles RIB request HierarchicalSubdivisionMesh scheme [ nvertices ] [ vertices ] [ tags ] [ nargs ] [ intargs ] [ floatargs ] [ strargs ] ...
	 */
	class CHierarchicalSubdivisionMeshRibRequest : public CRibRequest {
	public:
		virtual void operator()(IRibParserState &parser, CRibRequestData &request) const;
		inline virtual EnumRequests interfaceIdx() const { return REQ_HIERARCHICAL_SUBDIVISION_MESH; }
	}; // CSHierarchicalubdivisionMeshRibRequest

	/** @brief Handles RIB request Sphere radius zmin zmax thetamax ...
	 */
	class CSphereRibRequest : public CRibRequest {
	public:
		virtual void operator()(IRibParserState &parser, CRibRequestData &request) const;
		inline virtual EnumRequests interfaceIdx() const { return REQ_SPHERE; }
	}; // CSphereRibRequest

	/** @brief Handles RIB request Cone height radius thetamax ...
	 */
	class CConeRibRequest : public CRibRequest {
	public:
		virtual void operator()(IRibParserState &parser, CRibRequestData &request) const;
		inline virtual EnumRequests interfaceIdx() const { return REQ_CONE; }
	}; // CConeRibRequest

	/** @brief Handles RIB request Cylinder radius zmin zmax thetamax ...
	 */
	class CCylinderRibRequest : public CRibRequest {
	public:
		virtual void operator()(IRibParserState &parser, CRibRequestData &request) const;
		inline virtual EnumRequests interfaceIdx() const { return REQ_CYLINDER; }
	}; // CCylinderRibRequest

	/** @brief Handles RIB request Hyperboloid point1 point2 thetamax ...
	 */
	class CHyperboloidRibRequest : public CRibRequest {
	public:
		virtual void operator()(IRibParserState &parser, CRibRequestData &request) const;
		inline virtual EnumRequests interfaceIdx() const { return REQ_HYPERBOLOID; }
	}; // CHyperboloidRibRequest

	/** @brief Handles RIB request Paraboloid rmax zmin zmax thetamax ...
	 */
	class CParaboloidRibRequest : public CRibRequest {
	public:
		virtual void operator()(IRibParserState &parser, CRibRequestData &request) const;
		inline virtual EnumRequests interfaceIdx() const { return REQ_PARABOLOID; }
	}; // CParaboloidRibRequest

	/** @brief Handles RIB request Disk height radius thetamax ...
	 */
	class CDiskRibRequest : public CRibRequest {
	public:
		virtual void operator()(IRibParserState &parser, CRibRequestData &request) const;
		inline virtual EnumRequests interfaceIdx() const { return REQ_DISK; }
	}; // CDiskRibRequest

	/** @brief Handles RIB request Torus majorrad minorrad phimin phimax thetamax ...
	 */
	class CTorusRibRequest : public CRibRequest {
	public:
		virtual void operator()(IRibParserState &parser, CRibRequestData &request) const;
		inline virtual EnumRequests interfaceIdx() const { return REQ_TORUS; }
	}; // CTorusRibRequest

	/** @brief Handles RIB request Points ...
	 */
	class CPointsRibRequest : public CRibRequest {
	public:
		virtual void operator()(IRibParserState &parser, CRibRequestData &request) const;
		inline virtual EnumRequests interfaceIdx() const { return REQ_POINTS; }
	}; // CPointsRibRequest

	/** @brief Handles RIB request Curves ...
	 */
	class CCurvesRibRequest : public CRibRequest {
	public:
		virtual void operator()(IRibParserState &parser, CRibRequestData &request) const;
		inline virtual EnumRequests interfaceIdx() const { return REQ_CURVES; }
	}; // CCurvesRibRequest

	/** @brief Handles RIB request Blobby nleaf [ codes ] [ floats ] [ strings ] ...
	 */
	class CBlobbyRibRequest : public CRibRequest {
	public:
		virtual void operator()(IRibParserState &parser, CRibRequestData &request) const;
		inline virtual EnumRequests interfaceIdx() const { return REQ_BLOBBY; }
	}; // CBlobbyRibRequest

	/** @brief Handles RIB request Procedural procname [ args ] [ bounds ] ...
	 */
	class CProceduralRibRequest : public CRibRequest {
	public:
		virtual void operator()(IRibParserState &parser, CRibRequestData &request) const;
		inline virtual EnumRequests interfaceIdx() const { return REQ_PROCEDURAL; }
	}; // CProceduralRibRequest

	/** @brief Handles RIB request Geometry ...
	 */
	class CGeometryRibRequest : public CRibRequest {
	public:
		virtual void operator()(IRibParserState &parser, CRibRequestData &request) const;
		inline virtual EnumRequests interfaceIdx() const { return REQ_GEOMETRY; }
	}; // CGeometryRibRequest
}

#endif // _RICPP_RIBPARSER_RIBPRIMS_H
