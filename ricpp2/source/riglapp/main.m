//
//  main.m
//

#import <Cocoa/Cocoa.h>
#import <ricpp/ricpp.h>

int main(int argc, char *argv[])
{
	const char *searchPath = ".:&";
    int ret = 0;
	RiCPPControl("searchpath", "renderer", &searchPath, RI_NULL);
	RiBegin("glrenderer");
	// RiBegin("ribwriter");

	RiWorldBegin();
	// Test, no GL Context here
	RiWorldEnd();

	ret = NSApplicationMain(argc,  (const char **) argv);
	RiEnd();
	return ret;
}
