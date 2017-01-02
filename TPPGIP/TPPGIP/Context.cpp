#include "Context.h"

int Context::Init(void)
{
	if (!glfwInit()) {
		std::cerr << "Failed to initialzie GLFW" << std::endl;

		return -1;
	}

	glfwWindowHint(GLFW_SAMPLES, 4); // 4x antialiasing
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // We want OpenGL 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // We don't want the old OpenGL 

	// Open a window and create its OpenGL context
	m_window = glfwCreateWindow(m_width, m_height, m_windowName.c_str(), NULL, NULL);
	if (!m_window) {
		std::cerr << "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials." << std::endl;
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(m_window); // Initialize GLEW
	glewExperimental = true; // Needed in core profile
	if (glewInit() != GLEW_OK) {
		std::cerr << "Failed to initialize GLEW" << std::endl;
		return -1;
	}

	// Ensure we can capture the escape key being pressed below
	glfwSetInputMode(m_window, GLFW_STICKY_KEYS, GL_TRUE);

	// Hide the mouse and enable unlimited mouvement
	glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPos(m_window, m_width / 2, m_height / 2);

	// Enable settings
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glEnable(GL_CULL_FACE);
	// Enable blending
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glClearColor(0.0f, 0.0f, 0.3f, 1.0f);

	// Init camera movement
	// position
	m_position = glm::vec3(0, 0, 99);
	// horizontal angle : toward -Z
	m_horizontalAngle = 3.14f;
	// vertical angle : 0, look at the horizon
	m_verticalAngle = 0.0f;
	// Initial Field of View
	m_initialFoV = 45.0f;

	m_speed = 3.0f; // 3 units / second
	m_mouseSpeed = 0.0005f;


	// Init VAO
	glGenVertexArrays(1, &m_vaoID);
	glBindVertexArray(m_vaoID);

	// Load Shaders
	m_programID = LoadShaders("vertexShader.glsl", "fragmentShader.glsl");


	// Init data
	static const float M_PI = 3.1596f;
	m_noObjs = 500; // points * lines

	int j = -m_noObjs / 2;
	float ratio = static_cast<float>(M_PI / 250.0);

	m_points.resize(m_noObjs * 2);
	m_indices.resize(m_noObjs * 2);

	for (int i = 0; i < m_noObjs * 2; i += 2) {
		float ang = sin(ratio * i);
		m_points[i].coords = glm::vec3(.5f * j, -15.f + ang, 0.f);
		m_points[i + 1].coords = glm::vec3(.5f * j, 15.f + ang, 0.f);

		m_points[i].colors = glm::vec3(1,1,1);
		m_points[i + 1].colors = glm::vec3(1,1,1);

		m_points[i].normals = glm::vec3(1.f, 0.f, 0.f);
		m_points[i + 1].normals = glm::vec3(1.f, 0.f, 0.f);

		m_points[i].angle = ang;
		m_points[i + 1].angle = ang;

		m_points[i].ModelMatrix = glm::mat4(1.0);
		m_points[i + 1].ModelMatrix = glm::mat4(1.0);

		m_indices[i] = i;
		m_indices[i + 1] = i + 1;

		j++;
	}

	glGenBuffers(1, &m_vboID);
	glBindBuffer(GL_ARRAY_BUFFER, m_vboID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertex) * m_noObjs * 2, &m_points[0].coords.x, GL_STATIC_DRAW);

	glGenBuffers(1, &m_iboID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_iboID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indices.size() * sizeof(unsigned int), &m_indices[0], GL_STATIC_DRAW);

	return 1;
}

