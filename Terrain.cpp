#include "Terrain.h"
#include "Window.h"
#include <iostream>
#include <fstream>
#include <cmath>
#include <ctime>    // For time()
#include <cstdlib>  // For srand() and rand()

static const GLint lRes = Terrain::vertexCount;
static const GLint wRes = Terrain::vertexCount;
int sz = lRes * wRes;
std::vector<glm::vec3> terrainVertices(sz);
std::vector<glm::vec3> terrainNormals(sz);


//GLuint terrainTexture[1];
GLuint texture0;

Terrain::Terrain() {
	toWorld = glm::mat4(1.0f);
	
	generateTerrain();
	std::string str("../grass2.ppm");
	if(str.c_str()==NULL){
		std::cout << "null" << std::endl;
		exit(1);
	}
	//unsigned char* tdata = loadPPM(str.c_str(), twidth, theight);
	texture0 = loadTexture2(str.c_str());
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	glGenBuffers(1, &Norms);
	glGenBuffers(1, &TO);
	

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(*tVertices) * sz*3, tVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0,// This first parameter x should be the same as the number passed into the line "layout (location = x)" in the vertex shader. In this case, it's 0. Valid values are 0 to GL_MAX_UNIFORM_LOCATIONS.
		3, // This second line tells us how any components there are per vertex. In this case, it's 3 (we have an x, y, and z component)
		GL_FLOAT, // What type these components are
		GL_FALSE, // GL_TRUE means the values should be normalized. GL_FALSE means they shouldn't
		3 * sizeof(GLfloat), // Offset between consecutive indices. Since each of our vertices have 3 floats, they should have the size of 3 floats in between
		(GLvoid*)0); // Offset of the first vertex's component. In our case it's 0 since we don't pad the vertices array wi
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, Norms);
	glBufferData(GL_ARRAY_BUFFER, sizeof(*tNormals) * sz * 3, tNormals, GL_STATIC_DRAW);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	
	//std::cout << "1" << std::endl;
	glBindBuffer(GL_ARRAY_BUFFER, TO);
	glBufferData(GL_ARRAY_BUFFER, sz * 2 * sizeof(*textureCoords), textureCoords, GL_STATIC_DRAW);
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (GLvoid*)0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(*index) * 6 * (lRes - 1)*(wRes - 1) , index, GL_STATIC_DRAW);
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
	glDeleteBuffers(1, &TO);
}

