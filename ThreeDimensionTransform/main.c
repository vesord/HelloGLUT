/**
	\author Vesord
 	\date march 2021
	\brief simple 3D visualisation of glutFigures

	key x - rotate clockwise
 	key y - rotate contr clockwise
 	key s - enable/disable autoscale on x and y axis
 	key r - enable/disable autorotation around y axis
 	key 1 - show/hide figure 1
 	key 0 - show/hide figure 0
 	key e - ???

 	mouse Left - increase x and y scale
 	mouse Right - decrease x and y scale
 */

#define GL_SILENCE_DEPRECATION
#include <GLUT/glut.h>
#include <math.h>

GLfloat R=640.0/480; //Форматное соотношение
GLfloat w=640;       //Ширина мирового окна
GLfloat h=480;       //Высота мирового окна
GLfloat l, r, b, t;  //Параметры мирового окна

GLdouble radius = 10.;
GLdouble eyex = 10.;
GLdouble eyez = 0.;
GLdouble eyey = 1.5;
GLfloat scalex = 1.f;
GLfloat scaley = 1.f;
GLfloat scalez = 1.f;

GLfloat angle = 0.f;
GLfloat dAngleStep = 0.02f;
GLfloat dScaleStep = 0.1f;

GLint showFig0 = 1;
GLint showFig1 = 0;
GLint enableAutoRotate = 0;
GLint enableAutoScale = 0;
GLint easterEggFigure = 0;

void init(void) {
	h=w/R; l=-w/80; r=w/80; b=-h/80; t=h/80;
	glClearColor(0.3f, 0.3f, 0.3f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
//	glOrtho(l, r, b, t, -1000.0, 1000.0);				// switch
	gluPerspective(45.f, R, 2., 200);	// switch
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void increaseAngle(GLfloat step) {
	if (angle + step < -M_PI)
		angle = M_PI;
	else if (angle + step > M_PI)
		angle = -M_PI;
	else
		angle += step;
}

void keyFunction(unsigned char key, int x, int y) {
	switch (key) {
		case 'x': increaseAngle(dAngleStep); break;
		case 'y': increaseAngle(-dAngleStep); break;
		case 'r': enableAutoRotate = !enableAutoRotate; break;
		case 's': enableAutoScale = !enableAutoScale; break;
		case '1': showFig1 = !showFig1; break;
		case '0': showFig0 = !showFig0; break;
		case 'e': easterEggFigure = !easterEggFigure; break;
		default: break;
	}
	glutPostRedisplay();
}

void increaseScale(GLfloat mult) {
	scalex *= mult;
	scaley *= mult;
}

void mouseFunction(int button, int state, int x, int y) {
	if (state != GLUT_UP)
		return;
	switch (button) {
		case GLUT_LEFT_BUTTON:
			increaseScale(1. + dScaleStep);
			break;
		case GLUT_RIGHT_BUTTON:
			increaseScale(1. - dScaleStep);
			break;
		default: break;
	}
	glutPostRedisplay();
}

void reshape(GLsizei W, GLsizei H) {
	if(R>W/H) glViewport(0,0,W,W/R);
	else glViewport(0,0,H*R,H);
}

void countEyes() {
	eyex = sin(angle) * radius;
	eyez = cos(angle) * radius;
}

void fig(GLfloat x, GLfloat y, GLfloat z) {
	glPushMatrix();
	glTranslatef(x, y, x);
	glScalef(scalex, scaley, scalez);
	if (!easterEggFigure)
		glutWireOctahedron();
	else
		glutWireTeapot(1);
	glPopMatrix();
}

void autoIncreaseScale() {
	static GLint phase = 1;
	static GLint period = 100;
	static GLfloat scaleMin = 0.5f;
	static GLfloat scaleMax = 2.5f;
	GLfloat sc;

	if (phase < period >> 1) {
		sc = scaleMin + (scaleMax - scaleMin) / (period << 1) * (phase++ << 1);
		scalex = sc;
		scaley = sc;
		scalez = sc;
	}
	else {
		sc = scaleMax - (scaleMax - scaleMin) / (period << 1) * (phase++ << 1);
		scalex = sc;
		scaley = sc;
		scalez = sc;
	}
	if (phase == period)
		phase = 1;
}

void idle() {
	if (enableAutoRotate)
		increaseAngle(dAngleStep);
	if (enableAutoScale)
		autoIncreaseScale();
	glutPostRedisplay();
}

void display() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glColor3f(1.f, 0.f, 0.f);

	countEyes();
	glLoadIdentity();
	gluLookAt(eyex, eyey, eyez, 0.f, 0.f, 0.f, 0.f, 1.f, 0.f);
	if (showFig0)
		fig(0.f, 0.f, 0.f);
	if (showFig1)
		fig(3.f, 0.f, 0.f);

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
	glutIdleFunc(idle);
	glutKeyboardFunc(keyFunction);
	glutMouseFunc(mouseFunction);
	init();

	glutMainLoop();
}