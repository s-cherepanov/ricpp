#include "ricpp/ricppbridge.h"
#include "tools/stringlist.h"

#include <iostream>
/**
 *  Interface test program
 */
int main (int argc, char * const argv[]) {
	RiCPP::CRiCPPBridge ri;
	std::cout << "Hello, World!" << std::endl;

#if 0
	RiCPP::TStringList<> stringList;
	stringList.push("eins");
	std::cout << &(stringList.top()[0]) << std::endl;
	stringList.push("zwei");
	std::cout << &(stringList.top()[0]) << std::endl;
	std::cout << stringList.size() << std::endl;
	stringList.addSubst("PATH", "/Ein Pfad/");
	stringList.explode("ribout \"$PATH$Eine Datei.rib\"", ' ');
	RiCPP::TStringList<>::const_iterator i = stringList.begin();
	for ( ; i != stringList.end(); i++ ) {
		std::cout << &((*i)[0]) << std::endl;
	}
#endif

	ri.errorHandler(ri.errorPrint());
	ri.begin("test.rib");
	ri.end();
	ri.begin("ribwriter 'A test.rib'");
	ri.end();

	ri.errorHandler(ri.errorAbort());

	ri.begin("ribwriter");
	ri.end();
    std::cout << "Good bye, World!" << std::endl;

    return 0;
}
