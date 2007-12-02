#ifndef _RICPP_RIBPARSER_RIBATTRIBUTES_H
#define _RICPP_RIBPARSER_RIBATTRIBUTES_H

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

/** @file ribattributes.h
 *  @author Andreas Pidde (andreas@pidde.de)
 *  @brief Handles RIB requests for attributes (Color, Surface, ...).
 *
 *  The objects are used only by the implementation of the rib attributes.
 */

#ifndef _RICPP_RIBPARSER_RIBPARSER_H
#include "ricpp/ribparser/ribparser.h"
#endif // _RICPP_RIBPARSER_RIBPARSER_H

namespace RiCPP {

	/** @brief Handles RIB request Attribute name ...
	 */
	class CAttributeRibRequest : public CRibRequest {
	public:
		virtual void operator()(IRibParserState &parser, CRibRequestData &request) const;
		inline virtual EnumRequests interfaceIdx() const { return REQ_ATTRIBUTE; }
	}; // CAttributeRibRequest

	/** @brief Handles RIB request Color c1 .. cn
	 */
	class CColorRibRequest : public CRibRequest {
	public:
		virtual void operator()(IRibParserState &parser, CRibRequestData &request) const;
		inline virtual EnumRequests interfaceIdx() const { return REQ_COLOR; }
	}; // CColorRibRequest

	/** @brief Handles RIB request Opacity o1 .. on
	 */
	class COpacityRibRequest : public CRibRequest {
	public:
		virtual void operator()(IRibParserState &parser, CRibRequestData &request) const;
		inline virtual EnumRequests interfaceIdx() const { return REQ_OPACITY; }
	}; // COpacityRibRequest

	/** @brief Handles RIB request Surface name ...
	 */
	class CSurfaceRibRequest : public CRibRequest {
	public:
		virtual void operator()(IRibParserState &parser, CRibRequestData &request) const;
		inline virtual EnumRequests interfaceIdx() const { return REQ_SURFACE; }
	}; // CSurfaceRibRequest

	/** @brief Handles RIB request Atmosphere name ...
	 */
	class CAtmosphereRibRequest : public CRibRequest {
	public:
		virtual void operator()(IRibParserState &parser, CRibRequestData &request) const;
		inline virtual EnumRequests interfaceIdx() const { return REQ_ATMOSPHERE; }
	}; // CAtmosphereRibRequest

	/** @brief Handles RIB request Interior name ...
	 */
	class CInteriorRibRequest : public CRibRequest {
	public:
		virtual void operator()(IRibParserState &parser, CRibRequestData &request) const;
		inline virtual EnumRequests interfaceIdx() const { return REQ_INTERIOR; }
	}; // CInteriorRibRequest

	/** @brief Handles RIB request Exterior name ...
	 */
	class CExteriorRibRequest : public CRibRequest {
	public:
		virtual void operator()(IRibParserState &parser, CRibRequestData &request) const;
		inline virtual EnumRequests interfaceIdx() const { return REQ_EXTERIOR; }
	}; // CExteriorRibRequest

	/** @brief Handles RIB request Displacement name ...
	 */
	class CDisplacementRibRequest : public CRibRequest {
	public:
		virtual void operator()(IRibParserState &parser, CRibRequestData &request) const;
		inline virtual EnumRequests interfaceIdx() const { return REQ_DISPLACEMENT; }
	}; // CDisplacementRibRequest

	/** @brief Handles RIB request TextureCoordinates s1 t1 s2 t2 s3 t3 s4 t4
	 */
	class CTextureCoordinatesRibRequest : public CRibRequest {
	public:
		virtual void operator()(IRibParserState &parser, CRibRequestData &request) const;
		inline virtual EnumRequests interfaceIdx() const { return REQ_TEXTURE_COORDINATES; }
	}; // CTextureCoordinatesRibRequest

	/** @brief Handles RIB request ShadingRate size
	 */
	class CShadingRateRibRequest : public CRibRequest {
	public:
		virtual void operator()(IRibParserState &parser, CRibRequestData &request) const;
		inline virtual EnumRequests interfaceIdx() const { return REQ_SHADING_RATE; }
	}; // CShadingRateRibRequest

