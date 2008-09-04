#import "MyOpenGLView.h"
#import <ricpp/ri/ri.h>

static const float defPivotDepth = 2;
static char *screenAction[2] = {"clear", "finish"};
static RtInt noYes[2] = {0, 1};

// static NSString *fileType = @"rib";

@implementation MyOpenGLView

-(id) resetCamera
{
	// NSLog(@"resetCamera");
	sphi = stheta = 0;
	sdepth = 0;
	pivotDepth = defPivotDepth;
	return self;
}

-(void) awakeFromNib
{
	// NSLog(@"awakeFromNib");
	flagDrawNormals = false;
#ifdef _DEBUG
	flagDrawNormals = true;
#endif // _DEBUG
	[self registerForDraggedTypes:[NSArray arrayWithObjects:NSFilenamesPboardType, nil]];
	[self resetCamera];
}

- (void) loadRibFile: (const char *)filename
{
	RiEnd();
	
	[self resetCamera];

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


- (IBAction)toggleNormals:(id)sender
{
	flagDrawNormals = !flagDrawNormals;
	[self display];
}

- (NSDragOperation)draggingEntered:(id < NSDraggingInfo >)sender
{
	// NSLog(@"draggingEntered");
	return NSDragOperationCopy;
}

- (BOOL)performDragOperation:(id < NSDraggingInfo >)sender
{
	// NSLog(@"performDragOperation");
    NSPasteboard *pboard = [sender draggingPasteboard];
	
    if ( [[pboard types] containsObject:NSFilenamesPboardType] ) {
        NSArray *files = [pboard propertyListForType:NSFilenamesPboardType];
        int numberOfFiles = [files count];
		if ( numberOfFiles > 1 )
			return NO;
		/*
		if ( ![[files lastObject] hasSuffix:fileType] )
			return false;
		*/
		[(NSControl *)statusLine setStringValue:[files lastObject]];
		[self loadRibFile:[[files lastObject] cStringUsingEncoding:NSISOLatin1StringEncoding]];		
    }
    return YES;
}

- (void)concludeDragOperation:(id < NSDraggingInfo >)sender
{
	// NSLog(@"concludeDragOperation");
	[self display];
}

- (void) drawRect: (NSRect) bounds
{
	RtInt width = (RtInt)bounds.size.width;
	RtInt height = (RtInt)bounds.size.height;

	RiCPPControl("glrenderer", "draw-normals", &noYes[flagDrawNormals ? 1 : 0], RI_NULL);
	RiCPPControl("glrenderer", "screen", &screenAction[0], RI_NULL);
//	RiFrameBegin(1);
	RtFloat clippingFront = 0.1F, clippingBack = 1024.F;
	RiClipping(clippingFront, clippingBack);
	RtFloat pixelAspect = 1.0F;
	RiFormat(width, height, pixelAspect);
	RiOption("glrenderer", RI_DISPXRES, &width, RI_DISPYRES, &height, RI_NULL);

	RiWorldBegin(); {
		RiTransformBegin(); {
			char *matrixName[] = {"pre-camera"};
			RiIdentity();
			RiTranslate(0.0F,0.0F,sdepth); // Move back and forth
			RiTranslate(0, 0, pivotDepth); // Move back to previous pos
			
			// Rotation
			RiRotate(-stheta, 1.0, 0.0, 0.0); // Rotate x
			RiRotate(-sphi, 0.0, 1.0, 0.0); // Rotate y
			
			RiTranslate(0, 0, -pivotDepth); // Move to a pivot
			RiCPPControl("state", "string store-transform", matrixName, RI_NULL); // Candidate for RiResource
		} RiTransformEnd();
	} RiWorldEnd();
	
	RiProjection(RI_PERSPECTIVE, RI_NULL);
	
	RiReadArchive("__theArchive", 0, RI_NULL);
//	RiFrameEnd();
	RiCPPControl("glrenderer", "screen", &screenAction[1], RI_NULL);
	RiIdentity();
	RiSynchronize(RI_RESTART);
}


- (void)rightMouseUp:(NSEvent *)theEvent
{
	// NSLog(@"rightMouseUp");
	[self resetCamera];
	[self display];
}

- (void)mouseDragged:(NSEvent *)theEvent
{
	// NSLog(@"mouseDragged");
	if ( !theEvent )
		return;
	sphi += [theEvent deltaX] / 4.0;
	stheta += [theEvent deltaY] / 4.0;
	[self display];
}

- (void)otherMouseDragged:(NSEvent *)theEvent;
{
	// NSLog(@"scrollWheel");
	if ( !theEvent )
		return;
	sdepth -= [theEvent deltaY] / 10.0;
	[self display];
}

- (void)scrollWheel:(NSEvent *)theEvent
{
	// NSLog(@"scrollWheel");
	if ( !theEvent )
		return;
	sdepth += [theEvent deltaY] / 5.0;
	[self display];
}

@end
