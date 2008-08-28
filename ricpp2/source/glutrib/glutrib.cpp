/*
 *  riabtest.cpp
 *  ABLoader
 *
 *  Created by Andreas Pidde on 06.08.08.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */

#include "ricpp/ri/ri.h"

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif


static RtFloat blueish[] = {0.5,  0.5, 1};
static RtInt width = 512;
static RtInt height = 512;

void testCone()
{
	const RtFloat rad = 0.6;
	const RtFloat height = 1.6;
	RiAttributeBegin();
	RiSides(2);
	RiOrientation(RI_OUTSIDE);
	RiColor(blueish);
	RiTransformBegin();
	RiTranslate(0, -height/4.0, 3);
	RiScale(1.0, 1.0, 1.0);
	RiRotate(-45, 1, 0, 0);
	RiCone(height, rad, 360, RI_NULL);
	RiTransformEnd();
	RiAttributeEnd();
}

void testPolygon()
{
	RtFloat p[12] = {
		-.5F,  .5F, 0,
		 .5F,  .5F, 0,
		 .5F, -.5F, 0,
		-.5F, -.5F, 0
	};
	RiPolygon(4, RI_P, p, RI_NULL);
}
	
void testScene(void)
{
	RiArchiveBegin("RIBARCHIVE", RI_NULL);
	RiWorldBegin();
		RiAttributeBegin();
		RtFloat Cs[] = {0.33F, 0.33F, 1.00F};
			RiSides(1);
			RiColor(Cs);
			RiTranslate(0, -.25, 3);
			RiScale(0.25F, 0.25F, 0.25F);
			RiRotate(-135, 1, 0, 0);
			RiGeometry(RI_TEAPOT, RI_NULL);
		RiAttributeEnd();
	RiWorldEnd();
	// testCone();
	// testPolygon();
	RiArchiveEnd();
}

void loadScene(const char *filename)
{
	RiArchiveBegin("RIBARCHIVE", RI_NULL);
	RiReadArchive(filename, 0, RI_NULL);
	RiArchiveEnd();
}

void display(void)
{
	RiFormat(width, height, 1.0F);
	RiOption("glrenderer", RI_DISPXRES, &width, RI_DISPYRES, &height, RI_NULL);
	RiReadArchive("RIBARCHIVE", 0, RI_NULL);
	RiIdentity();
	glutSwapBuffers();
}

void reshape(int aWidth, int aHeight)
{
	width = aWidth;
	height = aHeight;
}

int main(int argc, char **argv)
{
	// const char *searchPath = ".:&";
  	// RiCPPControl("searchpath", "renderer", &searchPath, RI_NULL);

	glutInit(&argc, argv);
	glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowSize (width, height); 
	glutInitWindowPosition (100, 100);
	glutCreateWindow ("GLUT RIB");

	RiBegin("glrenderer");
	// RiBegin(RI_NULL);

	if ( argc <= 1 ) {
		testScene();
	} else {
		loadScene(argv[1]);
	}
	
   	glutDisplayFunc(display); 
	glutReshapeFunc(reshape);
   	glutMainLoop();

	RiEnd();

	return 0;
}
