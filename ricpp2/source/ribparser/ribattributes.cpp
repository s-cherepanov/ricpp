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

/** @file ribattributes.cpp
 *  @author Andreas Pidde (andreas@pidde.de)
 *  @brief Implementation of the handling of RIB requests for attributes.
 */

#include "ricpp/ribparser/ribattributes.h"

#ifndef _RICPP_RICONTEXT_ATTRIBUTES_H
#include "ricpp/ricontext/attributes.h"
#endif // _RICPP_RICONTEXT_ATTRIBUTES_H

using namespace RiCPP;


void CAttributeRibRequest::operator()(IRibParserState &parser, CRibRequestData &request) const
{
	// Attribute name <paramlist>

	if ( request.size() >= 1 ) {

		CRibParameter &p0 = request[0];
		const char *name;

		if ( p0.getString(name) ) {

			CParameterClasses p;
			int n = request.getTokenList(1, p);
			if ( n > 0 ) {
				parser.ribFilter().attributeV(name, n, request.tokenList(), request.valueList());
			} else {
				parser.ribFilter().attributeV(name, 0, 0, 0);
			}

		} else {
			parser.errHandler().handleError(
				RIE_CONSISTENCY, RIE_ERROR,
				"Line %ld, File \"%s\", badargument: '%s' argument %s is not a string",
				p0.lineNo(), parser.resourceName(),
				requestName(), "1 (name)", RI_NULL);
		}

	} else {

		parser.errHandler().handleError(
			RIE_MISSINGDATA, RIE_ERROR,
			"Line %ld, File \"%s\", badargument: '%s' argument %s missing",
			parser.lineNo(), parser.resourceName(),
			requestName(), "(name)", RI_NULL);
	}
}

void CColorRibRequest::operator()(IRibParserState &parser, CRibRequestData &request) const
{
	// Color c1 ... cn
	// Color [color]

	bool isArray = false;
	if ( request.size()== 1 ) {
		CRibParameter &p0 = request[0];
		isArray = p0.isArray();
	}

	if ( isArray ) {
		CRibParameter &p0 = request[0];

		p0.convertIntToFloat();

		if ( p0.typeID() != BASICTYPE_FLOAT ) {
			parser.errHandler().handleError(
				RIE_CONSISTENCY, RIE_ERROR,
				"Line %ld, File \"%s\", badargument: '%s' elements of argument %s are not numeric",
				p0.lineNo(), parser.resourceName(),
				requestName(), "1 ([color])", RI_NULL);
			return;
		}

		if ( p0.getCard() != parser.renderState().options().colorDescr().colorSamples() ) {
			parser.errHandler().handleError(
				RIE_CONSISTENCY, RIE_ERROR,
				"Line %ld, File \"%s\", badargument: '%s' arguments %s have not the correct color size",
				p0.lineNo(), parser.resourceName(),
				requestName(), "1 ([color])", RI_NULL);
			return;
		}

		parser.ribFilter().color((RtColor)p0.getValue());
		return;
	}
	
	
	if ( request.size() > 0 ) {

		if ( request.size() != parser.renderState().options().colorDescr().colorSamples() ) {
			parser.errHandler().handleError(
				RIE_CONSISTENCY, RIE_ERROR,
				"Line %ld, File \"%s\", badargument: '%s' arguments %s have not the correct color size",
				parser.lineNo(), parser.resourceName(),
				requestName(), "(c1 ... cn)", RI_NULL);
			return;
		}

		int size = (int)request.size();
		std::vector<RtFloat> color;
		color.reserve(size);
		int i;
		RtFloat fval;
		for ( i = 0; i < size; ++i ) {
			if ( request[i].getFloat(fval) ) {
				color.push_back(fval);
			} else {
				parser.errHandler().handleError(
					RIE_CONSISTENCY, RIE_ERROR,
					"Line %ld, File \"%s\", badargument: '%s' parameter %d (color) is not numeric, using 1",
					request[i].lineNo(), parser.resourceName(),
					requestName(), i, RI_NULL);
				color.push_back(1);
			}
		}

		parser.ribFilter().color((RtColor)&color[0]);

	} else {

		parser.errHandler().handleError(
			RIE_MISSINGDATA, RIE_ERROR,
			"Line %ld, File \"%s\", badargument: '%s' arguments %s are missing", 
			parser.lineNo(), parser.resourceName(),
			requestName(), "(c1 ... cn)", RI_NULL);

	}
}

