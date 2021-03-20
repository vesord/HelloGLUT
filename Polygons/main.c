/**
	\author Vesord
	\date march 2021
	\brief simple 3D visualisation of polygons made by glBegin() glEnd()

	key f - show cull face
	key b - show cull back
	key 1 - show/hide figure 1
	key 2 - show/hide figure 2
 */

#define GL_SILENCE_DEPRECATION
#include <GLUT/glut.h>

GLfloat R=640.0/480; //Форматное соотношение
GLfloat w=640;       //Ширина мирового окна
GLfloat h=480;       //Высота мирового окна

GLint showFig1 = 1;
GLint showFig2 = 0;

void init(void) {
	glClearColor(0.3f, 0.3f, 0.3f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_NORMALIZE);
	glEnable(GL_CULL_FACE);

	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60.f, R, 2., 200);	// switch
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void reshape(GLsizei W, GLsizei H) {
	if(R>W/H) glViewport(0,0,W,W/R);
	else glViewport(0,0,H*R,H);
}

void keyFunction(unsigned char key, int x, int y) {
	switch (key) {
		case 'f': glCullFace(GL_FRONT); break;
		case 'b': glCullFace(GL_BACK); break;
		case '1': showFig1 = !showFig1; break;
		case '2': showFig2 = !showFig2; break;
		default: break;
	}
	glutPostRedisplay();
}

void fig1() {
	glPushMatrix();
// грань f0
	glColor3f(0.8f, 0.2f, 0.1f);
	glBegin(GL_POLYGON);
	glNormal3f(0.577, 0.577, 0.577);
	glVertex3f(1, 0, 0);
	glVertex3f(0, 1, 0);
	glVertex3f(0, 0, 1);
	glEnd();
// грань f1
	glColor3f(0.1f, 0.2f, 0.6f);
	glBegin(GL_POLYGON);
	glNormal3f(0, 0, -1);
	glVertex3f(0, 0, 0);
	glVertex3f(0, 1, 0);
	glVertex3f(1, 0, 0);
	glEnd();
// грань f2
	glColor3f(0.5f, 0.1f, 0.7f);
	glBegin(GL_POLYGON);
	glNormal3f(-1, 0, 0);
	glVertex3f(0, 0, 0);
	glVertex3f(0, 0, 1);
	glVertex3f(0, 1, 0);
	glEnd();
// грань f3
	glColor3f(0.3f, 0.7f, 0.1f);
	glBegin(GL_POLYGON);
	glNormal3f(0, -1, 0);
	glVertex3f(1, 0, 0);
	glVertex3f(0, 0, 1);
	glVertex3f(0, 0, 0);
	glEnd();
	glPopMatrix();
}

void fig2() {
	// back edge
	glColor3f(0.8f, 0.8f, 0.8f);
	glBegin(GL_POLYGON);
	glNormal3f(0.f, 0.f, -1.f);
	glVertex3f(0.f, 0.f, 0.f);
	glVertex3f(1.f, 1.f, 0.f);
	glVertex3f(2.f, 1.f, 0.f);
	glVertex3f(2.f, 0.f, 0.f);
	glEnd();
	// right edge
	glColor3f(0.1f, 0.7f, 1.f);
	glBegin(GL_POLYGON);
	glNormal3f(1.f, 0.f, 0.f);
	glVertex3f(2.f, 0.f, 0.f);
	glVertex3f(2.f, 1.f, 0.f);
	glVertex3f(2.f, 1.f, 1.f);
	glVertex3f(2.f, 0.f, 1.f);
	glEnd();
	// top edge
	glColor3f(0.7f, 0.1f, 0.4f);
	glBegin(GL_POLYGON);
	glNormal3f(0.f, 1.f, 0.f);
	glVertex3f(2.f, 1.f, 0.f);
	glVertex3f(1.f, 1.f, 0.f);
	glVertex3f(1.f, 1.f, 1.f);
	glVertex3f(2.f, 1.f, 1.f);
	glEnd();
	// left edge
	glColor3f(0.1f, 0.9f, 0.7f);
	glBegin(GL_POLYGON);
	glNormal3f(-1.f, 1.f, 0.f);
	glVertex3f(1.f, 1.f, 0.f);
	glVertex3f(0.f, 0.f, 0.f);
	glVertex3f(0.f, 0.f, 1.f);
	glVertex3f(1.f, 1.f, 1.f);
	glEnd();
	// front edge
	glColor3f(0.8f, 0.4f, 0.9f);
	glBegin(GL_POLYGON);
	glNormal3f(0.f, 0.f, 1.f);
	glVertex3f(1.f, 1.f, 1.f);
	glVertex3f(0.f, 0.f, 1.f);
	glVertex3f(2.f, 0.f, 1.f);
	glVertex3f(2.f, 1.f, 1.f);
	glEnd();
	// bottom edge
	glColor3f(0.8f, 0.4f, 0.9f);
	glBegin(GL_POLYGON);
	glNormal3f(0.f, -1.f, 0.f);
	glVertex3f(0.f, 0.f, 0.f);
	glVertex3f(2.f, 0.f, 0.f);
	glVertex3f(2.f, 0.f, 1.f);
	glVertex3f(0.f, 0.f, 1.f);
	glEnd();
}

void display() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glColor3f(1.f, 0.f, 0.f);
	glLoadIdentity();
	gluLookAt(0.5, 3., 3., 0., 0., 0., 0., 1., 0.);
	if (showFig1)
		fig1();
	if (showFig2)
		fig2();
	glFlush();
	glutSwapBuffers();
}

int main(int argc, char **argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(640, 480);
	glutInitWindowPosition(20, 20);
	glutCreateWindow("Myprog");
	glutReshapeFunc(reshape);
	glutDisplayFunc(display);
	glutIdleFunc(display);
	glutKeyboardFunc(keyFunction);
	init();

	glutMainLoop();
}