#ifndef _RICPP_RIBPARSER_RIBEXTERNALS_H
#define _RICPP_RIBPARSER_RIBEXTERNALS_H

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

/** @file ribexternals.h
 *  @author Andreas Pidde (andreas@pidde.de)
 *  @brief Handles RIB requests for externals (like textures).
 *
 *  The objects are used only by the implementation of the rib parser.
 */

#ifndef _RICPP_RIBPARSER_RIBPARSER_H
#include "ricpp/ribparser/ribparser.h"
#endif // _RICPP_RIBPARSER_RIBPARSER_H

namespace RiCPP {
	/** @brief Handles RIB request MakeTexture picturename texturename swrap twrap filter swidth twidth <paramlist>
	 */
	class CMakeTextureRibRequest : public CRibRequest {
	public:
		virtual void operator()(IRibParserState &parser, CRibRequestData &request) const;
		inline virtual EnumRequests interfaceIdx() const { return REQ_MAKE_TEXTURE; }
	}; // CMakeTextureRibRequest

	/** @brief Handles RIB request MakeBump picturename texturename swrap twrap filter swidth twidth <paramlist>
	 */
	class CMakeBumpRibRequest : public CRibRequest {
	public:
		virtual void operator()(IRibParserState &parser, CRibRequestData &request) const;
		inline virtual EnumRequests interfaceIdx() const { return REQ_MAKE_BUMP; }
	}; // CMakeBumpRibRequest

	/** @brief Handles RIB request MakeLatLongEnvironment picturename texturename filter swidth twidth <paramlist>
	 */
	class CMakeLatLongEnvironmentRibRequest : public CRibRequest {
	public:
		virtual void operator()(IRibParserState &parser, CRibRequestData &request) const;
		inline virtual EnumRequests interfaceIdx() const { return REQ_MAKE_LAT_LONG_ENVIRONMENT; }
	}; // CMakeLatLongEnvironmentRibRequest

	/** @brief Handles RIB request MakeCubeFaceEnvironment px nx py ny pz nz texturename fov filter swidth twidth <paramlist>
	 */
	class CMakeCubeFaceEnvironmentRibRequest : public CRibRequest {
	public:
		virtual void operator()(IRibParserState &parser, CRibRequestData &request) const;
		inline virtual EnumRequests interfaceIdx() const { return REQ_MAKE_CUBE_FACE_ENVIRONMENT; }
	}; // CMakeCubeFaceEnvironmentRibRequest

	/** @brief Handles RIB request MakeShadow picturename texturename <paramlist>
	 */
	class CMakeShadowRibRequest : public CRibRequest {
	public:
		virtual void operator()(IRibParserState &parser, CRibRequestData &request) const;
		inline virtual EnumRequests interfaceIdx() const { return REQ_MAKE_SHADOW; }
	}; // CMakeShadowRibRequest

	/** @brief Handles RIB request MakeBrickMap ptcname(s) bkmname <paramlist>
	 */
	class CMakeBrickMapRibRequest : public CRibRequest {
	public:
		virtual void operator()(IRibParserState &parser, CRibRequestData &request) const;
		inline virtual EnumRequests interfaceIdx() const { return REQ_MAKE_BRICK_MAP; }
	}; // CMakeBrickMapRibRequest
}

#endif // _RICPP_RIBPARSER_RIBEXTERNALS_H
