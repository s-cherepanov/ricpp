#import "MyOpenGLView.h"
#import "ricpp/ri/ri.h"
#import <OpenGL/gl.h>
#import <stdio.h>

#ifdef _DEBUG
#define _TRACE
#endif

void testPolyHole10()
{
	RtFloat p[] = {
		// outer
		-.5, -.5, 1, //  1
		-.5,  .5, 1, //  2
		.5,  .5, 1, //  3
		.5, -.5, 1, //  4
		0
	};
	RtInt nverts[] = {
		4
	};
	RtInt i;
	RtInt nloops = sizeof(nverts)/sizeof(nverts[0]);
	RiTransformBegin();
	RiGeneralPolygon(nloops, nverts, RI_P, &p, RI_NULL);
#   ifdef _TRACE
	RiTransformPoints(RI_OBJECT, RI_RASTER, sizeof(p)/sizeof(RtPoint), (RtPoint *)p);
	for ( i = 0; i < 4; ++i ) {
		printf("x %f, y %f, z %f\n", p[i*3+0], p[i*3+1], p[i*3+2]);
	}
	printf("Inverse\n");
	RiTransformPoints(RI_RASTER, RI_OBJECT, sizeof(p)/sizeof(RtPoint), (RtPoint *)p);
	for ( i = 0; i < 4; ++i ) {
		printf("x %f, y %f, z %f\n", p[i*3+0], p[i*3+1], p[i*3+2]);
	}
#   endif
	RiTransformEnd();
}

void testPolyHole9()
{
	RtFloat p[] = {
		// outer
		0, 0, 0, //  1
		4, 0, 0, //  2
		4, 4, 0, //  3
		0, 4, 0, //  4
		
		//  5
		//  6
		2, 0, 0, //  7
		4, 0, 0, //  8
		4, 4, 0, //  9
		2, 4, 0, // 10
		
		// 11
		// 12
		
		1, 1, 0, // 13
		2, 1, 0, // 14
		2, 3, 0, // 15
		1, 3, 0, // 16
		
		0
	};
	
	RtInt nverts[] = {
		4,
		4,
		4
	};
	
	RtInt i,j,n,nloops;
	
	nloops = sizeof(nverts)/sizeof(nverts[0]);
	
	for ( j = 0, n = 0; j < nloops; ++j ) {
		for ( i = 0; i < nverts[j]; ++i ) {
			p[n+i*3+0] = p[n+i*3+0] / 8.0 - 0.5;
			p[n+i*3+1] = p[n+i*3+1] / 8.0 - 0.5;
			p[n+i*3+2] = p[n+i*3+2] / 8.0 + 0.5;
		}
		n += nverts[j]*3;
	}
	
	RiGeneralPolygon(nloops, nverts, RI_P, &p, RI_NULL);
}

void testPolyHole8()
{
	RtFloat p[] = {
		// outer
		0,   1, 0, //  1
		2,   4, 0, //  2
		4,   1, 0, //  3
		2.5, 1, 0, //  4
		3,   0, 0, //  5
		1,   0, 0, //  6
		1.5, 1, 0, //  7
		2.5, 1, 0, //  8
		2,   2, 0, //  9
		1.5, 1, 0, // 10
		
		0
	};
	
	RtInt nverts[] = {
		10
	};
	
	RtInt i,j,n,nloops;
	
	nloops = sizeof(nverts)/sizeof(nverts[0]);
	
	for ( j = 0, n = 0; j < nloops; ++j ) {
		for ( i = 0; i < nverts[j]; ++i ) {
			p[n+i*3+0] = p[n+i*3+0] / 8.0 - 0.5;
			p[n+i*3+1] = p[n+i*3+1] / 8.0 - 0.5;
			p[n+i*3+2] = p[n+i*3+2] / 8.0 + 0.5;
		}
		n += nverts[j]*3;
	}
	
	RiGeneralPolygon(nloops, nverts, RI_P, &p, RI_NULL);
}

