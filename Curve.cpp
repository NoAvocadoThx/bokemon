#include "Curve.h"
#include "Window.h"
#include <iostream>
#include <fstream>
#include <cmath>
#include <ctime>    // For time()
#include <cstdlib>  // For srand() and rand()


Curve::Curve(glm::vec3 p0, glm::vec3 p1, glm::vec3 p2, glm::vec3 p3) {
	G = glm::mat4(glm::vec4(p0, 1.0f), glm::vec4(p1, 1.0f),
		glm::vec4(p2, 1.0f), glm::vec4(p3, 1.0f));
	B = glm::mat4(glm::vec4(-1.0f, 3.0f, -3.0f, 1.0f),
		glm::vec4(3.0f, -6.0f, 3.0f, 0.0f),
		glm::vec4(-3.0f, 3.0f, 0.0f, 0.0f),
		glm::vec4(1.0f, 0.0f, 0.0f, 0.0f));
	this->p0 = p0;
	this->p1 = p1;
	this->p2 = p2;
	this->p3 = p3;

	
	lerp(); 
 
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, curvePts.size() * sizeof(glm::vec3), &curvePts[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0); 

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	
	glGenVertexArrays(1, &VAO2);
	glGenBuffers(1, &VBO2);
	glBindVertexArray(VAO2);
	glBindBuffer(GL_ARRAY_BUFFER, VBO2);
	glBufferData(GL_ARRAY_BUFFER, ctrPts.size() * sizeof(glm::vec3), ctrPts.data(), GL_STATIC_DRAW);
	

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0); 
	
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}
Curve::~Curve() {
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	// point VAO VBO
	glDeleteVertexArrays(1, &VAO2);
	glDeleteBuffers(1, &VBO2);
}

void Curve::lerp() {
	for (int i = 0; i < 151; i++) {
		T = glm::vec4(pow(i / 150.0f, 3.0f), pow(i / 150.0f, 2.0f), i / 150.0f, 1.0f);
		glm::vec4 xt = G * B*T;
		curvePts.push_back(glm::vec3(xt.x,xt.y,xt.z));
	}
}
void Curve::draw(GLuint shaderProgram) {
	// Calculate the combination of the model and view (camera inverse) matrices
	glm::mat4 modelview = Window::V * toWorld;

	uProjection = glGetUniformLocation(shaderProgram, "projection");
	uModelview = glGetUniformLocation(shaderProgram, "modelview");
	glUniformMatrix4fv(uProjection, 1, GL_FALSE, &Window::P[0][0]);
	glUniformMatrix4fv(uModelview, 1, GL_FALSE, &modelview[0][0]);
	glBindVertexArray(VAO);
	glDrawArrays(GL_LINE_STRIP, 0, curvePts.size());
	glBindVertexArray(0);
}