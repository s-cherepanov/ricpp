#ifndef _RICPP_STREAMS_BACKBUFFER_H
#define _RICPP_STREAMS_BACKBUFFER_H

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

/** @file backbuffer.h
 *  @author Andreas Pidde (andreas@pidde.de)
 *  @brief Headerfile with templates and base classes for zlib-enabled streambuffer facets.
 */

#ifndef _RICPP_STREAMS_URI_H
#include "ricpp/streams/uri.h"
#endif // _RICPP_STREAMS_URI_H

#ifndef _RICPP_PLUGINHANDLER_PLUGINHANDLER_H
#include "ricpp/pluginhandler/pluginhandler.h"
#endif // _RICPP_PLUGINHANDLER_PLUGINHANDLER_H

#ifndef _RICPP_STREAMS_BUFFER_H
#include "ricpp/streams/buffer.h"
#endif // _RICPP_STREAMS_BUFFER_H

#include "zlib.h"

#include <fstream>

namespace RiCPP {

class CBackBufferRoot {
protected:
	CUri m_lastFileName;
public:
	inline virtual ~CBackBufferRoot() {}
	inline virtual void close() { }
	inline virtual bool open(
		const CUri &absUri,
		TypeOpenMode mode = std::ios_base::in|std::ios_base::binary)
	{
		m_lastFileName = absUri;
		return false;
	}
	inline virtual bool isOpen() const { return false; }
	inline virtual std::streamsize sgetn(char *b, std::streamsize size) { return 0; }
	inline virtual std::streamsize sputn(const char *b, std::streamsize size) { return 0; }
	inline virtual const CUri &lastFileName() const { return m_lastFileName; }
}; // CBackBufferRoot


class CFileBackBuffer : public CBackBufferRoot {
	std::filebuf m_filebuf;
	TypeOpenMode m_mode;
public:
	inline CFileBackBuffer() {}

	inline virtual ~CFileBackBuffer()
	{
		close();
	}

	inline virtual void close()
	{
		if ( m_filebuf.is_open() ) {
			m_filebuf.close();
		}
	}

	inline virtual bool open(
		const CUri &absUri,
		TypeOpenMode mode = std::ios_base::in|std::ios_base::binary)
	{
		CBackBufferRoot::open(absUri, mode);
		close();
		m_mode = mode;
		std::string filename(absUri.getHierPart());
		CFilepathConverter::convertToNative(filename);
		m_filebuf.open(filename.c_str(), mode);
		return m_filebuf.is_open();
	}

	inline virtual bool isOpen() const
	{
		return m_filebuf.is_open();
	}

	inline virtual std::streamsize sgetn(char *b, std::streamsize size) 
	{
		if ( !isOpen() || !b || !size ) {
			return 0;
		}
		if ( !(m_mode & std::ios_base::in) ) {
			return 0;
		}
#ifdef _MSC_VER
		return m_filebuf._Sgetn_s(b, size, size);
#else
		return m_filebuf.sgetn(b, size);
#endif
	}

	inline virtual std::streamsize sputn(const char *b, std::streamsize size) 
	{
		if ( !isOpen() || !b || !size ) {
			return 0;
		}
		if ( !(m_mode & std::ios_base::out) ) {
			return 0;
		}
		return m_filebuf.sputn(b, size);
	}
}; // CFileBackBuffer


class CBackBufferFactory : public IPlugin {

	CStringList m_schemes;
	TemplObjPtrRegistry<CBackBufferRoot *, CBackBufferRoot *> m_myBuffers;

protected:

	void addScheme(const char *scheme)
	{
		std::string str_scheme(noNullStr(scheme));
		if ( str_scheme.empty() )
			return;
		std::for_each(str_scheme.begin(), str_scheme.end(), std::ptr_fun(toupper));
		m_schemes.push(str_scheme, false, false, false);
	}

	bool registerObj(CBackBufferRoot *b)
	{
		if ( !b )
			return false;
		return m_myBuffers.registerObj(b, b);
	}

