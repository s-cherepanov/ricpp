#include "ricpp/ricppbridge.h"
#include "tools/stringlist.h"
#include "tools/env.h"
#include "tools/filepath.h"

#include <iostream>

using namespace RiCPP;

/**
 *  Interface test program
 */
int main (int argc, char * const argv[]) {
	CRiCPPBridge ri;
	std::cout << "Hello, World!" << std::endl;

	std::string str;
	std::cout << CEnv::tmpName() << ": " << CEnv::find(str, CEnv::tmpName()) << std::endl;
	std::cout << CEnv::homeName() << ": " << CEnv::find(str, CEnv::homeName()) << std::endl;
	std::cout << CEnv::pathName() << ": "  << CEnv::find(str, CEnv::pathName()) << std::endl;
	std::cout << CEnv::progDirName() << ": "  << CEnv::find(str, CEnv::progDirName()) << std::endl;

	ri.errorHandler(ri.errorPrint());
	ri.begin("test.rib");
	ri.end();
	ri.option("searchpath", "renderer", ".", RI_NULL);
	ri.begin("ribwriter \"$HOME$/test.rib\"");
	ri.end();

	ri.errorHandler(ri.errorAbort());

	ri.option("searchpath", "renderer", ".;$PROGDIR$;$PATH$", RI_NULL);
	ri.begin("ribwriter");
	ri.end();
    std::cout << "Good bye, World!" << std::endl;

    return 0;
}
