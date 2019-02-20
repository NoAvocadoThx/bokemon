//
//  ROBObject.hpp
//  Project2
//
//  Created by Shengxiang Zhou on 10/30/18.
//  Copyright © 2018 Shengxiang Zhou. All rights reserved.
//

#ifndef BALLObject_h
#define BALLObject_h

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

class BALLObject
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
	glm::vec3 position;
	int object_mode;
	glm::vec3 color_diff;
	glm::vec3 color_spec;
	glm::vec3 color_ambi;
	glm::mat4 selfRotate;
	float spinSpeed;
	bool IS_SPHERE;
	float centerx, centery, centerz;
	float shininess;
	float size;
	float angle;
	bool rotateMode, scaleMode;
	bool rotatelock_point, rotatelock_spot;
	glm::vec3 rotAxis;
	float rotAngle;
	float scale_size;
	~BALLObject();
	glm::mat4 toWorld;
	glm::mat4 lightM;
	BALLObject();
	BALLObject(const char* filepath);
	void parse(const char* filepath);
	void draw(GLuint);
	void move(GLfloat distance);
	void spin(GLfloat dt);
	float movespeed;
	// void update();
	void scaleProcess();
	void loadTexture();
	void drawLines(GLuint shaderProgram, glm::mat4 C);
	//void spin(float);
	GLuint VBO, VBO2, VBO3, VAO, EBO, textureID;
	void setPosition(glm::vec3 newPos);

	GLuint uProjection, uModelview, sModelview;
	void translate(glm::vec3 transVec);
	unsigned char* loadPPM(const char* filename, int& width, int& height);
};

#endif
