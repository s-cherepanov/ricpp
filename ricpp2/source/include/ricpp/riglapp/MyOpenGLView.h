#import <Cocoa/Cocoa.h>

@interface MyOpenGLView : NSOpenGLView {
	int counter;
    IBOutlet id statusLine;
}
- (void) awakeFromNib;
- (void) drawRect: (NSRect) bounds;
- (IBAction)prevPoly:(id)sender;
- (IBAction)nextPoly:(id)sender;
@end
