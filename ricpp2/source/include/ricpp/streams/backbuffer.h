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
 *  @brief Headerfile with templates for zlib-enabled dtreams and base classes for plugable streambuffer facets.
 * @todo Much to do here: Random access, stream buffering,
 * more channels (HTTP:, FTP:, SMB:, ...), test routines, better interface, i/o stream objects for the TemplFrontStreambuf,
 * CR/LF LF/CR LF if file is zipped and not binary, exceptions, more documentation.
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

#ifndef _RICPP_TOOLS_TEMPLATEFUNCS_H
#include "ricpp/tools/templatefuncs.h"
#endif // _RICPP_TOOLS_TEMPLATEFUNCS_H

#include "zlib.h"

#include <fstream>

namespace RiCPP {

/** @brief Root for the backbuffer facet
 *
 * Used by TemplFrontStreamBuf as simple back end streambuf (block read/write) plug-in. Can be overwritten
 * for various channels. CFileBackBuffer is the backend for files. A specialiced factory inheriting
 * from CBackBufferFactory is needed to create those back ends,. e.g. CFileBackBufferFactory is
 * the factory for CFileBackBuffer objects. CBackBufferRoot will not be instanciated.
 */
class CBackBufferRoot {
protected:
	CUri m_lastFileName; ///< URI of the associated resource.
public:
	/** @brief Destructor
	 *
	 *  Makes the destructor virtual. Derived objects may free their
	 *  resources here.
	 */
	inline virtual ~CBackBufferRoot() {}
	
	/** @brief Closes the resource.
	 */
	inline virtual void close() { }
	
	/** @brief Opens a resource.
	 *
	 *  Reused the TypeOpenMode of standard streams to indicate the open mode of the resource.
	 *  This base function only stores the @a absURI in m_lastFileName. And should be called
	 *  from derived element functions.
	 *
	 *  @param absUri The absolute URi of the resource to open.
	 *  @param mode The mode used to open the resource.
	 *  @return Return false if resource couldn't be opened, true if succeeded.
	 */
	inline virtual bool open(
		const CUri &absUri,
		TypeOpenMode mode = std::ios_base::in|std::ios_base::binary)
	{
		m_lastFileName = absUri;
		return true;
	}
	
	/** @brief Gets the open state.
	 *
	 *  @return true, if the resource is open.
	 */
	inline virtual bool isOpen() const { return true; }

	/** @brief Reads a chunk of data.
	 *
	 *  @param b Points to the location where the data will be stored.
	 *  @param size Maximal number of bytes that can be storaed at *b.
	 *  @return Number of bytes read.
	 */
	inline virtual std::streamsize sgetn(char *b, std::streamsize size) { return 0; }

	/** @brief Writes a chunk of data.
	 *
	 *  @param b Points to the location where the data is stored.
	 *  @param size Number of bytes that hould be written.
	 *  @return Number of bytes written.
	 */
	inline virtual std::streamsize sputn(const char *b, std::streamsize size) { return 0; }
	
	/** @brief Gets the URI (read-only) of the associated resource.
	 *
	 *  @return The URI (read-only) of the associated resource.
	 */
	inline virtual const CUri &lastFileName() const { return m_lastFileName; }
}; // CBackBufferRoot


/** @brief Back end buffer for file i/o.
 *
 *  @see CBackBufferRoot
 */
class CFileBackBuffer : public CBackBufferRoot {
	std::filebuf m_filebuf; ///< Associated file buffer.
	TypeOpenMode m_mode;    ///< Open mode
public:
    /** @brief Standard constructor.
	 *
	 *  Just clears m_mode.
	 */
	inline CFileBackBuffer()
	{
		m_mode = (TypeOpenMode)0;
	}

	/** @brief Destructor
	 *
	 *  Closes the file.
	 */
	inline virtual ~CFileBackBuffer()
	{
		close();
	}

	/** @brief Closes a file buffer.
	 */
	inline virtual void close()
	{
		if ( m_filebuf.is_open() ) {
			m_filebuf.close();
		}
	}

	/** @brief Opens a file buffer.
	 *
	 *  @param absUri The absolute URi of the file resource to open.
	 *  @param mode The mode used to open the resource.
	 *  @return Return false if resource couldn't be opened, true if succeeded.
	 */
	inline virtual bool open(
		const CUri &absUri,
		TypeOpenMode mode = std::ios_base::in|std::ios_base::binary)
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

	/** @brief Query if the file buffer is open.
	 *
	 *  @return true, the file buffer is open.
	 */
	inline virtual bool isOpen() const
	{
		return m_filebuf.is_open();
	}

	/** @brief Reads bytes from the opened stream.
	 *
	 *  Reads at most @a size bytes from the associated stream and stores them
	 *  in @a b using sgetn() of std::filebuf.
	 *
	 *  @return Number of bytes read.
	 */
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

