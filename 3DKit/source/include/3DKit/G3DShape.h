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
#import <3Dkit/G3DCamera.h>
#import <3Dkit/G3DShader.h>

typedef struct {
	char id[6]; /* self, encoded to 6 non-0 bytes */
	char name;  /* optional visible name */
} G3DShapeName;

/* flags for CTM and inverse dirty bits */
#define G3D_BOTH_CLEAN        0
#define G3D_CTM_DIRTY         1
#define G3D_CTM_INVERSE_DIRTY 2
#define G3D_CTM_BOTH_DIRTY    3

@interface G3DShape : NSObject
{

	RtMatrix transform;
	RtMatrix compositeTransform;
	RtMatrix inverseCompositeTransform;
	RtBound  boundingBox;
	G3DShapeName *shapeName;
	G3DSurfaceType surfaceType;
	
	/* Pointers to each kind of shader available */
	id surfaceShader;
	id displacementShader;
	id lightShader;
	id imagerShader;
	id interiorShader;
	id exteriorShader;
	id atmosphereShader;
	id deformationShader;
	
	struct {
		unsigned int selectable:1;      /* Can you select this N3DShape */
		unsigned int visible:1;         /* Self and descendants are invisible */
		unsigned int ancestorChanged:1; /* need to update bounds etc. */
		unsigned int compositeDirty:2;  /* flags for CTM & inverse */
		unsigned int drawAsBox:1;       /* draw the box */
		unsigned int isInstance:1;      /* Is this an instance of another? */
		unsigned int hasShader:1;       /* Are there any shaders? */
		unsigned int filler:26;
	} shapeFlags;
	
	/* Shape hierarchy */
	id nextPeer;
	id previousPeer; /* back link */
	id descendant;
	id ancestor;     /* back link */
	
	id renderDelegate; /* Set if this object is an instance */
	
	// @private
	// void *_CTMRel;
	// char *_RNM;
	// void *_G3Dprivate
}

- init;

- linkPeer:aPeer;
- nextPeer;
- previousPeer;
- firstPeer;
- lastPeer;

- linkDescendant:aDescendant;
- descendant;
- lastDescendant;
- (BOOL)hasDescendant:(G3DShape *)aShape;

- linkAncestor:anAncestor;
- ancestor;
- firstAncestor;
- (BOOL)hasAncestor:(G3DShape *)aShape;

- unlink;
- group:toShape;
- ungroup;

- (BOOL)isWorld;

- setSurfaceType:(G3DSurfaceType)aSurfaceType andDescendants:(BOOL)flag;
- (G3DSurfaceType)surfaceType;

- setShader:aShader;
- shaderType:(G3D_SLO_TYPE)type;

- getBoundingBox:(RtBound *)bbox;
- setDrawAsBox:(BOOL)flag;
- (BOOL)doesDrawAsBox;
- getBoundingBox:(RtBound *)bbox inCamera:aCamera;
- convertObjectPoints:(RtPoint *)points count:(int)n toCamera:camera;
- convertPoints:(RtPoint *)points count:(int)n fromAncestor:(G3DShape *)aShape;
- convertPoints:(RtPoint *)points count:(int)n toAncestor:(G3DShape *)aShape;

- setSelectable:(BOOL)flag;
- (BOOL)isSelectable;
- setVisible:(BOOL)flag;
- (BOOL)isVisible;

- setShapeName:(const char *)name;
- (const char *)shapeName;

- setRenderDelegate:anObject;
- removeRenderDelegate;
- renderDelegate;

- setTransformMatrix:(RtMatrix)tm;
- getTransformMatrix:(RtMatrix)tm;
- concatTransformMatrix:(RtMatrix)ctm premultiply:(BOOL)flag;
- getCompositeTransformMatrix:(RtMatrix)ctm relativeToAncestor:(G3DShape *)aShape;
- getInverseCompositeTransformMatrix:(RtMatrix)ictm relativeToAncestor:(G3DShape *)aShape;

- rotateAngle:(float)ang axis:(RtPoint)anAxis;
- preRotateAngle:(float)ang axis:(RtPoint)anAxis;
- scale:(float)sc :(float)sy :(float)sz;
- preScale:(float)sc :(float)sy :(float)sz;
- scaleUniformly:(float)s;
- preScaleUniformly:(float)s;
- translate:(float)tx :(float)ty :(float)tz;
- preTranslate:(float)tx :(float)ty :(float)tz;

- render:(G3DCamera *)camera;
- renderSelf:(G3DCamera *)camera;
- renderSelfAsBox:(G3DCamera *)camera;
- free;
- freeAll;

- awake;
- read:(NSCoder *)stream;
- write:(NSCoder *)stream;

// private
- setSurfaceType:(G3DSurfaceType)aSurfaceType;
- setRecursiveSurfaceType:(G3DSurfaceType)aSurfaceType;
- linkAllAncestors:anAncestor;
- setCompositeDirty;
@end
