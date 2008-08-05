#ifndef _RICPP_RICPP_RICPPERROR_H
#define _RICPP_RICPP_RICPPERROR_H

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

/** @file ricpperror.h
 *  @author Andreas Pidde (andreas@pidde.de)
 *  @brief Error exception and base for error handling used internally for RiCPP
 */

#ifndef _RICPP_RIBASE_RICPPCONST_H
#include "ricpp/ribase/ricppconst.h"
#endif // _RICPP_RIBASE_RICPPCONST_H

#ifndef _RICPP_TOOLS_INLINETOOLS_H
#include "ricpp/tools/inlinetools.h"
#endif // _RICPP_TOOLS_INLINETOOLS_H

#include <exception>

namespace RiCPP {

#if defined(__GNUC__)
/** @brief Symbolic name for visibility("default") attribute.
 * (MAC)
 * see http://developer.apple.com/technotes/tn2007/tn2185.html
 */
#pragma GCC visibility push(default)
#endif

/** @brief Helper class, errormessages as Strings
 */
class CRiCPPErrMsg {
public:
	/** Returns the descriptive string for an error code
	 *  @param aCode The error code
	 *  @return error string for the error code aCode
	 */
	static RtString errorMessage(RtInt aCode);

	/** Returns the descriptive string for a severity level
	 *  @param aSeverity severity level
	 *  @return The descriptive string for an error severity level aSeverity
	 */
	static RtString errorSeverity(RtInt aSeverity);
}; // CRiCPPErrMsg

/** @brief The renderer error is used internally by the back end to throw exceptions
 *
 *  Since Apple discourages the use STL for the exported interface of shared libraries
 *  ExceptRiCPPError will not inherit from std::exception.
 */
class ExceptRiCPPError // : public std::exception
{
	RtInt m_code;           ///< @brief Which of error occured, 'RIE_...'
	RtInt m_severity;       ///< @brief Severity level RIE_INFO, RIE_WARNING, RIE_ERROR, RIE_SEVERE
	std::string m_message;  ///< @brief error string
	long m_line;            ///< @brief Line where the error occured
	std::string m_file;     ///< @brief File where the error occured

public:
	/** @brief Constructor, sets error codes and additional error message with source line and source file added
	 *  @param aCode Which error ('RIE_...')
	 *  @param aSeverity Severity level RIE_INFO, RIE_WARNING, RIE_ERROR, RIE_SEVERE
	 *  @param aMessage Additional describing error string
	 *  @param aLine Line number of the source file where the ExceptRiCPPError is constructed (the error occured), normally __LINE__
	 *  @param aFile Filke where the ExceptRiCPPError occured, normally __FILE__
	 */
	 inline ExceptRiCPPError(
		 RtInt aCode = RIE_NOERROR, RtInt aSeverity = RIE_ERROR,
		 RtString aMessage = 0,
		 long aLine = 0, const char *aFile = 0
		 )
	 {
		 set(aCode, aSeverity, aMessage, aLine, aFile);
	 }

	/** @brief Constructor, sets error codes and additional error message with source line and source file added
	 *  @param aCode Which error ('RIE_...')
	 *  @param aSeverity Severity level RIE_INFO, RIE_WARNING, RIE_ERROR, RIE_SEVERE
	 *  @param aLine Line number of the source file where the ExceptRiCPPError is constructed (the error occured), normally __LINE__
	 *  @param aFile Filke where the ExceptRiCPPError occured, normally __FILE__
	 *  @param aMessage format string (like printf) describing the error, followed by
	 *  first argument like in printf, if there is not such an argument formating is not done
	 */
	 ExceptRiCPPError(
		 RtInt aCode, RtInt aSeverity,
		 long aLine, const char *aFile,
		 RtString aMessage, ...
		 );

	/** @brief Copy Constructor
	 *  @param err The error object to copy
	 */
	inline ExceptRiCPPError(const ExceptRiCPPError &err)
	{
		*this = err;
	}

	/** @brief Virtual destructor
	 */
	inline virtual ~ExceptRiCPPError()
	{}