	/** @brief Writes bytes to the opened stream.
	 *
	 *  Writes @a size bytes of buffer @a b to the associated stream
	 *  using sputn() of std::filebuf.
	 *
	 *  @return Number of bytes read.
	 */
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


/** @brief Base class for the factory classes of specialiced CBackBufferRoot objects.
 *
 *  If the factory is deleted, also all back buffers created by the factory will be deleted.
 *  Factories can be implemented as plugin, to be loaded at runtime. 
 *
 * @see CBackBufferRoot
 */
class CBackBufferFactory : public IPlugin {

	CStringList m_schemes; ///< List of Scehmes (like "FILE" in CFileBackBufferFactory).
	TemplObjPtrRegistry<CBackBufferRoot *, CBackBufferRoot *> m_myBuffers; ///< Buffers created by a factory.

protected:
	/** @brief Adds a new scheme handled by the factory.
	 *
	 *  @param scheme Scheme (like "FILE") handled by the factory.
	 */
	inline void addScheme(const char *scheme)
	{
		std::string str_scheme(noNullStr(scheme));
		if ( str_scheme.empty() )
			return;
		std::for_each(str_scheme.begin(), str_scheme.end(), std::ptr_fun(toupper));
		m_schemes.push(str_scheme, false, false, false);
	}

	/** @brief Registers a newly created back buffer.
	 *
	 *  member function is called by factories to register newly created buffers.
	 *
	 *  @param b Newly created back buffer.
	 */
	inline bool registerObj(CBackBufferRoot *b)
	{
		if ( !b )
			return false;
		return m_myBuffers.registerObj(b, b);
	}

	/** @brief Unregisters (and deletes) back buffer.
	 *
	 *  Buffer @a b should be created by this factory.
	 *
	 *  @param b back buffer.
	 */
	inline bool unRegisterObj(CBackBufferRoot *b)
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

	/** @brief Gets the classes plugin name (backbufferfactory).
	 *
	 *  @return The the classes plugin name.
	 */
	static const char *myName();
	
	/** @brief Gets the classes plugin type (also backbufferfactory).
	 *
	 *  @return The plugin type.
	 */
	static const char *myType();

	/** @brief Major version of the plugin class.
	 *
	 *  @return The major version of the plugin class.
	 */
	static unsigned long myMajorVersion();

	/** @brief Minor version of the plugin class.
	 *
	 *  @return The minor version of the plugin class.
	 */
	static unsigned long myMinorVersion();

	/** @brief Revision number of the plugin class.
	 *
	 *  @return The revision number version of the plugin class.
	 */
	static unsigned long myRevision();

	/** @brief Standard constructor
	 *
	 *  The registry @a m_myBuffers also manages object deletion of the buffers.
	 */
	inline CBackBufferFactory() : m_myBuffers(true) {}
	
	/** @brief Destructor
	 *
	 *  Registered buffer objects will also be deleted.
	 */
	inline virtual ~CBackBufferFactory() {}

	/** @brief Gets the object's plugin name.
	 *
	 *  @return The object's plugin name.
	 */
	inline virtual const char *name() const { return myName(); }

	/** @brief Gets the object's plugin type.
	 *
	 *  @return The object's plugin type.
	 */
	inline virtual const char *type() const { return myType(); }
	
	/** @brief Gets the object's major version number.
	 *
	 *  @return The object's major version number.
	 */
	inline virtual unsigned long majorVersion() const {return myMajorVersion(); }
	
	/** @brief Gets the object's minor version number.
	 *
	 *  @return The object's minor version number.
	 */
	inline virtual unsigned long minorVersion() const {return myMinorVersion(); }

	/** @brief Gets the object's revision number.
	 *
	 *  @return The object's revision number.
	 */
	inline virtual unsigned long revision() const { return myRevision(); }

	inline virtual void startup() {}
	inline virtual void shutdown() {}

	/** @brief Tests if a specific scheme is supported by this factory.
	 *
	 *  Testing is case insensitive.
	 *
	 *  @param scheme Name of a scheme (e.g. "FILE")
	 *  @return true, scheme is supported.
	 */
	inline virtual bool acceptsScheme(const char *scheme) const
	{
		std::string str_scheme(noNullStr(scheme));
		std::for_each(str_scheme.begin(), str_scheme.end(), std::ptr_fun(asciiToUpper));
		return m_schemes.isMember(str_scheme.c_str());
	}

	/** @brief Gets the const iterator of supported schemes.
	 *
	 *  @return The const iterator of supported schemes.
	 */
	inline virtual const_iterator begin() const { return m_schemes.begin(); }

	/** @brief Gets the end iterator of supported schemes.
	 *
	 *  @return The end iterator of supported schemes.
	 */
	inline virtual const_iterator end() const { return m_schemes.end(); }

