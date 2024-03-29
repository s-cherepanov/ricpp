#import "MyNSObjectController.h"
#import <ricpp/ricpp.h>

static const float pi_360 = 3.1415926535897932384626433832795/360.0;

/* ------------------------------------------------------------------------- */

@implementation MyNSObjectController

/* ------------------------------------------------------------------------- */

- (IBAction)formatCheck:(id)sender
{
	/* NSLog(@"formatCheck"); */
	if ( [self formatEnabled] ) {
		[(NSTextField *)formatWid setEnabled:YES];
		[(NSTextField *)formatHt setEnabled:YES];
		[(NSTextField *)formatPixelAspect setEnabled:YES];
	} else {
		[(NSTextField *)formatWid setEnabled:NO];
		[(NSTextField *)formatHt setEnabled:NO];
		[(NSTextField *)formatPixelAspect setEnabled:NO];
		[self updateView];
	}
}

- (IBAction)formatWidthChanged:(id)sender
{
	[self updateView];
}

- (IBAction)formatHeightChanged:(id)sender
{
	[self updateView];
}

- (IBAction)formatPixelAspectChanged:(id)sender
{
	[self updateView];
}

/* ----- */

- (IBAction)displayCheck:(id)sender
{
	/* NSLog(@"displayCheck"); */
	if ( [self displayEnabled] ) {
		[(NSTextField *)displayOriginX setEnabled:YES];
		[(NSTextField *)displayOriginY setEnabled:YES];
	} else {
		[(NSTextField *)displayOriginX setEnabled:NO];
		[(NSTextField *)displayOriginY setEnabled:NO];
		[self updateView];
	}
}

- (IBAction)displayOriginXChanged:(id)sender
{
	[self updateView];
}

- (IBAction)displayOriginYChanged:(id)sender
{
	[self updateView];
}

/* ----- */

- (IBAction)clippingCheck:(id)sender
{
	/* NSLog(@"clippingCheck"); */
	if ( [self clippingEnabled] ) {
		[(NSTextField *)clippingHither setEnabled:YES];
		[(NSTextField *)clippingYonder setEnabled:YES];
	} else {
		[(NSTextField *)clippingHither setEnabled:NO];
		[(NSTextField *)clippingYonder setEnabled:NO];
		[self updateView];
	}
}

- (IBAction)clippingHitherChanged:(id)sender
{
	[self updateView];
}

- (IBAction)clippingYonderChanged:(id)sender
{
	[self updateView];
}

/* ----- */

- (IBAction)frameAspectCheck:(id)sender
{
	/* NSLog(@"frameAspectCheck"); */
	if ( [self frameAspectEnabled] ) {
		[(NSTextField *)frameAspect setEnabled:YES];
		[(NSTextField *)frameAspectHt setEnabled:YES];
		[(NSTextField *)frameAspectWid setEnabled:YES];
		[(NSButton *)frameAspectCalc setEnabled:YES];
	} else {
		[(NSTextField *)frameAspect setEnabled:NO];
		[(NSTextField *)frameAspectHt setEnabled:NO];
		[(NSTextField *)frameAspectWid setEnabled:NO];
		[(NSButton *)frameAspectCalc setEnabled:NO];
		[self updateView];
	}
}

- (IBAction)frameAspectChanged:(id)sender;
{
	[self updateView];
}

- (IBAction)frameAspectCalc:(id)sender;
{
	float wid = [self frameAspectWid];
	float ht = [self frameAspectHt];
	if ( ht != 0 && wid != 0 ) {
		[(NSTextField *)frameAspect setFloatValue:wid/ht];
	}
	[self updateView];
}

/* ----- */

- (IBAction)screenCheck:(id)sender
{
	/* NSLog(@"screenCheck"); */
	if ( [self screenEnabled] ) {
		[(NSTextField *)screenTop setEnabled:YES];
		[(NSTextField *)screenLeft setEnabled:YES];
		[(NSTextField *)screenRight setEnabled:YES];
		[(NSTextField *)screenBottom setEnabled:YES];
	} else {
		[(NSTextField *)screenTop setEnabled:NO];
		[(NSTextField *)screenLeft setEnabled:NO];
		[(NSTextField *)screenRight setEnabled:NO];
		[(NSTextField *)screenBottom setEnabled:NO];
		[self updateView];
	}
}

- (IBAction)screenTopChanged:(id)sender
{
	[self updateView];
}

- (IBAction)screenLeftChanged:(id)sender
{
	[self updateView];
}

- (IBAction)screenRightChanged:(id)sender
{
	[self updateView];
}

