// RICPP - RenderMan(R) Interface CPP Language Binding
//
//     RenderMan(R) is a registered trademark of Pixar
// The RenderMan(R) Interface Procedures and Protocol are:
//         Copyright 1988, 1989, 2000, 2005 Pixar
//                 All rights Reservered
// 
// Copyright (c) of RiCPP 2007, Andreas Pidde
// Contact: andreas@pidde.de
//
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public
// License as published by the Free Software Foundation; either
// version 2 of the License, or (at your option) any later version.
//  
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Public License for more details.
//
// You should have received a copy of the GNU General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

/** @file glutrib.cpp
 *  @author Andreas Pidde (andreas@pidde.de)
 *  @brief Simple rib loader using GLUT to for a GL context.
 *  @todo Seems to be something wrong with the depth buffer.
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

static float sphi=0.0, stheta=0.0;
static float sdepth = 0.0;
static int downX=0, downY=0;
static bool leftButton = false, middleButton = false;

static int storedArgc = 0;
static char **storedArgv = 0;

void testTeapot()
{
	RtObjectHandle handle = RiObjectBegin(); {
		RiGeometry(RI_TEAPOT, RI_NULL);
	} RiObjectEnd();

	RiAttributeBegin(); {
		RtFloat Cs[] = {0.33F, 0.33F, 1.0F};
		RiColor(Cs);
		RiTranslate(0, -.25, 2.75);
		RiScale(0.25F, 0.25F, 0.25F);
		RiRotate(-90, 1, 0, 0);
		RiSides(2);
		RiObjectInstance(handle);
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
	RiArchiveBegin("RIBARCHIVE", RI_NULL); {
		RiClipping(0.1F, 50.0F);
		RiWorldBegin(); {
			// testPolygon();
			// testCone();
			testTeapot();
		} RiWorldEnd();
	} RiArchiveEnd();
}

void loadScene(const char *filename)
{
	RiArchiveBegin("RIBARCHIVE", RI_NULL); {
		RiReadArchive(filename, 0, RI_NULL);
	} RiArchiveEnd();
}

void display(void)
{
	char *screenAction[2] = {"clear", "finish"};
	// RtInt noYes[2] = {0, 1};

	// RiCPPControl("rib", "cache-file-archives", &noYes[1], RI_NULL);
	RiCPPControl("glrenderer", "screen", &screenAction[0], RI_NULL);
	
	RiFormat(width, height, 1.0F);
	RiOption("glrenderer", RI_DISPXRES, &width, RI_DISPYRES, &height, RI_NULL);
    
	if ( storedArgc == 1 ) {
		RtFloat fov = 45.0;
		RiProjection(RI_PERSPECTIVE, RI_FOV, &fov, RI_NULL);
	}
	
	RiTransformBegin(); {
		char *matrixName[] = {"pre-camera"};
		RiIdentity();
		RiTranslate(0.0F,0.0F,-sdepth); // Move back and forth
		RiTranslate(0, 0, 2.75); // Move back to previous pos
		RiRotate(stheta, 1.0, 0.0, 0.0); // Rotate x
		RiRotate(-sphi, 0.0, 1.0, 0.0); // Rotate y
		RiTranslate(0, 0, -2.75); // Move to a pivot
		RiCPPControl("state", "string store-transform", matrixName, RI_NULL); // Candidate for RiResource
	} RiTransformEnd();

	RiReadArchive("RIBARCHIVE", 0, RI_NULL);
	
	RiCPPControl("glrenderer", "screen", &screenAction[1], RI_NULL);
	// RiIdentity(); // done by restart
	glutSwapBuffers();

	RiSynchronize("restart");
}

void reshape(int aWidth, int aHeight)
{
	width = aWidth;
	height = aHeight;
}

// Copied from GLUT newave.c
void motion(int x, int y)
{
    if (leftButton)
    {
        sphi += (float)(x - downX) / 4.0;
        stheta += (float)(downY - y) / 4.0;
    }
    if (middleButton)
    {
        sdepth += (float)(downY - y) / 10.0;
    }
    downX = x;
    downY = y;
    glutPostRedisplay();
}


// Copied from GLUT newave.c
void mouse(int button, int state, int x, int y)
{
    downX = x;
    downY = y;
    leftButton = ((button == GLUT_LEFT_BUTTON) && 
                  (state == GLUT_DOWN));
    middleButton = ((button == GLUT_MIDDLE_BUTTON) && 
                    (state == GLUT_DOWN));
}

int main(int argc, char **argv)
{
	// const char *searchPath = ".:&";
  	// RiCPPControl("searchpath", "renderer", &searchPath, RI_NULL);

	storedArgc = argc;
	storedArgv = argv;
	
	glutInit(&argc, argv);
	glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH );
	glutInitWindowSize (width, height); 
	glutInitWindowPosition (100, 100);
	glutCreateWindow(argc <= 1 ? "GLUT RIB" : argv[1]);
	
   	glutDisplayFunc(display); 
	glutReshapeFunc(reshape);
    glutMouseFunc(mouse);
    glutMotionFunc(motion);

	
	RiBegin("glrenderer"); {
		if ( argc <= 1 ) {
			testScene();
		} else {
			loadScene(argv[1]);
		}
		glutMainLoop();
	} RiEnd();
	
	return 0;
}
