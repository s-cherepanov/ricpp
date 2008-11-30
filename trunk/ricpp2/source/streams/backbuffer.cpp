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

/** @file backbuffer.cpp
 *  @author Andreas Pidde (andreas@pidde.de)
 *  @brief Implementation for zlib-enabled streams and base classes for
 *         plugable streambuffer facets.
 */

#include "ricpp/streams/backbuffer.h"
#include <cassert>

using namespace RiCPP;

// ----------------------------------------------------------------------------

bool CFileBackBuffer::open(const CUri &absUri, TypeOpenMode mode)
{
	close();
	
	CBackBufferRoot::open(absUri, mode);
	
	m_mode = mode;
	std::string filename;
	absUri.decodeFilepath(filename);
	
	// std::cerr << "# FILENAME: '" << filename << "'" << std::endl;
	
	m_filebuf.open(filename.c_str(), mode);
	return m_filebuf.is_open();
}

std::streamsize CFileBackBuffer::sgetn(char *b, std::streamsize size) 
{
	if ( !isOpen() || !b || !size ) {
		// File not open or no buffer
		return 0;
	}
	if ( !(m_mode & std::ios_base::in) ) {
		// File not opened for reading
		return 0;
	}
	
	// Read from buffer
#ifdef _MSC_VER
	return m_filebuf._Sgetn_s(b, size, size);
#else
	return m_filebuf.sgetn(b, size);
#endif
}

std::streamsize CFileBackBuffer::sputn(const char *b, std::streamsize size) 
{
	if ( !isOpen() || !b || !size ) {
		// File not open or no buffer
		return 0;
	}
	if ( !(m_mode & std::ios_base::out) ) {
		// File not opened for writing
		return 0;
	}
	return m_filebuf.sputn(b, size);
}

// ----------------------------------------------------------------------------

const char *CBackBufferFactory::myType() { return "backbufferfactory"; }
const char *CBackBufferFactory::myName() { return "backbufferfactory"; }
unsigned long CBackBufferFactory::myMajorVersion() { return 1; }
unsigned long CBackBufferFactory::myMinorVersion() { return 1; }
unsigned long CBackBufferFactory::myRevision() { return 1; }

void CBackBufferFactory::addScheme(const char *scheme)
{
	std::string str_scheme(noNullStr(scheme));
	if ( str_scheme.empty() )
		return;
	std::for_each(str_scheme.begin(),
				  str_scheme.end(),
				  std::ptr_fun(toupper));
	m_schemes.push(str_scheme.c_str());
}

bool CBackBufferFactory::acceptsScheme(const char *scheme) const
{
	std::string str_scheme(noNullStr(scheme));
	std::for_each(str_scheme.begin(),
				  str_scheme.end(),
				  std::ptr_fun(asciiToUpper));
	return m_schemes.isMember(str_scheme.c_str());
}

bool CBackBufferFactory::close(CBackBufferRoot *bbr)
{
	bool rval = false;
	if ( bbr ) {
		bbr->close();
		return unRegisterObj(bbr);
	}
	return rval;
}

// ----------------------------------------------------------------------------

const char *CFileBackBufferFactory::myType()
{
	return CBackBufferFactory::myType();
}

const char *CFileBackBufferFactory::myName()
{
	return "file_backbuffer";
}

unsigned long CFileBackBufferFactory::myMajorVersion()
{
	return CBackBufferFactory::myMajorVersion();
}

unsigned long CFileBackBufferFactory::myMinorVersion() { return 1; }
unsigned long CFileBackBufferFactory::myRevision() { return 1; }

CBackBufferRoot *
CFileBackBufferFactory::open(const CUri &absUri, TypeOpenMode mode)
{
	CFileBackBuffer *buf = new CFileBackBuffer;
	if ( buf ) {
		if ( !buf->open(absUri, mode) ) {
			delete buf;
			return 0;
		}
	}
	registerObj(buf);
	return buf;
}

// ----------------------------------------------------------------------------

