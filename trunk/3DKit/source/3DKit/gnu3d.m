/*
 *  RiCPP 3dkit based havily on 3dkit and gnu3dkit. It is very experimental. The author tries to get
 *  the (Gnu)3dkit object structure working together with RiCPP. It will be first compiled for Apple's Cocoa and if that works
 *  for GnuStep. The main differences between the 3dkit and ricpp3dkit headers are some adoptions for Cocoa and using name
 *  scheme of G3DKit. 
 *  
 *  3dKit is Copyright (c) 1991, 1992 by NeXT Computer, Inc. All rights reserved.
 *  gnu3dkit is Copyright (c) Philippe C.D. Robert
 *  Adapted for RiCPP Andreas Pidde
 *
 *     RenderMan(R) is a registered trademark of Pixar
 * The RenderMan(R) Interface Procedures and Protocol are:
 *         Copyright 1988, 1989, 2000, 2005 Pixar
 *                 All rights Reservered
 */
#import <3Dkit/gnu3d.h>

void G3DMult3DPoint( RtPoint p, RtMatrix m, RtPoint result )
{
}

void G3DMult3DPoints( RtPoint *p, int count, RtMatrix m, RtPoint *result )
{
}

void G3DIntersectLinePlane( RtPoint *L, RtPoint pnorm, RtPoint ppoint,
						   RtPoint *psect )
{
}

void G3DMultiplyMatrix( RtMatrix a, RtMatrix b, RtMatrix result )
{
}

float G3DInvertMatrix( RtMatrix m, RtMatrix inverse_m )
{
	return 0;
}


NSString *G3DRIBPboardType=0;
const RtPoint G3DOrigin = {(RtFloat)0, (RtFloat)0, (RtFloat)0};
const RtMatrix G3DIdentityMatrix
 = {
	 (RtFloat)1, (RtFloat)0, (RtFloat)0, (RtFloat)0,
	 (RtFloat)0, (RtFloat)1, (RtFloat)0, (RtFloat)0,
	 (RtFloat)0, (RtFloat)0, (RtFloat)1, (RtFloat)0,
	 (RtFloat)0, (RtFloat)0, (RtFloat)0, (RtFloat)1
 };
