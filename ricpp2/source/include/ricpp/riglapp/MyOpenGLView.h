#import <Cocoa/Cocoa.h>

@interface MyOpenGLView : NSOpenGLView {
	int testprog;
    IBOutlet id statusLine;
}
- (void) drawRect: (NSRect) bounds;
- (IBAction)nextPoly:(id)sender;
@end
