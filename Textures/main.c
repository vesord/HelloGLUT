/**
	\author Vesord
 	\date march 2021
	\brief
	simple 3D visualisation of object with textures
	also manual reading bitmap of BMP image

 	key f - show cull face
 	key b - show cull back
 	key e - ???
 	key E - ???
 */

#define GL_SILENCE_DEPRECATION
#include <GLUT/glut.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

static const GLint windowWidth =  640;
static const GLint windowHeight = 640;
static const GLfloat windowRatio = (float)windowWidth / windowHeight;

static GLfloat figurAngle = 0.f;
static const GLfloat figurDAngle = 1.f;

static GLint figuresCount = 1;
static GLuint texture;

void init(void) {
	glClearColor(0.6f, 0.6f, 0.9f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_NORMALIZE);
	glEnable(GL_CULL_FACE);
	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);

	glViewport(0, 0, windowWidth, windowHeight);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60.f, windowRatio, 2., 200);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

typedef struct	s_bmp_file_header
{
	char	type_1;
	char	type_2;
	int		size;
	int		reserved;
	int		offset_to_color_bits;
}				t_bmp_file_header;

typedef struct	s_bmp_info
{
	int		size;
	int		width;
	int		height;
	short	planes;
	short	btp;
	int		compression;
	int		img_size;
	int		ppm_x;
	int		ppm_y;
	int		color_table;
	int		color_table_size;
}				t_bmp_info;

unsigned char *myBMPLoader(const char *filename, int *width, int *height) {
	int fd;
	t_bmp_file_header	bfh;
	t_bmp_info			bmi;
	unsigned char *		data = NULL;
	int ret;

	fd = open(filename, O_RDONLY);
	if (fd < 0) {
		printf("can't open file %s\n", filename);
		exit(1);
	}

	read(fd, &(bfh.type_1), 1);
	read(fd, &(bfh.type_2), 1);
	read(fd, &(bfh.size), 4);
	read(fd, &(bfh.reserved), 4);
	read(fd, &(bfh.offset_to_color_bits), 4);
	read(fd, &(bmi.size), 4);
	read(fd, &(bmi.width), 4);
	read(fd, &(bmi.height), 4);
	read(fd, &(bmi.planes), 2);
	read(fd, &(bmi.btp), 2);
	read(fd, &(bmi.compression), 4);
	read(fd, &(bmi.img_size), 4);
	read(fd, &(bmi.ppm_x), 4);
	read(fd, &(bmi.ppm_y), 4);
	read(fd, &(bmi.color_table), 4);
	read(fd, &(bmi.color_table_size), 4);

	data = malloc(bmi.img_size);
	if (!data)
		exit(1);

	read(fd, data, bmi.img_size);

	close(fd);
	*width = bmi.width;
	*height = bmi.height;
	return data;
}

void fixImage(unsigned char * image, int width, int height, int channels) {
	int size = width * height * channels;
	char tmp;
	for (int i = 0; i < size; i += channels) {
		tmp = image[i];
		image[i] = image[i + 2];
		image[i + 2] = tmp;
	}
}

void initTexture() {
	glEnable(GL_TEXTURE_2D);

	glGenTextures (1, &texture);
	glBindTexture (GL_TEXTURE_2D, texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	int width, height;
	unsigned char* image = myBMPLoader("Star.bmp", &width, &height);
	if (!image)
		exit(1);
	fixImage(image, width, height, 4);

	gluBuild2DMipmaps(GL_TEXTURE_2D, 4, width, height, GL_RGBA, GL_UNSIGNED_BYTE, image);
	free(image);
}

void reshape(GLsizei W, GLsizei H) {
	if(windowRatio>W/H) glViewport(0,0,W,W/windowRatio);
	else glViewport(0,0,H*windowRatio,H);
}

void key(unsigned char key, int x, int y) {
	switch (key) {
		case 'f': glCullFace(GL_FRONT); break;
		case 'b': glCullFace(GL_BACK); break;
		case 'e': figuresCount = figuresCount % 17 + 1; break;
		case 'E': figuresCount = (figuresCount + 15) % 17 + 1; break;
		default: break;
	}
	glutPostRedisplay();
}

void figure() {
	glColor4f(1.f, 1.f, 1.f, 0.f);
	glEnable(GL_TEXTURE_2D);

	// back edge
	glBegin(GL_QUADS);
	glNormal3f(0.f, 0.f, -1.f);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(0.f, 0.f, 0.f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(1.f, 1.f, 0.f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(2.f, 1.f, 0.f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(2.f, 0.f, 0.f);
	glEnd();
	// right edge
	glBegin(GL_QUADS);
	glNormal3f(1.f, 0.f, 0.f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(2.f, 0.f, 0.f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(2.f, 1.f, 0.f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(2.f, 1.f, 1.f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(2.f, 0.f, 1.f);
	glEnd();
	// top edge
	glBegin(GL_QUADS);
	glNormal3f(0.f, 1.f, 0.f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(2.f, 1.f, 0.f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(1.f, 1.f, 0.f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(1.f, 1.f, 1.f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(2.f, 1.f, 1.f);
	glEnd();
	// left edge
	glBegin(GL_QUADS);
	glNormal3f(-1.f, 1.f, 0.f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(1.f, 1.f, 0.f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(0.f, 0.f, 0.f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(0.f, 0.f, 1.f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(1.f, 1.f, 1.f);
	glEnd();
	// front edge
	glBegin(GL_QUADS);
	glNormal3f(0.f, 0.f, 1.f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(1.f, 1.f, 1.f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(0.f, 0.f, 1.f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(2.f, 0.f, 1.f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(2.f, 1.f, 1.f);
	glEnd();
	// bottom edge
	glBegin(GL_QUADS);
	glNormal3f(0.f, -1.f, 0.f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(0.f, 0.f, 0.f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(2.f, 0.f, 0.f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(2.f, 0.f, 1.f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(0.f, 0.f, 1.f);
	glEnd();
}

void drawFigure() {
	glPushMatrix();
	glRotatef(figurAngle, 0.f, 1.f, 0.f);
	figure();
	glPopMatrix();
}

void drawFigures() {
	for (int i = 0; i < figuresCount; ++i) {
		glRotatef(360.f / figuresCount + figurAngle, 0.f, 1.f, 0.f);
		glTranslatef(2.2f, 0.f, 0.f);
		figure();
	}
}

void display() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	if (figuresCount == 1) {
		gluLookAt(0.0, 4.1, 4.1, 0., 0., 0., 0., 1., 0.);
		drawFigure();
	}
	else {
		gluLookAt(0.0, 14.1, 14.1, 0., 0., 0., 0., 1., 0.);
		drawFigures();
	}

	glFlush();
	glutSwapBuffers();
}

void normalizeAngle(GLfloat *angle) {
	if (*angle >= 360.f)
		*angle = 0.f;
}

void updateAngles() {
	figurAngle += figurDAngle; normalizeAngle(&figurAngle);
}

void idle() {
	updateAngles();
	glutPostRedisplay();
}

int main(int argc, char **argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(windowWidth, windowHeight);
	glutInitWindowPosition(20, 20);
	glutCreateWindow("MY SUPER PROGRAM!");
	glutReshapeFunc(reshape);
	glutDisplayFunc(display);
	glutIdleFunc(idle);
	glutKeyboardFunc(key);
	init();
	initTexture();
	glutMainLoop();
}
