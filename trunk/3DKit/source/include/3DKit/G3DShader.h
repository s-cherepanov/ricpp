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
 
typedef enum {
	G3D_SLO_TYPE_UNKNOWN,

	G3D_SLO_TYPE_POINT,
	G3D_SLO_TYPE_COLOR,
	G3D_SLO_TYPE_SCALAR,
	G3D_SLO_TYPE_STRING,
	G3D_SLO_TYPE_VECTOR,
	G3D_SLO_TYPE_NORMAL,
	G3D_SLO_TYPE_MATRIX,

	/* The following types are shader types */
	G3D_SLO_TYPE_SURFACE,
	G3D_SLO_TYPE_LIGHT,
	G3D_SLO_TYPE_DISPLACEMENT,
	G3D_SLO_TYPE_VOLUME,
	G3D_SLO_TYPE_TRANSFORMATION,
	G3D_SLO_TYPE_IMAGER,

	G3D_SLO_TYPE_DEFORMATION,

	/* Specific volume types */
    G3D_SLO_TYPE_INTERIOR,
	G3D_SLO_TYPE_EXTERIOR,
	G3D_SLO_TYPE_ATMOSPHERE
} G3D_SLO_TYPE;


typedef enum {
	G3D_SLO_STOR_UNKNOWN,
	
	G3D_SLO_STOR_PARAMETER,
	G3D_SLO_STOR_OUTPUTPARAMETER
} G3D_SLO_STORAGE;


typedef enum {
	G3D_SLO_DETAIL_UNKNOWN,
	
	G3D_SLO_DETAIL_VARYING,
	G3D_SLO_DETAIL_UNIFORM
} G3D_SLO_DETAIL;


typedef float G3D_SLO_FLOAT;


@interface G3DSLOScalar : NSObject <NSCopying, NSCoding>
{
	G3D_SLO_FLOAT val;
}

@property (readwrite) G3D_SLO_FLOAT val;

- (id)init;
- (id)initWithG3DSLOScalar:(G3DSLOScalar *)aVal;
- (id)initWithVal:(G3D_SLO_FLOAT)aVal;

@end /* G3DSLOScalar */


@interface G3DSLOMatrix : NSObject <NSCopying, NSCoding>
{
	G3D_SLO_FLOAT val[4][4];
}

- (id)init;
- (id)initWithG3DSLOMatrix:(G3DSLOMatrix *)aVal;
- (id)initWithVal:(G3D_SLO_FLOAT[4][4])aVal;

- (id)setRow:(NSUInteger)row col:(NSUInteger)col val:(G3D_SLO_FLOAT)value;
- (G3D_SLO_FLOAT)getRow:(NSUInteger)row col:(NSUInteger)col;

- (id)setVal:(G3D_SLO_FLOAT[4][4]) aVal;
- (void)getVal:(G3D_SLO_FLOAT[4][4]) aVal;

@end /* G3DSLOMatrix */


@interface G3DSLOPoint : NSObject <NSCopying, NSCoding>
{
	G3D_SLO_FLOAT xVal;
	G3D_SLO_FLOAT yVal;
	G3D_SLO_FLOAT zVal;
}

@property (readwrite) G3D_SLO_FLOAT xVal;
@property (readwrite) G3D_SLO_FLOAT yVal;
@property (readwrite) G3D_SLO_FLOAT zVal;

- (id)init;
- (id)initWithG3DSLOPoint:(G3DSLOPoint *)aVal;
- (id)initWithX:(G3D_SLO_FLOAT)x y:(G3D_SLO_FLOAT)y z:(G3D_SLO_FLOAT)z;

- (id)setX:(G3D_SLO_FLOAT)x y:(G3D_SLO_FLOAT)y z:(G3D_SLO_FLOAT)z;

@end /* G3DSLOPoint */


@interface G3DSLOColor : NSObject <NSCopying, NSCoding>
{
	NSMutableArray *val;
}

@property (readwrite, copy) NSMutableArray *val;

- (id)init;
- (id)initWithG3DSLOColor:(G3DSLOColor *)cv;
- (id)initWithColor:(NSColor *)cv;
- (id)initWithCount:(NSUInteger)numItems components:(G3D_SLO_FLOAT *)theColorComponents;
- (id)initWithCapacity:(NSUInteger)numItems;
- (void)dealloc;

- (id)setComponent:(NSUInteger)i val:(G3D_SLO_FLOAT)value;
- (G3D_SLO_FLOAT)component:(NSUInteger)i;

- (id)setColor:(NSColor *)cv;

- (NSUInteger)count;

@end /* G3DSLOColor */


@interface G3DSLOVisSymDef : NSObject <NSCopying, NSCoding>
{
	NSString *name;                /* name of symbol */
	G3D_SLO_TYPE type;             /* symbol type SLO_TYPE... */
	G3D_SLO_STORAGE storage;       /* symbol storage class SLO_STOR... */
	G3D_SLO_DETAIL detail;         /* symbol variance */
	NSString *spaceName;           /* name in which to interpret symbols value */
	NSUInteger arrayLen;           /* Number of elements in array, 1: no Array or [1] */
	NSMutableArray *defaultVals;   /* Default values */
}

