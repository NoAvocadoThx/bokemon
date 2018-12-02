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



using namespace std;
class Terrain{
private:
	const GLfloat size = 800.0f;
	const GLint vertexCount = 128;
	GLfloat x;
	GLfloat z;
	vector<glm::vec3> tris;
	vector<glm::vec3> vertices;
	vector<glm::vec3> normals;
public:
	GLuint VBO, VAO, EBO;
	const GLfloat width = 10.0f;
	const GLfloat length = 10.0f;
	Terrain::Terrain(int,int);
	Terrain::Terrain();
	Terrain::~Terrain();
	void generateTerrain(int, int);
	void draw(GLuint);


};




#endif