void CBackBufferProtocolHandlers::init(const char *direct)
{
	m_direct = direct ? direct : "";
	m_hasDirect = direct != 0;
	
	if ( !m_backBufferPluginHandler.isRegistered("filebuffer") ) {
		// The FILE handler is registered directly and is not loaded
		m_backBufferPluginHandler.registerFactory("file.buffer",
												  reinterpret_cast<
												  TemplPluginFactory
												  <CBackBufferFactory> *
												  >
												  (&m_fileBuffer));
		
		// Load more handlers (there are none at the moment)
		m_backBufferPluginHandler.registerFromDirectory(direct, ".buffer");
	}
}

CBackBufferFactory *
CBackBufferProtocolHandlers::getBufferFactory(const char *scheme)
{
	TemplPluginHandler<CBackBufferFactory>::const_iterator i;
	for ( i = m_backBufferPluginHandler.begin();
		 i != m_backBufferPluginHandler.end();
		 i++ )
	{
		CBackBufferFactory *f = (*i).second->lastPlugin();
		if ( f && f->acceptsScheme(scheme) )
			return f;
	}
	return 0;
}

// ----------------------------------------------------------------------------

void  CFrontStreambuf::init()
{
	m_backBuffer = 0;
	m_coupledBuffer = 0;
	m_factory = 0;
	
	m_mode = static_cast<TypeOpenMode>(0);
	
	m_buffersize = 8192; // std 8192, has to be a minimum size for zip header
	m_putbackSize = 128; // std 128
	
	assert ( m_buffersize >= m_putbackSize );
	
	m_frontInBuffer.clear();
	m_frontInBuffer.resize(m_buffersize);
	m_in = 0;
	m_crcIn = 0;
	m_transparentIn = true;
	m_strategyIn = Z_DEFAULT_STRATEGY;
	m_methodIn = Z_DEFLATED;
	setg(m_frontInBuffer.begin()+m_putbackSize,
		 m_frontInBuffer.begin()+m_putbackSize,
		 m_frontInBuffer.begin()+m_putbackSize);
	
	
	m_frontOutBuffer.clear();
	m_frontOutBuffer.resize(m_buffersize);
	m_out = 0;
	m_crcOut = 0;
	m_compressLevelOut = Z_DEFAULT_COMPRESSION;
	m_strategyOut = Z_DEFAULT_STRATEGY;
	m_methodOut = Z_DEFLATED;
	setp(m_frontOutBuffer.begin(), m_frontOutBuffer.begin()+(m_frontOutBuffer.size()-1));
	
	setBaseCwd();
}

void CFrontStreambuf::setBaseCwd()
{
	CFilepath p;
	std::string path(p.filepath());
	path += CFilepathConverter::internalPathSeparator();
	m_baseUri.encodeFilepath(path.c_str(), "file");
}

bool CFrontStreambuf::base(const CUri &base)
{
	if ( !base.isValid() ) {
		return false;
	}
	m_baseUri = base;
	if ( m_baseUri.toString().empty() ) {
		// No base URI, so use the current directory in file system
		setBaseCwd();
	}
	assert(m_baseUri.isValid());
	return true;
}

