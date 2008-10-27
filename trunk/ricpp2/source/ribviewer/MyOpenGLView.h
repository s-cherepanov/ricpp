/* MyOpenGLView */

#import <Cocoa/Cocoa.h>

@interface MyOpenGLView : NSOpenGLView
{
    IBOutlet NSTextField *statusLine;

	float sphi, stheta;
	float sdepth;
	float pivotDepth;
	bool flagDrawNormals;
}
- (void) awakeFromNib;
- (id) resetCamera;
- (void) loadRibFile: (const char *) filename;
- (void) drawRect: (NSRect) bounds ;
- (IBAction)loadRib:(id)sender;
- (IBAction)toggleNormals:(id)sender;

- (NSDragOperation)draggingEntered:(id < NSDraggingInfo >)sender;
- (BOOL)performDragOperation:(id < NSDraggingInfo >)sender;
- (void)concludeDragOperation:(id < NSDraggingInfo >)sender;

- (void)rightMouseUp:(NSEvent *)theEvent;
- (void)mouseDragged:(NSEvent *)theEvent;
- (void)scrollWheel:(NSEvent *)theEvent;
- (void)otherMouseDragged:(NSEvent *)theEvent;
@end
