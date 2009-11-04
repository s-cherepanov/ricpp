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


@implementation G3DSLOScalar;

@synthesize val;

- (id)init
{
	[self initWithVal:(G3D_SLO_FLOAT)0];
	
	return self;
}

- (id)initWithG3DSLOScalar:(G3DSLOScalar *)aVal
{
	if ( self == aVal )
		return self;
	
	G3D_SLO_FLOAT floatVal = 0;
	if ( self != nil )
		floatVal = [aVal val];

	[self initWithVal:floatVal];

	return self;
}

- (id)initWithVal:(G3D_SLO_FLOAT)aVal
{
	val = aVal;
	
	return self;
}

- (id)copyWithZone:(NSZone *)zone
{
	G3DSLOScalar *copy = [[[self class] allocWithZone: zone] init];

	[copy setVal:val];

	return copy;
}

- (id)initWithCoder:(NSCoder *)decoder
{	
	val = [decoder decodeFloatForKey:@"val"];

	return self;
}

- (void)encodeWithCoder:(NSCoder *)encoder
{
	[encoder encodeFloat:val forKey:@"val"];
}

@end /* G3DSLOScalar */


@implementation G3DSLOMatrix;

- (id)init
{
	G3D_SLO_FLOAT aVal[4][4] = {
		(G3D_SLO_FLOAT) 0, (G3D_SLO_FLOAT) 0, (G3D_SLO_FLOAT) 0, (G3D_SLO_FLOAT) 0, 
		(G3D_SLO_FLOAT) 0, (G3D_SLO_FLOAT) 0, (G3D_SLO_FLOAT) 0, (G3D_SLO_FLOAT) 0, 
		(G3D_SLO_FLOAT) 0, (G3D_SLO_FLOAT) 0, (G3D_SLO_FLOAT) 0, (G3D_SLO_FLOAT) 0, 
		(G3D_SLO_FLOAT) 0, (G3D_SLO_FLOAT) 0, (G3D_SLO_FLOAT) 0, (G3D_SLO_FLOAT) 0
	};
	
	[self initWithVal:aVal];

	return self;
}

- (id)initWithG3DSLOMatrix:(G3DSLOMatrix *)aVal
{
	G3D_SLO_FLOAT aMatrix[4][4] = {
		(G3D_SLO_FLOAT) 0, (G3D_SLO_FLOAT) 0, (G3D_SLO_FLOAT) 0, (G3D_SLO_FLOAT) 0, 
		(G3D_SLO_FLOAT) 0, (G3D_SLO_FLOAT) 0, (G3D_SLO_FLOAT) 0, (G3D_SLO_FLOAT) 0, 
		(G3D_SLO_FLOAT) 0, (G3D_SLO_FLOAT) 0, (G3D_SLO_FLOAT) 0, (G3D_SLO_FLOAT) 0, 
		(G3D_SLO_FLOAT) 0, (G3D_SLO_FLOAT) 0, (G3D_SLO_FLOAT) 0, (G3D_SLO_FLOAT) 0
	};
	
	if ( aVal == self )
		return self;
	
	[aVal getVal:aMatrix];		
	[self initWithVal:aMatrix];
	
	return self;
}

- (id)initWithVal:(G3D_SLO_FLOAT[4][4])aVal
{
	[self setVal:aVal];

	return self;
}

- (id)copyWithZone:(NSZone *)zone
{
	NSUInteger aRow, aCol;
	G3DSLOMatrix *copy = [[[self class] allocWithZone: zone] init];
	
	for ( aRow = 0; aRow < 4; aRow++ ) {
		for ( aCol = 0; aCol < 4; aCol++ ) {
			[copy setRow:aRow col:aCol val:val[aRow][aCol]];		
		}
	}
	
	return copy;	
}

