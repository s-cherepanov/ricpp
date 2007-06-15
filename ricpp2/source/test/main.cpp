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

#include <iostream>

using namespace RiCPP;

/** @brief Test a base URI with reference URI strings.
 *
 * Prints the results to cout
 *
 * @param baseUri The base URI
 * @param refUriStrs The reference URIs as char pointer array
 */
void testURI(const CUri &baseUri, const char **refUriStrs)
{
	if ( !refUriStrs )
		return;

	CUri refUri, absUri;
	std::string absoluteUriStr;

	for ( const char **strPtr = refUriStrs; *strPtr; ++strPtr ) {
		const char *str = strPtr[0];
		refUri = str;
		bool valid_absUri = false;

		if ( refUri.isValid() ) {
			valid_absUri = refUri.makeAbsolute(baseUri, absoluteUriStr);
			absUri = absoluteUriStr;
			valid_absUri = absUri.isValid();
			if ( refUri.toString().empty() )
				std::cout << "<>";
		}

		std::cout
			<< (const char *)(refUri.isValid() ? refUri.c_str() : "<invalid>") << " = "
			<< (const char *)(valid_absUri && absUri.isCurrentDocument() ? "(current document)" : "")
			<< (const char *)(valid_absUri ? absUri.c_str() : "<invalid>") << std::endl;
	}
}

/** @brief Test URI RFC 2396
 *
 * Prints the results to cout
 */
void testURI()
{
	std::cout << "RFC 2396" << std::endl;
	std::cout << "C. Examples of Resolving Relative URI References" << std::endl;

	std::cout << "Within an object with a well-defined base URI of" << std::endl;

	CUri baseUri("http://a/b/c/d;p?q");
	if ( baseUri.isValid() ) {
		std::cout << baseUri.c_str() << std::endl;
	} else {
		std::cout << "Base URI invalid" << std::endl;
		return;
	}

	std::cout << "The relative URI would be resolved as follows:" << std::endl;

	std::cout << "C.1 Normal Examples" << std::endl;

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

	std::cout << "C.2 Abnormal Examples" << std::endl;

	const char *abnormalExamples[] = {
		"",

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
}

/** @brief Interface test program
 */
int main (int argc, char * const argv[]) {
	std::cout << "Hello, World!" << std::endl;

	CUri testUri("http://myname:abcd@www.nowhere-1.com.:8080/A%20directory;find;more=1/subdirectory/index.php?i=wqqw&j=1234+6#frag");
	CUri testUri2("http://127.0.0.1");
	CUri testUri3("file:///C:/");
	CUri testUri4("ftp:/");
	CUri relUri("adir/name;str/./deldir/../index.html?#anchor");
	CUri relUri2("#fragment");
	CUri relUri3("/rootdir");
	CUri relUri4;

	std::string refUriStr;
	if ( relUri.makeAbsolute(testUri, refUriStr) )
		std::cout << refUriStr << std::endl;
	else
		std::cout << "Error in uri" << std::endl;

	if ( relUri.makeAbsolute(testUri2, refUriStr) )
		std::cout << refUriStr << std::endl;
	else
		std::cout << "Error in uri" << std::endl;

	if ( relUri.makeAbsolute(testUri3, refUriStr) )
		std::cout << refUriStr << std::endl;
	else
		std::cout << "Error in uri" << std::endl;

	if ( relUri.makeAbsolute(testUri4, refUriStr) )
		std::cout << refUriStr << std::endl;
	else
		std::cout << "Error in uri" << std::endl;

	if ( relUri2.makeAbsolute(testUri4, refUriStr) )
		std::cout << refUriStr << std::endl;
	else
		std::cout << "Error in uri" << std::endl;

	if ( relUri3.makeAbsolute(testUri4, refUriStr) )
		std::cout << refUriStr << std::endl;
	else
		std::cout << "Error in uri" << std::endl;

	testURI();

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
