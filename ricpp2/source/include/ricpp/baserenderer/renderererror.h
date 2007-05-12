#ifndef _RICPP_BASERENDERER_RENDERERERROR_H
#define _RICPP_BASERENDERER_RENDERERERROR_H

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

/** @file renderererror.h
 *  @author Andreas Pidde (andreas@pidde.de)
 *  @brief Error exception used internally for RiCPP
 */

#ifndef _RICPP_RICPP_RICPP_H
#include "ricpp/ricpp/ricpp.h"
#endif // _RICPP_RICPP_RICPP_H

#include <string>

namespace RiCPP {

/** @brief The renderer error is used internally by the back end to throw exceptions
 */
class ERendererError {
	RtInt m_severity;       ///< Severity level RIE_INFO, RIE_WARNING, RIE_ERROR, RIE_SEVERE
	RtInt m_code;           ///< Which of error occured, 'RIE_...'
	std::string m_message;  ///< error string
	int m_line;             ///< Line where the error occured
	std::string m_file;     ///< File where the error occured

public:
	/** Constructor, sets error codes and additional error message with source line and source file added
	 *  @param code Which error ('RIE_...')
	 *  @param severity Severity level RIE_INFO, RIE_WARNING, RIE_ERROR, RIE_SEVERE
	 *  @param message Additional describing error string
	 *  @param aLine Line number of the source file where the ERendererError is constructed (the error occured), normally __LINE__
	 *  @param aFile Filke where the ERendererError occured, normally __FILE__
	 */
	 ERendererError(
		 RtInt aCode, RtInt aSeverity = RIE_ERROR,
		 const char *aMessage = NULL,
		 int aLine = 0, const char *aFile = NULL
		 )
		: m_severity(aSeverity), m_code(aCode),
		m_message(aMessage ? aMessage : ""),
		m_line(aLine), m_file(aFile ? aFile : "")
	 {
	 }

	/** Virtual destructor
	 */
	virtual inline ~ERendererError() {}

	/** Formats the error/severity numbers to a string.
	 *  The format is "Error [<number>:<description>] Severity [<number>:<description>] <formated error message>",
	 *  File and message parts are optional.
	 *  @retVal strCode The error is formatted to this string
	 *  @return strCode.c_str()
	 */
	virtual const char *formatError(std::string &strCode) const;

	/** Formats the error/severity numbers to a string.
	 *  The format is "File [<line>: <file>] - <message>" if file and massege are set,
	 *  it can missing the file info, message or can be empty.
	 *  @retVal strCode The error is formatted to this string
	 *  @return strCode.c_str()
	 */
	virtual const char *formatErrorMessage(std::string &strCode) const;

	/** Gets the file string
	 *  @return Pointer to the file name stored in \a ERendererError::m_file
	 */
	inline const char *file() const
	{
		return m_file.c_str();
	}

	/** Gets the line number
	 *  @return Line number stored in \a ERendererError::m_line
	 */
	inline int line() const
	{
		return m_line;
	}

	/** Gets the current error string
	 *  @return Pointer to the error message stored in \a ERendererError::m_message
	 */
	inline const char *what() const
	{
		return m_message.c_str();
	}

	/** Gets the current error code
	 *  @return error code stored in \a RendererError::m_code
	 */
	inline RtInt code() const
	{
		return m_code;
	}

	/** Get the current error severity
	 *  @return error severity stored in \a ERendererError::m_severity
	 */
	inline RtInt severity() const
	{
		return m_severity;
	}

}; // ERendererError

/** The standard error handler of the backend to format error messages and throw
 *  an ERendererError.
 */
class CErrorExceptionHandler {
public:
	/** Virtual destructor
	 */
	virtual inline ~CErrorExceptionHandler() {}

	/** Error Handling, throws an ERendererError
	 */
	//@{

	/** @brief Handles an error with line and file
	 *
	 * Forward to \a handleErrorV(). Variable parameters like in printf(),
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
	 * Forward to \a handleErrorV(). Variable parameters like in printf(),
	 * if not known if message contains a format string use
	 * handleError(code, severity, "%s", message) or handleErrorV
	 * @param code Error Code (RIE_...)
	 * @param severity Severity level of the error (RIE_INFO, ..., RIE_SEVERE)
	 * @param message Format string (like in printf())
	 */
	virtual RtVoid handleError(RtInt code, RtInt severity, RtString message, ...);

	/** @brief Simply rethrows an exception
	 *  @param err Error Exception
	 */
	virtual RtVoid handleError(const ERendererError &err);

	/** Handles an error, sets m_lastError and calls the current error handler
	 * @param code Error Code (RIE_...)
	 * @param severity Severity level of the error (RIE_INFO, ..., RIE_SEVERE)
	 * @param message Format string (like in printf())
	 * @param argList variable list of parameters, if 0 message is treted like a string without format symbols
	 */
	inline virtual RtVoid handleErrorV(RtInt code, RtInt severity, RtString message, va_list argList=0)
	{
		handleErrorV(code, severity, 0, NULL, message, argList);
	}

	/** Handles an error, sets m_lastError and calls the current error handler
	 * @param code Error Code (RIE_...)
	 * @param severity Severity level of the error (RIE_INFO, ..., RIE_SEVERE)
	 * @param line Line number where error occured
	 * @param file file where error occured
	 * @param message Format string (like in printf()), not formatted if argList==NULL
	 * @param argList variable list of parameters, if 0 message is treted like a string without format symbols
	 */
	virtual RtVoid handleErrorV(RtInt code, RtInt severity, int line, const char *file, RtString message, va_list argList=0);
	//@}
}; // CErrorExceptionHandler

} // namespace RiCPP

#endif // _RICPP_BASERENDERER_RENDERERERROR_H