void COpacityRibRequest::operator()(IRibParserState &parser, CRibRequestData &request) const
{
	// Opacity o1 ... on
	// Opacity [opacity]

	bool isArray = false;
	if ( request.size()== 1 ) {
		CRibParameter &p0 = request[0];
		isArray = p0.isArray();
	}

	if ( isArray ) {
		CRibParameter &p0 = request[0];

		p0.convertIntToFloat();

		if ( p0.typeID() != BASICTYPE_FLOAT ) {
			parser.errHandler().handleError(
				RIE_CONSISTENCY, RIE_ERROR,
				"Line %ld, File \"%s\", badargument: '%s' elements of argument %s are not numeric",
				p0.lineNo(), parser.resourceName(),
				requestName(), "1 ([opacity])", RI_NULL);
			return;
		}

		if ( p0.getCard() != parser.renderState().options().colorDescr().colorSamples() ) {
			parser.errHandler().handleError(
				RIE_CONSISTENCY, RIE_ERROR,
				"Line %ld, File \"%s\", badargument: '%s' arguments %s have not the correct color size",
				p0.lineNo(), parser.resourceName(),
				requestName(), "1 ([opacity])", RI_NULL);
			return;
		}

		parser.ribFilter().opacity((RtColor)p0.getValue());
		return;
	}
	
	
	if ( request.size() > 0 ) {

		if ( request.size() != parser.renderState().options().colorDescr().colorSamples() ) {
			parser.errHandler().handleError(
				RIE_CONSISTENCY, RIE_ERROR,
				"Line %ld, File \"%s\", badargument: '%s' arguments %s have not the correct color size",
				parser.lineNo(), parser.resourceName(),
				requestName(), "(o1 ... on)", RI_NULL);
			return;
		}

		int size = (int)request.size();
		std::vector<RtFloat> color;
		color.reserve(size);
		int i;
		RtFloat fval;
		for ( i = 0; i < size; ++i ) {
			if ( request[i].getFloat(fval) ) {
				color.push_back(fval);
			} else {
				parser.errHandler().handleError(
					RIE_CONSISTENCY, RIE_ERROR,
					"Line %ld, File \"%s\", badargument: '%s' parameter %d (opacity) is not numeric, using 1",
					request[i].lineNo(), parser.resourceName(),
					requestName(), i, RI_NULL);
				color.push_back(1);
			}
		}

		parser.ribFilter().opacity((RtColor)&color[0]);

	} else {

		parser.errHandler().handleError(
			RIE_MISSINGDATA, RIE_ERROR,
			"Line %ld, File \"%s\", badargument: '%s' arguments %s are missing", 
			parser.lineNo(), parser.resourceName(),
			requestName(), "(o1 ... on)", RI_NULL);

	}
}


void CSurfaceRibRequest::operator()(IRibParserState &parser, CRibRequestData &request) const
{
	// Surface name <paramlist>

	if ( request.size() >= 1 ) {

		CRibParameter &p0 = request[0];
		const char *name;

		if ( p0.getString(name) ) {

			CParameterClasses p;
			int n = request.getTokenList(1, p);
			if ( n > 0 ) {
				parser.ribFilter().surfaceV(name, n, request.tokenList(), request.valueList());
			} else {
				parser.ribFilter().surfaceV(name, 0, 0, 0);
			}

		} else {
			parser.errHandler().handleError(
				RIE_CONSISTENCY, RIE_ERROR,
				"Line %ld, File \"%s\", badargument: '%s' argument %s is not a string",
				p0.lineNo(), parser.resourceName(),
				requestName(), "1 (name)", RI_NULL);
		}

	} else {

		parser.errHandler().handleError(
			RIE_MISSINGDATA, RIE_ERROR,
			"Line %ld, File \"%s\", badargument: '%s' argument %s missing",
			parser.lineNo(), parser.resourceName(),
			requestName(), "(name)", RI_NULL);
	}
}


void CAtmosphereRibRequest::operator()(IRibParserState &parser, CRibRequestData &request) const
{
	// Atmosphere name <paramlist>

	if ( request.size() >= 1 ) {

		CRibParameter &p0 = request[0];
		const char *name;

		if ( p0.getString(name) ) {

			CParameterClasses p;
			int n = request.getTokenList(1, p);
			if ( n > 0 ) {
				parser.ribFilter().atmosphereV(name, n, request.tokenList(), request.valueList());
			} else {
				parser.ribFilter().atmosphereV(name, 0, 0, 0);
			}

		} else {
			parser.errHandler().handleError(
				RIE_CONSISTENCY, RIE_ERROR,
				"Line %ld, File \"%s\", badargument: '%s' argument %s is not a string",
				p0.lineNo(), parser.resourceName(),
				requestName(), "1 (name)", RI_NULL);
		}

	} else {

		parser.errHandler().handleError(
			RIE_MISSINGDATA, RIE_ERROR,
			"Line %ld, File \"%s\", badargument: '%s' argument %s missing",
			parser.lineNo(), parser.resourceName(),
			requestName(), "(name)", RI_NULL);
	}
}


void CInteriorRibRequest::operator()(IRibParserState &parser, CRibRequestData &request) const
{
	// Interior name <paramlist>

	if ( request.size() >= 1 ) {

		CRibParameter &p0 = request[0];
		const char *name;

		if ( p0.getString(name) ) {

			CParameterClasses p;
			int n = request.getTokenList(1, p);
			if ( n > 0 ) {
				parser.ribFilter().interiorV(name, n, request.tokenList(), request.valueList());
			} else {
				parser.ribFilter().interiorV(name, 0, 0, 0);
			}

		} else {
			parser.errHandler().handleError(
				RIE_CONSISTENCY, RIE_ERROR,
				"Line %ld, File \"%s\", badargument: '%s' argument %s is not a string",
				p0.lineNo(), parser.resourceName(),
				requestName(), "1 (name)", RI_NULL);
		}

	} else {

		parser.errHandler().handleError(
			RIE_MISSINGDATA, RIE_ERROR,
			"Line %ld, File \"%s\", badargument: '%s' argument %s missing",
			parser.lineNo(), parser.resourceName(),
			requestName(), "(name)", RI_NULL);
	}
}