	bool unRegisterObj(CBackBufferRoot *b)
	{
		if ( !b )
			return false;
		return m_myBuffers.unregisterObj(b);
	}

public:
	/** @brief Const iterator for the protocol identifiers.
	 */
	typedef CStringList::const_iterator const_iterator;

	/** @brief Size type for the number of stored protocol identifiers.
	 */
	typedef CStringList::size_type size_type;

	static const char *myName();
	static const char *myType();
	static unsigned long myMajorVersion();
	static unsigned long myMinorVersion();
	static unsigned long myRevision();

	inline CBackBufferFactory::CBackBufferFactory() : m_myBuffers(true) {}
	inline virtual ~CBackBufferFactory() {}

	inline virtual const char *name() const { return myName(); }
	inline virtual const char *type() const { return myType(); }
	inline virtual unsigned long majorVersion() const {return myMajorVersion(); }
	inline virtual unsigned long minorVersion() const {return myMinorVersion(); }
	inline virtual unsigned long revision() const { return myRevision(); }

	inline virtual void startup() {}
	inline virtual void shutdown() {}

	inline virtual bool acceptsScheme(const char *scheme) const
	{
		std::string str_scheme(noNullStr(scheme));
		std::for_each(str_scheme.begin(), str_scheme.end(), std::ptr_fun(asciiToUpper));
		return m_schemes.isMember(str_scheme.c_str());
	}

	inline virtual const_iterator begin() const { return m_schemes.begin(); }
	inline virtual const_iterator end() const { return m_schemes.end(); }
	inline virtual size_type size() const { return m_schemes.size(); }

	inline virtual CBackBufferRoot *open(const CUri &absUri, TypeOpenMode mode = std::ios_base::in|std::ios_base::binary)
	{
		return 0;
	}
	inline virtual bool close(CBackBufferRoot *bbr)
	{
		bool rval = false;
		if ( bbr ) {
			bbr->close();
			return unRegisterObj(bbr);
		}
		return rval;
	}
}; // CBackBufferFactory


class CFileBackBufferFactory : public CBackBufferFactory {
public:
	static const char *myType();
	static const char *myName();
	static unsigned long myMajorVersion();
	static unsigned long myMinorVersion();
	static unsigned long myRevision();

	inline CFileBackBufferFactory::CFileBackBufferFactory() { addScheme("FILE"); }
	inline virtual ~CFileBackBufferFactory() {}

	inline virtual const char *type() const { return myType(); }
	inline virtual const char *name() const { return myName(); }
	inline virtual unsigned long majorVersion() const { return myMajorVersion(); }
	inline virtual unsigned long minorVersion() const { return myMinorVersion(); }
	inline virtual unsigned long revision() const { return myRevision(); }

	inline virtual void startup() {}
	inline virtual void shutdown() {}

	inline virtual CBackBufferRoot *open(
		const CUri &absUri,
		TypeOpenMode mode = std::ios_base::in|std::ios_base::binary)
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
}; // CFileBackBufferFactory


class CBackBufferRegistry {
	
	std::string m_direct;
	bool m_hasDirect;
	TemplPluginHandler<CBackBufferFactory> m_backBufferPluginHandler;
	TemplPluginFactory<CFileBackBufferFactory> m_fileBuffer;
	
	inline void init(const char *direct)
	{
		m_direct = direct ? direct : "";
		m_hasDirect = direct != 0;
		if ( !m_backBufferPluginHandler.isRegistered("filebuffer") ) {
			m_backBufferPluginHandler.registerFactory(
				"file.buffer",
				(TemplPluginFactory<CBackBufferFactory> *)&m_fileBuffer );
			try {
				m_backBufferPluginHandler.registerFromDirectory(direct, ".buffer");
			} catch ( ... ) {
				throw;
			}
		}
	}