	/** @brief Gets the number of supported schemes.
	 *
	 * @return Rhe number of supported schemes.
	 */
	inline virtual size_type size() const { return m_schemes.size(); }

	/** @brief Open a new back buffer object.
	 *
	 *  Overwrite this to create and open a new back buffer.
	 *  @param absUri Absolute URI of the resource to open.
	 *  @param mode Mode used to open the resource.
	 *  @return A new, opened back buffer object.
	 */
	inline virtual CBackBufferRoot *open(const CUri &absUri, TypeOpenMode mode = std::ios_base::in|std::ios_base::binary)
	{
		return 0;
	}

	/** @brief Closes and deletes the back buffer object.
	 *
	 *  Need not to be overwritten. The back buffer @p bbr has to be created by the same factory.
	 *  @param bbr A back buffer to close and delete
	 *  @return true, back buffer is deleted.
	 */
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


/** @brief Base class for the factory classes of CFileBackBuffer objects
 * @see CBackBufferRoot
 */
class CFileBackBufferFactory : public CBackBufferFactory {
public:
	/** @brief Gets the classes plugin name (file_backbuffer).
	 *
	 *  @return The the classes plugin name.
	 */
	static const char *myName();
	
	/** @brief Gets the classes plugin type (backbufferfactory).
	 *
	 *  @return The plugin type.
	 */
	static const char *myType();

	/** @brief Major version of the plugin class.
	 *
	 *  @return The major version of the plugin class.
	 */
	static unsigned long myMajorVersion();

	/** @brief Minor version of the plugin class.
	 *
	 *  @return The minor version of the plugin class.
	 */
	static unsigned long myMinorVersion();

	/** @brief Revision number of the plugin class.
	 *
	 *  @return The revision number version of the plugin class.
	 */
	static unsigned long myRevision();

	/** @brief Default constructor
	 *
	 *  Supports the "FILE" schem for local files only.
	 */
	inline CFileBackBufferFactory() { addScheme("FILE"); }
	inline virtual ~CFileBackBufferFactory() {}

	inline virtual const char *type() const { return myType(); }
	inline virtual const char *name() const { return myName(); }
	inline virtual unsigned long majorVersion() const { return myMajorVersion(); }
	inline virtual unsigned long minorVersion() const { return myMinorVersion(); }
	inline virtual unsigned long revision() const { return myRevision(); }

	inline virtual void startup() {}
	inline virtual void shutdown() {}

	/** @brief Open a new file back buffer object.
	 *
	 *  Overwrite this to create and open a new back buffer.
	 *  @param absUri Absolute URI of the file resource to open.
	 *  @param mode Mode used to open the file resource.
	 *  @return A new, opened file back buffer object.
	 */
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


/** @brief Registration for back buffer factories.
 *
 * It's not a singleton, because every RiCPP front end has one. Can
 * load all ".buffer" files (dynamic libraries) containing buffer factories
 * from a directory.
 *
 * @see CBackBufferRoot
 *
 * @todo Implement handlers.
 */
class CBackBufferProtocolHandlers {
	std::string m_direct; ///< @brief Path of the directory used to load buffer factories.
	bool m_hasDirect; ///< @brief @c m_direct has been set.

	/** @brief Plugin-loader for the factories.
	 */
	TemplPluginHandler<CBackBufferFactory> m_backBufferPluginHandler;

	/** @brief Factory for file buffers is immanent.
	 */
	TemplPluginFactory<CFileBackBufferFactory> m_fileBuffer;
	
	/** @brief Sets a directory, registers the file buffer factory and loads factories from the directory.
	 *  @param direct Name of the directory containing the ".buffer" buffer factory files.
	 */
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

public:
	/** @brief Constructor, sets a directory and initializes the object.
	 *  @param direct Name of the directory to load from.
	 */
	inline CBackBufferProtocolHandlers(const char *direct=0) {init(direct);}

	/** @brief Virtual destructor.
	 *
	 *  The loaded libraries are destructed automatically.
	 *
	 */
	inline ~CBackBufferProtocolHandlers() {}


	/** @brief Gets a buffer factory for a specific protocol scheme.
	 *
	 *  At the moment only FILE: is supported.
	 *
	 *  @param scheme Protocol name
	 *  @return Buffer factory for the protocol @a scheme
	 */
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

