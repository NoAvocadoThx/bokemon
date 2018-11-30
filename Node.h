#ifndef NODE_H
#define NODE_H
#include <iostream>
#include <math.h>
#include <glm/gtc/type_ptr.hpp>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Plane.h"
class Node {
public:
	glm::vec3 origin;
	GLfloat radius;
	bool isSphere;
	glm::mat4 M;
	virtual void draw(GLuint shaderProgram, glm::mat4 m) = 0;
	virtual void update() = 0;
	virtual void scale(float val) = 0;
	virtual bool inSphere(glm::vec3 origin,GLfloat radius, Plane pl[])=0;
	virtual glm::vec4 getOrigin(glm::mat4 M)=0;
};
#endif