	inline TemplPluginHandler<CBackBufferFactory> &getHandler()
	{
		return m_backBufferPluginHandler;
	}

public:
	inline CBackBufferRegistry(const char *direct=0) {init(direct);}
	inline ~CBackBufferRegistry() {}
	inline CBackBufferFactory *getBufferFactory(const char *scheme)
	{
		TemplPluginHandler<CBackBufferFactory>::const_iterator i;
		for ( i = m_backBufferPluginHandler.begin(); i != m_backBufferPluginHandler.end(); i++ ) {
			CBackBufferFactory *f = (*i).second->lastPlugin();
			if ( f && f->acceptsScheme(scheme) )
				return f;
		}
		return 0;
	}
}; // CBackBufferRegistry

template<class charT, class traits=std::char_traits<charT> >
class TemplFrontStreambuf : public std::basic_streambuf<charT, traits> {

public:
    typedef typename std::basic_streambuf<charT, traits>::int_type int_type;
    typedef typename std::basic_streambuf<charT, traits>::pos_type pos_type;
    typedef typename std::basic_streambuf<charT, traits>::off_type off_type;

	typedef typename std::basic_streambuf<charT, traits> TypeParent;
	  
private:
	static const int gz_magic_0 = 0x1f;
	static const int gz_magic_1 = 0x8b; /* gzip magic header */

	/* gzip flag byte */
	static int const ASCII_FLAG   = 0x01; /* bit 0 set: file probably ascii text */
	static int const HEAD_CRC     = 0x02; /* bit 1 set: header CRC present */
	static int const EXTRA_FIELD  = 0x04; /* bit 2 set: extra field present */
	static int const ORIG_NAME    = 0x08; /* bit 3 set: original file name present */
	static int const COMMENT      = 0x10; /* bit 4 set: file comment present */
	static int const RESERVED     = 0xE0; /* bits 5..7: reserved */

	// RiCPP will be only WIN32, MACOS or Unix
	#if defined(WIN32)
		static int const OS_CODE = 0x0b;
	#else
		#if defined(MACOS) || defined(TARGET_OS_MAC)
			static int const OS_CODE = 0x07;
		#else
			static int const OS_CODE = 0x03;
		#endif
	#endif

	#if MAX_MEM_LEVEL >= 8
		static int const DEF_MEM_LEVEL = 8;
	#else
		static int const DEF_MEM_LEVEL = MAX_MEM_LEVEL;
	#endif

	TypeParent *m_coupledBuffer;
	CBackBufferRoot *m_backBuffer;
	CBackBufferFactory *m_factory;
	CBackBufferRegistry *m_bufferReg;

	TypeOpenMode m_mode;
	CUri m_baseUri;
	CUri m_resolutionUri;

	int_type m_buffersize;
	int_type m_putbackSize;

	TemplBuffer<charT> m_frontInBuffer;
	z_stream m_strmIn;
	TemplBuffer<unsigned char> m_transferInBuffer;
	bool m_inIsEOF;
	long m_in;
	long m_crcIn;
	int m_transparentIn;
	int m_strategyIn;
	char m_methodIn;

	TemplBuffer<charT> m_frontOutBuffer;
	z_stream m_strmOut;
	TemplBuffer<char> m_transferOutBuffer;
	long m_out;
	long m_crcOut;
	int m_compressLevelOut;
	int m_strategyOut;
	char m_methodOut;

	inline void init()
	{
		m_backBuffer = 0;
		m_coupledBuffer = 0;
		m_factory = 0;

		m_mode = static_cast<TypeOpenMode>(0);

		m_buffersize = 8192;
		m_putbackSize = 128;

		m_frontInBuffer.clear();
		m_frontInBuffer.resize(m_buffersize);
		m_in = 0;
		m_crcIn = 0;
		m_transparentIn = true;
		m_strategyIn = Z_DEFAULT_STRATEGY;
		m_methodIn = Z_DEFLATED;
		setg(
			m_frontInBuffer.begin()+m_putbackSize,
			m_frontInBuffer.begin()+m_putbackSize,
			m_frontInBuffer.begin()+m_putbackSize);


		m_frontOutBuffer.clear();
		m_frontOutBuffer.resize(m_buffersize);
		m_out = 0;
		m_crcOut = 0;
		m_compressLevelOut = Z_DEFAULT_COMPRESSION;
		m_strategyOut = Z_DEFAULT_STRATEGY;
		m_methodOut = Z_DEFLATED;
		setp(m_frontOutBuffer.begin(), m_frontOutBuffer.end()-1);

	}

