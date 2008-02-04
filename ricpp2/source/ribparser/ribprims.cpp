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
 *  @brief Implementation of the handling of RIB requests for prims.
 */

#include "ricpp/ribparser/ribprims.h"

#ifndef _RICPP_RICPP_SUBDIVFUNC_H
#include "ricpp/ricpp/subdivfunc.h"
#endif // _RICPP_RICPP_SUBDIVFUNC_H

using namespace RiCPP;

void CPolygonRibRequest::operator()(IRibParserState &parser, CRibRequestData &request) const
{
	// Polygon <paramlist>
	int n = request.getTokenListNoCheck(0);	// Needed for numVertices
	if ( n > 0 ) {
		RtInt npts = request.numVertices(0, n);
		if ( npts > 0 ) {
			CPolygonClasses p(npts);
			n = request.getTokenList(0, p); // Test if number of elements are correct
			if ( n > 0 ) {
				parser.ribFilter().polygonV(
					npts,
					n, request.tokenList(), request.valueList());
			} else {
				parser.ribFilter().polygonV(
					0,
					0, 0, 0);
			}
		} else {
			parser.errHandler().handleError(
				RIE_MISSINGDATA, RIE_ERROR,
				"Line %ld, File \"%s\", badargument: '%s' arguments missing, there must be at least positional data",
				parser.lineNo(), parser.resourceName(), requestName(), RI_NULL);
		}
	} else {
		parser.errHandler().handleError(
			RIE_MISSINGDATA, RIE_ERROR,
			"Line %ld, File \"%s\", badargument: '%s' arguments missing, there must be at least positional data",
			parser.lineNo(), parser.resourceName(), requestName(), RI_NULL);
	}
}

void CGeneralPolygonRibRequest::operator()(IRibParserState &parser, CRibRequestData &request) const
{
		// GeneralPolygon [ nvertices ] <paramlist>
		if ( request.size() >= 1 ) {
			CRibParameter &p0 = request[0];
			p0.convertFloatToInt();

			int i;
			RtInt nloops=0, *nverts=0;
			RtInt *pnverts=0;

			// The number of loops
			if ( p0.typeID() == BASICTYPE_INTEGER && p0.isArray() ) {
				nloops = (RtInt)p0.getCard();
				pnverts = (RtInt *)p0.getValue();
			} else {
				parser.errHandler().handleError(
					RIE_CONSISTENCY, RIE_ERROR,
					"Line %ld, File \"%s\", badargument: '%s' argument %s contains no integers or is not an array",
					p0.lineNo(), parser.resourceName(),
					requestName(), "1 [ nvertices ]", RI_NULL);
			}
			if ( nloops == 0 || pnverts == 0 ) // Nothing to do
				return;

			// The number of vertices
			RtInt i_nverts;
			std::vector<RtInt> nvertsVect;
			nvertsVect.reserve(nloops);
			int sumverts = 0;
			for ( i = 0; i < nloops; ++i ) {
				i_nverts = pnverts[i];
				nvertsVect.push_back(i_nverts);
				sumverts += i_nverts;
			}
			if ( (int)nvertsVect.size() != nloops ) {
				// not all values are defined
				parser.errHandler().handleError(
					RIE_BUG, RIE_ERROR,
					"Line %ld, File \"%s\", badargument: '%s' argument %s, couldn't store all values",
					p0.lineNo(), parser.resourceName(),
					requestName(), "1 [ nvertices ]", RI_NULL);
				return;
			}
			nverts = &nvertsVect[0];
			CGeneralPolygonClasses p(nloops, nverts);
			int n = request.getTokenList(1, p);
			if ( n >= 0 ) {
				int nvertices = request.numVertices(1, n);
				if ( nvertices > 0 ) {
					parser.ribFilter().generalPolygonV(
						nloops, nverts,
						n, request.tokenList(), request.valueList());
				} else {
					parser.errHandler().handleError(
						RIE_MISSINGDATA, RIE_ERROR,
						"Line %ld, File \"%s\", badargument: '%s' parameter missing, there must be a least one positional parameter",
						parser.lineNo(), parser.resourceName(),
						requestName(), RI_NULL);
				}
			}
		} else {
			parser.errHandler().handleError(
				RIE_MISSINGDATA, RIE_ERROR,
				"Line %ld, File \"%s\", badargument: '%s' argument %s missing, there must be a least one positional parameter",
				parser.lineNo(), parser.resourceName(),
				requestName(), "1 (nvertices array)", RI_NULL);
		}
}

void CPointsPolygonsRibRequest::operator()(IRibParserState &parser, CRibRequestData &request) const
{
	// PointsPolygons [ nvertices ] [ vertices ] <paramlist>
	if ( request.size() >= 2 ) {
		CRibParameter &p0 = request[0];
		CRibParameter &p1 = request[1];
		p0.convertFloatToInt();
		p1.convertFloatToInt();

		int i;
		RtInt npolys=0, *nverts=0, *verts=0;
		RtInt *pnverts=0, *pverts=0;

		// The number of polygons
		if ( p0.typeID() == BASICTYPE_INTEGER && p0.isArray() ) {
			npolys = (RtInt)p0.getCard();
			pnverts = (RtInt *)p0.getValue();
		} else {
			parser.errHandler().handleError(
				RIE_CONSISTENCY, RIE_ERROR,
				"Line %ld, File \"%s\", badargument: '%s' argument %s contains ni integers or is not an array",
				p0.lineNo(), parser.resourceName(),
				requestName(), "1 [ nvertices ]", RI_NULL);
		}
		if ( npolys == 0 || pnverts == 0 ) // Nothing to do
			return;

		if ( p1.typeID() == BASICTYPE_INTEGER && p1.isArray() ) {
			pverts = (RtInt *)p1.getValue();
		} else {
			parser.errHandler().handleError(
				RIE_CONSISTENCY, RIE_ERROR,
				"Line %ld, File \"%s\", badargument: '%s' argument %s contains no integers or is not an array",
				p1.lineNo(), parser.resourceName(),
				requestName(), "2 [ vertices ]", RI_NULL);
		}
		if ( pverts == 0 ) // Nothing to do
			return;

		// The number of vertices
		RtInt i_nverts;
		std::vector<RtInt> nvertsVect;
		nvertsVect.reserve(npolys);
		int sumverts = 0;
		for ( i = 0; i < npolys; ++i ) {
			i_nverts = pnverts[i];
			nvertsVect.push_back(i_nverts);
			sumverts += i_nverts;
		}
		if ( (int)nvertsVect.size() != npolys ) {
			// not all values are defined
			parser.errHandler().handleError(
				RIE_BUG, RIE_ERROR,
				"Line %ld, File \"%s\", badargument: '%s' argument %s, couldn't store all values",
				p0.lineNo(), parser.resourceName(),
				requestName(), "1 [ nvertices ]", RI_NULL);
			return;
		}
		nverts = &nvertsVect[0];

		// Test the size of the vertices array
		if ( sumverts < (RtInt)p1.getCard() ) {
			parser.errHandler().handleError(
				RIE_CONSISTENCY, RIE_ERROR,
				"Line %ld, File \"%s\", badargument: '%s' argument %s, invalid number of elements",
				p1.lineNo(), parser.resourceName(),
				requestName(), "2 [ vertices ]", RI_NULL);
			return;
		}
		if ( sumverts > (RtInt)p1.getCard() ) {
			parser.errHandler().handleError(
				RIE_CONSISTENCY, RIE_WARNING,
				"Line %ld, File \"%s\", badargument: '%s' argument %s, additional elements are ignored",
				p1.lineNo(), parser.resourceName(),
				requestName(), "2 [ vertices ]", RI_NULL);
		}

		// The vertices indexes
		RtInt i_verts;
		RtInt maxVertex=0;
		std::vector<RtInt> vertsVect;
		vertsVect.reserve(sumverts);
		for ( i = 0; i < sumverts; ++i ) {
			i_verts = pverts[i];
			if ( i_verts > maxVertex ) {
				maxVertex = i_verts;
			}
			vertsVect.push_back(i_verts);
		}
		if ( (int)vertsVect.size() != sumverts ) {
			// not all values are defined
			parser.errHandler().handleError(
				RIE_BUG, RIE_ERROR,
				"Line %ld, File \"%s\", badargument: '%s' argument %s, couldn't store all values",
				p1.lineNo(), parser.resourceName(),
				requestName(), "2 [ vertices ]", RI_NULL);
			return;
		}
		verts = &vertsVect[0];
		CPointsPolygonsClasses p(npolys, nverts, verts);
		int n = request.getTokenList(2, p);
		if ( n >= 0 ) {
			int nvertices = request.numVertices(2, n);
			if ( nvertices > 0 ) {
				parser.ribFilter().pointsPolygonsV(
					npolys, nverts, verts,
					n, request.tokenList(), request.valueList());
			} else {
				parser.errHandler().handleError(
					RIE_MISSINGDATA, RIE_ERROR,
					"Line %ld, File \"%s\", badargument: '%s' parameter missing, there must be a least one positional parameter",
					parser.lineNo(), parser.resourceName(),
					requestName(), RI_NULL);
			}
		}
	} else {
		parser.errHandler().handleError(
			RIE_MISSINGDATA, RIE_ERROR,
			"Line %ld, File \"%s\", badargument: '%s' arguments %s missing, there must be a least one positional parameter",
			parser.lineNo(), parser.resourceName(),
			requestName(), "(nveritces array, vertices array)", RI_NULL);
	}
}

void CPointsGeneralPolygonsRibRequest::operator()(IRibParserState &parser, CRibRequestData &request) const
{
	// PointsGeneralPolygons [ nloops ] [ nvertices ] [ vertices ] <paramlist>
	if ( request.size() >= 3 ) {
		CRibParameter &p0 = request[0];
		CRibParameter &p1 = request[1];
		CRibParameter &p2 = request[2];

		p0.convertFloatToInt();
		p1.convertFloatToInt();
		p2.convertFloatToInt();

		int i;
		RtInt npolys=0, *nloops=0, *nverts=0, *verts=0;
		RtInt *pnloops=0, *pnverts=0, *pverts=0;

		// The number of polygons
		if ( p0.typeID() == BASICTYPE_INTEGER && p0.isArray() ) {
			npolys = (RtInt)p0.getCard();
			pnloops = (RtInt *)p0.getValue();
		} else {
			parser.errHandler().handleError(
				RIE_CONSISTENCY, RIE_ERROR,
				"Line %ld, File \"%s\", badargument: '%s' argument %s contains no integers or is not an array",
				p0.lineNo(), parser.resourceName(),
				requestName(), "1 [ nloops ]", RI_NULL);
		}
		if ( npolys == 0 || pnloops == NULL ) // Nothing to do
			return;

		if ( p1.typeID() == BASICTYPE_INTEGER && p1.isArray() ) {
			pnverts = (RtInt *)p1.getValue();
		} else {
			parser.errHandler().handleError(
				RIE_CONSISTENCY, RIE_ERROR,
				"Line %ld, File \"%s\", badargument: '%s' argument %s contains no integers or is not an array",
				p1.lineNo(), parser.resourceName(),
				requestName(), "2 [ nvertices ]", RI_NULL);
		}
		if ( pnverts == NULL ) // Nothing to do
			return;

		if ( p2.typeID() == BASICTYPE_INTEGER && p2.isArray() ) {
			pverts = (RtInt *)p2.getValue();
		} else {
			parser.errHandler().handleError(
				RIE_CONSISTENCY, RIE_ERROR,
				"Line %ld, File \"%s\", badargument: '%s' argument %s contains no integers or is not an array",
				p2.lineNo(), parser.resourceName(),
				requestName(), "3 [ vertices ]", RI_NULL);
		}
		if ( pverts == NULL ) // Nothing to do
			return;

		// The number of loops
		RtInt i_nloops;
		std::vector<RtInt> nloopsVect;
		nloopsVect.reserve(npolys);
		int sumloops = 0;
		for ( i = 0; i < npolys; ++i ) {
			i_nloops = (RtInt)pnloops[i];
			nloopsVect.push_back(i_nloops);
			sumloops += i_nloops;
		}
		if ( (int)nloopsVect.size() != npolys ) {
			// not all values are defined
			parser.errHandler().handleError(
				RIE_BUG, RIE_ERROR,
				"Line %ld, File \"%s\", badargument: '%s' argument %s, couldn't store all values",
				p0.lineNo(), parser.resourceName(),
				requestName(), "1 [ nloops ]", RI_NULL);
			return;
		}
		nloops = &nloopsVect[0];

		// The number of vertices
		RtInt i_nverts;
		std::vector<RtInt> nvertsVect;
		nvertsVect.reserve(npolys);
		int sumverts = 0;
		for ( i = 0; i < sumloops; ++i ) {
			i_nverts = pnverts[i];
			nvertsVect.push_back(i_nverts);
			sumverts += i_nverts;
		}
		if ( (int)nvertsVect.size() != sumloops ) {
			// not all values are defined
			parser.errHandler().handleError(
				RIE_BUG, RIE_ERROR,
				"Line %ld, File \"%s\", badargument: '%s' argument %s, couldn't store all values",
				p1.lineNo(), parser.resourceName(),
				requestName(), "2 [ nvertices ]", RI_NULL);
			return;
		}
		nverts = &nvertsVect[0];

		// The vertices indexes
		RtInt i_verts;
		std::vector<RtInt> vertsVect;
		vertsVect.reserve(sumverts);
		for ( i = 0; i < sumverts; ++i ) {
			i_verts = pverts[i];
			vertsVect.push_back(i_verts);
		}
		if ( (int)vertsVect.size() != sumverts ) {
			// not all values are defined
			parser.errHandler().handleError(
				RIE_BUG, RIE_ERROR,
				"Line %ld, File \"%s\", badargument: '%s' argument %s, couldn't store all values",
				p2.lineNo(), parser.resourceName(),
				requestName(), "3 [ vertices ]", RI_NULL);
			return;
		}
		verts = &vertsVect[0];

		CPointsGeneralPolygonsClasses p(npolys, nloops, nverts, verts);
		int n = request.getTokenList(3, p);
		if ( n >= 0 ) {
			int nvertices = request.numVertices(3, n);
			if ( nvertices > 0 ) {
				parser.ribFilter().pointsGeneralPolygonsV(
					npolys, nloops, nverts, verts,
					n, request.tokenList(), request.valueList());
			} else {
				parser.errHandler().handleError(
					RIE_MISSINGDATA, RIE_ERROR,
					"Line %ld, File \"%s\", badargument: '%s' parameter missing, there must be a least one positional parameter",
					parser.lineNo(), parser.resourceName(),
					requestName(), RI_NULL);
			}
		}
	} else {
		parser.errHandler().handleError(
			RIE_MISSINGDATA, RIE_ERROR,
			"Line %ld, File \"%s\", badargument: '%s' arguments %s missing, there must be a least one positional parameter",
			parser.lineNo(), parser.resourceName(),
			requestName(), "(nloops array, nveritces array, vertices array)", RI_NULL);
	}
}