- (IBAction)screenBottomChanged:(id)sender
{
	[self updateView];
}

/* ----- */

- (IBAction)cropCheck:(id)sender
{
	/* NSLog(@"cropCheck"); */
	if ( [self cropEnabled] ) {
		[(NSTextField *)cropTop setEnabled:YES];
		[(NSTextField *)cropLeft setEnabled:YES];
		[(NSTextField *)cropRight setEnabled:YES];
		[(NSTextField *)cropBottom setEnabled:YES];
	} else {
		[(NSTextField *)cropTop setEnabled:NO];
		[(NSTextField *)cropLeft setEnabled:NO];
		[(NSTextField *)cropRight setEnabled:NO];
		[(NSTextField *)cropBottom setEnabled:NO];
		[self updateView];
	}
}

- (IBAction)cropTopChanged:(id)sender
{
	[self updateView];
}

- (IBAction)cropLeftChanged:(id)sender
{
	[self updateView];
}

- (IBAction)cropRightChanged:(id)sender
{
	[self updateView];
}

- (IBAction)cropBottomChanged:(id)sender
{
	[self updateView];
}

/* ----- */

- (IBAction)projectionCheck:(id)sender
{
	if ( [self projectionEnabled] ) {
		[(NSComboBox *)projectionType setEnabled:YES];		
	} else {
		[(NSComboBox *)projectionType setEnabled:NO];
		[(NSButton *)projectionFOVEnable setEnabled:NO];		
		[(NSSlider *)projectionFOVDial setEnabled:NO];		
		[(NSTextField *)projectionFOVValue setEnabled:NO];		

		[(NSButton *)projectionFOVEnable setIntValue:0];		
		[self fov:90.0];
		[self projectionType:RI_ORTHOGRAPHIC];

		[self updateView];
	}
}

- (IBAction)projectionSelect:(id)sender
{
	RtToken proj = [self projectionType];
	
	if ( proj == RI_PERSPECTIVE ) {
		[(NSButton *)projectionFOVEnable setEnabled:YES];
	} else {
		[(NSButton *)projectionFOVEnable setEnabled:NO];
		[(NSButton *)projectionFOVEnable setIntValue:0];
	}
	
	[self fovCheck:sender];
}

- (IBAction)fovCheck:(id)sender
{
	if ( [self fovEnabled] ) {
		[(NSSlider *)projectionFOVDial setEnabled:YES];		
		[(NSTextField *)projectionFOVValue setEnabled:YES];		
	} else {
		[(NSSlider *)projectionFOVDial setEnabled:NO];		
		[(NSTextField *)projectionFOVValue setEnabled:NO];
		[self fov:90.0];
		[self updateView];
	}
}

- (IBAction)fovDialChanged:(id)sender
{
	float fov = [(NSSlider *)projectionFOVDial floatValue];
	[self fov:fov];
	[self updateView];
}

- (IBAction)fovChanged:(id)sender
{
	float fov = [(NSTextField *)projectionFOVValue floatValue];
	if ( fov > 179.9 )
		fov = 179.9;
	if ( fov < 0.1 )
		fov = 0.1;
	[self fov:fov];
	[self updateView];
}

/* ------------------------------------------------------------------------- */

-(void) updateView
{
	[openGLView display];
}

-(void) awakeFromNib
{
	/* NSLog(@"awakeFromNib"); */
	[self fov:90.0];
	[self projectionType:RI_ORTHOGRAPHIC];
}

/* ----- */

- (void) formatWidth:(float)wid height:(float)ht pixelAspect:(float)pa;
{
	/* NSLog(@"MyNSObjectController:format"); */

    if ( [self formatEnabled] == 0 ) {
		[(NSTextField *)formatWid setFloatValue:wid];
		[(NSTextField *)formatHt setFloatValue:ht];
		[(NSTextField *)formatPixelAspect setFloatValue:pa];
	}	
}

- (int) formatEnabled
{
	return [(NSButton *)formatEnable intValue];
}

- (float) formatWidth
{
	return [(NSTextField *)formatWid floatValue];
}

- (float) formatHeight
{
	return [(NSTextField *)formatHt floatValue];
}

- (float) formatPixelAspect
{
	return [(NSTextField *)formatPixelAspect floatValue];
}

/* ----- */

- (int) displayEnabled
{
	return [(NSButton *)displayEnable intValue];
}

- (void) displayOriginX:(float)originX originY:(float)originY
{
    if ( [self displayEnabled] == 0 ) {
		[(NSTextField *)displayOriginX setFloatValue:originX];
		[(NSTextField *)displayOriginY setFloatValue:originY];
	}
}

