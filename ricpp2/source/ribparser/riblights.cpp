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

/** @file riblights.cpp
 *  @author Andreas Pidde (andreas@pidde.de)
 *  @brief Implementation of the handling of RIB requests for light sources.
 */

#include "ricpp/ribparser/riblights.h"

using namespace RiCPP;

void CLightSourceRibRequest::operator()(IRibParserState &parser, CRibRequestData &request) const
{
	// LightSource name lightnumber <paramlist>
	// LightSource name lightname <paramlist>

	if ( request.size() >= 1 ) {

		CRibParameter &p0 = request[0];
		CRibParameter &p1 = request[1];

		RtInt number;
		const char *name;
		const char *lightname = 0;

		bool b0 = p0.getString(name);
		bool b1 = p1.getInt(number);
		bool b2 = false;

		if ( !b1 ) {
			b2 = p1.getString(lightname);
		}

		if ( b0 && (b1||b2) ) {

			CParameterClasses p;
			int n = request.getTokenList(2, p, RI_LIGHT_SOURCE, name);
			if ( n >= 0 ) {
				RtLightHandle light = 0;
				if ( n > 0 )
					light = parser.ribFilter().lightSourceV(name, n, request.tokenList(), request.valueList());
				else
					light = parser.ribFilter().lightSourceV(name, 0, 0, 0);

				if ( b1 ) {
					if ( !parser.bindLightHandle(light, number) ) {
						// Internal Error
						parser.errHandler().handleError(
							RIE_BUG, RIE_ERROR,
							"Line %ld, File \"%s\", badargument: '%s' argument %s cannot bind lighthandle %d",
							parser.lineNo(), parser.resourceName(),
							requestName(), "2 (lighthandle)", (int)number, RI_NULL);
					}
				} else {
					// b2 string as Lighthandle
					if ( !parser.bindLightHandle(light, lightname) ) {
						// Internal Error
						parser.errHandler().handleError(
							RIE_BUG, RIE_ERROR,
							"Line %ld, File \"%s\", badargument: '%s' argument %s cannot bind lighthandle \"%s\"",
							parser.lineNo(), parser.resourceName(),
							requestName(), "2 (lighthandle)", lightname, RI_NULL);
					}
				}
			}

		} else {
			if ( !b0 ) {
				parser.errHandler().handleError(
					RIE_CONSISTENCY, RIE_ERROR,
					"Line %ld, File \"%s\", badargument: '%s' argument %s is not a string",
					parser.lineNo(), parser.resourceName(),
					requestName(), "1 (name)", RI_NULL);
			}
			if ( !b1 && !b2 ) {
				parser.errHandler().handleError(
					RIE_CONSISTENCY, RIE_ERROR,
					"Line %ld, File \"%s\", badargument: '%s' argument %s is not numeric",
					parser.lineNo(), parser.resourceName(),
					requestName(), "2 (lightnumber)", RI_NULL);
			}
		}

	} else {

		parser.errHandler().handleError(
			RIE_MISSINGDATA, RIE_ERROR,
			"Line %ld, File \"%s\", badargument: '%s' argument %s missing",
			parser.lineNo(), parser.resourceName(),
			requestName(), "(name, lightnumber, ...)", RI_NULL);
	}
}


void CAreaLightSourceRibRequest::operator()(IRibParserState &parser, CRibRequestData &request) const
{
	// AreaLightSource name lightnumber <paramlist>
	// AreaLightSource name lightname <paramlist>

	if ( request.size() >= 1 ) {

		CRibParameter &p0 = request[0];
		CRibParameter &p1 = request[1];

		RtInt number;
		const char *name;
		const char *lightname = 0;

		bool b0 = p0.getString(name);
		bool b1 = p1.getInt(number);
		bool b2 = false;

		if ( !b1 ) {
			b2 = p1.getString(lightname);
		}

		if ( b0 && (b1||b2) ) {

			CParameterClasses p;
			int n = request.getTokenList(2, p, RI_AREA_LIGHT_SOURCE, name);
			if ( n >= 0 ) {
				RtLightHandle light = 0;
				if ( n > 0 )
					light = parser.ribFilter().areaLightSourceV(name, n, request.tokenList(), request.valueList());
				else
					light = parser.ribFilter().areaLightSourceV(name, 0, 0, 0);

				if ( b1 ) {
					if ( !parser.bindLightHandle(light, number) ) {
						// Internal Error
						parser.errHandler().handleError(
							RIE_BUG, RIE_ERROR,
							"Line %ld, File \"%s\", badargument: '%s' argument %s cannot bind lighthandle %d",
							parser.lineNo(), parser.resourceName(),
							requestName(), "2 (lighthandle)", (int)number, RI_NULL);
					}
				} else {
					// b2 string as Lighthandle
					if ( !parser.bindLightHandle(light, lightname) ) {
						// Internal Error
						parser.errHandler().handleError(
							RIE_BUG, RIE_ERROR,
							"Line %ld, File \"%s\", badargument: '%s' argument %s cannot bind lighthandle \"%s\"",
							parser.lineNo(), parser.resourceName(),
							requestName(), "2 (lighthandle)", lightname, RI_NULL);
					}
				}
			}

		} else {
			if ( !b0 ) {
				parser.errHandler().handleError(
					RIE_CONSISTENCY, RIE_ERROR,
					"Line %ld, File \"%s\", badargument: '%s' argument %s is not a string",
					parser.lineNo(), parser.resourceName(),
					requestName(), "1 (name)", RI_NULL);
			}
			if ( !b1 && !b2 ) {
				parser.errHandler().handleError(
					RIE_CONSISTENCY, RIE_ERROR,
					"Line %ld, File \"%s\", badargument: '%s' argument %s is not numeric",
					parser.lineNo(), parser.resourceName(),
					requestName(), "2 (lightnumber)", RI_NULL);
			}
		}

	} else {

		parser.errHandler().handleError(
			RIE_MISSINGDATA, RIE_ERROR,
			"Line %ld, File \"%s\", badargument: '%s' argument %s missing",
			parser.lineNo(), parser.resourceName(),
			requestName(), "(name, lightnumber, ...)", RI_NULL);
	}
}


