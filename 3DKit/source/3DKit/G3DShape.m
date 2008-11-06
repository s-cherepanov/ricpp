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
#import <3Dkit/G3DShader.h>

@implementation G3DShape

- init
{
	[super init];
	
	G3D_CopyMatrix(G3DIdentityMatrix, transform);
	G3D_CopyMatrix(G3DIdentityMatrix, compositeTransform);
	G3D_CopyMatrix(G3DIdentityMatrix, inverseCompositeTransform);
	
	boundingBox[0] = 
	boundingBox[1] = 
	boundingBox[2] = 
	boundingBox[3] = 
	boundingBox[4] = 
	boundingBox[5] = 0;
	shapeName = 0;
	surfaceType = G3D_SmoothSolids;

	surfaceShader = nil;
	displacementShader = nil;
	lightShader = nil;
	imagerShader = nil;
	interiorShader = nil;
	exteriorShader = nil;
	atmosphereShader = nil;
	deformationShader = nil;

	shapeFlags.selectable = 0;
	shapeFlags.visible = 1;
	shapeFlags.ancestorChanged = 0;
	shapeFlags.compositeDirty = 0;
	shapeFlags.drawAsBox = 0;
	shapeFlags.isInstance = 0;
	shapeFlags.hasShader = 0;
	shapeFlags.filler = 0;

	nextPeer = nil;
	previousPeer = nil;
	descendant = nil;
	ancestor = nil;
	
	renderDelegate = nil;

	return self;
}

- setCompositeDirty
{
	shapeFlags.compositeDirty = G3D_CTM_BOTH_DIRTY;
	G3DShape *rp = descendant;
	while ( rp != nil ) {
		[rp setCompositeDirty];
		rp = [rp nextPeer];
	}
	return self;
}


- linkPeer:aPeer
{
	if ( aPeer == nil )
		return self;
		
	[aPeer linkAllAncestors:ancestor];
	[aPeer setCompositeDirty];
	
	if ( nextPeer != nil )
		((G3DShape *)nextPeer)->previousPeer = [aPeer lastPeer];
	((G3DShape *)[aPeer lastPeer])->nextPeer = nextPeer;

	nextPeer = aPeer;
	((G3DShape *)aPeer)->previousPeer = self; 

	return self;
}


- nextPeer
{
	return nextPeer;
}


- previousPeer
{
	return previousPeer;
}

- firstPeer
{
	G3DShape *rp = self;
	while ( [rp previousPeer] != nil ) {
		rp = [rp previousPeer];
	}
	return rp;
}

- lastPeer
{
	G3DShape *rp = self;
	while ( [rp nextPeer] != nil ) {
		rp = [rp nextPeer];
	}
	return rp;
}

- linkDescendant:aDescendant
{
	return self;
}

- descendant
{
	return descendant;
}

- lastDescendant
{
	G3DShape *rp = self;
	while ( [rp descendant] != nil ) {
		rp = [rp descendant];
	}
	return rp;
}

- (BOOL)hasDescendant:(G3DShape *)aShape
{
	if ( aShape == nil )
		return NO;
	return [aShape hasAncestor:self];
}

- linkAllAncestors:anAncestor
{
	G3DShape *rp = self;
	while ( rp != nil ) {
		[rp linkAncestor:ancestor];
		rp = [rp nextPeer];
	}
	
	return self;
}

- linkAncestor:anAncestor
{
	ancestor = anAncestor;
	shapeFlags.ancestorChanged = 1;
	return self;
}

- ancestor
{
	return ancestor;
}

- firstAncestor
{
	G3DShape *rp = self;
	while ( [rp ancestor] != nil ) {
		rp = [rp ancestor];
	}
	return rp;
}

- (BOOL)hasAncestor:(G3DShape *)aShape
{
	G3DShape *rp = self;

	if ( aShape == nil )
		return NO;
	
	while ( rp != nil ) {
		if ( rp == aShape )
			return YES;
		rp = [rp ancestor];
	}

	return NO;
}

- unlink
{
	if ( [self isWorld] )
		return self;

	assert(ancestor != nil);

	if ( previousPeer == nil ) {
		((G3DShape *)ancestor)->descendant = nextPeer;
	} else {
		((G3DShape *)previousPeer)->nextPeer = nextPeer;
	}
	
	if ( nextPeer != nil ) {
		((G3DShape *)nextPeer)->previousPeer = previousPeer;
	}
	
	ancestor = nil;
	nextPeer = nil;
	previousPeer = nil;
	
	[self setCompositeDirty];
	
	return self;
}

- group:toShape
{
	return self;
}

- ungroup
{
	if ( [self isWorld] )
		return self;

	return self;
}

- (BOOL)isWorld
{
	return ancestor == nil ? YES : NO;
}

- setRecursiveSurfaceType:(G3DSurfaceType)aSurfaceType
{
	G3DShape *rp = self;

	while ( [rp nextPeer] != nil ) {
		[rp setSurfaceType:aSurfaceType];
		if ( [rp descendant] != nil )
			[[rp descendant] setRecursiveSurfaceType:aSurfaceType];
		rp = [rp nextPeer];
	}

	return self;
}

- setSurfaceType:(G3DSurfaceType)aSurfaceType
{
	surfaceType = aSurfaceType;
	return self;
}

- setSurfaceType:(G3DSurfaceType)aSurfaceType andDescendants:(BOOL)flag
{
	[self setSurfaceType:aSurfaceType];
	if ( flag && [self descendant] != nil ) {
		[[self descendant] setRecursiveSurfaceType:surfaceType];
	}

	return self;
}