- (id)initWithCoder:(NSCoder *)decoder
{
	val[0][0] = [decoder decodeFloatForKey:@"val[0][0]"];
	val[0][1] = [decoder decodeFloatForKey:@"val[0][1]"];
	val[0][2] = [decoder decodeFloatForKey:@"val[0][2]"];
	val[0][3] = [decoder decodeFloatForKey:@"val[0][3]"];

	val[1][0] = [decoder decodeFloatForKey:@"val[1][0]"];
	val[1][1] = [decoder decodeFloatForKey:@"val[1][1]"];
	val[1][2] = [decoder decodeFloatForKey:@"val[1][2]"];
	val[1][3] = [decoder decodeFloatForKey:@"val[1][3]"];
	
	val[2][0] = [decoder decodeFloatForKey:@"val[2][0]"];
	val[2][1] = [decoder decodeFloatForKey:@"val[2][1]"];
	val[2][2] = [decoder decodeFloatForKey:@"val[2][2]"];
	val[2][3] = [decoder decodeFloatForKey:@"val[2][3]"];
	
	val[3][0] = [decoder decodeFloatForKey:@"val[3][0]"];
	val[3][1] = [decoder decodeFloatForKey:@"val[3][1]"];
	val[3][2] = [decoder decodeFloatForKey:@"val[3][2]"];
	val[3][3] = [decoder decodeFloatForKey:@"val[3][3]"];
	
	return self;
}

- (void)encodeWithCoder:(NSCoder *)encoder
{
	[encoder encodeFloat:val[0][0] forKey:@"val[0][0]"];		
	[encoder encodeFloat:val[0][1] forKey:@"val[0][1]"];		
	[encoder encodeFloat:val[0][2] forKey:@"val[0][2]"];		
	[encoder encodeFloat:val[0][3] forKey:@"val[0][3]"];		

	[encoder encodeFloat:val[1][0] forKey:@"val[1][0]"];		
	[encoder encodeFloat:val[1][1] forKey:@"val[1][1]"];		
	[encoder encodeFloat:val[1][2] forKey:@"val[1][2]"];		
	[encoder encodeFloat:val[1][3] forKey:@"val[1][3]"];		
	
	[encoder encodeFloat:val[2][0] forKey:@"val[2][0]"];		
	[encoder encodeFloat:val[2][1] forKey:@"val[2][1]"];		
	[encoder encodeFloat:val[2][2] forKey:@"val[2][2]"];		
	[encoder encodeFloat:val[2][3] forKey:@"val[2][3]"];		
	
	[encoder encodeFloat:val[3][0] forKey:@"val[3][0]"];		
	[encoder encodeFloat:val[3][1] forKey:@"val[3][1]"];		
	[encoder encodeFloat:val[3][2] forKey:@"val[3][2]"];		
	[encoder encodeFloat:val[3][3] forKey:@"val[3][3]"];		
}

- (id)setRow:(NSUInteger)row col:(NSUInteger)col val:(G3D_SLO_FLOAT)value
{
	if ( row < 0 || row > 3 || col < 0 || col > 3 )
		return nil;

	val[row][col] = value;

	return self;
}

- (G3D_SLO_FLOAT)getRow:(NSUInteger)row col:(NSUInteger)col
{
	if ( row < 0 || row > 3 || col < 0 || col > 3 )
		return (G3D_SLO_FLOAT)0;

	return val[row][col];
}

- (id)setVal:(G3D_SLO_FLOAT[4][4])aVal
{
	NSUInteger aRow, aCol;
	
	for ( aRow = 0; aRow < 4; aRow++ ) {
		for ( aCol = 0; aCol < 4; aCol++ ) {
			val[aRow][aCol] = aVal[aRow][aCol];
		}
	}
	
	return self;
}

- (void)getVal:(G3D_SLO_FLOAT[4][4])aVal
{
	NSUInteger aRow, aCol;
	
	for ( aRow = 0; aRow < 4; aRow++ ) {
		for ( aCol = 0; aCol < 4; aCol++ ) {
			aVal[aRow][aCol] = val[aRow][aCol];
		}
	}	
}

@end /* G3DSLOMatrix */


@implementation G3DSLOPoint

@synthesize xVal;
@synthesize yVal;
@synthesize zVal;

- (id)init
{
	[self initWithX:(G3D_SLO_FLOAT)0 y:(G3D_SLO_FLOAT)0 z:(G3D_SLO_FLOAT)0];
	
	return self;
}

