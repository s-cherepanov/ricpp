//  - RenderMan(R) Interface CPP Language Binding
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

/** @file ribtransforms.cpp
 *  @author Andreas Pidde (andreas@pidde.de)
 *  @brief Implementation of the handling of RIB requests for transformations.
 */

#include "ricpp/ribparser/ribtransforms.h"

using namespace RiCPP;

void CIdentityRibRequest::operator()(IRibParserState &parser, CRibRequestData &request) const
{
	// Identity

	parser.ribFilter().identity();

	if ( request.size() > 9 ) {
		parser.errHandler().handleError(
			RIE_CONSISTENCY, RIE_WARNING,
			"Line %ld, File \"%s\", badargument: '%s' has additional parameters, they are ignored",
			parser.lineNo(), parser.resourceName(),
			requestName(), RI_NULL);
	}
}


void CTransformRibRequest::operator()(IRibParserState &parser, CRibRequestData &request) const
{
	// Transform [t1 ... t16]

	if ( request.size() >= 1 ) {

		CRibParameter &p0 = request[0];

		if ( p0.typeID() == BASICTYPE_FLOAT && p0.isArray() && p0.getCard() == 16 ) {
			RtFloat *m = (RtFloat *)p0.getValue();
			RtMatrix matrix;

			if ( m ) {
				int i;
				for ( i = 0; i < 16; ++i ) {
					matrix[i/4][i%4] = m[i];
				}

				parser.ribFilter().transform(matrix);

			} else {
				parser.errHandler().handleError(
					RIE_CONSISTENCY, RIE_ERROR,
					"Line %ld, File \"%s\", badargument: '%s' could not access matrix",
					parser.lineNo(), parser.resourceName(),
					requestName(), RI_NULL);
			}


		} else {
			parser.errHandler().handleError(
				RIE_CONSISTENCY, RIE_ERROR,
				"Line %ld, File \"%s\", badarry: '%s' transformation matrix malformed",
				p0.lineNo(), parser.resourceName(),
				requestName(), RI_NULL);
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
			requestName(), "1 ([t1...t16])", RI_NULL);
	}
}


void CConcatTransformRibRequest::operator()(IRibParserState &parser, CRibRequestData &request) const
{
	// ConcatTransform [t1 ... t16]

	if ( request.size() >= 1 ) {

		CRibParameter &p0 = request[0];

		if ( p0.typeID() == BASICTYPE_FLOAT && p0.isArray() && p0.getCard() == 16 ) {
			RtFloat *m = (RtFloat *)p0.getValue();
			RtMatrix matrix;

			if ( m ) {
				int i;
				for ( i = 0; i < 16; ++i ) {
					matrix[i/4][i%4] = m[i];
				}

				parser.ribFilter().concatTransform(matrix);

			} else {
				parser.errHandler().handleError(
					RIE_CONSISTENCY, RIE_ERROR,
					"Line %ld, File \"%s\", badargument: '%s' could not access matrix",
					parser.lineNo(), parser.resourceName(),
					requestName(), RI_NULL);
			}


		} else {
			parser.errHandler().handleError(
				RIE_CONSISTENCY, RIE_ERROR,
				"Line %ld, File \"%s\", badarry: '%s' transformation matrix malformed",
				p0.lineNo(), parser.resourceName(),
				requestName(), RI_NULL);
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
			requestName(), "1 ([t1...t16])", RI_NULL);
	}
}


void CPerspectiveRibRequest::operator()(IRibParserState &parser, CRibRequestData &request) const
{
	// Perspective fov

	if ( request.size() >= 1 ) {

		CRibParameter &p0 = request[0];
		RtFloat fov;

		if ( p0.getFloat(fov) ) {

			parser.ribFilter().perspective(fov);

		} else {
			parser.errHandler().handleError(
				RIE_CONSISTENCY, RIE_ERROR,
				"Line %ld, File \"%s\", badargument: '%s' argument %s is not numeric",
				p0.lineNo(), parser.resourceName(),
				requestName(), "1 (fov)", RI_NULL);
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
			requestName(), "(fov)", RI_NULL);
	}
}


