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
#include <cstdarg>

using namespace RiCPP;

// Renderer Error

// display Message

const char *ExceptRiCPPError::formatErrorMessage(std::string &strCode) const
{
	char buffer[64];
	strCode.clear();

	if ( !m_file.empty() ) {
		strCode += "File [";
#ifdef WIN32
	sprintf_s(buffer, sizeof(buffer), "%d", m_line);
#else
	snprintf(buffer, sizeof(buffer)-1, "%d", m_line);
#endif
		strCode += buffer;
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
	buffer[0]=(char)0;
#ifdef WIN32
	sprintf_s(buffer, sizeof(buffer), "%d", m_code);
#else
	snprintf(buffer, sizeof(buffer)-1, "%d", m_code);
#endif
	buffer[sizeof(buffer)-1] = (char)0;
	strCode += buffer;
	strCode += ": ";
	strCode += CRiCPPErrMsg::errorMessage(m_code);
	strCode += "] Severity [";
	buffer[0]=(char)0;
#ifdef WIN32
	sprintf_s(buffer, sizeof(buffer), "%d", m_severity);
#else
	snprintf(buffer, sizeof(buffer)-1, "%d", m_severity);
#endif
	buffer[sizeof(buffer)-1] = (char)0;
	strCode += buffer;
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
	int aLine, const char *aFile,
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
	int aLine, const char *aFile,
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
