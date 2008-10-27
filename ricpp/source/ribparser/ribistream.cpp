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

/*! \file ribstram.cpp
 *  \brief Implementation of the input stream for RIB files, ASCII and binary (uses zlib)
 *  \author Andreas Pidde (andreas@pidde.de)
 */

#include "stdafx.h"
#include "ribistream.h"

#include <assert.h>


TRibIStream::TRibIStream(const char *filename, int blocksize) {
	m_filename = filename ? filename : "";
	m_zipFile = NULL; 
	m_blocksize = blocksize;
	m_block = new unsigned char[blocksize];
	m_lastRet = m_fp = 0;
	m_hasPutBack = false;
	m_lineCount = 0;
}


TRibIStream::~TRibIStream() {
	if ( m_block ) {
		delete[] m_block;
	}
	m_block = NULL;
}


void TRibIStream::close() {
	if ( m_zipFile )
		gzclose(m_zipFile);
	m_fp = m_lastRet = 0;
	m_hasPutBack = false;
	m_zipFile = NULL;
	m_lineCount = 0;
}

bool TRibIStream::reopen() {
	close();
	m_zipFile = gzopen(m_filename.c_str(), "rb"); 
	m_lineCount = 1;
	return m_zipFile != NULL;
}


bool TRibIStream::eof() const {
	if ( !m_zipFile )
		return false;

	return gzeof(m_zipFile) && m_fp >= m_lastRet ? true : false;
}


void TRibIStream::putback(unsigned char c) {
	m_hasPutBack = true;
	assert(m_lineCount > 0);
	if ( c == '\n' && m_lineCount > 0 )
		--m_lineCount;
	m_putback = c;
}


TRibIStream& TRibIStream::operator>>(unsigned char& val) {
	if ( m_hasPutBack ) {
		val = m_putback;
		m_hasPutBack = false;
		if ( val == '\n' )
			++m_lineCount;
		return *this;
	}

	if ( m_fp >= m_lastRet ) {
		if ( !m_zipFile ) {
			reopen();
		}
		if ( m_zipFile && !gzeof(m_zipFile) ) {
			m_lastRet = gzread(m_zipFile, m_block, m_blocksize);
		}
		m_fp = 0;
	}

	if ( m_fp < m_lastRet ) {
		val = m_block[m_fp++];
	} else {
		val = (unsigned char)0;
	}
	if ( val == '\n' )
		++m_lineCount;

	return *this;
}
