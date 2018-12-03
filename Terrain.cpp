#include "Terrain.h"
#include "Window.h"
#include <iostream>
#include <fstream>
#include <cmath>
#include <ctime>    // For time()
#include <cstdlib>  // For srand() and rand()

static const GLint lRes = 128.0f;
static const GLint wRes = 128.0f;
int sz = lRes * wRes;
std::vector<glm::vec3> terrainVertices(sz);
std::vector<glm::vec3> terrainNormals(sz);

Terrain::Terrain() {
	toWorld = glm::mat4(1.0f);
	
	generateTerrain();
	
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	glGenBuffers(1, &Norms);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, terrainVertices.size() * sizeof(glm::vec3), &terrainVertices[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0,// This first parameter x should be the same as the number passed into the line "layout (location = x)" in the vertex shader. In this case, it's 0. Valid values are 0 to GL_MAX_UNIFORM_LOCATIONS.
		3, // This second line tells us how any components there are per vertex. In this case, it's 3 (we have an x, y, and z component)
		GL_FLOAT, // What type these components are
		GL_FALSE, // GL_TRUE means the values should be normalized. GL_FALSE means they shouldn't
		3 * sizeof(GLfloat), // Offset between consecutive indices. Since each of our vertices have 3 floats, they should have the size of 3 floats in between
		(GLvoid*)0); // Offset of the first vertex's component. In our case it's 0 since we don't pad the vertices array wi
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, Norms);
	glBufferData(GL_ARRAY_BUFFER, terrainNormals.size() * sizeof(glm::vec3), &terrainNormals[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, tris.size() * sizeof(glm::vec3), &tris[0], GL_STATIC_DRAW);
	// Unbind the currently bound buffer so that we don't accidentally make unwanted changes to it.
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	// Unbind the VAO now so we don't accidentally tamper with it.
	// NOTE: You must NEVER unbind the element array buffer associated with a VAO!
	glBindVertexArray(0);
}


Terrain::~Terrain() {
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	glDeleteBuffers(1, &Norms);
}

void Terrain::generateTerrain() {
	int i = 0;
	for (int z = 0; z < lRes; z++) {
		for (int x = 0; x < wRes; x++) {
			glm::vec3 v = glm::vec3(x/(GLfloat)lRes, 0, z/(GLfloat)lRes);
			v.x *= width;
			v.z *= length;
			v.x -= width / 2;
			v.z -= length / 2;
			terrainVertices[i]=v;
			terrainNormals[i]=glm::vec3(0, 1, 0);
			//i       i+1        i+2 .... i+(width-1)
			//i+width i+width+1  .....    i+width+(width-1)
			if ((i + 1 % (int)width) != 0 && z + z < lRes) {
				glm::vec3 tri = glm::vec3(i, i + wRes, i + wRes + 1);
				glm::vec3 tri2 = glm::vec3(i, i + wRes + 1, i + 1);
				tris.push_back(tri);
				tris.push_back(tri2);
			}
			i++;
		}
	
	}
	terrainCount = tris.size();
}

void Terrain::draw(GLuint shaderProgram)
{


	// Calculate the combination of the model and view (camera inverse) matrices
	glm::mat4 modelview = Window::V * toWorld;

	//glm::mat4 translationMat = glm::translate(glm::mat4(1.0f), position);
	// We need to calcullate this because modern OpenGL does not keep track of any matrix other than the viewport (D)
	// Consequently, we need to forward the projection, view, and model matrices to the shader programs
	// Get the location of the uniform variables "projection" and "modelview"
	uProjection = glGetUniformLocation(shaderProgram, "projection");
	uModelview = glGetUniformLocation(shaderProgram, "modelview");
	// Now send these values to the shader program
    glUniformMatrix4fv(uProjection, 1, GL_FALSE, &Window::P[0][0]);
	glUniformMatrix4fv(uModelview, 1, GL_FALSE, &modelview[0][0]);

	//glUniformMatrix1i(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, &toWorld[0][0]);



	// Now draw the cube. We simply need to bind the VAO associated with it.
	glBindVertexArray(VAO);
	//glActiveTexture(GL_TEXTURE0);
	//glBindTexture(GL_TEXTURE_CUBE_MAP, sphereData);
	glDrawArrays(GL_TRIANGLES, 0, terrainVertices.size());
	// Tell OpenGL to draw with triangles, using indices.size() indices, the type of the indices, and the offset to start from
	glDrawElements(GL_TRIANGLES, tris.size(), GL_UNSIGNED_INT, 0);
	// Unbind the VAO when we're done so we don't accidentally draw extra stuff or tamper with its bound buffers
	glBindVertexArray(0);




}