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

#include <ricpp/ricpp.h>

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
// #ifdef __GNUC__
// #define _WCHAR_T_DEFINED
// #define GLUT_NO_LIB_PRAGMA
// #define GLUT_NO_WARNING_DISABLE
// #endif
#include <GL/glut.h>
#endif

// ---> cannot compile on Winodws
// #include <iostream>

static RtFloat opacity_25[] = {0.25,  0.25, 0.25};
static RtFloat opacity_50[] = {0.5,  0.5, 0.5};
static RtFloat opacity_75[] = {0.75,  0.75, 0.75};
static RtFloat greenish[] = {0.5,  1, 0.5};
static RtFloat redish[] = {1,  0.5, 0.5};
static RtFloat blueish[] = {0.5,  0.5, 1};

// Funny, 512x512 resulution did not work one laptop (ThinkPad T41)
// under Gnu/Linux
static RtInt width = 640;
static RtInt height = (RtInt)(width * 3.0 / 4.0);

static float sphi=0.0, stheta=0.0;
static float sdepth = 0.0;
static int downX=0, downY=0;
static bool leftButton = false, middleButton = false, rightButton = false;

static int storedArgc = 0;
static char **storedArgv = 0;

void testVase()
{
    static RtFloat vaseMesh[] = { // Data copied from BMRT
        0.0F, 0.0F, 0.0F, 0.869825F, 0.0F, 0.0F, 0.0F, 0.902369F, 0.0F, 0.0F, 0.0F, 1.0F,
		0.0F, 0.0F, 0.0F, 0.707107F, 0.0F, 0.0F, 0.0F, 1.0F, 0.0F, 0.0F, 0.0F, 0.707107F,
		0.0F, 0.0F, 0.0F, 1.0F, 0.0F, 0.0F, 0.0F, 0.707107F, 0.0F, 0.0F, 0.0F, 1.0F,
		0.0F, 0.0F, 0.0F, 0.804738F, 0.0F, 0.0F, 0.0F, 0.869825F,
        2.1269F, 3.79357F, 0.0F, 0.869825F, 1.17851F, 4.51184F, 0.0F, 0.902369F, 0.0F, 5.0F, 0.0F, 1.0F, -3.53553F, 3.53553F, 0.0F, 0.707107F, -5.0F, 3.06152e-016F, 0.0F, 1.0F, -3.53553F, -3.53553F, 0.0F,
        0.707107F, -6.12303e-016F, -5.0F, 0.0F, 1.0F, 3.53553F, -3.53553F, 0.0F, 0.707107F, 5.0F, -9.18455e-016F, 0.0F, 1.0F, 4.02369F, 2.35702F, 0.0F, 0.804738F, 2.1269F, 3.79357F, 0.0F, 0.869825F,
        12.1872F, 21.7372F, 0.0F, 4.9841F, 6.75287F, 25.8529F, 0.0F, 5.17057F, 0.0F, 28.65F, 0.0F, 5.73F, -20.2586F, 20.2586F, 0.0F, 4.05172F, -28.65F, 1.75425e-015F, 0.0F, 5.73F, -20.2586F,
        -20.2586F, 0.0F, 4.05172F, -3.5085e-015F, -28.65F, 0.0F, 5.73F, 20.2586F, -20.2586F, 0.0F, 4.05172F, 28.65F, -5.26275e-015F, 0.0F, 5.73F, 23.0557F, 13.5057F, 0.0F, 4.61115F, 12.1872F,
        21.7372F, 0.0F, 4.9841F, 2.1269F, 3.79357F, 0.260948F, 0.869825F, 1.17851F, 4.51184F, 0.270711F, 0.902369F, 0.0F, 5.0F, 0.3F, 1.0F, -3.53553F, 3.53553F, 0.212132F, 0.707107F, -5.0F,
        3.06152e-016F, 0.3F, 1.0F, -3.53553F, -3.53553F, 0.212132F, 0.707107F, -6.12303e-016F, -5.0F, 0.3F, 1.0F, 3.53553F, -3.53553F, 0.212132F, 0.707107F, 5.0F, -9.18455e-016F, 0.3F, 1.0F,
        4.02369F, 2.35702F, 0.241421F, 0.804738F, 2.1269F, 3.79357F, 0.260948F, 0.869825F, 6.38071F, 11.3807F, 0.782843F, 2.60948F, 3.53553F, 13.5355F, 0.812132F, 2.70711F, 0.0F,
        15.0F, 0.9F, 3.0F, -10.6066F, 10.6066F, 0.636396F, 2.12132F, -15.0F, 9.18455e-016F, 0.9F, 3.0F, -10.6066F, -10.6066F, 0.636396F, 2.12132F, -1.83691e-015F, -15.0F, 0.9F, 3.0F, 10.6066F,
        -10.6066F, 0.636396F, 2.12132F, 15.0F, -2.75536e-015F, 0.9F, 3.0F, 12.0711F, 7.07107F, 0.724264F, 2.41421F, 6.38071F, 11.3807F, 0.782843F, 2.60948F, 2.72244F, 4.85577F,
        2.0006F, 0.869825F, 1.50849F, 5.77516F, 2.07545F, 0.902369F, 0.0F, 6.4F, 2.3F, 1.0F, -4.52548F, 4.52548F, 1.62635F, 0.707107F, -6.4F, 3.91874e-016F, 2.3F, 1.0F, -4.52548F, -4.52548F,
        1.62635F, 0.707107F, -7.83748e-016F, -6.4F, 2.3F, 1.0F, 4.52548F, -4.52548F, 1.62635F, 0.707107F, 6.4F, -1.17562e-015F, 2.3F, 1.0F, 5.15032F, 3.01699F, 1.8509F, 0.804738F,
        2.72244F, 4.85577F, 2.0006F, 0.869825F, 2.97767F, 5.311F, 4.34913F, 0.869825F, 1.64992F, 6.31658F, 4.51184F, 0.902369F, 0.0F, 7.0F, 5.0F, 1.0F, -4.94975F, 4.94975F, 3.53553F,
        0.707107F, -7.0F, 4.28612e-016F, 5.0F, 1.0F, -4.94975F, -4.94975F, 3.53553F, 0.707107F, -8.57224e-016F, -7.0F, 5.0F, 1.0F, 4.94975F, -4.94975F, 3.53553F, 0.707107F, 7.0F, -1.28584e-015F, 5.0F,
        1.0F, 5.63316F, 3.29983F, 4.02369F, 0.804738F, 2.97767F, 5.311F, 4.34913F, 0.869825F, 2.55228F, 4.55228F, 6.9586F, 0.869825F, 1.41421F, 5.41421F, 7.21895F, 0.902369F, 0.0F, 6.0F, 8.0F,
        1.0F, -4.24264F, 4.24264F, 5.65685F, 0.707107F, -6.0F, 3.67382e-016F, 8.0F, 1.0F, -4.24264F, -4.24264F, 5.65685F, 0.707107F, -7.34764e-016F, -6.0F, 8.0F, 1.0F, 4.24264F, -4.24264F, 5.65685F,
        0.707107F, 6.0F, -1.10215e-015F, 8.0F, 1.0F, 4.82843F, 2.82843F, 6.4379F, 0.804738F, 2.55228F, 4.55228F, 6.9586F, 0.869825F, 1.27614F, 2.27614F, 8.69825F, 0.869825F, 0.707107F,
        2.70711F, 9.02369F, 0.902369F, 0.0F, 3.0F, 10.0F, 1.0F, -2.12132F, 2.12132F, 7.07107F, 0.707107F, -3.0F, 1.83691e-016F, 10.0F, 1.0F, -2.12132F, -2.12132F, 7.07107F, 0.707107F,
        -3.67382e-016F, -3.0F, 10.0F, 1.0F, 2.12132F, -2.12132F, 7.07107F, 0.707107F, 3.0F, -5.51073e-016F, 10.0F, 1.0F, 2.41421F, 1.41421F, 8.04738F, 0.804738F, 1.27614F, 2.27614F, 8.69825F,
        0.869825F, 1.27614F, 2.27614F, 10.4379F, 0.869825F, 0.707107F, 2.70711F, 10.8284F, 0.902369F, 0.0F, 3.0F, 12.0F, 1.0F, -2.12132F, 2.12132F, 8.48528F, 0.707107F, -3.0F, 1.83691e-016F,
        12.0F, 1.0F, -2.12132F, -2.12132F, 8.48528F, 0.707107F, -3.67382e-016F, -3.0F, 12.0F, 1.0F, 2.12132F, -2.12132F, 8.48528F, 0.707107F, 3.0F, -5.51073e-016F, 12.0F, 1.0F, 2.41421F, 1.41421F,
        9.65685F, 0.804738F, 1.27614F, 2.27614F, 10.4379F, 0.869825F, 1.44629F, 2.57963F, 10.4379F, 0.869825F, 0.801388F, 3.06805F, 10.8284F, 0.902369F, 0.0F, 3.4F, 12.0F, 1.0F, -2.40416F,
        2.40416F, 8.48528F, 0.707107F, -3.4F, 2.08183e-016F, 12.0F, 1.0F, -2.40416F, -2.40416F, 8.48528F, 0.707107F, -4.16366e-016F, -3.4F, 12.0F, 1.0F, 2.40416F, -2.40416F, 8.48528F,
        0.707107F, 3.4F, -6.24549e-016F, 12.0F, 1.0F, 2.73611F, 1.60278F, 9.65685F, 0.804738F, 1.44629F, 2.57963F, 10.4379F, 0.869825F, 1.44629F, 2.57963F, 11.3077F, 0.869825F,
        0.801388F, 3.06805F, 11.7308F, 0.902369F, 0.0F, 3.4F, 13.0F, 1.0F, -2.40416F, 2.40416F, 9.19239F, 0.707107F, -3.4F, 2.08183e-016F, 13.0F, 1.0F, -2.40416F, -2.40416F, 9.19239F, 0.707107F,
        -4.16366e-016F, -3.4F, 13.0F, 1.0F, 2.40416F, -2.40416F, 9.19239F, 0.707107F, 3.4F, -6.24549e-016F, 13.0F, 1.0F, 2.73611F, 1.60278F, 10.4616F, 0.804738F, 1.44629F, 2.57963F,
        11.3077F, 0.869825F, 1.19107F, 2.1244F, 11.3077F, 0.869825F, 0.659966F, 2.52663F, 11.7308F, 0.902369F, 0.0F, 2.8F, 13.0F, 1.0F, -1.9799F, 1.9799F, 9.19239F, 0.707107F, -2.8F,
        1.71445e-016F, 13.0F, 1.0F, -1.9799F, -1.9799F, 9.19239F, 0.707107F, -3.4289e-016F, -2.8F, 13.0F, 1.0F, 1.9799F, -1.9799F, 9.19239F, 0.707107F, 2.8F, -5.14335e-016F, 13.0F, 1.0F, 2.25327F,
        1.31993F, 10.4616F, 0.804738F, 1.19107F, 2.1244F, 11.3077F, 0.869825F, 1.19107F, 2.1244F, 10.4379F, 0.869825F, 0.659966F, 2.52663F, 10.8284F, 0.902369F, 0.0F, 2.8F, 12.0F, 1.0F,
        -1.9799F, 1.9799F, 8.48528F, 0.707107F, -2.8F, 1.71445e-016F, 12.0F, 1.0F, -1.9799F, -1.9799F, 8.48528F, 0.707107F, -3.4289e-016F, -2.8F, 12.0F, 1.0F, 1.9799F, -1.9799F, 8.48528F,
        0.707107F, 2.8F, -5.14335e-016F, 12.0F, 1.0F, 2.25327F, 1.31993F, 9.65685F, 0.804738F, 1.19107F, 2.1244F, 10.4379F, 0.869825F, 1.14853F, 2.04853F, 8.78523F, 0.869825F,
        0.636396F, 2.4364F, 9.11393F, 0.902369F, 0.0F, 2.7F, 10.1F, 1.0F, -1.90919F, 1.90919F, 7.14178F, 0.707107F, -2.7F, 1.65322e-016F, 10.1F, 1.0F, -1.90919F, -1.90919F, 7.14178F,
        0.707107F, -3.30644e-016F, -2.7F, 10.1F, 1.0F, 1.90919F, -1.90919F, 7.14178F, 0.707107F, 2.7F, -4.95966e-016F, 10.1F, 1.0F, 2.17279F, 1.27279F, 8.12785F, 0.804738F, 1.14853F,
        2.04853F, 8.78523F, 0.869825F, 2.38213F, 4.2488F, 6.9586F, 0.869825F, 1.31993F, 5.05327F, 7.21895F, 0.902369F, 0.0F, 5.6F, 8.0F, 1.0F, -3.9598F, 3.9598F, 5.65685F, 0.707107F, -5.6F,
        3.4289e-016F, 8.0F, 1.0F, -3.9598F, -3.9598F, 5.65685F, 0.707107F, -6.8578e-016F, -5.6F, 8.0F, 1.0F, 3.9598F, -3.9598F, 5.65685F, 0.707107F, 5.6F, -1.02867e-015F, 8.0F, 1.0F, 4.50653F,
        2.63987F, 6.4379F, 0.804738F, 2.38213F, 4.2488F, 6.9586F, 0.869825F, 2.80751F, 5.00751F, 4.34913F, 0.869825F, 1.55563F, 5.95564F, 4.51184F, 0.902369F, 0.0F, 6.6F, 5.0F, 1.0F,
        -4.6669F, 4.6669F, 3.53553F, 0.707107F, -6.6F, 4.0412e-016F, 5.0F, 1.0F, -4.6669F, -4.6669F, 3.53553F, 0.707107F, -8.0824e-016F, -6.6F, 5.0F, 1.0F, 4.6669F, -4.6669F, 3.53553F,
        0.707107F, 6.6F, -1.21236e-015F, 5.0F, 1.0F, 5.31127F, 3.11127F, 4.02369F, 0.804738F, 2.80751F, 5.00751F, 4.34913F, 0.869825F, 2.59482F, 4.62816F, 2.43551F, 0.869825F,
        1.43778F, 5.50445F, 2.52663F, 0.902369F, 0.0F, 6.1F, 2.8F, 1.0F, -4.31335F, 4.31335F, 1.9799F, 0.707107F, -6.1F, 3.73505e-016F, 2.8F, 1.0F, -4.31335F, -4.31335F, 1.9799F, 0.707107F,
        -7.4701e-016F, -6.1F, 2.8F, 1.0F, 4.31335F, -4.31335F, 1.9799F, 0.707107F, 6.1F, -1.12051e-015F, 2.8F, 1.0F, 4.9089F, 2.87557F, 2.25327F, 0.804738F, 2.59482F, 4.62816F, 2.43551F,
        0.869825F, 1.99929F, 3.56596F, 0.34793F, 0.869825F, 1.1078F, 4.24113F, 0.360948F, 0.902369F, 0.0F, 4.7F, 0.4F, 1.0F, -3.3234F, 3.3234F, 0.282843F, 0.707107F, -4.7F,
        2.87782e-016F, 0.4F, 1.0F, -3.3234F, -3.3234F, 0.282843F, 0.707107F, -5.75565e-016F, -4.7F, 0.4F, 1.0F, 3.3234F, -3.3234F, 0.282843F, 0.707107F, 4.7F, -8.63347e-016F, 0.4F, 1.0F,
        3.78227F, 2.2156F, 0.321895F, 0.804738F, 1.99929F, 3.56596F, 0.34793F, 0.869825F, 0.0F, 0.0F, 0.173965F, 0.869825F, 0.0F, 0.0F, 0.180474F, 0.902369F, 0.0F, 0.0F, 0.2F, 1.0F, 0.0F, 0.0F, 0.141421F,
        0.707107F, 0.0F, 0.0F, 0.2F, 1.0F, 0.0F, 0.0F, 0.141421F, 0.707107F, 0.0F, 0.0F, 0.2F, 1.0F, 0.0F, 0.0F, 0.141421F, 0.707107F, 0.0F, 0.0F, 0.2F, 1.0F, 0.0F, 0.0F, 0.160948F, 0.804738F, 0.0F, 0.0F, 0.173965F, 0.869825F
    };
	
    RtFloat uknot[] = {0.0F, 0.0F, 0.0F, 0.0833333F, 0.0833333F, 0.333333F, 0.333333F, 0.583333F, 0.583333F, 0.833333F, 0.833333F, 1.0F, 1.0F, 1.0F};
    RtFloat vknot[] = {0.0F, 0.0F, 0.0F, 0.0555556F, 0.111111F, 0.166667F, 0.222222F, 0.277778F, 0.333333F, 0.388889F, 0.444444F, 0.5F, 0.555556F, 0.611111F, 0.666667F, 0.722222F, 0.777778F, 0.833333F, 0.888889F, 0.944444F, 1.0F, 1.0F, 1.0F};
	
	RiAttributeBegin(); {
		RiColor(redish);
		RiSides(1);
		RiTranslate(0, -.3F, 2.75F);
		RiRotate(-90, 1, 0, 0);
		RiScale(.1F, .1F, .1F);
		RiNuPatch(11, 3, uknot, 0.0F, 1.0F, 20, 3, vknot, 0.0F, 1.0F, RI_PW, vaseMesh, RI_NULL);
	} RiAttributeEnd();
}


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
		RiGeometricApproximation(RI_TESSELATION, 32);
		RiObjectInstance(handle);
	} RiAttributeEnd();
}

