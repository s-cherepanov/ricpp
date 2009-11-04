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
#import <ricpp/ricpp.h>

@interface G3DContextManager : NSObject
{
	RtContextHandle mainContext;
	id contextTable;
	// RtContextHandle currentContext;

@private
	long _nameCnt;
	// id _focussedCamera;
	// RtPointer _reserved;
	// void *_G3Dprivate;
}

+ (id)contextManager;
- (id)init;
- (void)dealloc;

- (RtContextHandle)mainContext;
- (RtContextHandle)createContext:(const char *)name;
- (RtContextHandle)createContext:(const char *)name withRenderer: (RtToken)renderer;
- (RtContextHandle)createContext:(const char *)name toFile: (RtToken)ribFileName;

- (void)destroyContext:(RtContextHandle)aContext;
- (void)destroyContextByName:(const char *)name;

- (RtContextHandle)setCurrentContext:(RtContextHandle)aContext;
- (RtContextHandle)setCurrentContextByName:(RtToken)name;
- (RtContextHandle)currentContext;

@end
