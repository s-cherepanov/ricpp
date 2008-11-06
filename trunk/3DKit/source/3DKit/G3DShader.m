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
#import <3Dkit/G3DShader.h>

@implementation G3DShader

- init
{
	[super init];
	return self;
}

- initWithShader:(const char *)aShader
{
	[super init];
	return self;
}

- free
{
	return nil;
}

- setShader:(const char *)shader
{
	return self;
}

- (const char *)shader
{
	return 0;
}

- setColor:(NSColor *)aColor
{
	return self;
}

- (NSColor *)color
{
	return 0;
}

- setUseColor:(BOOL)flag;
{
	return self;
}

- (BOOL)doesUseColor
{
	return color != nil;
}

- setTransparency:(float)value
{
	transparency = value;
	return self;
}

- (float)transparency
{
	return transparency;
}

- (int)shaderArgCount
{
	return shaderArgCount;
}

- (const char *)shaderArgNameAt:(int)i
{
	if ( i >= shaderArgCount )
		return 0;
	return shaderArgs[i].symb.svd_name;
}

- (SLO_TYPE)shaderArgType:(const char *)aName
{
	return SLO_TYPE_UNKNOWN;
}

- (BOOL)isShaderArg:(const char *)aName
{
	return NO;
}

- (SLO_TYPE)shaderType
{
	return shaderType;
}

- setShaderArg:(const char *)aName floatValue:(float)fv
{
	return self;
}

- setShaderArg:(const char *)aName stringValue:(const char *)sv
{
	return self;
}

- setShaderArg:(const char *)aName pointValue:(RtPoint)pv
{
	return self;
}

- setShaderArg:(const char *)aName colorValue:(NSColor *)cv
{
	return self;
}

- setShaderArg:(const char *)aName floatValues:(float *)fvp
{
	return self;
}

- setShaderArg:(const char *)aName stringValues:(const char **)svp
{
	return self;
}

- setShaderArg:(const char *)aName pointValues:(RtPoint *)pvp
{
	return self;
}

- setShaderArg:(const char *)aName colorValues:(NSColor **)cvp
{
	return self;
}

- resetShaderArg:(const char *)aName
{
	return self;
}

- set
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