void testCone()
{
	const RtFloat rad = 0.6F;
	const RtFloat height = 1.6F;
	RiAttributeBegin(); {
		RiSides(2);
		RiOrientation(RI_OUTSIDE);
		RiColor(blueish);
		RiTransformBegin(); {
			RiTranslate(0, -height/4.0F, 3);
			RiScale(1.0, 1.0, 1.0);
			RiRotate(-45, 1, 0, 0);
			RiCone(height, rad, 360, RI_NULL);
		} RiTransformEnd();
	} RiAttributeEnd();
}

void testPolygon()
{
	RiAttributeBegin(); {
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
		RtFloat color[12] = {
			0, 0, 0,
			1, 0, 0,
			0, 1, 0,
			0, 0, 1
		};
		RiTranslate(0, 0, 3);
		RiPolygon(4, RI_P, p, RI_ST, st, RI_CS, color, RI_NULL);
	} RiAttributeEnd();
}
	
void testPoly10()
{
	RtFloat p[] = {
		// outer
		-.5, -.5, 1, //  1
		-.5,  .5, 1, //  2
		.5,  .5, 1, //  3
		.5, -.5, 1, //  4
		0
	};
	RtInt nloops[] = {
		1
	};
	RtInt nverts[] = {
		4
	};
	RtInt verts[] = {
		0, 1, 2, 3
	};

	// Disable warnings (variable not used)
	opacity_25[0] = opacity_25[0]+0;
	opacity_50[0] = opacity_50[0]+0;
	opacity_75[0] = opacity_75[0]+0;

	RiAttributeBegin(); {
		RiTranslate(0, 0, 1.5);
		RiAttributeBegin(); {
			RiRotate(15.0, 0, 0, 1);
			RiOpacity(opacity_50);
			RiColor(greenish);

			// RiGeneralPolygon(nloops[0], nverts, RI_P, &p, RI_NULL);
			RiPolygon(4, RI_P, &p, RI_NULL);
			// RiPointsPolygons(1, nverts, verts, RI_P, &p, RI_NULL);
			// RiPointsGeneralPolygons(1, nloops, nverts, verts, RI_P, &p, RI_NULL);
	
#   ifdef _TRACE
			{
				RtInt i;
				RiTransformPoints(RI_CURRENT, RI_RASTER, sizeof(p)/sizeof(RtPoint), (RtPoint *)p);
				for ( i = 0; i < 4; ++i ) {
					printf("x %f, y %f, z %f\n", p[i*3+0], p[i*3+1], p[i*3+2]);
				}
				printf("Inverse\n");
				RiTransformPoints(RI_RASTER, RI_CURRENT, sizeof(p)/sizeof(RtPoint), (RtPoint *)p);
				for ( i = 0; i < 4; ++i ) {
					printf("x %f, y %f, z %f\n", p[i*3+0], p[i*3+1], p[i*3+2]);
				}
			}
#   endif
		} RiAttributeEnd();
	
		RiAttributeBegin(); {
			RiTranslate(0, 0, 0.2F);
			// RiOpacity(opacity_50);
			RiColor(redish);

			// RiGeneralPolygon(nloops[0], nverts, RI_P, &p, RI_NULL);
			// RiPolygon(4, RI_P, &p, RI_NULL);
			RiPointsPolygons(1, nverts, verts, RI_P, &p, RI_NULL);
			// RiPointsGeneralPolygons(1, nloops, nverts, verts, RI_P, &p, RI_NULL);
			
		} RiAttributeEnd();

		RiAttributeBegin(); {
			RiRotate(-15.0F, 0, 0, 1);
			RiTranslate(0, 0, 0.3F);
			RiOpacity(opacity_75);
			RiColor(blueish);

			// RiGeneralPolygon(nloops[0], nverts, RI_P, &p, RI_NULL);
			// RiPolygon(4, RI_P, &p, RI_NULL);
			// RiPointsPolygons(1, nverts, verts, RI_P, &p, RI_NULL);
			RiPointsGeneralPolygons(1, nloops, nverts, verts, RI_P, &p, RI_NULL);
			
		} RiAttributeEnd();

	} RiAttributeEnd();
}