void Context::computeMatricesFromInputs(void) {

	// glfwGetTime is called only once, the first time this function is called
	static double lastTime = glfwGetTime();

	// Compute time difference between current and last frame
	double currentTime = glfwGetTime();
	float deltaTime = float(currentTime - lastTime);

	// Get mouse position
	double xpos, ypos;
	glfwGetCursorPos(m_window, &xpos, &ypos);

	// Reset mouse position for next frame
	glfwSetCursorPos(m_window, m_width / 2, m_height / 2);

	// Compute new orientation
	m_horizontalAngle += m_mouseSpeed * float(m_width / 2 - xpos);
	m_verticalAngle += m_mouseSpeed * float(m_height / 2 - ypos);

	// Direction : Spherical coordinates to Cartesian coordinates conversion
	glm::vec3 direction(
		cos(m_verticalAngle) * sin(m_horizontalAngle),
		sin(m_verticalAngle),
		cos(m_verticalAngle) * cos(m_horizontalAngle)
	);

	// Right vector
	glm::vec3 right = glm::vec3(
		sin(m_horizontalAngle - 3.14f / 2.0f),
		0,
		cos(m_horizontalAngle - 3.14f / 2.0f)
	);

	// Up vector
	glm::vec3 up = glm::cross(right, direction);

	// Move forward
	if (glfwGetKey(m_window, GLFW_KEY_UP) == GLFW_PRESS) {
		m_position += direction * deltaTime * m_speed;
	}
	// Move backward
	if (glfwGetKey(m_window, GLFW_KEY_DOWN) == GLFW_PRESS) {
		m_position -= direction * deltaTime * m_speed;
	}
	// Strafe right
	if (glfwGetKey(m_window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
		m_position += right * deltaTime * m_speed;
	}
	// Strafe left
	if (glfwGetKey(m_window, GLFW_KEY_LEFT) == GLFW_PRESS) {
		m_position -= right * deltaTime * m_speed;
	}

	float FoV = m_initialFoV;

	// Projection matrix : 45° Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
	m_ProjectionMatrix = glm::perspective(glm::radians(FoV), static_cast<float>(m_width) / static_cast<float>(m_height), 0.1f, 200.0f);
	// Camera matrix
	m_ViewMatrix = glm::lookAt(
		m_position,           // Camera is here
		m_position + direction, // and looks here : at the same position, plus "direction"
		up                  // Head is up (set to 0,-1,0 to look upside-down)
	);

	// For the next frame, the "last time" will be "now"
	lastTime = currentTime;
}

void Context::mainLoop(void)
{
	static const int idCount = 10;
	GLuint MatrixID = glGetUniformLocation(m_programID, "MVP");
	GLuint ViewMatrixID = glGetUniformLocation(m_programID, "V");
	GLuint ModelMatrixID = glGetUniformLocation(m_programID, "M");

	std::vector<glm::vec3> lightPositions = {
		{ 10, 10, 50 },
		{ -20, 10, 50 },
		{ 20, 10, 50 },
		{ 10, 20, 50 },
		{ 10, -20, 50 },
		{ 20, 20, 50 },
		{ 20, -20, 50 },
		{ -20, 20, 50 },
		{ -20, 20, 50 },
		{ 20, 20, 20 }
	};

	std::vector<glm::vec3> rotateToPosition = {
		{ 0,0,0},
		{ 0,0,0 },
		{ -200,0,0 },
		{ 200,0,0 },
		{ -150,0,0 },
		{ 150,0,0 },
		{ -100,0,0 },
		{ 100,0,0 },
		{ -50,0,0 },
		{ 50,0,0 }
	};

	std::vector<glm::vec3> rotateVector = {
		{ 0.25f, 0, 0 },
		{ 0.25f, 0.25f, 0 },
		{ 0.25f, 0.25f, 0.25f },
		{ 0.25f, 0, 0.25f },
		{ 0, 0, 0.25f },
		{ 0, 0.25f, 0.25f },
		{ 0, 0, 0.25f },
		{ 0, 0.25f, 0.25f },
		{ 0, 0, 0.25f },
		{ 0, 0.25f, 0 }
	};

	std::vector<GLint> LightIDs = {
		glGetUniformLocation(m_programID, "LightPosition_worldspace0"),
		glGetUniformLocation(m_programID, "LightPosition_worldspace1"), 
		glGetUniformLocation(m_programID, "LightPosition_worldspace2"), 
		glGetUniformLocation(m_programID, "LightPosition_worldspace3"), 
		glGetUniformLocation(m_programID, "LightPosition_worldspace4"), 
		glGetUniformLocation(m_programID, "LightPosition_worldspace5"), 
		glGetUniformLocation(m_programID, "LightPosition_worldspace6"), 
		glGetUniformLocation(m_programID, "LightPosition_worldspace7"), 
		glGetUniformLocation(m_programID, "LightPosition_worldspace8"), 
		glGetUniformLocation(m_programID, "LightPosition_worldspace9") };
	/*GLuint LightID2 = glGetUniformLocation(m_programID, "LightPosition_worldspace2");*/

	do {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glLineWidth(2.5f);
		// Use our shader
		glUseProgram(m_programID);

		// Compute the MVP matrix from keyboard and mouse input
		computeMatricesFromInputs();

		// 1rst attribute buffer : vertices
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);

		// Bind buffers
		glBindBuffer(GL_ARRAY_BUFFER, m_vboID);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_iboID);

		glVertexAttribPointer(
			0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
			3,                  // size
			GL_FLOAT,           // type
			GL_FALSE,           // normalized?
			sizeof(vertex),     // stride
			nullptr             // array buffer offset
		);
		glVertexAttribPointer(
			1,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
			3,                  // size
			GL_FLOAT,           // type
			GL_FALSE,           // normalized?
			sizeof(vertex),     // stride
			reinterpret_cast<void *>(offsetof(vertex, colors))           // array buffer offset
		);
		glVertexAttribPointer(
			2,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
			3,                  // size
			GL_FLOAT,           // type
			GL_FALSE,           // normalized?
			sizeof(vertex),     // stride
			reinterpret_cast<void *>(offsetof(vertex, normals))            // array buffer offset
		);

		glUniformMatrix4fv(ViewMatrixID, 1, GL_FALSE, &m_ViewMatrix[0][0]);
		for (int i = 0; i < m_points.size(); i+=2) {
			m_points[i].ModelMatrix = glm::translate(glm::vec3(0, sin(m_points[i].angle), 0));
			m_points[i + 1].ModelMatrix = glm::translate(glm::vec3(0, sin(m_points[i].angle), 0));
			m_mvp = m_ProjectionMatrix * m_ViewMatrix * m_points[i].ModelMatrix;

			glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &m_mvp[0][0]);
			glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &m_points[i].ModelMatrix[0][0]);
			
			/*glUniform3f(LightID2, lightPos.x, 0 - lightPos.y, lightPos.z);*/

			m_points[i].coords = glm::vec3(m_points[i].ModelMatrix * glm::vec4(m_points[i].coords, 1));
			m_points[i + 1].coords = glm::vec3(m_points[i + 1].ModelMatrix * glm::vec4(m_points[i].coords, 1));
			m_points[i].angle += 0.07f;
			m_points[i + 1].angle += 0.07f;

			glDrawArrays(GL_LINES, i, 2);
		}

		glm::vec3 lightPos = {0,0,0};// = m_points[i].coords;

		for (auto lightID : LightIDs) {
			lightPositions[lightID] = glm::translate(rotateToPosition[lightID]) * glm::rotate(glm::mat4(1.f), 0.01f, rotateVector[lightID]) * glm::translate(lightPositions[lightID]) *glm::vec4(lightPositions[lightID], 0);

			glUniform3f(lightID, lightPositions[lightID].x, lightPositions[lightID].y, lightPositions[lightID].z);
		}

		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);
		glDisableVertexAttribArray(2);

		// Swap buffers
		glfwSwapBuffers(m_window);
		glfwPollEvents();

	} // Check if the ESC key was pressed or the window was closed
	while (glfwGetKey(m_window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
		glfwWindowShouldClose(m_window) == 0);

	// Cleanup VBO and shader
	glDeleteBuffers(1, &m_vboID);
	glDeleteProgram(m_programID);
	glDeleteVertexArrays(1, &m_vaoID);
	glDeleteBuffers(1, &m_iboID);

	// Close OpenGL window and terminate GLFW
	glfwTerminate();
}

