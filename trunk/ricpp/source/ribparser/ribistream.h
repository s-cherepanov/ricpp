#ifndef _RICPP_RIBPARSER_RIBISTREAM_H
#define _RICPP_RIBPARSER_RIBISTREAM_H

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

/*! \file ribistram.cpp
 *  \brief Declaration of the input stream for RIB files, ASCII and binary (uses zlib)
 *  \author Andreas Pidde (andreas@pidde.de)
 */

#include <zlib.h>
#include <string>
#include <vector>

//! Class to read RIB streams
class TRibIStream {
	std::string m_filename;  //!< Current file name
	gzFile m_zipFile;        //!< Input zip file
	IndexType m_blocksize;   //!< Blocksize to read with one call
	unsigned char *m_block;  //!< Buffer for input data
	IndexType m_fp;          //!< Pointer to current character in m_block
	IndexType m_lastRet;     //!< Last return code
	bool m_hasPutBack;       //!< TRibIStream::putback() used to put back a character
	unsigned char m_putback; //!< Character put back by TRibIStream::putback()
	IndexType m_lineCount;   //!< Current line counter

public:
	//! Constructor sets filname and allocates buffer for m_block
	TRibIStream(const char *filename, int blocksize=1024);
	//! Destructor, closes file frees buffer
	~TRibIStream();
	//! Closes file and opens again
	/*! \return true if file could be opend, false otherwise
	 */
	bool reopen();
	//! Closes file
	void close();
	//! Lines are counted, this function returns the current line counter
	/*! \return current line counter
	 */
	inline IndexType lineCount() const { return m_lineCount; }
	//! Looks for end of file
	/*! \return true if end of file is reached, false otherwise
	 */
	bool eof() const;
	//! Puts back one character
	/*! You can only put back one character to the input stream, additional
	 *  calls overwrite the last character put back.
	 *  \param c Character to put back
	 */
	void putback(unsigned char c);
	//! Reads one character
	/*! \retval val Character read is stored in val
	 *  \return the stream itself
	 */
	TRibIStream& operator>>(unsigned char& val);
}; // TRibIStream

#endif // _RICPP_RIBPARSER_RIBISTREAM_H