- (float) originX
{
	return [(NSTextField *)displayOriginX floatValue];
}

- (float) originY
{
    return [(NSTextField *)displayOriginY floatValue];
}

/* ----- */

- (int) clippingEnabled
{
	return [(NSButton *)clippingEnable intValue];
}

- (void) clippingHither:(float)hither yonder:(float)yonder
{
    if ( [self clippingEnabled] == 0 ) {
		[(NSTextField *)clippingHither setFloatValue:hither];
		[(NSTextField *)clippingYonder setFloatValue:yonder];
	}
}

- (float) clippingHither
{
	return [(NSTextField *)clippingHither floatValue];
}

- (float) clippingYonder
{
    return [(NSTextField *)clippingYonder floatValue];
}

/* ----- */

- (int) frameAspectEnabled
{
	return [(NSButton *)frameEnable intValue];
}

- (void) frameAspect:(float)fa
{
    if ( [self frameAspectEnabled] == 0 ) {
		[(NSTextField *)frameAspect setFloatValue:fa];
	}
}

- (float) frameAspect
{
	return [(NSTextField *)frameAspect floatValue];
}

- (float) frameAspectWid
{
	return [(NSTextField *)frameAspectWid floatValue];
}

- (float) frameAspectHt
{
	return [(NSTextField *)frameAspectHt floatValue];
}

/* ----- */

- (int) screenEnabled
{
	return [(NSButton *)screenEnable intValue];
}

- (void) screenLeft:(float)left right:(float)right bottom:(float)bottom top:(float)top
{
    if ( [self screenEnabled] == 0 ) {
		[(NSTextField *)screenLeft setFloatValue:left];
		[(NSTextField *)screenRight setFloatValue:right];
		[(NSTextField *)screenBottom setFloatValue:bottom];
		[(NSTextField *)screenTop setFloatValue:top];
	}
}

- (float) screenLeft
{
	return [(NSTextField *)screenLeft floatValue];
}

- (float) screenRight
{
	return [(NSTextField *)screenRight floatValue];
}

- (float) screenBottom
{
	return [(NSTextField *)screenBottom floatValue];
}

- (float) screenTop
{
	return [(NSTextField *)screenTop floatValue];
}

/* ----- */

- (int) cropEnabled
{
	return [(NSButton *)cropEnable intValue];
}

- (void) cropLeft:(float)left right:(float)right bottom:(float)bottom top:(float)top
{
    if ( [self cropEnabled] == 0 ) {
		[(NSTextField *)cropLeft setFloatValue:left];
		[(NSTextField *)cropRight setFloatValue:right];
		[(NSTextField *)cropBottom setFloatValue:bottom];
		[(NSTextField *)cropTop setFloatValue:top];
	}
}

- (float) cropLeft
{
	return [(NSTextField *)cropLeft floatValue];
}

- (float) cropRight
{
	return [(NSTextField *)cropRight floatValue];
}

- (float) cropBottom
{
	return [(NSTextField *)cropBottom floatValue];
}

- (float) cropTop
{
	return [(NSTextField *)cropTop floatValue];
}

/* ----- */

- (int) projectionEnabled
{
	return [(NSButton *)projectionEnable intValue];
}

- (void) projectionType:(const char *)type
{
	if ( type == RI_ORTHOGRAPHIC ) {
		[(NSComboBox *)projectionType  selectItemAtIndex:0];
	} else if ( type == RI_PERSPECTIVE ) {
		[(NSComboBox *)projectionType  selectItemAtIndex:1];
	} else {
		[(NSComboBox *)projectionType  selectItemAtIndex:2];
	}
}

- (const char *)projectionType
{
	NSString *ptype = [(NSComboBox *)projectionType stringValue];

	if ( [ptype isEqual:@"Perspective"] )
		return RI_PERSPECTIVE;
	if ( [ptype isEqual:@"Orthographic"] )
		return RI_ORTHOGRAPHIC;
	
	return RI_NULL;
}

- (int) fovEnabled
{
	return [(NSButton *)projectionFOVEnable intValue];
}

- (void) fov:(float)fov
{
	[(NSSlider *)projectionFOVDial setFloatValue:fov];
	[(NSTextField *)projectionFOVValue setFloatValue:fov];
	float pv = (float)1.0 / (float)tan(fov * pi_360);
	[(NSTextField *)projectionViewplane setFloatValue:pv];
}

- (float) fov
{
	return [(NSSlider *)projectionFOVDial floatValue];
}

@end
