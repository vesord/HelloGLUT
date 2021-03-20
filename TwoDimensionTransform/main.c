/**
	\author Vesord
	\date march 2021
	\brief simple 2D visualisation of rectangles, some case of using matrices

	no keys are allowed, switch between scene rotation and point moving on spiral
	is made by hardcode in main() lol
 */

#define GL_SILENCE_DEPRECATION
#include <GLUT/glut.h>
#include <unistd.h>

GLfloat R=640.0/480; //Форматное соотношение
GLfloat w=640;       //Ширина мирового окна
GLfloat h=480;       //Высота мирового окна
GLfloat l, r, b, t;  //Параметры мирового окна
GLfloat f=0.0f;
GLfloat dStep = 60;
GLfloat dAngle = 16.0f;
GLfloat dScale = 1.0f;

void init(void) { //Расчет параметров мирового окна
	h=w/R; l=-w/2; r=w/2; b=-h/2; t=h/2;
	glClearColor(1.0,1.0,0.0,0.0);
	glClear(GL_COLOR_BUFFER_BIT);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(l, r, b, t);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void reshape(GLsizei W, GLsizei H) {
	if(R>W/H) glViewport(0,0,W,W/R);
	else glViewport(0,0,H*R,H);
}

void showAxis(void) {
	glColor3f(0.0f, 0.0f, 1.0f);
	glBegin(GL_LINES);
	glVertex2f(0, 0);
	glVertex2f(0, t);
	glVertex2f(0, 0);
	glVertex2f(r, 0);
	glEnd();
}

void myPoint(void) {
	glColor3f(1.0f, 0.0f, 0.0f);
	glBegin(GL_POINTS);
	glVertex2f(0.0f, 0.0f);
	glEnd();
}

void spiral(void) {
	glClear(GL_COLOR_BUFFER_BIT);
	showAxis();

	glPushMatrix();
	glRotatef(dAngle, 0.0, 0.0, 1.0);
	glTranslatef(dStep, 0.0f, 0.0f);
	myPoint();
	glPopMatrix();

	glFlush();
	glutSwapBuffers();

	dAngle -= 4.f; if (dAngle <= 0.f) dAngle = 360.0f;
	dStep -= 0.05f; if (dStep <= 0) dStep = 60.0f;
	usleep(20000);
}

void showAxisSmall(void) {
	glColor3f(0.0f, 0.0f, 0.0f);
	glBegin(GL_LINES);
	glVertex2f(0.f, 0.f);
	glVertex2f(0.f, 30.f);
	glVertex2f(0.f, 0.f);
	glVertex2f(30.f, 0.f);
	glEnd();
}

void fig0(void) {
	showAxisSmall();
	glColor3f(0.0f, 1.0f, 0.0f);
	glBegin(GL_LINE_LOOP);
	glVertex2f(100.0f, 100.0f);
	glVertex2f(-100.0f, 100.0f);
	glVertex2f(-100.0f, -100.0f);
	glVertex2f(100.0f, -100.0f);
	glEnd();
}

void fig1Modify(GLfloat step) {
	glTranslatef(-step, step, 0.f);
	glRotatef(90.f, 0.f, 0.f, 1.f);
}

void fig1(GLfloat step) {
	fig1Modify(step);
	glColor3f(1.0f, 0.0f, 0.0f);
	glBegin(GL_LINE_LOOP);
	glVertex2f(0.f, 50.f);
	glVertex2f(0.f, -50.f);
	glVertex2f(50.f, 0.f);
	glEnd();
	showAxisSmall();
}

void bigModify(GLfloat angle, GLfloat step, GLfloat scale) {
//	glScalef(scale, scale, 0.f);
	glRotatef(angle, 0.f, 0.f, 1.f);
	glTranslatef(step, 0.f, 0.f);
}

void squaresAndTriangles() {
	static GLfloat stepScaler = 3.f;
	static GLfloat scaleScaler = 0.05f;

	glClear(GL_COLOR_BUFFER_BIT);
	glLoadIdentity();
	fig0();
	bigModify(dAngle, dStep, dScale);
	for (int i = 0; i < 4; ++i)
		fig1(dStep);

	glFlush();
	glutSwapBuffers();

	dAngle += 1.f; if (dAngle >= 360.f) dAngle = 0.f;
	dStep += stepScaler; if (dStep >= 140.f || dStep <= 60) stepScaler *= -1.f;
	dScale += scaleScaler; if (dScale <= 0.5f || dScale >= 2.0f) scaleScaler *= -1.f;
}

int main(int argc, char **argv) {
	void (*task) ();
//	task = spiral;				// switch
	task = squaresAndTriangles;	// switch

	glutInit(&argc, argv);
//	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(640, 480);
	glutInitWindowPosition(20, 20);
	glutCreateWindow("Myprog");
	glutReshapeFunc(reshape);
	glutDisplayFunc(task);
	glutIdleFunc(task);
	init();
	glutMainLoop();
}