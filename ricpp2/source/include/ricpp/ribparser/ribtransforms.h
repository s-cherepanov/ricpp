#ifndef _RICPP_RIBPARSER_RIBTRANSFORMS_H
#define _RICPP_RIBPARSER_RIBTRANSFORMS_H

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

/** @file ribtransforms.h
 *  @author Andreas Pidde (andreas@pidde.de)
 *  @brief Handles RIB requests for transformations (ConcatTransform, Translate, Rotate, ...).
 *
 *  The objects are used only by the implementation of the rib parser.
 */

#ifndef _RICPP_RIBPARSER_RIBPARSER_H
#include "ricpp/ribparser/ribparser.h"
#endif // _RICPP_RIBPARSER_RIBPARSER_H

namespace RiCPP {

	/** @brief Handles RIB request Identity.
	 */
	class CIdentityRibRequest : public CRibRequest {
	public:
		virtual void operator()(IRibParserState &parser, CRibRequestData &request) const;
		inline virtual EnumRequests interfaceIdx() const { return REQ_IDENTITY; }
	}; // CIdentityRibRequest

	/** @brief Handles RIB request Transform [t1 .. t16].
	 */
	class CTransformRibRequest : public CRibRequest {
	public:
		virtual void operator()(IRibParserState &parser, CRibRequestData &request) const;
		inline virtual EnumRequests interfaceIdx() const { return REQ_TRANSFORM; }
	}; // CTransformRibRequest

	/** @brief Handles RIB request ConcatTransform [t1 .. t16].
	 */
	class CConcatTransformRibRequest : public CRibRequest {
	public:
		virtual void operator()(IRibParserState &parser, CRibRequestData &request) const;
		inline virtual EnumRequests interfaceIdx() const { return REQ_CONCAT_TRANSFORM; }
	}; // CConcatTransformRibRequest

	/** @brief Handles RIB request Perspective fov.
	 */
	class CPerspectiveRibRequest : public CRibRequest {
	public:
		virtual void operator()(IRibParserState &parser, CRibRequestData &request) const;
		inline virtual EnumRequests interfaceIdx() const { return REQ_PERSPECTIVE; }
	}; // CPerspectiveRibRequest

	/** @brief Handles RIB request Translate dx dy dz.
	 */
	class CTranslateRibRequest : public CRibRequest {
	public:
		virtual void operator()(IRibParserState &parser, CRibRequestData &request) const;
		inline virtual EnumRequests interfaceIdx() const { return REQ_TRANSLATE; }
	}; // CTranslateRibRequest

	/** @brief Handles RIB request Rotate angle x y z.
	 */
	class CRotateRibRequest : public CRibRequest {
	public:
		virtual void operator()(IRibParserState &parser, CRibRequestData &request) const;
		inline virtual EnumRequests interfaceIdx() const { return REQ_ROTATE; }
	}; // CRotateRibRequest

	/** @brief Handles RIB request Scale sx sy sz.
	 */
	class CScaleRibRequest : public CRibRequest {
	public:
		virtual void operator()(IRibParserState &parser, CRibRequestData &request) const;
		inline virtual EnumRequests interfaceIdx() const { return REQ_SCALE; }
	}; // CScaleRibRequest

	/** @brief Handles RIB request Skew dx1 dy1 dz1 dx2 dy2 dz2.
	 */
	class CSkewRibRequest : public CRibRequest {
	public:
		virtual void operator()(IRibParserState &parser, CRibRequestData &request) const;
		inline virtual EnumRequests interfaceIdx() const { return REQ_SKEW; }
	}; // CSkewRibRequest

	/** @brief Handles RIB request Deformation name ...
	 */
	class CDeformationRibRequest : public CRibRequest {
	public:
		virtual void operator()(IRibParserState &parser, CRibRequestData &request) const;
		inline virtual EnumRequests interfaceIdx() const { return REQ_DEFORMATION; }
	}; // CDeformationRibRequest

	/** @brief Handles RIB request ScopedCoordinateSystem name.
	 */
	class CScopedCoordinateSystemRibRequest : public CRibRequest {
	public:
		virtual void operator()(IRibParserState &parser, CRibRequestData &request) const;
		inline virtual EnumRequests interfaceIdx() const { return REQ_SCOPED_COORDINATE_SYSTEM; }
	}; // CScopedCoordinateSystemRibRequest

	/** @brief Handles RIB request CoordinateSystem name.
	 */
	class CCoordinateSystemRibRequest : public CRibRequest {
	public:
		virtual void operator()(IRibParserState &parser, CRibRequestData &request) const;
		inline virtual EnumRequests interfaceIdx() const { return REQ_COORDINATE_SYSTEM; }
	}; // CCoordinateSystemRibRequest

	/** @brief Handles RIB request CoordSysTransForm name.
	 */
	class CCoordSysTransformRibRequest : public CRibRequest {
	public:
		virtual void operator()(IRibParserState &parser, CRibRequestData &request) const;
		inline virtual EnumRequests interfaceIdx() const { return REQ_COORD_SYS_TRANSFORM; }
	}; // CCoordSysTransformRibRequest

	/** @brief Handles RIB request TransformPoints has no RIB binding.
	 */
	class CTransformPointsRibRequest : public CRibRequest {
	public:
		virtual void operator()(IRibParserState &parser, CRibRequestData &request) const;
		inline virtual EnumRequests interfaceIdx() const { return REQ_TRANSFORM_POINTS; }
	}; // CTransformPointsRibRequest
}

#endif // _RICPP_RIBPARSER_RIBTRANSFORMS_H
