
#include <iostream>

#include <GL\freeglut.h>

#include "glm\glm.hpp"
#include "glm\gtc\matrix_transform.hpp"

const static int width = 1336;
const static int height = 768;

void changeSize(int w, int h)
{
	// Prevent a divide by zero, when window is too short
	// (you cant make a window of zero width).
	if (h == 0)
		h = 1;

	// Reset the Projection Matrix
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	// Set the viewport to be the entire window
	glViewport(0, 0, w, h);

	// Set perspective based on ratio
	float ratio = w * 1.0f / h;
	gluPerspective(45, ratio, 1, 100);

	// Get Back to the Modelview
	glMatrixMode(GL_MODELVIEW);
}

void renderScene(void)
{
	// Clear buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glBegin(GL_TRIANGLES);
	glVertex3f(-2, -2, -5.0);
	glVertex3f(2, 0.0, -5.0);
	glVertex3f(0.0, 2, -5.0);
	glEnd();

	// Swap buffers
	glutSwapBuffers();
}

void processNormalKeys(unsigned char key, int x, int y)
{
	// If ESCAPE was pressed
	if (key == 27)
		exit(0);
}

int main(int argc, char **argv)
{
	// init GLUT and create Window
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(0, 0);
	glutInitWindowSize(width, height);
	glutCreateWindow("Aurora Borealis");

	// register callbacks
	glutDisplayFunc(renderScene); // What to display
	glutReshapeFunc(changeSize); // What to do on reshape
	glutKeyboardFunc(processNormalKeys); // How to handle normal keys events.

	// enter GLUT event processing cycle
	glutMainLoop();

	return 1;
}