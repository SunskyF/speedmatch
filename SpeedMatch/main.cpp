#define FREEGLUT_STATIC
#include <GL\freeglut.h>
#pragma comment(lib, "freeglut_staticd.lib")

#include <iostream>
#include <cstring>
#include <vector>
#include <glm\vec2.hpp>

#include "Objcar.h"
#include "Skybox.h"

#define WINDOW_WIDTH  1200
#define WINDOW_HEIGHT 1000

using namespace std;

//GLMmodel *g_model = NULL;
Objcar car0;
Skybox sky;
char* filename = "fll.obj";
#define PI 3.14159265358979323846

int mousex = 0, mousey = 0;

GLdouble eyeX, eyeY, eyeZ;
GLdouble centerX, centerY, centerZ;
GLint length = 1200;
GLdouble dir[3] = { 0, 1, 0 };// the direction of the camera
GLfloat theta = 0, phi = PI/2;
bool LEFT_BUTTON_DOWN = false, RIGHT_BUTTON_DOWN = false;

void drawmaze(){
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(1.0, 1.0, 1.0, .1);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60.0f, 1.0, 0.1, 20000.0);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	centerX = 0;
	centerY = 0;
	centerZ = 0;
	eyeX = centerX + length * sin(phi) * cos(theta);
	eyeY = centerZ + length * cos(phi);
	eyeZ = centerY + length * sin(phi) * sin(theta);
	gluLookAt(eyeX, eyeY, eyeZ, centerX, centerY, centerZ, 0, 1, 0);


	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glEnable(GL_LIGHTING);
	glEnable(GL_COLOR_MATERIAL);

	// light
	GLfloat ambientLight[] =
	{ 0.1f, 0.1f, 0.1f, 0.1f };
	GLfloat diffuseLight[] =
	{ 0.8f, 0.8f, 0.8f, 0.0f };
	GLfloat specular[] =
	{ 1.0f, 1.0f, 1.0f, 1.0f };
	GLfloat lightPos0[] =
	{ 0.0f, 0.0f, 10000.0f, 1.0f };

	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambientLight);

	glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight); glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight);
	glLightfv(GL_LIGHT0, GL_SPECULAR, specular); glLightfv(GL_LIGHT0, GL_POSITION, lightPos0);
	glEnable(GL_LIGHT0);

	glEnable(GL_LINE_SMOOTH);
	glEnable(GL_BLEND);

//	glmDraw(g_model, GLM_SMOOTH|GLM_MATERIAL);
	car0.draw();

	sky.render();
	glutSwapBuffers();
}

void mouseevent(int button, int state, int x, int y) {
	if (button == GLUT_LEFT_BUTTON){
		if (state == GLUT_DOWN) LEFT_BUTTON_DOWN = true;
		else LEFT_BUTTON_DOWN = false;
	}
	else if (button == GLUT_RIGHT_BUTTON){
		if (state == GLUT_DOWN) RIGHT_BUTTON_DOWN = true;
		else RIGHT_BUTTON_DOWN = false;
	}
	mousex = x, mousey = y;
}
void motionevent(int x, int y){
	if (LEFT_BUTTON_DOWN == TRUE){
		theta -= (x - mousex) / 80.0f;
		phi += (y - mousey) / 120.0f;
	}
	if (RIGHT_BUTTON_DOWN == TRUE){
		length += (y - mousey) * 10;
	}

	if (length < 500)   length = 500;
	//compute the direction
	dir[0] = sin(phi) * cos(theta);
	dir[1] = sin(phi) * sin(theta);
	dir[2] = cos(phi);
	mousex = x, mousey = y;
	glutPostRedisplay();
}
void directevent(int key, int x, int y){
	GLdouble movdir[3];
	switch (key){
		//compute the move direction
	case GLUT_KEY_UP://move forward
		movdir[0] = dir[0];
		movdir[1] = dir[1];
		movdir[2] = dir[2];
		glutPostRedisplay();
		break;
	case GLUT_KEY_LEFT://move left
		movdir[0] = -dir[1];
		movdir[1] = dir[0];
		movdir[2] = dir[2];
		glutPostRedisplay();
		break;
	case GLUT_KEY_DOWN://move backward
		movdir[0] = -dir[0];
		movdir[1] = -dir[1];
		movdir[2] = dir[2];
		glutPostRedisplay();
		break;
	case GLUT_KEY_RIGHT: //move right
		movdir[0] = dir[1];
		movdir[1] = -dir[0];
		movdir[2] = dir[2];
		glutPostRedisplay();
		break;
	}
}

void init(){
//	g_model = glmReadOBJ(filename);
//	glmScale(g_model,100);
	car0.ReadOBJ(filename);
	if (!sky.init())
	{
		cout << "³õÊ¼»¯Ìì¿ÕÊ§°Ü!" << endl;
	}
}
int main(int argc, char** argv){
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowPosition(50, 50);
	glutInitWindowSize(1000, 1000);
	glutCreateWindow("Speed Match");
	init();
	glutDisplayFunc(drawmaze);
	glutMotionFunc(motionevent);
	glutMouseFunc(mouseevent);
	glutSpecialFunc(directevent);
	glutMainLoop();
}