bool CFrontStreambuf::check_header()
{
	int method; // method byte
	int flags;  // flags byte
	uInt len;
	char c;
	
	// Stream buffer is greater than 2 - maybe zipped
	// if it is less than 2, the file is smaller as 2 Bytes, not zipped
	if ( m_strmIn.avail_in < 2 ) {
		fill_in_buffer();
		if ( m_strmIn.avail_in < 2 ) {
			m_transparentIn = true;
			return true;
		}
	}
	
	// Peek ahead to check the gzip magic header
	if (m_strmIn.next_in[0] != gz_magic_0 ||
		m_strmIn.next_in[1] != gz_magic_1)
	{
		m_transparentIn = true;
		return true;
	}
	
	// Skip the header
	get_byte();
	get_byte();
	
	m_transparentIn = false;
	
	// Check the rest of the gzip header
	method = get_byte();
	flags = get_byte();
	
	if ( method != Z_DEFLATED || flags == EOF || (flags & RESERVED) != 0 ) {
		return false;
	}
	
	// Discard time, xflags and OS code:
	for ( len = 0; len < 6; len++ ) {
		get_byte();
	}
	
	if ( (flags & EXTRA_FIELD) != 0 ) { // skip the extra field
		len  =  (uInt)get_byte();
		len += ((uInt)get_byte())<<8;
		// len is garbage if EOF but the loop below will quit anyway
		while ( len-- != 0 && get_byte() != EOF ) ;
	}
	if ((flags & ORIG_NAME) != 0) { // skip the extra field
		while ( (c = get_byte()) != 0 && c != EOF ) ;
	}
	if ((flags & COMMENT) != 0) {   // skip the .gz file comment
		while ( (c = get_byte()) != 0 && c != EOF ) ;
	}
	if ((flags & HEAD_CRC) != 0) {  // skip the header crc 16Bit
		for (len = 0; len < 2; len++) {
			get_byte();
		}
	}
	
	return !m_inIsEOF;
}

unsigned int CFrontStreambuf::fill_in_buffer()
{
	const size_t footerSize = 8;
	
	if ( m_strmIn.avail_in != 0 ) {
		return m_strmIn.avail_in;
	}
	
	if ( m_inIsEOF )
		return 0;
	
	// ToDo if not binary and Win32: \r\n -> \n (?)
	
	bool startstream = false;
	if ( m_transferInBuffer.size() == 0 ) {
		m_transferInBuffer.resize(m_buffersize + footerSize);
		m_strmIn.next_in = m_transferInBuffer.begin();
		startstream = true;
	} else if ( m_strmIn.next_in <
			   m_transferInBuffer.begin() +
			   (m_transferInBuffer.size() - footerSize) )
	{
		m_inIsEOF = true;
		return 0;
	} else {
		m_strmIn.next_in = m_transferInBuffer.begin();
	}
	
	if ( !startstream ) {
		memcpy(m_transferInBuffer.begin(),
			   m_transferInBuffer.begin() +
			   m_transferInBuffer.size() - footerSize,
			   footerSize);
	}
	
	if ( m_backBuffer ) {
		
		if ( startstream ) {
			m_strmIn.avail_in =
			(uInt)m_backBuffer->sgetn(reinterpret_cast<char *>
									  (m_transferInBuffer.begin()),
									  static_cast<std::streamsize>
									  (m_transferInBuffer.size()));
		} else {
			m_strmIn.avail_in =
			(uInt)m_backBuffer->sgetn(reinterpret_cast<char *>
									  (m_transferInBuffer.begin() +
									   footerSize),
									  static_cast<std::streamsize>
									  (m_transferInBuffer.size() - footerSize));
		}
		
	} else if ( m_coupledBuffer ) {
		
		if ( startstream ) {
#ifdef _MSC_VER
			m_strmIn.avail_in =
			(uInt)m_coupledBuffer->_Sgetn_s(reinterpret_cast<char *>
											(m_transferInBuffer.begin()),
											static_cast<std::streamsize>
											(m_transferInBuffer.size()),
											static_cast<std::streamsize>
											(m_transferInBuffer.size()));
#else
			m_strmIn.avail_in =
			m_coupledBuffer->sgetn(reinterpret_cast<char *>
								   (m_transferInBuffer.begin()),
								   static_cast<std::streamsize>
								   (m_transferInBuffer.size()));
#endif
		} else {
#ifdef _MSC_VER
			m_strmIn.avail_in =
			(uInt)m_coupledBuffer->_Sgetn_s(reinterpret_cast<char *>
											(m_transferInBuffer.begin() +
											 footerSize),
											static_cast<std::streamsize>
											(m_transferInBuffer.size() -
											 footerSize),
											static_cast<std::streamsize>
											(m_transferInBuffer.size() -
											 footerSize));
#else
			m_strmIn.avail_in =
			m_coupledBuffer->sgetn(reinterpret_cast<char *>
								   (m_transferInBuffer.begin() + footerSize),
								   static_cast<std::streamsize>
								   (m_transferInBuffer.size() - footerSize));
#endif
		}
		
	}
	
	if ( startstream ) {
		if ( m_strmIn.avail_in < m_transferInBuffer.size() ) {
			if (!m_transparentIn &&
				m_strmIn.next_in[0] == gz_magic_0 &&
				m_strmIn.next_in[1] == gz_magic_1)
			{
				if ( m_strmIn.avail_in >= 2+footerSize ) {
					// At least the 8 Byte at the and of the zipfile
					// (4 Byte CRC, 4 Byte length)
					m_strmIn.avail_in -= footerSize;
				}
				// If the filesize is smaller than header, the check will fail.
			}
		} else {
			m_strmIn.avail_in -= footerSize;
		}
	} else {
		if ( m_transparentIn ) {
			if ( m_strmIn.avail_in < m_transferInBuffer.size()-footerSize ) {
				// no zip -> no footer has to be truncated
				m_strmIn.avail_in += footerSize;
			}
		}
	}
	
	return m_strmIn.avail_in;
}

