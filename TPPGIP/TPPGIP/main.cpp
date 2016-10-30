
#include <iostream>
#include <vector>

#include <glew\glew.h>

#include <GL\freeglut.h>

#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>

#define M_PI 3.1415

// window size
const static int width = 1336;
const static int height = 768;
// angle of rotation for the camera direction
float angle = 0.0;
// actual vector representing the camera's direction
float lx = 0.0f, lz = -1.0f;
// XZ position of the camera
float x = 0.0f, z = 5.0f;

std::vector<glm::vec4> pointsCoord;

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
	gluPerspective(45, ratio, 1, 200);

	// Get Back to the Modelview
	glMatrixMode(GL_MODELVIEW);
}

void renderScene(void)
{
	static const int halfSize = 250;
	static const float ratio = static_cast<float>(1.0f * M_PI / halfSize);

	// Clear Color and Depth Buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.09f, 0.09f, 0.43f, 1.f);
	glLineWidth(1);

	// Reset transformations
	glLoadIdentity();
	// Set the camera
	gluLookAt(x, 1.0f, z,
			x + lx, 1.0f, z + lz,
			0.0f, 1.0f, 0.0f);

	// Draw ground
	glColor3f(0.36f, 0.29f, 0.19f);
	glBegin(GL_QUADS);
	glVertex3f(-100.0f, -10.0f, -100.0f);
	glVertex3f(-100.0f, -10.0f, 100.0f);
	glVertex3f(100.0f, -10.0f, 100.0f);
	glVertex3f(100.0f, -10.0f, -100.0f);
	glEnd();

	glColor3f(1.f, 1.f, 1.f);
	glLineWidth(1.f);
	for (int i = 0; i < pointsCoord.size(); i+=2) {
		float ang = static_cast<float>(sin(pointsCoord[i].w) * 0.005);
		pointsCoord[i].y += ang;
		pointsCoord[i + 1].y += ang;
		pointsCoord[i].w += 0.01f;
		pointsCoord[i + 1].w += 0.01f;

		glBegin(GL_LINES);
			glVertex3f(pointsCoord[i].x, pointsCoord[i].y, pointsCoord[i].z);
			glVertex3f(pointsCoord[i + 1].x, pointsCoord[i + 1].y, pointsCoord[i + 1].z);
		glEnd();
	}

	
	glutSwapBuffers();
}

void processNormalKeys(unsigned char key, int x, int y)
{
	// If ESCAPE was pressed
	if (key == 27)
		exit(0);
}

void processSpecialKeys(int key, int xx, int yy)
{
	float fraction = 0.1f;

	switch (key) {
	case GLUT_KEY_LEFT:
		angle -= 0.01f;
		lx = sin(angle);
		lz = -cos(angle);
		break;
	case GLUT_KEY_RIGHT:
		angle += 0.01f;
		lx = sin(angle);
		lz = -cos(angle);
		break;
	case GLUT_KEY_UP:
		x += lx * fraction;
		z += lz * fraction;
		break;
	case GLUT_KEY_DOWN:
		x -= lx * fraction;
		z -= lz * fraction;
		break;
	}
}

int main(int argc, char **argv)
{
	// init GLUT and create Window
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(0, 0);
	glutInitWindowSize(width, height);
	glutCreateWindow("Aurora Borealis");

	// Initialize GLEW
	glewExperimental = true; // Needed for core profile
	if (glewInit() != GLEW_OK) {
		std::cerr << "Failed to initialize GLEW" << std::endl;
		exit(0);
	}

	// Enable depth test
	glEnable(GL_DEPTH_TEST);
	// Accept fragment if it closer to the camera than the former one
	glDepthFunc(GL_LESS);

	int vecSize = 1000; // points * lines
	pointsCoord.resize(vecSize);

	int j = -vecSize / 4;
	float ratio = static_cast<float>(M_PI / 250.0);

	for (int i = 0; i < vecSize; i += 2)
	{
		float ang = sin(ratio * i);
		pointsCoord[i] = glm::vec4(.5f * j, 10.f + ang, -100.f, ang);
		pointsCoord[i + 1] = glm::vec4(.5f * j, 40.f + ang, -100.f, ang);

		j++;
	}


	// register callbacks
	glutDisplayFunc(renderScene); // What to display
	glutReshapeFunc(changeSize); // What to do on reshape
	glutIdleFunc(renderScene); // What to do on idle
	glutKeyboardFunc(processNormalKeys); // How to handle normal keys events
	glutSpecialFunc(processSpecialKeys); // How to handle special keys

	// enter GLUT event processing cycle
	glutMainLoop();

	return 1;
}