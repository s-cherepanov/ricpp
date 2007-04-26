// testmesh.cpp : Definiert den Einstiegspunkt für die Konsolenanwendung.
//

#include "stdafx.h"
#include "rendercontext/rendercontext.h"
#include "rendercontext/subdivision.h"
#include <iostream>


class TMyRenderer : public TIntermediateRenderer {
public:
    inline virtual void doSubdivisionMeshV(RtToken scheme, RtInt nfaces, RtInt nvertices[], RtInt vertices[], RtInt ntags, RtToken tags[], RtInt nargs[], RtInt intargs[], RtFloat floatargs[],  RtInt n, RtToken tokens[], RtPointer params[])
    {
		TSubdivisionMesh sub(lineNo(), getDeclMap(), 3, scheme, nfaces, nvertices, vertices, ntags, tags, nargs, intargs, floatargs, n, tokens, params);
		sub.subdivide(16.0, 16.0);
		std::cout << "TMyRenderer dump:" << std::endl;
		sub.dump(std::cout, "   ");
    }
};

void displaySubdivisionSurface(int argc) {
	TMyRenderer ri;
	ri.begin();
	ri.worldBegin();
	std::string pre="";

	if ( argc == 2 ) {

		RtInt nfaces = 9;
		RtInt nvertices[9] = {4, 4, 4, 4, 4, 4, 4, 4, 4};
		RtInt vertices[9*4] = {
			0, 4,   5, 1, 1, 5,   6,  2,  2,  6,  7,  3,
			4, 8,   9, 5, 5, 9,  10,  6,  6, 10, 11,  7,
			8, 12, 13, 9, 9, 13, 14, 10, 10, 14, 15, 11
		};
		RtFloat controlMesh[16*3] = {
			-0.6F, 0.6F, 0.0F, -0.6F, 0.2F, 0.0F,  -0.6F, -0.2F, 0.0F,  -0.6F, -0.6F, 0.0F,
			-0.2F, 0.6F, 0.0F, -0.2F, 0.2F, 0.45F, -0.2F, -0.2F, 0.45F, -0.2F, -0.6F, 0.0F,
			0.2F, 0.6F, 0.0F,  0.2F, 0.2F, 0.45F,  0.2F, -0.2F, 0.45F,  0.2F, -0.6F, 0.0F,
			0.6F, 0.6F, 0.0F,  0.6F, 0.2F, 0.0F,   0.6F, -0.2F, 0.0F,   0.6F, -0.6F, 0.0F
		};
		RtInt sumVerts = 16*3;
		RtInt ntags = 1;
		RtToken tags[1] = { TRi::RI_INTERPOLATEBOUNDARY };
		RtInt nargs[2] = { 0, 0 };

		ri.subdivisionMesh(TRi::RI_CATMULLCLARK, nfaces, nvertices, vertices, ntags, tags, nargs, NULL, NULL, TRi::RI_P, controlMesh, TRi::RI_NULL);

	} else {

		RtInt nfaces = 2;
		RtInt nvertices[2] = {4, 4};
		RtInt vertices[2*4] = {
			0, 1, 2, 3,
			0, 4, 5, 1
		};
		RtFloat controlMesh[6*3] = {
			1.0,  1.0, 0.0,
			-1.0,  1.0, 0.0,
			-1.0, -1.0, 0.0,
			1.0, -1.0, 0.0,
			1.0,  3.0, 0.0,
			-1.0,  3.0, 0.0
		};
		RtInt sumVerts = 6*3;
		RtInt ntags = 1;
		RtToken tags[1] = { TRi::RI_INTERPOLATEBOUNDARY };
		RtInt nargs[2] = { 0, 0 };

		ri.subdivisionMesh(TRi::RI_CATMULLCLARK, nfaces, nvertices, vertices, ntags, tags, nargs, NULL, NULL, TRi::RI_P, controlMesh, TRi::RI_NULL);

	}

	ri.worldEnd();
	ri.end();
}

int main(int argc, char *argv[])
{
	setlocale( LC_ALL, "C" );
	displaySubdivisionSurface(argc);
	return 0;
}