CFrontStreambuf::int_type CFrontStreambuf::flushBuffer(bool finish)
{
	if ( !(m_mode & std::ios_base::out) ) {
		return std::char_traits<char>::eof();
	}
	
	int num = static_cast<int>(TypeParent::pptr()-TypeParent::pbase());
	if ( num <= 0 ) {
		// TypeParent::pbump(0);
		return std::char_traits<char>::eof();
	}
	
	if ( !m_backBuffer && !m_coupledBuffer ) {
		// TypeParent::pbump(-num);
		return std::char_traits<char>::eof();
	}
	
	if ( m_compressLevelOut == Z_NO_COMPRESSION ) {
		if ( m_backBuffer ) {
			if ( m_backBuffer->sputn(m_frontOutBuffer.begin(), num) != num ) {
				return std::char_traits<char>::eof();
			}
		} else if ( m_coupledBuffer ) {
			if ( m_coupledBuffer->sputn(m_frontOutBuffer.begin(), num) != num ) {
				return std::char_traits<char>::eof();
			}
		}
	} else {
		// Copy to/from transferbuff with zlib, transfer the back buffer or
		// coupled buffer
		int ret;
		
		m_strmOut.avail_in = num;
		m_strmOut.next_in = (Bytef *)(m_frontOutBuffer.begin());
		
		if ( m_transferOutBuffer.size() == 0 ) {
			m_transferOutBuffer.resize(m_buffersize);
			
			// Write compress header
			char header[10] = {
				(char)gz_magic_0, (char)gz_magic_1,
				m_methodOut,
				(m_mode  & std::ios_base::binary) ? 0 : ASCII_FLAG, // flags
				0,0,0,0, // time
				0, //xflags 
				OS_CODE
			};
			if ( m_backBuffer ) {
				if ( m_backBuffer->sputn(header, sizeof(header)) != sizeof(header) ) {
					// TypeParent::pbump(-num);
					return std::char_traits<char>::eof();
				}
			} else if ( m_coupledBuffer ) {
				if ( m_coupledBuffer->sputn(header, sizeof(header)) != sizeof(header) ) {
					// TypeParent::pbump(-num);
					return std::char_traits<char>::eof();
				}
			}
		}
		
		do {
			int flush = finish ? Z_FINISH : Z_NO_FLUSH;
			
			/// @todo If not binary and Win32: \n ->  \r\n
			
			m_strmOut.avail_out = static_cast<uInt>(m_transferOutBuffer.size());
			m_strmOut.next_out = (Bytef *)(m_transferOutBuffer.begin());
			
			ret = deflate(&m_strmOut, flush);
			
			if ( ret == Z_STREAM_ERROR ) {
				// TypeParent::pbump(-num);
				return std::char_traits<char>::eof();
			}
			
			int have = m_transferOutBuffer.size() - m_strmOut.avail_out;
			
			if ( m_backBuffer ) {
				if ( m_backBuffer->sputn(m_transferOutBuffer.begin(), have) != have
					&&
					have > 0 )
				{
					// TypeParent::pbump(-num);
					return std::char_traits<char>::eof();
				}
			} else if ( m_coupledBuffer ) {
				if ( m_coupledBuffer->sputn(m_transferOutBuffer.begin(), have) != have
					&&
					have > 0 )
				{
					// TypeParent::pbump(-num);
					return std::char_traits<char>::eof();
				}
			} 
		} while ( m_strmOut.avail_out == 0 );
		
		m_out += num;
		m_crcOut = crc32(m_crcOut,
						 (const Bytef *)m_frontOutBuffer.begin(),
						 (unsigned int)num);
	}
	
	TypeParent::pbump(-num);
	return num;
}