void CTranslateRibRequest::operator()(IRibParserState &parser, CRibRequestData &request) const
{
	// Translate [ dx dy dz ] (added)
	// Translate dx dy dz

	if ( request.size() == 1 ) {

		CRibParameter &p0 = request[0];
		p0.convertIntToFloat();

		if ( p0.typeID() != BASICTYPE_FLOAT ) {
			parser.errHandler().handleError(
				RIE_CONSISTENCY, RIE_ERROR,
				"Line %ld, File \"%s\", badargument: '%s' argument %s is not numeric",
				p0.lineNo(), parser.resourceName(),
				requestName(), "1 [ dx dy dz ]", RI_NULL);
			return;
		}
		if ( !p0.isArray() || p0.getCard() < 3 ) {
			parser.errHandler().handleError(
				RIE_CONSISTENCY, RIE_ERROR,
				"Line %ld, File \"%s\", badargument: '%s' must have %s as arguments",
				p0.lineNo(), parser.resourceName(),
				requestName(), "(an array of) 3 floats [ dx dy dz ]", RI_NULL);
			return;
		}

		RtFloat *pv = (RtFloat *)p0.getValue();
		parser.ribFilter().translate(pv[0], pv[1], pv[2]);
		
		if ( p0.getCard() > 3 ) {
			parser.errHandler().handleError(
				RIE_CONSISTENCY, RIE_WARNING,
				"Line %ld, File \"%s\", badargument: '%s' has additional parameters, they are ignored",
				p0.lineNo(), parser.resourceName(),
				requestName(), RI_NULL);
		}

	} else if ( request.size() >= 3 ) {

		CRibParameter &p0 = request[0];
		CRibParameter &p1 = request[1];
		CRibParameter &p2 = request[2];

		RtFloat dx, dy, dz;

		bool b0 = p0.getFloat(dx),
		     b1 = p1.getFloat(dy),
			 b2 = p2.getFloat(dz);

		if ( b0 && b1 && b2 ) {

			parser.ribFilter().translate(dx, dy, dz);

		} else {
			if ( !b0 ) {
				parser.errHandler().handleError(
					RIE_CONSISTENCY, RIE_ERROR,
					"Line %ld, File \"%s\", badargument: '%s' argument %s is not numeric",
					p0.lineNo(), parser.resourceName(),
					requestName(), "1 (dx)", RI_NULL);
			}
			if ( !b1 ) {
				parser.errHandler().handleError(
					RIE_CONSISTENCY, RIE_ERROR,
					"Line %ld, File \"%s\", badargument: '%s' argument %s is not numeric",
					p1.lineNo(), parser.resourceName(),
					requestName(), "2 (dy)", RI_NULL);
			}
			if ( !b2 ) {
				parser.errHandler().handleError(
					RIE_CONSISTENCY, RIE_ERROR,
					"Line %ld, File \"%s\", badargument: '%s' argument %s is not numeric",
					p2.lineNo(), parser.resourceName(),
					requestName(), "3 (dz)", RI_NULL);
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
			"Line %ld, File \"%s\", badargument: '%s' arguments %s missing",
			parser.lineNo(), parser.resourceName(),
			requestName(), "(dx dy dz)", RI_NULL);
	}
}


void CRotateRibRequest::operator()(IRibParserState &parser, CRibRequestData &request) const
{
	// Rotate [ angle x y z ] (added)
	// Rotate angle x y z

	if ( request.size() == 1 ) {

		CRibParameter &p0 = request[0];
		p0.convertIntToFloat();

		if ( p0.typeID() != BASICTYPE_FLOAT ) {
			parser.errHandler().handleError(
				RIE_CONSISTENCY, RIE_ERROR,
				"Line %ld, File \"%s\", badargument: '%s' argument %s is not numeric",
				p0.lineNo(), parser.resourceName(),
				requestName(), "1 [ angle x y z ]", RI_NULL);
			return;
		}
		if ( !p0.isArray() || p0.getCard() < 4 ) {
			parser.errHandler().handleError(
				RIE_CONSISTENCY, RIE_ERROR,
				"Line %ld, File \"%s\", badargument: '%s' must have %s as arguments",
				p0.lineNo(), parser.resourceName(),
				requestName(), "(an array of) 4 floats [ angle x y z ]", RI_NULL);
			return;
		}

		RtFloat *pv = (RtFloat *)p0.getValue();
		parser.ribFilter().rotate(pv[0], pv[1], pv[2], pv[3]);
		
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

		RtFloat angle, x, y, z;

		bool b0 = p0.getFloat(angle),
		     b1 = p1.getFloat(x),
			 b2 = p2.getFloat(y),
			 b3 = p3.getFloat(z);

		if ( b0 && b1 && b2 && b3 ) {

			parser.ribFilter().rotate(angle, x, y, z);

		} else {
			if ( !b0 ) {
				parser.errHandler().handleError(
					RIE_CONSISTENCY, RIE_ERROR,
					"Line %ld, File \"%s\", badargument: '%s' argument %s is not numeric",
					p0.lineNo(), parser.resourceName(),
					requestName(), "1 (angle)", RI_NULL);
			}
			if ( !b1 ) {
				parser.errHandler().handleError(
					RIE_CONSISTENCY, RIE_ERROR,
					"Line %ld, File \"%s\", badargument: '%s' argument %s is not numeric",
					p1.lineNo(), parser.resourceName(),
					requestName(), "2 (x)", RI_NULL);
			}
			if ( !b2 ) {
				parser.errHandler().handleError(
					RIE_CONSISTENCY, RIE_ERROR,
					"Line %ld, File \"%s\", badargument: '%s' argument %s is not numeric",
					p2.lineNo(), parser.resourceName(),
					requestName(), "3 (y)", RI_NULL);
			}
			if ( !b3 ) {
				parser.errHandler().handleError(
					RIE_CONSISTENCY, RIE_ERROR,
					"Line %ld, File \"%s\", badargument: '%s' argument %s is not numeric",
					p3.lineNo(), parser.resourceName(),
					requestName(), "4 (z)", RI_NULL);
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
			requestName(), "(angle x y z)", RI_NULL);
	}
}


void CScaleRibRequest::operator()(IRibParserState &parser, CRibRequestData &request) const
{
	// Scale [ sx sy sz ] (added)
	// Scale sx sy sz

	if ( request.size() == 1 ) {

		CRibParameter &p0 = request[0];
		p0.convertIntToFloat();

		if ( p0.typeID() != BASICTYPE_FLOAT ) {
			parser.errHandler().handleError(
				RIE_CONSISTENCY, RIE_ERROR,
				"Line %ld, File \"%s\", badargument: '%s' argument %s is not numeric",
				p0.lineNo(), parser.resourceName(),
				requestName(), "1 [ sx sy sz ]", RI_NULL);
			return;
		}
		if ( !p0.isArray() || p0.getCard() < 3 ) {
			parser.errHandler().handleError(
				RIE_CONSISTENCY, RIE_ERROR,
				"Line %ld, File \"%s\", badargument: '%s' must have %s as arguments",
				p0.lineNo(), parser.resourceName(),
				requestName(), "(an array of) 3 floats [ sx sy sz ]", RI_NULL);
			return;
		}

		RtFloat *pv = (RtFloat *)p0.getValue();
		parser.ribFilter().scale(pv[0], pv[1], pv[2]);
		
		if ( p0.getCard() > 3 ) {
			parser.errHandler().handleError(
				RIE_CONSISTENCY, RIE_WARNING,
				"Line %ld, File \"%s\", badargument: '%s' has additional parameters, they are ignored",
				p0.lineNo(), parser.resourceName(),
				requestName(), RI_NULL);
		}

	} else if ( request.size() >= 3 ) {

		CRibParameter &p0 = request[0];
		CRibParameter &p1 = request[1];
		CRibParameter &p2 = request[2];

		RtFloat sx, sy, sz;

		bool b0 = p0.getFloat(sx),
		     b1 = p1.getFloat(sy),
			 b2 = p2.getFloat(sz);

		if ( b0 && b1 && b2 ) {

			parser.ribFilter().scale(sx, sy, sz);

		} else {
			if ( !b0 ) {
				parser.errHandler().handleError(
					RIE_CONSISTENCY, RIE_ERROR,
					"Line %ld, File \"%s\", badargument: '%s' argument %s is not numeric",
					p0.lineNo(), parser.resourceName(),
					requestName(), "1 (sx)", RI_NULL);
			}
			if ( !b1 ) {
				parser.errHandler().handleError(
					RIE_CONSISTENCY, RIE_ERROR,
					"Line %ld, File \"%s\", badargument: '%s' argument %s is not numeric",
					p1.lineNo(), parser.resourceName(),
					requestName(), "2 (sy)", RI_NULL);
			}
			if ( !b2 ) {
				parser.errHandler().handleError(
					RIE_CONSISTENCY, RIE_ERROR,
					"Line %ld, File \"%s\", badargument: '%s' argument %s is not numeric",
					p2.lineNo(), parser.resourceName(),
					requestName(), "3 (sz)", RI_NULL);
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
			"Line %ld, File \"%s\", badargument: '%s' arguments %s missing",
			parser.lineNo(), parser.resourceName(),
			requestName(), "(sx sy sz)", RI_NULL);
	}
}


void CSkewRibRequest::operator()(IRibParserState &parser, CRibRequestData &request) const
{
	// Skew [ angle dx1 dy1 dz1 dx2 dy2 dz2 ]
	// Skew angle dx1 dy1 dz1 dx2 dy2 dz2

	if ( request.size() == 1 ) {

		CRibParameter &p0 = request[0];
		p0.convertIntToFloat();

		if ( p0.typeID() != BASICTYPE_FLOAT ) {
			parser.errHandler().handleError(
				RIE_CONSISTENCY, RIE_ERROR,
				"Line %ld, File \"%s\", badargument: '%s' argument %s is not numeric",
				p0.lineNo(), parser.resourceName(),
				requestName(), "1 [ angle dx1 dy1 dz1 dx2 dy2 dz2 ]", RI_NULL);
			return;
		}
		if ( !p0.isArray() || p0.getCard() < 8 ) {
			parser.errHandler().handleError(
				RIE_CONSISTENCY, RIE_ERROR,
				"Line %ld, File \"%s\", badargument: '%s' must have %s as arguments",
				p0.lineNo(), parser.resourceName(),
				requestName(), "(an array of) 7 floats [ angle dx1 dy1 dz1 dx2 dy2 dz2 ]", RI_NULL);
			return;
		}

		RtFloat *pv = (RtFloat *)p0.getValue();
		
		parser.ribFilter().skew(pv[0], pv[1], pv[2], pv[3], pv[4], pv[5], pv[6]);
		
		if ( p0.getCard() > 7 ) {
			parser.errHandler().handleError(
				RIE_CONSISTENCY, RIE_WARNING,
				"Line %ld, File \"%s\", badargument: '%s' has additional parameters, they are ignored",
				p0.lineNo(), parser.resourceName(),
				requestName(), RI_NULL);
		}

	} else if ( request.size() >= 7 ) {

		CRibParameter &p0 = request[0];
		CRibParameter &p1 = request[1];
		CRibParameter &p2 = request[2];
		CRibParameter &p3 = request[3];
		CRibParameter &p4 = request[4];
		CRibParameter &p5 = request[5];
		CRibParameter &p6 = request[6];

		RtFloat angle, dx1, dy1, dz1, dx2, dy2, dz2;

		bool b0 = p0.getFloat(angle),
		     b1 = p1.getFloat(dx1),
			 b2 = p2.getFloat(dy1),
			 b3 = p3.getFloat(dz1),
			 b4 = p4.getFloat(dx2),
			 b5 = p5.getFloat(dy2),
			 b6 = p6.getFloat(dz2);

		if ( b0 && b1 && b2 && b3 && b4 && b5 && b6 ) {

			parser.ribFilter().skew(angle, dx1, dy1, dz1, dx2, dy2, dz2);

		} else {
			if ( !b0 ) {
				parser.errHandler().handleError(
					RIE_CONSISTENCY, RIE_ERROR,
					"Line %ld, File \"%s\", badargument: '%s' argument %s is not numeric",
					p0.lineNo(), parser.resourceName(),
					requestName(), "1 (angle)", RI_NULL);
			}
			if ( !b1 ) {
				parser.errHandler().handleError(
					RIE_CONSISTENCY, RIE_ERROR,
					"Line %ld, File \"%s\", badargument: '%s' argument %s is not numeric",
					p1.lineNo(), parser.resourceName(),
					requestName(), "2 (dx1)", RI_NULL);
			}
			if ( !b2 ) {
				parser.errHandler().handleError(
					RIE_CONSISTENCY, RIE_ERROR,
					"Line %ld, File \"%s\", badargument: '%s' argument %s is not numeric",
					p2.lineNo(), parser.resourceName(),
					requestName(), "3 (dy1)", RI_NULL);
			}
			if ( !b3 ) {
				parser.errHandler().handleError(
					RIE_CONSISTENCY, RIE_ERROR,
					"Line %ld, File \"%s\", badargument: '%s' argument %s is not numeric",
					p3.lineNo(), parser.resourceName(),
					requestName(), "4 (dz1)", RI_NULL);
			}
			if ( !b4 ) {
				parser.errHandler().handleError(
					RIE_CONSISTENCY, RIE_ERROR,
					"Line %ld, File \"%s\", badargument: '%s' argument %s is not numeric",
					p4.lineNo(), parser.resourceName(),
					requestName(), "5 (dx2)", RI_NULL);
			}
			if ( !b5 ) {
				parser.errHandler().handleError(
					RIE_CONSISTENCY, RIE_ERROR,
					"Line %ld, File \"%s\", badargument: '%s' argument %s is not numeric",
					p4.lineNo(), parser.resourceName(),
					requestName(), "6 (dy2)", RI_NULL);
			}
			if ( !b6 ) {
				parser.errHandler().handleError(
					RIE_CONSISTENCY, RIE_ERROR,
					"Line %ld, File \"%s\", badargument: '%s' argument %s is not numeric",
					p6.lineNo(), parser.resourceName(),
					requestName(), "7 (dz2)", RI_NULL);
			}
		}
		if ( request.size() > 7 ) {
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
			requestName(), "(angle dx1 dy1 dz1 dx2 dy2 dz2)", RI_NULL);
	}
}


void CDeformationRibRequest::operator()(IRibParserState &parser, CRibRequestData &request) const
{
	// Deformation name <paramlist>

	if ( request.size() >= 1 ) {

		CRibParameter &p0 = request[0];
		const char *name;

		if ( p0.getString(name) ) {

			CParameterClasses p;
			int n = request.getTokenList(1, p);
			if ( n > 0 ) {
				parser.ribFilter().deformationV(name, n, request.tokenList(), request.valueList());
			} else {
				parser.ribFilter().deformationV(name, 0, 0, 0);
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


void CScopedCoordinateSystemRibRequest::operator()(IRibParserState &parser, CRibRequestData &request) const
{
	// ScopedCoordinateSystem name

	if ( request.size() >= 1 ) {

		CRibParameter &p0 = request[0];
		const char *name;

		if ( p0.getString(name) ) {

			parser.ribFilter().scopedCoordinateSystem(name);

		} else {
			parser.errHandler().handleError(
				RIE_CONSISTENCY, RIE_ERROR,
				"Line %ld, File \"%s\", badargument: '%s' argument %s is not a string",
				p0.lineNo(), parser.resourceName(),
				requestName(), "1 (name)", RI_NULL);
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


void CCoordinateSystemRibRequest::operator()(IRibParserState &parser, CRibRequestData &request) const
{
	// CoordinateSystem name

	if ( request.size() >= 1 ) {

		CRibParameter &p0 = request[0];
		const char *name;

		if ( p0.getString(name) ) {

			parser.ribFilter().coordinateSystem(name);

		} else {
			parser.errHandler().handleError(
				RIE_CONSISTENCY, RIE_ERROR,
				"Line %ld, File \"%s\", badargument: '%s' argument %s is not a string",
				p0.lineNo(), parser.resourceName(),
				requestName(), "1 (name)", RI_NULL);
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


void CCoordSysTransformRibRequest::operator()(IRibParserState &parser, CRibRequestData &request) const
{
	// CoordSysTransform name

	if ( request.size() >= 1 ) {

		CRibParameter &p0 = request[0];
		const char *name;

		if ( p0.getString(name) ) {

			parser.ribFilter().coordSysTransform(name);

		} else {
			parser.errHandler().handleError(
				RIE_CONSISTENCY, RIE_ERROR,
				"Line %ld, File \"%s\", badargument: '%s' argument %s is not a string",
				p0.lineNo(), parser.resourceName(),
				requestName(), "1 (name)", RI_NULL);
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

void CTransformPointsRibRequest::operator()(IRibParserState &parser, CRibRequestData &request) const
{
	// CTransformPointsRibRequest has no RIB binding

	parser.errHandler().handleError(
		RIE_CONSISTENCY, RIE_ERROR,
		"Line %ld, File \"%s\", badargument: '%s' has no RIB binding, request is ignored",
		parser.lineNo(), parser.resourceName(),
		requestName(), RI_NULL);
}
