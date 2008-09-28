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

#import <Foundation/Foundation.h>
#import <3Dkit/gnu3d.h>

@interface G3DRotator : NSObject
{
  id camera;
  G3DAxis rotationAxis;
  NSRect bounds;
  NSPoint center;
  float radius;
}

//-----------------------------------------------------------------------------
// Init and free
//-----------------------------------------------------------------------------
- init;
- initWithCamera:aCamera;

//-----------------------------------------------------------------------------
// Accessor methods
//-----------------------------------------------------------------------------

- setCamera:aCamera;
- setCenter:(const NSPoint *)center andRadius:(float)r;

- setRotationAxis:(G3DAxis)axis;
- (G3DAxis)rotationAxis;

//-----------------------------------------------------------------------------
// Rotating
//-----------------------------------------------------------------------------

- trackMouseFrom:(const NSPoint *)from to:(const NSPoint *)to rotationMatrix:(RtMatrix)matrix andInverse:(RtMatrix)inverseMatrix;

//-----------------------------------------------------------------------------
// Archive
//-----------------------------------------------------------------------------
- read:(NSCoder *)stream;
- write:(NSCoder *)stream;

@end
