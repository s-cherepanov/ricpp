#include <GL/glut.h>

GLfloat light_diffuse[] = { 1.0, 0.0, 0.0, 1.0};
GLfloat light_position[] = { 1.0, 1.0, 1.0, 0.0 };
GLUquadricObj *qobj;

void display(void) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glCallList(1);
	glutSwapBuffers();
}

void gfxinit(void) {
	qobj = gluNewQuadric();
	gluQuadricDrawStyle(qobj, GLU_FILL);
	glNewList(1, GL_COMPILE);
	gluSphere(qobj, 1.0, 20, 20);
	glEndList();
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_DEPTH_TEST);
	glMatrixMode(GL_PROJECTION);
	gluPerspective(40.0, 1.0, 1.0, 10.0);
	glMatrixMode(GL_MODELVIEW);
	gluLookAt(0.0, 0.0, 3.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
	glTranslatef(0.0, 0.0, -1.0);
}

int main(int argc, char *argv[]) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutCreateWindow("sphere");
	glutDisplayFunc(display);
	gfxinit();
	glutMainLoop();
	return 0;
}