void CPatchRibRequest::operator()(IRibParserState &parser, CRibRequestData &request) const
{
	// Patch type <paramlist>
	if ( request.size() >= 1 ) {
		CRibParameter &p0 = request[0];

		const char *type;
		if ( p0.getString(type) ) {
			bool isBilinear = !strcmp(type, RI_BILINEAR);
			bool isBicubic = !strcmp(type, RI_BICUBIC);
			if ( !(isBilinear || isBicubic) ) {
				parser.errHandler().handleError(
					RIE_CONSISTENCY, RIE_ERROR,
					"Line %ld, File \"%s\", badargument: '%s' argument %s==\"%s\" must be either \"bicubic\" or \"bilinear\"",
					p0.lineNo(), parser.resourceName(),
					requestName(), "1 (type)", type, RI_NULL);
				return;
			}
			CPatchClasses p(type);
			int n = request.getTokenList(1, p);
			// int nvertices = request.numVertices(1, n);
			// if ( nvertices > 0 ) {
				if ( n > 0 ) {
					parser.ribFilter().patchV(type, n, request.tokenList(), request.valueList());
				} else {
					parser.ribFilter().patchV(type, 0, 0, 0);
				}
			// } else {
			// 	parser.errHandler().handleError(
			// 		RIE_MISSINGDATA, RIE_ERROR,
			// 		"Line %ld, File \"%s\", badargument: '%s' parameter missing, there must be a least one positional parameter",
			// 		parser.lineNo(), parser.resourceName(),
			// 		requestName(), RI_NULL);
			// }
		} else {
			parser.errHandler().handleError(
				RIE_CONSISTENCY, RIE_ERROR,
				"Line %ld, File \"%s\", badargument: '%s' argument %s is not a string",
				p0.lineNo(), parser.resourceName(),
				requestName(), "1 (type)", RI_NULL);
		}
	} else {
		parser.errHandler().handleError(
			RIE_MISSINGDATA, RIE_ERROR,
			"Line %ld, File \"%s\", badargument: '%s' argument %s missing",
			parser.lineNo(), parser.resourceName(),
			requestName(), "(type)", RI_NULL);
	}
}

void CPatchMeshRibRequest::operator()(IRibParserState &parser, CRibRequestData &request) const
{
		// PatchMesh type nu uwrap nv vwrap <paramlist>
		if ( request.size() >= 5 ) {
			CRibParameter &p0 = request[0];
			CRibParameter &p1 = request[1];
			CRibParameter &p2 = request[2];
			CRibParameter &p3 = request[3];
			CRibParameter &p4 = request[4];

			RtInt nu, nv;
			const char *type, *uwrap, *vwrap;
			bool b0 = p0.getString(type),
				 b1 = p1.getInt(nu), b2 = p2.getString(uwrap),
				 b3 = p3.getInt(nv), b4 = p4.getString(vwrap);

			if ( b0 && b1 && b2 && b3 && b4 ) {
				bool correct = true;

				if ( strcmp(uwrap, RI_PERIODIC) && strcmp(uwrap, RI_NONPERIODIC) ) {
					parser.errHandler().handleError(
						RIE_CONSISTENCY, RIE_ERROR,
						"Line %ld, File \"%s\", badargument: '%s' argument %s==\"%s\" must be \"periodic\" or \"nonperiodic\"",
						p2.lineNo(), parser.resourceName(),
						requestName(), "3 (uwrap)", uwrap, RI_NULL
						);
					correct = false;
				}

				if ( strcmp(vwrap, RI_PERIODIC) && strcmp(vwrap, RI_NONPERIODIC) ) {
					parser.errHandler().handleError(
						RIE_CONSISTENCY, RIE_ERROR,
						"Line %ld, File \"%s\", badargument: '%s' argument %s==\"%s\" must be \"periodic\" or \"nonperiodic\"",
						p4.lineNo(), parser.resourceName(),
						requestName(), "5 (vwrap)", uwrap, RI_NULL
						);
					correct = false;
				}

				if ( strcmp(type, RI_BILINEAR) && strcmp(type, RI_BICUBIC) ) {
					parser.errHandler().handleError(
						RIE_CONSISTENCY, RIE_ERROR,
						"Line %ld, File \"%s\", badargument: '%s' argument %s==\"%s\" must be either \"bicubic\" or \"bilinear\"",
						p0.lineNo(), parser.resourceName(),
						requestName(), "1 (type)", type, RI_NULL);
					correct = false;
				}

				if ( !correct ) {
					return;
				}

				CPatchMeshClasses p(type, nu, parser.renderState().attributes().uStep(), uwrap, nv, parser.renderState().attributes().vStep(), vwrap);
				int n = request.getTokenList(5, p);
				if ( n > 0 )
					parser.ribFilter().patchMeshV(
						type, nu, uwrap, nv, vwrap,
						n, request.tokenList(), request.valueList());
				else
					parser.ribFilter().patchMeshV(
						type, nu, uwrap, nv, vwrap,
						0, 0, 0);
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
						requestName(), "2 (nu)", RI_NULL);
				}
				if ( !b2 ) {
					parser.errHandler().handleError(
						RIE_CONSISTENCY, RIE_ERROR,
						"Line %ld, File \"%s\", badargument: '%s' argument %s is not a string",
						p2.lineNo(), parser.resourceName(),
						requestName(), "3 (uwrap)", RI_NULL);
				}
				if ( !b3 ) {
					parser.errHandler().handleError(
						RIE_CONSISTENCY, RIE_ERROR,
						"Line %ld, File \"%s\", badargument: '%s' argument %s is not numeric",
						p3.lineNo(), parser.resourceName(),
						requestName(), "4 (nv)", RI_NULL);
				}
				if ( !b4 ) {
					parser.errHandler().handleError(
						RIE_CONSISTENCY, RIE_ERROR,
						"Line %ld, File \"%s\", badargument: '%s' argument %s is not a string",
						p4.lineNo(), parser.resourceName(),
						requestName(), "5 (vwrap)", RI_NULL);
				}
			}
		} else {
			parser.errHandler().handleError(
				RIE_MISSINGDATA, RIE_ERROR,
				"Line %ld, File \"%s\", badargument: '%s' arguments %s missing",
				parser.lineNo(), parser.resourceName(),
				requestName(), "(type, nu, uwrap, nv, vwrap)", RI_NULL);
		}
}

void CNuPatchRibRequest::operator()(IRibParserState &parser, CRibRequestData &request) const
{
	// NuPatch nu uorder [ uknot ] umin umax nv vorder [ vknot ] vmin vmax <paramlist>
	if ( request.size() >= 10 ) {
		CRibParameter &p0 = request[0];
		CRibParameter &p1 = request[1];
		CRibParameter &p2 = request[2];
		CRibParameter &p3 = request[3];
		CRibParameter &p4 = request[4];
		CRibParameter &p5 = request[5];
		CRibParameter &p6 = request[6];
		CRibParameter &p7 = request[7];
		CRibParameter &p8 = request[8];
		CRibParameter &p9 = request[9];

		p2.convertIntToFloat();	// uknot
		p7.convertIntToFloat();	// vknot

		RtInt nu, uorder, nv, vorder;
		RtFloat *uknot=0, umin, umax, *vknot=NULL, vmin, vmax;

		if ( p2.isArray() && p2.typeID() == BASICTYPE_FLOAT ) {
			uknot = (RtFloat *)p2.getValue();
		}
		// else error message !b2 below

		if ( p7.isArray() && p7.typeID() == BASICTYPE_FLOAT ) {
			vknot = (RtFloat *)p7.getValue();
		}
		// else error message !b7 below

		bool b0=p0.getInt(nu), b1=p1.getInt(uorder), b2=uknot!=0, b3=p3.getFloat(umin), b4=p4.getFloat(umax), b5=p5.getInt(nv), b6=p6.getInt(vorder), b7=vknot!=0, b8=p8.getFloat(vmin), b9=p9.getFloat(vmax);

		if ( b0 && b1 && b2 && b3 && b4 && b5 && b6 && b7 && b8 && b9 ) {
			bool correct = true;
			if ( (RtInt)p2.getCard() < nu+uorder ) {
				parser.errHandler().handleError(
					RIE_CONSISTENCY, RIE_ERROR,
					"Line %ld, File \"%s\", badargument: '%s' argument %s, the number of knots must be nu+uorder",
					p2.lineNo(), parser.resourceName(),
					requestName(), "3 (uknot)", RI_NULL
					);
				correct = false;
			}
			if ( (RtInt)p2.getCard() > nu+uorder ) {
				parser.errHandler().handleError(
					RIE_CONSISTENCY, RIE_WARNING,
					"Line %ld, File \"%s\", badargument: '%s' argument %s, the number of knots must be nu+uorder, additional knots are ignored",
					p2.lineNo(), parser.resourceName(),
					requestName(), "3 (uknot)", RI_NULL
					);
			}

			if ( (RtInt)p7.getCard() < nv+vorder ) {
				parser.errHandler().handleError(
					RIE_CONSISTENCY, RIE_ERROR,
					"Line %ld, File \"%s\", badargument: '%s' argument %s, the number of knots must be nv+vorder",
					p7.lineNo(), parser.resourceName(),
					requestName(), "8 (vknot)", RI_NULL
					);
				correct = false;
			}
			if ( (RtInt)p7.getCard() > nv+vorder ) {
				parser.errHandler().handleError(
					RIE_CONSISTENCY, RIE_WARNING,
					"Line %ld, File \"%s\", badargument: '%s' argument %s, the number of knots must be nv+vorder, additional knots are ignored",
					p7.lineNo(), parser.resourceName(),
					requestName(), "8 (vknot)", RI_NULL
					);
			}

			CNuPatchClasses p(nu, uorder, nv, vorder);
			int n = request.getTokenList(10, p);

			if ( !correct )
				return;

			if ( n > 0 ) {
				parser.ribFilter().nuPatchV(
					nu, uorder, uknot, umin, umax, nv, vorder, vknot, vmin, vmax,
					n, request.tokenList(), request.valueList()
				);
			} else {
				parser.ribFilter().nuPatchV(
					nu, uorder, uknot, umin, umax, nv, vorder, vknot, vmin, vmax,
					0, 0, 0
				);
			}
		} else {
			if ( !b0 ) {
				parser.errHandler().handleError(
					RIE_CONSISTENCY, RIE_ERROR,
					"Line %ld, File \"%s\", badargument: '%s' argument %s is not numeric",
					p0.lineNo(), parser.resourceName(),
					requestName(), "1 (nu)", RI_NULL);
			}
			if ( !b1 ) {
				parser.errHandler().handleError(
					RIE_CONSISTENCY, RIE_ERROR,
					"Line %ld, File \"%s\", badargument: '%s' argument %s is not numeric",
					p1.lineNo(), parser.resourceName(),
					requestName(), "2 (uorder)", RI_NULL);
			}
			if ( !b2 ) {
				parser.errHandler().handleError(
					RIE_CONSISTENCY, RIE_ERROR,
					"Line %ld, File \"%s\", badargument: '%s' argument %s is not numeric or not an array",
					p2.lineNo(), parser.resourceName(),
					requestName(), "3 (uknot)", RI_NULL);
			}
			if ( !b3 ) {
				parser.errHandler().handleError(
					RIE_CONSISTENCY, RIE_ERROR,
					"Line %ld, File \"%s\", badargument: '%s' argument %s is not numeric",
					p3.lineNo(), parser.resourceName(),
					requestName(), "4 (umin)", RI_NULL);
			}
			if ( !b4 ) {
				parser.errHandler().handleError(
					RIE_CONSISTENCY, RIE_ERROR,
					"Line %ld, File \"%s\", badargument: '%s' argument %s is not numeric",
					p4.lineNo(), parser.resourceName(),
					requestName(), "5 (umax)", RI_NULL);
			}
			if ( !b5 ) {
				parser.errHandler().handleError(
					RIE_CONSISTENCY, RIE_ERROR,
					"Line %ld, File \"%s\", badargument: '%s' argument %s is not numeric",
					p5.lineNo(), parser.resourceName(),
					requestName(), "6 (nv)", RI_NULL);
			}
			if ( !b6 ) {
				parser.errHandler().handleError(
					RIE_CONSISTENCY, RIE_ERROR,
					"Line %ld, File \"%s\", badargument: '%s' argument %s is not numeric",
					p6.lineNo(), parser.resourceName(),
					requestName(), "7 (vorder)", RI_NULL);
			}
			if ( !b7 ) {
				parser.errHandler().handleError(
					RIE_CONSISTENCY, RIE_ERROR,
					"Line %ld, File \"%s\", badargument: '%s' argument %s is not numeric or not an array",
					p7.lineNo(), parser.resourceName(),
					requestName(), "8 (vknot)", RI_NULL);
			}
			if ( !b8 ) {
				parser.errHandler().handleError(
					RIE_CONSISTENCY, RIE_ERROR,
					"Line %ld, File \"%s\", badargument: '%s' argument %s is not numeric",
					p8.lineNo(), parser.resourceName(),
					requestName(), "9 (vmin)", RI_NULL);
			}
			if ( !b9 ) {
				parser.errHandler().handleError(
					RIE_CONSISTENCY, RIE_ERROR,
					"Line %ld, File \"%s\", badargument: '%s' argument %s is not numeric",
					p9.lineNo(), parser.resourceName(),
					requestName(), "10 (vmax)", RI_NULL);
			}
		}
	} else {
		parser.errHandler().handleError(
			RIE_MISSINGDATA, RIE_ERROR,
			"Line %ld, File \"%s\", badargument: '%s' arguments %s missing",
			parser.lineNo(), parser.resourceName(),
			requestName(), "(nu, uorder, uknot, umin, umax, nv, vorder, vknot, vmin, vmax)", RI_NULL);
	}
}

