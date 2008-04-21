/*
 *  testpoly.cpp
 *
 */

#include "ricpp/ricontext/polygon.h"

using namespace RiCPP;

void test2(CTriangulatedPolygon &poly)
{
	RtFloat p[] = {
		// outer
		1, 1, 0, // 0
		1, 8, 0, // 1
		4, 8, 0, // 2
		5, 6, 0, // 3
		6, 8, 0, // 4
		7, 8, 0, // 5
		
		// inner (hole)
		2, 5, 0,  // 6
		2, 6, 0,  // 7
		3, 5.5, 0 // 8
	};
	
	RtInt loops[] = {
		6,
		3
	};

	RtInt verts[] = {
		0, 1, 2, 3, 4, 5,
		6, 7, 8
	};
	
	poly.triangulate(sizeof(loops)/sizeof(loops[0]), loops, verts, p);
}

void test1(CTriangulatedPolygon &poly)
{
	RtFloat p[] = {
		1, 1, 0, // 0
		1, 4, 0, // 1
		4, 4, 0, // 2
		4, 1, 0, // 3
		1, 1, 0  // 4
	};
	
	RtInt loops[] = {
		4
	};
	
	RtInt verts[] = {
		0, 1, 2, 3, 4
	};
	
	poly.triangulate(sizeof(loops)/sizeof(loops[0]), loops, verts, p);
}


int main(int argc, char * const argv[])
{
	CEarClipper earClipper;
	CTriangulatedPolygon poly(earClipper);
	test2(poly);
	return 0;
}
