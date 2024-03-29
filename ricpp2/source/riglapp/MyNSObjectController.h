#import <Cocoa/Cocoa.h>

@interface MyNSObjectController : NSObjectController {
    IBOutlet id clippingEnable;
    IBOutlet id clippingHither;
    IBOutlet id clippingYonder;
    IBOutlet id cropBottom;
    IBOutlet id cropEnable;
    IBOutlet id cropLeft;
    IBOutlet id cropRight;
    IBOutlet id cropTop;
    IBOutlet id displayEnable;
    IBOutlet id displayOriginX;
    IBOutlet id displayOriginY;
    IBOutlet id formatEnable;
    IBOutlet id formatHt;
    IBOutlet id formatPixelAspect;
    IBOutlet id formatWid;
    IBOutlet id frameAspect;
    IBOutlet id frameEnable;
    IBOutlet id frameAspectHt;
    IBOutlet id frameAspectWid;
    IBOutlet id frameAspectCalc;
    IBOutlet id openGLView;
    IBOutlet id projectionEnable;
    IBOutlet id projectionFOVDial;
    IBOutlet id projectionFOVEnable;
    IBOutlet id projectionFOVValue;
    IBOutlet id projectionType;
    IBOutlet id projectionViewplane;
    IBOutlet id screenBottom;
    IBOutlet id screenEnable;
    IBOutlet id screenLeft;
    IBOutlet id screenRight;
    IBOutlet id screenTop;
}

- (IBAction)formatCheck:(id)sender;
- (IBAction)formatWidthChanged:(id)sender;
- (IBAction)formatHeightChanged:(id)sender;
- (IBAction)formatPixelAspectChanged:(id)sender;

- (IBAction)displayCheck:(id)sender;
- (IBAction)displayOriginXChanged:(id)sender;
- (IBAction)displayOriginYChanged:(id)sender;

- (IBAction)clippingCheck:(id)sender;
- (IBAction)clippingHitherChanged:(id)sender;
- (IBAction)clippingYonderChanged:(id)sender;

- (IBAction)frameAspectCheck:(id)sender;
- (IBAction)frameAspectChanged:(id)sender;
- (IBAction)frameAspectCalc:(id)sender;

- (IBAction)screenCheck:(id)sender;
- (IBAction)screenTopChanged:(id)sender;
- (IBAction)screenLeftChanged:(id)sender;
- (IBAction)screenRightChanged:(id)sender;
- (IBAction)screenBottomChanged:(id)sender;

- (IBAction)cropCheck:(id)sender;
- (IBAction)cropTopChanged:(id)sender;
- (IBAction)cropLeftChanged:(id)sender;
- (IBAction)cropRightChanged:(id)sender;
- (IBAction)cropBottomChanged:(id)sender;

- (IBAction)projectionCheck:(id)sender;
- (IBAction)projectionSelect:(id)sender;
- (IBAction)fovCheck:(id)sender;
- (IBAction)fovDialChanged:(id)sender;
- (IBAction)fovChanged:(id)sender;

/* ----- */

- (void) updateView;
- (void) awakeFromNib;

- (void) formatWidth:(float)wid height:(float)ht pixelAspect:(float)pa;
- (int) formatEnabled;
- (float) formatWidth;
- (float) formatHeight;
- (float) formatPixelAspect;

- (void) displayOriginX:(float)originX originY:(float)originY;
- (int) displayEnabled;
- (float) originX;
- (float) originY;

- (int) clippingEnabled;
- (void) clippingHither:(float)hither yonder:(float)yonder;
- (float) clippingHither;
- (float) clippingYonder;

- (int) frameAspectEnabled;
- (void) frameAspect:(float)fa;
- (float) frameAspect;
- (float) frameAspectWid;
- (float) frameAspectHt;

- (int) screenEnabled;
- (void) screenLeft:(float)left right:(float)right bottom:(float)bottom top:(float)top;
- (float) screenLeft;
- (float) screenRight;
- (float) screenBottom;
- (float) screenTop;

- (int) cropEnabled;
- (void) cropLeft:(float)left right:(float)right bottom:(float)bottom top:(float)top;
- (float) cropLeft;
- (float) cropRight;
- (float) cropBottom;
- (float) cropTop;

- (int) projectionEnabled;
- (void) projectionType:(const char *)type;
- (const char *) projectionType;
- (int) fovEnabled;
- (void) fov:(float)fov;
- (float) fov;

@end
