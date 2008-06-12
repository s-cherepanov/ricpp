/*
 *  testpoly.cpp
 *
 */

#include "ricpp/ricontext/polygon.h"
#include <iostream>

using namespace RiCPP;

void printPoly(const CTriangulatedPolygon &poly,
			   const RtInt verts[],
			   const RtFloat *p,
			   RtFloat scale=50)
{
	std::cout << "100 100 translate" << std::endl;
	std::cout << scale << " " << scale << " scale" << std::endl;
	std::cout << "0.01 setlinewidth" << std::endl;
	std::cout << "1 setlinejoin" << std::endl;
	std::cout << "0 setgray" << std::endl;

	const std::vector<CPolygonNode::IndexType> &triangles = poly.triangles();
	std::vector<CPolygonNode::IndexType>::const_iterator iter = triangles.begin();

	RtFloat x, y, z;
	while ( iter != triangles.end() ) {
		x = p[verts[*iter]*3];
		y = p[verts[*iter]*3+1];
		z = p[verts[*iter]*3+2];
		++iter;
		std::cout << "newpath " << x << " " << y << " " << "moveto ";
		x = p[verts[*iter]*3];
		y = p[verts[*iter]*3+1];
		z = p[verts[*iter]*3+2];
		++iter;
		std::cout << x << " " << y << " " << "lineto ";
		x = p[verts[*iter]*3];
		y = p[verts[*iter]*3+1];
		z = p[verts[*iter]*3+2];
		++iter;
		std::cout << x << " " << y << " " << "lineto ";
		std::cout << "closepath " << std::endl;
		std::cout << "    gsave 0.75 setgray fill grestore stroke" << std::endl;
	}
	std::cout << "showpage" << std::endl;	
}


void test4(CTriangulatedPolygon &poly, const IPolygonTriangulationStrategy &strategy)
{
	// O'Rourke's poly 1.1
	RtFloat p[] = {
		// outer
		 0,  0, 0, //  0
		10,  7, 0, //  1
		12,  3, 0, //  2
		20,  8, 0, //  3
		13, 17, 0, //  4
		10, 12, 0, //  5
		12, 14, 0, //  6
		14,  9, 0, //  7
		 8, 10, 0, //  8
		 6, 14, 0, //  9
		10, 15, 0, // 10
		 7, 18, 0, // 11
		 0, 16, 0, // 12
		 1, 13, 0, // 13
		 3, 15, 0, // 14
		 5,  8, 0, // 15
		-2,  9, 0, // 16
		 5,  5, 0  // 17
	};
	
	RtInt nverts[] = {
		18
	};

	RtInt verts[] = {
		 0,  1,  2,  3,  4,
		 5,  6,  7,  8,  9,
		10, 11, 12, 13, 14,
		15, 16, 17
	};
	
	poly.triangulate(strategy, sizeof(nverts)/sizeof(nverts[0]), nverts, verts, p);
	printPoly(poly, verts, p, 20);
}

void test3(CTriangulatedPolygon &poly, const IPolygonTriangulationStrategy &strategy)
{
	RtFloat p[] = {
		// outer
		7, 8, 0, // 5
		6, 8, 0, // 4
		5, 6, 0, // 3
		4, 8, 0, // 2
		1, 8, 0, // 1
		1, 1, 0, // 0
		
		// inner (hole)
		2, 5,   0,  // 6
		2, 6,   0,  // 7
		3, 5.5, 0,  // 8

		// inner (hole)
		2, 3,   0,  //  9
		2, 4,   0,  // 10
		3, 3.5, 0   // 11
	};
	
	RtInt nverts[] = {
		6,
		3,
		3
	};

	RtInt verts[] = {
		0,  1,  2, 3, 4, 5,
		6,  7,  8,
		9, 10, 11,
	};
	
	poly.triangulate(strategy, sizeof(nverts)/sizeof(nverts[0]), nverts, verts, p);
	printPoly(poly, verts, p);
}

void test2(CTriangulatedPolygon &poly, const IPolygonTriangulationStrategy &strategy)
{
	RtFloat p[] = {
		// outer
		1, 1, 0, // 0
		1, 8, 0, // 1
		8, 8, 0, // 2
		8, 1, 0, // 3

		// inner (hole)
		2, 2, 0, // 4
		2, 7, 0, // 5
		7, 7, 0, // 6
		7, 2, 0, // 7
	};
	
	RtInt nverts[] = {
		4, 4
	};
	
	RtInt verts[] = {
		3, 2, 1, 0,
		4, 5, 6, 7
	};
	
	poly.triangulate(strategy, sizeof(nverts)/sizeof(nverts[0]), nverts, verts, p);
	printPoly(poly, verts, p);
}

void test1(CTriangulatedPolygon &poly, const IPolygonTriangulationStrategy &strategy)
{
	RtFloat p[] = {
		// outer
		1, 1, 0, // 0
		1, 8, 0, // 1
		8, 8, 0, // 2
		8, 1, 0  // 3
	};
	
	RtInt nverts[] = {
		4
	};
	
	RtInt verts[] = {
		3, 2, 1, 0
	};
	
	poly.triangulate(strategy, sizeof(nverts)/sizeof(nverts[0]), nverts, verts, p);
	printPoly(poly, verts, p);
}

void test0(CTriangulatedPolygon &poly, const IPolygonTriangulationStrategy &strategy)
{
	RtFloat p[] = {
		// outer
		1, 1, 0, // 0
		1, 8, 0, // 1
		8, 8, 0, // 2
	};

	RtInt nverts[] = {
		3
	};

	RtInt verts[] = {
		0, 1, 2
	};
	
	poly.triangulate(strategy, sizeof(nverts)/sizeof(nverts[0]), nverts, verts, p);
	printPoly(poly, verts, p);
}

int main(int argc, char * const argv[])
{
	// Initializing the triangulation
	CEarClipper earClipper;
	CTriangulatedPolygon poly;
	
	// PostScript Header
	std::cout << "%!PS" << std::endl;
	
	// Put out some polygons
	test0(poly, earClipper);
	test1(poly, earClipper);
	test2(poly, earClipper);
	test3(poly, earClipper);
	test4(poly, earClipper);
	
	return 0;
}