- (id)initWithG3DSLOPoint:(G3DSLOPoint *)aVal
{
	if ( aVal == self )
		return self;
	
	if ( aVal == nil ) {
		[self initWithX:(G3D_SLO_FLOAT)0 y:(G3D_SLO_FLOAT)0 z:(G3D_SLO_FLOAT)0];
	} else {
		[self initWithX:[aVal xVal] y:[aVal yVal] z:[aVal zVal]];
	}
	
	return self;
}

- (id)initWithX:(G3D_SLO_FLOAT)x y:(G3D_SLO_FLOAT)y z:(G3D_SLO_FLOAT)z
{
	[self setX:(G3D_SLO_FLOAT)x y:(G3D_SLO_FLOAT)y z:(G3D_SLO_FLOAT)z];
	
	return self;	
}

- (id)copyWithZone:(NSZone *)zone
{
	G3DSLOPoint *copy = [[[self class] allocWithZone: zone] init];

	[copy setXVal:xVal];
	[copy setYVal:yVal];
	[copy setZVal:zVal];

	return copy;
}

- (id)initWithCoder:(NSCoder *)decoder
{
	xVal = [decoder decodeFloatForKey:@"xVal"];
	yVal = [decoder decodeFloatForKey:@"yVal"];
	zVal = [decoder decodeFloatForKey:@"zVal"];
	
	return self;
}

- (void)encodeWithCoder:(NSCoder *)encoder
{
	[encoder encodeFloat:xVal forKey:@"xVal"];
	[encoder encodeFloat:yVal forKey:@"yVal"];
	[encoder encodeFloat:zVal forKey:@"zVal"];
}


- (id)setX:(G3D_SLO_FLOAT)x y:(G3D_SLO_FLOAT)y z:(G3D_SLO_FLOAT)z
{
	[self setXVal:x];
	[self setYVal:y];
	[self setZVal:z];

	return self;
}

@end /* G3DSLOPoint */


@implementation G3DSLOColor

@synthesize val;

- (id)init
{
	[self initWithCapacity:3];
	
	return self;
}

- (id)initWithCount:(NSUInteger)numItems components:(G3D_SLO_FLOAT *)theColorComponents
{
	NSUInteger i;
	
	[self initWithCapacity:numItems];
	for ( i = 0; i < [self count]; ++i ) {
		[self setComponent:i val:theColorComponents[i]];
	}
	
	return self;
}

- (id)initWithScalarArrayCopy:(NSArray *)scalarAry
{
	NSUInteger i;
	
	if ( scalarAry == nil ) {
		[self initWithCapacity:3];
		return self;
	}

	[self initWithCapacity:[scalarAry count]];
	for ( i = 0; i < [self count]; ++i ) {
		G3DSLOScalar *s = [scalarAry objectAtIndex:i];
		[self setComponent:i val:[s val]];
	}
	
	return self;
}

- (id)initWithG3DSLOColor:(G3DSLOColor *)cv
{
	NSUInteger i;
	
	if ( cv == self )
		return self;

	int aCount = 3;
	if ( cv != nil ) {
		aCount = [cv count];
	}

	[self initWithCapacity:aCount];

	if ( cv == nil )
		return self;
	
	for ( i = 0; i < aCount; ++i ) {
		if ( cv != nil ) {
			[self setComponent:i val:[cv component:i]];		
		}
	}

	return self;
}

- (id)initWithColor:(NSColor *)cv
{
	[self initWithCapacity:3];
	[self setColor:cv];
	
	return self;
}

- (id)initWithCapacity:(NSUInteger)numItems
{
	NSUInteger i;
	
	val = [NSMutableArray arrayWithCapacity:numItems];
	for ( i = 0; i < [self count]; ++i ) {
		[self setComponent:i val:(G3D_SLO_FLOAT)0];
	}
	
	return self;
}

- (void)dealloc
{
	[val release];
	[super dealloc];
}

- (id)copyWithZone:(NSZone *)zone
{
	G3DSLOColor *copy = [[[self class] allocWithZone:zone] initWithScalarArrayCopy:val];
	
	return copy;
}

