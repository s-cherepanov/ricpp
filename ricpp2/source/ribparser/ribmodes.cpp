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
			parser.lineNo(), parser.resourceName(), requestName(), RI_NULL);
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
			parser.lineNo(), parser.resourceName(), requestName(), RI_NULL);
	}
	parser.ribFilter().resourceEnd();
}

void CResourceRibRequest::operator()(IRibParserState &parser, CRibRequestData &request) const
{
	// Resource handle type ...

	if ( request.size() < 2 ) {
		parser.errHandler().handleError(
			RIE_MISSINGDATA, RIE_ERROR,
			"Line %ld, File \"%s\", badargument: '%s' at least one of the arguments (%s) is missing",
			parser.lineNo(), parser.resourceName(), requestName(), "handle type", RI_NULL);
		return;
	}

	CRibParameter &p0 = request[0];
	CRibParameter &p1 = request[1];

	RtToken handle = 0, type = 0;
	if ( !p0.getString(handle) ) {
		parser.errHandler().handleError(
			RIE_CONSISTENCY, RIE_ERROR,
			"Line %ld, File \"%s\", badargument: '%s' argument %s is not a string",
			p1.lineNo(), parser.resourceName(), requestName(), "1 (handle)", RI_NULL);
		return;
	}

	if ( !p1.getString(type) ) {
		parser.errHandler().handleError(
			RIE_CONSISTENCY, RIE_ERROR,
			"Line %ld, File \"%s\", badargument: '%s' argument %s is not a string",
			p1.lineNo(), parser.resourceName(), requestName(), "2 (type)", RI_NULL);
		return;
	}

	int n = request.getTokenList(2, CParameterClasses(), RI_RESOURCE, type);
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
			parser.lineNo(), parser.resourceName(), requestName(), "1 (frameno)", RI_NULL);
	} else {
		if ( request.size() > 1 ) {
			parser.errHandler().handleError(
				RIE_CONSISTENCY, RIE_WARNING,
				"Line %ld, File \"%s\", badargument: '%s' has additional parameters, they are ignored",
				parser.lineNo(), parser.resourceName(), requestName(), RI_NULL);
		}
		CRibParameter &p0 = request[0];
		if ( !p0.getInt(frameNo) ) {
			parser.errHandler().handleError(
				RIE_CONSISTENCY, RIE_WARNING,
				"Line %ld, File \"%s\", badargument: '%s' argument %s is not an integer, using value %d",
				p0.lineNo(), parser.resourceName(), requestName(), "1 (frameno)", (int)frameNo, RI_NULL);
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
			parser.lineNo(), parser.resourceName(), requestName(), RI_NULL);
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
			parser.lineNo(), parser.resourceName(), requestName(), RI_NULL);
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
			parser.lineNo(), parser.resourceName(), requestName(), RI_NULL);
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
			parser.lineNo(), parser.resourceName(), requestName(), RI_NULL);
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
			parser.lineNo(), parser.resourceName(), requestName(), RI_NULL);
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
			parser.lineNo(), parser.resourceName(), requestName(), RI_NULL);
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
			parser.lineNo(), parser.resourceName(), requestName(), RI_NULL);
	}
	parser.ribFilter().transformEnd();
}

void CSolidBeginRibRequest::operator()(IRibParserState &parser, CRibRequestData &request) const
{
	// SolidBegin type
	RtToken type = RI_PRIMITIVE;
	if ( request.size() < 1 ) {
		parser.errHandler().handleError(
			RIE_CONSISTENCY, RIE_WARNING,
			"Line %ld, File \"%s\", badargument: '%s' argument %s is missing, using value \"%s\".",
			parser.lineNo(), parser.resourceName(), requestName(), "1 (frameno)", type, RI_NULL);
	} else {
		if ( request.size() > 1 ) {
			parser.errHandler().handleError(
				RIE_CONSISTENCY, RIE_WARNING,
				"Line %ld, File \"%s\", badargument: '%s' has additional parameters, they are ignored",
				parser.lineNo(), parser.resourceName(), requestName(), RI_NULL);
		}
		CRibParameter &p0 = request[0];
		if ( !p0.getString(type) ) {
			parser.errHandler().handleError(
				RIE_CONSISTENCY, RIE_WARNING,
				"Line %ld, File \"%s\", badargument: '%s' argument %s is not a string, using value \"%s\"",
				p0.lineNo(), parser.resourceName(), requestName(), "1 (type)", type, RI_NULL);
		}
	}
	parser.ribFilter().solidBegin(type);
}

