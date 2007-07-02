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

/** @file main.cpp
 *  @author Andreas Pidde (andreas@pidde.de)
 *  @brief Used to test.
 */

#include "ricpp/ricppbridge/ricppbridge.h"
#include "ricpp/tools/filepath.h"
#include "ricpp/streams/uri.h"
#include "ricpp/streams/buffer.h"

#include "zlib.h"

#include <iostream>
#include <fstream>
#include <streambuf>

using namespace RiCPP;


// Extracted from zutil.h
// RiCPP will be only WIN32, MACOS or Unix
#ifdef WIN32
#define OS_CODE  0x0b
#endif

#if defined(MACOS) || defined(TARGET_OS_MAC)
#define OS_CODE  0x07
#endif

#ifndef OS_CODE
#define OS_CODE  0x03  // assume Unix
#endif

#if MAX_MEM_LEVEL >= 8
#  define DEF_MEM_LEVEL 8
#else
#  define DEF_MEM_LEVEL  MAX_MEM_LEVEL
#endif
// End of extraction from zutil.h

class CBackBufferRoot {
protected:
	CUri m_lastFileName;
public:
	inline virtual ~CBackBufferRoot() {}
	virtual void close() { }
	virtual bool open(const CUri &absUri, TypeOpenMode mode = std::ios_base::in|std::ios_base::binary) { m_lastFileName = absUri; return false; }
	inline virtual bool isOpen() const { return false; }
	inline virtual std::streamsize sgetn(char *b, std::streamsize size) { return 0; }
	inline virtual std::streamsize sputn(const char *b, std::streamsize size) { return 0; }
	inline virtual const CUri &lastFileName() const { return m_lastFileName; }
};

class CFileBackBuffer : public CBackBufferRoot {
	std::fstream m_stream;
	TypeOpenMode m_mode;
public:
	inline CFileBackBuffer()
	{
	}
	inline virtual ~CFileBackBuffer()
	{
		close();
	}
	virtual void close()
	{
		if ( m_stream.is_open() ) {
			m_stream.close();
			m_stream.clear();
		}
	}
	inline virtual bool open(const CUri &absUri, TypeOpenMode mode = std::ios_base::in|std::ios_base::binary)
	{
		CBackBufferRoot::open(absUri, mode);
		close();
		m_stream.clear();
		m_mode = mode;
		std::string filename(absUri.getHierPart());
		CFilepathConverter::convertToNative(filename);
		m_stream.open(filename.c_str(), mode);
		return m_stream && m_stream.is_open();
	}

	inline virtual bool isOpen() const
	{
		return m_stream.is_open();
	}

	inline virtual std::streamsize sgetn(char *b, std::streamsize size) 
	{
		if ( !m_stream || !b || !size )
			return 0;
		if ( !(m_mode & std::ios_base::in) )
			return 0;

#ifdef _MSC_VER
		m_stream._Read_s(b, size, size);
		return m_stream.gcount();
#else
		m_stream.read(b, size);
		return m_stream.gcount();
#endif
	}

	inline virtual std::streamsize sputn(const char *b, std::streamsize size) 
	{
		if ( !m_stream || !b || !size )
			return 0;
		if ( !(m_mode & std::ios_base::out) )
			return 0;

		m_stream.write(b, size);

		return size;
	}
};

class CBackBufferFactory : IPlugin {
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

	bool registerObj(CBackBufferRoot *b) {
		if ( !b )
			return false;
		return m_myBuffers.registerObj(b, b);
	}

	bool unRegisterObj(CBackBufferRoot *b) {
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
	inline virtual bool close(CBackBufferRoot *bbr) {
		bool rval = false;
		if ( bbr ) {
			bbr->close();
			return unRegisterObj(bbr);
		}
		return rval;
	}
};	

const char *CBackBufferFactory::myType() { return "backbufferfactory"; }
const char *CBackBufferFactory::myName() { return "backbufferfactory"; }
unsigned long CBackBufferFactory::myMajorVersion() { return 1; }
unsigned long CBackBufferFactory::myMinorVersion() { return 1; }
unsigned long CBackBufferFactory::myRevision() { return 1; }

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

	inline virtual CBackBufferRoot *open(const CUri &absUri, TypeOpenMode mode = std::ios_base::in|std::ios_base::binary)
	{
		CFileBackBuffer *buf;
		buf = new CFileBackBuffer;
		if ( buf ) {
			if ( !buf->open(absUri, mode) ) {
				delete buf;
				return 0;
			}
		}
		registerObj(buf);
		return buf;
	}
};	

