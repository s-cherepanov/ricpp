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
    NSMutableArray		*lightList;	 /* list of global G3DLights */
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

@property (readwrite) G3DProjectionType projectionType;
@property (readwrite) RtContextHandle	contextHandle;
@property (readwrite, retain) id worldShape;
@property (readonly, retain) NSMutableArray *lightList;
@property (readwrite, retain) id delegate;
@property (readwrite, copy) NSColor *backgroundColor;
@property (readwrite) G3DHider hider;

#if 0
- initFrame:(const NSRect *)fRect;

/* frame and view management methods */
- (BOOL)lockFocus;
- (id)unlockFocus;
- (id)setFlushRIB:(BOOL)flag;
- (BOOL)doesFlushRIB;
- (id)flushRIB;

- (id)drawPS:(NSRect *)rects :(int)nRects;
- (id)drawSelf:(const NSRect *)rects :(int)nRects;
- (id)worldBegin:(RtContextHandle)context;
- (id)worldEnd:(RtContextHandle)context;
- (id)copyRIBCode:(NSStream *)stream;
- (id)setDrawBackgroundColor:(BOOL)flag;
- (BOOL)doesDrawBackgroundColor;
- (id)setFrame:(const NSRect *)fRect;
- (id)moveTo:(NXCoord)x :(NXCoord)y;
- (id)moveBy:(NXCoord)deltaX :(NXCoord)deltaY;
- (id)sizeTo:(NXCoord)width :(NXCoord)height;
- (id)sizeBy:(NXCoord)deltaWidth :(NXCoord)deltaHeight;
- (id)rotateTo:(NXCoord)angle;
- (id)rotateBy:(NXCoord)deltaAngle;
- (id)addLight:(id)aLight;
- (id)removeLight:(id)aLight;
- (id)setSurfaceTypeForAll:(G3DSurfaceType)surfaceType chooseHider:(BOOL)flag;

/* Picking */
- (id)selectShapesIn:(const NSRect *)selectionRect;

/* eye vector and viewing frustum manipulation methods */
- (id)setProjectionRectangle:(float)l :(float)r :(float)t :(float)b;
- (id)getProjectionRectangle:(float *)lp :(float *)rp :(float *)tp :(float *)bp;
- (id)setPreTransformMatrix:(RtMatrix)newPreTM;
- (id)getPreTransformMatrix:(RtMatrix)preTM;
- (id)setUsePreTransformMatrix:(BOOL)flag;
- (BOOL)usesPreTransformMatrix;
- (id)setEyeAt:(RtPoint)fromPoint toward:(RtPoint)toPoint roll:(float)aRollAngle;
- (id)getEyeAt:(RtPoint *)anEyePoint toward:(RtPoint *)aViewPoint
	  roll:(float *)aRollAngle;
- (id)setClipPlanesNear:(float)aNearPlane far:(float)aFarPlane;
- (id)getClipPlanesNear:(float *)aNearPlane far:(float *)aFarPlane;
- (id)setFieldOfViewByAngle:(float)aFieldOfView;
- (id)setFieldOfViewByFocalLength:(float)aFocalLength;
- (float)fieldOfView;
- (id)setPixelAspectRatio:(float)pixAspect;
- (float)pixelAspectRatio;

- (id)moveEyeBy:(float)ds :(float)dt :(float)du;
- (id)rotateEyeBy:(float)dElev :(float)dAzim about:(RtPoint)pivotPtr;

- (int)numCropWindows;
- (id)cropInRects:(NSRect *)rects nRects:(int)n;
- (int)frameNumber;
- (BOOL)canPrintRIB;

- (id)copyWithZone:(NSZone *)zone;

- (id)initWithCoder:(NSCoder *)coder;
- (void)encodeWithCoder:(NSCoder *)coder;

- (int)renderAsTIFF;
- (int)renderAsEPS;
#endif

-(id)init;
-(void)dealloc;

- (void)awakeFromNib;
- (id)render;
- (id)renderSelf:(RtContextHandle)context;
- (void)drawRect:(NSRect) bounds;

- (id)convertPoints:(NXPoint *)mcoords count:(int)npts toWorld:(RtPoint *)wcoords;
- (id)convertPoints:(RtPoint *)points count:(int)n fromSpace:aShape;

@end

@interface NSObject(G3DCameraDelegate)
- (id)camera:(id)sender didRenderStream:(NSStream *)s tag:(int)atag frameNumber:(int)n;
@end
