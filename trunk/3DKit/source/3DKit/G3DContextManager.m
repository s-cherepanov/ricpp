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
#import <3Dkit/G3DContextManager.h>

@interface G3DContextHandle : NSObject
{
	RtContextHandle handle;
}
- initWithHandle:(RtContextHandle) aHandle;
@property (readwrite) RtContextHandle handle;

@end


@implementation G3DContextHandle

@synthesize handle;

- initWithHandle: (RtContextHandle) aHandle
{
	[super init];
	handle = aHandle;

	return self;
}

@end


@implementation G3DContextManager

static G3DContextManager *contextManagerSingleton = 0;

+ (id)contextManager
{
	if ( !contextManagerSingleton ) {
		contextManagerSingleton = [[G3DContextManager alloc] init];
	}
	return contextManagerSingleton;
}

- (void)dealloc
{
	id key;

	for ( key in contextTable ) {
		RiContext([[contextTable objectForKey:key] handle]);
		RiEnd();
	}

	if ( mainContext != illContextHandle ) {
		RiContext(mainContext);
		RiEnd();
	}

	[contextTable dealloc];
	mainContext = illContextHandle;
	
	[super dealloc];
}

- (id)init
{
	mainContext = illContextHandle;
	contextTable = [NSMutableDictionary dictionary];
	_nameCnt = 0;

	return self;
}

- (RtContextHandle)mainContext
{
	if ( mainContext == illContextHandle ) {
		RiBegin("glrenderer");
		mainContext = RiGetContext();
	}
	
	return mainContext;
}

- (RtContextHandle)registerContext:(RtContextHandle)aContext withName:(RtToken)name
{
	G3DContextHandle *aHandle;
	NSString *str;
	char buffer[64];
	
	if ( name == RI_NULL ) {
		sprintf(buffer, "__Context_%ld", _nameCnt);
		_nameCnt++;
		name = buffer;
	}

	aHandle = [[G3DContextHandle alloc] initWithHandle:aContext];
	str = [[NSString alloc] initWithCString:name encoding:NSASCIIStringEncoding];
	
	[contextTable setObject:aHandle forKey:str];
	[str release];
	[aHandle release];
	
	return aContext;
}

- (void)removeContext:(RtContextHandle)aContext
{
	NSString *key;

	for ( key in contextTable ) {
		if ( [[contextTable objectForKey:key] handle] == aContext ) {
			[contextTable removeObjectForKey:key];
			return;
		}
	}
}

- (RtContextHandle)contextForKey:(RtToken)name
{
	NSString *str = [[NSString alloc] initWithCString:name encoding:NSASCIIStringEncoding];
	G3DContextHandle *aHandle = [contextTable objectForKey:str];
	[str release];
	
	if ( aHandle == nil )
		return illContextHandle;

	return [aHandle handle];
}


- (RtContextHandle)createContext:(const char *)name
{
	RiBegin("glrenderer");
	return [self registerContext:[self currentContext] withName:name];
}

- (RtContextHandle)createContext:(const char *)name withRenderer:(RtToken)renderer
{
	if ( renderer && !strcmp(renderer, RI_ARCHIVE) )
		RiBegin(RI_NULL);
	else if ( renderer && !strcmp(renderer, RI_DRAFT) )
		RiBegin("glrenderer");
	else
		RiBegin(renderer);
	
	return [self registerContext:[self currentContext] withName:name];
}

- (RtContextHandle)createContext:(const char *)name toFile:(RtToken)ribFileName
{
	RiBegin(ribFileName);

	return [self registerContext:[self currentContext] withName:name];
}

- (void)destroyContext:(RtContextHandle)aContext
{
	if ( aContext == illContextHandle )
		return;
	
	RtContextHandle old = [self setCurrentContext:aContext];
	RiEnd();

	if ( old != illContextHandle && old != aContext ) {
		[self setCurrentContext:old];
	}

	if ( aContext == mainContext )
		mainContext = illContextHandle;
	else
		[self removeContext:aContext];
}

- (void)destroyContextByName:(const char *)name
{
	RtContextHandle aContext = [self contextForKey:name];
	if ( aContext != illContextHandle ) {
		[self destroyContext:aContext];
	}
}

- (RtContextHandle)setCurrentContext:(RtContextHandle)aContext
{
	RtContextHandle lastContext = RiGetContext();
	RiContext(aContext);
	
	return lastContext;
}

- (RtContextHandle)setCurrentContextByName:(RtToken)name
{
	RtContextHandle aContext = [self contextForKey:name];
	if ( aContext != illContextHandle ) {
		return [self setCurrentContext:aContext];
	}
	
	return illContextHandle;
}

- (RtContextHandle)currentContext
{
	return RiGetContext();
}

@end