const char *CFileBackBufferFactory::myType() { return CBackBufferFactory::myType(); }
const char *CFileBackBufferFactory::myName() { return "file_backbuffer"; }
unsigned long CFileBackBufferFactory::myMajorVersion() { return CBackBufferFactory::myMajorVersion(); }
unsigned long CFileBackBufferFactory::myMinorVersion() { return 1; }
unsigned long CFileBackBufferFactory::myRevision() { return 1; }

class CBackBufferRegistry {
	std::string m_direct;
	bool m_hasDirect;
	TemplPluginHandler<CBackBufferFactory> m_backBufferPluginHandler;
	TemplPluginFactory<CFileBackBufferFactory> m_fileBuffer;
	inline void init(const char *direct) {
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
};

template<class charT, class traits=std::char_traits<charT> >
class TemplFrontStreambuf : public std::basic_streambuf<charT, traits> {

public:
      typedef typename std::basic_streambuf<charT, traits>::int_type int_type;
      typedef typename std::basic_streambuf<charT, traits>::pos_type pos_type;
      typedef typename std::basic_streambuf<charT, traits>::off_type off_type;
	  
private:
	typedef typename std::basic_streambuf<charT, traits> TypeParent;
	
	static const int gz_magic_0 = 0x1f;
	static const int gz_magic_1 = 0x8b; /* gzip magic header */

	/* gzip flag byte */
	static int const ASCII_FLAG   = 0x01; /* bit 0 set: file probably ascii text */
	static int const HEAD_CRC     = 0x02; /* bit 1 set: header CRC present */
	static int const EXTRA_FIELD  = 0x04; /* bit 2 set: extra field present */
	static int const ORIG_NAME    = 0x08; /* bit 3 set: original file name present */
	static int const COMMENT      = 0x10; /* bit 4 set: file comment present */
	static int const RESERVED     = 0xE0; /* bits 5..7: reserved */

	std::basic_streambuf<charT, traits> *m_coupledBuffer;
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
		setp(m_frontOutBuffer.begin(), m_frontOutBuffer.end());

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


protected:
	inline virtual int flushBuffer(bool finish=false)
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

				m_strmOut.avail_out = static_cast<uInt>(m_transferOutBuffer.size());
				m_strmOut.next_out = (Bytef *)(m_transferOutBuffer.begin());

				ret = deflate(&m_strmOut, flush);

				if ( ret == Z_STREAM_ERROR ) {
					TypeParent::pbump(-num);
					return 0;
				}

				int have = (int)(m_transferOutBuffer.size() - m_strmOut.avail_out);

