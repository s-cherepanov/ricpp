#ifndef _RICPP_STREAMS_FILEREADER_H
#define _RICPP_STREAMS_FILEREADER_H

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

/** @file filereader.h
 *  @author Andreas Pidde (andreas@pidde.de)
 *  @brief Read files
 */

#ifndef _RICPP_STREAMS_READER_H
#include "ricpp/streams/reader.h"
#endif // _RICPP_STREAMS_READER_H

#include <stdio.h>
namespace RiCPP {

	/** @brief Read a file using the file: scheme
	 */
	class CFileReader : public CReader {
		std::string m_uri;
		FILE *m_file;
	public:
		static const char *myName();
		static const char *myType();
		static unsigned long myMajorVersion();
		static unsigned long myMinorVersion();
		static unsigned long myRevision();

		CFileReader();
		virtual ~CFileReader();

		inline virtual const char *name() const { return myName(); }
		inline virtual const char *type() const { return myType(); }
		inline virtual unsigned long majorVersion() const {return myMajorVersion(); }
		inline virtual unsigned long minorVersion() const {return myMinorVersion(); }
		inline virtual unsigned long revision() const { return myRevision(); }

		inline virtual void startup() {}
		inline virtual void shutdown() {}

		virtual bool open(const char *an_uri);
		virtual TBuffer<>::size_type fillBuffer(TBuffer<> &buffer);
		virtual bool close();
	}; // CFileReader
} // namespace RiCPP

#endif // _RICPP_STREAMS_FILEREADER_H