void Terrain::generateTerrain() {
	int i = 0;
	tVertices = new GLfloat[sz * 3];
	tNormals = new GLfloat[sz * 3];
	textureCoords = new GLfloat[sz * 2];
	index = new int[6 * (lRes - 1)*(wRes - 1)];
	for (int z = 0; z < lRes; z++) {
		for (int x = 0; x < wRes; x++) {
			/*glm::vec3 v = glm::vec3(x/(GLfloat)lRes, 0, z/(GLfloat)lRes);
			v.x *= width;
			v.z *= length;
			v.x -= width / 2;
			v.z -= length / 2;
			terrainVertices[i]=v;
			terrainNormals[i]=glm::vec3(0, 1, 0);
			//i       i+1        i+2 .... i+(width-1)
			//i+width i+width+1  .....    i+width+(width-1)
			if ((i + 1 % (int)wRes) != 0 && z + 1 < lRes) {
				glm::vec3 tri = glm::vec3(i, i + wRes, i + wRes + 1);
				glm::vec3 tri2 = glm::vec3(i, i + wRes + 1, i + 1);
				tris.push_back(tri);
				tris.push_back(tri2);
			}
			i++;*/
			tVertices[i * 3] = (float)x / ((float)lRes - 1)*size;
			tVertices[i * 3 + 1] = 0;
			tVertices[i * 3 + 2] = (float)z / ((float)wRes - 1)*size;
			tNormals[i * 3] = 0;
			tNormals[i * 3 + 1] = 1;
			tNormals[i * 3 + 2] = 0;
			textureCoords[i * 2] = (float)x / ((float)lRes - 1);
			textureCoords[i * 2 + 1] = (float)z / (float)wRes - 1;
			i++;
		}
	
	}
	int count = 0;
	for (int iz = 0; iz < lRes-1; iz++) {
		for (int ix = 0; ix < wRes-1; ix++) {
			int topLeft = (iz*lRes) + ix;
			int topRight = (topLeft + 1);
			int bottomLeft = ((iz + 1)*lRes) + ix;
			int bottomRight = bottomLeft + 1;
			/*indices.push_back(topLeft);
			indices.push_back(bottomLeft);
			indices.push_back(topRight);
			indices.push_back(topRight);
			indices.push_back(bottomLeft);
			indices.push_back(bottomRight);*/
			index[count++] = topLeft;
			index[count++] = bottomLeft;
			index[count++] = topRight;
			index[count++] = topRight;
			index[count++] = bottomLeft;
			index[count++] = bottomRight;
			
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
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture0);
	glUniform1i(glGetUniformLocation(shaderProgram, "myTextureSampler"), 0);
	//glDrawArrays(GL_TRIANGLES, 0, sizeof(*tVertices));
	// Tell OpenGL to draw with triangles, using indices.size() indices, the type of the indices, and the offset to start from
	glDrawElements(GL_TRIANGLES, 6 * (lRes - 1)*(wRes - 1), GL_UNSIGNED_INT, 0);
	// Unbind the VAO when we're done so we don't accidentally draw extra stuff or tamper with its bound buffers
	glBindVertexArray(0);




}
unsigned char* Terrain::loadPPM(const char* filename, int& width, int& height)
{
	const int BUFSIZE = 128;
	FILE* fp;
	unsigned int read;
	unsigned char* rawData;
	char buf[3][BUFSIZE];
	char* retval_fgets;
	size_t retval_sscanf;

	if ((fp = fopen(filename, "rb")) == NULL)
	{
		std::cerr << "error reading ppm file, could not locate " << filename << std::endl;
		width = 0;
		height = 0;
		return NULL;
	}

	// Read magic number:
	retval_fgets = fgets(buf[0], BUFSIZE, fp);

	// Read width and height:
	do
	{
		retval_fgets = fgets(buf[0], BUFSIZE, fp);
	} while (buf[0][0] == '#');
	retval_sscanf = sscanf(buf[0], "%s %s", buf[1], buf[2]);
	width = atoi(buf[1]);
	height = atoi(buf[2]);

	// Read maxval:
	do
	{
		retval_fgets = fgets(buf[0], BUFSIZE, fp);
	} while (buf[0][0] == '#');

	// Read image data:
	rawData = new unsigned char[width * height * 3];
	read = fread(rawData, width * height * 3, 1, fp);
	fclose(fp);
	if (read != 1)
	{
		std::cerr << "error parsing ppm file, incomplete data" << std::endl;
		delete[] rawData;
		width = 0;
		height = 0;
		return NULL;
	}

	return rawData;
}

// load image file into texture object
void Terrain::loadTexture(unsigned char* tdata)
{



	if (tdata == NULL) return;

	// Create ID for texture
	/*glGenTextures(1, &terrainTexture[0]);
	glActiveTexture(terrainTexture[0]);
	// Set this texture to be the one we are working with
	glBindTexture(GL_TEXTURE_2D, terrainTexture[0]);*/

	// Generate the texture
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, twidth, theight, 0, GL_RGB, GL_UNSIGNED_BYTE, tdata);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// Set bi-linear filtering for both minification and magnification
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

}
GLuint Terrain::loadTexture2(const char *textureFile) {
	//unsigned int textureID;
	glGenTextures(1, &texture0);

	glBindTexture(GL_TEXTURE_2D, texture0);

	int width, height, nrChannels;
	unsigned char *data = stbi_load(textureFile, &width, &height, &nrChannels, 0);
	if (data) glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_BGR, GL_UNSIGNED_BYTE, data);
	else std::cout << "Image failed to load at path: " << textureFile << std::endl;
	stbi_image_free(data);

	glGenerateMipmap(GL_TEXTURE_2D);  // Generate mipmaps
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

	return texture0;
}

void Terrain::translate(glm::vec3 transVec)
{
	toWorld = glm::translate(glm::mat4(1.0f), transVec) * toWorld;
}