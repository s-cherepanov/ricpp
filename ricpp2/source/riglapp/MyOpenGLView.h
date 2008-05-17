#import <Cocoa/Cocoa.h>

@interface MyOpenGLView : NSOpenGLView {
	int counter;
    IBOutlet id panelController;
    IBOutlet id statusLine;
}
- (void) awakeFromNib;
- (void) drawRect: (NSRect) bounds;
- (IBAction)nextPoly:(id)sender;
- (IBAction)prevPoly:(id)sender;
@end
