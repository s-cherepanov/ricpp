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
#import <3Dkit/G3DShape.h>

@interface G3DLight : G3DShape
{
	RtLightHandle lightHandle;
	G3DLightType  type;
	RtPoint       from, to;
	NSColor       *color;
	RtFloat       intensity, coneAnge, coneDelta, beamDistribution;
	struct {
		unsigned int global : 1; /* turned on by G3DCamera */
		unsigned int on : 1;
	} lightFlags;

@private
	RtColor _rmColor;
	// void *_G3Dlprivate;
}

- init;

/* These next 4 methods are shorthand methods for the build-in light type;
   they only affect the values appropriate for the particular light. */
- makeAmbientWithIntensity:(RtFloat)i;
- makePointFrom:(RtPoint)pf intensity:(RtFloat)i;
- makeDistantFrom:(RtPoint)pf to:(RtPoint)pt intensity:(RtFloat)i;
- makeSpotFrom:(RtPoint)pf to:(RtPoint)pt
		  coneAngle:(RtFloat)ca coneDelta:(RtFloat)cd
          beamdistribution:(RtFloat)bd
          intensity:(RtFloat)i;

- renderSelf:(N3DCamera *)camera;
- renderGlobal:(N3DCamera *)camera;

- setGlobal:(BOOL)flag;
- (BOOL)isGlobal;
- switchLight:(BOOL)onOff;
- setType:(G3DLightType)aType;
- (G3DLightType)type;
- setFrom:(RtPoint)pf;
- setFrom:(RtPoint)pf to:(RtPoint)pt;
- getFrom:(RtPoint *)pf to:(RtPoint *)pt;
- setColor:(NSColor *)c;
- (NSColor *)color;
- setIntensity:(RtFloat)i;
- (RtFloat)intensity;
- setConeAngle:(RtFloat)ca coneDelta:(RtFloat)cd beamdistribution:(RtFloat)bd;
- getConeAngle:(RtFloat *)ca coneDelta:(RtFloat *)cd beamdistribution:(RtFloat *)bd;

- awake;
- read:(NSCoder *)stream;
- write:(NSCoder *)stream;

@end
