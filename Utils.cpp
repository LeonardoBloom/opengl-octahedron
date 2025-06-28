#include <iostream>
#include <string>
#include <fstream>

#include <glad/glad.h>

#include <GLFW/glfw3.h>

#define _USE_MATH_DEFINES
#include <cmath>
#include <cstdlib>

using namespace std;


// function to read the shader glsl file here
string readShaderSource(const char* filePath) {
	string content;
	ifstream fileStream(filePath, ios::in);
	string line = "";
	while (!fileStream.eof()) {
		getline(fileStream, line);
		content.append(line + "\n");
		cout << line << endl;
	}
	fileStream.close();
	return content;
}

// START ERROR LOG HANDLING
void printShaderLog(GLuint shader) {
	int len = 0;
	int chWrittn = 0;
	char* log;
	glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &len);
	if (len > 0) {
		log = (char*)malloc(len);
		glGetShaderInfoLog(shader, len, &chWrittn, log);
		cout << "Shader Info Log: " << log << endl;
		free(log);
	}
}

void printProgramLog(int prog) {
	int len = 0;
	int chWrittn = 0;
	char* log;
	glGetProgramiv(prog, GL_INFO_LOG_LENGTH, &len);
	if (len > 0) {
		log = (char*)malloc(len);
		glGetProgramInfoLog(prog, len, &chWrittn, log);
		cout << "Program Info Log: " << log << endl;
		free(log);
	}
}
bool checkOpenGLError() {
	bool foundError = false;
	int glErr = glGetError();
	while (glErr != GL_NO_ERROR) {
		cout << "glError: " << glErr << endl;
		foundError = true;
		glErr = glGetError();
	}
	return foundError;
}

// END ERROR LOG HANDLING

// CREATE SHADER PROGRAM
GLuint createShaderProgram(const char* vertexShader, const char* fragShader) {

	GLint vertCompiled;
	GLint fragCompiled;
	GLint linked;

	std::cout << "render successful" << std::endl;
	string vertShaderStr = readShaderSource(vertexShader);
	string fragShaderStr = readShaderSource(fragShader);

	// purpose of vertex is simply to plot the points we desire to display the pixels
	const char* vshaderSource = vertShaderStr.c_str();
	// verices move through the pipeline to the rasterizer (transforms vertices into pixel locations)

	// purpose of fragment is to set the rgb color of a pixel to be displayed
	const char* fshaderSource = fragShaderStr.c_str();
	//"{ if (gl_FragCoord.x < 295) color = vec4(1.0, 0.0, 0.0, 1.0); else color = vec4(0.0, 0.0, 1.0, 1.0); }"

	std::cout << "loaded shaders" << std::endl;

	// creates empty shader objects of type VERTEX, and FRAGMENT
	GLuint vShader = glCreateShader(GL_VERTEX_SHADER);
	GLuint fShader = glCreateShader(GL_FRAGMENT_SHADER);
	std::cout << "empty shader object created" << std::endl;

	// loads GLSL code from the shaderSource strings, into vShader and fShader respectively
	// glShaderSource( 
	//				[destination shader], 
	//				[# of strings in shader source code] , 
	//				[array of pointers of strings containing source code], 
	//				[unused parameter?]) 
	glShaderSource(vShader, 1, &vshaderSource, NULL);
	glShaderSource(fShader, 1, &fshaderSource, NULL);
	std::cout << "shaders attached" << std::endl;

	// compiles the shaders
	glCompileShader(vShader);
	checkOpenGLError();
	glGetShaderiv(vShader, GL_COMPILE_STATUS, &vertCompiled);
	if (vertCompiled != 1) {
		cout << "vertex compilation failed" << endl;
		printShaderLog(vShader);
	}


	glCompileShader(fShader);
	checkOpenGLError();
	glGetShaderiv(fShader, GL_COMPILE_STATUS, &fragCompiled);
	if (fragCompiled != 1) {
		cout << "fragment compilation failed" << endl;
		printShaderLog(fShader);
	}

	GLuint vfProgram = glCreateProgram(); // creates a program object and saves the integer ID that points to it

	// attach each created shader to the program object above "vfProgram"
	glAttachShader(vfProgram, vShader);
	glAttachShader(vfProgram, fShader);


	glLinkProgram(vfProgram); // requests GLSL compiler to insure they are compatible
	glGetProgramiv(vfProgram, GL_LINK_STATUS, &linked);
	if (linked != 1) {
		cout << "linking failed" << endl;
		printProgramLog(vfProgram);
	}

	std::cout << "render successful" << std::endl;

	return vfProgram;
}