void testPolyHole7()
{
	RtFloat p[] = {
		// outer
		0,   0,   0, //  1
		
		0,   2,   0, //  2
		1,   1.5, 0, //  3
		1,   2.5, 0, //  4
		0,   2,   0, //  5
		
		0,   5,   0, //  6
		3,   5,   0, //  7
		
		3,   3,   0, //  8
		2,   3.5, 0, //  9
		2,   2.5, 0, // 10
		3,   3,   0, // 11
		
		3,   2,   0, // 12
		2,   2,   0, // 13
		2,   1,   0, // 14
		3,   1,   0, // 15
		3,   2,   0, // 16
		
		5,   2,   0, // 17
		4.5, 1,   0, // 18
		5.5, 1,   0, // 19
		5,   2,   0, // 20
		
		7,   2,   0, // 21
		7,   0,   0, // 22
		
		6,   0,   0, // 23
		6.5, 0.5, 0, // 24
		5.5, 0.5, 0, // 25
		6,   0,   0, // 26
		
		0
	};
	
	RtInt nverts[] = {
		26
	};
	
	RtInt i,j,n,nloops;
	
	nloops = sizeof(nverts)/sizeof(nverts[0]);
	
	for ( j = 0, n = 0; j < nloops; ++j ) {
		for ( i = 0; i < nverts[j]; ++i ) {
			p[n+i*3+0] = p[n+i*3+0] / 8.0 - 0.5;
			p[n+i*3+1] = p[n+i*3+1] / 8.0 - 0.5;
			p[n+i*3+2] = p[n+i*3+2] / 8.0 + 0.5;
		}
		n += nverts[j]*3;
	}
	
	RiGeneralPolygon(nloops, nverts, RI_P, &p, RI_NULL);
}

void testPolyHole6()
{
	RtFloat p[] = {
		// outer
		0, 0, 0, //  1
		0, 5, 0, //  2
		3, 5, 0, //  3
		3, 2, 0, //  4
		7, 2, 0, //  5
		7, 0, 0, //  6
		
		//  7
		//  8
		
		2, 2.5, 0, //  9
		2, 3.5, 0, // 10
		3, 3,   0, // 11
		
		// 12
		// 13
		
		2, 1, 0, // 14
		2, 2, 0, // 15
		3, 2, 0, // 16
		3, 1, 0, // 17
		
		// 18
		// 19
		
		4.5, 1, 0, // 20
		5.5, 1, 0, // 21
		5,   2, 0, // 22
		
		0
	};
	
	RtInt nverts[] = {
		6,
		3,
		4,
		3
	};
	
	RtInt i,j,n,nloops;
	
	nloops = sizeof(nverts)/sizeof(nverts[0]);
	
	for ( j = 0, n = 0; j < nloops; ++j ) {
		for ( i = 0; i < nverts[j]; ++i ) {
			p[n+i*3+0] = p[n+i*3+0] / 8.0 - 0.5;
			p[n+i*3+1] = p[n+i*3+1] / 8.0 - 0.5;
			p[n+i*3+2] = p[n+i*3+2] / 8.0 + 0.5;
		}
		n += nverts[j]*3;
	}
	
	RiGeneralPolygon(nloops, nverts, RI_P, &p, RI_NULL);
}

void testPolyHole5()
{
	RtFloat p[] = {
		// outer
		0, 0, 0, //  1
		0, 3, 0, //  2
		0, 6, 0, //  3
		6, 6, 0, //  4
		6, 0, 0, //  5
		
		0
	};
	
	RtInt nverts[] = {
		5
	};
	
	RtInt i,j,n,nloops;
	
	nloops = sizeof(nverts)/sizeof(nverts[0]);
	
	for ( j = 0, n = 0; j < nloops; ++j ) {
		for ( i = 0; i < nverts[j]; ++i ) {
			p[n+i*3+0] = p[n+i*3+0] / 8.0 - 0.5;
			p[n+i*3+1] = p[n+i*3+1] / 8.0 - 0.5;
			p[n+i*3+2] = p[n+i*3+2] / 8.0 + 0.5;
		}
		n += nverts[j]*3;
	}
	
	RiGeneralPolygon(nloops, nverts, RI_P, &p, RI_NULL);
}

void testPolyHole4()
{
	RtFloat p[] = {
		// outer
		0, 0, 0, //  1
		0, 6, 0, //  2
		6, 6, 0, //  3
		6, 0, 0, //  4
		
		//  5
		//  6
		
		// inner (hole)
		1, 1, 0,  //  7
		3, 1, 0,  //  8
		3, 3, 0,  //  9
		1, 3, 0,  // 10
		
		//  11
		//  12
		
		// inner (hole)
		3, 3, 0,  // 13
		5, 3, 0,  // 14
		5, 5, 0,  // 15
		3, 5, 0,  // 16
		
		0
	};
	
	RtInt nverts[] = {
		4,
		4,
		4
	};
	
	RtInt i,j,n,nloops;
	
	nloops = sizeof(nverts)/sizeof(nverts[0]);
	
	for ( j = 0, n = 0; j < nloops; ++j ) {
		for ( i = 0; i < nverts[j]; ++i ) {
			p[n+i*3+0] = p[n+i*3+0] / 8.0 - 0.5;
			p[n+i*3+1] = p[n+i*3+1] / 8.0 - 0.5;
			p[n+i*3+2] = p[n+i*3+2] / 8.0 + 0.5;
		}
		n += nverts[j]*3;
	}
	
	RiGeneralPolygon(nloops, nverts, RI_P, &p, RI_NULL);
}