	/** @brief Handles RIB request ShadingInterpolation type
	 */
	class CShadingInterpolationRibRequest : public CRibRequest {
	public:
		virtual void operator()(IRibParserState &parser, CRibRequestData &request) const;
		inline virtual EnumRequests interfaceIdx() const { return REQ_SHADING_INTERPOLATION; }
	}; // CShadingInterpolationRibRequest

	/** @brief Handles RIB request Matte onoff
	 */
	class CMatteRibRequest : public CRibRequest {
	public:
		virtual void operator()(IRibParserState &parser, CRibRequestData &request) const;
		inline virtual EnumRequests interfaceIdx() const { return REQ_MATTE; }
	}; // CMatteRibRequest

	/** @brief Handles RIB request Bound [ bound ]
	 */
	class CBoundRibRequest : public CRibRequest {
	public:
		virtual void operator()(IRibParserState &parser, CRibRequestData &request) const;
		inline virtual EnumRequests interfaceIdx() const { return REQ_BOUND; }
	}; // CBoundRibRequest

	/** @brief Handles RIB request Detail [ bound ]
	 */
	class CDetailRibRequest : public CRibRequest {
	public:
		virtual void operator()(IRibParserState &parser, CRibRequestData &request) const;
		inline virtual EnumRequests interfaceIdx() const { return REQ_DETAIL; }
	}; // CDetailRibRequest

	/** @brief Handles RIB request DetailRange minvis lowtran uptran maxvis
	 */
	class CDetailRangeRibRequest : public CRibRequest {
	public:
		virtual void operator()(IRibParserState &parser, CRibRequestData &request) const;
		inline virtual EnumRequests interfaceIdx() const { return REQ_DETAIL_RANGE; }
	}; // CDetailRangeRibRequest

	/** @brief Handles RIB request GeometricApproximation type value
	 */
	class CGeometricApproximationRibRequest : public CRibRequest {
	public:
		virtual void operator()(IRibParserState &parser, CRibRequestData &request) const;
		inline virtual EnumRequests interfaceIdx() const { return REQ_GEOMETRIC_APPROXIMATION; }
	}; // CGeometricApproximationRibRequest

	/** @brief Handles RIB request GeometricRepresentation type
	 */
	class CGeometricRepresentationRibRequest : public CRibRequest {
	public:
		virtual void operator()(IRibParserState &parser, CRibRequestData &request) const;
		inline virtual EnumRequests interfaceIdx() const { return REQ_GEOMETRIC_REPRESENTATION; }
	}; // CGeometricRepresentationRibRequest

	/** @brief Handles RIB request Orientation orientation
	 */
	class COrientationRibRequest : public CRibRequest {
	public:
		virtual void operator()(IRibParserState &parser, CRibRequestData &request) const;
		inline virtual EnumRequests interfaceIdx() const { return REQ_ORIENTATION; }
	}; // COrientationRibRequest

	/** @brief Handles RIB request ReverseOrientation
	 */
	class CReverseOrientationRibRequest : public CRibRequest {
	public:
		virtual void operator()(IRibParserState &parser, CRibRequestData &request) const;
		inline virtual EnumRequests interfaceIdx() const { return REQ_REVERSE_ORIENTATION; }
	}; // CReverseOrientationRibRequest

	/** @brief Handles RIB request Sides sides
	 */
	class CSidesRibRequest : public CRibRequest {
	public:
		virtual void operator()(IRibParserState &parser, CRibRequestData &request) const;
		inline virtual EnumRequests interfaceIdx() const { return REQ_SIDES; }
	}; // CSidesRibRequest

	/** @brief Handles RIB request Basis [ubasis] ustep [vbasis] vstep
	 */
	class CBasisRibRequest : public CRibRequest {
	public:
		inline virtual void operator()(IRibParserState &parser, CRibRequestData &request) const {}
		inline virtual EnumRequests interfaceIdx() const { return REQ_BASIS; }
	}; // CBasisRibRequest

	/** @brief Handles RIB request TrimCurve [ncurves] [order] [knot] [min] [max] [n] [u] [v] [w]
	 */
	class CTrimCurveRibRequest : public CRibRequest {
	public:
		inline virtual void operator()(IRibParserState &parser, CRibRequestData &request) const {}
		inline virtual EnumRequests interfaceIdx() const { return REQ_TRIM_CURVE; }
	}; // CTrimCurveRibRequest
}

#endif // _RICPP_RIBPARSER_RIBATTRIBUTES_H
