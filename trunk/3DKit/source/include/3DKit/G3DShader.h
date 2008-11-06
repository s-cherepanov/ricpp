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
 
#import <ricpp/ri/ri.h>
#import <ricpp/ri/slo.h>


typedef struct {
	SLO_VISSYMDEF symb;
	union {
		float fval;
		RtPoint pval;
		RtFloat *cval;
		NSColor *sval;
	} value;
	RtPoint _rgbv;
} SLOArgs;

@interface G3DShader : NSObject
{
	NSColor *color;
	float transparency;
	char *shader;
	SLO_TYPE shaderType;
	int shaderArgCount; // size of shaderArgs
	SLOArgs *shaderArgs;
	// NXZone *zone;
	
@private
	RtToken *_sTokens;
	RtPointer *_sValues;
	BOOL _doColor;
	RtColor _rgbColor;
	RtColor _rgbOpacity;
	char * _rsName;
	// void *_G3Dprivate;
}

- init;
- initWithShader:(const char *)aShader;
- free;

- setShader:(const char *)shader;
- (const char *)shader;
- setColor:(NSColor *)aColor;
- (NSColor *)color;
- setUseColor:(BOOL)flag;
- (BOOL)doesUseColor;
- setTransparency:(float)value;
- (float)transparency;
- (int)shaderArgCount;
- (const char *)shaderArgNameAt:(int)i;
- (SLO_TYPE)shaderArgType:(const char *)aName;
- (BOOL)isShaderArg:(const char *)aName;
- (SLO_TYPE)shaderType;
- setShaderArg:(const char *)aName floatValue:(float)fv;
- setShaderArg:(const char *)aName stringValue:(const char *)sv;
- setShaderArg:(const char *)aName pointValue:(RtPoint)pv;
- setShaderArg:(const char *)aName colorValue:(NSColor *)cv;
- setShaderArg:(const char *)aName floatValues:(float *)fvp;
- setShaderArg:(const char *)aName stringValues:(const char **)svp;
- setShaderArg:(const char *)aName pointValues:(RtPoint *)pvp;
- setShaderArg:(const char *)aName colorValues:(NSColor **)cvp;
- resetShaderArg:(const char *)aName;
- set;

- read:(NSCoder *)stream;
- write:(NSCoder *)stream;
@end
