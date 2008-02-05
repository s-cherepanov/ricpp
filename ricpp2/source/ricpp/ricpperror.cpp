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

/** @file ricpperror.cpp
 *  @author Andreas Pidde (andreas@pidde.de)
 *  @brief Error exception and base for errorhandling used internally for RiCPP
 */

#include "ricpp/ricpp/ricpperror.h"
#include "ricpp/ricpp/ricpptokens.h"
#include <cstdarg>

using namespace RiCPP;

// Error messages RIE_ as clear text

RtString CRiCPPErrMsg::errorMessage(RtInt aCode)
{
	RtString riErrorMessages[] = {
		"RIE_NOERROR",
		"RIE_NOMEM: Out of memory",
		"RIE_SYSTEM: Miscellaneous system error",
		"RIE_NOFILE: File nonexistant",
		"RIE_BADFILE: Bad file format",
		"RIE_VERSION: File version mismatch",
		"RIE_DISKFULL: Target disk is full",
		RI_UNKNOWN,
		RI_UNKNOWN,
		RI_UNKNOWN,
		RI_UNKNOWN,
		"RIE_INCAPABLE, RIE_OPTIONAL: Optional RI feature",
		"RIE_UNIMPLEMENT: Unimplemented feature",
		"RIE_LIMIT: Arbitrary program limit",
		"RIE_BUG: Probably a bug in renderer",
		RI_UNKNOWN,
		RI_UNKNOWN,
		RI_UNKNOWN,
		RI_UNKNOWN,
		RI_UNKNOWN,
		RI_UNKNOWN,
		RI_UNKNOWN,
		RI_UNKNOWN,
		"RIE_NOTSTARTED: RiBegin not called",
		"RIE_NESTING: Bad begin-end nesting",
		"RIE_NOTOPTIONS: Invalid state for options",
		"RIE_NOTATTRIBS: Invalid state for attributes",
		"RIE_NOTPRIMS: Invalid state for primitives",
		"RIE_ILLSTATE: Other invalid state",
		"RIE_BADMOTION: Badly formed motion block",
		"RIE_BADSOLID: Badly formed solid block",
		RI_UNKNOWN,
		RI_UNKNOWN,
		RI_UNKNOWN,
		RI_UNKNOWN,
		RI_UNKNOWN,
		RI_UNKNOWN,
		RI_UNKNOWN,
		RI_UNKNOWN,
		RI_UNKNOWN,
		RI_UNKNOWN,
		"RIE_BADTOKEN: Invalid token for request",
		"RIE_RANGE: Parameter out of range",
		"RIE_CONSISTENCY: Parameters inconsistent",
		"RIE_BADHANDLE: Bad object, light or context handle",
		"RIE_NOSHADER: Can't load requested shader",
		"RIE_MISSINGDATA: Required parameters not provided",
		"RIE_SYNTAX: Declare type syntax error",
		RI_UNKNOWN,
		RI_UNKNOWN,
		RI_UNKNOWN,
		RI_UNKNOWN,
		RI_UNKNOWN,
		RI_UNKNOWN,
		RI_UNKNOWN,
		RI_UNKNOWN,
		RI_UNKNOWN,
		RI_UNKNOWN,
		RI_UNKNOWN,
		RI_UNKNOWN,
		RI_UNKNOWN,
		"RIE_MATH: Zerodivide, noninvert matrix, etc.",
		RI_NULL
	};

	if ( aCode < 0 || aCode > RIE_LASTERROR )
		return RI_UNKNOWN;
	return riErrorMessages[(int)aCode];
}

RtString CRiCPPErrMsg::errorSeverity(RtInt aSeverity)
{
	RtString riErrorSeverity[] = {
		"RIE_INFO: Info",
		"RIE_WARNING: Warning",
		"RIE_ERROR: Error",
		"RIE_SEVERE: Severe",
		RI_NULL
	};
	if ( aSeverity < 0 || aSeverity > RIE_LASTSEVERITY )
		return RI_UNKNOWN;
	return riErrorSeverity[(int)aSeverity];
}

// Renderer Error

// display Message

const char *ExceptRiCPPError::formatErrorMessage(std::string &strCode) const
{
	char buffer[64];
	strCode.clear();

	if ( !m_file.empty() ) {
		strCode += "File [";
		strCode += valToStr(buffer, sizeof(buffer), m_line);;
		strCode += ": ";
		strCode += m_file;
		strCode += "]";
	}

	if ( !m_message.empty() ) {
		if ( !m_message.empty() ) {
			strCode += " - ";
		}
		strCode += m_message;
	}

	return strCode.c_str();
}

