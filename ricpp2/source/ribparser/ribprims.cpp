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

using namespace RiCPP;

void CPolygonRibRequest::operator()(IRibParserState &parser, CRibRequestData &request) const
{
}

void CGeneralPolygonRibRequest::operator()(IRibParserState &parser, CRibRequestData &request) const
{
}

void CPointsPolygonsRibRequest::operator()(IRibParserState &parser, CRibRequestData &request) const
{
}

void CPointsGeneralPolygonsRibRequest::operator()(IRibParserState &parser, CRibRequestData &request) const
{
}

void CPatchRibRequest::operator()(IRibParserState &parser, CRibRequestData &request) const
{
}

void CPatchMeshRibRequest::operator()(IRibParserState &parser, CRibRequestData &request) const
{
}

void CNuPatchRibRequest::operator()(IRibParserState &parser, CRibRequestData &request) const
{
}

void CSubdivisionMeshRibRequest::operator()(IRibParserState &parser, CRibRequestData &request) const
{
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
				parser.ribFilter().sphereV(radius, zmin, zmax, thetamax,
					n, request.tokenList(), request.valueList());
			} else {
				parser.ribFilter().sphereV(radius, zmin, zmax, thetamax,
					0, 0, 0);
			}
		} else {
			if ( !b0 ) {
				parser.errHandler().handleError(
					RIE_CONSISTENCY, RIE_ERROR,
					"Line %ld, File \"%s\", badargument: 'Sphere' argument 1 (radius) is not numeric",
					p0.lineNo(), parser.resourceName(), RI_NULL);
			}
			if ( !b1 ) {
				parser.errHandler().handleError(
					RIE_CONSISTENCY, RIE_ERROR,
					"Line %ld, File \"%s\", badargument: 'Sphere' argument 2 (zmin) is not numeric",
					p1.lineNo(), parser.resourceName(), RI_NULL);
			}
			if ( !b2 ) {
				parser.errHandler().handleError(
					RIE_CONSISTENCY, RIE_ERROR,
					"Line %ld, File \"%s\", badarray: 'Sphere' argument 3 (zmax) is not numeric",
					p2.lineNo(), parser.resourceName(), RI_NULL);
			}
			if ( !b3 ) {
				parser.errHandler().handleError(
					RIE_CONSISTENCY, RIE_ERROR,
					"Line %ld, File \"%s\", badargument: 'Sphere' argument 4 (thetamax) is not numeric",
					p3.lineNo(), parser.resourceName(), RI_NULL);
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
				"Line %ld, File \"%s\", badarray: 'Sphere' arguments (radius, zmin, zmax, thetamax) missing",
				p0.lineNo(), parser.resourceName(), RI_NULL);
			correct = false;
		}
		if ( p0.getCard() > 4 ) {
			parser.errHandler().handleError(
				RIE_CONSISTENCY, RIE_WARNING,
				"Line %ld, File \"%s\", badarray: 'Sphere' additional arguments ignored",
				p0.lineNo(), parser.resourceName(), RI_NULL);
		}
		if (  p0.typeID() != BASICTYPE_FLOAT ) {
			parser.errHandler().handleError(
				RIE_CONSISTENCY, RIE_ERROR, "Line %ld, File \"%s\", badarray: 'Sphere' argument array 1, not all elements are numeric",
				p0.lineNo(), parser.resourceName(), RI_NULL);
			correct = false;
		}

		int n = request.getTokenList(1, p);

		if ( !correct )
			return;

		RtFloat *vals = (RtFloat *)p0.getValue();
		if ( !vals ) {
			parser.errHandler().handleError(
				RIE_BUG, RIE_ERROR,
				"Line %ld, File \"%s\", badarray: 'Sphere' could not store arguments",
				p0.lineNo(), parser.resourceName(), RI_NULL);
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
}

void CCylinderRibRequest::operator()(IRibParserState &parser, CRibRequestData &request) const
{
}

void CHyperboloidRibRequest::operator()(IRibParserState &parser, CRibRequestData &request) const
{
}

void CParaboloidRibRequest::operator()(IRibParserState &parser, CRibRequestData &request) const
{
}

void CDiskRibRequest::operator()(IRibParserState &parser, CRibRequestData &request) const
{
}

void CTorusRibRequest::operator()(IRibParserState &parser, CRibRequestData &request) const
{
}

void CPointsRibRequest::operator()(IRibParserState &parser, CRibRequestData &request) const
{
}

void CCurvesRibRequest::operator()(IRibParserState &parser, CRibRequestData &request) const
{
}

void CBlobbyRibRequest::operator()(IRibParserState &parser, CRibRequestData &request) const
{
}

void CProceduralRibRequest::operator()(IRibParserState &parser, CRibRequestData &request) const
{
}

void CGeometryRibRequest::operator()(IRibParserState &parser, CRibRequestData &request) const
{
}
