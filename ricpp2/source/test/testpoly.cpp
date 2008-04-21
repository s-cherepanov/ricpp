/*
 *  testpoly.cpp
 *
 */

#include "ricpp/ricontext/polygon.h"

using namespace RiCPP;

void test2(CTriangulatedPolygon &poly)
{
	RtFloat p[] = {
		1, 1, 0, // 0
		1, 8, 0, // 1
		4, 8, 0, // 2
		5, 6, 0, // 3
		6, 8, 0, // 4
		7, 8, 0, // 5
		1, 1, 0, // 6
		
		2, 5, 0, // 7
		2, 6, 0, // 8
		3, 5.5, 0 // 9
	};
	
	RtInt loops[] = {
		8,
		4
	};

	RtInt verts[] = {
		0, 1, 2, 3, 4, 5, 6, 0,
		7, 8, 9, 7
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
		5
	};
	
	RtInt verts[] = {
		0, 1, 2, 3, 4, 0
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
