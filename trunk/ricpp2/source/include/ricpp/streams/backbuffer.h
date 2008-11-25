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
 *  @brief Headerfile with templates for zlib-enabled streams and base classes for plugable streambuffer facets.
 * @todo Much to do here: Random access, stream buffering,
 * more channels (HTTP:, FTP:, SMB:, ...), test routines, better interface, i/o stream objects for the TemplFrontStreambuf,
 * CR/LF LF/CR LF if file is zipped and not binary, exceptions, more documentation... or use BOOST/iostreams.
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
	 *  Used by TemplFrontStreamBuf as simple backend streambuf (block read/write) plug-in. Can be overwritten
	 *  for various channels, e.g. CFileBackBuffer is the backend for files. A specialiced factory inheriting
	 *  from CBackBufferFactory is needed to create those backends,. e.g. CFileBackBufferFactory is
	 *  the factory for CFileBackBuffer objects. CBackBufferRoot will not be instanciated.
	 */
	class CBackBufferRoot {
	protected:
		CUri m_lastFileName; ///< URI of the associated resource.
	public:
		/** @brief Constructor
		 */
		inline CBackBufferRoot() { }

		/** @brief Destructor
		 *
		 *  Makes the destructor virtual. Derived objects may free their
		 *  resources here.
		 */
		inline virtual ~CBackBufferRoot() { }
		
		/** @brief Closes the resource.
		 */
		virtual void close() = 0;
		
		/** @brief Opens a resource.
		 *
		 *  Reused the TypeOpenMode of standard streams to indicate the open mode of the resource.
		 *  This base function only stores the @a absURI in m_lastFileName. And should be called
		 *  from the derived element functions.
		 *
		 *  @param absUri The absolute URI of the resource to open.
		 *  @param mode   The mode used to open the resource.
		 *  @return       false, if the resource couldn't be opened, true if succeeded.
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
		virtual bool isOpen() const = 0;

		/** @brief Reads a chunk of data.
		 *
		 *  @param  b Points to the location where the data will be stored.
		 *  @param  size Maximal number of bytes that can be stored at *b.
		 *  @return Number of bytes read.
		 */
		virtual std::streamsize sgetn(char *b, std::streamsize size) = 0;

		/** @brief Writes a chunk of data.
		 *
		 *  @param b Points to the location where the data is stored.
		 *  @param size Number of bytes that should be written.
		 *  @return Number of bytes written.
		 */
		virtual std::streamsize sputn(const char *b, std::streamsize size) = 0;
		
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
		 *  Closes the file buffer.
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
		 *  @param absUri The absolute URI of the file resource to open.
		 *  @param mode The mode used to open the resource.
		 *  @return false, if resource couldn't be opened, true if succeeded.
		 */
		virtual bool open(
			const CUri &absUri,
			TypeOpenMode mode = std::ios_base::in|std::ios_base::binary);

		/** @brief Query whether the file buffer is open.
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
		 *  @param  b Points to the location where the data will be stored.
		 *  @param  size Maximal number of bytes that can be stored at *b.
		 *  @return Number of bytes read.
		 */
		virtual std::streamsize sgetn(char *b, std::streamsize size);

		/** @brief Writes bytes to the opened stream.
		 *
		 *  Writes @a size bytes of buffer @a b to the associated stream
		 *  using sputn() of std::filebuf.
		 *
		 *  @return Number of bytes read.
		 */
		inline virtual std::streamsize sputn(const char *b, std::streamsize size);
	}; // CFileBackBuffer


	/** @brief Base class for the factory classes of specialiced CBackBufferRoot objects.
	 *
	 *  If the factory is deleted, also all back buffers created by the factory will be deleted.
	 *  Factories can be implemented as plugin, to be loaded at runtime. 
	 *
	 * @see CBackBufferRoot
	 */
	class CBackBufferFactory : public IPlugin {

		CStringList m_schemes; ///< List of Schemes (like "FILE" in CFileBackBufferFactory).
		TemplObjPtrRegistry<CBackBufferRoot *, CBackBufferRoot *> m_myBuffers; ///< Buffers created by a factory, indexed by its own pointer.

	protected:
		/** @brief Adds a new scheme handled by the factory.
		 *
		 *  @param scheme Scheme (like "FILE") handled by the factory.
		 */
		void addScheme(const char *scheme);

		/** @brief Registers a newly created back buffer.
		 *
		 *  Member function is called by factories to register newly created buffers.
		 *
		 *  @param b Newly created back buffer (b is handle and pointer)
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

		/** @brief Gets the classes plugin name ("backbufferfactory").
		 *
		 *  @return The the classes plugin name.
		 */
		static const char *myName();
		
		/** @brief Gets the classes plugin type (also "backbufferfactory").
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
		virtual bool acceptsScheme(const char *scheme) const;

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

		/** @brief Opens a new back buffer object.
		 *
		 *  Overwrite this to create and open a new back buffer.
		 *
		 *  @param absUri Absolute URI of the resource to open.
		 *  @param mode Mode used to open the resource.
		 *  @return A new, opened back buffer object.
		 */
		inline virtual CBackBufferRoot *open(const CUri &absUri, TypeOpenMode mode = std::ios_base::in|std::ios_base::binary) { return 0; }

		/** @brief Closes and deletes the back buffer object.
		 *
		 *  Need not to be overwritten. The back buffer @p bbr has to be created by the same factory.
		 *
		 *  @param bbr A back buffer to close and delete
		 *  @return true, back buffer is deleted.
		 */
		virtual bool close(CBackBufferRoot *bbr);
	}; // CBackBufferFactory


	/** @brief Base class for the factory classes of CFileBackBuffer objects
	 *  @see CBackBufferRoot
	 */
	class CFileBackBufferFactory : public CBackBufferFactory {
	public:
		/** @brief Gets the classes plugin name ("file_backbuffer").
		 *
		 *  @return The the classes plugin name.
		 */
		static const char *myName();
		
		/** @brief Gets the classes plugin type ("backbufferfactory").
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
		 *  Supports the "FILE" scheme for local files only.
		 */
		inline CFileBackBufferFactory() { addScheme("FILE"); }

		/** @brief Destructor, nothing to do
		 */
		inline virtual ~CFileBackBufferFactory() {}

		inline virtual const char *type() const { return myType(); }
		inline virtual const char *name() const { return myName(); }
		inline virtual unsigned long majorVersion() const { return myMajorVersion(); }
		inline virtual unsigned long minorVersion() const { return myMinorVersion(); }
		inline virtual unsigned long revision() const { return myRevision(); }

		inline virtual void startup() {}
		inline virtual void shutdown() {}

		/** @brief Opens a new file back buffer object.
		 *
		 *  @param absUri Absolute URI of the file resource to open.
		 *  @param mode Mode used to open the file resource.
		 *  @return A new, opened file back buffer object.
		 *  @see CFileBackBuffer
		 */
		virtual CBackBufferRoot *open(
			const CUri &absUri,
			TypeOpenMode mode = std::ios_base::in|std::ios_base::binary);
	}; // CFileBackBufferFactory


	/** @brief Registration for back buffer factories.
	 *
	 * It's not a singleton, because every RiCPP frontend has its own. Can
	 * load all ".buffer" files (dynamic libraries) containing buffer factories
	 * from a directory.
	 *
	 * @see CBackBufferRoot
	 *
	 * @todo Implement more handlers.
	 */
	class CBackBufferProtocolHandlers {
		std::string m_direct; ///< @brief Path of the directory used to load buffer factories.
		bool m_hasDirect;     ///< @brief @c m_direct has been set.

		/** @brief Plugin-loader for the factories.
		 */
		TemplPluginHandler<CBackBufferFactory> m_backBufferPluginHandler;

		/** @brief Factory for file buffers is immanent.
		 */
		TemplPluginFactory<CFileBackBufferFactory> m_fileBuffer;
		
		/** @brief Sets a directory, registers the file buffer factory and loads factories from the directory.
		 *  @param direct Name of the directory containing the ".buffer" buffer factory files.
		 */
		void init(const char *direct);

	public:
		/** @brief Constructor, sets a directory and initializes the object.
		 *  @param direct Name of the directory to load from.
		 */
		inline CBackBufferProtocolHandlers(const char *direct=0) { init(direct); }

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
		CBackBufferFactory *getBufferFactory(const char *scheme);

		/** @brief Gets the plugin loader object
		 *  @return A reference to the plugin loader (loader of buffer factories).
		 */
		inline TemplPluginHandler<CBackBufferFactory> &getHandler()
		{
			return m_backBufferPluginHandler;
		}
	}; // CBackBufferProtocolHandlers


	/** @brief Class for zlib stream buffers.
	 *
	 * Can be used by istream, ostream as streambuf. A coupled back end buffer CBackBufferRoot or a
	 * basic_streambuf can be used as data source/drain. TemplFrontStreambuf adds zlib in between the
	 * stream and the buffer. By using open(), backend buffers for various channels can be used.
	 * However, at the moment only FILE: is supported. open() uses a generic URI as 
	 * resource name/locator.
	 *
	 * I used copied code from the zlib (gzio.c) here to support zipped streams.
	 *
	 * @todo Debug, using of basic_streambuf as m_coupledBuffer doesn't work at the moment.
	 *
	 * @see CBackBufferRoot
	 */
	class CFrontStreambuf : public std::basic_streambuf<char, std::char_traits<char> > {

	public:
		typedef std::basic_streambuf<char, std::char_traits<char> > TypeParent;
		typedef TypeParent::int_type int_type;
		typedef TypeParent::pos_type pos_type;
		typedef TypeParent::off_type off_type;

		  
	private:
		static const int gz_magic_0 = 0x1f; ///< @brief gzip magic header first byte
		static const int gz_magic_1 = 0x8b; ///< @brief gzip magic header second byte

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

		TypeOpenMode m_mode; ///< @brief Open mode of the ressource.
		CUri m_baseUri; ///< @brief URI used as basis for relative URIs used for open().
		CUri m_resolutionUri; ///< @brief Resolved URI of the current resource usin @i m_baseUri.

		int_type m_buffersize; ///< @brief Buffer size incl. m_putbackSize
		int_type m_putbackSize; ///< @brief Additional chars for put back.

		TemplBuffer<char> m_frontInBuffer;
		z_stream m_strmIn;
		TemplBuffer<unsigned char> m_transferInBuffer;
		bool m_inIsEOF;
		long m_in;
		long m_crcIn;
		int m_transparentIn; ///< @brief Read input without unpacking zipped content.
		int m_strategyIn;
		char m_methodIn;

		TemplBuffer<char> m_frontOutBuffer;
		z_stream m_strmOut;
		TemplBuffer<char> m_transferOutBuffer;
		long m_out;
		long m_crcOut;
		int m_compressLevelOut;
		int m_strategyOut;
		char m_methodOut;

		/** @brief Initialize the front buffers
		 */
		void init();

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

		bool check_header();
		unsigned int fill_in_buffer();
		
		inline CFrontStreambuf(CFrontStreambuf &) {}
		inline CFrontStreambuf() {}
		
	protected:
		
		inline virtual int flushBuffer(bool finish);
		inline virtual int_type overflow(int_type c);
		inline virtual int_type underflow();

		/** @brief Set m_baseUri to the file path current directory
		 */
		inline virtual void setBaseCwd()
		{
			CFilepath p;
			std::string path(p.filepath());
			path += CFilepathConverter::internalPathSeparator();
			m_baseUri.encodeFilepath(path.c_str(), "file");
		}

		/** @brief Initializes buffer for input and output
		 *
		 *  @param mode Open mode (problem. zipped will be binary, but unzipped content isn't necessarily binary)
		 *  @param compressLevel minly for for output. But if the compressLevel is Z_NO_COMPRESSION, input will be read as is, zlib header will not interpreted.
		 */
		bool postOpen(TypeOpenMode mode = std::ios_base::in|std::ios_base::binary,
					  int compressLevel = Z_DEFAULT_COMPRESSION);
		
	public:
		/** @brief Initializes the buffer.
		 *  @param bufferReg Object is used to get a buffer for a specfic URI-scheme
		 */
		inline CFrontStreambuf(CBackBufferProtocolHandlers &bufferReg) :
			m_bufferReg(&bufferReg)
		{
			init();
		}

		/** @brief Destructor, colses the stream.
		 */
		inline virtual ~CFrontStreambuf() throw() { close(); }

		/** @brief Sets the base URI.
		 *  @param base New base URI
		 *  @returns false, base URI is invalid.
		 */
		inline virtual bool base(const CUri &base)
		{
			m_baseUri = base;
			if ( m_baseUri.isValid() && m_baseUri.toString().empty() ) {
				// No base URI, so use the current directory in file system
				setBaseCwd();
			}
			return m_baseUri.isValid();
		}

		/** @brief Gets the base URI.
		 */
		inline virtual const CUri &base() const
		{
			return m_baseUri;
		}

		/** @brief Opens a buffer for input or output from a reference URI
		 *
		 *  Initializes buffer pointers and back buffer
		 *
		 *  @param refURI Uniform ressource identifier
		 *  @param mode Open mode (problem. zipped will be binary, but unzipped content isn't necessarily binary)
		 *  @param compressLevel minly for for output. But if the compressLevel is Z_NO_COMPRESSION, input will be read as is, zlib header will not interpreted.
		 */
		virtual bool open(
						  const CUri &refUri,
						  TypeOpenMode mode = std::ios_base::in|std::ios_base::binary,
						  int compressLevel = Z_DEFAULT_COMPRESSION);
		
		/** @brief Opens a buffer for input or output from a streambuf
		 *
		 *  Initializes buffer pointers and back buffer
		 *
		 *  @param refURI Uniform ressource identifier
		 *  @param mode Open mode (problem. zipped will be binary, but unzipped content isn't necessarily binary)
		 *  @param compressLevel minly for for output. But if the compressLevel is Z_NO_COMPRESSION, input will be read as is, zlib header will not interpreted.
		 */
		virtual void open(TypeParent *aBuffer,
						  TypeOpenMode mode = std::ios_base::in|std::ios_base::binary,
						  int compressLevel = Z_DEFAULT_COMPRESSION);
		
		/** @brief Closes the buffer.
		 */
		virtual bool close();

		/** @brief Gets a coupled buffer.
		 */
		inline TypeParent *coupledBuffer()
		{
			return m_coupledBuffer;
		}

		/** @brief Cannot rewind the stream.
		 */
		inline virtual bool rewind() const
		{
			return false;
		}
	}; // CFrontStreambuf

} // namespace RiCPP

#endif // _RICPP_STREAMS_BACKBUFFER_H
