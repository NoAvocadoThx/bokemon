#ifndef _WATER_H
#define _WATER_H

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



class Water {
private:

public:

	GLuint VBO, VAO, EBO,reflectionFrameBuffer,refractionFrameBuffer;
	//depth buffer
	GLuint reflectionDB, refractionDB;
	GLuint reflectionTexture, refractionTexture;
	GLuint refracDepthTexture;
	GLuint uProjection, uModelview;
	glm::vec3 position;
	glm::mat4 toWorld;
	GLfloat size=100;
	GLuint dudvMap;
	GLuint texture[1];
	GLint reflec_width = 1920;
	GLint reflec_height = 1080;
	GLint refrac_width = 1920;
	GLint refrac_height = 1080;
	GLfloat waterHeight = 1.0f;
	GLdouble startTime,curTime;
	GLfloat moveSpeed = 0.00003;

	Water();
	~Water();
	GLuint loadTexture(std::string str);
	void draw(GLuint,glm::vec3);
	void update();
	void bindReflectionFrameBuffer();
	void bindRefractionFrameBuffer();
	void unbindCurrentFrameBuffer();
    void createDepthTexture();
	GLfloat getHeight();
	void scale(GLfloat);
	void translate(glm::vec3);
	//void update();


	

};


#endif