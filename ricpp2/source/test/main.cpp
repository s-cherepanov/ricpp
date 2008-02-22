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

// #include <iostream>

using namespace RiCPP;

/** @brief Create and read a .gz file 
 */
void testStream(CBackBufferProtocolHandlers &factory)
{
	TemplFrontStreambuf<char> ob(factory);
	std::string str;
	CEnv::find(str, CEnv::tmpName(), true);
	str += CFilepathConverter::nativePathSeperator();
	CFilepathConverter::convertToInternal(str);

	CUri base("file", "", str.c_str(), 0, 0);
	ob.base(base);
	ob.open("ReadMe3.txt.gz", std::ios_base::out|std::ios_base::binary, 3);
	// ob.open("ReadMe3.txt", std::ios_base::out|std::ios_base::binary, 0);
	std::ostream myostream(&ob);
	// std::ostream myostream(std::cout.rdbuf());
	if ( myostream ) {
		for ( int i = 1; i < 10; ++i )
			myostream <<
				"Testing\t" <<
				"123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890\t" <<
				18.3 << std::endl;
	}
	ob.close();

	TemplFrontStreambuf<char> ib(factory);
	ib.base(base);
	ib.open("ReadMe3.txt.gz", std::ios_base::in|std::ios_base::binary);
	// ib.open("ReadMe3.txt", std::ios_base::in|std::ios_base::binary);
	std::istream myistream(&ib);
	std::string teststr;
	while ( myistream ) {
		myistream >> teststr;
		if ( myistream )
			std::cout << "'" << teststr  << "'" << std::endl;
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
	CUri testUri3("file:///C|/test");
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
	if ( CUri::makeAbsolute(refUri, testUri, relUri, true) )
		std::cout << refUri.toString() << std::endl;
	else
		std::cout << "Error in uri" << std::endl;

	if ( CUri::makeAbsolute(refUri, testUri2, relUri, true) )
		std::cout << refUri.toString() << std::endl;
	else
		std::cout << "Error in uri" << std::endl;

	if ( CUri::makeAbsolute(refUri, testUri3, relUri, true) )
		std::cout << refUri.toString() << std::endl;
	else
		std::cout << "Error in uri" << std::endl;

	if ( CUri::makeAbsolute(refUri, testUri4, relUri, true) )
		std::cout << refUri.toString() << std::endl;
	else
		std::cout << "Error in uri" << std::endl;

	if ( CUri::makeAbsolute(refUri, testUri4, relUri2, true) )
		std::cout << refUri.toString() << std::endl;
	else
		std::cout << "Error in uri" << std::endl;

	if ( CUri::makeAbsolute(refUri, testUri4, relUri3, true) )
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

/** @brief Test rendering
 * @param ri A renderer 
 */
void testrun(CRiCPPBridge &ri)
{
	RtFloat frommonochr[]={
		static_cast<RtFloat>(.3),
		static_cast<RtFloat>(.3),
		static_cast<RtFloat>(.4)
	};
	RtFloat tomonochr[] = {
		static_cast<RtFloat>(1.0),
		static_cast<RtFloat>(1.0),
		static_cast<RtFloat>(1.0)
	};

	RtFloat id[] = {
		static_cast<RtFloat>(1.0), static_cast<RtFloat>(0.0), static_cast<RtFloat>(0.0),
		static_cast<RtFloat>(0.0), static_cast<RtFloat>(1.0), static_cast<RtFloat>(0.0),
		static_cast<RtFloat>(0.0), static_cast<RtFloat>(0.0), static_cast<RtFloat>(1.0)
	};

	RtFloat floats[] = { static_cast<RtFloat>(1.234) };

	RtFloat c[3] = {static_cast<RtFloat>(1.0), static_cast<RtFloat>(1.0), static_cast<RtFloat>(1.0)};

	RtFloat fov = static_cast<RtFloat>(90.0);

	char *save = "save";
	char *restore = "restore";
	char *all = "all";

	// Print error, does not abort
	ri.errorHandler(ri.errorPrint());

	ri.begin(RI_NULL);
		RtToken myOptionValue = ri.declare("myOptionValue", "float");
		ri.colorSamples(1, frommonochr, tomonochr);
		ri.option("myOption", myOptionValue, floats, RI_NULL);
		ri.projection("perspective", "float[1] fov", &fov, RI_NULL);
		ri.shutter(0.0, 1.0);

		RtObjectHandle myObject = ri.objectBegin();
			ri.attributeBegin();
				ri.sphere(0.5, -1.0, 1.0, 360.0, RI_NULL);
			ri.attributeEnd();
		ri.objectEnd();

		RtArchiveHandle myArchive = ri.archiveBegin("MyArchive", RI_NULL);
			ri.attributeBegin();
				ri.ifBegin("$Frame==1");
					ri.sphere(0.75, -1, 1, 360.0, RI_NULL);
				ri.elseBegin();
					ri.torus(0.5, 0.125, 0.0, 360.0, 360.0, RI_NULL);
				ri.ifEnd();
			ri.attributeEnd();
		ri.archiveEnd();

		ri.frameBegin(1);
			ri.colorSamples(3, id, id);
			ri.worldBegin();
			ri.color(c);
			ri.surface(RI_MATTE, RI_NULL);
			ri.resource("fullresource", "attributes", "string operation", &save, RI_NULL);
			ri.objectInstance(myObject);
			ri.transformBegin();
				ri.motionBegin(2, (RtFloat)0.0, (RtFloat)1.0);
					ri.translate(0, 0, 0);
					ri.translate((RtFloat)0.2, 0, 0);
				ri.motionEnd();
				ri.resourceBegin();
					ri.resource("fullresource", "attributes", "string operation", &restore, "string subset", &all, RI_NULL);
					ri.readArchive(myArchive, 0, RI_NULL);
				ri.resourceEnd();
			ri.transformEnd();

			ri.ifBegin(" 1 + 4 * 7.0 == '29' && false");
				ri.sphere(0.75, -1, 1, 360.0, RI_NULL);
			ri.elseIfBegin("'*abc\\'' =~ '\\*abc?' || $Frame==1 ");
				ri.sphere(1, -1, 1, 360.0, RI_NULL);
			ri.elseBegin();
				ri.sphere(1, -1, 1, 180.0, RI_NULL);
			ri.ifEnd();

			ri.worldEnd();
		ri.frameEnd();

		ri.colorSamples(3, id, id);
		ri.readArchive("testread.rib", 0, RI_NULL);

		ri.frameBegin(3);
			ri.worldBegin();
				ri.readArchive(myArchive, 0, RI_NULL);
			ri.worldEnd();
		ri.frameEnd();

		// ri.readArchive("", 0, RI_NULL);
	ri.end();
}

void testMatch(const CStringPattern &p, const char *str)
{
	str = noNullStr(str);
	if ( p.matches(str) ) {
		std::cout << "matches \"" << str << "\"" << std::endl;
	} else {
		std::cout << "does not match \"" << str << "\"" << std::endl;
	}
}

void testGlob()
{
	CStringPattern p("a*bc?d");
	std::cout << "TestGlob: " << p.pattern() << std::endl;
	char *str;

	str = "abccdbccd"; // a(bccd)bc(c)d
	testMatch(p, str);
	str = "abccdbcd"; // no match
	testMatch(p, str);
}


void sometests(CRiCPPBridge ri)
{
	std::cout << "Hello, World!" << std::endl;

	CBackBufferProtocolHandlers globalFactory;
	testURI();
	testStream(globalFactory);

	std::string str;
	std::cout << CEnv::tmpName() << ": " << CEnv::find(str, CEnv::tmpName(), true) << std::endl;
	std::cout << CEnv::homeName() << ": " << CEnv::find(str, CEnv::homeName(), true) << std::endl;
	std::cout << CEnv::pathName() << ": "  << CEnv::find(str, CEnv::pathName(), true) << std::endl;
	std::cout << CEnv::progDirName() << ": "  << CEnv::find(str, CEnv::progDirName(), true) << std::endl;

	std::string tmp(CEnv::tmpName());
	tmp = "gztest.gz";
	gzFile f = gzopen(tmp.c_str(), "w");
	if ( f ) {
		gzwrite(f, "Teste", 4);
		gzclose(f);
	}

#ifdef WIN32
	CStringList testpath;
	testpath.explode(':', "$HOME:C|/testdir:C:Anothertest:C|/test;.:$PROGDIR:$PATH", true, true, true);

	std::string teststr;
	testpath.implode(':', teststr, true);
	testpath.clear();
	testpath.explode(':', teststr.c_str(), false, false, true);
#endif

	testGlob();
	testrun(ri);

	// Print error, does not abort
	ri.errorHandler(ri.errorPrint());

	// Test to load a rib filter
	ri.option("searchpath", "ribfilter", "@:$PATH", RI_NULL);
	ri.addFrontRibFilter("passthrough");

	ri.begin("test.rib");
	RtContextHandle ch1 = ri.getContext();
	ri.end();

	const char *filename = "$TMP/test.rib";
	RtInt compress = 3;
	RtContextHandle ch2 = ri.begin("ribwriter", RI_FILE, &filename, "compress", &compress, RI_NULL);
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
		ri.begin("testrun.rib");
			ri.archiveRecord(RI_STRUCTURE, "RenderMan RIB-Structure 1.1", RI_NULL);
			ri.archiveRecord(RI_STRUCTURE, "For %s", "somebody", RI_NULL);
			ri.version();
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

	const char *rendererPath = "&:$PATH";
	ri.option("searchpath", "renderer", &rendererPath, RI_NULL);
	ri.begin("ribwriter");
//	ri.readArchive("../../../../ribsamples/hermite.rib", 0, RI_NULL);
	ri.end();

	std::cout << "Good bye, World!" << std::endl;
}

/** @brief Interface test program
 */
int main(int argc, char * const argv[])
{
	CRiCPPBridge ri;

/*	
	// This is only for testing within IDE while programming...
	std::string ribfilename = "";
#if defined (__APPLE__)
	ribfilename = "../../";
#endif
	ribfilename += "../../../RibSamples/Archive.rib";
*/

	ri.errorHandler(ri.errorPrint());

//	ri.begin("|aqsis");

	ri.begin(RI_NULL);

	RtInt yes = 1;
	RtInt no = 0;
	RtInt special = -1;

	/// @todo Skip header (special i.e. only one header, suppress the rest) with uncached archives doesn't work correctly
	ri.control("ribwriter", "skip-headers", &special, RI_NULL);
	ri.control("ribwriter", "postpone-inline-archives", &no, RI_NULL);
	ri.control("ribwriter", "postpone-file-archives", &no, RI_NULL);
	ri.control("ribwriter", "postpone-objects", &no, RI_NULL);
	/// @todo postpone-procedurals and postpone-delayed postpone-run postpone-dynamic
	// ri.control("ribwriter", "postpone-procedurals", &yes, RI_NULL);
	// ri.control("rib", "cache-file-archives", &yes, RI_NULL);

	/*
	ri.archiveRecord(RI_STRUCTURE, "RenderMan %s", "RIB-Structure 1.1");
	ri.archiveRecord(RI_STRUCTURE, "Scene %s", "Test");
	ri.archiveRecord(RI_STRUCTURE, "Creator %s", "RiCPP");
	ri.archiveRecord(RI_STRUCTURE, "Frames %d", 1);
	// ri.version();
	*/
	
	std::string filename;
	if ( argc > 1 ) {
		for ( int i = 1; i < argc; ++i ) {
			filename = noNullStr(argv[i]);
			// CFilepathConverter::convertToURI(filename);
			ri.readArchive(filename.c_str(), 0, RI_NULL);
		}
	} else {
		ri.readArchive(RI_NULL, 0, RI_NULL);
	}

	ri.end();
/* // Wait for a key
#if defined (_DEBUG)
#if defined (WIN32)
	char a;
	std::cin >> a;
#endif
#endif
*/
    return 0;
}
