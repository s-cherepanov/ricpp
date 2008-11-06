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
#import <3Dkit/G3DCamera.h>
#import <3Dkit/G3DContextManager.h>


@implementation G3DCamera
#if 0
- initFrame:(const NSRect *)fRect;
{
	return self;
}

/* frame and view management methods */
- (BOOL)lockFocus
{
	return NO;
}

- unlockFocus
{
	return self;
}

- drawPS:(NSRect *)rects :(int)nRects
{
	return self;
}

- drawSelf:(const NSRect *)rects :(int)nRects
{
	return self;
}

- worldBegin:(RtToken)context
{
	return self;
}

- worldEnd:(RtToken)context
{
	return self;
}

- copyRIBCode:(NSStream *)stream
{
	return self;
}

- setBackgroundColor:(NSColor *) c
{
	return self;
}

- (NSColor *)backgroundColor
{
	return backgroundColor;
}

- setDrawBackgroundColor:(BOOL)flag
{
	return self;
}

- (BOOL)doesDrawBackgroundColor
{
	return NO;
}

- setFrame:(const NSRect *)fRect
{
	return self;
}

- moveTo:(NXCoord)x :(NXCoord)y
{
	return self;
}

- moveBy:(NXCoord)deltaX :(NXCoord)deltaY
{
	return self;
}

- sizeTo:(NXCoord)width :(NXCoord)height
{
	return self;
}

- sizeBy:(NXCoord)deltaWidth :(NXCoord)deltaHeight
{
	return self;
}

- rotateTo:(NXCoord)angle
{
	return self;
}

- rotateBy:(NXCoord)deltaAngle
{
	return self;
}

- setWorldShape:a3DObject
{
	return self;
}

- worldShape
{
	return self;
}

- addLight:aLight
{
	return self;
}

- removeLight:aLight
{
	return self;
}

- lightList
{
	return self;
}

- setHider:(G3DHider)hider
{
	return self;
}

- (G3DHider)hider
{
	return hider;
}

- setSurfaceTypeForAll:(G3DSurfaceType)surfaceType chooseHider:(BOOL)flag
{
	return self;
}


/* Picking */
- selectShapesIn:(const NSRect *)selectionRect
{
	return self;
}


/* eye vector and viewing frustum manipulation methods */
- setProjectionRectangle:(float)l :(float)r :(float)t :(float)b
{
	return self;
}

- getProjectionRectangle:(float *)lp :(float *)rp :(float *)tp :(float *)bp
{
	return self;
}

- setProjection:(G3DProjectionType)aProjectionType
{
	return self;
}

- (G3DProjectionType)projectionType
{
	return projectionType;
}

- setPreTransformMatrix:(RtMatrix)newPreTM
{
	return self;
}

- getPreTransformMatrix:(RtMatrix)preTM
{
	return self;
}

- setUsePreTransformMatrix:(BOOL)flag
{
	return self;
}

- (BOOL)usesPreTransformMatrix
{
	return NO;
}

- setEyeAt:(RtPoint)fromPoint toward:(RtPoint)toPoint roll:(float)aRollAngle
{
	return self;
}

- getEyeAt:(RtPoint *)anEyePoint toward:(RtPoint *)aViewPoint
	  roll:(float *)aRollAngle
{
	return self;
}

- setClipPlanesNear:(float)aNearPlane far:(float)aFarPlane
{
	return self;
}

- getClipPlanesNear:(float *)aNearPlane far:(float *)aFarPlane
{
	return self;
}

- setFieldOfViewByAngle:(float)aFieldOfView
{
	return self;
}

- setFieldOfViewByFocalLength:(float)aFocalLength
{
	return self;
}

- (float)fieldOfView
{
	return fov;
}

- setPixelAspectRatio:(float)pixAspect;
{
	return self;
}

- (float)pixelAspectRatio
{
	return pixelAspectRatio;
}


- moveEyeBy:(float)ds :(float)dt :(float)du
{
	return self;
}

- rotateEyeBy:(float)dElev :(float)dAzim about:(RtPoint)pivotPtr
{
	return self;
}


- (int)numCropWindows
{
	return 0;
}

- cropInRects:(NSRect *)rects nRects:(int)n
{
	return self;
}

- (int)frameNumber
{
	return 0;
}

- (BOOL)canPrintRIB
{
	return NO;
}


