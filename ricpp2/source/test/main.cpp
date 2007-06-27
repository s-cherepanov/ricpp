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
#include "ricpp/tools/stringlist.h"
#include "ricpp/streams/uri.h"
#include "ricpp/streams/buffer.h"

#include "zlib.h"

#include <iostream>
#include <fstream>
#include <streambuf>

using namespace RiCPP;

class CBackBufferRoot : public std::basic_streambuf<char> {
protected:
	virtual int_type overflow (int_type c) {
		return  0;
	}
	virtual int_type underflow () {
		return  0;
	}

public:
	inline virtual ~CBackBufferRoot() {}
	virtual void close() { }
	virtual bool open(const CUri &absUri, std::_Ios_Openmode mode = std::ios_base::in|std::ios_base::binary) { return false; }
	inline virtual bool isOpen() const { return false; }
};

class CFileBackBuffer : public CBackBufferRoot {
	std::fstream m_stream;
	TemplBuffer<char> m_backBuffer;
protected:
	virtual int_type overflow (int_type c) {
		return  0;
	}
	virtual int_type underflow () {
		return  0;
	}
public:
	inline CFileBackBuffer()
		: m_backBuffer(8192)
	{
	}
	inline virtual ~CFileBackBuffer()
	{
		close();
	}
	virtual void close()
	{
		if ( m_stream.is_open() )
			m_stream.close();
	}
	inline virtual bool open(const CUri &absUri, std::_Ios_Openmode mode = std::ios_base::in|std::ios_base::binary)
	{
		close();
		std::string filename(absUri.getHierPart());
		CFilepathConverter::convertToNative(filename);
		m_stream.open(filename.c_str(), mode);
		return m_stream.is_open();
	}
	inline virtual bool isOpen() const
	{
		return m_stream.is_open();
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

	inline virtual CBackBufferRoot *open(const CUri &absUri, std::_Ios_Openmode mode = std::ios_base::in|std::ios_base::binary)
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

	inline virtual CBackBufferRoot *open(const CUri &absUri, std::_Ios_Openmode mode = std::ios_base::in|std::ios_base::binary)
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
	TemplPluginHandler<CBackBufferFactory> m_backBufferPluginHandler;
	TemplPluginFactory<CFileBackBufferFactory> m_fileBuffer;
	inline void init() {
		if ( !m_backBufferPluginHandler.isRegistered("filebuffer") ) {
			m_backBufferPluginHandler.registerFactory(
				"file.buffer",
				(TemplPluginFactory<CBackBufferFactory> *)&m_fileBuffer );
			std::string str;
			CEnv::find(str, CEnv::progDirName());
			try {
				m_backBufferPluginHandler.registerFromDirectory(0, ".buffer");
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
	inline CBackBufferRegistry() {init();}
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

class CByteStreambuf : public std::basic_streambuf<char> {
	std::list<TemplBuffer<char> > m_frontBufferList;
	CBackBufferRoot *m_backBuffer;
	CBackBufferFactory *m_factory;

	CUri m_baseUri;
	CUri m_resolutionUri;
	bool m_cache;
	CBackBufferRegistry *m_bufferReg;

	z_stream m_strm;
	bool m_compress;

	void init()
	{
	}

protected:
	virtual int_type overflow (int_type c) {
		return  0;
	}
	virtual int_type underflow () {
		return  0;
	}
	virtual void setBaseCwd() {
		CFilepath p;
		std::string path(p.filepath());
		path += CFilepathConverter::internalPathSeperator();
		m_baseUri.set("file", "", path.c_str(), 0, 0);
	}

public:
	inline CByteStreambuf(CBackBufferRegistry &bufferReg) :
		m_backBuffer(0),
		m_factory(0),
		m_cache(false),
		m_compress(true),
		m_bufferReg(&bufferReg)
	{
		init();
	}

	inline CByteStreambuf(CByteStreambuf &)
	{
	}

	inline virtual ~CByteStreambuf() throw()
	{
		close();
	}

	inline virtual bool setBase(const CUri &base)
	{
		if ( base.isValid() ) {
			m_baseUri = base;
		} else {
			return true;
		}
		return m_baseUri.isValid();
	}

	inline virtual bool open(const CUri &refUri, std::_Ios_Openmode mode = std::ios_base::in|std::ios_base::binary)
	{
		if ( m_factory && m_backBuffer ) {
			m_factory->close(m_backBuffer);
			m_backBuffer = 0;
			m_factory = 0;
		}

		if ( m_baseUri.toString().empty() ) {
			setBaseCwd();
		}

		if ( !CUri::makeAbsolute(m_resolutionUri, m_baseUri, refUri) )
		{
			return false;
		}

		m_factory = m_bufferReg->getBufferFactory(m_resolutionUri.getScheme().c_str());
		if ( m_factory ) {
			m_backBuffer = m_factory->open(m_resolutionUri, mode);
			if ( !m_backBuffer )
				m_factory = 0;
		}
		return m_backBuffer != 0;
	}

	inline virtual bool close()
	{
		if ( m_factory && m_backBuffer ) {
			bool rval = m_factory->close(m_backBuffer);
			m_backBuffer = 0;
			m_factory = 0;
			return rval;
		}
		return false;
	}

	inline virtual void enableCache()
	{
		m_cache = true;
	}

	inline virtual void disableCache()
	{
		m_cache = false;
	}

	inline virtual bool cacheEnabled() const
	{
		return m_cache;
	}

	inline virtual bool rewind() const
	{
		if ( m_cache ) {
			// rewind on my own
		} else {
			// let the backbuffer rewind
		}
		return false;
	}
};

void testStream(CBackBufferRegistry &factory)
{
	CByteStreambuf bi(factory);
	std::istream myistream(&bi);

	bi.open("ReadMe.txt", std::ios_base::in|std::ios_base::binary);
	bi.close();

	CByteStreambuf bo(factory);
	std::string str;
	CEnv::find(str, CEnv::tmpName());
	str += CFilepathConverter::nativePathSeperator();
	CFilepathConverter::convertToInternal(str);

	CUri base("file", "", str.c_str(), 0, 0);
	bo.setBase(base);
	std::ostream myostream(&bo);
	bo.open("ReadMe2.txt", std::ios_base::out|std::ios_base::binary);
	bo.close();
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