void CSubdivisionMeshRibRequest::operator()(IRibParserState &parser, CRibRequestData &request) const
{
	// SubdivisionMesh scheme [ nvertices ] [ vertices ] [ tags ] [ nargs ] [ intargs ] [ floatargs ] <paramlist>
	// SubdivisionMesh scheme [ nvertices ] [ vertices ] <paramlist>
	if ( request.size() >= 3 ) {
		const char *scheme=0;
		RtInt nfaces=0, *nvertices=0, *vertices=0, ntags=0;
		const char **tags=0;
		RtInt *nargs=0, *intargs=0;
		RtFloat *floatargs=0;
		bool correct = true;
		int nparameters = 3;
		RtInt nintargs=-1, nfloatargs=-1;

		CRibParameter &p0 = request[0]; // scheme
		CRibParameter &p1 = request[1]; // *nvertices
		CRibParameter &p2 = request[2]; // *vertices
		p1.convertFloatToInt();
		p2.convertFloatToInt();

		if ( request.size() >= 6 ) {
			CRibParameter &p3 = request[3]; // *tags

			if ( p3.typeID() == BASICTYPE_STRING && !p3.isArray() ) {
				nparameters = 3;
			} else {
				nparameters = 7;

				CRibParameter &p4 = request[4]; // *nargs
				CRibParameter &p5 = request[5]; // *intargs
				CRibParameter &p6 = request[6]; // *floatargs
				p4.convertFloatToInt();
				p5.convertFloatToInt();
				p6.convertIntToFloat();

				ntags = (RtInt)p3.getCard();
				if ( ntags != 0 ) {
					if ( p3.typeID() != BASICTYPE_STRING || !p3.isArray() ) {
						parser.errHandler().handleError(
							RIE_CONSISTENCY, RIE_ERROR,
							"Line %ld, File \"%s\", badargument: '%s' argument %s contains no strings or is not an array",
							p3.lineNo(), parser.resourceName(),
							requestName(), "4 (tags)", RI_NULL);
						correct = false;
					} else {
						tags = (const char **)p3.getValue();
					}
				}

				if ( p4.getCard() != 0 ) {
					if ( p4.typeID() != BASICTYPE_INTEGER || !p4.isArray() ) {
						parser.errHandler().handleError(
							RIE_CONSISTENCY, RIE_ERROR,
							"Line %ld, File \"%s\", badargument: '%s' argument %s contains no integers or is not an array",
							p4.lineNo(), parser.resourceName(),
							requestName(), "5 (nargs)", RI_NULL);
						correct = false;
					} else {
						nargs = (RtInt *)p4.getValue();
					}
				}

				if ( (RtInt)p4.getCard() < ntags*2 ) {
					parser.errHandler().handleError(
						RIE_CONSISTENCY, RIE_ERROR,
						"Line %ld, File \"%s\", badargument: '%s' argument %s is not of the right size: %d expected, but size is %d",
						p4.lineNo(), parser.resourceName(),
						requestName(), "5 (nargs)", (int)ntags*2, (int)p4.getCard(), RI_NULL);
					correct = false;
				}
				if ( (RtInt)p4.getCard() > ntags*2 ) {
					parser.errHandler().handleError(
						RIE_CONSISTENCY, RIE_WARNING,
						"Line %ld, File \"%s\", badargument: '%s' argument %s is not of the right size: %d expected, but size is %d, additional values are ignored",
						p4.lineNo(), parser.resourceName(),
						requestName(), "5 (nargs)", (int)ntags*2, (int)p4.getCard(), RI_NULL);
				}

				if ( (RtInt)p4.getCard() >= ntags*2 ) {
					int i;
					nintargs = nfloatargs = 0;
					for ( i=0; i <ntags*2; i+=2 ) {
						nintargs += nargs[i];
						nfloatargs += nargs[i+1];
					}
				}

				if ( p5.getCard() != 0 ) {
					if ( p5.typeID() != BASICTYPE_INTEGER || !p5.isArray() ) {
						parser.errHandler().handleError(
							RIE_CONSISTENCY, RIE_ERROR,
							"Line %ld, File \"%s\", badargument: '%s' argument %s contains no integers or is not an array",
							p5.lineNo(), parser.resourceName(),
							requestName(), "6 (intargs)", RI_NULL);
						correct = false;
					} else {
						intargs = (RtInt *)p5.getValue();
					}
				}

				if ( nintargs >= 0 ) {
					if ( (RtInt)p5.getCard() < nintargs ) {
						parser.errHandler().handleError(
							RIE_CONSISTENCY, RIE_ERROR,
							"Line %ld, File \"%s\", badargument: '%s' argument %s is not of the right size: %d expected, but size is %d",
							p5.lineNo(), parser.resourceName(),
							requestName(), "6 (intargs)", (int)nintargs, (int)p5.getCard(), RI_NULL);
						correct = false;
					}
					if ( (RtInt)p5.getCard() > nintargs ) {
						parser.errHandler().handleError(
							RIE_CONSISTENCY, RIE_WARNING,
							"Line %ld, File \"%s\", badargument: '%s' argument %s is not of the right size: %d expected, but size is %d, additional values are ignored",
							p5.lineNo(), parser.resourceName(),
							requestName(), "6 (intargs)", (int)nintargs, (int)p5.getCard(), RI_NULL);
					}
				}

				if ( p6.getCard() != 0 ) {
					if ( p6.typeID() != BASICTYPE_FLOAT || !p6.isArray() ) {
						parser.errHandler().handleError(
							RIE_CONSISTENCY, RIE_ERROR,
							"Line %ld, File \"%s\", badargument: '%s' argument %s contains no numerics or is not an array",
							p6.lineNo(), parser.resourceName(),
							requestName(), "7 (floatargs)", RI_NULL);
						correct = false;
					} else {
						floatargs = (RtFloat *)p6.getValue();
					}
				}

				if ( nfloatargs >= 0 ) {
					if ( (RtInt)p6.getCard() < nfloatargs ) {
						parser.errHandler().handleError(
							RIE_CONSISTENCY, RIE_ERROR,
							"Line %ld, File \"%s\", badargument: '%s' argument %s is not of the right size: %d expected, but size is %d",
							p6.lineNo(), parser.resourceName(),
							requestName(), "7 (floatargs)", (int)nfloatargs, (int)p6.getCard(), RI_NULL);
						correct = false;
					}
					if ( (RtInt)p6.getCard() > nfloatargs ) {
						parser.errHandler().handleError(
							RIE_CONSISTENCY, RIE_WARNING,
							"Line %ld, File \"%s\", badargument: '%s' argument %s is not of the right size: %d expected, but size is %d, additional values are ignored",
							p6.lineNo(), parser.resourceName(),
							requestName(), "7 (floatargs)", (int)nfloatargs, (int)p6.getCard(), RI_NULL);
					}
				}

			}
		}

		if ( !p0.getString(scheme) ) {
			parser.errHandler().handleError(
				RIE_CONSISTENCY, RIE_ERROR,
				"Line %ld, File \"%s\", badargument: '%s' argument %s is not a string",
				p0.lineNo(), parser.resourceName(),
				requestName(), "1 (scheme)", RI_NULL);
			correct = false;
		}

		nfaces = (RtInt)p1.getCard();
		// if ( nfaces == 0 ) {
		//	m_ricb->handleError(RIE_CONSISTENCY, RIE_ERROR, "Line %ld, File \"%s\", badarray: 'SubdivisionMesh' argument 2 (nverices) has no values", p1.lineCount(), m_strFileName.c_str(), m_ri->RI_NULL);
		//	correct = false;
		// }

		if ( nfaces != 0 ) {
			if ( p1.typeID() != BASICTYPE_INTEGER || !p1.isArray() ) {
				parser.errHandler().handleError(
					RIE_CONSISTENCY, RIE_ERROR,
					"Line %ld, File \"%s\", badargument: '%s' argument %s contains no integers or is not an array",
					p1.lineNo(), parser.resourceName(),
					requestName(), "2 (nvertices)", RI_NULL);
				correct = false;
			} else {
				nvertices = (RtInt *)p1.getValue();
			}
		}

		if ( p2.getCard() != 0 ) {
			if ( p2.typeID() != BASICTYPE_INTEGER || !p2.isArray() ) {
				parser.errHandler().handleError(
					RIE_CONSISTENCY, RIE_ERROR,
					"Line %ld, File \"%s\", badargument: '%s' argument %s contains no integers or is not an array",
					p2.lineNo(), parser.resourceName(),
					requestName(), "3 (vertices)", RI_NULL);
				correct = false;
			} else {
				vertices = (RtInt *)p2.getValue();
			}
		}

		RtInt i;
		RtInt sumVertices=0;
		if ( nvertices ) {
			for ( i=0; i<nfaces; ++i ) {
				sumVertices += nvertices[i];
			}
		}

		if ( sumVertices > (RtInt)p2.getCard() ) {
			parser.errHandler().handleError(
				RIE_CONSISTENCY, RIE_ERROR,
				"Line %ld, File \"%s\", badargument: '%s' argument %s array has not enough vertex index numbers, expected %d, but size is %d",
				p2.lineNo(), parser.resourceName(),
				requestName(), "3 (vertices)", sumVertices, (RtInt)p2.getCard(), RI_NULL);
			correct = false;
		}
		if ( sumVertices < (RtInt)p2.getCard() ) {
			parser.errHandler().handleError(
				RIE_CONSISTENCY, RIE_WARNING,
				"Line %ld, File \"%s\", badargument: '%s' argument %s array has not enough vertex index numbers, expected %d, but size is %d, additional index numbers are ignored",
				p2.lineNo(), parser.resourceName(),
				requestName(), "3 (vertices)", sumVertices, (RtInt)p2.getCard(), RI_NULL);
		}

		CSubdivisionMeshClasses p(nfaces, nvertices, vertices);
		int n = request.getTokenList(nparameters, p);
		if ( correct ) {
			if ( n > 0 ) 
				parser.ribFilter().subdivisionMeshV(
					scheme, nfaces, nvertices, vertices, ntags, tags, nargs, intargs, floatargs,
					n, request.tokenList(), request.valueList()
				);
			else
				parser.ribFilter().subdivisionMeshV(
					scheme, nfaces, nvertices, vertices, ntags, tags, nargs, intargs, floatargs,
					0, 0, 0
				);
		}
	} else {
		parser.errHandler().handleError(
			RIE_MISSINGDATA, RIE_ERROR,
			"Line %ld, File \"%s\", badargument: '%s' arguments %s missing",
			parser.lineNo(), parser.resourceName(),
			requestName(), "(scheme, nvertices, vertices, tags, nargs, intargs, floatargs)", RI_NULL);
	}
}

