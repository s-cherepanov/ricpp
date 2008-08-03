#import "MyOpenGLView.h"#import "MyNSObjectController.h"#import "ricpp/ri/ri.h"#include <OpenGL/gl.h>#include <OpenGL/OpenGL.h>#import <stdio.h>#ifdef _DEBUG// #define _TRACE#endifstatic const int startScreen = 11; // 0-maxScreensstatic const int maxScreens = 13;// static RtFloat opacity_25[] = {0.25,  0.25, 0.25};static RtFloat opacity_50[] = {0.5,  0.5, 0.5};static RtFloat opacity_75[] = {0.75,  0.75, 0.75};static RtFloat greenish[] = {0.5,  1, 0.5};static RtFloat redish[] = {1,  0.5, 0.5};static RtFloat blueish[] = {0.5,  0.5, 1};void testCylinder(){	const RtFloat rad = 0.8;	RiAttributeBegin();	RiSides(2);	RiOrientation(RI_OUTSIDE);	// RiOpacity(opacity_25);	RiColor(blueish);	RiTransformBegin();	RiTranslate(0, 0, 3);	RiScale(1.0, 1.0, 1.0);	RiRotate(-90, 1, 0, 0);	RiCylinder(rad, -rad, rad, 360, RI_NULL);	RiTransformEnd();	RiAttributeEnd();}void testParaboloid(){	const RtFloat rad = 0.8;	RiAttributeBegin();	RiSides(1);	// RiOpacity(opacity_25);	RiColor(blueish);	RiTransformBegin();	RiTranslate(0, 0, 1.5);	// RiOrientation(RI_OUTSIDE);	// RiTranslate(0, rad, 1.0+rad);	// RiTranslate(0, -rad, 1.0+rad);	// RiTranslate(0, 0, 1);	// RiRotate(180, 0, 1, 0);	// RiRotate(-90, 1, 0, 0);	RiRotate(45, 0, 1, 0);	RiTranslate(-1.0, 0, 0);	RiScale(1.0, 1.0, 1.0);	RiParaboloid(rad, 0, rad*2.0, 360, RI_NULL);	RiTransformEnd();	RiAttributeEnd();}void testSphere(){	const RtFloat rad = 0.8;	RiAttributeBegin();	RiSides(1);	RiOrientation(RI_OUTSIDE);	// RiOpacity(opacity_25);	RiColor(blueish);	RiTransformBegin();	RiTranslate(0, 0, 3);	RiRotate(45, 0, 1, 0);	RiScale(1.0, 1.0, 1.0);	RiSphere(rad, -rad, rad, 360, RI_NULL);	RiTransformEnd();	RiAttributeEnd();}void testPolyHole10(){	RtFloat p[] = {		// outer		-.5, -.5, 1, //  1		-.5,  .5, 1, //  2		.5,  .5, 1, //  3		.5, -.5, 1, //  4		0	};	RtInt nloops[] = {		1	};	RtInt nverts[] = {		4	};	RtInt verts[] = {		0, 1, 2, 3	};	RiAttributeBegin();	RiAttributeBegin();	RiRotate(15.0, 0, 0, 1);	RiOpacity(opacity_50);	RiColor(greenish);	// RiGeneralPolygon(nloops[0], nverts, RI_P, &p, RI_NULL);	RiPolygon(4, RI_P, &p, RI_NULL);	// RiPointsPolygons(1, nverts, verts, RI_P, &p, RI_NULL);	// RiPointsGeneralPolygons(1, nloops, nverts, verts, RI_P, &p, RI_NULL);	#   ifdef _TRACE	{		RtInt i;		RiTransformPoints(RI_CURRENT, RI_RASTER, sizeof(p)/sizeof(RtPoint), (RtPoint *)p);		for ( i = 0; i < 4; ++i ) {			printf("x %f, y %f, z %f\n", p[i*3+0], p[i*3+1], p[i*3+2]);		}		printf("Inverse\n");		RiTransformPoints(RI_RASTER, RI_CURRENT, sizeof(p)/sizeof(RtPoint), (RtPoint *)p);		for ( i = 0; i < 4; ++i ) {			printf("x %f, y %f, z %f\n", p[i*3+0], p[i*3+1], p[i*3+2]);		}	}#   endif	RiAttributeEnd();		RiAttributeBegin();	RiTranslate(0, 0, 0.2);	// RiOpacity(opacity_50);	RiColor(redish);	// RiGeneralPolygon(nloops[0], nverts, RI_P, &p, RI_NULL);	// RiPolygon(4, RI_P, &p, RI_NULL);	RiPointsPolygons(1, nverts, verts, RI_P, &p, RI_NULL);	// RiPointsGeneralPolygons(1, nloops, nverts, verts, RI_P, &p, RI_NULL);		RiAttributeEnd();	RiAttributeBegin();	RiRotate(-15.0, 0, 0, 1);	RiTranslate(0, 0, 0.3);	RiOpacity(opacity_75);	RiColor(blueish);	// RiGeneralPolygon(nloops[0], nverts, RI_P, &p, RI_NULL);	// RiPolygon(4, RI_P, &p, RI_NULL);	// RiPointsPolygons(1, nverts, verts, RI_P, &p, RI_NULL);	RiPointsGeneralPolygons(1, nloops, nverts, verts, RI_P, &p, RI_NULL);		RiAttributeEnd();	RiAttributeEnd();}void testPolyHole9(){	RtFloat p[] = {		// outer		0, 0, 1, //  1		4, 0, 1, //  2		4, 4, 1, //  3		0, 4, 1, //  4				//  5		//  6		2, 0, 1, //  7		4, 0, 1, //  8		4, 4, 1, //  9		2, 4, 1, // 10				// 11		// 12				1, 1, 1, // 13		2, 1, 1, // 14		2, 3, 1, // 15		1, 3, 1, // 16				0	};		RtInt nverts[] = {		4,		4,		4	};		RtInt i,j,n,nloops;		nloops = sizeof(nverts)/sizeof(nverts[0]);		for ( j = 0, n = 0; j < nloops; ++j ) {		for ( i = 0; i < nverts[j]; ++i ) {			p[n+i*3+0] = p[n+i*3+0] / 8.0 - 0.5;			p[n+i*3+1] = p[n+i*3+1] / 8.0 - 0.5;			p[n+i*3+2] = p[n+i*3+2];		}		n += nverts[j]*3;	}		RiAttributeBegin();	RiOrientation(RI_RH);	RiGeneralPolygon(nloops, nverts, RI_P, &p, RI_NULL);	RiAttributeEnd();}void testPolyHole8(){	// orientation is rh on purpose	RtFloat p[] = {		// outer		0,   1, 1, //  1		2,   4, 1, //  2		4,   1, 1, //  3		2.5, 1, 1, //  4		3,   0, 1, //  5		1,   0, 1, //  6		1.5, 1, 1, //  7		2.5, 1, 1, //  8		2,   2, 1, //  9		1.5, 1, 1, // 10				0	};		RtInt nverts[] = {		10	};		RtInt i,j,n,nloops;		nloops = sizeof(nverts)/sizeof(nverts[0]);		for ( j = 0, n = 0; j < nloops; ++j ) {		for ( i = 0; i < nverts[j]; ++i ) {			p[n+i*3+0] = p[n+i*3+0] / 8.0 - 0.5;			p[n+i*3+1] = p[n+i*3+1] / 8.0 - 0.5;			p[n+i*3+2] = p[n+i*3+2];		}		n += nverts[j]*3;	}		RiGeneralPolygon(nloops, nverts, RI_P, &p, RI_NULL);}void testPolyHole7(){	RtFloat p[] = {		// outer		0,   0,   1, //  1				0,   2,   1, //  2		1,   1.5, 1, //  3		1,   2.5, 1, //  4		0,   2,   1, //  5				0,   5,   1, //  6		3,   5,   1, //  7				3,   3,   1, //  8		2,   3.5, 1, //  9		2,   2.5, 1, // 10		3,   3,   1, // 11				3,   2,   1, // 12		2,   2,   1, // 13		2,   1,   1, // 14		3,   1,   1, // 15		3,   2,   1, // 16				5,   2,   1, // 17		4.5, 1,   1, // 18		5.5, 1,   1, // 19		5,   2,   1, // 20				7,   2,   1, // 21		7,   0,   1, // 22				6,   0,   1, // 23		6.5, 0.5, 1, // 24		5.5, 0.5, 1, // 25		6,   0,   1, // 26				0	};		RtInt nverts[] = {		26	};		RtInt i,j,n,nloops;		nloops = sizeof(nverts)/sizeof(nverts[0]);		for ( j = 0, n = 0; j < nloops; ++j ) {		for ( i = 0; i < nverts[j]; ++i ) {			p[n+i*3+0] = p[n+i*3+0] / 8.0 - 0.5;			p[n+i*3+1] = p[n+i*3+1] / 8.0 - 0.5;			p[n+i*3+2] = p[n+i*3+2];		}		n += nverts[j]*3;	}		RiGeneralPolygon(nloops, nverts, RI_P, &p, RI_NULL);}void testPolyHole6(){	RtFloat p[] = {		// outer		0, 0, 1, //  1		0, 5, 1, //  2		3, 5, 1, //  3		3, 2, 1, //  4		7, 2, 1, //  5		7, 0, 1, //  6				//  7		//  8				2, 2.5, 1, //  9		2, 3.5, 1, // 10		3, 3,   1, // 11				// 12		// 13				2, 1, 1, // 14		2, 2, 1, // 15		3, 2, 1, // 16		3, 1, 1, // 17				// 18		// 19				4.5, 1, 1, // 20		5.5, 1, 1, // 21		5,   2, 1, // 22				0	};		RtInt nverts[] = {		6,		3,		4,		3	};		RtInt i,j,n,nloops;		nloops = sizeof(nverts)/sizeof(nverts[0]);		for ( j = 0, n = 0; j < nloops; ++j ) {		for ( i = 0; i < nverts[j]; ++i ) {			p[n+i*3+0] = p[n+i*3+0] / 8.0 - 0.5;			p[n+i*3+1] = p[n+i*3+1] / 8.0 - 0.5;			p[n+i*3+2] = p[n+i*3+2];		}		n += nverts[j]*3;	}		RiGeneralPolygon(nloops, nverts, RI_P, &p, RI_NULL);}void testPolyHole5(){	RtFloat p[] = {		// outer		-.5, -.5, 1, //  1		-.5,   0, 1, //  2		-.5,  .5, 1, //  3		 .5,  .5, 1, //  4		 .5, -.5, 1, //  5		 0	};		RtInt nverts[] = {		5	};		RtInt i,j,n,nloops;		nloops = sizeof(nverts)/sizeof(nverts[0]);		for ( j = 0, n = 0; j < nloops; ++j ) {		for ( i = 0; i < nverts[j]; ++i ) {			p[n+i*3+0] = p[n+i*3+0];			p[n+i*3+1] = p[n+i*3+1];			p[n+i*3+2] = p[n+i*3+2];		}		n += nverts[j]*3;	}		RiGeneralPolygon(nloops, nverts, RI_P, &p, RI_NULL);}void testPolyHole4(){	RtFloat p[] = {		// outer		0, 0, 1, //  1		0, 6, 1, //  2		6, 6, 1, //  3		6, 0, 1, //  4				//  5		//  6				// inner (hole)		1, 1, 1,  //  7		3, 1, 1,  //  8		3, 3, 1,  //  9		1, 3, 1,  // 10				//  11		//  12				// inner (hole)		3, 3, 1,  // 13		5, 3, 1,  // 14		5, 5, 1,  // 15		3, 5, 1,  // 16				0	};		RtInt nverts[] = {		4,		4,		4	};		RtInt i,j,n,nloops;		nloops = sizeof(nverts)/sizeof(nverts[0]);		for ( j = 0, n = 0; j < nloops; ++j ) {		for ( i = 0; i < nverts[j]; ++i ) {			p[n+i*3+0] = p[n+i*3+0] / 8.0 - 0.5;			p[n+i*3+1] = p[n+i*3+1] / 8.0 - 0.5;			p[n+i*3+2] = p[n+i*3+2];		}		n += nverts[j]*3;	}		RiGeneralPolygon(nloops, nverts, RI_P, &p, RI_NULL);}void testPolyHole3(){	RtFloat p[] = {		// outer		0, 0, 1, //  1		0, 6, 1, //  2		7, 6, 1, //  3		7, 0, 1, //  4				//  5		//  6				// inner (hole)		1, 1, 1,  //  7		3, 1, 1,  //  8		3, 3, 1,  //  9		1, 3, 1,  // 10				//  11		//  12				// inner (hole)		4, 3, 1,  // 13		6, 3, 1,  // 14		6, 5, 1,  // 15		4, 5, 1,  // 16				0	};		RtInt nverts[] = {		4,		4,		4	};		RtInt i,j,n,nloops;		nloops = sizeof(nverts)/sizeof(nverts[0]);		for ( j = 0, n = 0; j < nloops; ++j ) {		for ( i = 0; i < nverts[j]; ++i ) {			p[n+i*3+0] = p[n+i*3+0] / 8.0 - 0.5;			p[n+i*3+1] = p[n+i*3+1] / 8.0 - 0.5;			p[n+i*3+2] = p[n+i*3+2];		}		n += nverts[j]*3;	}		RiGeneralPolygon(nloops, nverts, RI_P, &p, RI_NULL);}void testPolyHole2(){	RtFloat p[] = {		// outer		0, 0, 1, //  1		0, 7, 1, //  2		7, 7, 1, //  3		7, 0, 1, //  4				//  5		//  6				// inner (hole)		1, 1, 1,  //  7		3, 1, 1,  //  8		3, 3, 1,  //  9		1, 3, 1,  // 10				//  11		//  12				// inner (hole)		4, 1, 1,  // 13		6, 1, 1,  // 14		6, 3, 1,  // 15		4, 3, 1,  // 16				//  17		//  18				// inner (hole)		4, 4, 1,  // 19		6, 4, 1,  // 20		6, 6, 1,  // 21		4, 6, 1,  // 22				//  23		//  24				// inner (hole)		1, 4, 1,  //  7		3, 4, 1,  //  8		3, 6, 1,  //  9		1, 6, 1,  // 10				0	};		RtInt nverts[] = {		4,		4,		4,		4,		4	};		RtInt i,j,n,nloops;		nloops = sizeof(nverts)/sizeof(nverts[0]);		for ( j = 0, n = 0; j < nloops; ++j ) {		for ( i = 0; i < nverts[j]; ++i ) {			p[n+i*3+0] = p[n+i*3+0] / 8.0 - 0.5;			p[n+i*3+1] = p[n+i*3+1] / 8.0 - 0.5;			p[n+i*3+2] = p[n+i*3+2];		}		n += nverts[j]*3;	}		RiGeneralPolygon(nloops, nverts, RI_P, &p, RI_NULL);}void testPolyHole(){	RtFloat p[] = {		// outer		1, 1, 1, // 0		1, 8, 1, // 1		4, 8, 1, // 2		5, 6, 1, // 3		6, 8, 1, // 4		7, 8, 1, // 5				// inner (hole)		2, 5,   1,  // 6		2, 6,   1,  // 7		3, 5.5, 1,  // 8				// inner (hole)		2, 3,   1,  //  9		2, 4,   1,  // 10		3, 3.5, 1,  // 11				0	};		RtInt nverts[] = {		6,		3,		3	};		RtInt i,j,n,nloops;		nloops = sizeof(nverts)/sizeof(nverts[0]);		for ( j = 0, n = 0; j < nloops; ++j ) {		for ( i = 0; i < nverts[j]; ++i ) {			p[n+i*3+0] = p[n+i*3+0] / 8.0 - 0.5;			p[n+i*3+1] = p[n+i*3+1] / 8.0 - 0.5;			p[n+i*3+2] = p[n+i*3+2];		}		n += nverts[j]*3;	}		RiGeneralPolygon(nloops, nverts, RI_P, &p, RI_NULL);}void testPoly(){	// O'Rourke's poly 1.1	// orientation is rh on purpose	RtFloat p[] = {		// outer		0,   0, 1, //  0		10,  7, 1, //  1		12,  3, 1, //  2		20,  8, 1, //  3		13, 17, 1, //  4		10, 12, 1, //  5		12, 14, 1, //  6		14,  9, 1, //  7		8,  10, 1, //  8		6,  14, 1, //  9		10, 15, 1, // 10		7,  18, 1, // 11		0,  16, 1, // 12		1,  13, 1, // 13		3,  15, 1, // 14		5,   8, 1, // 15		-2,  9, 1, // 16		5,   5, 1  // 17	};		RtInt nverts[] = {		18	};		RtInt i,j,n,nloops;		nloops = sizeof(nverts)/sizeof(nverts[0]);		for ( j = 0, n = 0; j < nloops; ++j ) {		for ( i = 0; i < nverts[0]; ++i ) {			p[n+i*3+0] = p[n+i*3+0] / 18.0 - 0.5;			p[n+i*3+1] = p[n+i*3+1] / 18.0 - 0.5;			p[n+i*3+2] = p[n+i*3+2];		}		n += nverts[j]*3;	}		RiReverseOrientation();	RiGeneralPolygon(nloops, nverts, RI_P, &p, RI_NULL);	RiReverseOrientation();}static void drawAnObject (){		// Opaque	glColor4f(0.0f, 0.35f, 0.0f, 1.0f);	glBegin(GL_TRIANGLES);	{		glVertex3f( 0.1, 0.5, -0.15);		glVertex3f( -0.1, -0.3, -0.15);		glVertex3f( 0.3, -0.3 , 0.3);	}	glEnd();		// Transparent from far to near		glColor4f(1.0f, 0.85f, 0.35f, 0.5f);	glBegin(GL_TRIANGLES);	{		glVertex3f(  0, 0.5, -0.1);		glVertex3f( -0.5, -0.3, -0.1);		glVertex3f( 0.5, -0.3 , 0.1);	}	glEnd();		glColor4f(0.0f, 0.0f, 0.35f, 0.5f);	glBegin(GL_TRIANGLES);	{		glVertex3f( 0.3, 0.5, -0.3);		glVertex3f( 0.1, -0.3, -0.3);		glVertex3f( 0.5, -0.3 , 0.15);	}	glEnd();}@implementation MyOpenGLView-(void) awakeFromNib{	/* NSLog(@"awakeFromNib"); */	counter = startScreen;}-(void) drawRect: (NSRect) bounds{	/* NSLog(@"drawRect"); */	/* Clear Screen (because of possible multiple viewports) - Should make a RiControl to hide this */	glDisable(GL_SCISSOR_TEST);	glClearColor(0.5, 0.5, 1.0, 0);	glClear(GL_COLOR_BUFFER_BIT);	glFlush();	/* *** */		if ( counter < 0 ) 		counter = maxScreens;	else if ( counter > maxScreens )		counter = 0;		RtInt width = (RtInt)bounds.size.width;	RtInt height = (RtInt)bounds.size.height;	RtFloat pixelAspect = (RtFloat)1.0;		RtInt origin[2] = {0, 0};		RiFrameBegin(counter);	// Set the display spec	if ( [(MyNSObjectController *)panelController displayEnabled] ) {		origin[0] = (RtFloat)[(MyNSObjectController *)panelController originX];		origin[1] = (RtFloat)[(MyNSObjectController *)panelController originY];#ifdef _TRACE		printf("originX %f, originY %f\n", origin[0], origin[1]);#endif		RiDisplay("OpenGL Demo", RI_FRAMEBUFFER, RI_RGBA,				  RI_DISPXRES, &width, RI_DISPYRES, &height, RI_PIXELASPECT, &pixelAspect,				  RI_ORIGIN, origin, RI_NULL);	} else {		[(MyNSObjectController *)panelController displayOriginX:origin[0] originY:origin[1]];		RiDisplay("OpenGL Demo", RI_FRAMEBUFFER, RI_RGBA,				  RI_DISPXRES, &width, RI_DISPYRES, &height, RI_PIXELASPECT, &pixelAspect,				  RI_ORIGIN, origin, RI_NULL);	}		width -= origin[0];	height -= origin[1];	#ifdef _TRACE	printf("Bounds width %f, height %f\n", bounds.size.width, bounds.size.height);#endif		// Raster format	if ( [(MyNSObjectController *)panelController formatEnabled] ) {		width = (RtInt)[(MyNSObjectController *)panelController formatWidth];		height = (RtInt)[(MyNSObjectController *)panelController formatHeight];		pixelAspect = (RtFloat)[(MyNSObjectController *)panelController formatPixelAspect];		RiFormat(width, height, pixelAspect);	} else {		[(MyNSObjectController *)panelController formatWidth:width height:height pixelAspect:pixelAspect];		// default: frame size = display size		// RiFormat(width, height, pixelAspect);	}		// Frame aspect ratio	RtFloat frameAspect = (RtFloat)width*pixelAspect/(RtFloat)height;	if ( [(MyNSObjectController *)panelController frameAspectEnabled] ) {		frameAspect = (RtFloat)[(MyNSObjectController *)panelController frameAspect];		RiFrameAspectRatio((RtFloat)frameAspect);	} else {		[(MyNSObjectController *)panelController frameAspect:frameAspect];		// That's the default		// RiFrameAspectRatio(frameAspect);	}	// Screen Window	RtFloat screenLeft, screenRight, screenBottom, screenTop;	if ( frameAspect < 1 ) {		screenLeft = -1.0;		screenRight = 1.0;		screenBottom = 1.0/-frameAspect;		screenTop = 1.0/frameAspect;	} else {		screenLeft = -frameAspect;		screenRight = frameAspect;		screenBottom = -1.0;		screenTop = 1.0;	}	if ( [(MyNSObjectController *)panelController screenEnabled] ) {		screenLeft = (RtFloat)[(MyNSObjectController *)panelController screenLeft];		screenRight = (RtFloat)[(MyNSObjectController *)panelController screenRight];		screenBottom = (RtFloat)[(MyNSObjectController *)panelController screenBottom];		screenTop = (RtFloat)[(MyNSObjectController *)panelController screenTop];		RiScreenWindow(screenLeft, screenRight, screenBottom, screenTop);	} else {		[(MyNSObjectController *)panelController screenLeft:screenLeft right:screenRight bottom:screenBottom top:screenTop];		// That's the default		// RiScreenWindow(screenLeft, screenRight, screenBottom, screenTop);	}		// Clipping	// RtFloat hither = RI_EPSILON, yonder = RI_INFINITY;	RtFloat hither = (RtFloat)0.4, yonder = 4;	if ( [(MyNSObjectController *)panelController clippingEnabled] ) {		hither = (RtFloat)[(MyNSObjectController *)panelController clippingHither];		yonder = (RtFloat)[(MyNSObjectController *)panelController clippingYonder];		RiClipping(hither, yonder);	} else {		[(MyNSObjectController *)panelController clippingHither:hither yonder:yonder];		RiClipping(hither, yonder);	}	// Projection (here: defaults set by controller - maybe preferable)	if ( [(MyNSObjectController *)panelController projectionEnabled] ) {		RtToken proj = [(MyNSObjectController *)panelController projectionType];		if ( proj == RI_PERSPECTIVE ) {			if ( [(MyNSObjectController *)panelController fovEnabled] ) {				RtFloat fov = [(MyNSObjectController *)panelController fov];				RiProjection(RI_PERSPECTIVE, RI_FOV, &fov, RI_NULL);			} else {				// Default fov is 90 degree				RiProjection(RI_PERSPECTIVE, RI_NULL);			}		} else if ( proj == RI_ORTHOGRAPHIC ) {			RiProjection(RI_ORTHOGRAPHIC, RI_NULL);		} else {			// RI_NULL projection			RiProjection(RI_NULL, RI_NULL);		}	} else {		// Defaults automatic, orthographic projection		// RiProjection(RI_ORTHOGRAPHIC, RI_NULL);	}	// Crop	RtFloat xmin=0.0, xmax=1.0, ymin=0.0, ymax=1.0;	if ( [(MyNSObjectController *)panelController cropEnabled] ) {		xmin = (RtFloat)[(MyNSObjectController *)panelController cropLeft];		xmax = (RtFloat)[(MyNSObjectController *)panelController cropRight];		ymax = (RtFloat)[(MyNSObjectController *)panelController cropBottom];		ymin = (RtFloat)[(MyNSObjectController *)panelController cropTop];		RiCropWindow(xmin, xmax, ymin, ymax);	} else {		[(MyNSObjectController *)panelController cropLeft:xmin right:xmax bottom:ymax top:ymin];		// That's the default		// RiCropWindow(xmin, xmax, ymin, ymax);	}		// World Block		RiWorldBegin();		RiSides(1);	RiColor(greenish);		switch ( counter ) {		case 0:			testPoly(); // Simple concave no holes			break;		case 1:			testPolyHole(); // Simple concave with holes			break;		case 2:			testPolyHole2(); // Simple concave with holes (window)			break;		case 3:			testPolyHole3(); // Simple concave with holes (two rectangles)			break;		case 4:			testPolyHole4(); // Simple concave with holes (touching two rectangles)			break;		case 5:			testPolyHole5(); // Simple convex (vertex in a line)			break;		case 6:			testPolyHole6(); // Simple concave with holes (holes touch boundary)			break;		case 7:			testPolyHole7(); // Simple concave no holes (boundary vertices form holes)			break;		case 8:			testPolyHole8(); // Simple concave no holes (boundary vertices form holes and isles)			break;		case 9:			testPolyHole9(); // Simple convex with holes (holes to boundary)			break;		case 10:			testPolyHole10(); // Simple squares with different opacity			break;		case 11: // Cylinder			testCylinder();			break;		case 12: // Paraboloid			testParaboloid();			break;		case 13: // Sphere			testSphere();			break;		default:			drawAnObject(); // GL test			break;	}		RiWorldEnd();	RiFrameEnd();	/* Flush buffer because of possible multi viewport and double buffer - Should make a RiControl to hide this */	CGLContextObj ctx = CGLGetCurrentContext();	if ( ctx != 0 ) {		CGLFlushDrawable(ctx);	}	/* *** */}- (IBAction)nextPoly:(id)sender{	/* NSLog(@"nextPoly"); */	++counter;	[self display];}- (IBAction)prevPoly:(id)sender{	/* NSLog(@"nextPoly"); */	--counter;	[self display];}@end