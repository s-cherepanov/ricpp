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

/** @file riboptions.cpp
 *  @author Andreas Pidde (andreas@pidde.de)
 *  @brief Implementation of the handling of RIB requests for options.
 */

#include "ricpp/ribparser/riboptions.h"

#ifndef _RICPP_RICONTEXT_OPTIONS_H
#include "ricpp/ricontext/options.h"
#endif // _RICPP_RICONTEXT_OPTIONS_H

using namespace RiCPP;


void CFormatRibRequest::operator()(IRibParserState &parser, CRibRequestData &request) const
{
	// Format [ xres yres pixelaspect ] (added)
	// Format xres yres pixelaspect

	if ( request.size() == 1 ) {

		CRibParameter &p0 = request[0];
		p0.convertIntToFloat();

		if ( p0.typeID() != BASICTYPE_FLOAT ) {
			parser.errHandler().handleError(
				RIE_CONSISTENCY, RIE_ERROR,
				"Line %ld, File \"%s\", badargument: '%s' argument %s is not numeric",
				p0.lineNo(), parser.resourceName(),
				requestName(), "1 [ xres yres pixelaspect ]", RI_NULL);
			return;
		}

		if ( !p0.isArray() || p0.getCard() < 3 ) {
			parser.errHandler().handleError(
				RIE_CONSISTENCY, RIE_ERROR,
				"Line %ld, File \"%s\", badargument: '%s' must have %s as arguments",
				p0.lineNo(), parser.resourceName(),
				requestName(), "(an array of) 3 floats", RI_NULL);
			return;
		}

		RtFloat *pv = (RtFloat *)p0.getValue();

		parser.ribFilter().format((RtInt)pv[0], (RtInt)pv[1], pv[2]);

		if ( p0.getCard() > 3 ) {
			parser.errHandler().handleError(
				RIE_CONSISTENCY, RIE_WARNING,
				"Line %ld, File \"%s\", badargument: '%s' has additional parameters, they are ignored",
				parser.lineNo(), parser.resourceName(),
				requestName(), RI_NULL);
		}

	} else if ( request.size() >= 3 ) {

		CRibParameter &p0 = request[0];
		CRibParameter &p1 = request[1];
		CRibParameter &p2 = request[2];
		RtInt xres, yres;
		RtFloat pixelaspect;
		bool b0 = p0.getInt(xres), b1 = p1.getInt(yres), b2 = p2.getFloat(pixelaspect);

		if ( b0 && b1 && b2 ) {

			parser.ribFilter().format(xres, yres, pixelaspect);

		} else {

			if ( !b0 ) {
				parser.errHandler().handleError(
					RIE_CONSISTENCY, RIE_ERROR,
					"Line %ld, File \"%s\", badargument: '%s' argument %s is not numeric",
					p0.lineNo(), parser.resourceName(),
					requestName(), "1 (xres)", RI_NULL);
			}
			if ( !b1 ) {
				parser.errHandler().handleError(
					RIE_CONSISTENCY, RIE_ERROR,
					"Line %ld, File \"%s\", badargument: '%s' argument %s is not numeric",
					p1.lineNo(), parser.resourceName(),
					requestName(), "2 (yres)", RI_NULL);
			}
			if ( !b2 ) {
				parser.errHandler().handleError(
					RIE_CONSISTENCY, RIE_ERROR,
					"Line %ld, File \"%s\", badargument: '%s' argument %s is not numeric",
					p1.lineNo(), parser.resourceName(),
					requestName(), "3 (pixelaspect)", RI_NULL);
			}
		}

		if ( request.size() > 3 ) {
			parser.errHandler().handleError(
				RIE_CONSISTENCY, RIE_WARNING,
				"Line %ld, File \"%s\", badargument: '%s' has additional parameters, they are ignored",
				parser.lineNo(), parser.resourceName(),
				requestName(), RI_NULL);
		}

	} else {

		parser.errHandler().handleError(
			RIE_MISSINGDATA, RIE_ERROR,
			"Line %ld, File \"%s\", badargument: '%s' arguments %s are missing", 
			parser.lineNo(), parser.resourceName(),
			requestName(), "(xres, yres, pixelaspect)", RI_NULL);

	}
}


void CFrameAspectRatioRibRequest::operator()(IRibParserState &parser, CRibRequestData &request) const
{
	// FrameAspectRatio ratio

	if ( request.size() >= 1 ) {

		CRibParameter &p0 = request[0];
		RtFloat aspect;

		if ( p0.getFloat(aspect) ) {

			parser.ribFilter().frameAspectRatio(aspect);

		} else {
			parser.errHandler().handleError(
				RIE_CONSISTENCY, RIE_ERROR,
				"Line %ld, File \"%s\", badargument: '%s' argument %s is not numeric",
				p0.lineNo(), parser.resourceName(),
				requestName(), "1 (aspect)", RI_NULL);
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
			requestName(), "(aspect)", RI_NULL);
	}
}