void testPolyHole3()
{
	RtFloat p[] = {
		// outer
		0, 0, 0, //  1
		0, 6, 0, //  2
		7, 6, 0, //  3
		7, 0, 0, //  4
		
		//  5
		//  6
		
		// inner (hole)
		1, 1, 0,  //  7
		3, 1, 0,  //  8
		3, 3, 0,  //  9
		1, 3, 0,  // 10
		
		//  11
		//  12
		
		// inner (hole)
		4, 3, 0,  // 13
		6, 3, 0,  // 14
		6, 5, 0,  // 15
		4, 5, 0,  // 16
		
		0
	};
	
	RtInt nverts[] = {
		4,
		4,
		4
	};
	
	RtInt i,j,n,nloops;
	
	nloops = sizeof(nverts)/sizeof(nverts[0]);
	
	for ( j = 0, n = 0; j < nloops; ++j ) {
		for ( i = 0; i < nverts[j]; ++i ) {
			p[n+i*3+0] = p[n+i*3+0] / 8.0 - 0.5;
			p[n+i*3+1] = p[n+i*3+1] / 8.0 - 0.5;
			p[n+i*3+2] = p[n+i*3+2] / 8.0 + 0.5;
		}
		n += nverts[j]*3;
	}
	
	RiGeneralPolygon(nloops, nverts, RI_P, &p, RI_NULL);
}

void testPolyHole2()
{
	RtFloat p[] = {
		// outer
		0, 0, 0, //  1
		0, 7, 0, //  2
		7, 7, 0, //  3
		7, 0, 0, //  4
		
		//  5
		//  6
		
		// inner (hole)
		1, 1, 0,  //  7
		3, 1, 0,  //  8
		3, 3, 0,  //  9
		1, 3, 0,  // 10
		
		//  11
		//  12
		
		// inner (hole)
		4, 1, 0,  // 13
		6, 1, 0,  // 14
		6, 3, 0,  // 15
		4, 3, 0,  // 16
		
		//  17
		//  18
		
		// inner (hole)
		4, 4, 0,  // 19
		6, 4, 0,  // 20
		6, 6, 0,  // 21
		4, 6, 0,  // 22
		
		//  23
		//  24
		
		// inner (hole)
		1, 4, 0,  //  7
		3, 4, 0,  //  8
		3, 6, 0,  //  9
		1, 6, 0,  // 10
		
		0
	};
	
	RtInt nverts[] = {
		4,
		4,
		4,
		4,
		4
	};
	
	RtInt i,j,n,nloops;
	
	nloops = sizeof(nverts)/sizeof(nverts[0]);
	
	for ( j = 0, n = 0; j < nloops; ++j ) {
		for ( i = 0; i < nverts[j]; ++i ) {
			p[n+i*3+0] = p[n+i*3+0] / 8.0 - 0.5;
			p[n+i*3+1] = p[n+i*3+1] / 8.0 - 0.5;
			p[n+i*3+2] = p[n+i*3+2] / 8.0 + 0.5;
		}
		n += nverts[j]*3;
	}
	
	RiGeneralPolygon(nloops, nverts, RI_P, &p, RI_NULL);
}

void testPolyHole()
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
		2, 5,   0,  // 6
		2, 6,   0,  // 7
		3, 5.5, 0,  // 8
		
		// inner (hole)
		2, 3,   0,  //  9
		2, 4,   0,  // 10
		3, 3.5, 0,  // 11
		
		0
	};
	
	RtInt nverts[] = {
		6,
		3,
		3
	};
	
	RtInt i,j,n,nloops;
	
	nloops = sizeof(nverts)/sizeof(nverts[0]);
	
	for ( j = 0, n = 0; j < nloops; ++j ) {
		for ( i = 0; i < nverts[j]; ++i ) {
			p[n+i*3+0] = p[n+i*3+0] / 8.0 - 0.5;
			p[n+i*3+1] = p[n+i*3+1] / 8.0 - 0.5;
			p[n+i*3+2] = p[n+i*3+2] / 8.0 + 0.5;
		}
		n += nverts[j]*3;
	}
	
	RiGeneralPolygon(nloops, nverts, RI_P, &p, RI_NULL);
}