void CSolidEndRibRequest::operator()(IRibParserState &parser, CRibRequestData &request) const
{
	// SolidEnd
	if ( request.size() != 0 ) {
		parser.errHandler().handleError(
			RIE_CONSISTENCY, RIE_WARNING,
			"Line %ld, File \"%s\", badargument: '%s' has additional parameters, they are ignored",
			parser.lineNo(), parser.resourceName(), requestName(), RI_NULL);
	}
	parser.ribFilter().solidEnd();
}

void CObjectBeginRibRequest::operator()(IRibParserState &parser, CRibRequestData &request) const
{
	// ObjectBegin number
	// ObjectBegin name (added)

	if ( request.size() >= 1 ) {
		CRibParameter &p0 = request[0];

		RtInt number = 0;
		char namebuf[32];
		const char *name = 0;

		if ( p0.getInt(number) ) {

			RtObjectHandle handle = parser.ribFilter().objectBegin(RI_NULL);

			// Convert handle to string (equivalence of numbers to appropriate strings 42 == "42")
			name = valToStr(namebuf, sizeof(namebuf), number);
			parser.bindObjectHandle(handle, name);
		} else if ( p0.getString(name) ) {

			RtObjectHandle handle = parser.ribFilter().objectBegin(name);

			parser.bindObjectHandle(handle, name);
		} else {
			parser.errHandler().handleError(
				RIE_CONSISTENCY, RIE_ERROR,
				"Line %ld, File \"%s\", badargument: '%s' the type of the object number not numeric or name",
				p0.lineNo(), parser.resourceName(), requestName(), RI_NULL);
			return;
		}
		if ( request.size() > 1 ) {
			parser.errHandler().handleError(
				RIE_CONSISTENCY, RIE_WARNING, "Line %ld, File \"%s\", badargument: '%s' additional parameters are ignored",
				parser.lineNo(), parser.resourceName(), requestName(), RI_NULL);
		}
	} else {
		parser.errHandler().handleError(
			RIE_MISSINGDATA, RIE_ERROR, "Line %ld, File \"%s\", badargument: '%s' the object number is missing.",
			parser.lineNo(), parser.resourceName(), requestName(), RI_NULL);
		return;
	}
}

void CObjectEndRibRequest::operator()(IRibParserState &parser, CRibRequestData &request) const
{
	// ObjectEnd
	if ( request.size() != 0 ) {
		parser.errHandler().handleError(
			RIE_CONSISTENCY, RIE_WARNING,
			"Line %ld, File \"%s\", badargument: '%s' has additional parameters, they are ignored",
			parser.lineNo(), parser.resourceName(), requestName(), RI_NULL);
	}
	parser.ribFilter().objectEnd();
}

void CObjectInstanceRibRequest::operator()(IRibParserState &parser, CRibRequestData &request) const
{
	// ObjectInstance number
	// ObjectInstance name (added)

	if ( request.size() >= 1 ) {
		CRibParameter &p0 = request[0];

		RtInt number = 0;
		char namebuf[32];
		const char *name = 0;
		RtObjectHandle object = 0;

		bool b2 = false;

		bool b1 = p0.getInt(number);
		if ( b1 ) {
			// Temporary object name
			name = valToStr(namebuf, sizeof(namebuf), number);
		} else {
			b2 = p0.getString(name);
		}
		
		if ( b1 || b2 ) {
			if ( b1 ) {
				// Temporary name only
				if ( !parser.getObjectHandle(object, name) ) {
					parser.errHandler().handleError(
						RIE_CONSISTENCY, RIE_ERROR,
						"Line %ld, File \"%s\", badargument: '%s' argument %s, could not find objecthandle for number %d",
						p0.lineNo(), parser.resourceName(), requestName(), "1 (objectnumber)", number, RI_NULL);
					return;
				}
			} else {
				if ( !parser.getObjectHandle(object, name) ) {
					// Try the found string as handle
					object = name;
				}
			}

			parser.ribFilter().objectInstance(object);

			if ( request.size() > 1 ) {
				parser.errHandler().handleError(
					RIE_CONSISTENCY, RIE_WARNING,
					"Line %ld, File \"%s\", badargument: '%s' has additional parameters, they are ignored",
					parser.lineNo(), parser.resourceName(), requestName(), RI_NULL);
			}
		} else {
			parser.errHandler().handleError(
				RIE_CONSISTENCY, RIE_ERROR,
				"Line %ld, File \"%s\", badargument: '%s' argument %s is not numeric or string",
				parser.lineNo(), parser.resourceName(), requestName(), "1 (objectname)", RI_NULL);
		}
	} else {
		parser.errHandler().handleError(
			RIE_CONSISTENCY, RIE_ERROR,
			"Line %ld, File \"%s\", badargument: '%s' argument %s is missing",
			parser.lineNo(), parser.resourceName(), requestName(), "1 (objectname)", RI_NULL);
	}
}

