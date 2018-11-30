#include "OBJObject.h"
#include "Window.h"
#include <iostream>
#include <fstream>
#include <cmath>
#include <ctime>    // For time()
#include <cstdlib>  // For srand() and rand()

GLfloat min_x, max_x, min_y, max_y, min_z, max_z;
glm::vec3 centerP;

GLfloat r;
GLfloat randX,randY,randZ;
//1 for original color, 2 for new color
GLint colorMod = 1;
GLfloat radian;
GLfloat scaleUp=1.3f;
GLfloat scaleDown=0.7f;
GLuint texture[1];
unsigned char sphereData;

OBJObject::OBJObject(const char *filepath) 
{
	toWorld = glm::mat4(1.0f);
	
	parse(filepath);
	center();
	faces = {
	  "../right.ppm",
	  "../left.ppm",
	  "../up.ppm",
	  "../down.ppm",
	  "../back.ppm",
	  "../front.ppm"
	};
	//std::string str("../image4.ppm");
	sphereData = loadCubemap(faces);
	std::string str("../image4.ppm");
	unsigned char* tdata = loadPPM(str.c_str(), twidth, theight);
	loadTexture(tdata);
	if (ifDirSource || ifPtSource || ifSptSource) {
		toWorld = toWorld * glm::translate(glm::mat4(1.0f), ptPos);
		toWorld = toWorld * glm::scale(glm::mat4(1.0f), glm::vec3(10.0f));
	}
	// Create array object and buffers. Remember to delete your buffers when the object is destroyed!
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	glGenBuffers(1, &Norms);
	glGenBuffers(1, &TO);

	// Bind the Vertex Array Object (VAO) first, then bind the associated buffers to it.
	// Consider the VAO as a container for all your buffers.
	glBindVertexArray(VAO);

	// Now bind a VBO to it as a GL_ARRAY_BUFFER. The GL_ARRAY_BUFFER is an array containing relevant data to what
	// you want to draw, such as vertices, normals, colors, etc.
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	// glBufferData populates the most recently bound buffer with data starting at the 3rd argument and ending after
	// the 2nd argument number of indices. How does OpenGL know how long an index spans? Go to glVertexAttribPointer.
	glBufferData(GL_ARRAY_BUFFER, vertices.size()*sizeof(glm::vec3), &vertices[0], GL_STATIC_DRAW);
	// Enable the usage of layout location 0 (check the vertex shader to see what this is)
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0,// This first parameter x should be the same as the number passed into the line "layout (location = x)" in the vertex shader. In this case, it's 0. Valid values are 0 to GL_MAX_UNIFORM_LOCATIONS.
		3, // This second line tells us how any components there are per vertex. In this case, it's 3 (we have an x, y, and z component)
		GL_FLOAT, // What type these components are
		GL_FALSE, // GL_TRUE means the values should be normalized. GL_FALSE means they shouldn't
		3 * sizeof(GLfloat), // Offset between consecutive indices. Since each of our vertices have 3 floats, they should have the size of 3 floats in between
		(GLvoid*)0); // Offset of the first vertex's component. In our case it's 0 since we don't pad the vertices array with anything.

	

	//glBindBuffer(GL_ARRAY_BUFFER, Norms); //Normal Buffer Object
	//glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(glm::vec3), &normals[0], GL_STATIC_DRAW);

	glBindVertexArray(Norms);
	glBindBuffer(GL_ARRAY_BUFFER, Norms);
	glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(glm::vec3), &normals[0], GL_STATIC_DRAW);
	

	//glBindBuffer(GL_ARRAY_BUFFER, TO);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2)*a_uv.size(), &a_uv[0], GL_STATIC_DRAW);
	
	
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	// We've sent the vertex data over to OpenGL, but there's still something missing.
	// In what order should it draw those vertices? That's why we'll need a GL_ELEMENT_ARRAY_BUFFER for this.
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, TO);
	//glBufferData(GL_ELEMENT_ARRAY_BUFFER, a_uv.size() * sizeof(glm::vec2), &a_uv[0], GL_STATIC_DRAW);
	//texture cpprd attribute


    // Unbind the currently bound buffer so that we don't accidentally make unwanted changes to it.
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	// Unbind the VAO now so we don't accidentally tamper with it.
	// NOTE: You must NEVER unbind the element array buffer associated with a VAO!
	glBindVertexArray(0);
	
}

