#ifndef BoundingBox_hpp
#define BoundingBox_hpp

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/mat4x4.hpp> // glm::mat4
#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective

#include <vector>

class BoundingBox {
public:
	// constructor destructor
	BoundingBox(std::vector<GLfloat>, std::vector<glm::vec3>);
	~BoundingBox();

	void draw(GLuint shaderProgram, bool dead);
	std::vector<float> getBoundary();

	glm::mat4 toWorld;
	glm::vec3 min;
	glm::vec3 max;

	GLuint VBO, VAO, EBO;
	std::vector<glm::vec3> verticesBoundingBox;
	std::vector<GLfloat> edgesBoundingBox;
	std::vector<GLfloat> indicesBoundingBox = {
		// Front face
		0, 1, 2,
		2, 3, 0,
		// Top face
		1, 5, 6,
		6, 2, 1,
		// Back face
		7, 6, 5,
		5, 4, 7,
		// Bottom face
		4, 0, 3,
		3, 7, 4,
		// Left face
		4, 5, 1,
		1, 0, 4,
		// Right face
		3, 2, 6,
		6, 7, 3
	};
};

#endif /* BoundingBox_hpp */