void CArchiveBeginRibRequest::operator()(IRibParserState &parser, CRibRequestData &request) const
{
	// ArchiveBegin name ...

	if ( request.size() >= 1 ) {
		CRibParameter &p0 = request[0];
		const char *name = 0;

		int n = request.getTokenList(1, CParameterClasses());

		if ( p0.getString(name) ) {
			RtArchiveHandle handle = 0;
			if ( n > 0 ) {
				handle = parser.ribFilter().archiveBeginV(name, n, request.tokenList(), request.valueList());
			} else {
				handle = parser.ribFilter().archiveBeginV(name, 0, 0, 0);
			}
			parser.bindArchiveHandle(handle, name);
		} else {
			parser.errHandler().handleError(
				RIE_CONSISTENCY, RIE_ERROR,
				"Line %ld, File \"%s\", badargument: '%s' the type of the archive name not defined",
				p0.lineNo(), parser.resourceName(), requestName(), RI_NULL);
			return;
		}
	} else {
		parser.errHandler().handleError(
			RIE_MISSINGDATA, RIE_ERROR, "Line %ld, File \"%s\", badargument: '%s' the archive name is missing",
			parser.lineNo(), parser.resourceName(), requestName(), RI_NULL);
		return;
	}
}

void CArchiveEndRibRequest::operator()(IRibParserState &parser, CRibRequestData &request) const
{
	// ArchiveEnd
	if ( request.size() != 0 ) {
		parser.errHandler().handleError(
			RIE_CONSISTENCY, RIE_WARNING,
			"Line %ld, File \"%s\", badargument: '%s' has additional parameters, they are ignored",
			parser.lineNo(), parser.resourceName(), requestName(), RI_NULL);
	}
	parser.ribFilter().archiveEnd();
}

void CMotionBeginRibRequest::operator()(IRibParserState &parser, CRibRequestData &request) const
{
	// MotionBegin [ v1 v2 ... vn ]
	if ( request.size() >= 1 ) {
		CRibParameter &p0 = request[0];
		p0.convertIntToFloat();
		if ( p0.isArray() ) {
			if ( p0.typeID() == BASICTYPE_FLOAT ) {
				size_t number = p0.getCard();
				parser.ribFilter().motionBeginV((RtInt)number, (RtFloat *)p0.getValue());
				if ( request.size() > 1 ) {
					parser.errHandler().handleError(
						RIE_CONSISTENCY, RIE_WARNING,
						"Line %ld, File \"%s\", badargument: '%s' additional parameters are ignored",
						parser.lineNo(), parser.resourceName(), requestName(), RI_NULL);
				}
			} else {
				parser.errHandler().handleError(
					RIE_CONSISTENCY, RIE_ERROR,
					"Line %ld, File \"%s\", badargument: '%s' array doesn't contain floats",
					p0.lineNo(), parser.resourceName(), requestName(), RI_NULL);
				return;
			}
		} else {
			parser.errHandler().handleError(
				RIE_CONSISTENCY, RIE_ERROR,
				"Line %ld, File \"%s\", badargument: '%s' the float array is missing",
				p0.lineNo(), parser.resourceName(), requestName(), RI_NULL);
			return;
		}
	} else {
		parser.errHandler().handleError(
			RIE_MISSINGDATA, RIE_ERROR,
			"Line %ld, File \"%s\", badargument: '%s' the float array is missing",
			parser.lineNo(), parser.resourceName(), requestName(), RI_NULL);
		return;
	}
}