	inline void putLong(unsigned char c[4], unsigned long x) const
	{
		for (int n = 0; n < 4; n++) {
			c[n] = (unsigned char)(x & 0xff);
			x >>= 8;
		}
	}

	inline unsigned long getLong(const unsigned char c[4]) const
	{
		unsigned long x = (unsigned long)c[0];
		x += ((unsigned long)c[1])<<8;
		x += ((unsigned long)c[2])<<16;
		x += ((unsigned long)c[3])<<24;
		return x;
	}

		
	inline int get_byte()
	{
		if ( m_strmIn.avail_in == 0 ) {
			fill_in_buffer();
			if (m_strmIn.avail_in == 0) {
				return EOF;
			}
			m_strmIn.next_in = m_transferInBuffer.begin();
		}

		m_strmIn.avail_in--;
		return (int)*(m_strmIn.next_in++);
	}

	inline bool check_header()
	{
		int method; // method byte
		int flags;  // flags byte
		uInt len;

		// Stream buffer is greater than 2, if it is less tahn 2,
		// the file is smaller as 2 Bytes
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
			while ( get_byte() != EOF ) ;
		}
		if ((flags & COMMENT) != 0) {   // skip the .gz file comment
			while ( get_byte() != EOF ) ;
		}
		if ((flags & HEAD_CRC) != 0) {  // skip the header crc 16Bit
			for (len = 0; len < 2; len++) {
				get_byte();
			}
		}

		return !m_inIsEOF;
	}

	inline unsigned int fill_in_buffer()
	{
		if ( m_strmIn.avail_in != 0 ) {
			return m_strmIn.avail_in;
		}

		if ( m_inIsEOF )
			return 0;

		// ToDo if not binary and Win32: \r\n -> \n

		bool startstream = false;
		if ( m_transferInBuffer.size() == 0 ) {
			int additionalchars = 8;
			if ( m_buffersize < 16 ) {
				additionalchars += 16;
			}
			m_transferInBuffer.resize(m_buffersize*sizeof(charT)+additionalchars);
			m_strmIn.next_in = m_transferInBuffer.begin()+8;
			startstream = true;
		} else if ( m_strmIn.next_in < m_transferInBuffer.begin()+(m_transferInBuffer.size()-8) ) {
			m_inIsEOF = true;
			return 0;
		} else {
			TemplBuffer<unsigned char>::iterator e = m_transferInBuffer.begin()+(m_transferInBuffer.size()-8);
			TemplBuffer<unsigned char>::iterator b = m_transferInBuffer.begin();

			for ( int i=0; i<8; ++i ) {
				*(b++) = *(e++);
			}
			m_strmIn.next_in = m_transferInBuffer.begin();
		}

		if ( m_backBuffer ) {
			m_strmIn.avail_in =
				(uInt)m_backBuffer->sgetn(
					(char *)m_transferInBuffer.begin()+8,
					(std::streamsize)m_transferInBuffer.size()-8);
		} else if ( m_coupledBuffer ) {
			#ifdef _MSC_VER
				m_strmIn.avail_in =
					(uInt)m_coupledBuffer->_Sgetn_s(
						(char *)m_transferInBuffer.begin()+8,
						m_transferInBuffer.size()-8,
						(std::streamsize)m_transferInBuffer.size()-8);
			#else
				m_strmIn.avail_in =
					m_coupledBuffer->sgetn(
						(char *)m_transferInBuffer.begin()+8,
						m_transferInBuffer.size()-8);
			#endif
		}

		if ( startstream && m_strmIn.avail_in > 2 ) {
			if (m_strmIn.next_in[0] == gz_magic_0 &&
				m_strmIn.next_in[1] == gz_magic_1)
			{
				if ( m_strmIn.avail_in >= 10 ) {
					// At least the 8 Byte at the and of the zipfile (4 Byte CRC, 4 Byte length)
					m_strmIn.avail_in -= 8;
				}
				// If the filesize is smaller than 10 the header check will fail.
			}
		}

		return m_strmIn.avail_in;
	}

