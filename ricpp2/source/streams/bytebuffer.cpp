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

/** @file bytebuffer.cpp
 *  @author Andreas Pidde (andreas@pidde.de)
 *  @brief Implementation of the buffer for the stream classes.
 */

#include "ricpp/streams/bytebuffer.h"
#include "ricpp/ricpp/renderererror.h"

#include <memory.h>

using namespace RiCPP;


CByteBuffer::~CByteBuffer()
{
	if ( m_buffer )
		delete[] m_buffer;
}

void CByteBuffer::resize(sizetype newsize)
{
	unsigned char *tempbuffer = 0;
	if ( newsize > m_reserved ) {
		try {
			tempbuffer = new unsigned char[newsize];
		} catch ( ... ) {
		}
		if ( !tempbuffer ) {
			throw ERiCPPError(RIE_NOMEM, RIE_SEVERE, __LINE__, __FILE__, "Not enough memory for a byte buffer.");
		}
		m_reserved = newsize;
	}

	sizetype size = newsize < m_size ? newsize : m_size;

	if ( size && tempbuffer )
		memcpy(tempbuffer, m_buffer, size);

	if ( m_buffer && tempbuffer ) {
		delete[] m_buffer;
		m_buffer = tempbuffer;
	}

	m_size = newsize;
}

CByteBuffer &CByteBuffer::operator=(const CByteBuffer &bb)
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

void CByteBuffer::clear()
{
	if ( !m_size )
		return;
	memset(m_buffer, 0, m_size);
}
