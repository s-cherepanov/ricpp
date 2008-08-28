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
	RiArchiveBegin("RIBARCHIVE", RI_NULL); {
		RiClipping(0.1F, 30.0F);
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

	RiCPPControl("glrenderer", "screen", &screenAction[0], RI_NULL);
	
	RiFormat(width, height, 1.0F);
	RiOption("glrenderer", RI_DISPXRES, &width, RI_DISPYRES, &height, RI_NULL);
	RiReadArchive("RIBARCHIVE", 0, RI_NULL);
	RiIdentity();
	
	RiCPPControl("glrenderer", "screen", &screenAction[1], RI_NULL);
	// glutSwapBuffers();
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
	glutInitDisplayMode (GLUT_SINGLE | GLUT_RGBA | GLUT_DEPTH );
	glutInitWindowSize (width, height); 
	glutInitWindowPosition (100, 100);
	glutCreateWindow(argc <= 1 ? "GLUT RIB" : argv[1]);
   	glutDisplayFunc(display); 
	glutReshapeFunc(reshape);

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
