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

/** @file ribmisc.cpp
 *  @author Andreas Pidde (andreas@pidde.de)
 *  @brief Implementation of the handling of RIB requests, that cannot be sorted to the other groups (incl. Version).
 */

#include "ricpp/ribparser/ribmisc.h"

using namespace RiCPP;

void CErrorHandlerRibRequest::operator()(IRibParserState &parser, CRibRequestData &request) const
{
	// ErrorHandler handlername

	if ( request.size() >= 1 ) {

		CRibParameter &p0 = request[0];
		const char *handlerName = 0;

		if ( p0.getString(handlerName) ) {

			if ( !strcmp(handlerName, CAbortErrorHandler::myName()) ) {
				parser.ribFilter().errorHandler(CAbortErrorHandler::func);
			} else if ( !strcmp(handlerName, CPrintErrorHandler::myName()) ) {
				parser.ribFilter().errorHandler(CPrintErrorHandler::func);
			} else if ( !strcmp(handlerName, CIgnoreErrorHandler::myName()) ) {
				parser.ribFilter().errorHandler(CIgnoreErrorHandler::func);
			} else {
				parser.errHandler().handleError(
					RIE_CONSISTENCY, RIE_ERROR,
					"Line %ld, File \"%s\", badargument: '%s', handler \"%s\" is unknown",
					parser.lineNo(), parser.resourceName(),
					requestName(), handlerName, RI_NULL);
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
				RIE_CONSISTENCY, RIE_ERROR,
				"Line %ld, File \"%s\", badargument: '%s' argument %s is not a string",
				p0.lineNo(), parser.resourceName(),
				requestName(), "1 (variation)", RI_NULL);
		}

	} else {

		parser.errHandler().handleError(
			RIE_MISSINGDATA, RIE_ERROR,
			"Line %ld, File \"%s\", badargument: '%s' argument %s missing",
			parser.lineNo(), parser.resourceName(),
			requestName(), "(handlerName)", RI_NULL);
	}
}

void CDeclareRibRequest::operator()(IRibParserState &parser, CRibRequestData &request) const
{
	if ( request.size() >= 1 ) {
		CRibParameter &p0 = request[0];
		const char *name = 0;
		if ( !p0.getString(name) ) {
			parser.errHandler().handleError(
				RIE_CONSISTENCY, RIE_ERROR,
				"Line %ld, File \"%s\", badargument: '%s' argument %s is not a string",
				parser.lineNo(), parser.resourceName(),
				requestName(), "1 (name)", RI_NULL);
			return;
		}
		const char *declaration = 0;
		if ( request.size() > 1 ) {
			CRibParameter &p1 = request[1];
			if ( !p1.getString(declaration) ) {
				parser.errHandler().handleError(
					RIE_CONSISTENCY, RIE_ERROR,
					"Line %ld, File \"%s\", badargument: '%s' argument %s is not a string",
					parser.lineNo(), parser.resourceName(),
					requestName(), "2 (declaration)", RI_NULL);
				return;
			}
		}

		parser.ribFilter().declare(name, declaration);

		if ( request.size() > 2 ) {
			parser.errHandler().handleError(
				RIE_CONSISTENCY, RIE_WARNING,
				"Line %ld, File \"%s\", badargument: '%s' has additional parameters, they are ignored",
				parser.lineNo(), parser.resourceName(),
				requestName(), RI_NULL);
		}
	} else {
		// No Parameters
		parser.errHandler().handleError(
			RIE_MISSINGDATA, RIE_ERROR,
			"Line %ld, File \"%s\", badargument: '%s' arguments %s missing",
			parser.lineNo(), parser.resourceName(),
			requestName(), "(name declaration)", RI_NULL);
	}
}

void CReadArchiveRibRequest::operator()(IRibParserState &parser, CRibRequestData &request) const
{
	// ReadArchive filename
	if ( request.size() >= 1 ) {
		CRibParameter &p0 = request[0];
		const char *name=0;

		if ( p0.getString(name) ) {
			// Propagate the callback for Archives in Archives
			parser.ribFilter().readArchiveV(name, parser.callback(), (RtInt)parser.params().size(), parser.params().tokenPtr(), parser.params().valuePtr());
		} else {
			parser.errHandler().handleError(
				RIE_CONSISTENCY, RIE_ERROR,
				"Line %ld, File \"%s\", badargument: '%s' argument %s is not a string",
				parser.lineNo(), parser.resourceName(),
				requestName(), "1 (filename)", RI_NULL);
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
			requestName(), "(filename)", RI_NULL);
	}
}

void CVersionRibRequest::operator()(IRibParserState &parser, CRibRequestData &request) const
{
	if ( request.size() >= 1 ) {
		CRibParameter &p0 = request[0];
		RtFloat version;
		if ( p0.getFloat(version) ) {
			RtInt ver = (RtInt)(version*100.0+.5);
			if ( ver != 303 ) {
				parser.errHandler().handleError(
					RIE_CONSISTENCY, RIE_WARNING,
					"Line %ld, File \"%s\", illversion: '%s' has an unknown version number, using 3.03",
					parser.lineNo(), parser.resourceName(),
					requestName(), RI_NULL);
			}
			// maybe sprintf(versionstr, "version %1.2f\n", version);
			// Print the version number as verbatim string, so it appears if the
			// RIB input is copied to output. Version has no C++-binding.
			parser.ribFilter().archiveRecordV(RI_VERBATIM, "version 3.03\n");
		} else {
			parser.errHandler().handleError(
				RIE_CONSISTENCY, RIE_ERROR,
				"Line %ld, File \"%s\", badargument: '%s' argument %s is not numeric",
				parser.lineNo(), parser.resourceName(),
				requestName(), "1 (versionNo) - should be 3.03 -", RI_NULL);
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
			requestName(), "1 (versionNo) - should be 3.03 -", RI_NULL);
	}
}
