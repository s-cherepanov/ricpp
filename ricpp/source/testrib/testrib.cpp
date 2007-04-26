// testrib.cpp : Definiert den Einstiegspunkt für die Konsolenanwendung.
//

#include "stdafx.h"
// #include "ribparser/ribparser.h"
// #include "rendercontext/rendercontext.h"
#include "libribout/libribout.h"
/*
class TMyRenderer : public TIntermediateRenderer {
public:
    inline virtual void doPointsGeneralPolygonsV (RtInt npolys, RtInt *nloops, RtInt *nverts, RtInt *verts, RtInt n, RtToken tokens[], RtPointer params[])
    {
		// Put your code here
		int i = 5;
		i = i;
    }
};
*/

int main(int argc, char *argv[])
{
	setlocale( LC_ALL, "C" );
	if ( argc > 1 ) {
//		TMyRenderer ri;
		TRIBOut ri;
		RtBoolean b = ri.RI_TRUE;
		ri.errorHandler(ri.errorPrint);
		for ( int i=1; i<argc; ++i ) {
			std::string name(argv[i]);
			name += ".ribout.txt";
			ri.begin(name.c_str());
			ri.option("ricpp.control", "ricpp.control.writearchive", &b, ri.RI_NULL);
			// ri.option("ricpp.control", "ricpp.control.writeobject", &b, ri.RI_NULL);
			ri.readArchive(argv[i], NULL, ri.RI_NULL);
			ri.end();
		}
	}
	return 0;
}