void CExteriorRibRequest::operator()(IRibParserState &parser, CRibRequestData &request) const
{
	// Exterior name <paramlist>

	if ( request.size() >= 1 ) {

		CRibParameter &p0 = request[0];
		const char *name;

		if ( p0.getString(name) ) {

			CParameterClasses p;
			int n = request.getTokenList(1, p);
			if ( n > 0 ) {
				parser.ribFilter().exteriorV(name, n, request.tokenList(), request.valueList());
			} else {
				parser.ribFilter().exteriorV(name, 0, 0, 0);
			}

		} else {
			parser.errHandler().handleError(
				RIE_CONSISTENCY, RIE_ERROR,
				"Line %ld, File \"%s\", badargument: '%s' argument %s is not a string",
				p0.lineNo(), parser.resourceName(),
				requestName(), "1 (name)", RI_NULL);
		}

	} else {

		parser.errHandler().handleError(
			RIE_MISSINGDATA, RIE_ERROR,
			"Line %ld, File \"%s\", badargument: '%s' argument %s missing",
			parser.lineNo(), parser.resourceName(),
			requestName(), "(name)", RI_NULL);
	}
}


void CDisplacementRibRequest::operator()(IRibParserState &parser, CRibRequestData &request) const
{
	// Exterior name <paramlist>

	if ( request.size() >= 1 ) {

		CRibParameter &p0 = request[0];
		const char *name;

		if ( p0.getString(name) ) {

			CParameterClasses p;
			int n = request.getTokenList(1, p);
			if ( n > 0 ) {
				parser.ribFilter().displacementV(name, n, request.tokenList(), request.valueList());
			} else {
				parser.ribFilter().displacementV(name, 0, 0, 0);
			}

		} else {
			parser.errHandler().handleError(
				RIE_CONSISTENCY, RIE_ERROR,
				"Line %ld, File \"%s\", badargument: '%s' argument %s is not a string",
				p0.lineNo(), parser.resourceName(),
				requestName(), "1 (name)", RI_NULL);
		}

	} else {

		parser.errHandler().handleError(
			RIE_MISSINGDATA, RIE_ERROR,
			"Line %ld, File \"%s\", badargument: '%s' argument %s missing",
			parser.lineNo(), parser.resourceName(),
			requestName(), "(name)", RI_NULL);
	}
}


void CTextureCoordinatesRibRequest::operator()(IRibParserState &parser, CRibRequestData &request) const
{
	// TextureCoordinates [ s1 t1 s2 t2 s3 t3 s4 t4 ]
	// TextureCoordinates s1 t1 s2 t2 s3 t3 s4 t4

	if ( request.size() == 1 ) {

		CRibParameter &p0 = request[0];
		p0.convertIntToFloat();

		if ( p0.typeID() != BASICTYPE_FLOAT ) {
			parser.errHandler().handleError(
				RIE_CONSISTENCY, RIE_ERROR,
				"Line %ld, File \"%s\", badargument: '%s' argument %s is not numeric",
				p0.lineNo(), parser.resourceName(),
				requestName(), "1 [ s1 t1 s2 t2 s3 t3 s4 t4 ]", RI_NULL);
			return;
		}
		if ( !p0.isArray() || p0.getCard() < 8 ) {
			parser.errHandler().handleError(
				RIE_CONSISTENCY, RIE_ERROR,
				"Line %ld, File \"%s\", badargument: '%s' must have %s as arguments",
				p0.lineNo(), parser.resourceName(),
				requestName(), "(an array of) 8 floats", RI_NULL);
			return;
		}

		RtFloat *pv = (RtFloat *)p0.getValue();
		
		parser.ribFilter().textureCoordinates(pv[0], pv[1], pv[2], pv[3], pv[4], pv[5], pv[6], pv[7]);
		
		if ( p0.getCard() > 8 ) {
			parser.errHandler().handleError(
				RIE_CONSISTENCY, RIE_WARNING,
				"Line %ld, File \"%s\", badargument: '%s' has additional parameters, they are ignored",
				p0.lineNo(), parser.resourceName(),
				requestName(), RI_NULL);
		}

	} else if ( request.size() >= 8 ) {

		CRibParameter &p0 = request[0];
		CRibParameter &p1 = request[1];
		CRibParameter &p2 = request[2];
		CRibParameter &p3 = request[3];
		CRibParameter &p4 = request[4];
		CRibParameter &p5 = request[5];
		CRibParameter &p6 = request[6];
		CRibParameter &p7 = request[7];

		RtFloat s1, t1, s2, t2, s3, t3, s4, t4;

		bool b0 = p0.getFloat(s1),
		     b1 = p1.getFloat(t1),
			 b2 = p2.getFloat(s2),
			 b3 = p3.getFloat(t2),
			 b4 = p2.getFloat(s3),
			 b5 = p3.getFloat(t3),
			 b6 = p2.getFloat(s4),
			 b7 = p3.getFloat(t4);

		if ( b0 && b1 && b2 && b3 && b4 && b5 && b6 && b7 ) {

			parser.ribFilter().textureCoordinates(s1, t1, s2, t2, s3, t3, s4, t4);

		} else {
			if ( !b0 ) {
				parser.errHandler().handleError(
					RIE_CONSISTENCY, RIE_ERROR,
					"Line %ld, File \"%s\", badargument: '%s' argument %s is not numeric",
					p0.lineNo(), parser.resourceName(),
					requestName(), "1 (s1)", RI_NULL);
			}
			if ( !b1 ) {
				parser.errHandler().handleError(
					RIE_CONSISTENCY, RIE_ERROR,
					"Line %ld, File \"%s\", badargument: '%s' argument %s is not numeric",
					p1.lineNo(), parser.resourceName(),
					requestName(), "2 (t1)", RI_NULL);
			}
			if ( !b2 ) {
				parser.errHandler().handleError(
					RIE_CONSISTENCY, RIE_ERROR,
					"Line %ld, File \"%s\", badargument: '%s' argument %s is not numeric",
					p2.lineNo(), parser.resourceName(),
					requestName(), "3 (s2)", RI_NULL);
			}
			if ( !b3 ) {
				parser.errHandler().handleError(
					RIE_CONSISTENCY, RIE_ERROR,
					"Line %ld, File \"%s\", badargument: '%s' argument %s is not numeric",
					p3.lineNo(), parser.resourceName(),
					requestName(), "4 (t2)", RI_NULL);
			}
			if ( !b4 ) {
				parser.errHandler().handleError(
					RIE_CONSISTENCY, RIE_ERROR,
					"Line %ld, File \"%s\", badargument: '%s' argument %s is not numeric",
					p4.lineNo(), parser.resourceName(),
					requestName(), "5 (s3)", RI_NULL);
			}
			if ( !b5 ) {
				parser.errHandler().handleError(
					RIE_CONSISTENCY, RIE_ERROR,
					"Line %ld, File \"%s\", badargument: '%s' argument %s is not numeric",
					p4.lineNo(), parser.resourceName(),
					requestName(), "6 (t3)", RI_NULL);
			}
			if ( !b6 ) {
				parser.errHandler().handleError(
					RIE_CONSISTENCY, RIE_ERROR,
					"Line %ld, File \"%s\", badargument: '%s' argument %s is not numeric",
					p6.lineNo(), parser.resourceName(),
					requestName(), "7 (s4)", RI_NULL);
			}
			if ( !b7 ) {
				parser.errHandler().handleError(
					RIE_CONSISTENCY, RIE_ERROR,
					"Line %ld, File \"%s\", badargument: '%s' argument %s is not numeric",
					p7.lineNo(), parser.resourceName(),
					requestName(), "8 (t4)", RI_NULL);
			}
		}
		if ( request.size() > 8 ) {
			parser.errHandler().handleError(
				RIE_CONSISTENCY, RIE_WARNING,
				"Line %ld, File \"%s\", badargument: '%s' has additional parameters, they are ignored",
				parser.lineNo(), parser.resourceName(),
				requestName(), RI_NULL);
		}
	} else {
		parser.errHandler().handleError(
			RIE_MISSINGDATA, RIE_ERROR,
			"Line %ld, File \"%s\", badargument: '%s' arguments %s missing",
			parser.lineNo(), parser.resourceName(),
			requestName(), "(s1 t1 s2 t2 s3 t3 s4 t4)", RI_NULL);
	}
}


