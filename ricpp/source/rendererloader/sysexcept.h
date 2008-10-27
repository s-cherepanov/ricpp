#ifndef _RICPP_RENDERLOADER_SYSEXEPT_H
#define _RICPP_RENDERLOADER_SYSEXEPT_H

// RICPP - RenderMan Interface CPP Language Binding
//         RenderMan is (R) by Pixar
//         Windows is (R) by Microsoft
//
// Copyright © 2001 - 2002, Andreas Pidde
//
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

/*! \file sysexcept.h
 *  \brief System error exception, contains some systemdepended code, all is inline
 *  \author Andreas Pidde (andreas@pidde.de)
 */

#include <stdio.h>   //!< used for sprintf
#include <iostream>  //!< used to print an error message

//! Systemdependent, displays an error message
/*! Using Windows, it shows a dialog box, for Linux a
 *  message to standard output (std::cout) is printed.
 *
 *  \param caption the headline
 *  \param msg the message
 */
inline void DISPLAY_MESSAGE(const char *caption, const char *msg) {

#	ifdef WIN32
#		ifdef UNICODE
			MessageBoxA(NULL, msg, caption, MB_OK|MB_ICONEXCLAMATION);
#		else
			MessageBox(NULL, msg, caption, MB_OK|MB_ICONEXCLAMATION);
#		endif
#	endif // WIN32

#	ifdef LINUX
		std::cout << caption << std::endl << msg << std::endl;
#	endif // LINUX
}

//! Systemdependent, prints of error message
/*! Prints out an error message to standard output (std::cout).
 *  \param caption the headline
 *  \param msg the message
 */
inline void PRINT_MESSAGE(const char *caption, const char *msg) {
	std::cout << caption << std::endl << msg << std::endl;
}

//! Systemdependent deallocation of a message buffer
/*! Currently only the buffer for the Windows implementation is freed,
 *  because there is none in Linux.
 *  \param lpMsgBuf The message buffer
 */
inline void FREE_MESSAGE(const char * &lpMsgBuf) {
#	ifdef WIN32
		LocalFree((void *)lpMsgBuf);
#	endif // WIN32
	lpMsgBuf = NULL;
}

//! Systemdependent allocation of a message block and error number formatting
/*! Currently only used for the Windows implementation to generate
 *  an error meassage.
 *  \param  dwRetCode The errorcode
 *  \retval lpMsgBuf The message buffer (will be allocated)
 */
inline void FORMAT_MESSAGE(long dwRetCode, const char **lpMsgBuf) {
#	ifdef WIN32
		FormatMessage( 
			FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
			NULL,
			dwRetCode,
			MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
			(LPTSTR) lpMsgBuf,
			0,
			NULL 
		);
#	endif // WIN32
#	ifdef LINUX
		// ToDo
#	endif // LINUX
}

//! Class of the exception that is thrown, if a system error occures
/*! An instance of this object is thrown if system exceptions occurs.
 *  Used by many interface calls.
 */
class TSystemException {
public:
	//! Error string for an unknown RenderMan interface type name
	//! \return The string "Illegal typename"
	inline static const char *m_errIllegalTypename() {
		return "Illegal typename";
	}
	//! Error Index overflow
	//! \return The string "Index overflow"
	inline static const char *m_errIndexOverflow() {
		return "Index overflow";
	}
private:
        long        m_dwRetCode;    //!< Error code.
        const char *m_lpMsgBuf;     //!< Message buffer, memory is managed by object.
		bool        m_bIsLocal;     //!< Message buffer memory allocated.
		long        m_dwLineNo;     //!< Linenumber where error occured.
		const char *m_lpFilename;   //!< Filename where error occured.
public:
		//! Standard constructor (uses GetLastError() to get the last error code)
		//! \param lpFileName The filename __FILE__ where the exception is thrown
		//! \param dwLineNo The line number __LINE__ where the exception is thrown
		inline TSystemException(const char *lpFileName, long dwLineNo) {
			m_lpFilename = lpFileName;
			m_dwLineNo = dwLineNo;
			m_bIsLocal = false;
			m_lpMsgBuf = NULL;
			setErrorCode(GetLastError());
		}

		//! Constructor to set an error string.
		//! \param lpReason The reason why an exception occured
		//! \param lpFileName The filename __FILE__ where the exception is thrown
		//! \param dwLineNo The line number __LINE__ where the exception is thrown
        inline TSystemException(
			const char *lpReason,
			const char *lpFileName,
			long dwLineNo)
		{
			m_lpFilename = lpFileName;
			m_dwLineNo = dwLineNo;
			m_bIsLocal = false;
			m_dwRetCode = -1L;
			m_lpMsgBuf = lpReason;
		}

		//! Constructor to set error code
		//! \param dwRetCode The error code
		//! \param lpFileName The filename __FILE__ where the exception is thrown
		//! \param dwLineNo The line number __LINE__ where the exception is thrown
		inline TSystemException(
			long dwRetCode,
			const char *lpFileName,
			long dwLineNo)
		{
			m_lpFilename = lpFileName;
			m_dwLineNo = dwLineNo;
			m_bIsLocal = false;
			m_lpMsgBuf = NULL;
			setErrorCode(dwRetCode);
		}

		//! Destructor frees message buffer
        inline ~TSystemException() {
			if ( m_lpMsgBuf && m_bIsLocal ) {
				FREE_MESSAGE(m_lpMsgBuf);
			}
		}

		//! Formats error message for error code using FORMAT_MESSAGE()
		//! \param dwRetCode The error code
        inline void setErrorCode(long dwRetCode) {
			m_dwRetCode = dwRetCode;
			m_bIsLocal = true;
			FORMAT_MESSAGE(dwRetCode, &m_lpMsgBuf);
		}

		//! Formats an error message, returns the parameters strBuffer.c_str()
		//! \param strBuffer The error message is stored in strBuffer
		//! \return The C-string of strBuffer (strBuffer.c_str())
		inline const char *formatMessage(std::string &strBuffer) const {
			char buf[256];
			if ( m_lpFilename ) {
				sprintf(buf, "Error: %ld - Line: %ld - File: '", m_dwRetCode, m_dwLineNo);
				strBuffer = buf;
				strBuffer += m_lpFilename;
				strBuffer += "'";
			}
			else {
				sprintf(buf, "Error: %ld", m_dwRetCode);
				strBuffer = buf;
			}
			return strBuffer.c_str();
		}

		//! Shows the formatted error message using DISPLAY_MESSAGE()
        inline void displayMessage(void) const {
			std::string caption;
			DISPLAY_MESSAGE(m_lpMsgBuf, formatMessage(caption));
		}

		//! Prints formatted error message using PRINT_MESSAGE()
        inline void printMessage(void) const {
			std::string caption;
			PRINT_MESSAGE(m_lpMsgBuf, formatMessage(caption));
		}

		//! \return The pointer to the error message.
		inline const char *what() const { return m_lpMsgBuf; }

		//! \return The pointer to filename where the error was thrown
		inline const char *fileName() const { return m_lpFilename; }

		//! \return The line number where the error was thrown
		inline const long lineNo() const { return m_dwLineNo; }

		//! \return The error code that causes the exception or -1L if there is none
		inline const long errorCode() const { return m_dwRetCode; }
}; // class TSystemException

#endif // _RICPP_RENDERLOADER_SYSEXEPT_H