@property (readwrite, copy) NSString *name;
@property (readwrite) G3D_SLO_TYPE type;
@property (readwrite) G3D_SLO_STORAGE storage;
@property (readwrite) G3D_SLO_DETAIL detail;
@property (readwrite, copy) NSString *spaceName;
@property (readwrite) NSUInteger arrayLen;
@property (readwrite, copy) NSMutableArray *defaultVals;

- (id)init;
- (id)initWithName:(NSString *)aName;
- (void)dealloc;

- (BOOL)isValid;

@end /* G3DSLOVisSymDef */


@interface SLOArg : NSObject <NSCopying, NSCoding>
{
	G3DSLOVisSymDef *symDef;      /* Parameter definition and defaults */
	NSMutableArray *curVals;      /* Current values */
}

@property (readwrite, copy) G3DSLOVisSymDef *symDef;
@property (readwrite, copy) NSMutableArray *curVals;

- (id)init;
- (id)initWithName:(NSString *)aName;
- (void)dealloc;

- (id)copyWithZone:(NSZone *)zone;

- (id)reset;
- (id)setValue:(id)value;
- (id)setValues:(NSArray *)values;

- (id)setScalarValue:(G3DSLOScalar *)fv;
- (id)setStringValue:(NSString *)sv;
- (id)setPointValue:(G3DSLOPoint *)pv;
- (id)setVectorValue:(G3DSLOPoint *)pv;
- (id)setNormalValue:(G3DSLOPoint *)pv;
- (id)setColorValue:(G3DSLOColor *)cv;
- (id)setMatrixValue:(G3DSLOMatrix *)mv;

- (id)setScalarValues:(NSArray *)fvp;
- (id)setStringValues:(NSArray *)svp;
- (id)setPointValues:(NSArray *)pvp;
- (id)setVectorValues:(NSArray *)vvp;
- (id)setNormalValues:(NSArray *)nvp;
- (id)setColorValues:(NSArray *)cvp;
- (id)setMatrixValues:(NSArray *)mvp;

@end /* SLOArg */


@interface G3DShader : NSObject <NSCopying, NSCoding>
{
	NSString *shader;
	G3D_SLO_TYPE shaderType;
	NSMutableArray *shaderArgs;
}

@property (readwrite, copy) NSString *shader;
@property (readwrite) G3D_SLO_TYPE shaderType;
@property (readwrite, copy) NSMutableArray *shaderArgs;

/* Shader class may contain many shaders with different types */
+ (NSMutableArray *)getInstancesOfShader:(NSString *)aShader;

- (id)init;
- (id)initWithShader:(NSString *)aShader;
- (id)initWithShader:(NSString *)aShader type:(G3D_SLO_TYPE)t;
- (id)initWithShader:(NSString *)aShader type:(G3D_SLO_TYPE)t description:(NSString *)aShaderDescription;
- (void)dealloc;

- (id)copyWithZone:(NSZone *)zone;

- (id)initWithCoder:(NSCoder *)coder;
- (void)encodeWithCoder:(NSCoder *)coder;

- (BOOL)valid;

- (NSUInteger)shaderArgCount;
- (NSString *)shaderArgNameAt:(int)i;

- (G3D_SLO_TYPE)shaderArgType:(const NSString *)aName;
- (BOOL)isShaderArg:(const NSString *)aName;

- (id)resetShaderArg:(const NSString *)aName;
- (SLOArg *) getShaderArg:(const NSString *)aName;

- (id)setShaderArg:(const NSString *)aName value:(G3DSLOScalar *)value;
- (id)setShaderArg:(const NSString *)aName values:(NSArray *)values;

- (id)setShaderArg:(const NSString *)aName scalarValue:(G3DSLOScalar *)fv;
- (id)setShaderArg:(const NSString *)aName stringValue:(NSString *)sv;
- (id)setShaderArg:(const NSString *)aName pointValue:(G3DSLOPoint *)pv;
- (id)setShaderArg:(const NSString *)aName vectorValue:(G3DSLOPoint *)vv;
- (id)setShaderArg:(const NSString *)aName normalValue:(G3DSLOPoint *)nv;
- (id)setShaderArg:(const NSString *)aName colorValue:(G3DSLOColor *)cv;
- (id)setShaderArg:(const NSString *)aName matrixValue:(G3DSLOMatrix *)mv;

- (id)setShaderArg:(const NSString *)aName scalarValues:(NSArray *)fvp;
- (id)setShaderArg:(const NSString *)aName stringValues:(NSArray *)svp;
- (id)setShaderArg:(const NSString *)aName pointValues:(NSArray *)pvp;
- (id)setShaderArg:(const NSString *)aName vectorValues:(NSArray *)vv;
- (id)setShaderArg:(const NSString *)aName normalValues:(NSArray *)nv;
- (id)setShaderArg:(const NSString *)aName colorValues:(NSArray *)cvp;
- (id)setShaderArg:(const NSString *)aName matrixValues:(NSArray *)mv;

@end /* G3DShader */
