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

/** @file ribmodes.cpp
 *  @author Andreas Pidde (andreas@pidde.de)
 *  @brief Implementation of the handling of RIB requests for modes.
 */

#include "ricpp/ribparser/ribmodes.h"

using namespace RiCPP;

void CResourceBeginRibRequest::operator()(IRibParserState &parser, CRibRequestData &request) const
{
	// ResourceBegin
	if ( request.size() != 0 ) {
		parser.errHandler().handleError(
			RIE_CONSISTENCY, RIE_WARNING,
			"Line %ld, File \"%s\", badargument: '%s' has additional parameters, they are ignored",
			parser.lineno(), parser.resourceName(), requestName(), RI_NULL);
	}
	parser.ribFilter().resourceBegin();
}

void CResourceEndRibRequest::operator()(IRibParserState &parser, CRibRequestData &request) const
{
	// ResourceEnd
	if ( request.size() != 0 ) {
		parser.errHandler().handleError(
			RIE_CONSISTENCY, RIE_WARNING,
			"Line %ld, File \"%s\", badargument: '%s' has additional parameters, they are ignored",
			parser.lineno(), parser.resourceName(), requestName(), RI_NULL);
	}
	parser.ribFilter().resourceEnd();
}


void CResourceRibRequest::operator()(IRibParserState &parser, CRibRequestData &request) const
{
	// Resource handle type

	if ( request.size() < 2 ) {
		parser.errHandler().handleError(
			RIE_MISSINGDATA, RIE_ERROR,
			"Line %ld, File \"%s\", badargument: '%s' at least one of the arguments (%s) is missing",
			parser.lineno(), parser.resourceName(), requestName(), "handle type", RI_NULL);
		return;
	}

	if ( request.size() > 2 ) {
		parser.errHandler().handleError(
			RIE_CONSISTENCY, RIE_WARNING,
			"Line %ld, File \"%s\", badargument: '%s' has additional parameters, they are ignored",
			parser.lineno(), parser.resourceName(), requestName(), RI_NULL);
	}


	CRibParameter &p0 = request[0];
	CRibParameter &p1 = request[1];

	RtToken handle = 0, type = 0;
	if ( !p0.getString(handle) ) {
		parser.errHandler().handleError(
			RIE_CONSISTENCY, RIE_ERROR,
			"Line %ld, File \"%s\", badargument: '%s' argument %s is not a string",
			p1.lineno(), parser.resourceName(), requestName(), "1 (handle)", RI_NULL);
		return;
	}

	if ( !p1.getString(type) ) {
		parser.errHandler().handleError(
			RIE_CONSISTENCY, RIE_ERROR,
			"Line %ld, File \"%s\", badargument: '%s' argument %s is not a string",
			p1.lineno(), parser.resourceName(), requestName(), "2 (type)", RI_NULL);
		return;
	}

	int n = request.getTokenList(2, CParameterClasses());
	if ( n > 0 ) {
		parser.ribFilter().resourceV(handle, type,
			n, request.tokenList(), request.valueList());
	} else {
		parser.ribFilter().resourceV(handle, type,
			0, 0, 0);
	}
}

void CFrameBeginRibRequest::operator()(IRibParserState &parser, CRibRequestData &request) const
{
	// FrameBegin frameno
	RtInt frameNo = 1;
	if ( request.size() < 1 ) {
		parser.errHandler().handleError(
			RIE_CONSISTENCY, RIE_WARNING,
			"Line %ld, File \"%s\", badargument: '%s' argument %s is missing, using value 1.",
			parser.lineno(), parser.resourceName(), requestName(), "1 (frameno)", RI_NULL);
	} else {
		if ( request.size() > 1 ) {
			parser.errHandler().handleError(
				RIE_CONSISTENCY, RIE_WARNING,
				"Line %ld, File \"%s\", badargument: '%s' has additional parameters, they are ignored",
				parser.lineno(), parser.resourceName(), requestName(), RI_NULL);
		}
		CRibParameter &p0 = request[0];
		if ( !p0.getInt(frameNo) ) {
			parser.errHandler().handleError(
				RIE_CONSISTENCY, RIE_WARNING,
				"Line %ld, File \"%s\", badargument: '%s' argument %s is not an integer",
				p0.lineno(), parser.resourceName(), requestName(), "1 (frameno)", RI_NULL);
		}
	}
	parser.ribFilter().frameBegin(frameNo);
}