void CScreenWindowRibRequest::operator()(IRibParserState &parser, CRibRequestData &request) const
{
	// ScreenWindow [ left right bottom top ]
	// ScreenWindow left right bottom top

	if ( request.size() == 1 ) {

		CRibParameter &p0 = request[0];
		p0.convertIntToFloat();

		if ( p0.typeID() != BASICTYPE_FLOAT ) {
			parser.errHandler().handleError(
				RIE_CONSISTENCY, RIE_ERROR,
				"Line %ld, File \"%s\", badargument: '%s' argument %s is not numeric",
				p0.lineNo(), parser.resourceName(),
				requestName(), "1 [ left right bottom top ]", RI_NULL);
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
		
		parser.ribFilter().screenWindow(pv[0], pv[1], pv[2], pv[3]);
		
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

		RtFloat left, right, bottom, top;

		bool b0 = p0.getFloat(left),
		     b1 = p1.getFloat(right),
			 b2 = p2.getFloat(bottom),
			 b3 = p3.getFloat(top);

		if ( b0 && b1 && b2 && b3 ) {

			parser.ribFilter().screenWindow(left, right, bottom, top);

		} else {
			if ( !b0 ) {
				parser.errHandler().handleError(
					RIE_CONSISTENCY, RIE_ERROR,
					"Line %ld, File \"%s\", badargument: '%s' argument %s is not numeric",
					p0.lineNo(), parser.resourceName(),
					requestName(), "1 (left)", RI_NULL);
			}
			if ( !b1 ) {
				parser.errHandler().handleError(
					RIE_CONSISTENCY, RIE_ERROR,
					"Line %ld, File \"%s\", badargument: '%s' argument %s is not numeric",
					p1.lineNo(), parser.resourceName(),
					requestName(), "2 (right)", RI_NULL);
			}
			if ( !b2 ) {
				parser.errHandler().handleError(
					RIE_CONSISTENCY, RIE_ERROR,
					"Line %ld, File \"%s\", badargument: '%s' argument %s is not numeric",
					p2.lineNo(), parser.resourceName(),
					requestName(), "3 (bottom)", RI_NULL);
			}
			if ( !b3 ) {
				parser.errHandler().handleError(
					RIE_CONSISTENCY, RIE_ERROR,
					"Line %ld, File \"%s\", badargument: '%s' argument %s is not numeric",
					p3.lineNo(), parser.resourceName(),
					requestName(), "4 (top)", RI_NULL);
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
			requestName(), "(left, right, bottom, top)", RI_NULL);
	}
}


void CCropWindowRibRequest::operator()(IRibParserState &parser, CRibRequestData &request) const
{
	// CropWindow [ xmin xmax ymin ymax ] (added)
	// CropWindow xmin xmax ymin ymax

	if ( request.size() == 1 ) {

		CRibParameter &p0 = request[0];
		p0.convertIntToFloat();

		if ( p0.typeID() != BASICTYPE_FLOAT ) {
			parser.errHandler().handleError(
				RIE_CONSISTENCY, RIE_ERROR,
				"Line %ld, File \"%s\", badargument: '%s' argument %s is not numeric",
				p0.lineNo(), parser.resourceName(),
				requestName(), "1 [ xmin xmax ymin ymax ]", RI_NULL);
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
		
		parser.ribFilter().cropWindow(pv[0], pv[1], pv[2], pv[3]);
		
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

		RtFloat xmin, xmax, ymin, ymax;

		bool b0 = p0.getFloat(xmin),
		     b1 = p1.getFloat(xmax),
			 b2 = p2.getFloat(ymin),
			 b3 = p3.getFloat(ymax);

		if ( b0 && b1 && b2 && b3 ) {

			parser.ribFilter().cropWindow(xmin, xmax, ymin, ymax);

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
			requestName(), "(xmin, ymin, ymax, ymin)", RI_NULL);
	}
}


void CProjectionRibRequest::operator()(IRibParserState &parser, CRibRequestData &request) const
{
	// Projection name <paramlist>

	const char *name = COptions::defProjection;

	if ( request.size() >= 1 ) {

		CRibParameter &p0 = request[0];

		if ( p0.getString(name) ) {

			CParameterClasses p;
			int n = request.getTokenList(1, p);
			if ( n > 0 ) {
				parser.ribFilter().projectionV(name, n, request.tokenList(), request.valueList());
			} else {
				parser.ribFilter().projectionV(name, 0, 0, 0);
			}

		} else {
			parser.errHandler().handleError(
				RIE_CONSISTENCY, RIE_ERROR,
				"Line %ld, File \"%s\", badargument: '%s' argument %s is not a string, taking \"%s\" as default.",
				p0.lineNo(), parser.resourceName(),
				requestName(), "1 (name)", name, RI_NULL);

			parser.ribFilter().projectionV(name, 0, 0, 0);
		}

	} else {

		parser.errHandler().handleError(
			RIE_MISSINGDATA, RIE_ERROR,
			"Line %ld, File \"%s\", badargument: '%s' argument %s missing",
			parser.lineNo(), parser.resourceName(),
			requestName(), "(name)", RI_NULL);
	}
}


void CClippingRibRequest::operator()(IRibParserState &parser, CRibRequestData &request) const
{
	// Clipping [ near far ] (added)
	// Clipping near far

	if ( request.size() == 1 ) {

		CRibParameter &p0 = request[0];
		p0.convertIntToFloat();

		if ( p0.typeID() != BASICTYPE_FLOAT ) {
			parser.errHandler().handleError(
				RIE_CONSISTENCY, RIE_ERROR,
				"Line %ld, File \"%s\", badargument: '%s' one argument of %s is not numeric",
				p0.lineNo(), parser.resourceName(),
				requestName(), "[ near far ]", RI_NULL);
			return;
		}

		if ( !p0.isArray() || p0.getCard() < 2 ) {
			parser.errHandler().handleError(
				RIE_CONSISTENCY, RIE_ERROR,
				"Line %ld, File \"%s\", badargument: '%s' must have %s as arguments",
				p0.lineNo(), parser.resourceName(),
				requestName(), "(an array of) 2 floats", RI_NULL);
			return;
		}

		RtFloat *pv = (RtFloat *)p0.getValue();

		parser.ribFilter().clipping(pv[0], pv[1]);

		if ( p0.getCard() > 2 ) {
			parser.errHandler().handleError(
				RIE_CONSISTENCY, RIE_WARNING,
				"Line %ld, File \"%s\", badargument: '%s' has additional parameters, they are ignored",
				parser.lineNo(), parser.resourceName(),
				requestName(), RI_NULL);
		}

	} else if ( request.size() >= 2 ) {

		CRibParameter &p0 = request[0];
		CRibParameter &p1 = request[1];

		RtFloat fnear, ffar;
		bool b0 = p0.getFloat(fnear), b1 = p1.getFloat(ffar);

		if ( b0 && b1 ) {

			parser.ribFilter().clipping(fnear, ffar);

		} else {

			if ( !b0 ) {
				parser.errHandler().handleError(
					RIE_CONSISTENCY, RIE_ERROR,
					"Line %ld, File \"%s\", badargument: '%s' argument %s is not numeric",
					p0.lineNo(), parser.resourceName(),
					requestName(), "1 (near)", RI_NULL);
			}
			if ( !b1 ) {
				parser.errHandler().handleError(
					RIE_CONSISTENCY, RIE_ERROR,
					"Line %ld, File \"%s\", badargument: '%s' argument %s is not numeric",
					p1.lineNo(), parser.resourceName(),
					requestName(), "2 (far)", RI_NULL);
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
			"Line %ld, File \"%s\", badargument: '%s' arguments %s are missing", 
			parser.lineNo(), parser.resourceName(),
			requestName(), "(near far)", RI_NULL);

	}
}


void CClippingPlaneRibRequest::operator()(IRibParserState &parser, CRibRequestData &request) const
{
	// ClippingPlane [ x y z nx ny nz ] (added)
	// ClippingPlane x y z nx ny nz

	if ( request.size() == 1 ) {

		CRibParameter &p0 = request[0];
		p0.convertIntToFloat();

		if ( p0.typeID() != BASICTYPE_FLOAT ) {
			parser.errHandler().handleError(
				RIE_CONSISTENCY, RIE_ERROR,
				"Line %ld, File \"%s\", badargument: '%s' one argument of %s is not numeric",
				p0.lineNo(), parser.resourceName(),
				requestName(), "[ x y z nx ny nz ]", RI_NULL);
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

		RtFloat *pv = (RtFloat *)p0.getValue();

		parser.ribFilter().clippingPlane(pv[0], pv[1], pv[2], pv[3], pv[4], pv[5]);

		if ( p0.getCard() > 6 ) {
			parser.errHandler().handleError(
				RIE_CONSISTENCY, RIE_WARNING,
				"Line %ld, File \"%s\", badargument: '%s' has additional parameters, they are ignored",
				parser.lineNo(), parser.resourceName(),
				requestName(), RI_NULL);
		}

	} else if ( request.size() >= 6 ) {

		CRibParameter &p0 = request[0];
		CRibParameter &p1 = request[1];
		CRibParameter &p2 = request[2];
		CRibParameter &p3 = request[3];
		CRibParameter &p4 = request[4];
		CRibParameter &p5 = request[5];

		RtFloat x, y, z, nx, ny, nz;
		bool
			b0 = p0.getFloat(x),  b1 = p1.getFloat(y),
			b2 = p2.getFloat(z),  b3 = p3.getFloat(nx),
			b4 = p4.getFloat(ny), b5 = p5.getFloat(nz);

		if ( b0 && b1 && b2 && b3 && b4 && b5 ) {

			parser.ribFilter().clippingPlane(x, y, z, nx, ny, nz);

		} else {

			if ( !b0 ) {
				parser.errHandler().handleError(
					RIE_CONSISTENCY, RIE_ERROR,
					"Line %ld, File \"%s\", badargument: '%s' argument %s is not numeric",
					p0.lineNo(), parser.resourceName(),
					requestName(), "1 (x)", RI_NULL);
			}
			if ( !b1 ) {
				parser.errHandler().handleError(
					RIE_CONSISTENCY, RIE_ERROR,
					"Line %ld, File \"%s\", badargument: '%s' argument %s is not numeric",
					p1.lineNo(), parser.resourceName(),
					requestName(), "2 (y)", RI_NULL);
			}
			if ( !b2 ) {
				parser.errHandler().handleError(
					RIE_CONSISTENCY, RIE_ERROR,
					"Line %ld, File \"%s\", badargument: '%s' argument %s is not numeric",
					p2.lineNo(), parser.resourceName(),
					requestName(), "3 (z)", RI_NULL);
			}
			if ( !b3 ) {
				parser.errHandler().handleError(
					RIE_CONSISTENCY, RIE_ERROR,
					"Line %ld, File \"%s\", badargument: '%s' argument %s is not numeric",
					p3.lineNo(), parser.resourceName(),
					requestName(), "4 (nx)", RI_NULL);
			}
			if ( !b4 ) {
				parser.errHandler().handleError(
					RIE_CONSISTENCY, RIE_ERROR,
					"Line %ld, File \"%s\", badargument: '%s' argument %s is not numeric",
					p4.lineNo(), parser.resourceName(),
					requestName(), "5 (ny)", RI_NULL);
			}
			if ( !b5 ) {
				parser.errHandler().handleError(
					RIE_CONSISTENCY, RIE_ERROR,
					"Line %ld, File \"%s\", badargument: '%s' argument %s is not numeric",
					p5.lineNo(), parser.resourceName(),
					requestName(), "6 (nz)", RI_NULL);
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
			"Line %ld, File \"%s\", badargument: '%s' arguments %s are missing", 
			parser.lineNo(), parser.resourceName(),
			requestName(), "(x y z nx ny nz)", RI_NULL);

	}
}


void CDepthOfFieldRibRequest::operator()(IRibParserState &parser, CRibRequestData &request) const
{
	// DepthOfField [ fstop focallength focaldistance ] (added)
	// DepthOfField fstop focallength focaldistance

	if ( request.size() == 1 ) {

		CRibParameter &p0 = request[0];
		p0.convertIntToFloat();

		if ( p0.typeID() != BASICTYPE_FLOAT ) {
			parser.errHandler().handleError(
				RIE_CONSISTENCY, RIE_ERROR,
				"Line %ld, File \"%s\", badargument: '%s' one argument of %s is not numeric",
				p0.lineNo(), parser.resourceName(),
				requestName(), "[ fstop focallength focaldistance ]", RI_NULL);
			return;
		}

		if ( !p0.isArray() || p0.getCard() < 3 ) {
			parser.errHandler().handleError(
				RIE_CONSISTENCY, RIE_ERROR,
				"Line %ld, File \"%s\", badargument: '%s' must have %s as arguments",
				p0.lineNo(), parser.resourceName(),
				requestName(), "(an array of) 3 floats", RI_NULL);
			return;
		}

		RtFloat *pv = (RtFloat *)p0.getValue();

		parser.ribFilter().depthOfField(pv[0], pv[1], pv[2]);

		if ( p0.getCard() > 3 ) {
			parser.errHandler().handleError(
				RIE_CONSISTENCY, RIE_WARNING,
				"Line %ld, File \"%s\", badargument: '%s' has additional parameters, they are ignored",
				parser.lineNo(), parser.resourceName(),
				requestName(), RI_NULL);
		}

	} else if ( request.size() >= 3 ) {

		CRibParameter &p0 = request[0];
		CRibParameter &p1 = request[1];
		CRibParameter &p2 = request[2];

		RtFloat fstop, focallength, focaldistance;
		bool b0 = p0.getFloat(fstop), b1 = p1.getFloat(focallength), b2 = p2.getFloat(focaldistance);

		if ( b0 && b1 && b2 ) {

			parser.ribFilter().depthOfField(fstop, focallength, focaldistance);

		} else {

			if ( !b0 ) {
				parser.errHandler().handleError(
					RIE_CONSISTENCY, RIE_ERROR,
					"Line %ld, File \"%s\", badargument: '%s' argument %s is not numeric",
					p0.lineNo(), parser.resourceName(),
					requestName(), "1 (fstop)", RI_NULL);
			}
			if ( !b1 ) {
				parser.errHandler().handleError(
					RIE_CONSISTENCY, RIE_ERROR,
					"Line %ld, File \"%s\", badargument: '%s' argument %s is not numeric",
					p1.lineNo(), parser.resourceName(),
					requestName(), "2 (focallength)", RI_NULL);
			}
			if ( !b2 ) {
				parser.errHandler().handleError(
					RIE_CONSISTENCY, RIE_ERROR,
					"Line %ld, File \"%s\", badargument: '%s' argument %s is not numeric",
					p1.lineNo(), parser.resourceName(),
					requestName(), "3 (focaldistance)", RI_NULL);
			}
		}

		if ( request.size() > 3 ) {
			parser.errHandler().handleError(
				RIE_CONSISTENCY, RIE_WARNING,
				"Line %ld, File \"%s\", badargument: '%s' has additional parameters, they are ignored",
				parser.lineNo(), parser.resourceName(),
				requestName(), RI_NULL);
		}

	} else {

		parser.errHandler().handleError(
			RIE_MISSINGDATA, RIE_ERROR,
			"Line %ld, File \"%s\", badargument: '%s' arguments %s are missing", 
			parser.lineNo(), parser.resourceName(),
			requestName(), "(smin smax)", RI_NULL);

	}
}


void CShutterRibRequest::operator()(IRibParserState &parser, CRibRequestData &request) const
{
	// Shutter [ smin smax ] (added)
	// Shutter smin smax

	if ( request.size() == 1 ) {

		CRibParameter &p0 = request[0];
		p0.convertIntToFloat();

		if ( p0.typeID() != BASICTYPE_FLOAT ) {
			parser.errHandler().handleError(
				RIE_CONSISTENCY, RIE_ERROR,
				"Line %ld, File \"%s\", badargument: '%s' one argument of %s is not numeric",
				p0.lineNo(), parser.resourceName(),
				requestName(), "[ smin smax ]", RI_NULL);
			return;
		}

		if ( !p0.isArray() || p0.getCard() < 2 ) {
			parser.errHandler().handleError(
				RIE_CONSISTENCY, RIE_ERROR,
				"Line %ld, File \"%s\", badargument: '%s' must have %s as arguments",
				p0.lineNo(), parser.resourceName(),
				requestName(), "(an array of) 2 floats", RI_NULL);
			return;
		}

		RtFloat *pv = (RtFloat *)p0.getValue();

		parser.ribFilter().shutter(pv[0], pv[1]);

		if ( p0.getCard() > 2 ) {
			parser.errHandler().handleError(
				RIE_CONSISTENCY, RIE_WARNING,
				"Line %ld, File \"%s\", badargument: '%s' has additional parameters, they are ignored",
				parser.lineNo(), parser.resourceName(),
				requestName(), RI_NULL);
		}

	} else if ( request.size() >= 2 ) {

		CRibParameter &p0 = request[0];
		CRibParameter &p1 = request[1];

		RtFloat smin, smax;
		bool b0 = p0.getFloat(smin), b1 = p1.getFloat(smax);

		if ( b0 && b1 ) {

			parser.ribFilter().shutter(smin, smax);

		} else {

			if ( !b0 ) {
				parser.errHandler().handleError(
					RIE_CONSISTENCY, RIE_ERROR,
					"Line %ld, File \"%s\", badargument: '%s' argument %s is not numeric",
					p0.lineNo(), parser.resourceName(),
					requestName(), "1 (smin)", RI_NULL);
			}
			if ( !b1 ) {
				parser.errHandler().handleError(
					RIE_CONSISTENCY, RIE_ERROR,
					"Line %ld, File \"%s\", badargument: '%s' argument %s is not numeric",
					p1.lineNo(), parser.resourceName(),
					requestName(), "2 (smax)", RI_NULL);
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
			"Line %ld, File \"%s\", badargument: '%s' arguments %s are missing", 
			parser.lineNo(), parser.resourceName(),
			requestName(), "(smin smax)", RI_NULL);

	}
}


void CPixelVarianceRibRequest::operator()(IRibParserState &parser, CRibRequestData &request) const
{
	// PixelVariance variation

	if ( request.size() >= 1 ) {

		CRibParameter &p0 = request[0];
		RtFloat variation;

		if ( p0.getFloat(variation) ) {

			parser.ribFilter().pixelVariance(variation);

		} else {
			parser.errHandler().handleError(
				RIE_CONSISTENCY, RIE_ERROR,
				"Line %ld, File \"%s\", badargument: '%s' argument %s is not numeric",
				p0.lineNo(), parser.resourceName(),
				requestName(), "1 (variation)", RI_NULL);
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
			requestName(), "(variation)", RI_NULL);
	}
}


void CPixelSamplesRibRequest::operator()(IRibParserState &parser, CRibRequestData &request) const
{
	// PixelSamples [ xsamples ysamples ] (added)
	// PixelSamples xsamples ysamples

	if ( request.size() == 1 ) {

		CRibParameter &p0 = request[0];
		p0.convertIntToFloat();

		if ( p0.typeID() != BASICTYPE_FLOAT ) {
			parser.errHandler().handleError(
				RIE_CONSISTENCY, RIE_ERROR,
				"Line %ld, File \"%s\", badargument: '%s' one argument of %s is not numeric",
				p0.lineNo(), parser.resourceName(),
				requestName(), "[ xsamples ysamples ]", RI_NULL);
			return;
		}

		if ( !p0.isArray() || p0.getCard() < 2 ) {
			parser.errHandler().handleError(
				RIE_CONSISTENCY, RIE_ERROR,
				"Line %ld, File \"%s\", badargument: '%s' must have %s as arguments",
				p0.lineNo(), parser.resourceName(),
				requestName(), "(an array of) 2 floats", RI_NULL);
			return;
		}

		RtFloat *pv = (RtFloat *)p0.getValue();

		parser.ribFilter().pixelSamples(pv[0], pv[1]);

		if ( p0.getCard() > 2 ) {
			parser.errHandler().handleError(
				RIE_CONSISTENCY, RIE_WARNING,
				"Line %ld, File \"%s\", badargument: '%s' has additional parameters, they are ignored",
				parser.lineNo(), parser.resourceName(),
				requestName(), RI_NULL);
		}

	} else if ( request.size() >= 2 ) {

		CRibParameter &p0 = request[0];
		CRibParameter &p1 = request[1];

		RtFloat xsamples, ysamples;
		bool b0 = p0.getFloat(xsamples), b1 = p1.getFloat(ysamples);

		if ( b0 && b1 ) {

			parser.ribFilter().pixelSamples(xsamples, ysamples);

		} else {

			if ( !b0 ) {
				parser.errHandler().handleError(
					RIE_CONSISTENCY, RIE_ERROR,
					"Line %ld, File \"%s\", badargument: '%s' argument %s is not numeric",
					p0.lineNo(), parser.resourceName(),
					requestName(), "1 (xsamples)", RI_NULL);
			}
			if ( !b1 ) {
				parser.errHandler().handleError(
					RIE_CONSISTENCY, RIE_ERROR,
					"Line %ld, File \"%s\", badargument: '%s' argument %s is not numeric",
					p1.lineNo(), parser.resourceName(),
					requestName(), "2 (ysamples)", RI_NULL);
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
			"Line %ld, File \"%s\", badargument: '%s' arguments %s are missing", 
			parser.lineNo(), parser.resourceName(),
			requestName(), "(smin smax)", RI_NULL);

	}
}


void CPixelFilterRibRequest::operator()(IRibParserState &parser, CRibRequestData &request) const
{
	// PixelFilter functionName xwidth ywidth

	if ( request.size() >= 3 ) {

		CRibParameter &p0 = request[0];
		CRibParameter &p1 = request[1];
		CRibParameter &p2 = request[2];

		RtString functionName;
		RtFloat xwidth, ywidth;

		bool b0 = p0.getString(functionName), b1 = p1.getFloat(xwidth), b2 = p2.getFloat(ywidth);

		if ( b0 && b1 && b2 ) {
			const IFilterFunc *func = 0;
			RtToken name = parser.renderState().tokFind(functionName);
			if ( name )
				func = parser.renderState().filterFunc(name);

			if ( func ) {
				parser.ribFilter().pixelFilter(*func, xwidth, ywidth);
			} else {
				parser.errHandler().handleError(
					RIE_CONSISTENCY, RIE_ERROR,
					"Line %ld, File \"%s\", badargument: '%s' argument %s=%s is not a known pixel filter",
					p0.lineNo(), parser.resourceName(),
					requestName(), "1 (functionName)", name, RI_NULL);
			}

		} else {
			if ( !b0 ) {
				parser.errHandler().handleError(
					RIE_CONSISTENCY, RIE_ERROR,
					"Line %ld, File \"%s\", badargument: '%s' argument %s is not a string",
					p0.lineNo(), parser.resourceName(),
					requestName(), "1 (functionName)", RI_NULL);
			}
			if ( !b1 ) {
				parser.errHandler().handleError(
					RIE_CONSISTENCY, RIE_ERROR,
					"Line %ld, File \"%s\", badargument: '%s' argument %s is not numeric",
					p1.lineNo(), parser.resourceName(),
					requestName(), "2 (xwidth)", RI_NULL);
			}
			if ( !b2 ) {
				parser.errHandler().handleError(
					RIE_CONSISTENCY, RIE_ERROR,
					"Line %ld, File \"%s\", badargument: '%s' argument %s is not numeric",
					p1.lineNo(), parser.resourceName(),
					requestName(), "3 (ywidth)", RI_NULL);
			}
		}
		if ( request.size() > 3 ) {
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
			requestName(), "(functionName xwidth ywidth)", RI_NULL);
	}
}


void CExposureRibRequest::operator()(IRibParserState &parser, CRibRequestData &request) const
{
	// Exposure [ gain gamma ] (added)
	// Exposure gain gamma

	if ( request.size() == 1 ) {

		CRibParameter &p0 = request[0];
		p0.convertIntToFloat();

		if ( p0.typeID() != BASICTYPE_FLOAT ) {
			parser.errHandler().handleError(
				RIE_CONSISTENCY, RIE_ERROR,
				"Line %ld, File \"%s\", badargument: '%s' one argument of %s is not numeric",
				p0.lineNo(), parser.resourceName(),
				requestName(), "[ gain gamma ]", RI_NULL);
			return;
		}

		if ( !p0.isArray() || p0.getCard() < 2 ) {
			parser.errHandler().handleError(
				RIE_CONSISTENCY, RIE_ERROR,
				"Line %ld, File \"%s\", badargument: '%s' must have %s as arguments",
				p0.lineNo(), parser.resourceName(),
				requestName(), "(an array of) 2 floats", RI_NULL);
			return;
		}

		RtFloat *pv = (RtFloat *)p0.getValue();

		parser.ribFilter().exposure(pv[0], pv[1]);

		if ( p0.getCard() > 2 ) {
			parser.errHandler().handleError(
				RIE_CONSISTENCY, RIE_WARNING,
				"Line %ld, File \"%s\", badargument: '%s' has additional parameters, they are ignored",
				parser.lineNo(), parser.resourceName(),
				requestName(), RI_NULL);
		}

	} else if ( request.size() >= 2 ) {

		CRibParameter &p0 = request[0];
		CRibParameter &p1 = request[1];

		RtFloat gain, gamma;
		bool b0 = p0.getFloat(gain), b1 = p1.getFloat(gamma);

		if ( b0 && b1 ) {

			parser.ribFilter().exposure(gain, gamma);

		} else {

			if ( !b0 ) {
				parser.errHandler().handleError(
					RIE_CONSISTENCY, RIE_ERROR,
					"Line %ld, File \"%s\", badargument: '%s' argument %s is not numeric",
					p0.lineNo(), parser.resourceName(),
					requestName(), "1 (gain)", RI_NULL);
			}
			if ( !b1 ) {
				parser.errHandler().handleError(
					RIE_CONSISTENCY, RIE_ERROR,
					"Line %ld, File \"%s\", badargument: '%s' argument %s is not numeric",
					p1.lineNo(), parser.resourceName(),
					requestName(), "2 (gamma)", RI_NULL);
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
			"Line %ld, File \"%s\", badargument: '%s' arguments %s are missing", 
			parser.lineNo(), parser.resourceName(),
			requestName(), "(smin smax)", RI_NULL);

	}
}


void CImagerRibRequest::operator()(IRibParserState &parser, CRibRequestData &request) const
{
	// Imager name <paramlist>
	const char *name = COptions::defImagerName;

	if ( request.size() >= 1 ) {

		CRibParameter &p0 = request[0];

		if ( p0.getString(name) ) {

			CParameterClasses p;
			int n = request.getTokenList(1, p);
			if ( n > 0 ) {
				parser.ribFilter().imagerV(name, n, request.tokenList(), request.valueList());
			} else {
				parser.ribFilter().imagerV(name, 0, 0, 0);
			}

		} else {
			parser.errHandler().handleError(
				RIE_CONSISTENCY, RIE_ERROR,
				"Line %ld, File \"%s\", badargument: '%s' argument %s is not a string, taking \"%s\" as default.",
				p0.lineNo(), parser.resourceName(),
				requestName(), "1 (name)", name, RI_NULL);

			parser.ribFilter().imagerV(name, 0, 0, 0);
		}

	} else {

		parser.errHandler().handleError(
			RIE_MISSINGDATA, RIE_ERROR,
			"Line %ld, File \"%s\", badargument: '%s' argument %s missing",
			parser.lineNo(), parser.resourceName(),
			requestName(), "(name)", RI_NULL);
	}
}


void CQuantizeRibRequest::operator()(IRibParserState &parser, CRibRequestData &request) const
{
	// Quantize type one qmin qmax amplifier

	if ( request.size() >= 5 ) {

		CRibParameter &p0 = request[0];
		CRibParameter &p1 = request[1];
		CRibParameter &p2 = request[2];
		CRibParameter &p3 = request[3];
		CRibParameter &p4 = request[4];

		const char *type;
		RtInt one, qmin, qmax;
		RtFloat amplifier;
		bool
			b0 = p0.getString(type),  b1 = p1.getInt(one),
			b2 = p2.getInt(qmin),  b3 = p3.getInt(qmax),
			b4 = p4.getFloat(amplifier);

		if ( b0 && b1 && b2 && b3 && b4 ) {

			parser.ribFilter().quantize(type, one, qmin, qmax, amplifier);

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
					"Line %ld, File \"%s\", badargument: '%s' argument %s is not an integer",
					p1.lineNo(), parser.resourceName(),
					requestName(), "2 (one)", RI_NULL);
			}
			if ( !b2 ) {
				parser.errHandler().handleError(
					RIE_CONSISTENCY, RIE_ERROR,
					"Line %ld, File \"%s\", badargument: '%s' argument %s is not an integer",
					p2.lineNo(), parser.resourceName(),
					requestName(), "3 (qmin)", RI_NULL);
			}
			if ( !b3 ) {
				parser.errHandler().handleError(
					RIE_CONSISTENCY, RIE_ERROR,
					"Line %ld, File \"%s\", badargument: '%s' argument %s is not an integer",
					p3.lineNo(), parser.resourceName(),
					requestName(), "4 (qmax)", RI_NULL);
			}
			if ( !b4 ) {
				parser.errHandler().handleError(
					RIE_CONSISTENCY, RIE_ERROR,
					"Line %ld, File \"%s\", badargument: '%s' argument %s is not numeric",
					p4.lineNo(), parser.resourceName(),
					requestName(), "5 (amplifier)", RI_NULL);
			}
		}

		if ( request.size() > 5 ) {
			parser.errHandler().handleError(
				RIE_CONSISTENCY, RIE_WARNING,
				"Line %ld, File \"%s\", badargument: '%s' has additional parameters, they are ignored",
				parser.lineNo(), parser.resourceName(),
				requestName(), RI_NULL);
		}

	} else {

		parser.errHandler().handleError(
			RIE_MISSINGDATA, RIE_ERROR,
			"Line %ld, File \"%s\", badargument: '%s' arguments %s are missing", 
			parser.lineNo(), parser.resourceName(),
			requestName(), "(type one qmin qmax amplifier)", RI_NULL);

	}
}


void CDisplayChannelRibRequest::operator()(IRibParserState &parser, CRibRequestData &request) const
{
	// DisplayChannel channel <paramlist>

	if ( request.size() >= 1 ) {

		CRibParameter &p0 = request[0];
		const char *channel;

		if ( p0.getString(channel) ) {

			CParameterClasses p;
			int n = request.getTokenList(1, p);
			if ( n > 0 ) {
				parser.ribFilter().displayChannelV(channel, n, request.tokenList(), request.valueList());
			} else {
				parser.ribFilter().displayChannelV(channel, 0, 0, 0);
			}

		} else {
			parser.errHandler().handleError(
				RIE_CONSISTENCY, RIE_ERROR,
				"Line %ld, File \"%s\", badargument: '%s' argument %s is not a string",
				p0.lineNo(), parser.resourceName(),
				requestName(), "1 (channel)", RI_NULL);
		}

	} else {

		parser.errHandler().handleError(
			RIE_MISSINGDATA, RIE_ERROR,
			"Line %ld, File \"%s\", badargument: '%s' argument %s missing",
			parser.lineNo(), parser.resourceName(),
			requestName(), "(channel)", RI_NULL);
	}
}


void CDisplayRibRequest::operator()(IRibParserState &parser, CRibRequestData &request) const
{
	// Display name type mode <paramlist>
	if ( request.size() >= 3 ) {

		CRibParameter &p0 = request[0];
		CRibParameter &p1 = request[1];
		CRibParameter &p2 = request[2];

		const char *name, *type, *mode;
		bool b0 = p0.getString(name), b1 = p1.getString(type), b2 = p2.getString(mode);

		if ( b0 && b1 && b2 ) {

			CParameterClasses p;
			int n = request.getTokenList(3, p);
			if ( n > 0 ) {
				parser.ribFilter().displayV(name, type, mode, n, request.tokenList(), request.valueList());
			} else {
				parser.ribFilter().displayV(name, type, mode, 0, 0, 0);
			}

		} else {
			if ( !b0 ) {
				parser.errHandler().handleError(
					RIE_CONSISTENCY, RIE_ERROR,
					"Line %ld, File \"%s\", badargument: '%s' argument %s is not a string",
					p0.lineNo(), parser.resourceName(),
					requestName(), "1 (name)", RI_NULL);
			}
			if ( !b1 ) {
				parser.errHandler().handleError(
					RIE_CONSISTENCY, RIE_ERROR,
					"Line %ld, File \"%s\", badargument: '%s' argument %s is not a string",
					p1.lineNo(), parser.resourceName(),
					requestName(), "2 (type)", RI_NULL);
			}
			if ( !b2 ) {
				parser.errHandler().handleError(
					RIE_CONSISTENCY, RIE_ERROR,
					"Line %ld, File \"%s\", badargument: '%s' argument %s is not a string",
					p2.lineNo(), parser.resourceName(),
					requestName(), "3 (mode)", RI_NULL);
			}
		}

	} else {

		parser.errHandler().handleError(
			RIE_MISSINGDATA, RIE_ERROR,
			"Line %ld, File \"%s\", badargument: '%s' arguments %s missing",
			parser.lineNo(), parser.resourceName(),
			requestName(), "(name type mode ...)", RI_NULL);
	}
}


void CHiderRibRequest::operator()(IRibParserState &parser, CRibRequestData &request) const
{
	// Hider type <paramlist>
	const char *type = COptions::defHiderType;

	if ( request.size() >= 1 ) {

		CRibParameter &p0 = request[0];

		if ( p0.getString(type) ) {

			CParameterClasses p;
			int n = request.getTokenList(1, p);
			if ( n > 0 ) {
				parser.ribFilter().hiderV(type, n, request.tokenList(), request.valueList());
			} else {
				parser.ribFilter().hiderV(type, 0, 0, 0);
			}

		} else {
			parser.errHandler().handleError(
				RIE_CONSISTENCY, RIE_ERROR,
				"Line %ld, File \"%s\", badargument: '%s' argument %s is not a string, taking \"%s\" as default.",
				p0.lineNo(), parser.resourceName(),
				requestName(), "1 (type)", type, RI_NULL);

			parser.ribFilter().hiderV(type, 0, 0, 0);
		}

	} else {

		parser.errHandler().handleError(
			RIE_MISSINGDATA, RIE_ERROR,
			"Line %ld, File \"%s\", badargument: '%s' argument %s missing",
			parser.lineNo(), parser.resourceName(),
			requestName(), "(type)", RI_NULL);
	}
}


void CColorSamplesRibRequest::operator()(IRibParserState &parser, CRibRequestData &request) const
{
	// Quantize [nRGB] [RGBn]

	if ( request.size() >= 2 ) {

		CRibParameter &p0 = request[0];
		CRibParameter &p1 = request[1];

		p1.convertIntToFloat();
		void *v0 = p0.getValue();
		void *v1 = p1.getValue();

		if ( p0.typeID() != BASICTYPE_FLOAT ) {
			parser.errHandler().handleError(
				RIE_CONSISTENCY, RIE_ERROR,
				"Line %ld, File \"%s\", badargument: '%s' elements of argument %s are not numeric",
				p0.lineNo(), parser.resourceName(),
				requestName(), "1 ([nRGB])", RI_NULL);
			return;
		}

		if ( !p0.isArray() ) {
			parser.errHandler().handleError(
				RIE_CONSISTENCY, RIE_ERROR,
				"Line %ld, File \"%s\", badargument: '%s' argument %s is not an array",
				p0.lineNo(), parser.resourceName(),
				requestName(), "1 ([nRGB])", RI_NULL);
			return;
		}

		if ( p1.typeID() != BASICTYPE_FLOAT ) {
			parser.errHandler().handleError(
				RIE_CONSISTENCY, RIE_ERROR,
				"Line %ld, File \"%s\", badargument: '%s' elements of argument %s are not numeric",
				p0.lineNo(), parser.resourceName(),
				requestName(), "2 ([RGBn])", RI_NULL);
			return;
		}

		if ( !p1.isArray() ) {
			parser.errHandler().handleError(
				RIE_CONSISTENCY, RIE_ERROR,
				"Line %ld, File \"%s\", badargument: '%s' argument %s is not an array",
				p0.lineNo(), parser.resourceName(),
				requestName(), "2 ([RGBn])", RI_NULL);
			return;
		}

		if ( p0.getCard() != p1.getCard() ) {
			parser.errHandler().handleError(
				RIE_CONSISTENCY, RIE_ERROR,
				"Line %ld, File \"%s\", badargument: '%s' arguments %s have not the same size",
				p0.lineNo(), parser.resourceName(),
				requestName(), "([nRGB] [RGBn])", RI_NULL);
			return;
		}

		if ( !v0 ) {
			parser.errHandler().handleError(
				RIE_CONSISTENCY, RIE_ERROR,
				"Line %ld, File \"%s\", badargument: '%s' argument %s could not be extracted",
				p0.lineNo(), parser.resourceName(),
				requestName(), "1 ([nRGB])", RI_NULL);
			return;
		}

		if ( !v1 ) {
			parser.errHandler().handleError(
				RIE_CONSISTENCY, RIE_ERROR,
				"Line %ld, File \"%s\", badargument: '%s' argument %s could not be extracted",
				p0.lineNo(), parser.resourceName(),
				requestName(), "2 ([RGBn])", RI_NULL);
			return;
		}

		parser.ribFilter().colorSamples((RtInt)p0.getCard(), (RtFloat *)v0, (RtFloat *)v1);

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
			"Line %ld, File \"%s\", badargument: '%s' arguments %s are missing", 
			parser.lineNo(), parser.resourceName(),
			requestName(), "([nRGB] [RGBn])", RI_NULL);

	}
}


void CRelativeDetailRibRequest::operator()(IRibParserState &parser, CRibRequestData &request) const
{
	// RelativeDetail relativedetail

	if ( request.size() >= 1 ) {

		CRibParameter &p0 = request[0];
		RtFloat relativedetail;

		if ( p0.getFloat(relativedetail) ) {

			parser.ribFilter().relativeDetail(relativedetail);

		} else {
			parser.errHandler().handleError(
				RIE_CONSISTENCY, RIE_ERROR,
				"Line %ld, File \"%s\", badargument: '%s' argument %s is not numeric",
				p0.lineNo(), parser.resourceName(),
				requestName(), "1 (relativedetail)", RI_NULL);
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
			requestName(), "(relativedetail)", RI_NULL);
	}
}

void COptionRibRequest::operator()(IRibParserState &parser, CRibRequestData &request) const
{
	// Option name <paramlist>

	if ( request.size() >= 1 ) {

		CRibParameter &p0 = request[0];
		const char *name;

		if ( p0.getString(name) ) {

			CParameterClasses p;
			int n = request.getTokenList(1, p);
			if ( n > 0 ) {
				parser.ribFilter().optionV(name, n, request.tokenList(), request.valueList());
			} else {
				parser.ribFilter().optionV(name, 0, 0, 0);
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