void testScene(void)
{
	RiArchiveBegin("RIBARCHIVE", RI_NULL); {
		RiClipping(0.1F, 50.0F);
		RiWorldBegin(); {
			// testPolygon();
			// testCone();
			testTeapot();
			// testVase();
			// testPoly10();
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
	const char *screenAction[2] = {"clear", "finish"};
#ifdef _DEBUG
	RtInt noYes[2] = {0, 1};
#endif

	const char *matrixName[] = {"pre-camera"};
	RiIdentity();
	RiTranslate(0.0F,0.0F,sdepth); // Move back and forth
	RiTranslate(0, 0, 2.75); // Move back to previous pos
			
	// Rotation
	RiRotate(stheta, 1.0, 0.0, 0.0); // Rotate x
	RiRotate(-sphi, 0.0, 1.0, 0.0); // Rotate y
			
	RiTranslate(0, 0, -2.75); // Move to a pivot
	RiCPPControl("state", "string store-transform", matrixName, RI_NULL); // Candidate for RiResource
	RiIdentity();

	// RiCPPControl("rib", "cache-file-archives", &noYes[1], RI_NULL);
	RiOption("glrenderer", RI_DISPXRES, &width, RI_DISPYRES, &height, RI_NULL);
	RiCPPControl("glrenderer", "screen", &screenAction[0], RI_NULL);
#ifdef _DEBUG
	RiAttribute("glrenderer", "draw-normals", &noYes[1], RI_NULL);
#endif
	    
	RiFormat(width, height, 1.0F);

	if ( storedArgc == 1 ) {
		RtFloat fov = 45.0;
		RiProjection(RI_PERSPECTIVE, RI_FOV, &fov, RI_NULL);
	}
	
	RiReadArchive("RIBARCHIVE", 0, RI_NULL);
	
	RiCPPControl("glrenderer", "screen", &screenAction[1], RI_NULL);
	// RiIdentity(); // done by restart

	// If double buffer
	glutSwapBuffers();

	RiSynchronize("restart");
}

void reshape(int aWidth, int aHeight)
{
	width = aWidth;
	height = aHeight;
    // glutPostRedisplay();
}

// Copied from GLUT newave.c
void motion(int x, int y)
{
    if (leftButton)
    {
        sphi += (float)(x - downX) / 4.0F;
        stheta += (float)(downY - y) / 4.0F;
    }
    if (middleButton)
    {
        sdepth += (float)(downY - y) / 10.0F;
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
    rightButton = ((button == GLUT_RIGHT_BUTTON) && 
                    (state == GLUT_DOWN));
	
	if ( rightButton ) {
        sphi = 0;
        stheta = 0;
        sdepth = 0;
		glutPostRedisplay();
	}
}

int main(int argc, char **argv)
{
	// const char *searchPath = ".:&";
  	// RiCPPControl("searchpath", "renderer", &searchPath, RI_NULL);

	storedArgc = argc;
	storedArgv = argv;
	
	glutInit(&argc, argv);
	glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH );
	glutInitWindowSize (width, height); 
	glutInitWindowPosition (100, 100);
	glutCreateWindow(argc <= 1 ? "GLUT RIB" : argv[1]);
	
   	glutDisplayFunc(display); 
	glutReshapeFunc(reshape);
	glutMouseFunc(mouse);
	glutMotionFunc(motion);

	// std::cerr << "BEGIN" << std::endl;
	
	// Print errors
	RiErrorHandler(RiErrorPrint);

	RiBegin("glrenderer"); {
		if ( argc <= 1 ) {
			testScene();
		} else {
			loadScene(argv[1]);
		}
		// std::cerr << "MainLoop" << std::endl;
		glutReshapeWindow(width, height); // Hack, display did not work on my laptop until window was resized...
		glutMainLoop();
	} RiEnd();
	
	// std::cerr << "END" << std::endl;

	return 0;
}