void CHierarchicalSubdivisionMeshRibRequest::operator()(IRibParserState &parser, CRibRequestData &request) const
{
	// HierarchicalSubdivisionMesh scheme [ nvertices ] [ vertices ] [ tags ] [ nargs ] [ intargs ] [ floatargs ] [ strargs ] <paramlist>
	// HierarchicalSubdivisionMesh scheme [ nvertices ] [ vertices ] <paramlist>
	if ( request.size() >= 3 ) {
		const char *scheme=0;
		RtInt nfaces=0, *nvertices=0, *vertices=0, ntags=0;
		const char **tags=0;
		RtInt *nargs=0, *intargs=0;
		RtFloat *floatargs=0;
		RtToken *strargs=0;
		bool correct = true;
		int nparameters = 3;
		RtInt nintargs=-1, nfloatargs=-1, nstrargs=-1;

		CRibParameter &p0 = request[0]; // scheme
		CRibParameter &p1 = request[1]; // *nvertices
		CRibParameter &p2 = request[2]; // *vertices
		p1.convertFloatToInt();
		p2.convertFloatToInt();

		if ( request.size() >= 7 ) {
			CRibParameter &p3 = request[3]; // *tags

			if ( p3.typeID() == BASICTYPE_STRING && !p3.isArray() ) {
				nparameters = 3;
			} else {
				nparameters = 8;

				CRibParameter &p4 = request[4]; // *nargs
				CRibParameter &p5 = request[5]; // *intargs
				CRibParameter &p6 = request[6]; // *floatargs
				CRibParameter &p7 = request[7]; // *strargs
				p4.convertFloatToInt();
				p5.convertFloatToInt();
				p6.convertIntToFloat();

				ntags = (RtInt)p3.getCard();
				if ( ntags != 0 ) {
					if ( p3.typeID() != BASICTYPE_STRING || !p3.isArray() ) {
						parser.errHandler().handleError(
							RIE_CONSISTENCY, RIE_ERROR,
							"Line %ld, File \"%s\", badargument: '%s' argument %s contains no strings or is not an array",
							p3.lineNo(), parser.resourceName(),
							requestName(), "4 (tags)", RI_NULL);
						correct = false;
					} else {
						tags = (const char **)p3.getValue();
					}
				}

				if ( p4.getCard() != 0 ) {
					if ( p4.typeID() != BASICTYPE_INTEGER || !p4.isArray() ) {
						parser.errHandler().handleError(
							RIE_CONSISTENCY, RIE_ERROR,
							"Line %ld, File \"%s\", badargument: '%s' argument %s contains no integers or is not an array",
							p4.lineNo(), parser.resourceName(),
							requestName(), "5 (nargs)", RI_NULL);
						correct = false;
					} else {
						nargs = (RtInt *)p4.getValue();
					}
				}

				if ( (RtInt)p4.getCard() < ntags*3 ) {
					parser.errHandler().handleError(
						RIE_CONSISTENCY, RIE_ERROR,
						"Line %ld, File \"%s\", badargument: '%s' argument %s is not of the right size: %d expected, but size is %d",
						p4.lineNo(), parser.resourceName(),
						requestName(), "5 (nargs)", (int)ntags*3, (int)p4.getCard(), RI_NULL);
					correct = false;
				}
				if ( (RtInt)p4.getCard() > ntags*3 ) {
					parser.errHandler().handleError(
						RIE_CONSISTENCY, RIE_WARNING,
						"Line %ld, File \"%s\", badargument: '%s' argument %s is not of the right size: %d expected, but size is %d, additional values are ignored",
						p4.lineNo(), parser.resourceName(),
						requestName(), "5 (nargs)", (int)ntags*3, (int)p4.getCard(), RI_NULL);
				}

				if ( (RtInt)p4.getCard() >= ntags*3 ) {
					int i;
					nintargs = nfloatargs = 0;
					for ( i=0; i <ntags*3; i+=3 ) {
						nintargs += nargs[i];
						nfloatargs += nargs[i+1];
						nstrargs += nargs[i+2];
					}
				}

				if ( p5.getCard() != 0 ) {
					if ( p5.typeID() != BASICTYPE_INTEGER || !p5.isArray() ) {
						parser.errHandler().handleError(
							RIE_CONSISTENCY, RIE_ERROR,
							"Line %ld, File \"%s\", badargument: '%s' argument %s contains no integers or is not an array",
							p5.lineNo(), parser.resourceName(),
							requestName(), "6 (intargs)", RI_NULL);
						correct = false;
					} else {
						intargs = (RtInt *)p5.getValue();
					}
				}

				if ( nintargs >= 0 ) {
					if ( (RtInt)p5.getCard() < nintargs ) {
						parser.errHandler().handleError(
							RIE_CONSISTENCY, RIE_ERROR,
							"Line %ld, File \"%s\", badargument: '%s' argument %s is not of the right size: %d expected, but size is %d",
							p5.lineNo(), parser.resourceName(),
							requestName(), "6 (intargs)", (int)nintargs, (int)p5.getCard(), RI_NULL);
						correct = false;
					}
					if ( (RtInt)p5.getCard() > nintargs ) {
						parser.errHandler().handleError(
							RIE_CONSISTENCY, RIE_WARNING,
							"Line %ld, File \"%s\", badargument: '%s' argument %s is not of the right size: %d expected, but size is %d, additional values are ignored",
							p5.lineNo(), parser.resourceName(),
							requestName(), "6 (intargs)", (int)nintargs, (int)p5.getCard(), RI_NULL);
					}
				}

				if ( p6.getCard() != 0 ) {
					if ( p6.typeID() != BASICTYPE_FLOAT || !p6.isArray() ) {
						parser.errHandler().handleError(
							RIE_CONSISTENCY, RIE_ERROR,
							"Line %ld, File \"%s\", badargument: '%s' argument %s contains no numerics or is not an array",
							p6.lineNo(), parser.resourceName(),
							requestName(), "7 (floatargs)", RI_NULL);
						correct = false;
					} else {
						floatargs = (RtFloat *)p6.getValue();
					}
				}

				if ( nfloatargs >= 0 ) {
					if ( (RtInt)p6.getCard() < nfloatargs ) {
						parser.errHandler().handleError(
							RIE_CONSISTENCY, RIE_ERROR,
							"Line %ld, File \"%s\", badargument: '%s' argument %s is not of the right size: %d expected, but size is %d",
							p6.lineNo(), parser.resourceName(),
							requestName(), "7 (floatargs)", (int)nfloatargs, (int)p6.getCard(), RI_NULL);
						correct = false;
					}
					if ( (RtInt)p6.getCard() > nfloatargs ) {
						parser.errHandler().handleError(
							RIE_CONSISTENCY, RIE_WARNING,
							"Line %ld, File \"%s\", badargument: '%s' argument %s is not of the right size: %d expected, but size is %d, additional values are ignored",
							p6.lineNo(), parser.resourceName(),
							requestName(), "7 (floatargs)", (int)nfloatargs, (int)p6.getCard(), RI_NULL);
					}
				}

				if ( p7.getCard() != 0 ) {
					if ( p7.typeID() != BASICTYPE_STRING || !p7.isArray() ) {
						parser.errHandler().handleError(
							RIE_CONSISTENCY, RIE_ERROR,
							"Line %ld, File \"%s\", badargument: '%s' argument %s contains no strings or is not an array",
							p7.lineNo(), parser.resourceName(),
							requestName(), "8 (strargs)", RI_NULL);
						correct = false;
					} else {
						strargs = (RtToken *)p7.getValue();
					}
				}

				if ( nstrargs >= 0 ) {
					if ( (RtInt)p7.getCard() < nstrargs ) {
						parser.errHandler().handleError(
							RIE_CONSISTENCY, RIE_ERROR,
							"Line %ld, File \"%s\", badargument: '%s' argument %s is not of the right size: %d expected, but size is %d",
							p7.lineNo(), parser.resourceName(),
							requestName(), "8 (strargs)", (int)nstrargs, (int)p7.getCard(), RI_NULL);
						correct = false;
					}
					if ( (RtInt)p7.getCard() > nstrargs ) {
						parser.errHandler().handleError(
							RIE_CONSISTENCY, RIE_WARNING,
							"Line %ld, File \"%s\", badargument: '%s' argument %s is not of the right size: %d expected, but size is %d, additional values are ignored",
							p7.lineNo(), parser.resourceName(),
							requestName(), "8 (strargs)", (int)nstrargs, (int)p7.getCard(), RI_NULL);
					}
				}

			}
		}

		if ( !p0.getString(scheme) ) {
			parser.errHandler().handleError(
				RIE_CONSISTENCY, RIE_ERROR,
				"Line %ld, File \"%s\", badargument: '%s' argument %s is not a string",
				p0.lineNo(), parser.resourceName(),
				requestName(), "1 (scheme)", RI_NULL);
			correct = false;
		}

		nfaces = (RtInt)p1.getCard();
		// if ( nfaces == 0 ) {
		//	m_ricb->handleError(RIE_CONSISTENCY, RIE_ERROR, "Line %ld, File \"%s\", badarray: 'SubdivisionMesh' argument 2 (nverices) has no values", p1.lineCount(), m_strFileName.c_str(), m_ri->RI_NULL);
		//	correct = false;
		// }

		if ( nfaces != 0 ) {
			if ( p1.typeID() != BASICTYPE_INTEGER || !p1.isArray() ) {
				parser.errHandler().handleError(
					RIE_CONSISTENCY, RIE_ERROR,
					"Line %ld, File \"%s\", badargument: '%s' argument %s contains no integers or is not an array",
					p1.lineNo(), parser.resourceName(),
					requestName(), "2 (nvertices)", RI_NULL);
				correct = false;
			} else {
				nvertices = (RtInt *)p1.getValue();
			}
		}

		if ( p2.getCard() != 0 ) {
			if ( p2.typeID() != BASICTYPE_INTEGER || !p2.isArray() ) {
				parser.errHandler().handleError(
					RIE_CONSISTENCY, RIE_ERROR,
					"Line %ld, File \"%s\", badargument: '%s' argument %s contains no integers or is not an array",
					p2.lineNo(), parser.resourceName(),
					requestName(), "3 (vertices)", RI_NULL);
				correct = false;
			} else {
				vertices = (RtInt *)p2.getValue();
			}
		}

		RtInt i;
		RtInt sumVertices=0;
		if ( nvertices ) {
			for ( i=0; i<nfaces; ++i ) {
				sumVertices += nvertices[i];
			}
		}

		if ( sumVertices > (RtInt)p2.getCard() ) {
			parser.errHandler().handleError(
				RIE_CONSISTENCY, RIE_ERROR,
				"Line %ld, File \"%s\", badargument: '%s' argument %s array has not enough vertex index numbers, expected %d, but size is %d",
				p2.lineNo(), parser.resourceName(),
				requestName(), "3 (vertices)", sumVertices, (RtInt)p2.getCard(), RI_NULL);
			correct = false;
		}
		if ( sumVertices < (RtInt)p2.getCard() ) {
			parser.errHandler().handleError(
				RIE_CONSISTENCY, RIE_WARNING,
				"Line %ld, File \"%s\", badargument: '%s' argument %s array has not enough vertex index numbers, expected %d, but size is %d, additional index numbers are ignored",
				p2.lineNo(), parser.resourceName(),
				requestName(), "3 (vertices)", sumVertices, (RtInt)p2.getCard(), RI_NULL);
		}

		CHierarchicalSubdivisionMeshClasses p(nfaces, nvertices, vertices);
		int n = request.getTokenList(nparameters, p);
		if ( correct ) {
			if ( n > 0 ) 
				parser.ribFilter().hierarchicalSubdivisionMeshV(
					scheme, nfaces, nvertices, vertices, ntags, tags, nargs, intargs, floatargs, strargs,
					n, request.tokenList(), request.valueList()
				);
			else
				parser.ribFilter().hierarchicalSubdivisionMeshV(
					scheme, nfaces, nvertices, vertices, ntags, tags, nargs, intargs, floatargs, strargs,
					0, 0, 0
				);
		}
	} else {
		parser.errHandler().handleError(
			RIE_MISSINGDATA, RIE_ERROR,
			"Line %ld, File \"%s\", badargument: '%s' arguments %s missing",
			parser.lineNo(), parser.resourceName(),
			requestName(), "(scheme, nvertices, vertices, tags, nargs, intargs, floatargs, strargs)", RI_NULL);
	}
}

