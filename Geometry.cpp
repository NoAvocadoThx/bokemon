#include "Geometry.h"
#include "Window.h"
#include <iostream>
#include <fstream>
#include <cmath>
#include <ctime>    // For time()
#include <cstdlib>  // For srand() and rand()

//GLuint texture[1];     // storage for one texture

glm::mat4 scaleMat=glm::mat4(1.0f);

Geometry::Geometry(const char* filepath)
{
	currObj = new OBJObject(filepath);
}
Geometry::~Geometry() {
	// Delete previously generated buffers. Note that forgetting to do this can waste GPU memory in a 
// large project! This could crash the graphics driver due to memory leaks, or slow down application performance!
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	glDeleteBuffers(1, &Norms);
}


/** Load a ppm file from disk.
 @input filename The location of the PPM file.  If the file is not found, an error message
		will be printed and this function will return 0
 @input width This will be modified to contain the width of the loaded image, or 0 if file not found
 @input height This will be modified to contain the height of the loaded image, or 0 if file not found
 @return Returns the RGB pixel data as interleaved unsigned chars (R0 G0 B0 R1 G1 B1 R2 G2 B2 .... etc) or 0 if an error ocured
**/
unsigned char* Geometry::loadPPM(const char* filename, int& width, int& height)
{
	const int BUFSIZE = 256;
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
/*void Geometry::loadTexture(unsigned char* tdata)
{
	


	if (tdata == NULL) return;

	// Create ID for texture
	glGenTextures(1, &texture[0]);

	// Set this texture to be the one we are working with
	glBindTexture(GL_TEXTURE_2D, texture[0]);

	// Generate the texture
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, twidth, theight, 0, GL_RGB, GL_UNSIGNED_BYTE, tdata);

	// Set bi-linear filtering for both minification and magnification
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}*/
void Geometry::draw(GLuint shaderProgram, glm::mat4 m)
{
	m = m * scaleMat;
	float midY = (currObj->toWorld[3][1] - currObj->toWorld[0][1]);
	if (isSphere&&Window::toggleSphere) {
		currObj->drawFrame(shaderProgram,  m * glm::scale(glm::mat4(1.0f), glm::vec3(25.0f)));
	}
	currObj->draw(shaderProgram, m );


}
void Geometry::update(){}
void Geometry::scale(float val) {
scaleMat =scaleMat* glm::scale(glm::mat4(1.0f), glm::vec3(val));
}
bool Geometry::inSphere(glm::vec3 origin, GLfloat radius, Plane pl[]) { return false; }

glm::vec4 Geometry::getOrigin(glm::mat4 M) {
	glm::vec4 realOrigin = Window::P*Window::V*M*glm::vec4(origin, 1.0f);
	return realOrigin;
}