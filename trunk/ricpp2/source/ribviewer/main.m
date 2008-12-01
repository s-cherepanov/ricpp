//
//  main.m
//  Rib Viewer
//
//  Created by Andreas Pidde on 16.10.06.
//  Copyright RiCPP 2006. All rights reserved.
//

#import <Cocoa/Cocoa.h>
#import <ricpp/ri/ri.h>

int main(int argc, char *argv[])
{
	const char *searchPath = ".:&";
	int result;

  	RiCPPControl("searchpath", "renderer", &searchPath, RI_NULL);

    result = NSApplicationMain(argc,  (const char **) argv);

	return result;
}
