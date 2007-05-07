#ifndef _RICPP_RICPP_RENDERERERROR_H
#define _RICPP_RICPP_RENDERERERROR_H

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
#include "ricpp/ricpp.h"
#endif // _RICPP_RICPP_RICPP_H

#include <string>

namespace RiCPP {

/** The renderer error is used internally to throw exceptions
 */
class ERendererError {
	RtInt m_severity;		///< Severity level RIE_INFO, RIE_WARNING, RIE_ERROR, RIE_SEVERE
	RtInt m_code;			///< Which of error occured, 'RIE_...'
	std::string m_message;	///< Formatted string describing occured error

	static const char *sErrorMessages[];	///< Static list of 'RIE_...' error codes as text
	static const char *sErrorSeverity[];	///< Static list of 'RIE_...' error codes as text

public:
	/** Constructor, sets error codes and additional error message
	 *  @param code Which error ('RIE_...')
	 *  @param severity Severity level RIE_INFO, RIE_WARNING, RIE_ERROR, RIE_SEVERE
	 *  @param message Additional describing error string
	 */
	inline ERendererError(RtInt aCode, RtInt aSeverity = RIE_ERROR, const char *aMessage = NULL)
		: m_severity(aSeverity), m_code(aCode), m_message(aMessage) { }

	/** Formats the error/severity numbers to a string.
	 *  The format is "Error [<number>:<description>] Severity [<number>:<description>]",
	 *  @retVal strCode The error is formatted to this string
	 *  @param aCode The error code (RIE_...)
	 *  @param aSeverity The Severity (RIE_Info, e.t.c.)
	 *  @return strCode.c_str()
	 */
	static const char *formatError(std::string &strCode, RtInt aCode, RtInt aSeverity);

	/** Gets the current error string
	 *  @return Pointer to the error message stored in ERendererError::m_message
	 */
	inline const char *what() const { return m_message.c_str(); }

	/** Gets the current error code
	 *  @return error code stored in RendererError::m_code
	 */
	inline RtInt code() const { return m_code; }

	/** Returns the descriptive string for an error code
	 *  @param aCode The error code
	 *  @return error string for the error code aCode
	 */
	inline static const char *errorMessage(RtInt aCode) {
		if ( aCode < 0 || aCode > 61 )
			return "unknown";

		return sErrorMessages[(int)aCode];
	}

	/** Get the current error severity
	 *  @return error severity stored in ERendererError::m_severity
	 */
	inline RtInt severity() const { return m_severity; }

	/** Returns the descriptive string for a severity level
	 *  @param aSeverity severity level
	 *  @return The descriptive string for an error severity level aSeverity
	 */
	inline static const char *errorSeverity(RtInt aSeverity) {
		if ( aSeverity < 0 || aSeverity > 3 )
			return "unknown";

		return sErrorSeverity[(int)aSeverity];
	}
}; // TRendererError
} // namespace RiCPP

#endif // _RICPP_RICPP_RENDERERERROR_H