void CFrameEndRibRequest::operator()(IRibParserState &parser, CRibRequestData &request) const
{
	// FrameEnd
	if ( request.size() != 0 ) {
		parser.errHandler().handleError(
			RIE_CONSISTENCY, RIE_WARNING,
			"Line %ld, File \"%s\", badargument: '%s' has additional parameters, they are ignored",
			parser.lineno(), parser.resourceName(), requestName(), RI_NULL);
	}
	parser.ribFilter().frameEnd();
}

void CWorldBeginRibRequest::operator()(IRibParserState &parser, CRibRequestData &request) const
{
	// WorldBegin
	if ( request.size() != 0 ) {
		parser.errHandler().handleError(
			RIE_CONSISTENCY, RIE_WARNING,
			"Line %ld, File \"%s\", badargument: '%s' has additional parameters, they are ignored",
			parser.lineno(), parser.resourceName(), requestName(), RI_NULL);
	}
	parser.ribFilter().worldBegin();
}

void CWorldEndRibRequest::operator()(IRibParserState &parser, CRibRequestData &request) const
{
	// WorldEnd
	if ( request.size() != 0 ) {
		parser.errHandler().handleError(
			RIE_CONSISTENCY, RIE_WARNING,
			"Line %ld, File \"%s\", badargument: '%s' has additional parameters, they are ignored",
			parser.lineno(), parser.resourceName(), requestName(), RI_NULL);
	}
	parser.ribFilter().worldEnd();
}

void CAttributeBeginRibRequest::operator()(IRibParserState &parser, CRibRequestData &request) const
{
	// AttributeBegin
	if ( request.size() != 0 ) {
		parser.errHandler().handleError(
			RIE_CONSISTENCY, RIE_WARNING,
			"Line %ld, File \"%s\", badargument: '%s' has additional parameters, they are ignored",
			parser.lineno(), parser.resourceName(), requestName(), RI_NULL);
	}
	parser.ribFilter().attributeBegin();
}

void CAttributeEndRibRequest::operator()(IRibParserState &parser, CRibRequestData &request) const
{
	// AttributeEnd
	if ( request.size() != 0 ) {
		parser.errHandler().handleError(
			RIE_CONSISTENCY, RIE_WARNING,
			"Line %ld, File \"%s\", badargument: '%s' has additional parameters, they are ignored",
			parser.lineno(), parser.resourceName(), requestName(), RI_NULL);
	}
	parser.ribFilter().attributeEnd();
}

void CTransformBeginRibRequest::operator()(IRibParserState &parser, CRibRequestData &request) const
{
	// TransformBegin
	if ( request.size() != 0 ) {
		parser.errHandler().handleError(
			RIE_CONSISTENCY, RIE_WARNING,
			"Line %ld, File \"%s\", badargument: '%s' has additional parameters, they are ignored",
			parser.lineno(), parser.resourceName(), requestName(), RI_NULL);
	}
	parser.ribFilter().transformBegin();
}

void CTransformEndRibRequest::operator()(IRibParserState &parser, CRibRequestData &request) const
{
	// TransformEnd
	if ( request.size() != 0 ) {
		parser.errHandler().handleError(
			RIE_CONSISTENCY, RIE_WARNING,
			"Line %ld, File \"%s\", badargument: '%s' has additional parameters, they are ignored",
			parser.lineno(), parser.resourceName(), requestName(), RI_NULL);
	}
	parser.ribFilter().transformEnd();
}
