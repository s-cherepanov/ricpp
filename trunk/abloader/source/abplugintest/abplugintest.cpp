/*
 *  abplugintest.cpp
 *  ABLoader
 *
 *  Created by Andreas Pidde on 09.08.08.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */
#include "API/Augenblick.h"
#include "abloader/ABLoaderPlugin/ABLoaderPlugin.h"

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

static ABLoaderPlugin *plugin = 0;

void display(void)
{
	AB::Augenblick::Instance()->requestNewRendering();
}


void mouse(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		AB::Augenblick::Instance()->setMousePosition(x,y);
	}
}

void motion(int x, int y)
{
    AB::Augenblick::Instance()->mouseMovedTo(x,y);
    AB::Augenblick::Instance()->requestNewRendering();
}

int main (int argc, char *argv[])
{
	if ( argc < 2 )
		return 1;
	
	glutInit(&argc, argv);
	glutInitDisplayMode (GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize (512, 512); 
	glutInitWindowPosition (100, 100);
	glutCreateWindow ("ABLoader Test");
	
	plugin = (ABLoaderPlugin *)Instance();

	if ( plugin ) {
		plugin->load(argv[1]);
	} else {
		return 0;
	}

	glutDisplayFunc(display); 
   	glutMouseFunc(mouse);
   	glutMotionFunc(motion);
   	glutMainLoop();
	
	ABLoaderPlugin::deleteInstance(plugin);
	return 0;
}
