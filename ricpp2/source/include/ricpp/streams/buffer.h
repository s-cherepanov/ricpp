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

/** @file buffer.h
 *  @author Andreas Pidde (andreas@pidde.de)
 *  @brief Headerfile of the template for a simple memory buffer.
 */

#ifndef _RICPP_RICPP_RICPPERROR_H
#include "ricpp/ricpp/ricpperror.h"
#endif // _RICPP_RICPP_RICPPERROR_H

#include <memory.h>

namespace RiCPP {

	/** @brief Template for a simple memory buffer.
	 */
	template<typename T_char=unsigned char>
	class TemplBuffer {
	public:
		typedef size_t size_type;             ///< @brief Size type for the buffer.
		typedef T_char *iterator;             ///< @brief Iterator for the content in @c m_buffer.
		typedef const T_char *const_iterator; ///< @brief Constant iterator for the content in @c m_buffer.

	private:
		T_char *m_buffer;     ///< @brief Pointer to the buffer.
		size_type m_size;     ///< @brief Current size of the buffer.
		size_type m_reserved; ///< @brief Reserved size of the buffer.

	public:
		/** @brief Constructs a buffer of the given @a size.
		 *  @param size Size of the buffer.
		 */
		inline TemplBuffer(size_type size=0)
		{
			m_reserved = 0;
			m_size = 0;
			m_buffer = 0;
			resize(size);
		}

		/** @brief Copy constructor.
		 *  @param bb Buffer to copy.
		 */
		inline TemplBuffer(const TemplBuffer<T_char> &bb)
		{
			m_reserved = 0;
			m_size = 0;
			m_buffer = 0;
			*this = bb;
		}

		/** @brief Destructor, frees the memory.
		 */
		inline ~TemplBuffer()
		{
			if ( m_buffer )
				delete[] m_buffer;
			m_buffer = 0;
		}

		/** @brief Resize the buffer.
		 *  @param newsize New buffer size.
		 *  @exception ExceptRiCPPError Throws severe RIE_MEMORY if memory could not be allocated.
		 */
		void resize(size_type newsize);

		/** @brief Gets the current size.
		 *  @return Current buffer size.
		 */
		inline size_type size() const
		{
			return m_size;
		}

		/** @brief Assignment.
		 *
		 *  Assigns a buffer @a bb to this object.
		 *
		 *  @param bb Buffer to assign.
		 *  @return Reference of this object (*this).
		 *  @exception ExceptRiCPPError Throws severe RIE_MEMORY if memory could not be allocated.
		 */
		TemplBuffer<T_char> &operator=(const TemplBuffer<T_char> &bb);

		/** @brief Clears the buffer by setting its size to 0.
		 */
		inline void clear()
		{
			/*
			if ( !m_size )
				return;
			memset(m_buffer, 0, m_size*sizeof(T_char));
			*/
			m_size = 0;
		}

		/** @brief Gets an iterator initialized with the first element.
		 *  @return Iterator initialized with the first element.
		 */
		inline iterator begin()
		{
			return m_buffer;
		}

		/** @brief Gets an iterator pointing behind the last element.
		 * @return Iterator pointing behind the last element.
		 */
		inline iterator end()
		{
			return m_buffer+m_size;
		}

		/** @brief Gets a constant iterator initialized with the first element.
		 *  @return Constant iterator initialized with the first element.
		 */
		inline const_iterator begin() const
		{
			return m_buffer;
		}

		/** @brief Gets a constant iterator pointing behind the last element.
		 *  @return Constant iterator pointing behind the last element.
		 */
		inline const_iterator end() const
		{
			return m_buffer+m_size;
		}

		/** @brief Gets the reference to the element at positon @a pos.
		 *  @param pos position (< size()).
		 *  @return Reference of element at the position @a pos.
		 *  @exception ExceptRiCPPError Throws severe RIE_RANGE if @a pos >= size()
		 */
		T_char &operator[](size_type pos);

		/** @brief Gets the constant reference to the element at positon @a pos.
		 *  @param pos position (< size()).
		 *  @return Constant reference of element at the position @a pos.
		 *  @exception ExceptRiCPPError Throws severe RIE_RANGE if @a pos >= size()
		 */
		const T_char &operator[](size_type pos) const;
	}; // TemplBuffer


	template<typename T_char>
	inline
	void
	TemplBuffer<T_char>::resize(size_type newsize)
	{
		T_char *tempbuffer = 0;
		if ( newsize > m_reserved ) {
			try {
				tempbuffer = new T_char[newsize];
			} catch ( ... ) {
			}
			if ( !tempbuffer ) {
				throw ExceptRiCPPError(
					RIE_NOMEM, RIE_SEVERE,
					__LINE__, __FILE__,
					"Not enough memory for a byte buffer.");
			}
		}

		size_type size = newsize < m_size ? newsize : m_size;

		if ( size && tempbuffer )
			memcpy(tempbuffer, m_buffer, size*sizeof(T_char));

		if ( m_buffer && tempbuffer ) {
			// Delete the old buffer (new buffer is bigger)
			m_reserved = 0;
			m_size = 0;
			delete[] m_buffer;
			m_buffer = 0;
		}

		if ( tempbuffer ) {
			// Set the new buffer
			m_buffer = tempbuffer;
			m_reserved = newsize;
		}

		m_size = newsize;
	}

	template <typename T_char>
	inline
	TemplBuffer<T_char> &
	TemplBuffer<T_char>::operator=(const TemplBuffer<T_char> &bb)
	{
		if ( this == &bb )
			return *this;
		clear();
		try {
			resize(bb.size());
		} catch (ExceptRiCPPError &e) {
			throw e;
		}
		if ( m_size ) {
			memcpy(m_buffer, bb.begin(), m_size*sizeof(T_char));
		}
		return *this;
	}

	template <typename T_char>
	inline
	T_char &
	TemplBuffer<T_char>::operator[](size_type pos)
	{
		if ( pos < m_size )
			return m_buffer[pos];

		throw ExceptRiCPPError(
			RIE_RANGE, RIE_SEVERE,
			__LINE__, __FILE__,
			"Index out of range for a byte buffer.");
	}

	template <typename T_char>
	inline
	const T_char &
	TemplBuffer<T_char>::operator[](size_type pos) const
	{
		if ( pos < m_size )
			return m_buffer[pos];

		throw ExceptRiCPPError(
			RIE_RANGE, RIE_SEVERE,
			__LINE__, __FILE__,
			"Index out of range for a byte buffer.");
	}
} // namespace RiCPP

#endif // _RICPP_STREAMS_BUFFER_H
