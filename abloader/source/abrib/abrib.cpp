/*
 *  abrib.cpp
 *  ABLoader
 *
 *  Created by Andreas Pidde on 06.08.08.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef _ABLOADER_TOOLS_TRACE
#include "abloader/tools/trace.h"
#endif // _ABLOADER_TOOLS_TRACE

#include <ricpp/ricpp.h>

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

static RtFloat blueish[] = {0.5,  0.5, 1};

void testTeapot()
{
	RiAttributeBegin(); {
		RtFloat Cs[] = {0.33F, 0.33F, 1.00F};
		RiColor(Cs);
		RiTranslate(0, -.25, 3);
		RiScale(0.25F, 0.25F, 0.25F);
		RiRotate(-135, 1, 0, 0);
		RiSides(2);
		RiGeometry(RI_TEAPOT, RI_NULL);
	} RiAttributeEnd();
}

void testCone()
{
	const RtFloat rad = 0.6;
	const RtFloat height = 1.6;
	RiAttributeBegin(); {
		RiSides(2);
		RiOrientation(RI_OUTSIDE);
		RiColor(blueish);
		RiTransformBegin(); {
			RiTranslate(0, -height/4.0, 3);
			RiScale(1.0, 1.0, 1.0);
			RiRotate(-45, 1, 0, 0);
			RiCone(height, rad, 360, RI_NULL);
		} RiTransformEnd();
	} RiAttributeEnd();
}

void testPolygon()
{
	RtFloat p[12] = {
		-.5F,  .5F, 0,
		.5F,  .5F, 0,
		.5F, -.5F, 0,
		-.5F, -.5F, 0
	};
	RtFloat st[8] = {
		0, 0,
		1, 0,
		1, 1,
		0, 1
	};
	RiPolygon(4, RI_P, p, RI_ST, st, RI_NULL);
}

void testScene(void)
{
	RiWorldBegin();
	testTeapot();
	// testCone();
	// testPolygon();
	RiWorldEnd();
}

void loadScene(const char *filename)
{
	RiReadArchive(filename, 0, RI_NULL);
}

void display(void)
{
	RiSynchronize("render");
	/*
	 AUGENBLICK->requestNewRendering();
	 */
}


void mouse(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		RtInt xy[2] = {x, y};
		RiCPPControl("abribloader", "setMousePosition", xy, RI_NULL); 
		// AUGENBLICK->setMousePosition(x,y);
	}
}

void motion(int x, int y)
{
	RtInt xy[2] = {x, y};
	RiCPPControl("abribloader", "mouseMovedTo", xy, RI_NULL); 
	RiSynchronize("render");
	/*
	AUGENBLICK->mouseMovedTo(x,y);
	AUGENBLICK->requestNewRendering();
	 */
}

int main(int argc, char **argv)
{
	// RtString hierarchyTypes[] = {"bvh", "sahbvh"}; // sahbvh is default
	RiBegin("abribloader");
	// RiBegin(RI_NULL);

	glutInit(&argc, argv);
	glutInitDisplayMode (GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize (512, 512); 
	glutInitWindowPosition (100, 100);
	glutCreateWindow (argc <= 1 ? "abrib" : argv[1]);

	// RiOption("abribloader", "hierarchyType", &hierarchyTypes[1], RI_NULL);
	if ( argc <= 1 )
		testScene();
	else
		loadScene(argv[1]);
	
   	glutDisplayFunc(display); 
   	glutMouseFunc(mouse);
   	glutMotionFunc(motion);
   	glutMainLoop();

	RiEnd();

	return 0;
}
