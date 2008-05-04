#import "ricpp/riglapp/MyOpenGLView.h"
#import <OpenGL/gl.h>

static void drawAnObject ()
{
	// Opaque
	glColor4f(0.0f, 0.35f, 0.0f, 1.0f);
	glBegin(GL_TRIANGLES);
	{
		glVertex3f( 0.1, 0.5, -0.15);
		glVertex3f( -0.1, -0.3, -0.15);
		glVertex3f( 0.3, -0.3 , -0.15);
	}
	glEnd();

	// Transparent from far to near

	glColor4f(1.0f, 0.85f, 0.35f, 0.5f);
	glBegin(GL_TRIANGLES);
	{
		glVertex3f(  0, 0.5, -0.1);
		glVertex3f( -0.5, -0.3, -0.1);
		glVertex3f( 0.5, -0.3 , -0.1);
	}
	glEnd();

	glColor4f(0.0f, 0.0f, 0.35f, 0.5f);
	glBegin(GL_TRIANGLES);
	{
		glVertex3f( 0.3, 0.5, -0.3);
		glVertex3f( 0.1, -0.3, -0.3);
		glVertex3f( 0.5, -0.3 , -0.3);
	}
	glEnd();
}

@implementation MyOpenGLView

-(void) drawRect: (NSRect) bounds
{
	/* NSLog(@"drawRect"); */

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	glClearDepth(1.0);
	glClearColor(1.0,1.0,1.0,0);

	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	
	glDepthFunc(GL_LEQUAL);
	
	glEnable(GL_ALPHA_TEST);
	glAlphaFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
	drawAnObject();

	glFlush();
}

@end