				if ( m_backBuffer ) {
					if ( !m_backBuffer->sputn(m_transferOutBuffer.begin(), have) ) {
						TypeParent::pbump(-num);
						return 0;
					}
				} else if ( m_coupledBuffer ) {
					if ( !m_coupledBuffer->sputn(m_transferOutBuffer.begin(), have) ) {
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

	inline virtual int sync(bool finish=false)
	{
		return flushBuffer(finish);
	}

	inline virtual int_type overflow(int_type c)
	{
		if ( c != traits::eof() ) {
			*TypeParent::pptr() = c;
			TypeParent::pbump(1);
		}
		if ( flushBuffer() == traits::eof() ) {
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


	inline TemplFrontStreambuf(TemplFrontStreambuf &)
	{
	}

	inline virtual ~TemplFrontStreambuf() throw()
	{
		close();
	}

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
			m_backBuffer = m_factory->open(m_resolutionUri, mode);
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

			setp(m_frontOutBuffer.begin(), m_frontOutBuffer.end());
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
		sync(true);
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
};

void testStream(CBackBufferRegistry &factory)
{
	TemplFrontStreambuf<char> ob(factory);
	std::string str;
	CEnv::find(str, CEnv::tmpName());
	str += CFilepathConverter::nativePathSeperator();
	CFilepathConverter::convertToInternal(str);

	CUri base("file", "", str.c_str(), 0, 0);
	ob.base(base);
	ob.open("ReadMe2.txt.gz", std::ios_base::out|std::ios_base::binary);
	std::ostream myostream(&ob);
	if ( myostream ) {
		myostream << "Teste";
	}
	ob.close();

	TemplFrontStreambuf<char> ib(factory);
	ib.base(base);
	ib.open("ReadMe2.txt.gz", std::ios_base::in|std::ios_base::binary);
	std::istream myistream(&ib);
	if ( myistream ) {
		std::string teststr;
		myistream >> teststr;
		std::cout << teststr << std::endl;
	}
	ib.close();
}

/** @brief Test a base URI with reference URI strings.
 *
 * Prints the results to cout
 *
 * @param baseUri The base URI
 * @param refUriStrs The reference URIs as char pointer array
 * @param isStrict Strict syntax for relative URI (no scheme)
 */
void testURI(const CUri &baseUri, const char **refUriStrs, bool isStrict = true)
{
	if ( !refUriStrs )
		return;

	CUri refUri, absUri;

	for ( const char **strPtr = refUriStrs; *strPtr; ++strPtr ) {
		const char *str = strPtr[0];
		refUri = str;
		bool valid_absUri = false;

		if ( refUri.isValid() ) {
			valid_absUri = CUri::makeAbsolute(absUri, baseUri, refUri, isStrict);
			valid_absUri = absUri.isValid();
			if ( refUri.toString().empty() )
				std::cout << "<>";
		}

		std::cout
			<< (const char *)(refUri.isValid() ? refUri.toString().c_str() : "<invalid>") << " = "
			<< (const char *)(valid_absUri && absUri.isCurrentDocument() ? "(current document)" : "")
			<< (const char *)(valid_absUri ? absUri.toString().c_str() : "<invalid>") << std::endl;
	}
}

/** @brief Test URI RFC 2396
 *
 * Prints the results to cout
 */
void testURI()
{
	CUri testUri("http://myname:abcd@www.nowhere-1.com.:8080/A%20directory;find;more=1/subdirectory/index.php?i=wqqw&j=1234+6#frag");
	CUri testUri2("http://127.0.0.1");
	CUri testUri3("file:///C:/");
	CUri testUri4("ftp:/");
	CUri testUri5("http://1testdom.2sub.3top/anotherpath/4711");
	CUri testUri6("http://1testdom.sub.3top/anotherpath/4711");
	CUri testUri7("http://127.0.0.1/path/index.html");
	CUri testUri8("mailto:a@b.c");

	CUri relUri("adir/name;str/./deldir/../index.html?#anchor");
	CUri relUri2("#fragment");
	CUri relUri3("/rootdir");
	CUri relUri4;

	CUri refUri;
	if ( CUri::makeAbsolute(refUri, testUri, relUri) )
		std::cout << refUri.toString() << std::endl;
	else
		std::cout << "Error in uri" << std::endl;

	if ( CUri::makeAbsolute(refUri, testUri2, relUri) )
		std::cout << refUri.toString() << std::endl;
	else
		std::cout << "Error in uri" << std::endl;

	if ( CUri::makeAbsolute(refUri, testUri3, relUri) )
		std::cout << refUri.toString() << std::endl;
	else
		std::cout << "Error in uri" << std::endl;

	if ( CUri::makeAbsolute(refUri, testUri4, relUri) )
		std::cout << refUri.toString() << std::endl;
	else
		std::cout << "Error in uri" << std::endl;

	if ( CUri::makeAbsolute(refUri, testUri4, relUri2) )
		std::cout << refUri.toString() << std::endl;
	else
		std::cout << "Error in uri" << std::endl;

	if ( CUri::makeAbsolute(refUri, testUri4, relUri3) )
		std::cout << refUri.toString() << std::endl;
	else
		std::cout << "Error in uri" << std::endl;


	std::cout << "RFC 3986" << std::endl;
	std::cout << "Examples of Resolving Relative URI References" << std::endl;

	std::cout << "Within an object with a well-defined base URI of" << std::endl;

	CUri baseUri("http://a/b/c/d;p?q");
	if ( baseUri.isValid() ) {
		std::cout << baseUri.toString().c_str() << std::endl;
	} else {
		std::cout << "Base URI invalid" << std::endl;
		return;
	}

	std::cout << "The relative URI would be resolved as follows:" << std::endl;

	std::cout << "Normal Examples" << std::endl;

	const char *normalExamples[] = {
		"g:h",
		"g",
		"./g",
		"g/",
		"/g",
		"//g",
		"?y",
		"g?y",
		"#s",
		"g#s",
		"g?y#s",
		";x",
		"g;x",
		"g;x?y#s",
		"",
		".",
		"./",
		"..",
		"../",
		"../g",
		"../..",
		"../../",
		"../../g",
		0
	};
	testURI(baseUri, normalExamples);

	std::cout << "Abnormal Examples" << std::endl;

	const char *abnormalExamples[] = {
		"../../../g",
		"../../../../g",

		"/./g",
		"/../g",
		"g.",
		".g",
		"g..",
		"..g",

		"./../g",
		"./g/.",
		"g/./h",
		"g/../h",
		"g;x=1/./y",
		"g;x=1/../y",

		"g?y/./x",
		"g?y/../x",
		"g#s/./x",
		"g#s/../x",

		"http:g",
		0
	};
	testURI(baseUri, abnormalExamples);

	const char *abnormalExamples2[] = {
		"http:g",
		0
	};
	testURI(baseUri, abnormalExamples2, false);

	std::cout << "IPv6 Examples" << std::endl;

	const char *ipV6Examples[] = {
		"hTTp://[1080:0:0:0:8:800:200C:417A]:8080/",
		"http://[1080::8:800:200C:417A]:80/",
		"http://[::13.1.68.3]:/",
		"http://[0:0:0:0:0:FFFF:129.144.52.38]/",
		"http://[::FFFF:129.144.52.38]",
		"http://[v7.0:0:0:0:0:FFFF:129.144.52.38]/%20Test",
		"http:",
		0
	};
	testURI(baseUri, ipV6Examples);


	std::cout << "Faulty Examples" << std::endl;

	const char *faultyExamples[] = {
		"error://[1080:0:0:0:8:800:200C]/",
		"error://[1080:0:0:0:8:800:200C:417A:0]:8080/",
		"error://[1080:0:0:0:8:800:200C:417A]:8AB/",
		"error://124.2.2.1er:8080/",
		"error://124.2.2.1/%",
		0
	};
	testURI(baseUri, faultyExamples);
}

/** @brief Interface test program
 */
int main (int argc, char * const argv[]) {
	std::cout << "Hello, World!" << std::endl;


	CBackBufferRegistry globalFactory;
	TemplBuffer<> test(1024);
	testURI();
	testStream(globalFactory);

	CRiCPPBridge ri;

	std::string str;
	std::cout << CEnv::tmpName() << ": " << CEnv::find(str, CEnv::tmpName()) << std::endl;
	std::cout << CEnv::homeName() << ": " << CEnv::find(str, CEnv::homeName()) << std::endl;
	std::cout << CEnv::pathName() << ": "  << CEnv::find(str, CEnv::pathName()) << std::endl;
	std::cout << CEnv::progDirName() << ": "  << CEnv::find(str, CEnv::progDirName()) << std::endl;

	std::string tmp(CEnv::tmpName());
	tmp = "Z:\\test.gz";
	gzFile f = gzopen(tmp.c_str(), "w");
	if ( f ) {
		gzwrite(f, "Teste", 4);
		gzclose(f);
	}

#ifdef WIN32
	CStringList testpath;
	testpath.explode(':', "$HOME:C:/testdir:C:\\Anothertest:C|/test;.:$PROGDIR:$PATH", true, true, true);

	std::string teststr;
	testpath.implode(':', teststr, true);
	testpath.clear();
	testpath.explode(':', teststr.c_str(), false, false, true);
#endif

	// Print error, does not abort
	ri.errorHandler(ri.errorPrint());

	// Test to load a rib filter
	ri.option("searchpath", "ribfilter", "@:$PATH", RI_NULL);
	ri.addFrontRibFilter("passthrough");

	ri.begin("test.rib");
	RtContextHandle ch1 = ri.getContext();
	ri.end();

	RtContextHandle ch2 = ri.begin("ribwriter", RI_FILE, "$HOME/test.rib", RI_NULL);
		ri.begin("test2.rib");
			ri.declare("surface:tile", "    constant    float "); // syntax error
			ri.declare("surface:tile:anarray", "    constant    float  [19] ");
			ri.declare("bla", "fdfw[19]ss"); // Error
			RtContextHandle ch3 = ri.getContext();
			ri.context(illContextHandle);
			ri.clipping(0, 1); /* Error */
			// Swapping contexts
			ri.context(ch2);
		ri.end();
		ri.context(ch3);
		ri.frameBegin(1);
		ri.frameEnd();
		ri.frameEnd(); /** Error **/
	ri.end();

	ri.frameEnd(); /* Error */
	ri.end(); /* Error */
	ri.context(ch1); /* Error */

	/* not existent */
	ri.begin("bla");
	ri.worldBegin();
	ri.worldEnd();
	ri.end();

	std::string name;
	name.reserve(200);
	for ( int i = 1; i < 100; ++i ) {
		ri.begin(RI_NULL);
			name = "name";
			for ( int j = 1; j < 100; ++j ) {
				ri.declare(name.c_str(), "    constant    string  [19] ");
				name += ".";
			}
			ri.frameBegin((RtInt)i);
				ri.worldBegin();
				ri.worldEnd();
			ri.frameEnd();
		ri.end();
	}
	name.clear();
	name.resize(0);

	// No abort context on error
	ri.errorHandler(ri.errorAbort());

	/* wrong lib */
	ri.begin("user32");
	ri.worldBegin();
	ri.worldEnd();
	ri.end();

	for ( int i = 1; i < 1000; ++i ) {
		ri.begin(RI_NULL);
		ri.end();
	}

	ri.option("searchpath", "renderer", "&:$PATH", RI_NULL);
	ri.begin("ribwriter");
	ri.end();

	std::cout << "Good bye, World!" << std::endl;

    return 0;
}