	/** @brief Sets error codes and additional error message with source line and source file added
	 *  @param aCode Which error ('RIE_...')
	 *  @param aSeverity Severity level RIE_INFO, RIE_WARNING, RIE_ERROR, RIE_SEVERE
	 *  @param aMessage Additional describing error string
	 *  @param aLine Line number of the source file where the ExceptRiCPPError is constructed (the error occured), normally __LINE__
	 *  @param aFile Filke where the ExceptRiCPPError occured, normally __FILE__
	 */
	 inline void set(
		 RtInt aCode = RIE_NOERROR, RtInt aSeverity = RIE_ERROR,
		 RtString aMessage = 0,
		 long aLine = 0, const char *aFile = 0
		 )
	 {
		m_code = aCode;
		m_severity = aSeverity;
		m_message = aMessage ? aMessage : "";
		m_line = aLine;
		m_file = aFile ? aFile : "";
	 }

	/** @brief Sets error codes and additional error message with source line and source file added
	 *  @param aCode Which error ('RIE_...')
	 *  @param aSeverity Severity level RIE_INFO, RIE_WARNING, RIE_ERROR, RIE_SEVERE
	 *  @param aLine Line number of the source file where the ExceptRiCPPError is constructed (the error occured), normally __LINE__
	 *  @param aFile Filke where the ExceptRiCPPError occured, normally __FILE__
	 *  @param aMessage format string (like printf) describing the error, followed by
	 *  first argument like in printf, if there is not such an argument formating is not done
	 */
	 void set(
		 RtInt aCode, RtInt aSeverity,
		 long aLine, const char *aFile,
		 RtString aMessage, ...
		 );

	/** @brief Formats the error/severity numbers to a string.
	 *
	 *  The format is "Error [number:description] Severity [number:description] formated error message",
	 *  File and message parts are optional.
	 *  @retval strCode The error is formatted to this string
	 *  @return strCode.c_str()
	 */
	const char *formatError(std::string &strCode) const;

	/** @brief Formats the error/severity numbers to a string.
	 *
	 *  The format is "File [<line>: <file>] - <message>" if file and massege are set,
	 *  it can missing the file info, message or can be empty.
	 *  @retval strCode The error is formatted to this string
	 *  @return strCode.c_str()
	 */
	const char *formatErrorMessage(std::string &strCode) const;

	/** @brief Sets the file name string.
	 *  @param aFile Pointer to a file name
	 */
	inline void file(const char *aFile)
	{
		m_file = noNullStr(aFile);
	}

	/** @brief Gets the file name string.
	 *  @return Pointer to the file name stored in @a ExceptRiCPPError::m_file
	 */
	inline const char *file() const
	{
		return m_file.c_str();
	}

	/** @brief Sets the line number.
	 *  @name aLine A line number
	 */
	inline void line(long aLine)
	{
		m_line = aLine;
	}

	/** @brief Gets the line number.
	 *  @return Line number stored in @a ExceptRiCPPError::m_line
	 */
	inline long line() const
	{
		return m_line;
	}

	/** Gets the current error string
	 *  @return Pointer to the error message stored in @a ExceptRiCPPError::m_message
	 */
	inline virtual const char *what() const throw()
	{
		return m_message.c_str();
	}

	/** @brief Gets the current error code.
	 *  @return error code stored in @a RendererError::m_code
	 */
	inline RtInt code() const
	{
		return m_code;
	}

	/** @brief Is the error code set?
	 *  @return true, if the error code is not RIE_NOERROR
	 */
	inline bool isError() const
	{
		return m_code != RIE_NOERROR;
	}

	/** @brief Get the current error severity.
	 *  @return error severity stored in @a ExceptRiCPPError::m_severity
	 */
	inline RtInt severity() const
	{
		return m_severity;
	}