- (id)initWithCoder:(NSCoder *)decoder
{
	val = [[decoder decodeObjectForKey:@"val"] retain];
	return self;
}

- (void)encodeWithCoder:(NSCoder *)encoder
{
	[encoder encodeObject:val forKey:@"val"];
}

- (id)setComponent:(NSUInteger)i val:(G3D_SLO_FLOAT)value
{
	G3DSLOScalar *scalar = nil;

	if ( i < 0 || i > [self count] )
		return nil;
	
	scalar = [(G3DSLOScalar *)[G3DSLOScalar alloc] initWithVal:value];
	[val insertObject:scalar atIndex:i];
	[scalar release];

	return self;
}

- (G3D_SLO_FLOAT)component:(NSUInteger)i
{
	G3DSLOScalar *scalar = nil;

	if ( i < 0 || i > [self count] )
		return (G3D_SLO_FLOAT)0;
	
	scalar = [val objectAtIndex:i];
	
	if ( scalar == nil )
		return (G3D_SLO_FLOAT)0;
	
	G3D_SLO_FLOAT f = [scalar val];
	return f;
}

- (id)setColor:(NSColor *)cv
{
	if ( [self count] != 3 ) {
		[val release];
		val = [NSMutableArray arrayWithCapacity:3];
	}
	
	[self setComponent:0 val:(G3D_SLO_FLOAT)[cv redComponent]];		
	[self setComponent:1 val:(G3D_SLO_FLOAT)[cv greenComponent]];		
	[self setComponent:2 val:(G3D_SLO_FLOAT)[cv blueComponent]];		

	return self;
}

- (NSUInteger)count
{
	return [val count];
}

@end /* G3DSLOColor */


@implementation G3DSLOVisSymDef

@synthesize name;
@synthesize type;
@synthesize storage;
@synthesize detail;
@synthesize spaceName;
@synthesize arrayLen;
@synthesize defaultVals;

- (id)init
{
	[self initWithName:nil];
	return self;
}

- (id)initWithName:(NSString *)aName
{
	name = nil;
	spaceName = nil;
	defaultVals = nil;
	
	[self setName:aName];
	[self setType:G3D_SLO_TYPE_UNKNOWN];
	[self setStorage:G3D_SLO_STOR_UNKNOWN];
	[self setDetail:G3D_SLO_DETAIL_UNKNOWN];
	[self setSpaceName:nil];
	[self setArrayLen:1];
	[self setDefaultVals:nil];
	
	return self;
}

- (void)dealloc
{
	[name release];
	[spaceName release];
	[defaultVals release];
	[super dealloc];
}

- (id)copyWithZone:(NSZone *)zone
{
	G3DSLOVisSymDef *copy = [[[self class] allocWithZone: zone] init];

	[copy setName:name];
	[copy setType:type];
	[copy setStorage:storage];
	[copy setDetail:detail];
	[copy setSpaceName:spaceName];
	[copy setArrayLen:arrayLen];
	[copy setDefaultVals:defaultVals];

	return copy;
}

- (id)initWithCoder:(NSCoder *)decoder
{
	name = [[decoder decodeObjectForKey:@"name"] retain];
	type = [decoder decodeIntForKey:@"type"];
	storage = [decoder decodeIntForKey:@"storage"];
	detail = [decoder decodeIntForKey:@"detail"];
	spaceName = [[decoder decodeObjectForKey:@"spaceName"] retain];
	arrayLen = [decoder decodeIntForKey:@"arrayLen"];
	defaultVals = [[decoder decodeObjectForKey:@"defaultVals"] retain];
	return self;
}

- (void)encodeWithCoder:(NSCoder *)encoder
{
	[encoder encodeObject:name forKey:@"name"];	
	[encoder encodeInt:type forKey:@"type"];	
	[encoder encodeInt:storage forKey:@"storage"];	
	[encoder encodeInt:detail forKey:@"detail"];	
	[encoder encodeObject:spaceName forKey:@"spaceName"];	
	[encoder encodeInt:arrayLen forKey:@"arrayLen"];	
	[encoder encodeObject:defaultVals forKey:@"defaultVals"];	
}