void CSphereRibRequest::operator()(IRibParserState &parser, CRibRequestData &request) const
{
	CQuadricClasses p;

	if ( request.size() >= 4 && !request[0].isArray() ) {
		// Sphere radius zmin zmax thetamax <paramlist>

		CRibParameter &p0 = request[0];
		CRibParameter &p1 = request[1];
		CRibParameter &p2 = request[2];
		CRibParameter &p3 = request[3];

		RtFloat radius, zmin, zmax, thetamax;
		bool b0 = p0.getFloat(radius),
		     b1 = p1.getFloat(zmin),
		     b2 = p2.getFloat(zmax),
			 b3 = p3.getFloat(thetamax);

		if ( b0 && b1 && b2 && b3 ) {
			int n = request.getTokenList(4, p);
			if ( n > 0 ) {
				parser.ribFilter().sphereV(
					radius, zmin, zmax, thetamax,
					n, request.tokenList(), request.valueList());
			} else {
				parser.ribFilter().sphereV(
					radius, zmin, zmax, thetamax,
					0, 0, 0);
			}
		} else {
			if ( !b0 ) {
				parser.errHandler().handleError(
					RIE_CONSISTENCY, RIE_ERROR,
					"Line %ld, File \"%s\", badargument: '%s' argument %s is not numeric",
					p0.lineNo(), parser.resourceName(), requestName(), "1 (radius)", RI_NULL);
			}
			if ( !b1 ) {
				parser.errHandler().handleError(
					RIE_CONSISTENCY, RIE_ERROR,
					"Line %ld, File \"%s\", badargument: '%s' argument %s is not numeric",
					p1.lineNo(), parser.resourceName(), requestName(), "2 (zmin)", RI_NULL);
			}
			if ( !b2 ) {
				parser.errHandler().handleError(
					RIE_CONSISTENCY, RIE_ERROR,
					"Line %ld, File \"%s\", badarray: '%s' argument %s is not numeric",
					p2.lineNo(), parser.resourceName(), requestName(), "3 (zmax)", RI_NULL);
			}
			if ( !b3 ) {
				parser.errHandler().handleError(
					RIE_CONSISTENCY, RIE_ERROR,
					"Line %ld, File \"%s\", badargument: '%s' argument %s is not numeric",
					p3.lineNo(), parser.resourceName(), requestName(), "4 (thetamax)", RI_NULL);
			}
		}

	} else if ( request.size() >= 1 && request[0].isArray() ) {
		// Sphere [ radius zmin zmax thetamax ] <paramlist>

		CRibParameter &p0 = request[0];
		p0.convertIntToFloat();
		bool correct = true;

		if ( p0.getCard() < 4 ) {
			parser.errHandler().handleError(
				RIE_MISSINGDATA, RIE_ERROR,
				"Line %ld, File \"%s\", badarray: '%s' arguments (%s) missing",
				p0.lineNo(), parser.resourceName(), requestName(), "radius, zmin, zmax, thetamax", RI_NULL);
			correct = false;
		}
		if ( p0.getCard() > 4 ) {
			parser.errHandler().handleError(
				RIE_CONSISTENCY, RIE_WARNING,
				"Line %ld, File \"%s\", badarray: '%s' additional arguments ignored",
				p0.lineNo(), parser.resourceName(), requestName(), RI_NULL);
		}
		if (  p0.typeID() != BASICTYPE_FLOAT ) {
			parser.errHandler().handleError(
				RIE_CONSISTENCY, RIE_ERROR, "Line %ld, File \"%s\", badarray: '%s' argument %s, not all elements are numeric",
				p0.lineNo(), parser.resourceName(), requestName(), "array 1", RI_NULL);
			correct = false;
		}

		int n = request.getTokenList(1, p);

		if ( !correct )
			return;

		RtFloat *vals = (RtFloat *)p0.getValue();
		if ( !vals ) {
			parser.errHandler().handleError(
				RIE_BUG, RIE_ERROR,
				"Line %ld, File \"%s\", badarray: '%s' could not store arguments",
				p0.lineNo(), parser.resourceName(), requestName(), RI_NULL);
			return;
		}

		RtFloat radius   = vals[0],
		        zmin     = vals[1],
		        zmax     = vals[2],
		        thetamax = vals[3];

		if ( n > 0 ) {
			parser.ribFilter().sphereV(
				radius, zmin, zmax, thetamax,
				n, request.tokenList(), request.valueList());
		} else {
			parser.ribFilter().sphereV(
				radius, zmin, zmax, thetamax,
				0, 0, 0);
		}
	} else {
		parser.errHandler().handleError(
			RIE_MISSINGDATA, RIE_ERROR,
			"Line %ld, File \"%s\", badargument: '%s' at least one of the arguments (%s) is missing",
			parser.lineNo(), parser.resourceName(), requestName(), "radius, zmin, zmax, thetamax", RI_NULL);
	}
}

void CConeRibRequest::operator()(IRibParserState &parser, CRibRequestData &request) const
{
	CQuadricClasses p;

	if ( request.size() >= 3 && !request[0].isArray() ) {
		// Cone height radius thetamax <paramlist>

		CRibParameter &p0 = request[0];
		CRibParameter &p1 = request[1];
		CRibParameter &p2 = request[2];

		RtFloat height, radius, thetamax;
		bool b0 = p0.getFloat(height),
		     b1 = p1.getFloat(radius),
			 b2 = p2.getFloat(thetamax);

		if ( b0 && b1 && b2 ) {
			int n = request.getTokenList(3, p);
			if ( n > 0 ) {
				parser.ribFilter().coneV(height, radius, thetamax,
					n, request.tokenList(), request.valueList());
			} else {
				parser.ribFilter().coneV(height, radius, thetamax,
					0, 0, 0);
			}
		} else {
			if ( !b0 ) {
				parser.errHandler().handleError(
					RIE_CONSISTENCY, RIE_ERROR,
					"Line %ld, File \"%s\", badargument: '%s' argument %s is not numeric",
					p1.lineNo(), parser.resourceName(), requestName(), "1 (height)", RI_NULL);
			}
			if ( !b1 ) {
				parser.errHandler().handleError(
					RIE_CONSISTENCY, RIE_ERROR,
					"Line %ld, File \"%s\", badargument: '%s' argument %s is not numeric",
					p1.lineNo(), parser.resourceName(), requestName(), "2 (radius)", RI_NULL);
			}
			if ( !b2 ) {
				parser.errHandler().handleError(
					RIE_CONSISTENCY, RIE_ERROR,
					"Line %ld, File \"%s\", badargument: '%s' argument %s is not numeric",
					p2.lineNo(), parser.resourceName(), requestName(), "3 (thetamax)", RI_NULL);
			}
		}

	} else if ( request.size() >= 1 && request[0].isArray() ) {
		// Cone [ height radius thetamax ] <paramlist>

		CRibParameter &p0 = request[0];
		p0.convertIntToFloat();
		bool correct = true;

		if ( p0.getCard() < 3 ) {
			parser.errHandler().handleError(
				RIE_MISSINGDATA, RIE_ERROR,
				"Line %ld, File \"%s\", badarray: '%s' arguments %s missing",
				p0.lineNo(), parser.resourceName(), requestName(), "(height, radius, thetamax)", RI_NULL);
			correct = false;
		}
		if ( p0.getCard() > 3 ) {
			parser.errHandler().handleError(
				RIE_CONSISTENCY, RIE_WARNING,
				"Line %ld, File \"%s\", badarray: '%s' additional arguments ignored",
				p0.lineNo(), parser.resourceName(), requestName(), RI_NULL);
		}
		if (  p0.typeID() != BASICTYPE_FLOAT ) {
			parser.errHandler().handleError(
				RIE_CONSISTENCY, RIE_ERROR, "Line %ld, File \"%s\", badarray: '%s' argument %s, not all elements are numeric",
				p0.lineNo(), parser.resourceName(), requestName(), "array 1", RI_NULL);
			correct = false;
		}

		int n = request.getTokenList(1, p);

		if ( !correct )
			return;

		RtFloat *vals = (RtFloat *)p0.getValue();
		if ( !vals ) {
			parser.errHandler().handleError(
				RIE_BUG, RIE_ERROR,
				"Line %ld, File \"%s\", badarray: '%s' could not store arguments",
				p0.lineNo(), parser.resourceName(), requestName(), RI_NULL);
			return;
		}

		RtFloat height   = vals[0],
		        radius   = vals[1],
		        thetamax = vals[2];

		if ( n > 0 ) {
			parser.ribFilter().coneV(
				height, radius, thetamax,
				n, request.tokenList(), request.valueList());
		} else {
			parser.ribFilter().coneV(
				height, radius, thetamax,
				0, 0, 0);
		}
	} else {
		parser.errHandler().handleError(
			RIE_MISSINGDATA, RIE_ERROR,
			"Line %ld, File \"%s\", badargument: '%s' at least one of the arguments (%s) is missing",
			parser.lineNo(), parser.resourceName(), requestName(), "radius, zmin, zmax, thetamax", RI_NULL);
	}
}


void CCylinderRibRequest::operator()(IRibParserState &parser, CRibRequestData &request) const
{
	CQuadricClasses p;

	if ( request.size() >= 4 && !request[0].isArray() ) {
		// Cylinder radius zmin zmax thetamax <paramlist>

		CRibParameter &p0 = request[0];
		CRibParameter &p1 = request[1];
		CRibParameter &p2 = request[2];
		CRibParameter &p3 = request[3];

		RtFloat radius, zmin, zmax, thetamax;
		bool b0 = p0.getFloat(radius),
		     b1 = p1.getFloat(zmin),
		     b2 = p2.getFloat(zmax),
			 b3 = p3.getFloat(thetamax);

		if ( b0 && b1 && b2 && b3 ) {
			int n = request.getTokenList(4, p);
			if ( n > 0 ) {
				parser.ribFilter().cylinderV(
					radius, zmin, zmax, thetamax,
					n, request.tokenList(), request.valueList());
			} else {
				parser.ribFilter().cylinderV(
					radius, zmin, zmax, thetamax,
					0, 0, 0);
			}
		} else {
			if ( !b0 ) {
				parser.errHandler().handleError(
					RIE_CONSISTENCY, RIE_ERROR,
					"Line %ld, File \"%s\", badargument: '%s' argument %s is not numeric",
					p0.lineNo(), parser.resourceName(), requestName(), "1 (radius)", RI_NULL);
			}
			if ( !b1 ) {
				parser.errHandler().handleError(
					RIE_CONSISTENCY, RIE_ERROR,
					"Line %ld, File \"%s\", badargument: '%s' argument %s is not numeric",
					p1.lineNo(), parser.resourceName(), requestName(), "2 (zmin)", RI_NULL);
			}
			if ( !b2 ) {
				parser.errHandler().handleError(
					RIE_CONSISTENCY, RIE_ERROR,
					"Line %ld, File \"%s\", badarray: '%s' argument %s is not numeric",
					p2.lineNo(), parser.resourceName(), requestName(), "3 (zmax)", RI_NULL);
			}
			if ( !b3 ) {
				parser.errHandler().handleError(
					RIE_CONSISTENCY, RIE_ERROR,
					"Line %ld, File \"%s\", badargument: '%s' argument %s is not numeric",
					p3.lineNo(), parser.resourceName(), requestName(), "4 (thetamax)", RI_NULL);
			}
		}

	} else if ( request.size() >= 1 && request[0].isArray() ) {
		// Cylinder [ radius zmin zmax thetamax ] <paramlist>

		CRibParameter &p0 = request[0];
		p0.convertIntToFloat();
		bool correct = true;

		if ( p0.getCard() < 4 ) {
			parser.errHandler().handleError(
				RIE_MISSINGDATA, RIE_ERROR,
				"Line %ld, File \"%s\", badarray: '%s' arguments (%s) missing",
				p0.lineNo(), parser.resourceName(), requestName(), "radius, zmin, zmax, thetamax", RI_NULL);
			correct = false;
		}
		if ( p0.getCard() > 4 ) {
			parser.errHandler().handleError(
				RIE_CONSISTENCY, RIE_WARNING,
				"Line %ld, File \"%s\", badarray: '%s' additional arguments ignored",
				p0.lineNo(), parser.resourceName(), requestName(), RI_NULL);
		}
		if (  p0.typeID() != BASICTYPE_FLOAT ) {
			parser.errHandler().handleError(
				RIE_CONSISTENCY, RIE_ERROR, "Line %ld, File \"%s\", badarray: '%s' argument %s, not all elements are numeric",
				p0.lineNo(), parser.resourceName(), requestName(), "array 1", RI_NULL);
			correct = false;
		}

		int n = request.getTokenList(1, p);

		if ( !correct )
			return;

		RtFloat *vals = (RtFloat *)p0.getValue();
		if ( !vals ) {
			parser.errHandler().handleError(
				RIE_BUG, RIE_ERROR,
				"Line %ld, File \"%s\", badarray: '%s' could not store arguments",
				p0.lineNo(), parser.resourceName(), requestName(), RI_NULL);
			return;
		}

		RtFloat radius   = vals[0],
		        zmin     = vals[1],
		        zmax     = vals[2],
		        thetamax = vals[3];

		if ( n > 0 ) {
			parser.ribFilter().cylinderV(
				radius, zmin, zmax, thetamax,
				n, request.tokenList(), request.valueList());
		} else {
			parser.ribFilter().cylinderV(
				radius, zmin, zmax, thetamax,
				0, 0, 0);
		}
	} else {
		parser.errHandler().handleError(
			RIE_MISSINGDATA, RIE_ERROR,
			"Line %ld, File \"%s\", badargument: '%s' at least one of the arguments (%s) is missing",
			parser.lineNo(), parser.resourceName(), requestName(), "radius, zmin, zmax, thetamax", RI_NULL);
	}
}