	inline TemplFrontStreambuf(TemplFrontStreambuf &) {}

protected:
	inline virtual int flushBuffer(bool finish)
	{
		if ( !(m_mode & std::ios_base::out) ) {
			return 0;
		}
		int num = static_cast<int>(TypeParent::pptr() - TypeParent::pbase());
		if ( num <= 0 ) {
			TypeParent::pbump(0);
			return 0;
		}

		if ( !m_backBuffer && !m_coupledBuffer ) {
			TypeParent::pbump(-num);
			return 0;
		}

		if ( m_compressLevelOut == Z_NO_COMPRESSION ) {
			if ( m_backBuffer ) {
				m_backBuffer->sputn(m_frontOutBuffer.begin(), num*sizeof(charT));
			} else if ( m_coupledBuffer ) {
				m_coupledBuffer->sputn(m_frontOutBuffer.begin(), num);
			}
		} else {
			// Copy to/from transferbuff with zlib, transfer the back buffer or
			// coupled buffer
			int ret;

			m_strmOut.avail_in = num*sizeof(charT);
			m_strmOut.next_in = (Bytef *)(m_frontOutBuffer.begin());

			if ( m_transferOutBuffer.size() == 0 ) {
				m_transferOutBuffer.resize(m_buffersize*sizeof(charT));

				// Write compress header
				char header[10] = {
					(char)gz_magic_0, (char)gz_magic_1,
					m_methodOut,
					0, // flags
					0,0,0,0, // time
					0, //xflags 
					OS_CODE
				};
				if ( m_backBuffer ) {
					if ( !m_backBuffer->sputn(header, sizeof(header)) ) {
						TypeParent::pbump(-num);
						return 0;
					}
				} else if ( m_coupledBuffer ) {
					if ( !m_coupledBuffer->sputn(header, sizeof(header)) ) {
						TypeParent::pbump(-num);
						return 0;
					}
				}
			}

			do {
				int flush = finish ? Z_FINISH : Z_NO_FLUSH;

				// ToDo if not binary and Win32: \n ->  \r\n

				m_strmOut.avail_out = static_cast<uInt>(m_transferOutBuffer.size());
				m_strmOut.next_out = (Bytef *)(m_transferOutBuffer.begin());

				ret = deflate(&m_strmOut, flush);

				if ( ret == Z_STREAM_ERROR ) {
					TypeParent::pbump(-num);
					return 0;
				}

				int have = (int)(m_transferOutBuffer.size() - m_strmOut.avail_out);

				if ( m_backBuffer ) {
					if ( !m_backBuffer->sputn(m_transferOutBuffer.begin(), have) && have != 0 ) {
						TypeParent::pbump(-num);
						return 0;
					}
				} else if ( m_coupledBuffer ) {
					if ( !m_coupledBuffer->sputn(m_transferOutBuffer.begin(), have) && have != 0 ) {
						TypeParent::pbump(-num);
						return 0;
					}
				} 
			} while ( m_strmOut.avail_out == 0 );

			m_out = num;
			m_crcOut = crc32(m_crcOut, (const Bytef *)m_frontOutBuffer.begin(), (unsigned int)num);
		}

		TypeParent::pbump(-num);
		return num;
	}

	inline virtual int_type overflow(int_type c)
	{
		if ( c != traits::eof() ) {
			*TypeParent::pptr() = c;
			TypeParent::pbump(1);
		}
		if ( flushBuffer(false) == traits::eof() ) {
			return traits::eof();
		}
		return c;
	}

