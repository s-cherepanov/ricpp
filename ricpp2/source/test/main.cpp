#include "ricpp/ricppbridge.h"
#include "tools/stringlist.h"
#include "tools/env.h"

#include <iostream>

using namespace RiCPP;

/**
 *  Interface test program
 */
int main (int argc, char * const argv[]) {
	CRiCPPBridge ri;
	std::cout << "Hello, World!" << std::endl;

#if 0
	TStringList<> stringList;
	stringList.push("eins");
	std::cout << &(stringList.top()[0]) << std::endl;
	stringList.push("zwei");
	std::cout << &(stringList.top()[0]) << std::endl;
	std::cout << stringList.size() << std::endl;
	stringList.addSubst("PATH", "/Ein Pfad/");
	stringList.explode("ribout \"$PATH$Eine Datei.rib\"", ' ');
	TStringList<>::const_iterator i = stringList.begin();
	for ( ; i != stringList.end(); i++ ) {
		std::cout << &((*i)[0]) << std::endl;
	}
#endif

	std::string str;
	CEnv::getTemp(str);
	std::cout << "TEMP:" << str << std::endl;
	CEnv::getHome(str);
	std::cout << "HOME:" << str << std::endl;
	CEnv::getPath(str);
	std::cout << "PATH:" << str << std::endl;
	CEnv::getProgDir(str);
	std::cout << "PROGDIR:" << str << std::endl;


	ri.errorHandler(ri.errorPrint());
	ri.option("searchpath", "renderer", "$PROG$;$PATH$", RI_NULL);
	ri.begin("test.rib");
	ri.end();
	ri.begin("ribwriter \"$HOME$test.rib\"");
	ri.end();

	ri.errorHandler(ri.errorAbort());

	ri.begin("ribwriter");
	ri.end();
    std::cout << "Good bye, World!" << std::endl;

    return 0;
}
