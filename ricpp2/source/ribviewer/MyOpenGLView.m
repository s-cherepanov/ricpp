#import "MyOpenGLView.h"
#import <ricpp/ri/ri.h>

@implementation MyOpenGLView

- (void) loadRibFile: (const char *)filename
{
	RiEnd();
	
	RiBegin("glrenderer");
	RiArchiveBegin("__theArchive", RI_NULL);

	if ( filename && *filename ) {
		RiReadArchive(filename, 0, RI_NULL);
	} else {
		RiWorldBegin();
		RiWorldEnd();
	}

	RiArchiveEnd();
}

- (IBAction)loadRib:(id)sender
{
	[self loadRibFile:[[(NSControl *)sender stringValue] cStringUsingEncoding:NSISOLatin1StringEncoding]];
	[self display];
}

- (void) drawRect: (NSRect) bounds
{
	char *screenAction[2] = {"clear", "finish"};
	RtInt width = (RtInt)bounds.size.width;
	RtInt height = (RtInt)bounds.size.height;
	
	RiCPPControl("glrenderer", "screen", &screenAction[0], RI_NULL);
//	RiFrameBegin(1);
	RtFloat clippingFront = 0.1F, clippingBack = 1024.F;
	RiClipping(clippingFront, clippingBack);
	RtFloat pixelAspect = 1.0F;
	RiFormat(width, height, pixelAspect);
	RiOption("glrenderer", RI_DISPXRES, &width, RI_DISPYRES, &height, RI_NULL);
	RiReadArchive("__theArchive", 0, RI_NULL);
//	RiFrameEnd();
	RiCPPControl("glrenderer", "screen", &screenAction[1], RI_NULL);
	RiIdentity();
	RiSynchronize(RI_RESTART);
}

@end
