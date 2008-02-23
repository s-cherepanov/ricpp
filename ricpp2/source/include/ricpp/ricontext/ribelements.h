#ifndef _RICPP_RICONTEXT_RIBELEMENTS_H
#define _RICPP_RICONTEXT_RIBELEMENTS_H

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

/** @file ribelements.h
 *  @author Andreas Pidde (andreas@pidde.de)
 *  @brief Contains class declaration for RIB writing
 */

#ifndef RICPP_RICPP_REQUESTS_H
#include "ricpp/ricpp/requests.h"
#endif // RICPP_RICPP_REQUESTS_H

#ifndef RICPP_RICPP_TYPES_H
#include "ricpp/ricpp/types.h"
#endif // RICPP_RICPP_TYPES_H

#include <streambuf>
#include <map>


namespace RiCPP {

class IRequestNotification
{
public:
	inline virtual ~IRequestNotification() {}
	virtual void requestWritten(EnumRequests aRequest) = 0;
};

/** @brief Helper class to write RIB elements
 *
 *  RIB streams can contain ascii and binary data. Instances
 *  of the class CRibElementsWriter can be used to put elementary
 *  RIB data, like request identifiers, numbers (floats and integers),
 *  characters and strings to a stream.
 *
 *  The stream uses an initialized TemplFrontStreambuf<char> object as
 *  buffer. The buffer has to be opened before passed to the constructor,
 *  it has also to be closed by the caller.
 *
 *  Binary RIB can encode double precision and fixed-point numbers. However,
 *  double precision will not be used, because RiCPP (C-binding also) only uses
 *  single precision floating point numbers. Fixed-point numbers are
 *  neither used, nor implemented by this object.
 *
 *  @see TemplFrontStreambuf
 */
class CRibElementsWriter {
private:
	std::ostream m_ostream; ///< RIB output stream.
	bool m_ascii; ///< true, indicates that ascii-data should be written. False, binary data.
	unsigned char m_reqEncoding[N_REQUESTS]; ///< Elements indicate (value!=0), that a request is defined.
	std::map<std::string, RtInt> m_stringTokens; ///< Elements indicate, that a string token is defined
	unsigned long m_countStrings; ///< Counter for the string numbers (used 16 bit)
	bool m_firstRequestWritten; ///< Gets true after first real request is written to suppress header
	IRequestNotification &m_notify;  ///< Notify request
	unsigned long m_linecnt; ///< line counter

	/** @brief Puts a binary encoded number (string length, RtInt) to the stream.
	 */
	void putLength(unsigned char code, unsigned long length);

	/** @brief Puts a binary encoded string token to the stream.
	 */
	void putStringTokenNum(unsigned char code, unsigned long tok);

	/** @brief Puts a binary encoded IEEE float to the stream.
	 */
	void putBinValue(float aFloat);

	/** @brief Puts a binary encoded double precision IEEE float to the stream.
	 */
	void putBinValue(double aFloat);

public:

	/** @brief Constructor, initializes the objects with the stram buffer to write to.
	 */
	CRibElementsWriter(std::basic_streambuf<char, std::char_traits<char> > *ribout, IRequestNotification &notify);
	// CRibElementsWriter(TemplFrontStreambuf<char> *ribout, IRequestNotification &notify);

	/** @brief Destructor, doesn't close the stream (because it is not opened by a CRibElementsWriter).
	 */
	inline ~CRibElementsWriter() {}

	/** @brief test if there is already a request written.
	 *
	 *  This is used to suppress structural comments and version info.
	 */
	inline bool firstRequestWritten() const
	{
		return m_firstRequestWritten;
	}
	
	/** @brief Gets the current line counter
	 *  @return the current line counter
	 */
	inline unsigned long linecnt() const
	{
		return m_linecnt;
	}
	

	/** @brief Gets the ascii mode flag.
	 */
	inline bool ascii() const
	{
		return m_ascii;
	}

