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
	// Displacement name <paramlist>

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
			 b4 = p4.getFloat(s3),
			 b5 = p5.getFloat(t3),
			 b6 = p6.getFloat(s4),
			 b7 = p7.getFloat(t4);

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
				"Line %ld, File \"%s\", badargument: '%s' argument %s is not a string",
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

	} else if ( request.size() >= 6 ) {

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
			 b4 = p4.getFloat(bound[4]),
			 b5 = p5.getFloat(bound[5]);

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

	} else if ( request.size() >= 6 ) {

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
			 b4 = p4.getFloat(bound[4]),
			 b5 = p5.getFloat(bound[5]);

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
				"Line %ld, File \"%s\", badargument: '%s' argument %s is not a string",
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
				"Line %ld, File \"%s\", badargument: '%s' argument %s is not a string",
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
			requestName(), "(orientation)", RI_NULL);
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


void CBasisRibRequest::operator()(IRibParserState &parser, CRibRequestData &request) const
{
	// Basis uname  ustep vname  vstep
	// Basis uname  ustep vbasis vstep
	// Basis ubasis ustep vname  vstep
	// Basis ubasis ustep vbasis vstep

	if ( request.size() >= 4 ) {

		CRibParameter &p0 = request[0];
		CRibParameter &p1 = request[1];
		CRibParameter &p2 = request[2];
		CRibParameter &p3 = request[3];

		RtString uname = 0, vname = 0;
		RtBasis ubasis, vbasis;
		RtInt ustep, vstep;

		p0.convertIntToFloat();
		p2.convertIntToFloat();

		bool b1 = p1.getInt(ustep), b3 = p3.getInt(vstep);

		if ( b1 && b3 ) {

			if ( p0.typeID() == BASICTYPE_FLOAT && p0.isArray() && p0.getCard() == 16 ) {
				memcpy(ubasis[0], p0.getValue(), sizeof(RtBasis));
			} else if ( p0.getString(uname) ) {
				RtToken name = parser.renderState().tokFind(uname);
				if ( name == RI_CATMULL_ROM )
					name = RI_CATMULL_ROM;
				if ( !name || !parser.renderState().getBasis(name, ubasis) ) {
					parser.errHandler().handleError(
						RIE_CONSISTENCY, RIE_WARNING,
						"Line %ld, File \"%s\", badarray: '%s' argument 1 ubasis '%s' is not a valid basis",
						p0.lineNo(), parser.resourceName(),
						requestName(), uname, RI_NULL);
					return;
				}
			} else {
				parser.errHandler().handleError(
					RIE_CONSISTENCY, RIE_WARNING,
					"Line %ld, File \"%s\", badarray: '%s' argument 1 ubasis is not numeric/string or wrong number of elements",
					p0.lineNo(), parser.resourceName(),
					requestName(), RI_NULL);
				return;
			}

			if ( p2.typeID() == BASICTYPE_FLOAT && p2.isArray() && p2.getCard() == 16 ) {
				memcpy(vbasis, p2.getValue(), sizeof(RtBasis));
			} else if ( p2.getString(vname) ) {
				RtToken name = parser.renderState().tokFind(vname);
				if ( name == RI_CATMULL_ROM )
					name = RI_CATMULL_ROM;
				if ( !name || !parser.renderState().getBasis(name, vbasis) ) {
					parser.errHandler().handleError(
						RIE_CONSISTENCY, RIE_WARNING,
						"Line %ld, File \"%s\", badarray: '%s' argument 1 vbasis '%s' is not a valid basis",
						p2.lineNo(), parser.resourceName(),
						requestName(), uname, RI_NULL);
					return;
				}
			} else {
				parser.errHandler().handleError(
					RIE_CONSISTENCY, RIE_WARNING,
					"Line %ld, File \"%s\", badarray: '%s' argument 1 vbasis is not numeric/string or wrong number of elements",
					p2.lineNo(), parser.resourceName(),
					requestName(), RI_NULL);
				return;
			}

			parser.ribFilter().basis(ubasis, ustep, vbasis, vstep);

		} else {
			if ( !b1 ) {
				parser.errHandler().handleError(
					RIE_CONSISTENCY, RIE_ERROR,
					"Line %ld, File \"%s\", badargument: '%s' argument %s is not a string",
					p1.lineNo(), parser.resourceName(),
					requestName(), "2 (ustep)", RI_NULL);
			}
			if ( !b3 ) {
				parser.errHandler().handleError(
					RIE_CONSISTENCY, RIE_ERROR,
					"Line %ld, File \"%s\", badargument: '%s' argument %s is not numeric",
					p3.lineNo(), parser.resourceName(),
					requestName(), "4 (vstep)", RI_NULL);
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
			"Line %ld, File \"%s\", badargument: '%s' argument %s missing",
			parser.lineNo(), parser.resourceName(),
			requestName(), "(ubasis ustep vbasis vstep)", RI_NULL);
	}
}


void CTrimCurveRibRequest::operator()(IRibParserState &parser, CRibRequestData &request) const
{
	// TrimCurve [ ncurves ] [ order ] [ knot ] [ min ] [ max ] [ n ] [ u ] [ v ] [ w ]
	// TrimCurve nloops [ ncurves ] [ order ] [ knot ] [ min ] [ max ] [ n ] [ u ] [ v ] [ w ]
	//           with nloops == card(ncurves)
	// Special cases recognized to switch off trim curves:
	// TrimCurve 0
	// TrimCurve [ 0 ]
	// TrimCurve [ 0 ] [] [] [] [] [] [] [] []
	// TrimCurve [] [] [] [] [] [] [] [] []
	/*
	Example:

	nloops           - 2 (elems of ncurves[])
	TrimCurve  ncurves[nloops]  - [ 1 1 ]
	order[sumcurves] - [ 4 4 ]
	knot[]           - [ 0 0 0 0 1 1 1 2 2 2 3 3 3 4 4 4 5 5 5 6 6 6 7 7 7 8 8 8 9 9 9 10 10 10 11 11 11 12 12 12 13 13 13 14 14 14 15 15 15 16 16 16 16 0 0 0 0 1 1 1 2 2 2 3 3 3 4 4 4 5 5 5 5 ]
	amin[]           - [ 0 0 ]
	amax[]           - [ 16 5 ]
	n[sumcurves]     - [ 49 16 ]
	u[]              - [ 0.216594 0.228702 0.257058 0.459505 0.627541 0.70847 0.70847 0.70847 0.657874 0.61537 0.517633 0.419601 0.321863 0.026317 0 0 0 0.137641 0.331995 0.538457 0.645765 0.714586 0.72064 0.744919 0.902823 0.94335 0.969667 1 1 1 1 0.979737 0.957497 0.941311 0.904862 0.88256 0.88256 0.882561 0.88256 0.88256 0.88256 0.574906 0.489899 0.228702 0.060728 0.052635 0.107234 0.161995 0.216594 0.704454 0.704454 0.542472 0.376475 0.242911 0.184222 0.184222 0.184222 0.323902 0.410947 0.631556 0.676098 0.704454 0.704455 0.704454 0.704454 ]
	v[]              - [ 0.314607 0.242316 0.132002 0.132002 0.132002 0.186248 0.285691 0.379725 0.394184 0.397826 0.408669 0.408668 0.430385 0.462888 0.64737 0.726947 0.889686 1 1 1 0.913195 0.846256 0.918603 0.99095 0.99095 0.99095 0.980078 0.972849 0.934315 0.934314 0.857127 0.860714 0.864356 0.864356 0.864356 0.848077 0.808289 0.627043 0.627042 0.264003 0.021688 0 0 0 0.088571 0.314607 0.314608 0.314607 0.314607 0.645549 0.772143 0.867943 0.867943 0.867943 0.806468 0.703438 0.584074 0.560565 0.549694 0.524419 0.509906 0.490039 0.541825 0.541824 0.645549 ]
	w[]              - [ 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 ]
	*/
	if ( request.size() == 1 ) {
		// assume nloops == 0, disable trim curve
		RtInt nloops = 0;
		CRibParameter &p0 = request[0]; // nloops
		if ( !p0.getInt(nloops) ) {
			parser.errHandler().handleError(
				RIE_MISSINGDATA, RIE_ERROR,
				"Line %ld, File \"%s\", badargument: '%s' argument (nloops) has to be a 0 if there is only one argument, trim curve is disabled",
				parser.lineNo(), parser.resourceName(),
				requestName(), RI_NULL);
		} else if ( nloops != 0 ) {
			parser.errHandler().handleError(
				RIE_MISSINGDATA, RIE_ERROR,
				"Line %ld, File \"%s\", badargument: '%s' argument (nloops) has to be a 0 if there is only one argument, trim curve is disabled",
				parser.lineNo(), parser.resourceName(),
				requestName(), RI_NULL);
		}
		// disable trim curve
		parser.ribFilter().trimCurve(0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
		return;
	}
	
	if ( request.size() >= 9 ) {
		int cnt=0;
		RtInt nloops=0;
		bool nloopsSet = false;

		CRibParameter &p = request[cnt]; // nloops (?)
		if ( !p.isArray() && request.size() >= 10 ) {
			cnt++;
			if ( !p.getInt(nloops) ) {
				parser.errHandler().handleError(
					RIE_MISSINGDATA, RIE_WARNING,
					"Line %ld, File \"%s\", badargument: '%s' optional argument 1 (nloops) has to be an integer and has to be the size of [ncurves]",
					p.lineNo(), parser.resourceName(),
					requestName(), RI_NULL);
			} else {
				nloopsSet = true;
			}
		}

		CRibParameter &p0 = request[cnt++]; // ncurves
		CRibParameter &p1 = request[cnt++]; // order
		CRibParameter &p2 = request[cnt++]; // knot
		CRibParameter &p3 = request[cnt++]; // amin
		CRibParameter &p4 = request[cnt++]; // amax
		CRibParameter &p5 = request[cnt++]; // n
		CRibParameter &p6 = request[cnt++]; // u
		CRibParameter &p7 = request[cnt++]; // v
		CRibParameter &p8 = request[cnt++]; // w

		p0.convertIntToFloat();
		p1.convertIntToFloat();
		p2.convertIntToFloat();
		p3.convertIntToFloat();
		p4.convertIntToFloat();
		p5.convertIntToFloat();
		p6.convertIntToFloat();
		p7.convertIntToFloat();
		p8.convertIntToFloat();

		int i = 0;
		RtInt sumcurves=0, *ncurves=0, *order=0, *n=0;
		RtFloat *knot=0, *amin=0, *amax=0, *u=0, *v=0, *w=0;
		RtFloat *pncurves = 0, *porder = 0, *pn = 0;

		// The number of loops is maybe empty
		if ( p0.typeID() != BASICTYPE_UNKNOWN ) {
			if ( p0.typeID() == BASICTYPE_FLOAT && p0.isArray() ) {
				RtInt nloops2 = (RtInt)p0.getCard();
				if ( nloopsSet ) {
					if ( nloops != nloops2 ) {
						parser.errHandler().handleError(
							RIE_MISSINGDATA, RIE_WARNING,
							"Line %ld, File \"%s\", badargument: '%s' optional argument 1 (nloops) has to be an integer and has to be the size of [ncurves]",
							parser.lineNo(), parser.resourceName(),
							requestName(), RI_NULL);
					}
				}
				nloops = nloops2;
				nloopsSet = true;
				pncurves = (RtFloat *)p0.getValue();
			} else {
				parser.errHandler().handleError(
					RIE_MISSINGDATA, RIE_ERROR,
					"Line %ld, File \"%s\", badarray: '%s' argument %s is not an array of numeric values, trim curve is disabled",
					parser.lineNo(), parser.resourceName(),
					requestName(), "1 (ncurves)", RI_NULL);
			}
		}

		if ( nloops == 0 || pncurves == 0 || (nloops == 1 && pncurves[0] == (RtFloat)0) ) {
			// Disables trim curves, does not interpret the rest
			nloops = 0;
			parser.ribFilter().trimCurve(0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
		} else {

			// The number of curves per loop
			RtInt i_ncurves;
			std::vector<RtInt> ncurvesVect;
			ncurvesVect.reserve(nloops);
			sumcurves = 0;
			for ( i = 0; i < nloops; ++i ) {
				i_ncurves = (RtInt)pncurves[i];
				ncurvesVect.push_back(i_ncurves);
				sumcurves += i_ncurves;
			}
			if ( (int)ncurvesVect.size() != nloops ) {
				// not all values are defined
				parser.errHandler().handleError(
					RIE_MISSINGDATA, RIE_ERROR,
					"Line %ld, File \"%s\", badargument: '%s' argument %s not all elements could get copied",
					p0.lineNo(), parser.resourceName(),
					requestName(), "1 (ncurves)", RI_NULL);
				return;
			}
			ncurves = &ncurvesVect[0];

			// order vector
			if ( p1.typeID() == BASICTYPE_FLOAT && p1.isArray() ) {
				porder = (RtFloat *)p1.getValue();
			} else {
				parser.errHandler().handleError(
					RIE_MISSINGDATA, RIE_ERROR,
					"Line %ld, File \"%s\", badargument: '%s' argument %s is not an array of numeric values",
					p1.lineNo(), parser.resourceName(),
					requestName(), "2 (order)", RI_NULL);
			}
			if ( porder == NULL ) {
				return;
			}

			RtInt i_order;
			std::vector<RtInt> orderVect;
			orderVect.reserve(sumcurves);
			for ( i = 0; i < sumcurves; ++i ) {
				i_order = (RtInt)porder[i];
				orderVect.push_back(i_order);
			}
			if ( (int)orderVect.size() != sumcurves ) {
				// not all values are defined
				parser.errHandler().handleError(
					RIE_MISSINGDATA, RIE_ERROR,
					"Line %ld, File \"%s\", badargument: '%s' argument %s not all elements could get copied",
					p1.lineNo(), parser.resourceName(),
					requestName(), "2 (order)", RI_NULL);
				return;
			}
			order = &orderVect[0];

			if ( p2.typeID() == BASICTYPE_FLOAT && p2.isArray() ) {
				knot = (RtFloat *)p2.getValue();
			} else {
				parser.errHandler().handleError(
					RIE_MISSINGDATA, RIE_ERROR,
					"Line %ld, File \"%s\", badargument: '%s' argument %s is not an array of numeric values",
					p2.lineNo(), parser.resourceName(),
					requestName(), "3 (knot)", RI_NULL);
			}
			if ( !knot ) {
				return;
			}

			if ( p3.typeID() == BASICTYPE_FLOAT && p3.isArray() ) {
				amin = (RtFloat *)p3.getValue();
			} else {
				parser.errHandler().handleError(
					RIE_MISSINGDATA, RIE_ERROR,
					"Line %ld, File \"%s\", badargument: '%s' argument %s is not an array of numeric values",
					p3.lineNo(), parser.resourceName(),
					requestName(), "4 (min)", RI_NULL);
			}
			if ( !amin ) {
				return;
			}

			if ( p4.typeID() == BASICTYPE_FLOAT && p4.isArray() ) {
				amax = (RtFloat *)p4.getValue();
			} else {
				parser.errHandler().handleError(
					RIE_MISSINGDATA, RIE_ERROR,
					"Line %ld, File \"%s\", badargument: '%s' argument %s is not an array of numeric values",
					p4.lineNo(), parser.resourceName(),
					requestName(), "5 (max)", RI_NULL);
			}
			if ( !amax ) {
				return;
			}

			// n
			if ( p5.typeID() == BASICTYPE_FLOAT && p5.isArray() ) {
				pn = (RtFloat *)p5.getValue();
			} else {
				parser.errHandler().handleError(
					RIE_MISSINGDATA, RIE_ERROR,
					"Line %ld, File \"%s\", badargument: '%s' argument %s is not an array of numeric values",
					p5.lineNo(), parser.resourceName(),
					requestName(), "6 (n)", RI_NULL);
			}
			if ( pn == NULL ) {
				return;
			}

			RtInt i_n;
			std::vector<RtInt> nVect;
			nVect.reserve(sumcurves);
			for ( i = 0; i < sumcurves; ++i ) {
				i_n = (RtInt)pn[i];
				nVect.push_back(i_n);
			}
			if ( (int)nVect.size() != sumcurves ) {
				// not all values are defined
				parser.errHandler().handleError(
					RIE_MISSINGDATA, RIE_ERROR,
					"Line %ld, File \"%s\", badargument: '%s' argument %s not all elements could get copied",
					p5.lineNo(), parser.resourceName(),
					requestName(), "6 (n)", RI_NULL);
				return;
			}
			n = &nVect[0];

			if ( p6.typeID() == BASICTYPE_FLOAT && p6.isArray() ) {
				u = (RtFloat *)p6.getValue();
			} else {
				parser.errHandler().handleError(
					RIE_MISSINGDATA, RIE_ERROR,
					"Line %ld, File \"%s\", badargument: '%s' argument %s is not an array of numeric values",
					p6.lineNo(), parser.resourceName(),
					requestName(), "7 (u)", RI_NULL);
			}
			if ( !u ) {
				return;
			}

			if ( p7.typeID() == BASICTYPE_FLOAT && p7.isArray() ) {
				v = (RtFloat *)p7.getValue();
			} else {
				parser.errHandler().handleError(
					RIE_MISSINGDATA, RIE_ERROR,
					"Line %ld, File \"%s\", badargument: '%s' argument %s is not an array of numeric values",
					p7.lineNo(), parser.resourceName(),
					requestName(), "8 (v)", RI_NULL);
			}
			if ( !v ) {
				return;
			}

			if ( p8.typeID() == BASICTYPE_FLOAT && p8.isArray() ) {
				w = (RtFloat *)p8.getValue();
			} else {
				parser.errHandler().handleError(
					RIE_MISSINGDATA, RIE_ERROR,
					"Line %ld, File \"%s\", badargument: '%s' argument %s is not an array of numeric values",
					p8.lineNo(), parser.resourceName(),
					requestName(), "9 (w)", RI_NULL);
			}
			if ( !w ) {
				return;
			}

			parser.ribFilter().trimCurve(nloops, ncurves, order, knot, amin, amax, n, u, v, w);
		}

		if ( (int)request.size() > cnt ) {
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
			requestName(), "(ncurves, order, knot, min, max, n, u, v, w)", RI_NULL);
	}
}
