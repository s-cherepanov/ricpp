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

@interface G3DMovieCamera : G3DCamera
{
	int frameNumber; /* Count given by FrameBegin */
	int startFrame;  /* Start frame number of movie */
	int endFrame;    /* End frame number of movie */
	/* char **_spoolFiles; */
	/* void *_G3Dmcprivate; */
}

- initFrame:(const NSRect *)fRect;
- render;
- displayMovie;
- setFrameNumber:(int)aFrameNumber;
- (int)frameNumber;
- setStartFrame:(int)start endFrame:(int)end incrementFrameBy:(int)skip;
- (int)startFrame;
- (int)endFrame;
- (int)frameIncrement;
- (BOOL)knowsPagesFirst:(int *)first last:(int *)last;
- (BOOL)getRect:(NSRect *)r forPage:(int)page;
- (int)numCropWindows;
- cropInRects:(NSRect *)rects nRects:(int)n;
- awake;
- read:(NSCoder *)stream;
- write:(NSCoder *)stream;

@end