void CHyperboloidRibRequest::operator()(IRibParserState &parser, CRibRequestData &request) const
{
	// Hyperboloid x1 y1 z1 x2 y2 z3 thetamax <paramlist>
	// Hyperboloid [ point1 ] [point2] [thetamax] <paramlist>
	// Hyperboloid [ point1 point2 thetamax ] <paramlist>
	CQuadricClasses p;
	if ( request.size() >= 7 && !request[0].isArray() ) {
		CRibParameter &p0 = request[0];
		CRibParameter &p1 = request[1];
		CRibParameter &p2 = request[2];
		CRibParameter &p3 = request[3];
		CRibParameter &p4 = request[4];
		CRibParameter &p5 = request[5];
		CRibParameter &p6 = request[6];

		RtPoint point1, point2;
		RtFloat thetamax;

		bool b0=p0.getFloat(point1[0]), b1=p1.getFloat(point1[1]), b2=p2.getFloat(point1[2]), b3=p3.getFloat(point2[0]), b4=p4.getFloat(point2[1]), b5=p5.getFloat(point2[2]), b6=p6.getFloat(thetamax);
		if ( b0 && b1 && b2 && b3 && b4 && b5 && b6 ) {
			int n = request.getTokenList(7, p);
			if ( n > 0 )
				parser.ribFilter().hyperboloidV(
					point1, point2, thetamax,
					n, request.tokenList(), request.valueList());
			else
				parser.ribFilter().hyperboloidV(
					point1, point2, thetamax,
					0, 0, 0);
		} else {
			if ( !b0 ) {
				parser.errHandler().handleError(
					RIE_CONSISTENCY, RIE_ERROR,
					"Line %ld, File \"%s\", badargument: '%s' argument %s is not numeric",
					p0.lineNo(), parser.resourceName(), requestName(), "1 (p0.x)", RI_NULL);
			}
			if ( !b1 ) {
				parser.errHandler().handleError(
					RIE_CONSISTENCY, RIE_ERROR,
					"Line %ld, File \"%s\", badargument: '%s' argument %s is not numeric",
					p1.lineNo(), parser.resourceName(), requestName(), "2 (p0.y)", RI_NULL);
			}
			if ( !b2 ) {
				parser.errHandler().handleError(
					RIE_CONSISTENCY, RIE_ERROR,
					"Line %ld, File \"%s\", badargument: '%s' argument %s is not numeric",
					p2.lineNo(), parser.resourceName(), requestName(), "3 (p0.z)", RI_NULL);
			}
			if ( !b3 ) {
				parser.errHandler().handleError(
					RIE_CONSISTENCY, RIE_ERROR,
					"Line %ld, File \"%s\", badargument: '%s' argument %s is not numeric",
					p3.lineNo(), parser.resourceName(), requestName(), "4 (p1.x)", RI_NULL);
			}
			if ( !b4 ) {
				parser.errHandler().handleError(
					RIE_CONSISTENCY, RIE_ERROR,
					"Line %ld, File \"%s\", badargument: '%s' argument %s is not numeric",
					p4.lineNo(), parser.resourceName(), requestName(), "5 (p1.y)", RI_NULL);
			}
			if ( !b5 ) {
				parser.errHandler().handleError(
					RIE_CONSISTENCY, RIE_ERROR,
					"Line %ld, File \"%s\", badargument: '%s' argument %s is not numeric",
					p5.lineNo(), parser.resourceName(), requestName(), "6 (p1.z)", RI_NULL);
			}
			if ( !b6 ) {
				parser.errHandler().handleError(
					RIE_CONSISTENCY, RIE_ERROR,
					"Line %ld, File \"%s\", badargument: '%s' argument %s is not numeric",
					p6.lineNo(), parser.resourceName(), requestName(), "7 (thetamax)", RI_NULL);
			}
		}
	} else if ( request.size() >= 3 && request[0].isArray() && request[1].isArray() ) {
		CRibParameter &p0 = request[0];
		p0.convertIntToFloat();

		CRibParameter &p1 = request[1];
		p1.convertIntToFloat();
		
		CRibParameter &p2 = request[2];

		RtFloat thetamax = 0;
		bool b2=p2.getFloat(thetamax);
		bool correct = b2;

		if ( p0.getCard() < 3 ) {
			parser.errHandler().handleError(
				RIE_CONSISTENCY, RIE_ERROR,
				"Line %ld, File \"%s\", badargument: '%s' arguments %s missing",
				p0.lineNo(), parser.resourceName(), requestName(), "*[p0.x p0.y p0.z] [p1.x p1.y p1.z] thetamax)", RI_NULL);
			correct = false;
		}
		if (  p0.typeID() != BASICTYPE_FLOAT ) {
			parser.errHandler().handleError(
				RIE_CONSISTENCY, RIE_WARNING,
				"Line %ld, File \"%s\", badargument: '%s' in argument %s not all elements are numeric",
				p0.lineNo(), parser.resourceName(), requestName(), "point 1", RI_NULL);
			correct = false;
		}

		if ( p1.getCard() < 3 ) {
			parser.errHandler().handleError(
				RIE_CONSISTENCY, RIE_ERROR,
				"Line %ld, File \"%s\", badargument: '%s' arguments %s missing",
				p0.lineNo(), parser.resourceName(), requestName(), "[p0.x p0.y p0.z] *[p1.x p1.y p1.z] thetamax)", RI_NULL);
			correct = false;
		}
		if (  p1.typeID() != BASICTYPE_FLOAT ) {
			parser.errHandler().handleError(
				RIE_CONSISTENCY, RIE_WARNING,
				"Line %ld, File \"%s\", badargument: '%s' in argument %s not all elements are numeric",
				p0.lineNo(), parser.resourceName(), requestName(), "point 2", RI_NULL);
			correct = false;
		}
		if ( !b2 ) {
			parser.errHandler().handleError(
				RIE_CONSISTENCY, RIE_ERROR,
				"Line %ld, File \"%s\", badargument: '%s' argument %s is not numeric",
				p2.lineNo(), parser.resourceName(), requestName(), "3 (thetamax)", RI_NULL);
		}

		int n = request.getTokenList(3, p);

		if ( !correct )
			return;

		RtFloat *vals0 = (RtFloat *)p0.getValue();
		if ( !vals0 ) {
			parser.errHandler().handleError(
				RIE_BUG, RIE_ERROR,
				"Line %ld, File \"%s\", badarray: '%s' could not store arguments of %s",
				p0.lineNo(), parser.resourceName(), requestName(), "point1", RI_NULL);
			return;
		}
		
		RtFloat *vals1 = (RtFloat *)p1.getValue();
		if ( !vals1 ) {
			parser.errHandler().handleError(
				RIE_BUG, RIE_ERROR,
				"Line %ld, File \"%s\", badarray: '%s' could not store arguments of %s",
				p0.lineNo(), parser.resourceName(), requestName(), "point2", RI_NULL);
			return;
		}

		RtPoint point1 = {vals0[0],vals0[1],vals0[2]}, point2 = {vals1[0],vals1[1],vals1[2]};

		if ( n > 0 )
			parser.ribFilter().hyperboloidV(
				point1, point2, thetamax,
				n, request.tokenList(), request.valueList());
		else
			parser.ribFilter().hyperboloidV(
				point1, point2, thetamax,
				0, 0, 0);

	} else if ( request.size() >= 1 && request[0].isArray() ) {
		CRibParameter &p0 = request[0];
		p0.convertIntToFloat();
		bool correct = true;

		if ( p0.getCard() < 7 ) {
			parser.errHandler().handleError(
				RIE_CONSISTENCY, RIE_ERROR,
				"Line %ld, File \"%s\", badargument: '%s' arguments %s missing",
				p0.lineNo(), parser.resourceName(), requestName(), "(p0.x, p0.y, p0.z, p1.x, p1.y, p1.z, thetamax)", RI_NULL);
			correct = false;
		}
		if ( p0.getCard() > 7 ) {
			parser.errHandler().handleError(
				RIE_CONSISTENCY, RIE_WARNING,
				"Line %ld, File \"%s\", badargument: '%s' additional arguments are ignored",
				p0.lineNo(), parser.resourceName(), requestName(), RI_NULL);
		}
		if (  p0.typeID() != BASICTYPE_FLOAT ) {
			parser.errHandler().handleError(
				RIE_CONSISTENCY, RIE_WARNING,
				"Line %ld, File \"%s\", badargument: '%s' in argument %s not all elements are numeric",
				p0.lineNo(), parser.resourceName(), requestName(), "array 1", RI_NULL);
			correct = false;
		}
		int n = request.getTokenList(1, p);

		if ( !correct )
			return;

		RtFloat *vals = (RtFloat *)p0.getValue();
		if ( !vals ) {
			parser.errHandler().handleError(
				RIE_BUG, RIE_ERROR,
				"Line %ld, File \"%s\", badarray: '%s' could not store arguments",
				p0.lineNo(), parser.resourceName(), requestName(), RI_NULL);
			return;
		}
		RtPoint point1 = {vals[0],vals[1],vals[2]}, point2 = {vals[3],vals[4],vals[5]};
		RtFloat thetamax = vals[6];

		if ( n > 0 )
			parser.ribFilter().hyperboloidV(
				point1, point2, thetamax,
				n, request.tokenList(), request.valueList());
		else
			parser.ribFilter().hyperboloidV(
				point1, point2, thetamax,
				0, 0, 0);
	} else {
		parser.errHandler().handleError(
			RIE_MISSINGDATA, RIE_ERROR,
			"Line %ld, File \"%s\", badargument: '%s' at least one of the arguments (%s) is missing",
			parser.lineNo(), parser.resourceName(), requestName(), "p0.x, p0.y, p0.z, p1.x, p1.y, p1.z, thetamax", RI_NULL);
	}
}

void CParaboloidRibRequest::operator()(IRibParserState &parser, CRibRequestData &request) const
{
	CQuadricClasses p;

	if ( request.size() >= 4 && !request[0].isArray() ) {
		// Paraboloid rmax zmin zmax thetamax <paramlist>

		CRibParameter &p0 = request[0];
		CRibParameter &p1 = request[1];
		CRibParameter &p2 = request[2];
		CRibParameter &p3 = request[3];

		RtFloat rmax, zmin, zmax, thetamax;
		bool b0 = p0.getFloat(rmax),
		     b1 = p1.getFloat(zmin),
		     b2 = p2.getFloat(zmax),
			 b3 = p3.getFloat(thetamax);

		if ( b0 && b1 && b2 && b3 ) {
			int n = request.getTokenList(4, p);
			if ( n > 0 ) {
				parser.ribFilter().paraboloidV(
					rmax, zmin, zmax, thetamax,
					n, request.tokenList(), request.valueList());
			} else {
				parser.ribFilter().paraboloidV(
					rmax, zmin, zmax, thetamax,
					0, 0, 0);
			}
		} else {
			if ( !b0 ) {
				parser.errHandler().handleError(
					RIE_CONSISTENCY, RIE_ERROR,
					"Line %ld, File \"%s\", badargument: '%s' argument %s is not numeric",
					p0.lineNo(), parser.resourceName(), requestName(), "1 (rmax)", RI_NULL);
			}
			if ( !b1 ) {
				parser.errHandler().handleError(
					RIE_CONSISTENCY, RIE_ERROR,
					"Line %ld, File \"%s\", badargument: '%s' argument %s is not numeric",
					p1.lineNo(), parser.resourceName(), requestName(), "2 (zmin)", RI_NULL);
			}
			if ( !b2 ) {
				parser.errHandler().handleError(
					RIE_CONSISTENCY, RIE_ERROR,
					"Line %ld, File \"%s\", badarray: '%s' argument %s is not numeric",
					p2.lineNo(), parser.resourceName(), requestName(), "3 (zmax)", RI_NULL);
			}
			if ( !b3 ) {
				parser.errHandler().handleError(
					RIE_CONSISTENCY, RIE_ERROR,
					"Line %ld, File \"%s\", badargument: '%s' argument %s is not numeric",
					p3.lineNo(), parser.resourceName(), requestName(), "4 (thetamax)", RI_NULL);
			}
		}

	} else if ( request.size() >= 1 && request[0].isArray() ) {
		// Paraboloid [ rmax zmin zmax thetamax ] <paramlist>

		CRibParameter &p0 = request[0];
		p0.convertIntToFloat();
		bool correct = true;

		if ( p0.getCard() < 4 ) {
			parser.errHandler().handleError(
				RIE_MISSINGDATA, RIE_ERROR,
				"Line %ld, File \"%s\", badarray: '%s' arguments (%s) missing",
				p0.lineNo(), parser.resourceName(), requestName(), "rmax, zmin, zmax, thetamax", RI_NULL);
			correct = false;
		}
		if ( p0.getCard() > 4 ) {
			parser.errHandler().handleError(
				RIE_CONSISTENCY, RIE_WARNING,
				"Line %ld, File \"%s\", badarray: '%s' additional arguments ignored",
				p0.lineNo(), parser.resourceName(), requestName(), RI_NULL);
		}
		if (  p0.typeID() != BASICTYPE_FLOAT ) {
			parser.errHandler().handleError(
				RIE_CONSISTENCY, RIE_ERROR, "Line %ld, File \"%s\", badarray: '%s' argument %s, not all elements are numeric",
				p0.lineNo(), parser.resourceName(), requestName(), "array 1", RI_NULL);
			correct = false;
		}

		int n = request.getTokenList(1, p);

		if ( !correct )
			return;

		RtFloat *vals = (RtFloat *)p0.getValue();
		if ( !vals ) {
			parser.errHandler().handleError(
				RIE_BUG, RIE_ERROR,
				"Line %ld, File \"%s\", badarray: '%s' could not store arguments",
				p0.lineNo(), parser.resourceName(), requestName(), RI_NULL);
			return;
		}

		RtFloat rmax     = vals[0],
		        zmin     = vals[1],
		        zmax     = vals[2],
		        thetamax = vals[3];

		if ( n > 0 ) {
			parser.ribFilter().paraboloidV(
				rmax, zmin, zmax, thetamax,
				n, request.tokenList(), request.valueList());
		} else {
			parser.ribFilter().paraboloidV(
				rmax, zmin, zmax, thetamax,
				0, 0, 0);
		}
	} else {
		parser.errHandler().handleError(
			RIE_MISSINGDATA, RIE_ERROR,
			"Line %ld, File \"%s\", badargument: '%s' at least one of the arguments (%s) is missing",
			parser.lineNo(), parser.resourceName(), requestName(), "rmax, zmin, zmax, thetamax", RI_NULL);
	}
}