CFrontStreambuf::int_type CFrontStreambuf::overflow(int_type c)
{
	if ( c != std::char_traits<char>::eof() ) {
		*TypeParent::pptr() = c;
		TypeParent::pbump(1);
	}
	if ( flushBuffer(false) == std::char_traits<char>::eof() ) {
		return std::char_traits<char>::eof();
	}
	return c;
}

CFrontStreambuf::int_type CFrontStreambuf::underflow()
{
	if ( TypeParent::gptr() < TypeParent::egptr() ) {
		return *TypeParent::gptr();
	}
	
	if ( m_inIsEOF ) {
		return std::char_traits<char>::eof();
	}
	
	int_type numPutback;
	numPutback = (int_type)(TypeParent::gptr() - TypeParent::eback());
	if ( numPutback > m_putbackSize )
		numPutback = m_putbackSize;
	
	if ( numPutback ) {
		memcpy(m_frontInBuffer.begin() + (m_putbackSize-numPutback),
			   TypeParent::gptr() - numPutback,
			   numPutback);
	}
	
	// Read new Characters
	std::streamsize num = 0;
	
	m_strmIn.avail_out = static_cast<uInt>
		(m_frontInBuffer.size()-m_putbackSize);
	m_strmIn.next_out = reinterpret_cast<Bytef *>
		(m_frontInBuffer.begin()+m_putbackSize);
	while ( m_strmIn.avail_out != 0 ) {
		fill_in_buffer();
		if ( m_strmIn.avail_in != 0 ) {
			if ( !m_transparentIn ) {
				inflate(&m_strmIn, Z_NO_FLUSH);
			} else {
				uInt avail = tmin(m_strmIn.avail_in, m_strmIn.avail_out);
				memcpy(m_strmIn.next_out, m_strmIn.next_in, avail);
				m_strmIn.avail_out -= avail;
				m_strmIn.avail_in -= avail;
				m_strmIn.next_out += avail;
				m_strmIn.next_in += avail;
			}
			continue;
		}
		break;
	}
	
	num = static_cast<std::streamsize>
	((m_frontInBuffer.size()-m_putbackSize) - m_strmIn.avail_out);
	
	setg(m_frontInBuffer.begin()+(m_putbackSize-numPutback), 
		 m_frontInBuffer.begin()+m_putbackSize,
		 m_frontInBuffer.begin()+m_putbackSize+num);
	
	if ( num == 0 )
		return std::char_traits<char>::eof();
	
	return *TypeParent::gptr();
}

