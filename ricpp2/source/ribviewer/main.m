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

	RiBegin("glrenderer");
	RiArchiveBegin("__theArchive", RI_NULL);
	if ( argc > 1 ) {
		RiReadArchive(argv[1], 0, RI_NULL);
	} else {
		RiWorldBegin();
		RiWorldEnd();
	}
	RiArchiveEnd();
    result = NSApplicationMain(argc,  (const char **) argv);
	RiEnd();
	return result;
}