OBJObject::~OBJObject()
{
	// Delete previously generated buffers. Note that forgetting to do this can waste GPU memory in a 
	// large project! This could crash the graphics driver due to memory leaks, or slow down application performance!
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	glDeleteBuffers(1, &Norms);
	glDeleteBuffers(1, &TO);
}
/*void OBJObject::parse(const char *filepath) 
{
	FILE* fp;
	GLfloat x, y, z;
	GLint f_x, f_y, f_z;
	GLint f_nx, f_ny, f_nz;
	GLint uv_x, uv_y;
	unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
	std::string temps;
	GLfloat r, g, b;  // vertex color
	GLint c1, c2;    // characters read from file
	fp = fopen(filepath, "rb");  // make the file name configurable so you can load other files
	if (fp == NULL) { std::cerr << "error loading file" << std::endl; exit(-1); }  // just in case the file can't be found or is corrupt
																		 //int i = 0;
	while (1) {

		c1 = fgetc(fp);
		if (c1 == EOF) {
			break;
		}

		if (c1 == 'v') {
			c2 = fgetc(fp);
			if (c2 == ' ')
			{
				fscanf(fp, "%f %f %f", &x, &y, &z);
				//populate vertices with the OBJ Object data
				vertices.push_back(glm::vec3(x, y, z));

			}
			// read normal data accordingly
			else if ( (c2 == 'n'))
			{

				fscanf(fp, "%f %f %f", &x, &y, &z);

				//populate normal vectors with the OBJ Object data
				normals.push_back(glm::vec3(x, y, z));

			}
			else if ((c2 == 't')) {

				fscanf(fp, "%f %f", &uv_x, &uv_y);
				tex.push_back(glm::vec2(uv_x, uv_y));
			}

		}
			else if ((c1 == 'f') || (c2 == 'f'))
			{

				fscanf(fp, "%d/%d/%d %d/%d/%d %d/%d/%d\n",
					&vertexIndex[0], &uvIndex[0], &normalIndex[0],
					&vertexIndex[1], &uvIndex[1], &normalIndex[1],
					&vertexIndex[2], &uvIndex[2], &normalIndex[2]);
				//push them to their indices accordingly
				vertexIndices.push_back(vertexIndex[0] - 1);
				vertexIndices.push_back(vertexIndex[1] - 1);
				vertexIndices.push_back(vertexIndex[2] - 1);
				uvIndices.push_back(uvIndex[0] - 1);
				uvIndices.push_back(uvIndex[1] - 1);
				uvIndices.push_back(uvIndex[2] - 1);
				normalIndices.push_back(normalIndex[0] - 1);
				normalIndices.push_back(normalIndex[1] - 1);
				normalIndices.push_back(normalIndex[2] - 1);
			}
		
		

	}

	//reordering data
	for (unsigned i = 0; i < vertexIndices.size(); i++) {
		a_vertices.push_back(vertices[vertexIndices[i]]);
		a_normals.push_back(normals[normalIndices[i]]);
		a_uv.push_back(tex[uvIndices[i]]);
		indices.push_back(i);
	}



	fclose(fp);   // make sure you don't forget to close the file when done

}*/
void OBJObject::parse(const char *filepath)
{
	// parse the OBJ file
	FILE* fp;     // file pointer
	GLfloat r, g, b;  // vertex color
	GLfloat x, y, z;
	GLint f_x, f_y, f_z;
	GLint f_nx, f_ny, f_nz;
	GLint c1, c2;    // characters read from file


	fp = fopen(filepath, "rb");  // make the file name configurable so you can load other files
	if (fp == NULL) { std::cerr << "error loading file" << std::endl; exit(-1); }  // just in case the file can't be found or is corrupt

	while (1) {

		c1 = fgetc(fp);
		c2 = fgetc(fp);
		if (c1 == EOF) {
			break;
		}
		if ((c1 == 'v') && (c2 == ' '))
		{

			fscanf(fp, "%f %f %f %f %f %f", &x, &y, &z, &r, &g, &b);
			//populate vertices with the OBJ Object data
			vertices.push_back(glm::vec3(x, y, z));


		}
		// read normal data accordingly
		else if ((c1 == 'v') && (c2 == 'n'))
		{

			fscanf(fp, "%f %f %f", &x, &y, &z);

			//populate normal vectors with the OBJ Object data
			normals.push_back(glm::vec3(x, y, z));

		}
		else if ((c1 == 'f') || (c2 == 'f'))
		{
			fscanf(fp, "%d//%d %d//%d %d//%d", &f_x, &f_nx, &f_y, &f_ny, &f_z, &f_nz);
			//check number before and after "//"
			//if (f_x == f_nx && f_y == f_ny && f_z == f_nz) {
				//push 3 corners to indices
			indices.push_back(f_x - 1);
			indices.push_back(f_y - 1);
			indices.push_back(f_z - 1);
			//}
		}
		fscanf(fp, "\n");

	}

	// Populate the face indices, vertices, and normals vectors with the OBJ Object data
	fclose(fp);



}