	/** @brief Gets the plugin loader object
	 *  @return A reference to the plugin loader (loader of buffer factories).
	 */
	inline TemplPluginHandler<CBackBufferFactory> &getHandler()
	{
		return m_backBufferPluginHandler;
	}
}; // CBackBufferProtocolHandlers


/** @brief Template for zlib stream buffers.
 *
 * Can be used by istream, ostream as streambuf. A coupled back end buffer CBackBufferRoot or a
 * basic_streambuf can be used as data source/drain. TemplFrontStreambuf adds zlib in between the
 * stream and the buffer. By using open(), back end buffers for various channels can be used. However,
 * at the moment only FILE: is supported. open() uses a generic URI as resource name/locator.
 *
 * I used copied code from the zlib (gzio.c) here.
 *
 * @todo Debug, there is a bug with m_additionalChars while filling the buffer. Using of basic_streambuf won't work.
 *
 * @see CBackBufferRoot
 */
template<class charT, class traits=std::char_traits<charT> >
class TemplFrontStreambuf : public std::basic_streambuf<charT, traits> {

public:
    typedef typename std::basic_streambuf<charT, traits>::int_type int_type;
    typedef typename std::basic_streambuf<charT, traits>::pos_type pos_type;
    typedef typename std::basic_streambuf<charT, traits>::off_type off_type;

	typedef typename std::basic_streambuf<charT, traits> TypeParent;
	  
private:
	static const int gz_magic_0 = 0x1f;
	static const int gz_magic_1 = 0x8b; ///< @brief gzip magic header

	/* gzip flag byte */
	static int const ASCII_FLAG   = 0x01; ///< @brief bit 0 set: file probably ascii text
	static int const HEAD_CRC     = 0x02; ///< @brief bit 1 set: header CRC present
	static int const EXTRA_FIELD  = 0x04; ///< @brief bit 2 set: extra field present
	static int const ORIG_NAME    = 0x08; ///< @brief bit 3 set: original file name present
	static int const COMMENT      = 0x10; ///< @brief bit 4 set: file comment present
	static int const RESERVED     = 0xE0; ///< @brief bits 5..7: reserved

	// RiCPP will be only implemented for WIN32, MACOS or Unix
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
	CBackBufferProtocolHandlers *m_bufferReg;

	TypeOpenMode m_mode;
	CUri m_baseUri;
	CUri m_resolutionUri;

	int_type m_buffersize;
	int_type m_putbackSize;
	int_type m_additionalChars;

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
		m_additionalChars = 0; // = 8; // Bug if > 0
		if ( m_buffersize < 16 ) {
			m_additionalChars += 16;
		}

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
		char c;

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
			m_transferInBuffer.resize(m_buffersize*sizeof(charT)+m_additionalChars);
			m_strmIn.next_in = m_transferInBuffer.begin()+m_additionalChars;
			startstream = true;
		} else if ( m_strmIn.next_in < m_transferInBuffer.begin()+(m_transferInBuffer.size()-m_additionalChars) ) {
			m_inIsEOF = true;
			return 0;
		} else {
			TemplBuffer<unsigned char>::iterator e = m_transferInBuffer.begin()+(m_transferInBuffer.size()-m_additionalChars);
			TemplBuffer<unsigned char>::iterator b = m_transferInBuffer.begin();

			for ( int i=0; i<m_additionalChars; ++i ) {
				*(b++) = *(e++);
			}
			m_strmIn.next_in = m_transferInBuffer.begin();
		}

		if ( m_backBuffer ) {
			m_strmIn.avail_in =
				(uInt)m_backBuffer->sgetn(
					(char *)m_transferInBuffer.begin()+m_additionalChars,
					(std::streamsize)m_transferInBuffer.size()-m_additionalChars);
		} else if ( m_coupledBuffer ) {
			#ifdef _MSC_VER
				m_strmIn.avail_in =
					(uInt)m_coupledBuffer->_Sgetn_s(
						(char *)m_transferInBuffer.begin()+m_additionalChars,
						m_transferInBuffer.size()-m_additionalChars,
						(std::streamsize)m_transferInBuffer.size()-m_additionalChars);
			#else
				m_strmIn.avail_in =
					m_coupledBuffer->sgetn(
						(char *)m_transferInBuffer.begin()+m_additionalChars,
						m_transferInBuffer.size()-m_additionalChars);
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
					(m_mode  & std::ios_base::binary) ? 0 : ASCII_FLAG, // flags
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

		m_strmIn.avail_out = (uInt)(sizeof(charT)*(m_frontInBuffer.size()-m_putbackSize));
		m_strmIn.next_out = (Bytef *)(m_frontInBuffer.begin()+m_putbackSize);
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

		num = (std::streamsize)(
			sizeof(charT) *
			(m_frontInBuffer.size()-m_putbackSize) -
			m_strmIn.avail_out);

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
		m_baseUri.encodeFilepath(path.c_str(), "file");
	}

public:
	inline TemplFrontStreambuf(CBackBufferProtocolHandlers &bufferReg) :
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

		if ( !CUri::makeAbsolute(m_resolutionUri, m_baseUri, refUri, false) )
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
		if ( m_coupledBuffer ) {
			disconnect();
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