void testPoly()
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
	
	RtInt i,j,n,nloops;
	
	nloops = sizeof(nverts)/sizeof(nverts[0]);
	
	for ( j = 0, n = 0; j < nloops; ++j ) {
		for ( i = 0; i < nverts[0]; ++i ) {
			p[n+i*3+0] = p[n+i*3+0] / 18.0 - 0.5;
			p[n+i*3+1] = p[n+i*3+1] / 18.0 - 0.5;
			p[n+i*3+2] = p[n+i*3+2] / 18.0 + 0.5;
		}
		n += nverts[j]*3;
	}
	
	RiGeneralPolygon(nloops, nverts, RI_P, &p, RI_NULL);
}


static void drawAnObject ()
{
	
	// Opaque
	glColor4f(0.0f, 0.35f, 0.0f, 1.0f);
	glBegin(GL_TRIANGLES);
	{
		glVertex3f( 0.1, 0.5, -0.15);
		glVertex3f( -0.1, -0.3, -0.15);
		glVertex3f( 0.3, -0.3 , 0.3);
	}
	glEnd();
	
	// Transparent from far to near
	
	glColor4f(1.0f, 0.85f, 0.35f, 0.5f);
	glBegin(GL_TRIANGLES);
	{
		glVertex3f(  0, 0.5, -0.1);
		glVertex3f( -0.5, -0.3, -0.1);
		glVertex3f( 0.5, -0.3 , 0.1);
	}
	glEnd();
	
	glColor4f(0.0f, 0.0f, 0.35f, 0.5f);
	glBegin(GL_TRIANGLES);
	{
		glVertex3f( 0.3, 0.5, -0.3);
		glVertex3f( 0.1, -0.3, -0.3);
		glVertex3f( 0.5, -0.3 , 0.15);
	}
	glEnd();
}

@implementation MyOpenGLView
-(void) awakeFromNib
{
	/* NSLog(@"awakeFromNib"); */
	counter = 10;
}

-(void) drawRect: (NSRect) bounds
{
	static const int numScreens = 10;
	/* NSLog(@"drawRect"); */
	
	if ( counter < 0 ) 
		counter = numScreens;
	else if ( counter > numScreens )
		counter = 0;
	
	RiFrameBegin(counter);
#ifdef _TRACE
	printf("\nwidth %f, height %f\n", bounds.size.width, bounds.size.height);
#endif
	RiFormat((RtInt)bounds.size.width, (RtInt)bounds.size.height, 1.0);
	RiClipping(0.4, 4.0);
	// RiFrameAspectRatio((RtFloat)(16.0/9.0));
	// RiIdentity();
	// RiProjection(RI_ORTHOGRAPHIC, RI_NULL);
	// RtFloat fov = 90;
	// RiProjection(RI_PERSPECTIVE, RI_FOV, &fov, RI_NULL);
	
	glColor4f(0.0f, 0.35f, 0.0f, 1.0f);
	
	RiWorldBegin();
	
	switch ( counter ) {
		case 0:
			testPoly(); // Simple concave no holes
			break;
		case 1:
			testPolyHole(); // Simple concave with holes
			break;
		case 2:
			testPolyHole2(); // Simple concave with holes (window)
			break;
		case 3:
			testPolyHole3(); // Simple concave with holes (two rectangles)
			break;
		case 4:
			testPolyHole4(); // Simple concave with holes (touching two rectangles)
			break;
		case 5:
			testPolyHole5(); // Simple convex (vertex in a line)
			break;
		case 6:
			testPolyHole6(); // Simple concave with holes (holes touch boundary)
			break;
		case 7:
			testPolyHole7(); // Simple concave no holes (boundary vertices form holes)
			break;
		case 8:
			testPolyHole8(); // Simple concave no holes (boundary vertices form holes and isles)
			break;
		case 9:
			testPolyHole9(); // Simple convex with holes (holes to boundary)
			break;
		case 10:
			testPolyHole10(); // Simple square
			break;
		default:
			drawAnObject(); // GL test
			break;
	}
	
	RiWorldEnd();
	RiFrameEnd();
}

- (IBAction)nextPoly:(id)sender
{
	/* NSLog(@"nextPoly"); */
	++counter;
	[self display];
}

- (IBAction)prevPoly:(id)sender
{
	/* NSLog(@"nextPoly"); */
	--counter;
	[self display];
}

@end
