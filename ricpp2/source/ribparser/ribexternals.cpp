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

/** @file ribexternals.cpp
 *  @author Andreas Pidde (andreas@pidde.de)
 *  @brief Implementation of the handling of RIB requests for external resources (like textures).
 */

#include "ricpp/ribparser/ribexternals.h"

using namespace RiCPP;

void CMakeTextureRibRequest::operator()(IRibParserState &parser, CRibRequestData &request) const
{
	// MakeTexture picturename texturename swrap twrap filter swidth twidth <paramlist>
	if ( request.size() >= 7 ) {
		CRibParameter &p0 = request[0];
		CRibParameter &p1 = request[1];
		CRibParameter &p2 = request[2];
		CRibParameter &p3 = request[3];
		CRibParameter &p4 = request[4];
		CRibParameter &p5 = request[5];
		CRibParameter &p6 = request[6];

		const char *pic = 0, *tex = 0, *filtername = 0;
		RtToken swrap = 0, twrap = 0;
		RtFloat swidth, twidth;

		bool b0=p0.getString(pic), b1=p1.getString(tex), b2=p2.getString(swrap), b3=p3.getString(twrap), b4=p4.getString(filtername), b5=p5.getFloat(swidth), b6=p6.getFloat(twidth);

		if ( b0 && b1 && b2 && b3 && b4 && b5 && b6 ) {
			const IFilterFunc *func = 0;
			RtToken name = parser.renderState().tokFind(filtername);
			if ( name )
				func = parser.renderState().filterFunc(name);

			if ( func ) {
				CParameterClasses p;
				int n = request.getTokenList(7, p);

				if ( n > 0 )
					parser.ribFilter().makeTextureV(pic, tex, swrap, twrap, *func, swidth, twidth, n, request.tokenList(), request.valueList());
				else
					parser.ribFilter().makeTextureV(pic, tex, swrap, twrap, *func, swidth, twidth, 0, 0, 0);
			} else {
				parser.errHandler().handleError(
					RIE_CONSISTENCY, RIE_ERROR,
					"Line %ld, File \"%s\", badargument: '%s' argument %s=%s is not a known pixel filter",
					p0.lineNo(), parser.resourceName(),
					requestName(), "1 (filtername)", name, RI_NULL);
			}

		} else {
			if ( !b0 ) {
				parser.errHandler().handleError(
					RIE_CONSISTENCY, RIE_ERROR,
					"Line %ld, File \"%s\", badargument: '%s' argument %s is not a string",
					p0.lineNo(), parser.resourceName(),
					requestName(), "1 (picturename)", RI_NULL);
			}
			if ( !b1 ) {
				parser.errHandler().handleError(
					RIE_CONSISTENCY, RIE_ERROR,
					"Line %ld, File \"%s\", badargument: '%s' argument %s is not a string",
					p0.lineNo(), parser.resourceName(),
					requestName(), "2 (texturename)", RI_NULL);
			}
			if ( !b2 ) {
				parser.errHandler().handleError(
					RIE_CONSISTENCY, RIE_ERROR,
					"Line %ld, File \"%s\", badargument: '%s' argument %s is not a string",
					p0.lineNo(), parser.resourceName(),
					requestName(), "3 (swrap)", RI_NULL);
			}
			if ( !b3 ) {
				parser.errHandler().handleError(
					RIE_CONSISTENCY, RIE_ERROR,
					"Line %ld, File \"%s\", badargument: '%s' argument %s is not a string",
					p0.lineNo(), parser.resourceName(),
					requestName(), "4 (twrap)", RI_NULL);
			}
			if ( !b4 ) {
				parser.errHandler().handleError(
					RIE_CONSISTENCY, RIE_ERROR,
					"Line %ld, File \"%s\", badargument: '%s' argument %s is not a string",
					p0.lineNo(), parser.resourceName(),
					requestName(), "5 (filter)", RI_NULL);
			}
			if ( !b5 ) {
				parser.errHandler().handleError(
					RIE_CONSISTENCY, RIE_ERROR,
					"Line %ld, File \"%s\", badargument: '%s' argument %s is not numeric",
					p0.lineNo(), parser.resourceName(),
					requestName(), "6 (swidth)", RI_NULL);
			}
			if ( !b6 ) {
				parser.errHandler().handleError(
					RIE_CONSISTENCY, RIE_ERROR,
					"Line %ld, File \"%s\", badargument: '%s' argument %s is not numeric",
					p0.lineNo(), parser.resourceName(),
					requestName(), "7 (twidth)", RI_NULL);
			}
		}
	} else {
		parser.errHandler().handleError(
			RIE_MISSINGDATA, RIE_ERROR,
			"Line %ld, File \"%s\", badargument: '%s' argument %s missing",
			parser.lineNo(), parser.resourceName(),
			requestName(), "(picturename texturename swrap twrap filter swidth twidth <paramlist>)", RI_NULL);
	}
}