- (BOOL)isValid
{
	if ( name == nil )
		return NO;
	if ( type == G3D_SLO_TYPE_UNKNOWN || type > G3D_SLO_TYPE_MATRIX )
		return NO;
	if ( storage == G3D_SLO_STOR_UNKNOWN )
		return NO;
	if ( detail == G3D_SLO_DETAIL_UNKNOWN )
		return NO;
	if ( arrayLen < 1 || arrayLen != [defaultVals count] )
		return NO;
	
	return YES;
}

@end /* G3DSLOVisSymDef */


@implementation SLOArg

@synthesize symDef;
@synthesize curVals;

- (id)init
{
	[self initWithName:nil];
	return self;
}

- (id)initWithName:(NSString *)aName
{
	symDef = [[G3DSLOVisSymDef alloc] initWithName:aName];
	curVals = [symDef defaultVals];
	return self;
}

- (void)dealloc
{
	[symDef release];
	[curVals release];
	[super dealloc];
}

- (id)copyWithZone:(NSZone *)zone
{
	SLOArg *copy = [[[self class] allocWithZone: zone] init];
	[copy setSymDef:symDef];
	[copy setCurVals:curVals];
	return copy;
}

- (id)initWithCoder:(NSCoder *)decoder
{
	symDef = [[decoder decodeObjectForKey:@"symDef"] retain];
	curVals = [[decoder decodeObjectForKey:@"curVals"] retain];

	return self;
}

- (void)encodeWithCoder:(NSCoder *)encoder
{
	[encoder encodeObject:symDef forKey:@"symDef"];
	[encoder encodeObject:curVals forKey:@"curVals"];
}

- (id)reset
{
	[curVals release];
	curVals = [symDef defaultVals]; /* retain? copy? */
	return self;
}

- (id)setValue:(id)value
{
	return self;
}

- (id)setValues:(NSArray *)values
{
	return self;
}

- (id)setScalarValue:(G3DSLOScalar *)fv
{
	return self;
}

- (id)setStringValue:(NSString *)sv
{
	return self;
}

- (id)setPointComponents:(G3DSLOPoint *)pv
{
	return self;
}

- (id)setPointValue:(G3DSLOPoint *)pv
{
	[self setPointComponents:pv];

	return self;
}

- (id)setVectorValue:(G3DSLOPoint *)vv
{
	[self setPointComponents:vv];

	return self;
}

- (id)setNormalValue:(G3DSLOPoint *)nv
{
	[self setPointComponents:nv];

	return self;
}

- (id)setColorValue:(G3DSLOColor *)cv
{
	return self;
}

- (id)setMatrixValue:(G3DSLOMatrix *)mv
{
	return self;
}

- (id)setScalarValues:(NSArray *)fvp
{
	return self;
}

- (id)setStringValues:(NSArray *)svp
{
	return self;
}

- (id)setPointsComponents:(NSArray *)pvp
{
	return self;
}

- (id)setPointValues:(NSArray *)pvp
{
	[self setPointsComponents:pvp];

	return self;
}

- (id)setVectorValues:(NSArray *)vvp
{
	[self setPointsComponents:vvp];

	return self;
}

- (id)setNormalValues:(NSArray *)nvp
{
	[self setPointsComponents:nvp];

	return self;
}

- (id)setColorValues:(NSArray *)cvp
{
	return self;
}

- (id)setMatrixValues:(NSArray *)mvp
{
	return self;
}

@end /* SLOArg */


@implementation G3DShader

@synthesize shader;
@synthesize shaderType;
@synthesize shaderArgs;

+ (NSMutableArray *)getInstancesOfShader:(NSString *)aShader
{
	NSMutableArray *shaders = [NSMutableArray array];
	/* ToDo open Shader, create instance for each shader in class
	   or one shader if single shader */
	return shaders;
}

- init
{
	[self initWithShader:nil];
	
	return self;
}

- (id)initWithShader:(NSString *)aShader
{
	[self initWithShader:aShader type:G3D_SLO_TYPE_UNKNOWN];
	
	return self;	
}

