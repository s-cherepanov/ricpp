#include "ricpp/ricppbridge.h"
#include <iostream>

/**
 *  Interface test program
 */
int main (int argc, char * const argv[]) {
	RiCPP::CRiCPPBridge ri;
    std::cout << "Hello, World!\n";
	ri.errorHandler(ri.errorPrint());
	ri.begin("test.rib");
	ri.end();
	ri.errorHandler(ri.errorAbort());
	// ri.begin("ribwriter test.rib");
	ri.begin("ribwriter");
	ri.end();
    std::cout << "Good bye, World!\n";
    return 0;
}
