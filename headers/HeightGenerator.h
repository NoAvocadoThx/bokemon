#ifndef _HEIGHTGENERATOR_H
#define _HEIGHTGENERATOR_H



#ifdef __APPLE__
#include <OpenGL/gl3.h>
#include <OpenGL/glext.h>
#include <OpenGL/gl.h> // Remove this line in future projects
#else
#include <GL/glew.h>
#endif

#include <GLFW/glfw3.h>
#include <GL/glut.h>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include <cstdlib>
#include <random>
#include <cmath>
#include <ctime> 

class HeightGenerator {
private:
	const GLfloat AMP = 260.0f;
	const GLint OCTAVES = 5;
	const GLfloat ROUGHNESS = 0.05f; 
	GLint randomInt() {
		srand((unsigned)time(0));
		int i;
		i = (rand() % 100000) + 1;
		return i;
	}
	
	GLint xOffset = 0;
	GLint zOffset = 0;

public:
	HeightGenerator();
	GLint seed;
	HeightGenerator(int gridX, int gridZ, int vertexCount, int seed);
	GLfloat genHeight(int x, int z);
	GLfloat getInterpolatedNoise(float, float);
	GLfloat interpolate(float a, float b, float blend);
	GLfloat getSmoothNoise(int x, int z);
	GLfloat getNoise(int x, int z);
	
};

#endif