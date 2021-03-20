#define GL_SILENCE_DEPRECATION
#include <GLUT/glut.h>
#include <unistd.h>
#include <stdlib.h>
#include <math.h>
#include "utils.h"

static const GLint windowWidth =  640;
static const GLint windowHeight = 640;
static const GLfloat windowRatio = (float)windowWidth / windowHeight;
static const GLint leftCorner = 	-windowWidth / 2;
static const GLint rightCorner =	 windowWidth / 2;
static const GLint topCorner = 		 windowHeight / 2;
static const GLint bottomCorner = 	-windowHeight / 2;

void init(void) {
	glClearColor(0.33f,0.33f,0.33f,0.0);
	glClear(GL_COLOR_BUFFER_BIT);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(leftCorner, rightCorner, bottomCorner, topCorner);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void reshape(GLsizei W, GLsizei H) {
	if(windowRatio>W/H) glViewport(0,0,W,W/windowRatio);
	else glViewport(0,0,H*windowRatio,H);
}

void triangle(GLfloat r, GLfloat g, GLfloat b) {
	glColor3f(r, g, b);
	glBegin(GL_TRIANGLES);
	glVertex2f(0.f, 0.f);
	glVertex2f(1.f, 0.f);
	glVertex2f(0.5f, 0.86602540378f);
	glEnd();
}

void line() {
	glColor3b(0, 0, 0);
	glBegin(GL_LINES);
	glVertex2d(0,0);
	glVertex2d(1,0);
	glEnd();
}

void drawTunnel(GLint count, GLfloat scale, GLfloat * colors, GLint max) {
	if (count == 0)
		return;

	glScalef(scale, scale, 0.f);
	triangle(colors[(max - count) * 3], colors[(max - count) * 3 + 1], colors[(max - count) * 3 + 2]);
	glRotatef(60.f, 0.f, 0.f, 1.f);
	glTranslatef(1.f, 0.f, 0.f);
	glRotatef(180.f, 0.f, 0.f, 1.f);
	glScalef(1.f / scale, 1.f / scale, 0.f);
	drawTunnel(count - 1, scale * 0.9f, colors, max);
}

void drawTriangles(GLint count, GLfloat scale, GLfloat * colors, GLint max) {
	if (count == 0)
		return;
	GLfloat r = colors[(max - count) * 3];
	GLfloat g = colors[(max - count) * 3 + 1];
	GLfloat b = colors[(max - count) * 3 + 2];
// left triangle
	glPushMatrix();
	glScalef(scale / 2.f, scale / 2.f, 1.f);
	triangle(r, g, b);
	glPopMatrix();
	glPushMatrix();
	drawTriangles(count - 1, scale / 2.f, colors, max);
	glPopMatrix();
// top triangle
	glPushMatrix();
	glScalef(scale / 2.f, scale / 2.f, 1.f);
	glRotatef(60.f, 0.f, 0.f, 1.f);
	glTranslatef(1.f, 0.f, 0.f);
	glRotatef(-60.f, 0.f, 0.f, 1.f);
	triangle(r, g, b);
	glScalef(2.f / scale, 2.f / scale, 1.f);
	drawTriangles(count - 1, scale / 2.f, colors, max);
	glPopMatrix();
//	 right triangle
	glPushMatrix();
	glScalef(scale / 2.f, scale / 2.f, 0.f);
	glTranslatef(1.f, 0.f, 0.f);
	triangle(r, g, b);
	glScalef(2.f / scale, 2.f / scale, 0.f);
	drawTriangles(count - 1, scale / 2.f, colors, max);
	glPopMatrix();
//	 mid triangle
//	glPushMatrix();
//	glScalef(scale / 2.f, scale / 2.f, 0.f);
//	glTranslatef(1.f, 0.f, 0.f);
//	glRotatef(60.f, 0.f, 0.f, 1.f);
//	triangle(r, g, b);
//	glScalef(2.f / scale, 2.f / scale, 0.f);
//	drawTriangles(count - 1, scale / 2.f, colors, max);
//	glPopMatrix();
}

GLfloat * reinitColors(GLfloat * colors, GLint count) {
	if (colors)
		free(colors);
	colors = malloc(sizeof(GLfloat) * count * 3);
	if (!colors)
		exit_fatal("malloc fails");

	for (int i = 0; i < count * 3; ++i) {
		colors[i] = randFloat();
	}
	return colors;
}

static GLfloat angle;
static GLfloat dAngle = .5f;

void display() {
	static GLint trianglesToDisplay;
	static GLint maxTrianglesToDisplay = 7;
	static GLfloat * colors;
	if (!colors)
		colors = reinitColors(colors, maxTrianglesToDisplay);

	glClear(GL_COLOR_BUFFER_BIT);
	for (int i = 0; i < 3; ++i) {
		glPushMatrix();
		glRotatef(angle + i * 120.f, 0.f, 0.f, 1.f);
		drawTriangles(trianglesToDisplay + 1, windowHeight * 0.4f, colors, trianglesToDisplay + 1);
		glPopMatrix();
	}

	angle += dAngle;
	if (angle >= 360.f) {
		angle = 0.f;
	}
	if ( (int)(angle * 2) % 36 == 0)
		++trianglesToDisplay;

	if (trianglesToDisplay == maxTrianglesToDisplay) {
		trianglesToDisplay = 1;
		colors = reinitColors(colors, maxTrianglesToDisplay);
	}

	glFlush();
	glutSwapBuffers();
}

int main(int argc, char **argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(windowWidth, windowHeight);
	glutInitWindowPosition(20, 20);
	glutCreateWindow("MY SUPER PROGRAM!");
	glutReshapeFunc(reshape);
	glutDisplayFunc(display);
	glutIdleFunc(display);
	init();
	glutMainLoop();
}