void CMakeBumbRibRequest::operator()(IRibParserState &parser, CRibRequestData &request) const
{
	// MakeTexture picturename texturename swrap twrap filter swidth twidth <paramlist>
	if ( request.size() >= 7 ) {
		CRibParameter &p0 = request[0];
		CRibParameter &p1 = request[1];
		CRibParameter &p2 = request[2];
		CRibParameter &p3 = request[3];
		CRibParameter &p4 = request[4];
		CRibParameter &p5 = request[5];
		CRibParameter &p6 = request[6];

		const char *pic = 0, *tex = 0, *filtername = 0;
		RtToken swrap = 0, twrap = 0;
		RtFloat swidth, twidth;

		bool b0=p0.getString(pic), b1=p1.getString(tex), b2=p2.getString(swrap), b3=p3.getString(twrap), b4=p4.getString(filtername), b5=p5.getFloat(swidth), b6=p6.getFloat(twidth);

		if ( b0 && b1 && b2 && b3 && b4 && b5 && b6 ) {
			const IFilterFunc *func = 0;
			RtToken name = parser.renderState().tokFind(filtername);
			if ( name )
				func = parser.renderState().filterFunc(name);

			if ( func ) {
				CParameterClasses p;
				int n = request.getTokenList(7, p);

				if ( n > 0 )
					parser.ribFilter().makeBumpV(pic, tex, swrap, twrap, *func, swidth, twidth, n, request.tokenList(), request.valueList());
				else
					parser.ribFilter().makeBumpV(pic, tex, swrap, twrap, *func, swidth, twidth, 0, 0, 0);
			} else {
				parser.errHandler().handleError(
					RIE_CONSISTENCY, RIE_ERROR,
					"Line %ld, File \"%s\", badargument: '%s' argument %s=%s is not a known pixel filter",
					p0.lineNo(), parser.resourceName(),
					requestName(), "1 (filtername)", name, RI_NULL);
			}

		} else {
			if ( !b0 ) {
				parser.errHandler().handleError(
					RIE_CONSISTENCY, RIE_ERROR,
					"Line %ld, File \"%s\", badargument: '%s' argument %s is not a string",
					p0.lineNo(), parser.resourceName(),
					requestName(), "1 (picturename)", RI_NULL);
			}
			if ( !b1 ) {
				parser.errHandler().handleError(
					RIE_CONSISTENCY, RIE_ERROR,
					"Line %ld, File \"%s\", badargument: '%s' argument %s is not a string",
					p0.lineNo(), parser.resourceName(),
					requestName(), "2 (texturename)", RI_NULL);
			}
			if ( !b2 ) {
				parser.errHandler().handleError(
					RIE_CONSISTENCY, RIE_ERROR,
					"Line %ld, File \"%s\", badargument: '%s' argument %s is not a string",
					p0.lineNo(), parser.resourceName(),
					requestName(), "3 (swrap)", RI_NULL);
			}
			if ( !b3 ) {
				parser.errHandler().handleError(
					RIE_CONSISTENCY, RIE_ERROR,
					"Line %ld, File \"%s\", badargument: '%s' argument %s is not a string",
					p0.lineNo(), parser.resourceName(),
					requestName(), "4 (twrap)", RI_NULL);
			}
			if ( !b4 ) {
				parser.errHandler().handleError(
					RIE_CONSISTENCY, RIE_ERROR,
					"Line %ld, File \"%s\", badargument: '%s' argument %s is not a string",
					p0.lineNo(), parser.resourceName(),
					requestName(), "5 (filter)", RI_NULL);
			}
			if ( !b5 ) {
				parser.errHandler().handleError(
					RIE_CONSISTENCY, RIE_ERROR,
					"Line %ld, File \"%s\", badargument: '%s' argument %s is not numeric",
					p0.lineNo(), parser.resourceName(),
					requestName(), "6 (swidth)", RI_NULL);
			}
			if ( !b6 ) {
				parser.errHandler().handleError(
					RIE_CONSISTENCY, RIE_ERROR,
					"Line %ld, File \"%s\", badargument: '%s' argument %s is not numeric",
					p0.lineNo(), parser.resourceName(),
					requestName(), "7 (twidth)", RI_NULL);
			}
		}
	} else {
		parser.errHandler().handleError(
			RIE_MISSINGDATA, RIE_ERROR,
			"Line %ld, File \"%s\", badargument: '%s' argument %s missing",
			parser.lineNo(), parser.resourceName(),
			requestName(), "(picturename texturename swrap twrap filter swidth twidth <paramlist>)", RI_NULL);
	}
}