void CShadingRateRibRequest::operator()(IRibParserState &parser, CRibRequestData &request) const
{
	// ShadingRate size

	if ( request.size() >= 1 ) {

		CRibParameter &p0 = request[0];
		RtFloat size;

		if ( p0.getFloat(size) ) {

			parser.ribFilter().shadingRate(size);

		} else {
			parser.errHandler().handleError(
				RIE_CONSISTENCY, RIE_ERROR,
				"Line %ld, File \"%s\", badargument: '%s' argument %s is not numeric",
				p0.lineNo(), parser.resourceName(),
				requestName(), "1 (size)", RI_NULL);
		}
		if ( request.size() > 1 ) {
			parser.errHandler().handleError(
				RIE_CONSISTENCY, RIE_WARNING,
				"Line %ld, File \"%s\", badargument: '%s' has additional parameters, they are ignored",
				parser.lineNo(), parser.resourceName(),
				requestName(), RI_NULL);
		}

	} else {

		parser.errHandler().handleError(
			RIE_MISSINGDATA, RIE_ERROR,
			"Line %ld, File \"%s\", badargument: '%s' argument %s missing",
			parser.lineNo(), parser.resourceName(),
			requestName(), "(size)", RI_NULL);
	}
}


void CShadingInterpolationRibRequest::operator()(IRibParserState &parser, CRibRequestData &request) const
{
	// ShadingInterpolation type

	if ( request.size() >= 1 ) {

		CRibParameter &p0 = request[0];
		const char *type;

		if ( p0.getString(type) ) {

			parser.ribFilter().shadingInterpolation(type);

		} else {
			parser.errHandler().handleError(
				RIE_CONSISTENCY, RIE_ERROR,
				"Line %ld, File \"%s\", badargument: '%s' argument %s is not numeric",
				p0.lineNo(), parser.resourceName(),
				requestName(), "1 (type)", RI_NULL);
		}
		if ( request.size() > 1 ) {
			parser.errHandler().handleError(
				RIE_CONSISTENCY, RIE_WARNING,
				"Line %ld, File \"%s\", badargument: '%s' has additional parameters, they are ignored",
				parser.lineNo(), parser.resourceName(),
				requestName(), RI_NULL);
		}

	} else {

		parser.errHandler().handleError(
			RIE_MISSINGDATA, RIE_ERROR,
			"Line %ld, File \"%s\", badargument: '%s' argument %s missing",
			parser.lineNo(), parser.resourceName(),
			requestName(), "(type)", RI_NULL);
	}
}


