#import <Cocoa/Cocoa.h>

@interface MyOpenGLView : NSOpenGLView {
	int counter;
	float sphi, stheta;
	float sdepth;
	float pivotDepth;

    IBOutlet id panelController;
    IBOutlet id statusLine;
}
- (id)init;
- (void) awakeFromNib;
- (id) resetCamera;
- (void) drawRect: (NSRect) bounds;
- (IBAction)nextPoly:(id)sender;
- (IBAction)prevPoly:(id)sender;

- (void)rightMouseUp:(NSEvent *)theEvent;
- (void)mouseDragged:(NSEvent *)theEvent;
- (void)scrollWheel:(NSEvent *)theEvent;
@end