void CMakeLatLongEnvironmentRibRequest::operator()(IRibParserState &parser, CRibRequestData &request) const
{
	// MakeLatLongEnvironment picturename texturename filter swidth twidth <paramlist>
	if ( request.size() >= 5 ) {
		CRibParameter &p0 = request[0];
		CRibParameter &p1 = request[1];
		CRibParameter &p2 = request[2];
		CRibParameter &p3 = request[3];
		CRibParameter &p4 = request[4];

		const char *pic = 0, *tex = 0, *filtername = 0;
		RtFloat swidth, twidth;

		bool b0=p0.getString(pic), b1=p1.getString(tex), b2=p2.getString(filtername), b3=p3.getFloat(swidth), b4=p4.getFloat(twidth);

		if ( b0 && b1 && b2 && b3 && b4 ) {
			const IFilterFunc *func = 0;
			RtToken name = parser.renderState().tokFind(filtername);
			if ( name )
				func = parser.renderState().filterFunc(name);

			if ( func ) {
				CParameterClasses p;
				int n = request.getTokenList(7, p);

				if ( n > 0 )
					parser.ribFilter().makeLatLongEnvironmentV(pic, tex, *func, swidth, twidth, n, request.tokenList(), request.valueList());
				else
					parser.ribFilter().makeLatLongEnvironmentV(pic, tex, *func, swidth, twidth, 0, 0, 0);
			} else {
				parser.errHandler().handleError(
					RIE_CONSISTENCY, RIE_ERROR,
					"Line %ld, File \"%s\", badargument: '%s' argument %s=%s is not a known pixel filter",
					p0.lineNo(), parser.resourceName(),
					requestName(), "1 (filtername)", name, RI_NULL);
			}

		} else {
			if ( !b0 ) {
				parser.errHandler().handleError(
					RIE_CONSISTENCY, RIE_ERROR,
					"Line %ld, File \"%s\", badargument: '%s' argument %s is not a string",
					p0.lineNo(), parser.resourceName(),
					requestName(), "1 (picturename)", RI_NULL);
			}
			if ( !b1 ) {
				parser.errHandler().handleError(
					RIE_CONSISTENCY, RIE_ERROR,
					"Line %ld, File \"%s\", badargument: '%s' argument %s is not a string",
					p0.lineNo(), parser.resourceName(),
					requestName(), "2 (texturename)", RI_NULL);
			}
			if ( !b2 ) {
				parser.errHandler().handleError(
					RIE_CONSISTENCY, RIE_ERROR,
					"Line %ld, File \"%s\", badargument: '%s' argument %s is not a string",
					p0.lineNo(), parser.resourceName(),
					requestName(), "3 (filter)", RI_NULL);
			}
			if ( !b3 ) {
				parser.errHandler().handleError(
					RIE_CONSISTENCY, RIE_ERROR,
					"Line %ld, File \"%s\", badargument: '%s' argument %s is not numeric",
					p0.lineNo(), parser.resourceName(),
					requestName(), "4 (swidth)", RI_NULL);
			}
			if ( !b4 ) {
				parser.errHandler().handleError(
					RIE_CONSISTENCY, RIE_ERROR,
					"Line %ld, File \"%s\", badargument: '%s' argument %s is not numeric",
					p0.lineNo(), parser.resourceName(),
					requestName(), "5 (twidth)", RI_NULL);
			}
		}
	} else {
		parser.errHandler().handleError(
			RIE_MISSINGDATA, RIE_ERROR,
			"Line %ld, File \"%s\", badargument: '%s' argument %s missing",
			parser.lineNo(), parser.resourceName(),
			requestName(), "(picturename texturename filter swidth twidth <paramlist>)", RI_NULL);
	}
}