void OBJObject::draw(GLuint shaderProgram)
{


	// Calculate the combination of the model and view (camera inverse) matrices
	glm::mat4 modelview = Window::V * toWorld;
	
	glm::mat4 translationMat = glm::translate(glm::mat4(1.0f), position);
	// We need to calcullate this because modern OpenGL does not keep track of any matrix other than the viewport (D)
	// Consequently, we need to forward the projection, view, and model matrices to the shader programs
	// Get the location of the uniform variables "projection" and "modelview"
	uProjection = glGetUniformLocation(shaderProgram, "projection");
	uModelview = glGetUniformLocation(shaderProgram, "modelview");
	// Now send these values to the shader program
	glUniformMatrix4fv(uProjection, 1, GL_FALSE, &Window::P[0][0]);
	glUniformMatrix4fv(uModelview, 1, GL_FALSE, &modelview[0][0]);
	
	glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, &toWorld[0][0]);

	

	// Now draw the cube. We simply need to bind the VAO associated with it.
	glBindVertexArray(VAO);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, sphereData);
	glDrawArrays(GL_TRIANGLES, 0, vertices.size());
	// Tell OpenGL to draw with triangles, using indices.size() indices, the type of the indices, and the offset to start from
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
	// Unbind the VAO when we're done so we don't accidentally draw extra stuff or tamper with its bound buffers
	glBindVertexArray(0);
	

	
	
}

void OBJObject::drawFrame(GLuint shaderProgram, glm::mat4 C)
{


	glm::mat4 modelview = Window::V * C; 

		// We need to calcullate this because modern OpenGL does not keep track of any matrix other than the viewport (D)
	// Consequently, we need to forward the projection, view, and model matrices to the shader programs
	// Get the location of the uniform variables "projection" and "modelview"
	uProjection = glGetUniformLocation(shaderProgram, "projection");
	uModelview = glGetUniformLocation(shaderProgram, "modelview");

	// Now send these values to the shader program
	glUniformMatrix4fv(uProjection, 1, GL_FALSE, &Window::P[0][0]);
	glUniformMatrix4fv(uModelview, 1, GL_FALSE, &modelview[0][0]);
	//toWorld
	glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, &toWorld[0][0]);

	// Now draw the cube. We simply need to bind the VAO associated with it.
	glBindVertexArray(VAO);
	// Tell OpenGL to draw with triangles, using 36 indices, the type of the indices, and the offset to start from
	glDrawElements(GL_LINES, (GLsizei)indices.size() * 3, GL_UNSIGNED_INT, 0);
	// Unbind the VAO when we're done so we don't accidentally draw extra stuff or tamper with its bound buffers
	glBindVertexArray(0);




}
void OBJObject::update()
{
	//spin(1.0f);
}

void OBJObject::spin(float deg)
{
	this->angle += deg;
	if (this->angle > 360.0f || this->angle < -360.0f) this->angle = 0.0f;
	// This creates the matrix to rotate the object
	this->toWorld = glm::rotate(glm::mat4(1.0f), this->angle / 180.0f * glm::pi<float>(), glm::vec3(0.0f, 1.0f, 0.0f));
	//centerP.x,centerP.y,centerP.z
}
void OBJObject::colorChange(GLint m) {
	
	colorMod = m;
}