void CMatteRibRequest::operator()(IRibParserState &parser, CRibRequestData &request) const
{
	// Matte onoff

	if ( request.size() >= 1 ) {

		CRibParameter &p0 = request[0];
		RtInt onoff;

		if ( p0.getInt(onoff) ) {

			parser.ribFilter().matte(onoff);

		} else {
			parser.errHandler().handleError(
				RIE_CONSISTENCY, RIE_ERROR,
				"Line %ld, File \"%s\", badargument: '%s' argument %s is not numeric",
				p0.lineNo(), parser.resourceName(),
				requestName(), "1 (onoff)", RI_NULL);
		}
		if ( request.size() > 1 ) {
			parser.errHandler().handleError(
				RIE_CONSISTENCY, RIE_WARNING,
				"Line %ld, File \"%s\", badargument: '%s' has additional parameters, they are ignored",
				parser.lineNo(), parser.resourceName(),
				requestName(), RI_NULL);
		}

	} else {

		parser.errHandler().handleError(
			RIE_MISSINGDATA, RIE_ERROR,
			"Line %ld, File \"%s\", badargument: '%s' argument %s missing",
			parser.lineNo(), parser.resourceName(),
			requestName(), "(onoff)", RI_NULL);
	}
}


void CBoundRibRequest::operator()(IRibParserState &parser, CRibRequestData &request) const
{
	// Bound [ xmin xmax ymin ymax zmin zmax ]
	// Bound xmin xmax ymin ymax zmin zmax

	if ( request.size() == 1 ) {

		CRibParameter &p0 = request[0];
		p0.convertIntToFloat();

		if ( p0.typeID() != BASICTYPE_FLOAT ) {
			parser.errHandler().handleError(
				RIE_CONSISTENCY, RIE_ERROR,
				"Line %ld, File \"%s\", badargument: '%s' argument %s is not numeric",
				p0.lineNo(), parser.resourceName(),
				requestName(), "1 [ xmin xmax ymin ymax zmin zmax ]", RI_NULL);
			return;
		}
		if ( !p0.isArray() || p0.getCard() < 6 ) {
			parser.errHandler().handleError(
				RIE_CONSISTENCY, RIE_ERROR,
				"Line %ld, File \"%s\", badargument: '%s' must have %s as arguments",
				p0.lineNo(), parser.resourceName(),
				requestName(), "(an array of) 6 floats", RI_NULL);
			return;
		}

		parser.ribFilter().bound((RtFloat *)p0.getValue());
		
		if ( p0.getCard() > 6 ) {
			parser.errHandler().handleError(
				RIE_CONSISTENCY, RIE_WARNING,
				"Line %ld, File \"%s\", badargument: '%s' has additional parameters, they are ignored",
				p0.lineNo(), parser.resourceName(),
				requestName(), RI_NULL);
		}

	} else if ( request.size() >= 8 ) {

		CRibParameter &p0 = request[0];
		CRibParameter &p1 = request[1];
		CRibParameter &p2 = request[2];
		CRibParameter &p3 = request[3];
		CRibParameter &p4 = request[4];
		CRibParameter &p5 = request[5];

		RtBound bound;

		bool b0 = p0.getFloat(bound[0]),
		     b1 = p1.getFloat(bound[1]),
			 b2 = p2.getFloat(bound[2]),
			 b3 = p3.getFloat(bound[3]),
			 b4 = p2.getFloat(bound[4]),
			 b5 = p3.getFloat(bound[5]);

		if ( b0 && b1 && b2 && b3 && b4 && b5 ) {

			parser.ribFilter().bound(bound);

		} else {
			if ( !b0 ) {
				parser.errHandler().handleError(
					RIE_CONSISTENCY, RIE_ERROR,
					"Line %ld, File \"%s\", badargument: '%s' argument %s is not numeric",
					p0.lineNo(), parser.resourceName(),
					requestName(), "1 (xmin)", RI_NULL);
			}
			if ( !b1 ) {
				parser.errHandler().handleError(
					RIE_CONSISTENCY, RIE_ERROR,
					"Line %ld, File \"%s\", badargument: '%s' argument %s is not numeric",
					p1.lineNo(), parser.resourceName(),
					requestName(), "2 (xmax)", RI_NULL);
			}
			if ( !b2 ) {
				parser.errHandler().handleError(
					RIE_CONSISTENCY, RIE_ERROR,
					"Line %ld, File \"%s\", badargument: '%s' argument %s is not numeric",
					p2.lineNo(), parser.resourceName(),
					requestName(), "3 (ymin)", RI_NULL);
			}
			if ( !b3 ) {
				parser.errHandler().handleError(
					RIE_CONSISTENCY, RIE_ERROR,
					"Line %ld, File \"%s\", badargument: '%s' argument %s is not numeric",
					p3.lineNo(), parser.resourceName(),
					requestName(), "4 (ymax)", RI_NULL);
			}
			if ( !b4 ) {
				parser.errHandler().handleError(
					RIE_CONSISTENCY, RIE_ERROR,
					"Line %ld, File \"%s\", badargument: '%s' argument %s is not numeric",
					p4.lineNo(), parser.resourceName(),
					requestName(), "5 (zmin)", RI_NULL);
			}
			if ( !b5 ) {
				parser.errHandler().handleError(
					RIE_CONSISTENCY, RIE_ERROR,
					"Line %ld, File \"%s\", badargument: '%s' argument %s is not numeric",
					p4.lineNo(), parser.resourceName(),
					requestName(), "6 (zmax)", RI_NULL);
			}
		}
		if ( request.size() > 6 ) {
			parser.errHandler().handleError(
				RIE_CONSISTENCY, RIE_WARNING,
				"Line %ld, File \"%s\", badargument: '%s' has additional parameters, they are ignored",
				parser.lineNo(), parser.resourceName(),
				requestName(), RI_NULL);
		}
	} else {
		parser.errHandler().handleError(
			RIE_MISSINGDATA, RIE_ERROR,
			"Line %ld, File \"%s\", badargument: '%s' arguments %s missing",
			parser.lineNo(), parser.resourceName(),
			requestName(), "(xmin xmax ymin ymax zmin zmax)", RI_NULL);
	}
}


