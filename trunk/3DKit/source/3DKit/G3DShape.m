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

@implementation G3DShape

-init
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
}


- linkPeer:aPeer
{
	if ( aPeer == nil )
		return self;
		
	[aPeer linkAllAncestors:ancestor];
	[aPeer setCompositeDirty];
	
	if ( nextPeer != nil )
		nextPeer->previousPeer = [aPeer lastPeer];
	[aPeer lastPeer]->nextPeer = nextPeer;

	nextPeer = aPeer;
	aPeer->previousPeer = self; 

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

- unlink
{
	if ( [self isWorld] )
		return self;

	assert(ancestor != nil);

	if ( previousPeer == nil ) {
		ancestor->decendant = nextPeer;
	} else {
		previousPeer->nextPeer = nextPeer;
	}
	
	if ( nextPeer != nil ) {
		nextPeer->previousPeer = previousPeer;
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
		[rp surfaceType:aSurfaceType];
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
	[self surfaceType:aSurfaceType];
	if ( flag && [rp descendant] != nil ) {
		[[rp descendant] setSurfaceRecursiveType:surfaceType];
	}

	return self;
}

- (G3DSurfaceType)surfaceType
{
	return surfaceType;
}


@end
