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

GLfloat R=640.0/640; //Форматное соотношение
GLfloat w=640.;       //Ширина мирового окна
GLfloat h=640.;       //Высота мирового окна
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

void updateParams() {
	static GLfloat stepScaler = 3.f;
	static GLfloat scaleScaler = 0.05f;
	dAngle += 1.f; if (dAngle >= 360.f) dAngle = 0.f;
	dStep += stepScaler; if (dStep >= 140.f || dStep <= 60) stepScaler *= -1.f;
	dScale += scaleScaler; if (dScale <= 0.5f || dScale >= 2.0f) scaleScaler *= -1.f;
}

void centralSquare(void) {
//	showAxisSmall();	// switch
	glColor3f(0.0f, 1.0f, 0.0f);
	glBegin(GL_LINE_LOOP);
	glVertex2f(100.0f, 100.0f);
	glVertex2f(-100.0f, 100.0f);
	glVertex2f(-100.0f, -100.0f);
	glVertex2f(100.0f, -100.0f);
	glEnd();
}

void triangle(GLfloat red, GLfloat green, GLfloat blue) {
//	showAxisSmall();	// switch
	glColor3f(red, green, blue);
	glBegin(GL_LINE_LOOP);
	glVertex2f(0.f, 50.f);
	glVertex2f(0.f, -50.f);
	glVertex2f(50.f, 0.f);
	glEnd();
}

void star(GLfloat red, GLfloat green, GLfloat blue) {
	glBegin(GL_LINE_LOOP);
	glColor3f(red, green, blue);
	glVertex2d(0, 25);
	glVertex2d(-5, 5);
	glVertex2d(-25, 0);
	glVertex2d(-5, -5);
	glVertex2d(0, -25);
	glVertex2d(5, -5);
	glVertex2d(25, 0);
	glVertex2d(5, 5);
	glEnd();
}

void dot(GLfloat red, GLfloat green, GLfloat blue) {
	glColor3f(red, green, blue);
	glBegin(GL_LINE_LOOP);
	glVertex2d(3, 3);
	glVertex2d(-3, 3);
	glVertex2d(-3, -3);
	glVertex2d(3, -3);
	glEnd();
}

void drawCentralSquare(GLfloat red, GLfloat green, GLfloat blue) {
	glColor3f(red, green, blue);
	glPushMatrix();
	glRotatef(-dAngle, 0.f, 0.f, 1.f);
	centralSquare();
	glPopMatrix();
}

void drawTriangles(GLfloat red, GLfloat green, GLfloat blue) {
	static GLint count = 4;
	static GLfloat angle = 360.f / 4.f;

	for (int i = 0; i < 4; ++i) {
		glPushMatrix();
		glRotatef(angle * i, 0.f, 0.f, 1.f);
		glTranslatef(100.f, 0.f, 0.f);
		glRotatef(dAngle, 0.f, 0.f, 1.f);
		triangle(red, green, blue);
		glPopMatrix();
	}
}

void drawStars(GLfloat red, GLfloat green, GLfloat blue) {
	static GLint count = 8;
	static GLfloat angle = 360.f / 8.f;

	for (int i = 0; i < 8; ++i) {
		glPushMatrix();
		glRotatef(angle * i - dAngle + angle / 2, 0.f, 0.f, 1.f);
		glTranslatef(250.f, 0.f, 0.f);
		glRotatef(-dAngle * 2, 0.f, 0.f, 1.f);
		star(red, green, blue);
		glPopMatrix();
	}
}

void drawDots(GLfloat red, GLfloat green, GLfloat blue) {
	static GLint count = 4;
	static GLfloat angle = 360.f / 4.f;
	static GLfloat curStep;
	static GLint phase = 1;
	static GLint period = 100;
	static GLfloat stepMin = 150.f;
	static GLfloat stepMax = 350.f;

	if (phase < period >> 1) {
		curStep = stepMin + (stepMax - stepMin) / (period << 1) * (phase++ << 1);
	}
	else {
		curStep = stepMax - (stepMax - stepMin) / (period << 1) * (phase++ << 1);
	}
	if (phase == period)
		phase = 1;

	for (int i = 0; i < 8; ++i) {
		glPushMatrix();
		glRotatef(angle * i + angle / 2.f - dAngle, 0.f, 0.f, 1.f);
		glTranslatef(curStep, 0.f, 0.f);
		dot(red, green, blue);
		glPopMatrix();
	}
}

void squaresAndTriangles() {
	glClear(GL_COLOR_BUFFER_BIT);

//	showAxis();
	drawCentralSquare(0.f, 0.9f, 0.f);
	drawTriangles(9.f, 0.5f, 0.f);
	drawStars(9.f, 0.1f, 9.f);			// switch
	drawDots(0.1f, 0.5f, 0.5f);			// switch

	glFlush();
	glutSwapBuffers();

	updateParams();
}

int main(int argc, char **argv) {
	void (*task) ();
//	task = spiral;				// switch
	task = squaresAndTriangles;	// switch

	glutInit(&argc, argv);
//	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(640, 640);
	glutInitWindowPosition(20, 20);
	glutCreateWindow("Myprog");
	glutReshapeFunc(reshape);
	glutDisplayFunc(task);
	glutIdleFunc(task);
	init();
	glutMainLoop();
}