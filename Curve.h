#ifndef _CURVE_H_
#define _CURVE_H_

#include <math.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
using namespace std;
class Curve {
private:

public:
	glm::mat4 toWorld = glm::mat4(1.0f);
	GLuint uProjection, uModelview; 
	GLuint VBO, VAO, EBO;
	GLuint VAO2, VBO2;
	glm::vec3 p0, p1, p2, p3;
	glm::mat4 G,B;
	glm::vec4 T;

	Curve(glm::vec3 p0, glm::vec3 p1, glm::vec3 p2, glm::vec3 p3);
	~Curve();

	vector<glm::vec3> anchor;
	vector<glm::vec3> ctrPts; 
	vector<glm::vec3> curvePts; 

	void lerp();


	void draw(GLuint shaderProgram);
	


};
#endif