	/** @brief Copy an Error.
	 *  @param err The error object to copy
	 *  @return *this
	 */
	inline ExceptRiCPPError &operator=(const ExceptRiCPPError &err)
	{
		if ( this == &err )
			return *this;
		set(err.m_code, err.m_severity, err.m_message.c_str(), err.m_line, err.m_file.c_str());
		return *this;
	}
}; // ExceptRiCPPError


/** @brief Interface for easier error handling
 *
 *  CErrorExceptionHandler and CRiCPPBridge implement this interface to manage
 *  the error handling.
 */
class IRiCPPErrorHandler {
public:
	/** @brief Virtual destructor
	 */
	virtual ~IRiCPPErrorHandler();

	/** @brief Handles an error with line and file
	 *
	 * Forward to @a handleErrorV(). Variable parameters like in printf(),
	 * if not known if message contains a format string use
	 * handleError(code, severity, line, file, "%s", message) or handleErrorV
	 * @param code Error Code (RIE_...)
	 * @param severity Severity level of the error (RIE_INFO, ..., RIE_SEVERE)
	 * @param line Line number where error occured
	 * @param file file where error occured
	 * @param message Format string (like in printf())
	 */
	virtual RtVoid handleError(RtInt code, RtInt severity, int line, const char *file, RtString message, ...);

	/** @brief handle an error
	 *
	 * Forward to @a handleErrorV(). Variable parameters like in printf(),
	 * if not known if message contains a format string use
	 * handleError(code, severity, "%s", message) or handleErrorV
	 * @param code Error Code (RIE_...)
	 * @param severity Severity level of the error (RIE_INFO, ..., RIE_SEVERE)
	 * @param message Format string (like in printf())
	 */
	virtual RtVoid handleError(RtInt code, RtInt severity, RtString message, ...);

	/** @brief Forward the contents of an ExceptRiCPPError to handleErrorV()
	 *  @param err Error Exception
	 */
	virtual RtVoid handleError(const ExceptRiCPPError &err);

	/** Forwards to handleErrorV with cleard line and filename
	 * @param code Error Code (RIE_...)
	 * @param severity Severity level of the error (RIE_INFO, ..., RIE_SEVERE)
	 * @param message Format string (like in printf())
	 * @param argList variable list of parameters, if 0 message is treted like a string without format symbols
	 */
	virtual RtVoid handleErrorV(RtInt code, RtInt severity, RtString message, va_list argList=0);

	/** @brief Handles an error must be overloaded for concrete error handling
	 * @param code Error Code (RIE_...)
	 * @param severity Severity level of the error (RIE_INFO, ..., RIE_SEVERE)
	 * @param line Line number where error occured
	 * @param file file where error occured
	 * @param message Format string (like in printf()), not formatted if argList==0
	 * @param argList variable list of parameters, if 0 message is treted like a string without format symbols
	 */
	virtual RtVoid handleErrorV(RtInt code, RtInt severity, int line, const char *file, RtString message, va_list argList=0) = 0;
}; // IRiCPPErrorHandler

/** @brief The standard error handler of the backend.
 *
 *  Used to format error messages and throw
 *  an ExceptRiCPPError.
 */
class CErrorExceptionHandler : public IRiCPPErrorHandler {
public:
	/** @brief Virtual destructor
	 */
	virtual ~CErrorExceptionHandler();
	
	inline virtual RtVoid handleErrorV(RtInt code, RtInt severity, RtString message, va_list argList=0)
	{
		IRiCPPErrorHandler::handleErrorV(code, severity, message, argList);
	}
	
	/** @brief Handles an error, sets m_lastError and calls the current error handler
	 * @param code Error Code (RIE_...)
	 * @param severity Severity level of the error (RIE_INFO, ..., RIE_SEVERE)
	 * @param line Line number where error occured
	 * @param file file where error occured
	 * @param message Format string (like in printf()), not formatted if argList==0
	 * @param argList variable list of parameters, if 0 message is treted like a string without format symbols
	 */
	virtual RtVoid handleErrorV(RtInt code, RtInt severity, int line, const char *file, RtString message, va_list argList=0);
}; // CErrorExceptionHandler

#if defined(__GNUC__)
#pragma GCC visibility pop
#endif

} // namespace RiCPP

#endif // _RICPP_RICPP_RICPPERROR_H
