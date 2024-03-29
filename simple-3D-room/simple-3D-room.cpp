#include <iostream>
#include "gl/glut.h"
#ifdef _DEBUG
#pragma comment(lib, "freeglutd.lib")
#else
#pragma comment(lib, "freeglut.lib")
#endif

// Light values and coordinates
GLfloat ambient[] = {0.8, 0.2, 0.2, 1};
GLfloat diffuse[] = {1.0f, 1.0f, 1.0f, 1.0f};
GLfloat specular[] = {1.0f, 1.0f, 1.0f, 1.0f};
GLfloat light0Position[] = {3, 2, 1, 0};
GLfloat specref[] = {3.0f, 3.0f, 3.0f, 3.0f};

void buildRoom(); // from room.cpp

struct
{
	double x = 0; // location
	double y = 1.8;
	double z = -10;
	int rotate = 0; // 0-359
	const int rotateSpeed = 10;
	const double step = 0.5;
	const double jump = 0.6;
	const struct
	{
		const double verticalRange = 120;
		const double nearDistance = 0.5;
		const double farDistance = 100;
	} eye;
} player;

double radians(double degree)
{
	return degree * 3.14 / 180;
}

// display callback function
void render()
{
	// clear window with glClearColor
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	// set camera, must be called before drawing object
	gluLookAt(
			player.x,
			player.y,
			player.z,
			player.x + sin(radians(player.rotate)),
			player.y,
			player.z + cos(radians(player.rotate)),
			0, 1, 0);
	glLightfv(GL_LIGHT0, GL_POSITION, light0Position);

	buildRoom();


	// finish drawing
	// glFlush(); // for network
	// glFinish(); // for network
	glutSwapBuffers(); // for double buffering
}

// setup rendering context
void setupRC()
{
	glClearColor(1, 1, 1, 1); // white background

	// Enable Depth Testing, front objects will block hinder objects
	glEnable(GL_DEPTH_TEST);

	// Enable lighting
	glEnable(GL_LIGHTING);
	// setup global env light
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambient);
	// set view point to local view point
	glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);
	// setup double side light
	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
	// Setup and enable light 0
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, specular);
	glEnable(GL_LIGHT0);
	// setup shade model
	glShadeModel(GL_SMOOTH);

	// Set Material properties to follow glColor values
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

	// Enable meterial
	glEnable(GL_COLOR_MATERIAL);
	// All materials hereafter have full specular reflectivity
	// with a high shine
	glMaterialfv(GL_FRONT, GL_SPECULAR, specref);
	glMateriali(GL_FRONT, GL_SHININESS, 50);
}

// keyboard callback function(for ascii chars)
// x and y are the coordinates of mouse when key is pressed
void keyPressedEvent(unsigned char key, int x, int y)
{
	// key to lower case
	if (key >= 'A' && key <= 'Z')
		key = key - 'A' + 'a';

	switch (key)
	{
	case 'w':
		player.z += cos(radians(player.rotate)) * player.step;
		player.x += sin(radians(player.rotate)) * player.step;
		break;
	case 'q':
		player.rotate += player.rotateSpeed;
		player.rotate %= 360;
		break;
	case 's':
		player.z -= cos(radians(player.rotate)) * player.step;
		player.x -= sin(radians(player.rotate)) * player.step;
		break;
	case 'e':
		player.rotate -= player.rotateSpeed;
		player.rotate %= 360;
		break;
	case ' ':
		player.y += player.jump;
		break;
	case 'c':
		player.y -= player.jump;
		break;
	case 'a':
	player.z += cos(radians(player.rotate + 90)) * player.step;
	player.x += sin(radians(player.rotate + 90)) * player.step;
		break;
	case 'd':
	player.z -= cos(radians(player.rotate + 90)) * player.step;
	player.x -= sin(radians(player.rotate + 90)) * player.step;
		break;
	default:
		break;
	}

	// refresh the window
	glutPostRedisplay();
}

void reshape(int w, int h)
{
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);

	// set projection matrix
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(player.eye.verticalRange, (double)w / h, player.eye.nearDistance, player.eye.farDistance);

	// set matrix mode back to model-view matrix
	glMatrixMode(GL_MODELVIEW);
}

int main(int argc, char **argv)
{
	glutInit(&argc, argv);

	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH); // use double buffer, rgb color mode, depth buffer
	glutInitWindowSize(1280, 720);
	glutCreateWindow("simple-3D-room by DiscreteTom"); // window title

	glutDisplayFunc(render);					 // set display callback function
	glutReshapeFunc(reshape);					 // set reshape callback function
	glutKeyboardFunc(keyPressedEvent); // set keyboard callback function(for ascii chars)

	setupRC();
	glutMainLoop(); // start event loop
}