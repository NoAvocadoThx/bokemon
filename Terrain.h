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



class Terrain{
private:
	const GLfloat size = 800.0f;

	
	
public:

	static const GLint vertexCount = 128;
	GLint area = vertexCount * vertexCount;
	std::vector<glm::vec3> tris;
	std::vector<unsigned int> indices;
	GLuint VBO, VAO, EBO,Norms,TO;
	GLuint uProjection, uModelview;
	glm::vec3 position;
	GLfloat * tVertices,*tNormals,*textureCoords;
	int * index;
	glm::mat4 toWorld;
	GLint terrainCount = 0;
	const GLfloat width = 10.0f;
	const GLfloat length = 10.0f;
	GLint twidth = 512;
	GLint theight = 512;
	Terrain::Terrain(int,int);
	Terrain::Terrain();
	Terrain::~Terrain();
	void generateTerrain();
	void draw(GLuint);
	unsigned char* loadPPM(const char* filename, int& width, int& height);
	void loadTexture(unsigned char* tdata);
	GLuint loadTexture2(const char *textureFile);
	void Terrain::translate(glm::vec3 transVec);

};




#endif