void CMotionEndRibRequest::operator()(IRibParserState &parser, CRibRequestData &request) const
{
	// MotionEnd
	if ( request.size() != 0 ) {
		parser.errHandler().handleError(
			RIE_CONSISTENCY, RIE_WARNING,
			"Line %ld, File \"%s\", badargument: '%s' has additional parameters, they are ignored",
			parser.lineNo(), parser.resourceName(), requestName(), RI_NULL);
	}
	parser.ribFilter().motionEnd();
}

void CIfBeginRibRequest::operator()(IRibParserState &parser, CRibRequestData &request) const
{
	// IfBegin condition
	RtString condition = "true";
	if ( request.size() < 1 ) {
		parser.errHandler().handleError(
			RIE_CONSISTENCY, RIE_WARNING,
			"Line %ld, File \"%s\", badargument: '%s' argument %s is missing, using '%s'",
			parser.lineNo(), parser.resourceName(), requestName(), "1 (condition)", condition, RI_NULL);
	} else {
		if ( request.size() > 1 ) {
			parser.errHandler().handleError(
				RIE_CONSISTENCY, RIE_WARNING,
				"Line %ld, File \"%s\", badargument: '%s' has additional parameters, they are ignored",
				parser.lineNo(), parser.resourceName(), requestName(), RI_NULL);
		}
		CRibParameter &p0 = request[0];
		if ( !p0.getString(condition) ) {
			parser.errHandler().handleError(
				RIE_CONSISTENCY, RIE_WARNING,
				"Line %ld, File \"%s\", badargument: '%s' argument %s is not a string, using '%s'",
				p0.lineNo(), parser.resourceName(), requestName(), "1 (condition)", condition, RI_NULL);
		}
	}
	parser.ribFilter().ifBegin(condition);
}

void CElseIfRibRequest::operator()(IRibParserState &parser, CRibRequestData &request) const
{
	// ElseIf condition
	RtString condition = "true";
	if ( request.size() < 1 ) {
		parser.errHandler().handleError(
			RIE_CONSISTENCY, RIE_WARNING,
			"Line %ld, File \"%s\", badargument: '%s' argument %s is missing, using '%s'",
			parser.lineNo(), parser.resourceName(), requestName(), "1 (condition)", condition, RI_NULL);
	} else {
		if ( request.size() > 1 ) {
			parser.errHandler().handleError(
				RIE_CONSISTENCY, RIE_WARNING,
				"Line %ld, File \"%s\", badargument: '%s' has additional parameters, they are ignored",
				parser.lineNo(), parser.resourceName(), requestName(), RI_NULL);
		}
		CRibParameter &p0 = request[0];
		if ( !p0.getString(condition) ) {
			parser.errHandler().handleError(
				RIE_CONSISTENCY, RIE_WARNING,
				"Line %ld, File \"%s\", badargument: '%s' argument %s is not a string, using '%s'",
				p0.lineNo(), parser.resourceName(), requestName(), "1 (condition)", condition, RI_NULL);
		}
	}
	parser.ribFilter().elseIfBegin(condition);
}

void CElseRibRequest::operator()(IRibParserState &parser, CRibRequestData &request) const
{
	// Else
	if ( request.size() != 0 ) {
		parser.errHandler().handleError(
			RIE_CONSISTENCY, RIE_WARNING,
			"Line %ld, File \"%s\", badargument: '%s' has additional parameters, they are ignored",
			parser.lineNo(), parser.resourceName(), requestName(), RI_NULL);
	}
	parser.ribFilter().elseBegin();
}

void CIfEndRibRequest::operator()(IRibParserState &parser, CRibRequestData &request) const
{
	// IfEnd
	if ( request.size() != 0 ) {
		parser.errHandler().handleError(
			RIE_CONSISTENCY, RIE_WARNING,
			"Line %ld, File \"%s\", badargument: '%s' has additional parameters, they are ignored",
			parser.lineNo(), parser.resourceName(), requestName(), RI_NULL);
	}
	parser.ribFilter().ifEnd();
}
