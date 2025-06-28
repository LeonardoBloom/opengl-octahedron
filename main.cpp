// standard library calls
//#include <GL/glew.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>
#include <iostream>
#include <fstream>
#include <cmath>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Utils.h"

#define _USE_MATH_DEFINES
#include <cstdlib>

#define numVAOs 1
#define numVBOs 2

float cameraX, cameraY, cameraZ;
float shapeLocX, shapeLocY, shapeLocZ;

GLuint renderingProgram;
GLuint vao[numVAOs];
GLuint vbo[numVBOs];

GLuint mvLoc, pLoc;
int width, height;
float aspect;

glm::mat4 pMat, vMat, mMat, mvMat, tMat, rMat;


void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
		std::cout << "Pressed ESC" << std::endl;
	}
}


void setupVertices(void) {

	float vertexPositions[72] = {
		0.0f, 0.0f, 3.0f, 1.5f, 1.5f, 0.0f, -1.5f, 1.5f, 0.0f,
		0.0f, 0.0f, 3.0f, -1.5f, 1.5f, 0.0f, -1.5f, -1.5f, 0.0f,
		0.0f, 0.0f, 3.0f, -1.5f, -1.5f, 0.0f, 1.5f, -1.5f, 0.0f,
		0.0f, 0.0f, 3.0f, 1.5f, -1.5f, 0.0f, 1.5f, 1.5f, 0.0f,

		0.0f, 0.0f, -3.0f, 1.5f, 1.5f, 0.0f, -1.5f, 1.5f, 0.0f,
		0.0f, 0.0f, -3.0f, -1.5f, 1.5f, 0.0f, -1.5f, -1.5f, 0.0f,
		0.0f, 0.0f, -3.0f, -1.5f, -1.5f, 0.0f, 1.5f, -1.5f, 0.0f,
		0.0f, 0.0f, -3.0f, 1.5f, -1.5f, 0.0f, 1.5f, 1.5f, 0.0f,
	};

	glGenVertexArrays(1, vao);
	glBindVertexArray(vao[0]);
	glGenBuffers(numVBOs, vbo);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexPositions), vertexPositions, GL_STATIC_DRAW);

	std::cout << "vertices created" << std::endl;

}



void init(GLFWwindow* window) {
	renderingProgram = createShaderProgram("vertexShader.glsl", "fragShader.glsl");
	
	cameraX = 3.0f; cameraY = 5.0f; cameraZ = 25.0f;
	shapeLocX = 0.0f; shapeLocY = -2.0f; shapeLocZ = 0.0f;

	setupVertices();

}

void display(GLFWwindow* window, double currentTime) {
	glClear(GL_DEPTH_BUFFER_BIT);
	glClear(GL_COLOR_BUFFER_BIT);

	glUseProgram(renderingProgram);

	mvLoc = glGetUniformLocation(renderingProgram, "mv_matrix");
	pLoc = glGetUniformLocation(renderingProgram, "p_matrix");

	glfwGetFramebufferSize(window, &width, &height);
	aspect = (float)width / (float)height;
	pMat = glm::perspective(glm::radians(60.0f), aspect, 0.1f, 1000.0f);

	// build VIEW matrix, MODEL matrix, and MODEL-VIEW MATRIX
	vMat = glm::translate(glm::mat4(1.0f), glm::vec3(-cameraX, -cameraY, -cameraZ));



	tMat = glm::translate(glm::mat4(1.0f), glm::vec3(sin(0.35f) * 8.0f, cos(0.52f) * 8.0f, sin(0.7f) * 8.0f));
	rMat = glm::rotate(glm::mat4(1.0f), 1.75f * (float)currentTime, glm::vec3(0.0f, 1.0f, 0.0f));
	rMat = glm::rotate(rMat, 1.75f * (float)currentTime, glm::vec3(1.0f, 0.0f, 0.0f));
	rMat = glm::rotate(rMat, 1.75f * (float)currentTime, glm::vec3(0.0f, 0.0f, 1.0f));
	// the 1.75 adjusts the rotation speed

	mMat = tMat * rMat;

	mvMat = vMat * mMat;

	// copy PERSPECTIVE and MV matrices to corresponding uniform variables
	glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(mvMat));
	glUniformMatrix4fv(pLoc, 1, GL_FALSE, glm::value_ptr(pMat));

	// associate VBO with the corresponding vertex attribute in the vertex shader
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	// adjust OpenGL settings and draw model
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glDrawArrays(GL_TRIANGLES, 0, 72);

}



int main()
{
	// initialize glfw for window creation
	glfwInit();
	// tell glfw what version of opengl to use
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	// configure the profile for open ( CORE in this case )
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// create a window object that holds all the windowing data
	GLFWwindow* window = glfwCreateWindow(800, 600, "My OCTAHEDRON", NULL, NULL);

	if (window == NULL)		// if window fails to create:
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	// since window was successful in being created, then make our window object the star of the show basically
	glfwMakeContextCurrent(window);

	
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	glViewport(0, 0, 800, 600);

	// We register the callback functions after we've created the widnow and before the render loop is initiated.
	glfwSetFramebufferSizeCallback(window,  framebuffer_size_callback);
	glfwSwapInterval(1);
	init(window);
	while (!glfwWindowShouldClose(window)) {

		display(window, glfwGetTime());

		//processInput(window);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;

}