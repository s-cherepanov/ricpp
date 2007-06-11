#ifndef _RICPP_STREAMS_BUFFER_H
#define _RICPP_STREAMS_BUFFER_H

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

#ifndef _RICPP_RICPP_RICPPERROR_H
#include "ricpp/ricpp/ricpperror.h"
#endif _RICPP_RICPP_RICPPERROR_H

#include <memory.h>

namespace RiCPP {

	template<typename T=unsigned char>
	class TBuffer {
	public:
		typedef size_t size_type;
		typedef T contenttype;
		typedef contenttype * iterator;
		typedef const contenttype * const_iterator;
	private:
		contenttype *m_buffer;
		size_type m_size;
		size_type m_reserved;
	public:
		inline TBuffer(size_type size=0)
		{
			m_reserved = 0;
			m_size = 0;
			m_buffer = 0;
			resize(size);
		}
		inline TBuffer(const TBuffer<T> &bb)
		{
			m_reserved = 0;
			m_size = 0;
			m_buffer = 0;
			*this = bb;
		}
		inline ~TBuffer()
		{
			if ( m_buffer )
				delete[] m_buffer;
		}
		void resize(size_type newsize);
		inline size_type size() const { return m_size; }
		TBuffer<T> &operator=(const TBuffer<T> &bb);
		inline void clear()
		{
			if ( !m_size )
				return;
			memset(m_buffer, 0, m_size);
		}
		inline iterator begin() { return m_buffer; }
		inline iterator end() { return m_buffer ? m_buffer+m_size+1: m_buffer; }
		inline const_iterator begin() const { return m_buffer; }
		inline const_iterator end() const { return m_buffer ? m_buffer+m_size+1: m_buffer; }
	}; // TBuffer


	template<typename T>
	inline
	void TBuffer<T>::
	resize(size_type newsize)
	{
		contenttype *tempbuffer = 0;
		if ( newsize > m_reserved ) {
			try {
				tempbuffer = new contenttype[newsize];
			} catch ( ... ) {
			}
			if ( !tempbuffer ) {
				throw ERiCPPError(RIE_NOMEM, RIE_SEVERE, __LINE__, __FILE__, "Not enough memory for a byte buffer.");
			}
			m_reserved = newsize;
		}

		size_type size = newsize < m_size ? newsize : m_size;

		if ( size && tempbuffer )
			memcpy(tempbuffer, m_buffer, size);

		if ( m_buffer && tempbuffer ) {
			delete[] m_buffer;
			m_buffer = tempbuffer;
		}

		m_size = newsize;
	}

	template <typename T>
	inline
	TBuffer<T> &TBuffer<T>::
	operator=(const TBuffer<T> &bb)
	{
		if ( this == &bb )
			return *this;
		resize(0);
		resize(bb.size());
		if ( m_size ) {
			memcpy(m_buffer, bb.begin(), m_size);
		}
		return *this;
	}

} // namespace RiCPP

#endif // _RICPP_STREAMS_BUFFER_H