void CMakeCubeFaceEnvironmentRibRequest::operator()(IRibParserState &parser, CRibRequestData &request) const
{
	// MakeCubeFaceEnvironment px nx py ny pz nz texturename fov filter swidth twidth <paramlist>
	if ( request.size() >= 11 ) {
		CRibParameter &p0  = request[0];
		CRibParameter &p1  = request[1];
		CRibParameter &p2  = request[2];
		CRibParameter &p3  = request[3];
		CRibParameter &p4  = request[4];
		CRibParameter &p5  = request[5];
		CRibParameter &p6  = request[6];
		CRibParameter &p7  = request[7];
		CRibParameter &p8  = request[8];
		CRibParameter &p9  = request[9];
		CRibParameter &p10 = request[10];

		const char *px=0, *nx=0, *py=0, *ny=0, *pz=0, *nz=0, *tex=0;
		RtFloat fov;
		const char *filtername=0;
		RtFloat swidth, twidth;

		bool b0 = p0.getString(px),         b1 = p1.getString(nx),
			 b2 = p2.getString(py),         b3 = p3.getString(ny),
			 b4 = p4.getString(pz),         b5 = p5.getString(nz),
			 b6 = p6.getString(tex),        b7 = p7.getFloat(fov),
			 b8 = p8.getString(filtername), b9 = p9.getFloat(swidth), b10 = p10.getFloat(twidth);

		if ( b0 && b1 && b2 && b3 && b4 && b5 && b6 && b7 && b8 && b9 && b10 ) {
			const IFilterFunc *func = 0;
			RtToken name = parser.renderState().tokFind(filtername);
			if ( name )
				func = parser.renderState().filterFunc(name);

			if ( func ) {
				CParameterClasses p;
				int n = request.getTokenList(7, p);

				if ( n > 0 )
					parser.ribFilter().makeCubeFaceEnvironmentV(px, nx, py, ny, pz, nz, tex, fov, *func, swidth, twidth, n, request.tokenList(), request.valueList());
				else
					parser.ribFilter().makeCubeFaceEnvironmentV(px, nx, py, ny, pz, nz, tex, fov, *func, swidth, twidth, 0, 0, 0);

			} else {
				parser.errHandler().handleError(
					RIE_CONSISTENCY, RIE_ERROR,
					"Line %ld, File \"%s\", badargument: '%s' argument %s=%s is not a known pixel filter",
					p0.lineNo(), parser.resourceName(),
					requestName(), "9 (filtername)", name, RI_NULL);
			}

		} else {
			if ( !b0 ) {
				parser.errHandler().handleError(
					RIE_CONSISTENCY, RIE_ERROR,
					"Line %ld, File \"%s\", badargument: '%s' argument %s is not a string",
					p0.lineNo(), parser.resourceName(),
					requestName(), "1 (px)", RI_NULL);
			}
			if ( !b1 ) {
				parser.errHandler().handleError(
					RIE_CONSISTENCY, RIE_ERROR,
					"Line %ld, File \"%s\", badargument: '%s' argument %s is not a string",
					p0.lineNo(), parser.resourceName(),
					requestName(), "2 (nx)", RI_NULL);
			}
			if ( !b2 ) {
				parser.errHandler().handleError(
					RIE_CONSISTENCY, RIE_ERROR,
					"Line %ld, File \"%s\", badargument: '%s' argument %s is not a string",
					p0.lineNo(), parser.resourceName(),
					requestName(), "3 (py)", RI_NULL);
			}
			if ( !b3 ) {
				parser.errHandler().handleError(
					RIE_CONSISTENCY, RIE_ERROR,
					"Line %ld, File \"%s\", badargument: '%s' argument %s is not numeric",
					p0.lineNo(), parser.resourceName(),
					requestName(), "4 (ny)", RI_NULL);
			}
			if ( !b4 ) {
				parser.errHandler().handleError(
					RIE_CONSISTENCY, RIE_ERROR,
					"Line %ld, File \"%s\", badargument: '%s' argument %s is not numeric",
					p0.lineNo(), parser.resourceName(),
					requestName(), "5 (pz)", RI_NULL);
			}
			if ( !b5 ) {
				parser.errHandler().handleError(
					RIE_CONSISTENCY, RIE_ERROR,
					"Line %ld, File \"%s\", badargument: '%s' argument %s is not a string",
					p0.lineNo(), parser.resourceName(),
					requestName(), "6 (nz)", RI_NULL);
			}
			if ( !b6 ) {
				parser.errHandler().handleError(
					RIE_CONSISTENCY, RIE_ERROR,
					"Line %ld, File \"%s\", badargument: '%s' argument %s is not a string",
					p0.lineNo(), parser.resourceName(),
					requestName(), "7 (texturename)", RI_NULL);
			}
			if ( !b7 ) {
				parser.errHandler().handleError(
					RIE_CONSISTENCY, RIE_ERROR,
					"Line %ld, File \"%s\", badargument: '%s' argument %s is not numeric",
					p0.lineNo(), parser.resourceName(),
					requestName(), "8 (fov)", RI_NULL);
			}
			if ( !b8 ) {
				parser.errHandler().handleError(
					RIE_CONSISTENCY, RIE_ERROR,
					"Line %ld, File \"%s\", badargument: '%s' argument %s is not a string",
					p0.lineNo(), parser.resourceName(),
					requestName(), "9 (filter)", RI_NULL);
			}
			if ( !b9 ) {
				parser.errHandler().handleError(
					RIE_CONSISTENCY, RIE_ERROR,
					"Line %ld, File \"%s\", badargument: '%s' argument %s is not numeric",
					p0.lineNo(), parser.resourceName(),
					requestName(), "10 (swidth)", RI_NULL);
			}
			if ( !b10 ) {
				parser.errHandler().handleError(
					RIE_CONSISTENCY, RIE_ERROR,
					"Line %ld, File \"%s\", badargument: '%s' argument %s is not numeric",
					p0.lineNo(), parser.resourceName(),
					requestName(), "11 (twidth)", RI_NULL);
			}
		}
	} else {
		parser.errHandler().handleError(
			RIE_MISSINGDATA, RIE_ERROR,
			"Line %ld, File \"%s\", badargument: '%s' argument %s missing",
			parser.lineNo(), parser.resourceName(),
			requestName(), "(px nx py ny pz nz texturename fov filter swidth twidth <paramlist>)", RI_NULL);
	}
}

