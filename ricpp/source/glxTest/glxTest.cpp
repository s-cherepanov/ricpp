#include "stdafx.h"
#include "libglrenderer/libglrenderer.h"

#include <GL/gl.h>
#include <GL/glx.h>

#include <stdio.h>
#include <stdlib.h>

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xresource.h>

static unsigned int win_width = 512;
static unsigned int win_height = 384;

class TFormatFinder : public TIntermediateRenderer {
protected:
   virtual RtVoid doFormat(RtInt width, RtInt height, RtFloat aspect) {
      win_width = width;
      win_height = height;
   }
};

TGLRenderer *ri;
XVisualInfo *visualInfo;
GLXContext ctx;

static const char *file = NULL;

static void redraw( Display *dpy, Window w )
{
   // printf("Redraw event\n");

   /*
   // Check(glXMakeCurrent(XServer, Win, Context), "Can't Make GLX Context Current");
   if ( !glXMakeCurrent( dpy, w, ctx ) ) {
      printf("Can't Make GLX Context Current\n");
      exit(1);
   }
   */

   RtFloat d = (RtFloat)39.6;

   ri->display(
      "glxTest", ri->RI_FRAMEBUFFER, ri->RI_RGB,
      "devicecontext", &dpy, "windowhandle", &w, "visualinfo", &visualInfo,
      ri->RI_NULL
   );
   ri->format(win_width, win_height, 1.0);

   if ( file ) {
      ri->readArchive(file, NULL, ri->RI_NULL);
   } else {
      ri->frameBegin(1);
	 ri->clipping(1.0, 20.0);
	 ri->projection(ri->RI_PERSPECTIVE, ri->RI_FOV, &d, ri->RI_NULL);
	 ri->worldBegin();
            RtFloat intensity = (RtFloat)27.0;
            ri->lightSource(ri->RI_POINTLIGHT, ri->RI_INTENSITY, &intensity, ri->RI_NULL);
	    ri->attributeBegin();
               ri->translate((RtFloat)0.0, (RtFloat)-1.0, (RtFloat)10.0);
               ri->rotate((RtFloat)-135.0, (RtFloat)1.0, (RtFloat)0.0, (RtFloat)0.0);
               ri->surface(ri->RI_MATTE, ri->RI_NULL);
               ri->geometry("teapot", ri->RI_NULL);
	    ri->attributeEnd();
            /*
               glClearColor( 0.5, 0.5, 0.5, 1.0 );
               glClear( GL_COLOR_BUFFER_BIT );
               glColor3f( 1.0, 1.0, 0.0 );
               glRectf( -0.8, -0.8, 0.8, 0.8 );
            */
	 ri->worldEnd();
      ri->frameEnd();
   }
}



static void resize( unsigned int width, unsigned int height )
{
   // printf("Resize event\n");
   // glViewport( 0, 0, width, height );
   // glMatrixMode( GL_PROJECTION );
   // glLoadIdentity();
   // glOrtho( -1.0, 1.0, -1.0, 1.0, -1.0, 1.0 );
   win_width = width;
   win_height = height;
}



static Window make_rgb_db_window( Display *XServer,
				  unsigned int width, unsigned int height )
{
   int attribList[] = { GLX_RGBA,
		    GLX_RED_SIZE, 1,
		    GLX_GREEN_SIZE, 1,
		    GLX_BLUE_SIZE, 1,
		    GLX_DEPTH_SIZE, 1,
		    GLX_DOUBLEBUFFER,
		    GLX_USE_GL,
		    None };

   // int attribList[] = {GLX_RGBA, GLX_DOUBLEBUFFER, true, None};
   int screen;
   Window root;
   Window win;

   screen = DefaultScreen( XServer );
   root = RootWindow( XServer, screen );


   visualInfo = glXChooseVisual(XServer, screen, attribList);
   // Check(visualInfo != NULL, "Can't Choose Visual. I Need RGBA With Double Buffering");
   if ( !visualInfo ) {
      printf("Error: couldn't get an RGB, Double-buffered visual\n");
      exit(1);
   }

   ctx = glXCreateContext(XServer, visualInfo, NULL, true);
   // Check(Context != NULL, "Can't Create GLX Context");
   if ( !ctx ) {
      printf("Can't Create GLX Context\n");
      exit(1);
   }

   // win = XCreateSimpleWindow(XServer, root, 100, 100, width, height, 1, BlackPixel(XServer, screen), WhitePixel(XServer, screen));

   XSetWindowAttributes attr;
   unsigned long mask;

   /* window attributes */
   attr.background_pixel = 0;
   attr.border_pixel = 0;
   attr.colormap = XCreateColormap( XServer, root, visualInfo->visual, AllocNone);
   attr.event_mask = StructureNotifyMask | ExposureMask;
   mask = CWBackPixel | CWBorderPixel | CWColormap | CWEventMask;

   win = XCreateWindow( XServer, root, 0, 0, width, height,
		        0, visualInfo->depth, InputOutput,
		        visualInfo->visual, mask, &attr );

   // XSelectInput(XServer, win, eventMask);
   XMapWindow(XServer, win);


   // Check(glXMakeCurrent(XServer, Win, Context), "Can't Make GLX Context Current");
   if ( !glXMakeCurrent( XServer, win, ctx ) ) {
      printf("Can't Make GLX Context Current\n");
      exit(1);
   }

   // Check(glXQueryExtension(XServer, &GLXErrorBase, &GLXEventBase), "X Server Does Not Support GLX");
   /*
   if ( !glXQueryExtension(XServer, &GLXErrorBase, &GLXEventBase) ) {
      printf("X Server Does Not Support GLX\n");
      exit(1);
   }
   */


   return win;
}


static void event_loop( Display *dpy )
{
   XEvent event;

   while (1) {
      XNextEvent( dpy, &event );

      switch (event.type) {
	 case Expose:
	    redraw( dpy, event.xany.window );
	    break;
	 case ConfigureNotify:
	    resize( event.xconfigure.width, event.xconfigure.height );
	    break;
      }
   }
}



int main( int argc, char *argv[] )
{
   Display *XServer;

   ri = new TGLRenderer;
   if ( !ri )
      return 1;


   if ( argc > 1 ) {
      file = argv[1];

      TFormatFinder fri;
      // fri.initialize();
      fri.errorHandler(fri.errorPrint);
      fri.begin();
         fri.readArchive(file, NULL, fri.RI_NULL);
      fri.end();
   }

   // ri->initialize();
   ri->errorHandler(ri->errorPrint);
   ri->begin();

   // glShadeModel( GL_FLAT );


   XServer = XOpenDisplay(NULL);
   make_rgb_db_window( XServer, win_width, win_height );

   event_loop( XServer );

   ri->end();
   delete ri;
   return 0;
}
