#ifndef GEOMETRY_H
#define GEOMETRY_H

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
#include "Node.h"
#include "OBJObject.h"

class Geometry :public Node
{




public:
	OBJObject *currObj;
	GLfloat size;
	GLfloat angle;
	glm::vec3 position;
	GLint dirOn = 1;
	//std::vector<GLint> faces;
	glm::mat4 toWorld;
	GLfloat newX;
	GLfloat newY;
	GLfloat newZ;
	GLfloat scaleX, scaleY, scaleZ;
	GLfloat min_x, max_x, min_y, max_y, min_z, max_z;
	GLfloat radius=5.0f;

	glm::vec3 centerP;
	glm::vec3 origin;
	glm::mat4 M;
	std::vector<unsigned int> indices;
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec3> normals;
	std::vector<glm::vec3> random;
	std::vector<glm::vec2> tex;
	std::vector< unsigned int > vertexIndices, uvIndices, normalIndices;
	std::vector<glm::vec3> a_vertices;
	std::vector<glm::vec3> a_normals;
	std::vector<glm::vec2> a_uv;
	Geometry(const char* filepath);
	Geometry::~Geometry();
	// These variables are needed for the shader program
	GLuint VBO, VAO, EBO, Norms,TexO;
	GLuint uProjection, uModelview;
	bool objRotation;
	bool isSphere;
	bool toggleSphere;

	//for material
	glm::mat3 material;
	//for texture
	GLint twidth = 512;
	GLint theight = 512;   // texture width/height [pixels]

	//trackBall color
	glm::vec3 dirTColor;
	glm::vec3 ptTColor;
	glm::vec3 sptTColor;

	

	void parse(const char* filepath);
	void draw(GLuint, glm::mat4 m);
	void update();
	void center();
	unsigned char* loadPPM(const char* filename, int& width, int& height);
	void loadTexture(unsigned char* tdata);
	void scale(float val);
	bool inSphere(glm::vec3 origin, GLfloat radius, Plane pl[]);
	glm::vec4 getOrigin(glm::mat4 M);
	
};

#endif