void CDetailRibRequest::operator()(IRibParserState &parser, CRibRequestData &request) const
{
	// Detail [ xmin xmax ymin ymax zmin zmax ]
	// Detail xmin xmax ymin ymax zmin zmax

	if ( request.size() == 1 ) {

		CRibParameter &p0 = request[0];
		p0.convertIntToFloat();

		if ( p0.typeID() != BASICTYPE_FLOAT ) {
			parser.errHandler().handleError(
				RIE_CONSISTENCY, RIE_ERROR,
				"Line %ld, File \"%s\", badargument: '%s' argument %s is not numeric",
				p0.lineNo(), parser.resourceName(),
				requestName(), "1 [ xmin xmax ymin ymax zmin zmax ]", RI_NULL);
			return;
		}
		if ( !p0.isArray() || p0.getCard() < 6 ) {
			parser.errHandler().handleError(
				RIE_CONSISTENCY, RIE_ERROR,
				"Line %ld, File \"%s\", badargument: '%s' must have %s as arguments",
				p0.lineNo(), parser.resourceName(),
				requestName(), "(an array of) 6 floats", RI_NULL);
			return;
		}

		parser.ribFilter().detail((RtFloat *)p0.getValue());
		
		if ( p0.getCard() > 6 ) {
			parser.errHandler().handleError(
				RIE_CONSISTENCY, RIE_WARNING,
				"Line %ld, File \"%s\", badargument: '%s' has additional parameters, they are ignored",
				p0.lineNo(), parser.resourceName(),
				requestName(), RI_NULL);
		}

	} else if ( request.size() >= 8 ) {

		CRibParameter &p0 = request[0];
		CRibParameter &p1 = request[1];
		CRibParameter &p2 = request[2];
		CRibParameter &p3 = request[3];
		CRibParameter &p4 = request[4];
		CRibParameter &p5 = request[5];

		RtBound bound;

		bool b0 = p0.getFloat(bound[0]),
		     b1 = p1.getFloat(bound[1]),
			 b2 = p2.getFloat(bound[2]),
			 b3 = p3.getFloat(bound[3]),
			 b4 = p2.getFloat(bound[4]),
			 b5 = p3.getFloat(bound[5]);

		if ( b0 && b1 && b2 && b3 && b4 && b5 ) {

			parser.ribFilter().detail(bound);

		} else {
			if ( !b0 ) {
				parser.errHandler().handleError(
					RIE_CONSISTENCY, RIE_ERROR,
					"Line %ld, File \"%s\", badargument: '%s' argument %s is not numeric",
					p0.lineNo(), parser.resourceName(),
					requestName(), "1 (xmin)", RI_NULL);
			}
			if ( !b1 ) {
				parser.errHandler().handleError(
					RIE_CONSISTENCY, RIE_ERROR,
					"Line %ld, File \"%s\", badargument: '%s' argument %s is not numeric",
					p1.lineNo(), parser.resourceName(),
					requestName(), "2 (xmax)", RI_NULL);
			}
			if ( !b2 ) {
				parser.errHandler().handleError(
					RIE_CONSISTENCY, RIE_ERROR,
					"Line %ld, File \"%s\", badargument: '%s' argument %s is not numeric",
					p2.lineNo(), parser.resourceName(),
					requestName(), "3 (ymin)", RI_NULL);
			}
			if ( !b3 ) {
				parser.errHandler().handleError(
					RIE_CONSISTENCY, RIE_ERROR,
					"Line %ld, File \"%s\", badargument: '%s' argument %s is not numeric",
					p3.lineNo(), parser.resourceName(),
					requestName(), "4 (ymax)", RI_NULL);
			}
			if ( !b4 ) {
				parser.errHandler().handleError(
					RIE_CONSISTENCY, RIE_ERROR,
					"Line %ld, File \"%s\", badargument: '%s' argument %s is not numeric",
					p4.lineNo(), parser.resourceName(),
					requestName(), "5 (zmin)", RI_NULL);
			}
			if ( !b5 ) {
				parser.errHandler().handleError(
					RIE_CONSISTENCY, RIE_ERROR,
					"Line %ld, File \"%s\", badargument: '%s' argument %s is not numeric",
					p4.lineNo(), parser.resourceName(),
					requestName(), "6 (zmax)", RI_NULL);
			}
		}
		if ( request.size() > 6 ) {
			parser.errHandler().handleError(
				RIE_CONSISTENCY, RIE_WARNING,
				"Line %ld, File \"%s\", badargument: '%s' has additional parameters, they are ignored",
				parser.lineNo(), parser.resourceName(),
				requestName(), RI_NULL);
		}
	} else {
		parser.errHandler().handleError(
			RIE_MISSINGDATA, RIE_ERROR,
			"Line %ld, File \"%s\", badargument: '%s' arguments %s missing",
			parser.lineNo(), parser.resourceName(),
			requestName(), "(xmin xmax ymin ymax zmin zmax)", RI_NULL);
	}
}