	inline virtual int_type underflow()
	{
		if ( TypeParent::gptr() < TypeParent::egptr() ) {
			return *TypeParent::gptr();
		}
		
		if ( m_inIsEOF ) {
			return traits::eof();
		}

		int_type numPutback;
		numPutback = (int_type)(TypeParent::gptr() - TypeParent::eback());
		if ( numPutback > m_putbackSize )
			numPutback = m_putbackSize;

		if ( numPutback ) {
			memcpy(m_frontInBuffer.begin()+(m_putbackSize-numPutback), TypeParent::gptr()-numPutback, numPutback*sizeof(charT));
		}

		// Read new Characters
		std::streamsize num = 0;
		if ( m_transparentIn ) {
			if ( m_backBuffer ) {
				num =
					m_backBuffer->sgetn(
						(char *)(m_frontInBuffer.begin()+m_putbackSize),
							(std::streamsize)(sizeof(charT)*(m_frontInBuffer.size()-m_putbackSize)));
			} else if ( m_coupledBuffer ) {
				#ifdef _MSC_VER
					num =
						m_coupledBuffer->_Sgetn_s(
							(char *)(m_frontInBuffer.begin()+m_putbackSize),
							sizeof(charT)*(m_frontInBuffer.size()-m_putbackSize),
							(std::streamsize)(sizeof(charT)*(m_frontInBuffer.size()-m_putbackSize)));
				#else
					num =
						m_coupledBuffer->sgetn(
							(char *)(m_frontInBuffer.begin()+m_putbackSize),
							sizeof(charT)*(m_frontInBuffer.size()-m_putbackSize));
				#endif
			}
		} else {
			m_strmIn.avail_out = (uInt)(sizeof(charT)*(m_frontInBuffer.size()-m_putbackSize));
			m_strmIn.next_out = (Bytef *)(m_frontInBuffer.begin()+m_putbackSize);
			while ( m_strmIn.avail_out != 0 ) {
				fill_in_buffer();
				if ( m_strmIn.avail_in != 0 ) {
					inflate(&m_strmIn, Z_NO_FLUSH);
					continue;
				}
				break;
			}
			num = (std::streamsize)(
				sizeof(charT) *
				(m_frontInBuffer.size()-m_putbackSize) -
				m_strmIn.avail_out);
		}

		setg(
			m_frontInBuffer.begin()+(m_putbackSize-numPutback), 
			m_frontInBuffer.begin()+m_putbackSize,
			m_frontInBuffer.begin()+m_putbackSize+num);

		if ( num == 0 )
			return traits::eof();

		return *TypeParent::gptr();
	}

	inline virtual void setBaseCwd()
	{
		CFilepath p;
		std::string path(p.filepath());
		path += CFilepathConverter::internalPathSeperator();
		m_baseUri.set("file", "", path.c_str(), 0, 0);
	}

public:
	inline TemplFrontStreambuf(CBackBufferRegistry &bufferReg) :
		m_bufferReg(&bufferReg)
	{
		init();
	}

	inline virtual ~TemplFrontStreambuf() throw() { close(); }

	inline virtual bool base(const CUri &base)
	{
		if ( base.isValid() ) {
			m_baseUri = base;
		} else {
			return true;
		}
		return m_baseUri.isValid();
	}

	inline virtual const CUri &base() const
	{
		return m_baseUri;
	}

