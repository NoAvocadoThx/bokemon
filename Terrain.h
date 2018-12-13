#ifndef _TERRAIN_H
#define _TERRAIN_H



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
#include "HeightGenerator.h"



class Terrain{
private:
	const GLfloat size = 800.0f;

	
	
public:
	GLfloat x, z;
	static const GLint vertexCount = 128;
	GLint area = vertexCount * vertexCount;
	std::vector<glm::vec3> tris;
	std::vector<unsigned int> indices;
	GLint seed;
	GLuint VBO, VAO, EBO,Norms,TO;
	GLuint uProjection, uModelview;
	glm::vec3 position;
	GLfloat * tVertices,*tNormals,*textureCoords;
	int * index;
	float heightMove[vertexCount][vertexCount];
	glm::mat4 toWorld;
	GLint terrainCount = 0;
	const GLfloat width = 10.0f;
	const GLfloat length = 10.0f;
	GLint twidth = 512;
	GLint theight = 512;


	Terrain::Terrain(int,int);
	Terrain::Terrain();
	Terrain::Terrain(HeightGenerator*);
	Terrain::~Terrain();
	void generateTerrain(HeightGenerator*);
	void draw(GLuint);
	unsigned char* loadPPM(const char* filename, int& width, int& height);
	void loadTexture(unsigned char* tdata);
	void loadTexture2();
	void Terrain::translate(glm::vec3 transVec);
	glm::vec3 calculateNormal(int x, int z, HeightGenerator*);
	float getHeight(int x, int z, HeightGenerator*);
	void scaleSize(GLfloat scaleV,GLfloat,GLfloat);
	float getHeightMove(float, float);
	static float barryCentric(glm::vec3 p1, glm::vec3 p2, glm::vec3 p3, glm::vec2  pos);
};




#endif