void CDiskRibRequest::operator()(IRibParserState &parser, CRibRequestData &request) const
{
	CQuadricClasses p;

	if ( request.size() >= 3 && !request[0].isArray() ) {
		// Disk height radius thetamax <paramlist>

		CRibParameter &p0 = request[0];
		CRibParameter &p1 = request[1];
		CRibParameter &p2 = request[2];

		RtFloat height, radius, thetamax;
		bool b0 = p0.getFloat(height),
		     b1 = p1.getFloat(radius),
			 b2 = p2.getFloat(thetamax);

		if ( b0 && b1 && b2 ) {
			int n = request.getTokenList(3, p);
			if ( n > 0 ) {
				parser.ribFilter().diskV(height, radius, thetamax,
					n, request.tokenList(), request.valueList());
			} else {
				parser.ribFilter().diskV(height, radius, thetamax,
					0, 0, 0);
			}
		} else {
			if ( !b0 ) {
				parser.errHandler().handleError(
					RIE_CONSISTENCY, RIE_ERROR,
					"Line %ld, File \"%s\", badargument: '%s' argument %s is not numeric",
					p1.lineNo(), parser.resourceName(), requestName(), "1 (height)", RI_NULL);
			}
			if ( !b1 ) {
				parser.errHandler().handleError(
					RIE_CONSISTENCY, RIE_ERROR,
					"Line %ld, File \"%s\", badargument: '%s' argument %s is not numeric",
					p1.lineNo(), parser.resourceName(), requestName(), "2 (radius)", RI_NULL);
			}
			if ( !b2 ) {
				parser.errHandler().handleError(
					RIE_CONSISTENCY, RIE_ERROR,
					"Line %ld, File \"%s\", badargument: '%s' argument %s is not numeric",
					p2.lineNo(), parser.resourceName(), requestName(), "3 (thetamax)", RI_NULL);
			}
		}

	} else if ( request.size() >= 1 && request[0].isArray() ) {
		// Cone [ height radius thetamax ] <paramlist>

		CRibParameter &p0 = request[0];
		p0.convertIntToFloat();
		bool correct = true;

		if ( p0.getCard() < 3 ) {
			parser.errHandler().handleError(
				RIE_MISSINGDATA, RIE_ERROR,
				"Line %ld, File \"%s\", badarray: '%s' arguments %s missing",
				p0.lineNo(), parser.resourceName(), requestName(), "(height, radius, thetamax)", RI_NULL);
			correct = false;
		}
		if ( p0.getCard() > 3 ) {
			parser.errHandler().handleError(
				RIE_CONSISTENCY, RIE_WARNING,
				"Line %ld, File \"%s\", badarray: '%s' additional arguments ignored",
				p0.lineNo(), parser.resourceName(), requestName(), RI_NULL);
		}
		if (  p0.typeID() != BASICTYPE_FLOAT ) {
			parser.errHandler().handleError(
				RIE_CONSISTENCY, RIE_ERROR, "Line %ld, File \"%s\", badarray: '%s' argument %s, not all elements are numeric",
				p0.lineNo(), parser.resourceName(), requestName(), "array 1", RI_NULL);
			correct = false;
		}

		int n = request.getTokenList(1, p);

		if ( !correct )
			return;

		RtFloat *vals = (RtFloat *)p0.getValue();
		if ( !vals ) {
			parser.errHandler().handleError(
				RIE_BUG, RIE_ERROR,
				"Line %ld, File \"%s\", badarray: '%s' could not store arguments",
				p0.lineNo(), parser.resourceName(), requestName(), RI_NULL);
			return;
		}

		RtFloat height   = vals[0],
		        radius   = vals[1],
		        thetamax = vals[2];

		if ( n > 0 ) {
			parser.ribFilter().diskV(
				height, radius, thetamax,
				n, request.tokenList(), request.valueList());
		} else {
			parser.ribFilter().diskV(
				height, radius, thetamax,
				0, 0, 0);
		}
	} else {
		parser.errHandler().handleError(
			RIE_MISSINGDATA, RIE_ERROR,
			"Line %ld, File \"%s\", badargument: '%s' at least one of the arguments (%s) is missing",
			parser.lineNo(), parser.resourceName(), requestName(), "radius, zmin, zmax, thetamax", RI_NULL);
	}
}

void CTorusRibRequest::operator()(IRibParserState &parser, CRibRequestData &request) const
{
	CQuadricClasses p;

	if ( request.size() >= 5 && !request[0].isArray() ) {
		// Torus majorrad minorrad phimin phimax thetamax <paramlist>

		CRibParameter &p0 = request[0];
		CRibParameter &p1 = request[1];
		CRibParameter &p2 = request[2];
		CRibParameter &p3 = request[3];
		CRibParameter &p4 = request[4];

		RtFloat majorrad, minorrad, phimin, phimax, thetamax;
		bool b0 = p0.getFloat(majorrad),
		     b1 = p1.getFloat(minorrad),
		     b2 = p2.getFloat(phimin),
			 b3 = p3.getFloat(phimax),
			 b4 = p4.getFloat(thetamax);

		if ( b0 && b1 && b2 && b3 && b4 ) {
			int n = request.getTokenList(5, p);
			if ( n > 0 ) {
				parser.ribFilter().torusV(
					majorrad, minorrad, phimin, phimax, thetamax,
					n, request.tokenList(), request.valueList());
			} else {
				parser.ribFilter().torusV(
					majorrad, minorrad, phimin, phimax, thetamax,
					0, 0, 0);
			}
		} else {
			if ( !b0 ) {
				parser.errHandler().handleError(
					RIE_CONSISTENCY, RIE_ERROR,
					"Line %ld, File \"%s\", badargument: '%s' argument %s is not numeric",
					p0.lineNo(), parser.resourceName(), requestName(), "1 (majorrad)", RI_NULL);
			}
			if ( !b1 ) {
				parser.errHandler().handleError(
					RIE_CONSISTENCY, RIE_ERROR,
					"Line %ld, File \"%s\", badargument: '%s' argument %s is not numeric",
					p1.lineNo(), parser.resourceName(), requestName(), "2 (minorrad)", RI_NULL);
			}
			if ( !b2 ) {
				parser.errHandler().handleError(
					RIE_CONSISTENCY, RIE_ERROR,
					"Line %ld, File \"%s\", badarray: '%s' argument %s is not numeric",
					p2.lineNo(), parser.resourceName(), requestName(), "3 (phimin)", RI_NULL);
			}
			if ( !b3 ) {
				parser.errHandler().handleError(
					RIE_CONSISTENCY, RIE_ERROR,
					"Line %ld, File \"%s\", badargument: '%s' argument %s is not numeric",
					p3.lineNo(), parser.resourceName(), requestName(), "4 (phimax)", RI_NULL);
			}
			if ( !b4 ) {
				parser.errHandler().handleError(
					RIE_CONSISTENCY, RIE_ERROR,
					"Line %ld, File \"%s\", badargument: '%s' argument %s is not numeric",
					p4.lineNo(), parser.resourceName(), requestName(), "5 (thetamax)", RI_NULL);
			}
		}

	} else if ( request.size() >= 1 && request[0].isArray() ) {
		// Torus [ majorrad minorrad phimin phimax thetamax ] <paramlist>

		CRibParameter &p0 = request[0];
		p0.convertIntToFloat();
		bool correct = true;

		if ( p0.getCard() < 5 ) {
			parser.errHandler().handleError(
				RIE_MISSINGDATA, RIE_ERROR,
				"Line %ld, File \"%s\", badarray: '%s' arguments (%s) missing",
				p0.lineNo(), parser.resourceName(), requestName(), "majorrad, minorrad, phimin, phimax, thetamax", RI_NULL);
			correct = false;
		}
		if ( p0.getCard() > 5 ) {
			parser.errHandler().handleError(
				RIE_CONSISTENCY, RIE_WARNING,
				"Line %ld, File \"%s\", badarray: '%s' additional arguments ignored",
				p0.lineNo(), parser.resourceName(), requestName(), RI_NULL);
		}
		if (  p0.typeID() != BASICTYPE_FLOAT ) {
			parser.errHandler().handleError(
				RIE_CONSISTENCY, RIE_ERROR, "Line %ld, File \"%s\", badarray: '%s' argument %s, not all elements are numeric",
				p0.lineNo(), parser.resourceName(), requestName(), "array 1", RI_NULL);
			correct = false;
		}

		int n = request.getTokenList(1, p);

		if ( !correct )
			return;

		RtFloat *vals = (RtFloat *)p0.getValue();
		if ( !vals ) {
			parser.errHandler().handleError(
				RIE_BUG, RIE_ERROR,
				"Line %ld, File \"%s\", badarray: '%s' could not store arguments",
				p0.lineNo(), parser.resourceName(), requestName(), RI_NULL);
			return;
		}

		RtFloat majorrad = vals[0],
		        minorrad = vals[1],
		        phimin   = vals[2],
		        phimax   = vals[3],
		        thetamax = vals[3];

		if ( n > 0 ) {
			parser.ribFilter().torusV(
				majorrad, minorrad, phimin, phimax, thetamax,
				n, request.tokenList(), request.valueList());
		} else {
			parser.ribFilter().torusV(
				majorrad, minorrad, phimin, phimax, thetamax,
				0, 0, 0);
		}
	} else {
		parser.errHandler().handleError(
			RIE_MISSINGDATA, RIE_ERROR,
			"Line %ld, File \"%s\", badargument: '%s' at least one of the arguments (%s) is missing",
			parser.lineNo(), parser.resourceName(), requestName(), "majorrad, minorrad, phimin, phimax, thetamax", RI_NULL);
	}
}

void CPointsRibRequest::operator()(IRibParserState &parser, CRibRequestData &request) const
{
	// Points <paramlist>
	int n = request.getTokenListNoCheck(0);	// Needed for numVertices
	if ( n > 0 ) {
		RtInt npts = request.numVertices(0, n);
		if ( npts > 0 ) {
			CPointsClasses p(npts);
			n = request.getTokenList(0, p); // Test if number of elements are correct
			if ( n > 0 ) {
				parser.ribFilter().pointsV(
					npts,
					n, request.tokenList(), request.valueList());
			} else {
				parser.ribFilter().pointsV(
					npts,
					0, 0, 0);
			}
		} else {
			parser.errHandler().handleError(
				RIE_MISSINGDATA, RIE_ERROR,
				"Line %ld, File \"%s\", badargument: '%s' arguments missing, there must be at least positional data",
				parser.lineNo(), parser.resourceName(), requestName(), RI_NULL);
		}
	} else {
		parser.errHandler().handleError(
			RIE_MISSINGDATA, RIE_ERROR,
			"Line %ld, File \"%s\", badargument: '%s' arguments missing, there must be at least positional data",
			parser.lineNo(), parser.resourceName(), requestName(), RI_NULL);
	}
}

void CCurvesRibRequest::operator()(IRibParserState &parser, CRibRequestData &request) const
{
	// Curves type [ nvertices ] wrap <paramlist>
	if ( request.size() >= 3 ) {
		CRibParameter &p0 = request[0];	// type
		CRibParameter &p1 = request[1];    // nvertices
		CRibParameter &p2 = request[2];    // wrap

		p1.convertFloatToInt();

		RtInt ncurves = 0, *nverts = 0;
		bool correct = true;

		if ( p1.typeID() != BASICTYPE_INTEGER || !p1.isArray() ) {
			parser.errHandler().handleError(
				RIE_CONSISTENCY, RIE_ERROR,
				"Line %ld, File \"%s\", badargument: '%s' argument %s contains no integers or is not an array",
				p1.lineNo(), parser.resourceName(),
				requestName(), "2 (nverices)", RI_NULL);
			correct = false;
		} else {
			ncurves = (RtInt)p1.getCard();
			nverts = (RtInt *)p1.getValue();
		}

		const char *type = 0, *wrap = 0;
		bool b0 = p0.getString(type), b2 = p2.getString(wrap);

		if ( b0 && b2 ) {
			bool isLinear = false;
			bool isPeriodic = false;
			if ( !strcmp(type, RI_LINEAR) ) {
				isLinear = true;
			} else if ( strcmp(type, RI_CUBIC) ) {
				parser.errHandler().handleError(
					RIE_CONSISTENCY, RIE_ERROR,
					"Line %ld, File \"%s\", badargument: '%s' argument %s==\"%s\" must be \"linear\" or \"cubic\"",
					p1.lineNo(), parser.resourceName(),
					requestName(), "1 (type)", type, RI_NULL);
				correct = false;
			}
			if ( !strcmp(wrap, RI_PERIODIC) ) {
				isPeriodic = true;
			} else if ( strcmp(wrap, RI_NONPERIODIC) ) {
				parser.errHandler().handleError(
					RIE_CONSISTENCY, RIE_ERROR,
					"Line %ld, File \"%s\", badargument: '%s' argument %s==\"%s\" must be \"periodic\" or \"nonperiodic\"",
					p2.lineNo(), parser.resourceName(),
					requestName(), "3 (wrap)", wrap, RI_NULL);
				correct = false;
			}

			if ( !correct ) {
				return;
			}

			RtInt i;
			for ( i = 0; i < ncurves; ++i ) {
				if ( isLinear ) {
					if ( nverts[i] < 2 ) {
						correct = false;
						parser.errHandler().handleError(
							RIE_CONSISTENCY, RIE_ERROR,
							"Line %ld, File \"%s\", badargument: '%s %s' argument %s at position %d must be %s",
							p1.lineNo(), parser.resourceName(),
							requestName(), "\"linear\"", "2 (ncurves)", i, ">= 2", RI_NULL);
					}
				} else {
					if ( nverts[i] < 4 ) {
						correct = false;
						parser.errHandler().handleError(
							RIE_CONSISTENCY, RIE_ERROR,
							"Line %ld, File \"%s\", badargument: '%s %s' argument %s at position %d must be %s",
							p1.lineNo(), parser.resourceName(),
							requestName(), "\"bicubic\"", "2 (ncurves)", i, ">= 4", RI_NULL);
					}
				}
			}

			if ( !correct ) {
				return;
			}

			CCurvesClasses p(type, ncurves, nverts, wrap, parser.renderState().attributes().vStep());

			int n = request.getTokenList(3, p);
			if ( n > 0 )
				parser.ribFilter().curvesV(
					type, ncurves, nverts, wrap,
					n, request.tokenList(), request.valueList());
			else
				parser.ribFilter().curvesV(
					type, ncurves, nverts, wrap,
					0, 0, 0);
		} else {
			if ( !b0 ) {
				parser.errHandler().handleError(
					RIE_CONSISTENCY, RIE_ERROR,
					"Line %ld, File \"%s\", badargument: '%s' argument %s is not a string",
					p0.lineNo(), parser.resourceName(), requestName(), "1 (type)", RI_NULL);
			}
			if ( !b2 ) {
				parser.errHandler().handleError(
					RIE_CONSISTENCY, RIE_ERROR,
					"Line %ld, File \"%s\", badargument: '%s' argument %s is not a string",
					p2.lineNo(), parser.resourceName(), requestName(), "3 (wrap)", RI_NULL);
			}
		}
	} else {
		parser.errHandler().handleError(
			RIE_MISSINGDATA, RIE_ERROR,
			"Line %ld, File \"%s\", badargument: '%s' arguments (%s) missing",
			parser.lineNo(), parser.resourceName(), requestName(), "type, nvertices array, wrap", RI_NULL);
	}
}