void OBJObject::center() {
	min_x = vertices[0].x;
	max_x = vertices[0].x;
	min_y = vertices[0].y;
	max_y = vertices[0].y;
	min_z = vertices[0].z;
	max_z = vertices[0].z;
	for (int i = 0; i < vertices.size(); i++) {

		//find min max x
		if (min_x > vertices[i].x) {
			min_x = vertices[i].x;
		}
		if (max_x < vertices[i].x) {
			max_x = vertices[i].x;
		}
		//find min max y
		if (min_y > vertices[i].y) {
			min_y = vertices[i].y;
		}
		if (max_y < vertices[i].y) {
			max_y = vertices[i].y;
		}
		//find min max z
		if (min_z > vertices[i].z) {
			min_z = vertices[i].z;
		}
		if (max_z < vertices[i].z) {
			max_z = vertices[i].z;
		}

	}
	//calculate center of the object
	centerP.x = (min_x + max_x) / 2.0f;
	centerP.y = (min_y + max_y) / 2.0f;
	centerP.z = (min_z + max_z) / 2.0f;
	for (int i = 0; i < vertices.size(); i++) {
		vertices[i].x -= centerP.x;
		vertices[i].y -= centerP.y;
		vertices[i].z -= centerP.z;
	}
}
//return a random float between a and b
GLfloat randomFloat(GLfloat a, GLfloat b) {
	GLfloat random = ((GLfloat)rand()) / (GLfloat)RAND_MAX;
	GLfloat diff = b - a;
	GLfloat r = random * diff;
	return a + r;
}

void OBJObject::scale(bool ifScale) {
	if (ifScale) {
		this->toWorld = toWorld * glm::scale(glm::mat4(1.0f), glm::vec3(scaleUp,scaleUp,scaleUp));
	}
	else if (!ifScale) {
		this->toWorld = toWorld * glm::scale(glm::mat4(1.0f), glm::vec3(scaleDown,scaleDown,scaleDown));
	}

}

void OBJObject::translate(glm::vec3 transVec)
{
	toWorld = glm::translate(glm::mat4(1.0f), transVec) * toWorld;
}
void OBJObject::scaleSize(GLfloat scaleV) {

    toWorld = toWorld * glm::scale(glm::mat4(1.0f), glm::vec3(scaleV, scaleV, scaleV));
}

unsigned char* OBJObject::loadPPM(const char* filename, int& width, int& height)
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
void OBJObject::loadTexture(unsigned char* tdata)
{



	if (tdata == NULL) return;

	// Create ID for texture
	glGenTextures(1, &texture[0]);
	glActiveTexture(texture[0]);
	// Set this texture to be the one we are working with
	glBindTexture(GL_TEXTURE_2D, texture[0]);

	// Generate the texture
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, twidth, theight, 0, GL_RGB, GL_UNSIGNED_BYTE, tdata);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// Set bi-linear filtering for both minification and magnification
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	
}
void OBJObject::draw(GLuint shaderProgram, glm::mat4 C)
{
	
	glm::mat4 modelview = Window::V * C; 

		// We need to calcullate this because modern OpenGL does not keep track of any matrix other than the viewport (D)
	// Consequently, we need to forward the projection, view, and model matrices to the shader programs
	// Get the location of the uniform variables "projection" and "modelview"
	uProjection = glGetUniformLocation(shaderProgram, "projection");
	uModelview = glGetUniformLocation(shaderProgram, "modelview");

	// Now send these values to the shader program
	glUniformMatrix4fv(uProjection, 1, GL_FALSE, &Window::P[0][0]);
	glUniformMatrix4fv(uModelview, 1, GL_FALSE, &modelview[0][0]);
	//toWorld
	glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, &toWorld[0][0]);

	// Now draw the cube. We simply need to bind the VAO associated with it.
	glBindVertexArray(VAO);
	// Tell OpenGL to draw with triangles, using 36 indices, the type of the indices, and the offset to start from
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
	
	// Unbind the VAO when we're done so we don't accidentally draw extra stuff or tamper with its bound buffers
	glBindVertexArray(0);
}
unsigned int OBJObject::loadCubemap(std::vector<std::string> faces)
{
	unsigned int textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

	int width, height, nrChannels;
	for (unsigned int i = 0; i < faces.size(); i++)
	{
		unsigned char *data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
		//        unsigned char *data = loadPPM(faces[i].c_str(), width, height);
		if (data)
		{
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
				0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data
			);
			stbi_image_free(data);
		}
		else
		{
			//std::cout << "Cubemap texture failed to load at path: " << faces[i] << std::endl;
			stbi_image_free(data);
		}
	}

	// Make sure no bytes are padded:
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	//glEnable(GL_CULL_FACE);
	//glCullFace(GL_BACK);
	// Use bilinear interpolation:
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	return textureID;
}