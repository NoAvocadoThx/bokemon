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
	GLuint reflectionDB, refractionDP;
	GLuint reflectionTexture, refractionTexture;
	GLuint uProjection, uModelview;
	glm::vec3 position;
	glm::vec3 toWorld;
	GLuint dudvMap;
	GLuint texture[1];


	Water();
	~Water();
	GLuint loadTexture(std::string str);
	void draw(GLuint);
	void update();

	

};


#endif