	inline virtual bool open(
		const CUri &refUri,
		TypeOpenMode mode = std::ios_base::in|std::ios_base::binary,
		int compressLevel = Z_DEFAULT_COMPRESSION)
	{
		m_mode = mode;
		m_compressLevelOut = compressLevel;
		if ( m_factory && m_backBuffer ) {
			m_factory->close(m_backBuffer);
			m_backBuffer = 0;
			m_factory = 0;
		}
		m_coupledBuffer = 0;

		if ( m_baseUri.toString().empty() ) {
			setBaseCwd();
		}

		if ( !CUri::makeAbsolute(m_resolutionUri, m_baseUri, refUri, true) )
		{
			return false;
		}

		m_factory = m_bufferReg->getBufferFactory(m_resolutionUri.getScheme().c_str());
		if ( m_factory ) {
			m_backBuffer = m_factory->open(m_resolutionUri, compressLevel != 0 ? mode|std::ios_base::binary : mode);
			if ( !m_backBuffer )
				m_factory = 0;
		}

		if ( m_compressLevelOut != Z_NO_COMPRESSION  && (m_mode & std::ios_base::out) ) {
			m_strmOut.zalloc = Z_NULL;
			m_strmOut.zfree = Z_NULL;
			m_strmOut.opaque = Z_NULL;
			m_strmOut.avail_in = 0;
			m_strmOut.avail_out = 0;
			
			m_out = 0;
			// m_crcOut = 0;
			m_crcOut = crc32(0L, Z_NULL, 0);
			m_strategyOut = Z_DEFAULT_STRATEGY;

			setp(m_frontOutBuffer.begin(), m_frontOutBuffer.end()-1);
			int ret = deflateInit2(
				&m_strmOut,
				m_compressLevelOut,
				m_methodOut,
				-MAX_WBITS,
				DEF_MEM_LEVEL,
				m_strategyOut);
			if ( ret != Z_OK ) {
				return false;
			}
		}

		if ( m_mode & std::ios_base::in ) {
			m_strmIn.zalloc = Z_NULL;
			m_strmIn.zfree = Z_NULL;
			m_strmIn.opaque = Z_NULL;
			m_strmIn.avail_in = 0;
			m_strmIn.avail_out = 0;
			
			m_strategyIn = Z_DEFAULT_STRATEGY;
			m_transparentIn = compressLevel == Z_NO_COMPRESSION;
			m_in = 0;
			m_inIsEOF = false;
			// m_crcIn = 0;
			m_crcIn = crc32(0L, Z_NULL, 0);
			setg(
				m_frontInBuffer.begin()+m_putbackSize,
				m_frontInBuffer.begin()+m_putbackSize,
				m_frontInBuffer.begin()+m_putbackSize);

			if ( !m_transparentIn && !check_header() )
				return false;

			if ( !m_transparentIn ) {
				int ret = inflateInit2(
					&m_strmIn,
					-MAX_WBITS);
				if ( ret != Z_OK ) {
					return false;
				}
			}
		}

		return m_backBuffer != 0;
	}

	inline virtual bool close()
	{
		flushBuffer(true);
		if ( m_factory && m_backBuffer ) {
			if ( m_compressLevelOut != Z_NO_COMPRESSION && (m_mode & std::ios_base::out) ) {
				unsigned char c[2][4];
				putLong (c[0], m_crcOut);
				putLong (c[1], (unsigned long)(m_out & 0xffffffff));
				if ( m_backBuffer ) {
					if ( !m_backBuffer->sputn((const char *)(&c[0][0]), 8) ) {
					}
				} else if ( m_coupledBuffer ) {
					if ( !m_coupledBuffer->sputn((const char *)(&c[0][0]), 8) ) {
					}
				} else {
					// Do nothing
				}
				deflateEnd(&m_strmOut);
			}
			if ( !m_transparentIn && (m_mode & std::ios_base::in) ) {
				inflateEnd(&m_strmIn);
			}
			bool rval = m_factory->close(m_backBuffer);
			m_backBuffer = 0;
			m_factory = 0;
			m_transferOutBuffer.resize(0);
			return rval;
		}
		return false;
	}

	inline virtual bool rewind() const
	{
		return false;
	}

	inline virtual void connect(TypeParent *aBuffer)
	{
		close();
		m_coupledBuffer = aBuffer;
	}

	inline virtual void disconnect()
	{
		m_coupledBuffer = 0;
	}

	inline TypeParent *rdbuf()
	{
		return m_coupledBuffer;
	}
}; // TemplFrontStreambuf

} // namespace RiCPP

#endif // _RICPP_STREAMS_BACKBUFFER_H
