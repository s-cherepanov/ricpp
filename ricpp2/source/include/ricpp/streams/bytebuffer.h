#ifndef _RICPP_STREAMS_BYTEBUFFER_H
#define _RICPP_STREAMS_BYTEBUFFER_H

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

/** @file bytebuffer.h
 *  @author Andreas Pidde (andreas@pidde.de)
 *  @brief Buffer for the stream classes.
 */

namespace RiCPP {

	class CByteBuffer {
	public:
		typedef size_t sizetype;
		typedef unsigned char * iterator;
		typedef const unsigned char * const_iterator;
	private:
		unsigned char *m_buffer;
		sizetype m_size;
		sizetype m_reserved;
	public:
		inline CByteBuffer(sizetype size=0)
		{
			m_reserved = 0;
			m_size = 0;
			m_buffer = 0;
			resize(size);
		}
		inline CByteBuffer(const CByteBuffer &bb)
		{
			m_reserved = 0;
			m_size = 0;
			m_buffer = 0;
			*this = bb;
		}
		~CByteBuffer();
		void resize(sizetype newsize);
		inline sizetype size() const { return m_size; }
		CByteBuffer &operator=(const CByteBuffer &bb);
		void clear();
		inline iterator begin() { return m_buffer; }
		inline iterator end() { return m_buffer ? m_buffer+m_size+1: m_buffer; }
		inline const_iterator begin() const { return m_buffer; }
		inline const_iterator end() const { return m_buffer ? m_buffer+m_size+1: m_buffer; }
	}; // CByteBuffer

} // namespace RiCPP

#endif // _RICPP_STREAMS_BYTEBUFFER_H