	/** @brief Sets the ascii mode flag.
	 */
	inline void ascii(bool isAscii)
	{
		m_ascii = isAscii;
	}

	/** @brief Puts out a new line character (in ascii mode only).
	 */
	void putNewLine();

	/** @brief Puts out a blank as seperator (in ascii mode only).
	 */
	void putBlank();

	/** @brief Puts out a single character.
	 *
	 *  Since a 7-bit ASCII stream is used, c has to be positive.
	 *  Negative characters (highest bit set) are used to encode binary data.
	 *
	 *  @param c Character to put out (c >= 0).
	 */
	void putChar(char c);
	
	/** @brief Puts out a characters until character '\\0' is reached.
	 *  @param cs Character string to put out.
	 */
	void putChars(const char *cs);

	/** @brief Puts out a comment line, started with '#' and terminated by endl.
	 *  @param type Type of the comment (RICOMMENT, RI_STRUCTURE, RI_VERBATIM)
	 *  @param cs Character string to put out (without the first '#').
	 */
	void putComment(RtToken type, const char *cs);

	/** @brief Puts out a RIB request.
	 *  @param aRequest Identifier number of the request.
	 */
	void putRequest(EnumRequests aRequest);
	
	/** @brief Puts out a vector of single precision floats.
	 *  @param floats Vector of floats to put out.
	 */
	void putArray(const std::vector<float> &floats);
	
	/** @brief Puts out an array of single precision floats.
	 *  @param length Length of @a floats.
	 *  @param floats Array of floats to put out.
	 */
	void putArray(unsigned long length, const float *floats);

	/** @brief Puts out a vector of double precision floats.
	 *  @param floats Vector of double precision floats to put out.
	 */
	void putArray(const std::vector<double> &floats);
	
	/** @brief Puts out an array of double precision floats.
	 *  @param length Length of @a floats.
	 *  @param floats Array of double precision floats to put out.
	 */
	void putArray(unsigned long length, const double *floats);

	/** @brief Puts out a vector of integers.
	 *  @param integers Vector of integers to put out.
	 */
	void putArray(const std::vector<RtInt> &integers);

	/** @brief Puts out an array of integers.
	 *  @param length Length of @a integers.
	 *  @param integers Array of integers to put out.
	 */
	void putArray(unsigned long length, const RtInt *integers);

	/** @brief Puts out a vector of strings.
	 *  @param strings Vector of strings to put out.
	 */
	void putArray(const std::vector<RtString> &strings);

	/** @brief Puts out an array of strings.
	 *  @param length Length of @a strings.
	 *  @param strings Array of strings to put out.
	 */
	void putArray(unsigned long length, const RtString *strings);

	/** @brief Puts out a vector of token strings.
	 *  @param strings Vector of token strings to put out.
	 */
	void putTokenArray(const std::vector<RtString> &strings);

	/** @brief Puts out an array of token strings.
	 *  @param length Length of @a strings.
	 *  @param strings Array of token strings to put out.
	 */
	void putTokenArray(unsigned long length, const RtString *strings);

	/** @brief Puts out one single precision float.
	 *  @param aFloat Float value to put out.
	 */
	void putValue(float aFloat);

	/** @brief Puts out one double precision float.
	 *  @param aFloat Double precision float value to put out.
	 */
	void putValue(double aFloat);

	/** @brief Puts out one integer.
	 *  @param anInteger Integer value to put out.
	 */
	void putValue(int anInteger);
	
	/** @brief Puts out one integer.
	 *  @param anInteger Integer value to put out.
	 */
	void putValue(unsigned long anInteger);
	
	/** @brief Puts out a string.
	 *  @param aString Character string value to put out.
	 */
	void putString(RtString aString);

	/** @brief Tokenize a string if binary mode and put out the string (token) in either mode.
	 *  @param aString Character string value to put out (in binary mode as token).
	 */
	void putStringToken(RtString aString);
}; // CRibElementsWriter

}

#endif // _RICPP_RICONTEXT_RIBELEMENTS_H
