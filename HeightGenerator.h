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
	const GLfloat AMP = 160.0f;
	const GLint OCTAVES = 5;
	const GLfloat ROUGHNESS = 0.05f; 
	GLint randomInt(GLint a, GLint b) {
		GLint random = ((GLint)rand()) / (GLint)RAND_MAX;
		GLint diff = b - a;
		GLint r = random * diff;
		return (GLint)a + r;
	}
	GLint seed;
	GLint xOffset = 0;
	GLint zOffset = 0;

public:
	HeightGenerator();
	HeightGenerator(int gridX, int gridZ, int vertexCount, int seed);
	GLfloat genHeight(int x, int z);
	GLfloat getInterpolatedNoise(float, float);
	GLfloat interpolate(float a, float b, float blend);
	GLfloat getSmoothNoise(int x, int z);
	GLfloat getNoise(int x, int z);
	
};

#endif