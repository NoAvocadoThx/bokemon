//
//  ROBObject.hpp
//  Project2
//
//  Created by Shengxiang Zhou on 10/30/18.
//  Copyright © 2018 Shengxiang Zhou. All rights reserved.
//

#ifndef ROBObject_h
#define ROBObject_h

#ifdef __APPLE__
#include <OpenGL/gl3.h>
#include <OpenGL/glext.h>
#include <OpenGL/gl.h> // Remove this line in future projects
#else
#include <GL/glew.h>
#endif

#include <GLFW/glfw3.h>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include "Terrain.h"

class ROBObject
{
private:
	std::vector<unsigned int> vindices;
	std::vector<unsigned int> nindices;
	std::vector<unsigned int> tindices;
	std::vector<glm::vec3> temp_vertices;
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec3> temp_normals;
	std::vector<glm::vec3> normals;
	std::vector<glm::vec2> temp_textures;
	std::vector<glm::vec2> textures;
	std::vector<unsigned int> indices;

public:
	int duration, durationdead;
	bool finishedfire, finisheddie;
	bool firing = false;
	bool dying = false;
	std::vector<glm::vec3> boxVertices;
	std::vector<GLfloat> boundingbox;
	glm::vec3 position;
	int object_mode;
	glm::vec3 color_diff;
	glm::vec3 color_spec;
	glm::vec3 color_ambi;
	bool IS_SPHERE;
	bool isSphere;
	float centerx, centery, centerz;
	float minx, miny, minz, maxx, maxy, maxz;
	float minX, maxX, minY, maxY, maxZ, minZ;
	float shininess;
	float size;
	float angle;
	bool rotateMode=false;
	bool rotatelock_point, rotatelock_spot;
	glm::vec3 rotAxis;
	float rotAngle;
	glm::vec3 viewdir;	float scale_size;
	~ROBObject();
	glm::mat4 toWorld;
	glm::mat4 lightM;
	ROBObject();
	ROBObject(const char* filepath);
	void parse(const char* filepath);
	void draw(GLuint);
	void move(float distance);
	float movespeed;
	
	// void update();
	void scaleProcess();
	void loadTexture();
	void draw(GLuint shaderProgram, glm::mat4 C);
	void drawLines(GLuint shaderProgram, glm::mat4 C);
	//void spin(float);
	GLuint VBO, VBO2, VBO3, VAO, EBO, textureID;
	void setPosition(glm::vec3 newPos);
	void ROBObject::translateX(float xVal);
	void ROBObject::translateY(float xVal);
	void ROBObject::translateZ(float xVal);
	GLuint uProjection, uModelview, sModelview;
	void translate(glm::vec3 transVec);
	void scalesize(float val);
	void fire(void);
	void die(void);
	unsigned char* loadPPM(const char* filename, int& width, int& height);
	void walk();
};

#endif