void CDetailRangeRibRequest::operator()(IRibParserState &parser, CRibRequestData &request) const
{
	// DetailRange [ minvisible lowertransition uppertransition maxvisible ]
	// DetailRange minvisible lowertransition uppertransition maxvisible

	if ( request.size() == 1 ) {

		CRibParameter &p0 = request[0];
		p0.convertIntToFloat();

		if ( p0.typeID() != BASICTYPE_FLOAT ) {
			parser.errHandler().handleError(
				RIE_CONSISTENCY, RIE_ERROR,
				"Line %ld, File \"%s\", badargument: '%s' argument %s is not numeric",
				p0.lineNo(), parser.resourceName(),
				requestName(), "1 [ minvisible lowertransition uppertransition maxvisible ]", RI_NULL);
			return;
		}
		if ( !p0.isArray() || p0.getCard() < 4 ) {
			parser.errHandler().handleError(
				RIE_CONSISTENCY, RIE_ERROR,
				"Line %ld, File \"%s\", badargument: '%s' must have %s as arguments",
				p0.lineNo(), parser.resourceName(),
				requestName(), "(an array of) 4 floats", RI_NULL);
			return;
		}

		RtFloat *pv = (RtFloat *)p0.getValue();
		parser.ribFilter().detailRange(pv[0], pv[1], pv[2], pv[3]);
		
		if ( p0.getCard() > 4 ) {
			parser.errHandler().handleError(
				RIE_CONSISTENCY, RIE_WARNING,
				"Line %ld, File \"%s\", badargument: '%s' has additional parameters, they are ignored",
				p0.lineNo(), parser.resourceName(),
				requestName(), RI_NULL);
		}

	} else if ( request.size() >= 4 ) {

		CRibParameter &p0 = request[0];
		CRibParameter &p1 = request[1];
		CRibParameter &p2 = request[2];
		CRibParameter &p3 = request[3];

		RtFloat minvisible, lowertransition, uppertransition, maxvisible;

		bool b0 = p0.getFloat(minvisible),
		     b1 = p1.getFloat(lowertransition),
			 b2 = p2.getFloat(uppertransition),
			 b3 = p3.getFloat(maxvisible);

		if ( b0 && b1 && b2 && b3 ) {

			parser.ribFilter().detailRange(minvisible, lowertransition, uppertransition, maxvisible);

		} else {
			if ( !b0 ) {
				parser.errHandler().handleError(
					RIE_CONSISTENCY, RIE_ERROR,
					"Line %ld, File \"%s\", badargument: '%s' argument %s is not numeric",
					p0.lineNo(), parser.resourceName(),
					requestName(), "1 (minvisible)", RI_NULL);
			}
			if ( !b1 ) {
				parser.errHandler().handleError(
					RIE_CONSISTENCY, RIE_ERROR,
					"Line %ld, File \"%s\", badargument: '%s' argument %s is not numeric",
					p1.lineNo(), parser.resourceName(),
					requestName(), "2 (lowertransition)", RI_NULL);
			}
			if ( !b2 ) {
				parser.errHandler().handleError(
					RIE_CONSISTENCY, RIE_ERROR,
					"Line %ld, File \"%s\", badargument: '%s' argument %s is not numeric",
					p2.lineNo(), parser.resourceName(),
					requestName(), "3 (uppertransition)", RI_NULL);
			}
			if ( !b3 ) {
				parser.errHandler().handleError(
					RIE_CONSISTENCY, RIE_ERROR,
					"Line %ld, File \"%s\", badargument: '%s' argument %s is not numeric",
					p3.lineNo(), parser.resourceName(),
					requestName(), "4 (maxvisible)", RI_NULL);
			}
		}
		if ( request.size() > 4 ) {
			parser.errHandler().handleError(
				RIE_CONSISTENCY, RIE_WARNING,
				"Line %ld, File \"%s\", badargument: '%s' has additional parameters, they are ignored",
				parser.lineNo(), parser.resourceName(),
				requestName(), RI_NULL);
		}
	} else {
		parser.errHandler().handleError(
			RIE_MISSINGDATA, RIE_ERROR,
			"Line %ld, File \"%s\", badargument: '%s' arguments %s missing",
			parser.lineNo(), parser.resourceName(),
			requestName(), "(minvisible lowertransition uppertransition maxvisible)", RI_NULL);
	}
}