GLuint Context::LoadShaders(const char * vertex_file_path, const char * fragment_file_path)
{
	// Create the shaders
	GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

	// Read the Vertex Shader code from the file
	std::string VertexShaderCode;
	std::ifstream VertexShaderStream(vertex_file_path, std::ios::in);
	if (VertexShaderStream.is_open()) {
		std::string Line = "";
		while (getline(VertexShaderStream, Line))
			VertexShaderCode += "\n" + Line;
		VertexShaderStream.close();
	}
	else {
		printf("Impossible to open %s. Are you in the right directory ? Don't forget to read the FAQ !\n", vertex_file_path);
		getchar();
		return 0;
	}

	// Read the Fragment Shader code from the file
	std::string FragmentShaderCode;
	std::ifstream FragmentShaderStream(fragment_file_path, std::ios::in);
	if (FragmentShaderStream.is_open()) {
		std::string Line = "";
		while (getline(FragmentShaderStream, Line))
			FragmentShaderCode += "\n" + Line;
		FragmentShaderStream.close();
	}

	GLint Result = GL_FALSE;
	int InfoLogLength;


	// Compile Vertex Shader
	printf("Compiling shader : %s\n", vertex_file_path);
	char const * VertexSourcePointer = VertexShaderCode.c_str();
	glShaderSource(VertexShaderID, 1, &VertexSourcePointer, NULL);
	glCompileShader(VertexShaderID);

	// Check Vertex Shader
	glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(VertexShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (InfoLogLength > 0) {
		std::vector<char> VertexShaderErrorMessage(InfoLogLength + 1);
		glGetShaderInfoLog(VertexShaderID, InfoLogLength, NULL, &VertexShaderErrorMessage[0]);
		printf("%s\n", &VertexShaderErrorMessage[0]);
	}

	// Compile Fragment Shader
	printf("Compiling shader : %s\n", fragment_file_path);
	char const * FragmentSourcePointer = FragmentShaderCode.c_str();
	glShaderSource(FragmentShaderID, 1, &FragmentSourcePointer, NULL);
	glCompileShader(FragmentShaderID);

	// Check Fragment Shader
	glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(FragmentShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (InfoLogLength > 0) {
		std::vector<char> FragmentShaderErrorMessage(InfoLogLength + 1);
		glGetShaderInfoLog(FragmentShaderID, InfoLogLength, NULL, &FragmentShaderErrorMessage[0]);
		printf("%s\n", &FragmentShaderErrorMessage[0]);
	}

	// Link the program
	printf("Linking program\n");
	GLuint ProgramID = glCreateProgram();
	glAttachShader(ProgramID, VertexShaderID);
	glAttachShader(ProgramID, FragmentShaderID);
	glLinkProgram(ProgramID);

	// Check the program
	glGetProgramiv(ProgramID, GL_LINK_STATUS, &Result);
	glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (InfoLogLength > 0) {
		std::vector<char> ProgramErrorMessage(InfoLogLength + 1);
		glGetProgramInfoLog(ProgramID, InfoLogLength, NULL, &ProgramErrorMessage[0]);
		printf("%s\n", &ProgramErrorMessage[0]);
	}

	glDetachShader(ProgramID, VertexShaderID);
	glDetachShader(ProgramID, FragmentShaderID);

	glDeleteShader(VertexShaderID);
	glDeleteShader(FragmentShaderID);

	return ProgramID;
}


GLFWwindow* Context::getWindow()
{
	return this->m_window;
}

Context::Context()
{
	this->m_width = 1336;
	this->m_height = 768;
	
	this->m_windowName = "Aurora Borealis";

}

Context::~Context()
{
}
