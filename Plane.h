
#ifndef PLANE_H
#define PLANE_H

#ifdef __APPLE__
#include <OpenGL/gl3.h>
#include <OpenGL/glext.h>
#include <OpenGL/gl.h> // Remove this line in future projects
#else
#include <GL/glew.h>

#include <GLFW/glfw3.h>
#include <GL/glut.h>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#endif


#include <vector>
class Plane
{

public:

	float d;

	glm::vec3 normal, point,pOnPlane;
	Plane::Plane(glm::vec3, glm::vec3, glm::vec3);
	Plane::Plane();

	Plane::~Plane();
	void set(glm::vec3, glm::vec3, glm::vec3);


	float distance(glm::vec3 p);


};
#endif