void CMakeShadowRibRequest::operator()(IRibParserState &parser, CRibRequestData &request) const
{
	// MakeShadow picturename texturename <paramlist>
	if ( request.size() >= 2 ) {
		CRibParameter &p0 = request[0];
		CRibParameter &p1 = request[1];

		const char *pic = 0, *tex = 0;

		bool b0=p0.getString(pic), b1=p1.getString(tex);

		if ( b0 && b1 ) {
			CParameterClasses p;
			int n = request.getTokenList(7, p);

			if ( n > 0 )
				parser.ribFilter().makeShadowV(pic, tex, n, request.tokenList(), request.valueList());
			else
				parser.ribFilter().makeShadowV(pic, tex, 0, 0, 0);
		} else {
			if ( !b0 ) {
				parser.errHandler().handleError(
					RIE_CONSISTENCY, RIE_ERROR,
					"Line %ld, File \"%s\", badargument: '%s' argument %s is not a string",
					p0.lineNo(), parser.resourceName(),
					requestName(), "1 (picturename)", RI_NULL);
			}
			if ( !b1 ) {
				parser.errHandler().handleError(
					RIE_CONSISTENCY, RIE_ERROR,
					"Line %ld, File \"%s\", badargument: '%s' argument %s is not a string",
					p0.lineNo(), parser.resourceName(),
					requestName(), "2 (texturename)", RI_NULL);
			}
		}
	} else {
		parser.errHandler().handleError(
			RIE_MISSINGDATA, RIE_ERROR,
			"Line %ld, File \"%s\", badargument: '%s' argument %s missing",
			parser.lineNo(), parser.resourceName(),
			requestName(), "(picturename texturename)", RI_NULL);
	}
}
