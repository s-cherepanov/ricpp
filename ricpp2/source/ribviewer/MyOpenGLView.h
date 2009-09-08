/* MyOpenGLView */

#import <Cocoa/Cocoa.h>
#import <ricpp/ricpp.h>

@interface MyOpenGLView : NSOpenGLView <NSWindowDelegate>
{
    IBOutlet NSTextField *statusLine;

	float sphi, stheta;
	float sdepth;
	float pivotDepth;
	bool flagDrawNormals;
	RtContextHandle contextHandle;
}
- (void) awakeFromNib;
- (void) closeContext;
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


- (void)windowWillClose:(NSNotification *)notification;
@end
