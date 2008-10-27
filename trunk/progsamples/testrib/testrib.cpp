// testrib.cpp : Definiert den Einstiegspunkt für die Konsolenanwendung.
//

#include "stdafx.h"
#include "ribparser/ribparser.h"
#include "rendercontext/rendercontext.h"

class TMyRenderer : public TIntermediateRenderer {
public:
    inline virtual void doPointsGeneralPolygonsV (RtInt npolys, RtInt *nloops, RtInt *nverts, RtInt *verts, RtInt n, RtToken tokens[], RtPointer params[])
    {
		// Put your code here
		int i = 5;
		i = i;
    }
};

int main(int argc, char *argv[])
{
	setlocale( LC_ALL, "C" );
	if ( argc > 1 ) {
		TMyRenderer ri;
		// ri.initialize();
		ri.errorHandler(ri.errorPrint);
		for ( int i=1; i<argc; ++i ) {
			ri.begin();
			ri.readArchive(argv[i], NULL, ri.RI_NULL);
			ri.end();
		}
	}
	return 0;
}
