////  main.m//#import <Cocoa/Cocoa.h>#import "ricpp/ri/ri.h"int main(int argc, char *argv[]){	const char *searchPath = ".:&";    int ret = 0;	RiCPPControl("searchpath", "renderer", &searchPath, RI_NULL);	RiBegin("glrenderer");	RiWorldBegin();	// No GL Context here	RiWorldEnd();	ret = NSApplicationMain(argc,  (const char **) argv);	RiEnd();	return ret;}