- awake
{
	return self;
}

- read:(NSCoder *)stream
{
	return self;
}

- write:(NSCoder *)stream
{
	return self;
}


- (int)renderAsTIFF
{
	return 0;
}

- (int)renderAsEPS
{
	return 0;
}

- setDelegate:sender
{
	return self;
}

- delegate
{
	return nil;
}
#endif

- init
{
	_contextManager = [G3DContextManager new];
	contextHandle = [_contextManager createContext:0];
	
	projectionType = G3D_Orthographic;
	worldShape = nil;
	lightList = nil;
	delegate = nil;
	backgroundColor = nil;
	hider = G3D_HiddenRendering;

	cameraFlags.degenerate = 0;
	cameraFlags.windowChanged = 0;
	cameraFlags.needsWindow = 0;
	cameraFlags.basisChanged = 0;
	cameraFlags.canRender = 1;
	cameraFlags.usePreTM = 0;
	cameraFlags.doFlush = 1;
	cameraFlags.inWorldBlock = 0;
	cameraFlags.drawBackground = 0;
	cameraFlags._computeAspect = 0;
	cameraFlags._computeSWindow = 0;
	cameraFlags._reserved = 0;

	projectionRectangle.l = -1.0;
	projectionRectangle.r =  1.0;
	projectionRectangle.t =  1.0;
	projectionRectangle.b = -1.0;
	
	eyePoint[0] = 0.0;
	eyePoint[1] = 0.0;
	eyePoint[2] = 0.0;

	viewPoint[0] = 0.0;
	viewPoint[1] = 0.0;
	viewPoint[2] = 1.0;
    
	rollAngle = 0;
	fov = 90.0;
    pixelAspectRatio = -1.0;
    nearPlane = 1.0;
	farPlane = 128.0;
    
	sBasis[0] = 1.0;
	sBasis[1] = 0.0;
	sBasis[2] = 0.0;
	
	tBasis[0] = 0.0;
	tBasis[1] = 1.0;
	tBasis[2] = 0.0;
	
	uBasis[0] = 0.0;
	uBasis[1] = 0.0;
	uBasis[2] = 1.0;

	memcpy(preTransform, RiIdentityMatrix, sizeof(RtMatrix));
	memcpy(preTransform, transform, sizeof(RtMatrix));
	_printribfile = 0;
	_selList = 0;
	
	return self;
}


- free
{
	[_contextManager destroyContext:contextHandle];
	return nil;
}

- (void)awakeFromNib;
{
	[self init];
}

- setFlushRIB:(BOOL)flag
{
	cameraFlags.doFlush = flag ? 1 : 0;
	return self;
}

- (BOOL)doesFlushRIB
{
	return cameraFlags.doFlush == 1;
}

- flushRIB
{
	return self;
}

- render
{
	// NSLog(@"render");
	return [self renderSelf:contextHandle];
}

- renderSelf:(RtContextHandle)context
{
	// NSLog(@"renderSelf");
	[_contextManager setCurrentContext:context];

	// char *screenAction[2] = {"clear", "finish", "auto"};
	// RiCPPControl("glrenderer", "screen", &screenAction[0], RI_NULL);
	RiProjection(RI_PERSPECTIVE, RI_NULL);
	
	RiFrameBegin(0);
	RiWorldBegin();
	RiTranslate(0.0, 0.0, 2.0);
	RiSphere(0.5, -0.5, 0.5, 360.0, RI_NULL);
	RiWorldEnd();
	RiFrameEnd();
	
	// RiCPPControl("glrenderer", "screen", &screenAction[1], RI_NULL);

	return self;
}

- (void) drawRect: (NSRect) bounds
{
	// NSLog(@"drawRect");

	[_contextManager setCurrentContext:contextHandle];
	
	RtInt width = (RtInt)bounds.size.width;
	RtInt height = (RtInt)bounds.size.height;
	
	RiFormat(width, height, -1.0);
	RiOption("glrenderer", RI_DISPXRES, &width, RI_DISPYRES, &height, RI_NULL);
	[self render];
}

- convertPoints:(NXPoint *)mcoords count:(int)npts toWorld:(RtPoint *)wcoords
{
	return self;
}

- convertPoints:(RtPoint *)points count:(int)n fromSpace:aShape
{
	return self;
}

@end