- (id)initWithShader:(NSString *)aShader type:(G3D_SLO_TYPE)t
{	
	/* ToDo read shader to NSString and call initWithShaderDescription */
	[self initWithShader:aShader type:t description:nil];
	
	return self;
}

- (id)initWithShader:(NSString *)aShader type:(G3D_SLO_TYPE)t description:(NSString *)aShaderDescription
{
	shader = nil;
	[self setShader:aShader];
	shaderType = t;
	shaderArgs = [NSMutableArray array];

	/* ToDo: Read shader description from aShaderDescription
	 (if shader class use type t, G3D_SLO_TYPE_UNKNOWN use 1st shader found),
	 set type to actual type */
	
	return self;
}

- (void)dealloc
{
	[shader release];
	[shaderArgs release];
	[super dealloc];
}

- (id)copyWithZone:(NSZone *)zone
{
	G3DShader *copy = [[[self class] allocWithZone: zone] init];

	[copy setShader:shader];
	[copy setShaderArgs:shaderArgs];
	[copy setShaderType:shaderType];

	return copy;
}

- (id)initWithCoder:(NSCoder *)decoder
{
	shader = [[decoder decodeObjectForKey:@"shader"] retain];
	shaderArgs = [[decoder decodeObjectForKey:@"shaderArgs"] retain];
	shaderType = [decoder decodeIntegerForKey:@"shaderType"];
	
	return self;
}

- (void)encodeWithCoder:(NSCoder *)encoder
{
	[encoder encodeObject:shader forKey:@"shader"];
	[encoder encodeObject:shaderArgs forKey:@"shaderArgs"];
	[encoder encodeInteger:shaderType forKey:@"shaderType"];
}

- (BOOL)valid
{
	return YES;
}

- (NSUInteger)shaderArgCount
{
	return [shaderArgs count];
}

- (NSString *)shaderArgNameAt:(int)i
{
	return nil;
}

- (G3D_SLO_TYPE)shaderArgType:(const NSString *)aName
{
	return G3D_SLO_TYPE_UNKNOWN;
}

- (BOOL)isShaderArg:(const NSString *)aName
{
	return NO;
}

- (id)resetShaderArg:(const NSString *)aName
{
	return self;
}

- (SLOArg *)getShaderArg:(const NSString *)aName;
{
	return nil;
}

- (id)setShaderArg:(const NSString *)aName value:(G3DSLOScalar *)value
{
	return self;
}

- (id)setShaderArg:(const NSString *)aName values:(NSArray *)values;
{
	return self;
}

- (id)setShaderArg:(const NSString *)aName scalarValue:(G3DSLOScalar *)fv
{
	return self;
}

- (id)setShaderArg:(const NSString *)aName stringValue:(NSString *)sv
{
	return self;
}

- (id)setShaderArg:(const NSString *)aName pointValue:(G3DSLOPoint *)pv
{
	return self;
}

- (id)setShaderArg:(const NSString *)aName vectorValue:(G3DSLOPoint *)vv
{
	return self;
}

- (id)setShaderArg:(const NSString *)aName normalValue:(G3DSLOPoint *)nv
{
	return self;
}

- (id)setShaderArg:(const NSString *)aName colorValue:(G3DSLOColor *)cv
{
	return self;
}

- (id)setShaderArg:(const NSString *)aName matrixValue:(G3DSLOMatrix *)mv
{
	return self;
}

- (id)setShaderArg:(const NSString *)aName scalarValues:(NSArray *)fvp
{
	return self;
}

- (id)setShaderArg:(const NSString *)aName stringValues:(NSArray *)svp
{
	return self;
}

- (id)setShaderArg:(const NSString *)aName pointValues:(NSArray *)pvp
{
	return self;
}

- (id)setShaderArg:(const NSString *)aName vectorValues:(NSArray *)vv
{
	return self;
}

- (id)setShaderArg:(const NSString *)aName normalValues:(NSArray *)nv
{
	return self;
}

- (id)setShaderArg:(const NSString *)aName colorValues:(NSArray *)cvp
{
	return self;
}

- (id)setShaderArg:(const NSString *)aName matrixValues:(NSArray *)mv
{
	return self;
}

@end /* G3DShader */
