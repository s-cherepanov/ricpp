//
//  main.m
//  3DKit
//
//  Created by Andreas Pidde on 01.09.08.
//  Copyright __MyCompanyName__ 2008. All rights reserved.
//

#import <Cocoa/Cocoa.h>
#include <3DKit/G3Dkit.h>

int main(int argc, char *argv[])
{
	int result;
    G3DContextManager *ctx = [G3DContextManager new];
	RtContextHandle mainHandle = [ctx mainContext];
	mainHandle = mainHandle;
	result = NSApplicationMain(argc,  (const char **) argv);
	[ctx free];
	return result;
}