void CBlobbyRibRequest::operator()(IRibParserState &parser, CRibRequestData &request) const
{
	// Blobby nleaf [ codes ] [ floats ] [ strings ] <paramlist>
	if ( request.size() >= 4 ) {
		CRibParameter &p0 = request[0]; // nleaf
		CRibParameter &p1 = request[1]; // *codes
		CRibParameter &p2 = request[2]; // *floats
		CRibParameter &p3 = request[3]; // *strings

		p1.convertFloatToInt();
		p2.convertIntToFloat();

		RtInt nleaf = 0, ncode = 0, nflt = 0, nstr = 0;
		RtInt       *code = 0;
		RtFloat     *flt  = 0;
		const char **str  = 0;

		bool correct = true;

		if ( p1.getCard() != 0 ) {
			if ( p1.typeID() != BASICTYPE_INTEGER || !p1.isArray() ) {
				correct = false;
				parser.errHandler().handleError(
					RIE_CONSISTENCY, RIE_ERROR,
					"Line %ld, File \"%s\", badargument: '%s' argument %s contains no integers or is not an array",
					p1.lineNo(), parser.resourceName(),
					requestName(), "2 (codes)", RI_NULL);
			} else {
				ncode = (RtInt)p1.getCard();
				code = (RtInt *)p1.getValue();
			}
		}

		if ( p2.getCard() != 0 ) {
			if ( p2.typeID() != BASICTYPE_FLOAT || !p2.isArray()  ) {
				correct = false;
				parser.errHandler().handleError(
					RIE_CONSISTENCY, RIE_ERROR,
					"Line %ld, File \"%s\", badargument: '%s' argument %s contains no numerics or is not an array",
					p2.lineNo(), parser.resourceName(),
					requestName(), "3 (floats)", RI_NULL);
			} else {
				nflt = (RtInt)p2.getCard();
				flt = (RtFloat *)p2.getValue();
			}
		}

		if ( p3.getCard() != 0 ) {
			if ( p3.typeID() != BASICTYPE_STRING || !p3.isArray()  ) {
				correct = false;
				parser.errHandler().handleError(
					RIE_CONSISTENCY, RIE_ERROR,
					"Line %ld, File \"%s\", badargument: '%s' argument %s contains no strings or is not an array",
					p3.lineNo(), parser.resourceName(),
					requestName(), "4 (strings)", RI_NULL);
			} else {
				nstr = (RtInt)p3.getCard();
				str = (const char **)p3.getValue();
			}
		}

		if ( p0.getInt(nleaf) ) {
			if ( correct ) {
				CBlobbyClasses p(nleaf);
				int n = request.getTokenList(4, p);
				if ( n > 0 ) {
					parser.ribFilter().blobbyV(
						nleaf, ncode, code, nflt, flt, nstr, str,
						n, request.tokenList(), request.valueList());
				} else {
					parser.ribFilter().blobbyV(
						nleaf, ncode, code, nflt, flt, nstr, str,
						0, 0, 0);
				}
			}
		} else {
			parser.errHandler().handleError(
				RIE_CONSISTENCY, RIE_ERROR,
				"Line %ld, File \"%s\", badargument: '%s' argument %s is not numeric",
				p0.lineNo(), parser.resourceName(), requestName(), "1 (nleaf)", RI_NULL);
		}
	} else {
		parser.errHandler().handleError(
			RIE_MISSINGDATA, RIE_ERROR,
			"Line %ld, File \"%s\", badargument: '%s' arguments (%s) missing",
			parser.lineNo(), parser.resourceName(), requestName(), "nleaf, codes array, floats array, strings array", RI_NULL);
	}
}

void CProceduralRibRequest::operator()(IRibParserState &parser, CRibRequestData &request) const
{
	// Procedural procname [ args ] [ bounds ]
	// Procedural "DelayedReadArchive" [ filename ] [ bounds ]
	// Procedural "ReadProgram" [ programname paramblock ] [ boundingbox ]
	// Procedural "DynamicLoad" [ dsoname paramblock ] [ boundingbox ]

	if ( request.size() >= 3 ) {
		CRibParameter &p0 = request[0];
		CRibParameter &p1 = request[1];
		CRibParameter &p2 = request[2];

		p2.convertIntToFloat();

		RtFloat *bound = 0;
		const char *procname = 0, **args = 0;
		bool correct = true;

		if ( p0.typeID() != BASICTYPE_STRING ) {
			correct = false;
			parser.errHandler().handleError(
				RIE_CONSISTENCY, RIE_ERROR,
				"Line %ld, File \"%s\", badargument: '%s' argument %s is not a string",
				p0.lineNo(), parser.resourceName(), requestName(), "1 (procname)", RI_NULL);
		} else if ( !p0.getString(procname) ) {
			correct = false;
			parser.errHandler().handleError(
				RIE_CONSISTENCY, RIE_ERROR,
				"Line %ld, File \"%s\", badargument: '%s' argument %s could not read string",
				p0.lineNo(), parser.resourceName(), requestName(), "1 (procname)", RI_NULL);
		}

		procname = noNullStr(procname);

		if ( p1.typeID() != BASICTYPE_STRING || !p1.isArray() ) {
			correct = false;
			parser.errHandler().handleError(
				RIE_CONSISTENCY, RIE_ERROR,
				"Line %ld, File \"%s\", badargument: '%s' argument %s contains no strings or is not an array",
				p1.lineNo(), parser.resourceName(),
				requestName(), "2 (args)", RI_NULL);
		} else {
			args = (const char **)p1.getValue();
			if ( !args ) {
				correct = false;
				parser.errHandler().handleError(
					RIE_CONSISTENCY, RIE_ERROR,
					"Line %ld, File \"%s\", badargument: '%s' argument %s could not read strings",
					p1.lineNo(), parser.resourceName(), requestName(), "2 (args)", RI_NULL);
			}
		}
	
		if ( p2.typeID() != BASICTYPE_FLOAT || !p2.isArray() ) {
			correct = false;
			parser.errHandler().handleError(
				RIE_CONSISTENCY, RIE_ERROR,
				"Line %ld, File \"%s\", badargument: '%s' argument %s contains no numerics or is not an array",
				p2.lineNo(), parser.resourceName(),
				requestName(), "3 (bounds)", RI_NULL);
		} else if ( p2.getCard() < 6 ) {
			parser.errHandler().handleError(
				RIE_CONSISTENCY, RIE_ERROR,
				"Line %ld, File \"%s\", badargument: '%s' argument %s %s",
				p2.lineNo(), parser.resourceName(),
				requestName(), "3 (bounds)", "has not 6 members", RI_NULL);
			correct = false;
		} else if ( p2.getCard() > 6 ) {
			parser.errHandler().handleError(
				RIE_CONSISTENCY, RIE_WARNING,
				"Line %ld, File \"%s\", badargument: '%s' argument %s %s, additional values are ignored",
				p2.lineNo(), parser.resourceName(),
				requestName(), "3 (bounds)", "has not 6 members", RI_NULL);
		} else {
			bound = (RtFloat *)p2.getValue();
			if ( !bound ) {
				correct = false;
				parser.errHandler().handleError(
					RIE_CONSISTENCY, RIE_ERROR,
					"Line %ld, File \"%s\", badargument: '%s' argument %s could not read numerics",
					p2.lineNo(), parser.resourceName(), requestName(), "3 (bounds)", RI_NULL);
			}
		}

		if ( !correct ) {
			return;
		}

		const CProcFree *freeFunc = 0;

		if ( !strcmp(procname, CProcDelayedReadArchive::myName()) ) {
			if ( p1.getCard() < 1 ) {
				parser.errHandler().handleError(
					RIE_CONSISTENCY, RIE_ERROR,
					"Line %ld, File \"%s\", badargument: '%s \"%s\"' argument %s %s",
					p1.lineNo(), parser.resourceName(),
					requestName(), CProcDelayedReadArchive::myName(), "2 (args)", "has no members", RI_NULL);
				correct = false;
			} else if ( p1.getCard() > 1 ) {
				parser.errHandler().handleError(
					RIE_CONSISTENCY, RIE_WARNING,
					"Line %ld, File \"%s\", badargument: '%s \"%s\"' argument %s %s, additional values are ignored",
					p1.lineNo(), parser.resourceName(),
					requestName(), CProcDelayedReadArchive::myName(), "2 (args)", "has more than one member", RI_NULL);
			}
			if ( correct ) {
				parser.ribFilter().procedural((RtPointer)args, bound, CProcDelayedReadArchive::func, freeFunc);
			}
		} else if ( !strcmp(procname, CProcRunProgram::myName()) ) {
			if ( p1.getCard() < 2 ) {
				parser.errHandler().handleError(
					RIE_CONSISTENCY, RIE_ERROR,
					"Line %ld, File \"%s\", badargument: '%s \"%s\"' argument %s %s",
					p1.lineNo(), parser.resourceName(),
					requestName(), CProcRunProgram::myName(), "2 (args)", "has less than two members", RI_NULL);
				correct = false;
			} else if ( p1.getCard() > 2 ) {
				parser.errHandler().handleError(
					RIE_CONSISTENCY, RIE_WARNING,
					"Line %ld, File \"%s\", badargument: '%s \"%s\"' argument %s %s, additional values are ignored",
					p1.lineNo(), parser.resourceName(),
					requestName(), CProcRunProgram::myName(), "2 (args)", "has more than two members", RI_NULL);
			}
			if ( correct ) {
				parser.ribFilter().procedural((RtPointer)args, bound, CProcRunProgram::func, freeFunc);
			}
		} else if ( !strcmp(procname, CProcDynamicLoad::myName()) ) {
			if ( p1.getCard() < 2 ) {
				parser.errHandler().handleError(
					RIE_CONSISTENCY, RIE_ERROR,
					"Line %ld, File \"%s\", badargument: '%s \"%s\"' argument %s %s",
					p1.lineNo(), parser.resourceName(),
					requestName(), CProcDynamicLoad::myName(), "2 (args)", "has less than two members", RI_NULL);
				correct = false;
			} else if ( p1.getCard() > 2 ) {
				parser.errHandler().handleError(
					RIE_CONSISTENCY, RIE_WARNING,
					"Line %ld, File \"%s\", badargument: '%s \"%s\"' argument %s %s, additional values are ignored",
					p1.lineNo(), parser.resourceName(),
					requestName(), CProcDynamicLoad::myName(), "2 (args)", "has more than two members", RI_NULL);
			}
			if ( correct ) {
				parser.ribFilter().procedural((RtPointer)args, bound, CProcDynamicLoad::func, freeFunc);
			}
		} else {
			parser.errHandler().handleError(
				RIE_CONSISTENCY, RIE_ERROR,
				"Line %ld, File \"%s\", badargument: '%s' argument %s==\"%s\" is neither \"%s\", \"%s\" or \"%s\"",
				p1.lineNo(), parser.resourceName(),
				requestName(), "1 (procname)", procname,
				CProcDelayedReadArchive::myName(),
				CProcRunProgram::myName(),
				CProcDynamicLoad::myName(),
				RI_NULL);
		}

		if ( request.size() > 3 ) {
			parser.errHandler().handleError(
				RIE_CONSISTENCY, RIE_WARNING,
				"Line %ld, File \"%s\", badargument: '%s' additional values are ignored",
				parser.lineNo(), parser.resourceName(),
				requestName(), RI_NULL);
		}

	} else {
		parser.errHandler().handleError(
			RIE_MISSINGDATA, RIE_ERROR,
			"Line %ld, File \"%s\", badargument: '%s' arguments (%s) missing",
			parser.lineNo(), parser.resourceName(), requestName(), "procname, args array, bounds array", RI_NULL);
	}
}

void CGeometryRibRequest::operator()(IRibParserState &parser, CRibRequestData &request) const
{
	// Geometry name <paramlist>
	if ( request.size() >= 1 ) {

		CRibParameter &p0 = request[0];
		const char *name;

		if ( p0.getString(name) ) {

			CParameterClasses p;
			int n = request.getTokenList(1, p);
			if ( n > 0 ) {
				parser.ribFilter().geometryV(name, n, request.tokenList(), request.valueList());
			} else {
				parser.ribFilter().geometryV(name, 0, 0, 0);
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