const char *ExceptRiCPPError::formatError(std::string &strCode) const
{
	char buffer[64];
	
	strCode = "Error [";
	strCode += valToStr(buffer, sizeof(buffer), m_code);
	strCode += ": ";
	strCode += CRiCPPErrMsg::errorMessage(m_code);
	strCode += "] Severity [";
	strCode += buffer;
	strCode += valToStr(buffer, sizeof(buffer), m_severity);
	strCode += ": ";
	strCode += CRiCPPErrMsg::errorSeverity(m_severity);
	strCode += "]";

	std::string temp;
	temp.clear();

	formatErrorMessage(temp);
	if ( !temp.empty() ) {
		strCode += temp;
	}

	return strCode.c_str();
}


ExceptRiCPPError::ExceptRiCPPError(
	RtInt aCode, RtInt aSeverity,
	long aLine, const char *aFile,
	RtString aMessage, ...
	)
{
	static const int ERROR_STR_SIZE = 256;
	va_list argList;
	va_start(argList, aMessage);
	if ( aMessage && *aMessage && argList ) {
		char str[ERROR_STR_SIZE];
		str[0] = (char)0;
#ifdef WIN32
		_vsnprintf_s(str, ERROR_STR_SIZE-1, ERROR_STR_SIZE-2, aMessage, argList);
#else
		vsnprintf(str, ERROR_STR_SIZE-1, aMessage, argList);
#endif
		str[ERROR_STR_SIZE-1] = (char)0;
		aMessage = str;
	}
	va_end(argList);
	set(aCode, aSeverity, aMessage, aLine, aFile);
}

void ExceptRiCPPError::set(
	RtInt aCode, RtInt aSeverity,
	long aLine, const char *aFile,
	RtString aMessage, ...
	)
{
	static const int ERROR_STR_SIZE = 256;
	va_list argList;
	va_start(argList, aMessage);
	if ( aMessage && *aMessage && argList ) {
		char str[ERROR_STR_SIZE];
		str[0] = (char)0;
#ifdef WIN32
		_vsnprintf_s(str, ERROR_STR_SIZE-1, ERROR_STR_SIZE-2, aMessage, argList);
#else
		vsnprintf(str, ERROR_STR_SIZE-1, aMessage, argList);
#endif
		str[ERROR_STR_SIZE-1] = (char)0;
		aMessage = str;
	}
	va_end(argList);
	set(aCode, aSeverity, aMessage, aLine, aFile);
}

// ----------------------------------------------------------------------------

IRiCPPErrorHandler::~IRiCPPErrorHandler() {}

RtVoid IRiCPPErrorHandler::handleErrorV(RtInt code, RtInt severity, RtString message, va_list argList=0)
{
	handleErrorV(code, severity, 0, 0, message, argList);
}

RtVoid IRiCPPErrorHandler::handleError(RtInt code, RtInt severity, int line, const char *file, RtString message, ...)
{
	va_list argList;
	va_start(argList, message);
	handleErrorV(code, severity, line, file, message, argList);
	va_end(argList);
}

RtVoid IRiCPPErrorHandler::handleError(RtInt code, RtInt severity, RtString message, ...)
{
	va_list argList;
	va_start(argList, message);
	handleErrorV(code, severity, 0, 0, message, argList);
	va_end(argList);
}
RtVoid IRiCPPErrorHandler::handleError(const ExceptRiCPPError &err)
{
	handleErrorV(err.code(), err.severity(), err.line(), err.file(), err.what(), 0);
}

CErrorExceptionHandler::~CErrorExceptionHandler() {}

RtVoid CErrorExceptionHandler::handleErrorV(RtInt code, RtInt severity, int line, const char *file, RtString message, va_list argList) {
	static const int ERROR_STR_SIZE = 256;
	if ( message && *message && argList ) {
		char str[ERROR_STR_SIZE];
		str[0] = (char)0;
#ifdef WIN32
		_vsnprintf_s(str, ERROR_STR_SIZE-1, ERROR_STR_SIZE-2, message, argList);
#else
		vsnprintf(str, ERROR_STR_SIZE-1, message, argList);
#endif
		str[ERROR_STR_SIZE-1] = (char)0;
		message = str;
	}

	throw ExceptRiCPPError(code, severity, message, line, file);
}
