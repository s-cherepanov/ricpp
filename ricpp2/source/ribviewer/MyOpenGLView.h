/* MyOpenGLView */

#import <Cocoa/Cocoa.h>

@interface MyOpenGLView : NSOpenGLView
{
}
- (void) loadRibFile: (const char *) filename;
- (void) drawRect: (NSRect) bounds ;
- (IBAction)loadRib:(id)sender;
@end
