#pragma once

#include <cstdio>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <cstdlib>
#include <cstring>

#include <glew\glew.h>
#include <GLFW\glfw3.h>
#include <glm\glm.hpp>
#include <glm\gtx\transform.hpp>
#include <glm\gtc\matrix_transform.hpp>

struct vertex
{
	glm::vec3 coords;
	glm::vec3 colors;
	glm::vec3 normals;

	glm::mat4 ModelMatrix;

	float angle;
};

class Context
{
private:
	int m_width;
	int m_height;
	int m_noObjs;

	std::string m_windowName;

	GLFWwindow* m_window;

	GLuint m_vaoID, m_vboID, m_programID, m_iboID;

	// position
	glm::vec3 m_position;
	float m_horizontalAngle;
	float m_verticalAngle;
	float m_initialFoV;

	float m_speed;
	float m_mouseSpeed;

	// matrices
	glm::mat4 m_ViewMatrix, m_ProjectionMatrix, m_mvp;

	// data
	std::vector<vertex> m_points;
	std::vector<unsigned int> m_indices;

	void computeMatricesFromInputs(void);

public:
	int Init(void);
	void mainLoop(void);
	GLuint LoadShaders(const char * vertex_file_path, const char * fragment_file_path);

	// Getters
	GLFWwindow* getWindow();

	// Constructors
	Context();
	~Context();
};