void CIlluminateRibRequest::operator()(IRibParserState &parser, CRibRequestData &request) const
{
	// Illuminate [ lightnumber onoff ] (added)
	// Illuminate lightnumber onoff
	// Illuminate lightname onoff
	if ( request.size() == 1 ) {
		CRibParameter &p0 = request[0];
		p0.convertFloatToInt();
		if ( p0.typeID() != BASICTYPE_INTEGER ) {
			parser.errHandler().handleError(
				RIE_CONSISTENCY, RIE_ERROR,
				"Line %ld, File \"%s\", badargument: '%s' argument %s=%d is not an array of integers",
				parser.lineNo(), parser.resourceName(),
				requestName(), "1 [lightnumber onoff]", RI_NULL);
			return;
		}
		if ( !p0.isArray() || p0.getCard() < 2 ) {
			parser.errHandler().handleError(
				RIE_CONSISTENCY, RIE_ERROR,
				"Line %ld, File \"%s\", badargument: '%s' must have (an array of) 2 floats (%s) as arguments",
				parser.lineNo(), parser.resourceName(),
				requestName(), "lightnumber onoff", RI_NULL);
			return;
		}
		RtInt *pv = (RtInt *)p0.getValue();
		RtLightHandle light;

		if ( parser.getLightHandle(light, pv[0]) ) {
			parser.ribFilter().illuminate(light, pv[1] ? 1 : 0);
		} else {
			parser.errHandler().handleError(
				RIE_CONSISTENCY, RIE_ERROR,
				"Line %ld, File \"%s\", badhandle: '%s' argument 1 (lightnumber=%d) is not a valid",
				parser.lineNo(), parser.resourceName(),
				requestName(), "lightnumber onoff", pv[0], RI_NULL);
		}

		if ( p0.getCard() > 2 ) {
			parser.errHandler().handleError(
				RIE_CONSISTENCY, RIE_ERROR,
				"Line %ld, File \"%s\", badhandle: '%s' additional numbers are ignored",
				parser.lineNo(), parser.resourceName(),
				requestName(), pv[0], RI_NULL);
		}

	} else if ( request.size() >= 2 ) {
		CRibParameter &p0 = request[0];
		CRibParameter &p1 = request[1];

		const char *lightname = 0;
		RtInt number;
		RtInt onoff;
		RtLightHandle light;

		bool b0 = p0.getInt(number), b1 = p1.getInt(onoff);
		bool b2 = false;

		if ( !b0 ) {
			b2 = p0.getString(lightname);
		}
		if ( (b0 || b2) && b1 ) {
			if ( b0 ) {
				if ( parser.getLightHandle(light, number) ) {
					parser.ribFilter().illuminate(light, onoff ? 1 : 0);
				} else {
					parser.errHandler().handleError(
						RIE_CONSISTENCY, RIE_ERROR,
						"Line %ld, File \"%s\", badargument: '%s' argument %s=%d is not a valid",
						parser.lineNo(), parser.resourceName(),
						requestName(), "1 (lightnumber)", number, RI_NULL);
				}
			} else {
				if ( parser.getLightHandle(light, lightname) ) {
					parser.ribFilter().illuminate(light, onoff ? 1 : 0);
				} else {
					parser.errHandler().handleError(
						RIE_CONSISTENCY, RIE_ERROR,
						"Line %ld, File \"%s\", badargument: '%s' argument %s=\"%s\" is not a valid",
						parser.lineNo(), parser.resourceName(),
						requestName(), "1 (lightname)", lightname, RI_NULL);
				}
			}
		} else {
			if ( !(b0 || b2) ) {
				parser.errHandler().handleError(
					RIE_CONSISTENCY, RIE_ERROR,
					"Line %ld, File \"%s\", badargument: '%s' argument %s is not numeric or not a string",
					parser.lineNo(), parser.resourceName(),
					requestName(), "1 (lightnumber)", RI_NULL);
			}
			if ( !b1 ) {
				parser.errHandler().handleError(
					RIE_CONSISTENCY, RIE_ERROR,
					"Line %ld, File \"%s\", badargument: '%s' argument %s is not numeric",
					parser.lineNo(), parser.resourceName(),
					requestName(), "2 (onoff)", RI_NULL);
			}
		}
		if ( request.size() > 2 ) {
			parser.errHandler().handleError(
				RIE_CONSISTENCY, RIE_WARNING,
				"Line %ld, File \"%s\", badargument: additional parameters are ignored",
				parser.lineNo(), parser.resourceName(),
				requestName(), RI_NULL);
		}
	} else {
		parser.errHandler().handleError(
			RIE_MISSINGDATA, RIE_ERROR,
			"Line %ld, File \"%s\", badargument: '%s' argument %s missing",
			parser.lineNo(), parser.resourceName(),
			requestName(), "(lightnumber, onoff)", RI_NULL);
	}
}