void CGeometricApproximationRibRequest::operator()(IRibParserState &parser, CRibRequestData &request) const
{
	// GeometricApproximation type value

	if ( request.size() >= 2 ) {

		CRibParameter &p0 = request[0];
		CRibParameter &p1 = request[1];

		RtString type;
		RtFloat value;

		bool b0 = p0.getString(type), b1 = p1.getFloat(value);

		if ( b0 && b1 ) {

			parser.ribFilter().geometricApproximation(type, value);

		} else {
			if ( !b0 ) {
				parser.errHandler().handleError(
					RIE_CONSISTENCY, RIE_ERROR,
					"Line %ld, File \"%s\", badargument: '%s' argument %s is not a string",
					p0.lineNo(), parser.resourceName(),
					requestName(), "1 (type)", RI_NULL);
			}
			if ( !b1 ) {
				parser.errHandler().handleError(
					RIE_CONSISTENCY, RIE_ERROR,
					"Line %ld, File \"%s\", badargument: '%s' argument %s is not numeric",
					p1.lineNo(), parser.resourceName(),
					requestName(), "2 (value)", RI_NULL);
			}
		}
		if ( request.size() > 2 ) {
			parser.errHandler().handleError(
				RIE_CONSISTENCY, RIE_WARNING,
				"Line %ld, File \"%s\", badargument: '%s' has additional parameters, they are ignored",
				parser.lineNo(), parser.resourceName(),
				requestName(), RI_NULL);
		}

	} else {

		parser.errHandler().handleError(
			RIE_MISSINGDATA, RIE_ERROR,
			"Line %ld, File \"%s\", badargument: '%s' argument %s missing",
			parser.lineNo(), parser.resourceName(),
			requestName(), "(type value)", RI_NULL);
	}
}


void CGeometricRepresentationRibRequest::operator()(IRibParserState &parser, CRibRequestData &request) const
{
	// GeometricRepresentation type

	if ( request.size() >= 1 ) {

		CRibParameter &p0 = request[0];
		const char *type;

		if ( p0.getString(type) ) {

			parser.ribFilter().geometricRepresentation(type);

		} else {
			parser.errHandler().handleError(
				RIE_CONSISTENCY, RIE_ERROR,
				"Line %ld, File \"%s\", badargument: '%s' argument %s is not numeric",
				p0.lineNo(), parser.resourceName(),
				requestName(), "1 (type)", RI_NULL);
		}
		if ( request.size() > 1 ) {
			parser.errHandler().handleError(
				RIE_CONSISTENCY, RIE_WARNING,
				"Line %ld, File \"%s\", badargument: '%s' has additional parameters, they are ignored",
				parser.lineNo(), parser.resourceName(),
				requestName(), RI_NULL);
		}

	} else {

		parser.errHandler().handleError(
			RIE_MISSINGDATA, RIE_ERROR,
			"Line %ld, File \"%s\", badargument: '%s' argument %s missing",
			parser.lineNo(), parser.resourceName(),
			requestName(), "(type)", RI_NULL);
	}
}


void COrientationRibRequest::operator()(IRibParserState &parser, CRibRequestData &request) const
{
	// Orientation orientation

	if ( request.size() >= 1 ) {

		CRibParameter &p0 = request[0];
		const char *orientation;

		if ( p0.getString(orientation) ) {

			parser.ribFilter().orientation(orientation);

		} else {
			parser.errHandler().handleError(
				RIE_CONSISTENCY, RIE_ERROR,
				"Line %ld, File \"%s\", badargument: '%s' argument %s is not numeric",
				p0.lineNo(), parser.resourceName(),
				requestName(), "1 (orientation)", RI_NULL);
		}
		if ( request.size() > 1 ) {
			parser.errHandler().handleError(
				RIE_CONSISTENCY, RIE_WARNING,
				"Line %ld, File \"%s\", badargument: '%s' has additional parameters, they are ignored",
				parser.lineNo(), parser.resourceName(),
				requestName(), RI_NULL);
		}

	} else {

		parser.errHandler().handleError(
			RIE_MISSINGDATA, RIE_ERROR,
			"Line %ld, File \"%s\", badargument: '%s' argument %s missing",
			parser.lineNo(), parser.resourceName(),
			requestName(), "(type)", RI_NULL);
	}
}

void CReverseOrientationRibRequest::operator()(IRibParserState &parser, CRibRequestData &request) const
{
	// ReverseOrientation
	if ( request.size() != 0 ) {
		parser.errHandler().handleError(
			RIE_CONSISTENCY, RIE_WARNING,
			"Line %ld, File \"%s\", badargument: '%s' has additional parameters, they are ignored",
			parser.lineNo(), parser.resourceName(), requestName(), RI_NULL);
	}
	parser.ribFilter().reverseOrientation();
}


void CSidesRibRequest::operator()(IRibParserState &parser, CRibRequestData &request) const
{
	// Sides sides

	if ( request.size() >= 1 ) {

		CRibParameter &p0 = request[0];
		RtInt sides;

		if ( p0.getInt(sides) ) {

			parser.ribFilter().sides(sides);

		} else {
			parser.errHandler().handleError(
				RIE_CONSISTENCY, RIE_ERROR,
				"Line %ld, File \"%s\", badargument: '%s' argument %s is not numeric",
				p0.lineNo(), parser.resourceName(),
				requestName(), "1 (sides)", RI_NULL);
		}
		if ( request.size() > 1 ) {
			parser.errHandler().handleError(
				RIE_CONSISTENCY, RIE_WARNING,
				"Line %ld, File \"%s\", badargument: '%s' has additional parameters, they are ignored",
				parser.lineNo(), parser.resourceName(),
				requestName(), RI_NULL);
		}

	} else {

		parser.errHandler().handleError(
			RIE_MISSINGDATA, RIE_ERROR,
			"Line %ld, File \"%s\", badargument: '%s' argument %s missing",
			parser.lineNo(), parser.resourceName(),
			requestName(), "(onoff)", RI_NULL);
	}
}
