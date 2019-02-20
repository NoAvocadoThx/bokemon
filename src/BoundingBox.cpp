#include "BoundingBox.h"
#include "Window.h"

BoundingBox::BoundingBox(std::vector<GLfloat> edges, std::vector<glm::vec3> vertices) {
	this->toWorld = glm::mat4(1.0f);

	// Create buffers/arrays
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	edgesBoundingBox = edges;
	verticesBoundingBox = vertices;

	// Bind the Vertex Array Object first, then bind and set vertex buffer(s) and attribute pointer(s).
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * edgesBoundingBox.size(), &edgesBoundingBox[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLfloat) * indicesBoundingBox.size(), &indicesBoundingBox[0], GL_STATIC_DRAW);


	glVertexAttribPointer(0,// This first parameter x should be the same as the number passed into the line "layout (location = x)" in the vertex shader. In this case, it's 0. Valid values are 0 to GL_MAX_UNIFORM_LOCATIONS.
		3, // This second line tells us how any components there are per vertex. In this case, it's 3 (we have an x, y, and z component)
		GL_FLOAT, // What type these components are
		GL_FALSE, // GL_TRUE means the values should be normalized. GL_FALSE means they shouldn't
		3 * sizeof(GLfloat), // Offset between consecutive vertex attributes. Since each of our vertices have 3 floats, they should have the size of 3 floats in between
		(GLvoid*)0); // Offset of the first vertex's component. In our case it's 0 since we don't pad the vertices array with anything.

	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0); // Note that this is allowed, the call to glVertexAttribPointer registered VBO as the currently bound vertex buffer object so afterwards we can safely unbind

	glBindVertexArray(0); // Unbind VAO (it's always a good thing to unbind any buffer/array to prevent strange bugs), remember: do NOT unbind the EBO, keep it bound to this VAO
}

BoundingBox::~BoundingBox() {
	// Properly de-allocate all resources once they've outlived their purpose
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
}

void BoundingBox::draw(GLuint shaderProgram) {
	// Calculate combination of the model (toWorld), view (camera inverse), and perspective matrices
	glm::mat4 MVP = Window::P * Window::V * toWorld;
	// We need to calculate this because as of GLSL version 1.40 (OpenGL 3.1, released March 2009), gl_ModelViewProjectionMatrix has been
	// removed from the language. The user is expected to supply this matrix to the shader when using modern OpenGL.
	GLuint MatrixID = glGetUniformLocation(shaderProgram, "MVP");
	glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);

	glBindVertexArray(VAO);

	if (collisionflag) {
		GLuint color = glGetUniformLocation(shaderProgram, "c");
		glUniform4f(color, 1.0f, 0.0f, 0.0f, 1.0f);
	}
	else {
		GLuint color = glGetUniformLocation(shaderProgram, "c");
		glUniform4f(color, 1.0f, 1.0f, 1.0f, 1.0f);
	}

	glDrawArrays(GL_LINES, 0, 2);
	glDrawArrays(GL_LINES, 1, 2);
	glDrawArrays(GL_LINES, 2, 2);
	glDrawArrays(GL_LINES, 3, 2);
	glDrawArrays(GL_LINES, 4, 2);
	glDrawArrays(GL_LINES, 5, 2);
	glDrawArrays(GL_LINES, 6, 2);
	glDrawArrays(GL_LINES, 7, 2);
	glDrawArrays(GL_LINES, 8, 2);

	glDrawArrays(GL_LINES, 10, 2);
	glDrawArrays(GL_LINES, 12, 2);
	glDrawArrays(GL_LINES, 14, 2);

	glBindVertexArray(0);
}

std::vector<float> BoundingBox::getBoundary() {
	std::vector<glm::vec3> new_pos;
	std::vector<float> boundary;
	float minX = INFINITY, minY = INFINITY, minZ = INFINITY;   // min vertex
	float maxX = -INFINITY, maxY = -INFINITY, maxZ = -INFINITY;   // max vertex

	/*
	std::cout << toWorld[0][0] << " " << toWorld[1][0] << " " << toWorld[2][0] << " " << toWorld[3][0] << std::endl;
	std::cout << toWorld[0][1] << " " << toWorld[1][1] << " " << toWorld[2][1] << " " << toWorld[3][1] << std::endl;
	std::cout << toWorld[0][2] << " " << toWorld[1][2] << " " << toWorld[2][2] << " " << toWorld[3][2] << std::endl;
	std::cout << toWorld[0][3] << " " << toWorld[1][3] << " " << toWorld[2][3] << " " << toWorld[3][3] << std::endl;
	 */

	for (int i = 0; i < verticesBoundingBox.size(); i++) {
		glm::vec3 newpos = glm::mat3(toWorld) * verticesBoundingBox[i];
		newpos = newpos + glm::vec3(toWorld[3][0], toWorld[3][1], toWorld[3][2]);
		new_pos.push_back(newpos);

		// check for min and max
		if (newpos.x < minX) {
			minX = newpos.x;
		}
		if (newpos.y < minY) {
			minY = newpos.y;
		}
		if (newpos.z < minZ) {
			minZ = newpos.z;
		}
		if (newpos.x > maxX) {
			maxX = newpos.x;
		}
		if (newpos.y > maxY) {
			maxY = newpos.y;
		}
		if (newpos.z > maxZ) {
			maxZ = newpos.z;
		}
	}

	boundary.push_back(maxX);
	boundary.push_back(minX);
	boundary.push_back(maxY);
	boundary.push_back(minY);
	boundary.push_back(maxZ);
	boundary.push_back(minZ);

	return boundary;
}