bool CFrontStreambuf::postOpen(TypeOpenMode mode,
							   int compressLevel)
{			
	m_mode = mode;
	m_compressLevelOut = compressLevel;
	
	if ( m_compressLevelOut != Z_NO_COMPRESSION  &&
		(m_mode & std::ios_base::out) )
	{
		// Set the compression level for the output
		m_strmOut.zalloc = Z_NULL;
		m_strmOut.zfree = Z_NULL;
		m_strmOut.opaque = Z_NULL;
		m_strmOut.avail_in = 0;
		m_strmOut.avail_out = 0;
		
		m_out = 0;
		m_crcOut = crc32(0L, Z_NULL, 0);
		m_strategyOut = Z_DEFAULT_STRATEGY;
		
		setp(m_frontOutBuffer.begin(), m_frontOutBuffer.begin()+(m_frontOutBuffer.size()-1));
		int ret = deflateInit2(&m_strmOut,
							   m_compressLevelOut,
							   m_methodOut,
							   -MAX_WBITS,
							   DEF_MEM_LEVEL,
							   m_strategyOut);
		
		if ( ret != Z_OK ) {
			// Could not initialize the zip stream structure
			return false;
		}
	}
	
	if ( m_mode & std::ios_base::in ) {
		// Set the compression level for the output
		m_strmIn.zalloc = Z_NULL;
		m_strmIn.zfree = Z_NULL;
		m_strmIn.opaque = Z_NULL;
		m_strmIn.avail_in = 0;
		m_strmIn.avail_out = 0;
		
		m_strategyIn = Z_DEFAULT_STRATEGY;
		
		m_transparentIn = compressLevel == Z_NO_COMPRESSION;
		m_in = 0;
		m_inIsEOF = false;
		m_crcIn = crc32(0L, Z_NULL, 0);
		setg(m_frontInBuffer.begin()+m_putbackSize,
			 m_frontInBuffer.begin()+m_putbackSize,
			 m_frontInBuffer.begin()+m_putbackSize);
		
		if ( !m_transparentIn && !check_header() )
			return false;
		
		if ( !m_transparentIn ) {
			int ret = inflateInit2(&m_strmIn, -MAX_WBITS);
			if ( ret != Z_OK ) {
				return false;
			}
		}
	}
	
	return true;
}

bool CFrontStreambuf::open(const CUri &refUri,
						   TypeOpenMode mode,
						   int compressLevel)
{
	close();
	
	// Get the absolute URI using base and reference URI
	if ( !CUri::makeAbsolute(m_resolutionUri, m_baseUri, refUri, false) ) {
		return false;
	}
	
	// Get the back buffer and its factory for the URI scheme (e.g. file)
	m_factory = m_bufferReg->getBufferFactory(m_resolutionUri.getScheme().c_str());
	if ( m_factory ) {
		TypeOpenMode backMode = mode;
		if ( mode & std::ios_base::in || compressLevel != Z_NO_COMPRESSION  )
			backMode |= std::ios_base::binary;
		m_backBuffer = m_factory->open(m_resolutionUri, backMode);
		if ( !m_backBuffer )
			m_factory = 0;
	}
	
	if ( m_factory != 0 )
		return postOpen(mode, compressLevel);
	
	return false;
}

void CFrontStreambuf::open(TypeParent *aBuffer,
						   TypeOpenMode mode,
						   int compressLevel)
{
	close();
	m_coupledBuffer = aBuffer;
	postOpen(mode, compressLevel);
}

bool CFrontStreambuf::close()
{
	flushBuffer(true);
	if ( !(m_coupledBuffer || m_backBuffer) ) {
		return false;
	}
	
	bool result = true;
	if ( m_compressLevelOut != Z_NO_COMPRESSION &&
		(m_mode & std::ios_base::out) )
	{
		// Put compression trailer
		unsigned char c[2][4];
		putLong (c[0], m_crcOut);
		putLong (c[1], (unsigned long)(m_out & 0xffffffff));
		if ( m_backBuffer ) {
			if ( !m_backBuffer->sputn(reinterpret_cast<const char *>(&c[0][0]),
									  8) )
			{
				result = false;
			}
		} else if ( m_coupledBuffer ) {
			if ( !m_coupledBuffer->sputn(reinterpret_cast<const char *>(&c[0][0]),
										 8) )
			{
				result = false;
			}
		}
		deflateEnd(&m_strmOut);
	}
	
	if ( !m_transparentIn && (m_mode & std::ios_base::in) ) {
		inflateEnd(&m_strmIn);
	}
	
	if ( m_factory && m_backBuffer )
		result = m_factory->close(m_backBuffer) || result;
	
	m_coupledBuffer = 0;
	m_backBuffer = 0;
	m_factory = 0;
	m_transferOutBuffer.resize(0);
	
	return result;
}
