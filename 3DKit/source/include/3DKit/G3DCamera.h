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
#import <Cocoa/Cocoa.h>
#import <3Dkit/gnu3d.h>

@interface G3DCamera : NSOpenGLView
{
    // unsigned int	globalWindowNum; /* the server global window number */
    // RtToken		    windowResource;  /* the resource ID of our window */
    G3DProjectionType projectionType;	 /* perpective or orthographic */
    RtContextHandle	contextHandle;	 /* the RMan name for the context */
    id			worldShape;	 /* top of shape hierarchy */
    NSArray		*lightList;	 /* list of global G3DLights */
    id			delegate;	 /* get called when frames are done */
    NSColor		*backgroundColor; /* color of rectangle under RIB */
    G3DHider	hider;
	
    struct _cameraFlags {
		unsigned int degenerate : 1;  /* view is 0 width or height */
		unsigned int windowChanged : 1;	 /* the window changed */
		unsigned int needsWindow : 1;
		unsigned int basisChanged : 1;	/* basis changed w.r.t. world coords */
		unsigned int canRender : 1;  /* rendering legal */
		unsigned int usePreTM : 1;  /* pre-multiply the ct */
		unsigned int doFlush : 1;  /* Flush at FrameEnd? */
		unsigned int inWorldBlock : 1; /* inside a World{Begin,End} block ? */
		unsigned int drawBackground : 1; /* draw bkgnd each drawSelf? */
		
		/* private flags */
		unsigned int _computeAspect : 1; /* user-supplied aspect? */
		unsigned int _computeSWindow : 1; /* user-supplied screen window? */
		unsigned int _reserved : 21;
    } cameraFlags;
	
    struct _projectionRectangle {
		float l, r, t, b;
    } projectionRectangle;
	
    RtPoint		eyePoint, viewPoint;  /* view vector */
    float		rollAngle;  /* neg. rot. about uBasis */
    float		fov;
    float		pixelAspectRatio;
    float		nearPlane, farPlane;
    RtPoint		sBasis, tBasis, uBasis;  /* basis w.r.t. world space */
	
    RtMatrix		preTransform;	/* pre-multiply the camera matrix by
	 this matrix, if desired (cache for
	 above object) */
    RtMatrix		transform; /* world-to-eye transformation:
	 ct = Te . R, Te translates eye
	 point to origin, R rotates view
	 vector, V = view - eye, such
	 that it is parallel with the
	 z-axis, and rotates the resulting
	 vector about the z-axis by
	 -rollAngle. */
	
    char 		*_printribfile;  /* rib file generated during printing*/
    id			_contextManager; /* context manager for the app */
    NSArray		*_selList;
    // void		*_G3Dprivate;
}
#if 0
- initFrame:(const NSRect *)fRect;

/* frame and view management methods */
- (BOOL)lockFocus;
- unlockFocus;
- setFlushRIB:(BOOL)flag;
- (BOOL)doesFlushRIB;
- flushRIB;

- drawPS:(NSRect *)rects :(int)nRects;
- drawSelf:(const NSRect *)rects :(int)nRects;
- worldBegin:(RtContextHandle)context;
- worldEnd:(RtContextHandle)context;
- copyRIBCode:(NSStream *)stream;
- setBackgroundColor:(NSColor *) c;
- (NSColor *)backgroundColor;
- setDrawBackgroundColor:(BOOL)flag;
- (BOOL)doesDrawBackgroundColor;
- setFrame:(const NSRect *)fRect;
- moveTo:(NXCoord)x :(NXCoord)y;
- moveBy:(NXCoord)deltaX :(NXCoord)deltaY;
- sizeTo:(NXCoord)width :(NXCoord)height;
- sizeBy:(NXCoord)deltaWidth :(NXCoord)deltaHeight;
- rotateTo:(NXCoord)angle;
- rotateBy:(NXCoord)deltaAngle;
- setWorldShape:a3DObject;
- worldShape;
- addLight:aLight;
- removeLight:aLight;
- lightList;
- setHider:(G3DHider)hider;
- (G3DHider)hider;
- setSurfaceTypeForAll:(G3DSurfaceType)surfaceType chooseHider:(BOOL)flag;

/* Picking */
- selectShapesIn:(const NSRect *)selectionRect;

/* eye vector and viewing frustum manipulation methods */
- setProjectionRectangle:(float)l :(float)r :(float)t :(float)b;
- getProjectionRectangle:(float *)lp :(float *)rp :(float *)tp :(float *)bp;
- setProjection:(G3DProjectionType)aProjectionType;
- (G3DProjectionType)projectionType;
- setPreTransformMatrix:(RtMatrix)newPreTM;
- getPreTransformMatrix:(RtMatrix)preTM;
- setUsePreTransformMatrix:(BOOL)flag;
- (BOOL)usesPreTransformMatrix;
- setEyeAt:(RtPoint)fromPoint toward:(RtPoint)toPoint roll:(float)aRollAngle;
- getEyeAt:(RtPoint *)anEyePoint toward:(RtPoint *)aViewPoint
	  roll:(float *)aRollAngle;
- setClipPlanesNear:(float)aNearPlane far:(float)aFarPlane;
- getClipPlanesNear:(float *)aNearPlane far:(float *)aFarPlane;
- setFieldOfViewByAngle:(float)aFieldOfView;
- setFieldOfViewByFocalLength:(float)aFocalLength;
- (float)fieldOfView;
- setPixelAspectRatio:(float)pixAspect;
- (float)pixelAspectRatio;

- moveEyeBy:(float)ds :(float)dt :(float)du;
- rotateEyeBy:(float)dElev :(float)dAzim about:(RtPoint)pivotPtr;

- (int)numCropWindows;
- cropInRects:(NSRect *)rects nRects:(int)n;
- (int)frameNumber;
- (BOOL)canPrintRIB;

- awake;
- read:(NSCoder *)stream;
- write:(NSCoder *)stream;

- (int)renderAsTIFF;
- (int)renderAsEPS;
- setDelegate:sender;
- delegate;
#endif

- init;
- free;

- (void) awakeFromNib;
- render;
- renderSelf:(RtContextHandle)context;
- (void) drawRect: (NSRect) bounds;

- convertPoints:(NXPoint *)mcoords count:(int)npts toWorld:(RtPoint *)wcoords;
- convertPoints:(RtPoint *)points count:(int)n fromSpace:aShape;
@end

/*
@interface Object(G3DCameraDelegate)
- camera:sender didRenderStream:(NSStream *)s tag:(int)atag frameNumber:(int)n;
@end
*/