- (G3DSurfaceType)surfaceType
{
	return surfaceType;
}

- setShader:aShader
{
	switch ( [aShader shaderType] ) {
		case SLO_TYPE_SURFACE:
			surfaceShader = aShader;
			break;
		case SLO_TYPE_LIGHT:
			lightShader = aShader;
			break;
		case SLO_TYPE_DISPLACEMENT:
			displacementShader = aShader;
			break;
		case SLO_TYPE_INTERIOR:
			interiorShader = aShader;
			break;
		case SLO_TYPE_EXTERIOR:
			exteriorShader = aShader;
			break;
		case SLO_TYPE_ATMOSPHERE:
			atmosphereShader = aShader;
			break;
		case SLO_TYPE_DEFORMATION:
			deformationShader = aShader;
			break;
		case SLO_TYPE_IMAGER:
			imagerShader = aShader;
			break;
		default:
			break;
	}
	return self;
}

- shaderType:(SLO_TYPE)type
{
	switch ( type ) {
		case SLO_TYPE_SURFACE:
			return surfaceShader;
		case SLO_TYPE_LIGHT:
			return lightShader;
		case SLO_TYPE_DISPLACEMENT:
			return displacementShader;
		case SLO_TYPE_INTERIOR:
			return interiorShader;
		case SLO_TYPE_EXTERIOR:
			return exteriorShader;
		case SLO_TYPE_ATMOSPHERE:
			return atmosphereShader;
		case SLO_TYPE_DEFORMATION:
			return deformationShader;
		case SLO_TYPE_IMAGER:
			return imagerShader;
		default:
			return nil;
	};
}

- getBoundingBox:(RtBound *)bbox
{
	assert(bbox != 0);
	if ( bbox == 0 )
		return self;
	
	memcpy(&((*bbox)[0]), &(boundingBox[0]), sizeof(RtBound));
	
	return self;
}

- setDrawAsBox:(BOOL)flag
{
	shapeFlags.drawAsBox = flag ? 1 : 0;
	return self;
}

- (BOOL)doesDrawAsBox
{
	return shapeFlags.drawAsBox ? YES : NO;
}

- getBoundingBox:(RtBound *)bbox inCamera:aCamera
{
	assert(bbox != 0);
	if ( bbox == 0 )
		return self;
	
	RtPoint p[2] = {
		boundingBox[0],
		boundingBox[2],
		boundingBox[4],
		boundingBox[1],
		boundingBox[3],
		boundingBox[5]
	};
	[aCamera convertPoints:&(p[0]) count:2 fromSpace:self];
	(*bbox)[0] = p[0][0];
	(*bbox)[2] = p[0][1];
	(*bbox)[4] = p[0][2];
	(*bbox)[1] = p[1][0];
	(*bbox)[3] = p[1][1];
	(*bbox)[5] = p[1][2];
	
	return self;
}


- convertObjectPoints:(RtPoint *)points count:(int)n toCamera:aCamera
{
	[aCamera convertPoints:points count:n fromSpace:self];
	return self;
}

- convertPoints:(RtPoint *)points count:(int)n fromAncestor:(G3DShape *)aShape
{
	return self;
}

- convertPoints:(RtPoint *)points count:(int)n toAncestor:(G3DShape *)aShape
{
	return self;
}


- setSelectable:(BOOL)flag
{
	shapeFlags.selectable = flag ? 1 : 0;
	return self;
}

- (BOOL)isSelectable
{
	return shapeFlags.selectable;
}

- setVisible:(BOOL)flag
{
	shapeFlags.visible = flag ? 1 : 0;
	return self;
}

- (BOOL)isVisible
{
	return shapeFlags.visible;
}

- setShapeName:(const char *)name
{
	return self;
}

- (const char *)shapeName
{
	return 0;
}

- setRenderDelegate:anObject
{
	return self;
}

- removeRenderDelegate
{
	return self;
}

- renderDelegate
{
	return renderDelegate;
}

- setTransformMatrix:(RtMatrix)tm
{
	return self;
}

- getTransformMatrix:(RtMatrix)tm
{
	return self;
}

- concatTransformMatrix:(RtMatrix)ctm premultiply:(BOOL)flag
{
	return self;
}

- getCompositeTransformMatrix:(RtMatrix)ctm relativeToAncestor:(G3DShape *)aShape
{
	return self;
}

- getInverseCompositeTransformMatrix:(RtMatrix)ictm relativeToAncestor:(G3DShape *)aShape
{
	return self;
}

- rotateAngle:(float)ang axis:(RtPoint)anAxis
{
	return self;
}

- preRotateAngle:(float)ang axis:(RtPoint)anAxis
{
	return self;
}

- scale:(float)sc :(float)sy :(float)sz
{
	return self;
}

- preScale:(float)sc :(float)sy :(float)sz
{
	return self;
}

- scaleUniformly:(float)s
{
	return self;
}

- preScaleUniformly:(float)s
{
	return self;
}

- translate:(float)tx :(float)ty :(float)tz
{
	return self;
}

- preTranslate:(float)tx :(float)ty :(float)tz
{
	return self;
}

- render:(G3DCamera *)camera
{
	return self;
}

- renderSelf:(G3DCamera *)camera
{
	return self;
}

- renderSelfAsBox:(G3DCamera *)camera
{
	return self;
}

- free
{
	return nil;
}

- freeAll
{
	return nil;
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

@end
