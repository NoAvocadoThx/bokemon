//
//  ROBObject.cpp
//  Project2
//
//  Created by Shengxiang Zhou on 10/30/18.
//  Copyright © 2018 Shengxiang Zhou. All rights reserved.
//

#include "ROBObject.h"
#include "Window.h"
#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>
#include "Terrain.h"
using namespace std;
#define TEXTURE_PATH "./leather.PPM"
ROBObject::ROBObject() {}
ROBObject::ROBObject(const char *filepath)
{

	toWorld = glm::mat4(1.0f);
	//toWorld *= glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 45.0f, 0.0f));
	parse(filepath);
	scaleProcess();
	//oadTexture();
	scale_size = 1;
	rotAngle = 0;

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &VBO2);
	glGenBuffers(1, &VBO3);
	glGenBuffers(1, &EBO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0,// This first parameter x should be the same as the number passed into the line "layout (location = x)" in the vertex shader. In this case, it's 0. Valid values are 0 to GL_MAX_UNIFORM_LOCATIONS.
		3, // This second line tells us how any components there are per vertex. In this case, it's 3 (we have an x, y, and z component)
		GL_FLOAT, // What type these components are
		GL_FALSE, // GL_TRUE means the values should be normalized. GL_FALSE means they shouldn't
		3 * sizeof(GLfloat), // Offset between consecutive indices. Since each of our vertices have 3 floats, they should have the size of 3 floats in between
		(GLvoid*)0); // Offset of the first vertex's component. In our case it's 0 since we don't pad the vertices array with anything.
	glBindBuffer(GL_ARRAY_BUFFER, VBO2);
	glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(glm::vec3), &normals[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_TRUE, 3 * sizeof(GLfloat), (GLvoid*)0);

	glBindBuffer(GL_ARRAY_BUFFER, VBO3);
	glBufferData(GL_ARRAY_BUFFER, textures.size() * sizeof(glm::vec2), &textures[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_TRUE, 2 * sizeof(GLfloat), (GLvoid*)0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);


	// Unbind the currently bound buffer so that we don't accidentally make unwanted changes to it.
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	// Unbind the VAO now so we don't accidentally tamper with it.
	// NOTE: You must NEVER unbind the element array buffer associated with a VAO!
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 1);
	glBindVertexArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, 2);
	glBindVertexArray(2);






}

void ROBObject::parse(const char *filepath)
{

	// Populate the face indices, vertices, and normals vectors with the OBJ Object data
	FILE* fp;     // file pointer
	float x, y, z;  // vertex coordinates
	int c1, c2;    // characters read from file
	fp = fopen(filepath, "rb");  // make the file name configurable so you can load other files
	if (fp == NULL) { cerr << "error loading file" << endl; exit(-1); }  // just in case the file can't be found or is corrupt

	while (1) {
		c1 = fgetc(fp);
		c2 = fgetc(fp);
		if (c1 == EOF) {
			break;
		}

		if ((c1 == 'v') && (c2 == ' '))
		{
			glm::vec3 vertex;
			fscanf(fp, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z);
			temp_vertices.push_back(vertex);
		}
		if ((c1 == 'v') && (c2 == 'n'))
		{
			fscanf(fp, "%f %f %f\n", &x, &y, &z);
			temp_normals.push_back(glm::vec3(x, y, z));
		}
		if (!isSphere) {
			if ((c1 == 'v') && (c2 == 't'))
			{
				fscanf(fp, "%f %f\n", &x, &y);
				temp_textures.push_back(glm::vec2(x, y));
			}
		}
		if ((c1 == 'f') && (c2 == ' '))
		{
			unsigned int vertexIndex[3], normalIndex[3], textureIndex[3];
			fscanf(fp, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &vertexIndex[0], &textureIndex[0], &normalIndex[0], &vertexIndex[1], &textureIndex[1], &normalIndex[1], &vertexIndex[2], &textureIndex[2], &normalIndex[2]);
			vindices.push_back(vertexIndex[0] - 1);
			vindices.push_back(vertexIndex[1] - 1);
			vindices.push_back(vertexIndex[2] - 1);
			tindices.push_back(textureIndex[0] - 1);
			tindices.push_back(textureIndex[1] - 1);
			tindices.push_back(textureIndex[2] - 1);
			nindices.push_back(normalIndex[0] - 1);
			nindices.push_back(normalIndex[1] - 1);
			nindices.push_back(normalIndex[2] - 1);
		}
		fscanf(fp, "\n");

	}
	for (unsigned i = 0; i < vindices.size(); i++) {
		vertices.push_back(temp_vertices[vindices[i]]);
		normals.push_back(temp_normals[nindices[i]]);
		textures.push_back(temp_textures[tindices[i]]);
		indices.push_back(i);
	}
	fclose(fp);   // make sure you don't forget to close the file when done

}
ROBObject::~ROBObject() {
	// Delete previously generated buffers. Note that forgetting to do this can waste GPU memory in a
	// large project! This could crash the graphics driver due to memory leaks, or slow down application performance!
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &VBO2);
	glDeleteBuffers(1, &VBO3);
	glDeleteBuffers(1, &EBO);
}




void ROBObject::scaleProcess()
{
	maxx = -INFINITY;
	maxy = -INFINITY;
	maxz = -INFINITY;
	minx = INFINITY;
	miny = INFINITY;
	minz = INFINITY;
	//scalevalue = 3.0f;

	for (unsigned int i = 0; i < vertices.size(); ++i)
	{
		//find maxx,maxy,maxz,minx,miny,minz
		if (vertices[i].x > maxx)
		{
			maxx = vertices[i].x;
		}
		if (vertices[i].y > maxy)
		{
			maxy = vertices[i].y;
		}
		if (vertices[i].z > maxz)
		{
			maxz = vertices[i].z;
		}
		if (vertices[i].x < minx)
		{
			minx = vertices[i].x;
		}
		if (vertices[i].y < miny)
		{
			miny = vertices[i].y;
		}
		if (vertices[i].z < minz)
		{
			minz = vertices[i].z;
		}
	}
	/*
	if (maxx > maxy) {
		//x>y
		if (maxx > maxz) {
			//x>z
			scalemax = scalevalue / maxx;
		}
		else {
			//z>x
			scalemax = scalevalue / maxz;
		}
	}
	else {
		//y>x
		if (maxy > maxz) {
			//y>z
			scalemax = scalevalue / maxy;
		}
		else {
			//z>y
			scalemax = scalevalue / maxz;
		}
	}

	if (minx > miny) {
		//x>y
		if (miny > minz) {
			//y>z
			scalemin = scalevalue / minz;
		}
		else {
			//y<z
			scalemin = scalevalue / miny;
		}
	}
	else {
		//x<y
		if (minx > minz) {
			//x>z
			scalemin = scalevalue / minz;
		}
		else {
			//x<z
			scalemin = scalevalue / minx;
		}
	}
	if (abs(scalemax) > abs(scalemin)) {
		scale = abs(scalemax);
	}
	else {
		scale = abs(scalemin);
	}
	*/
	this->centerx = (maxx + minx) / 2;
	this->centery = (maxy + miny) / 2;
	this->centerz = (maxz + minz) / 2;
	glm::vec3 center = glm::vec3(centerx, centery, centerz);
	for (unsigned int i = 0; i < vertices.size(); i++) {

		vertices[i].x = (vertices[i].x - this->centerx);
		vertices[i].y = (vertices[i].y - this->centery);
		vertices[i].z = (vertices[i].z - this->centerz);

	}
	float dimX, dimY, dimZ;
	dimX = maxx - minx;
	dimY = maxy - miny;
	dimZ = maxz - minz;

	// scale all vertices using the largest dimension, uniform scale
	if (dimX >= dimY && dimX >= dimZ) {
		for (int i = 0; i < vertices.size(); i++) {
			vertices[i] = glm::vec3(vertices[i].x / dimX, vertices[i].y / dimX, vertices[i].z / dimX);
		}
	}
	else if (dimY >= dimX && dimY >= dimZ) {
		for (int i = 0; i < vertices.size(); i++) {
			vertices[i] = glm::vec3(vertices[i].x / dimY, vertices[i].y / dimY, vertices[i].z / dimY);
		}
	}
	else if (dimZ >= dimX && dimZ >= dimY) {
		for (int i = 0; i < vertices.size(); i++) {
			vertices[i] = glm::vec3(vertices[i].x / dimZ, vertices[i].y / dimZ, vertices[i].z / dimZ);
		}
	}


	minX = INFINITY, minY = INFINITY, minZ = INFINITY;   // min vertex
	maxX = -INFINITY, maxY = -INFINITY, maxZ = -INFINITY;   // max vertex

	glm::vec3 minVertX, minVertY, minVertZ;
	glm::vec3 maxVertX, maxVertY, maxVertZ;

	// search for bounding box dimension
	for (int i = 0; i < vertices.size(); i++) {
		if (vertices[i].x < minX) {
			minX = vertices[i].x;
			minVertX = vertices[i];
		}
		if (vertices[i].y < minY) {
			minY = vertices[i].y;
			minVertY = vertices[i];
		}
		if (vertices[i].z < minZ) {
			minZ = vertices[i].z;
			minVertZ = vertices[i];
		}
		if (vertices[i].x > maxX) {
			maxX = vertices[i].x;
			maxVertX = vertices[i];
		}
		if (vertices[i].y > maxY) {
			maxY = vertices[i].y;
			maxVertY = vertices[i];
		}
		if (vertices[i].z > maxZ) {
			maxZ = vertices[i].z;
			maxVertZ = vertices[i];
		}
	}

	boxVertices.push_back(maxVertX);
	boxVertices.push_back(minVertX);
	boxVertices.push_back(maxVertY);
	boxVertices.push_back(minVertY);
	boxVertices.push_back(maxVertZ);
	boxVertices.push_back(minVertZ);

	// bounding box
	boundingbox.push_back(minX);
	boundingbox.push_back(minY);
	boundingbox.push_back(maxZ);


	boundingbox.push_back(maxX);
	boundingbox.push_back(minY);
	boundingbox.push_back(maxZ);


	boundingbox.push_back(maxX);
	boundingbox.push_back(maxY);
	boundingbox.push_back(maxZ);


	boundingbox.push_back(minX);
	boundingbox.push_back(maxY);
	boundingbox.push_back(maxZ);


	boundingbox.push_back(minX);
	boundingbox.push_back(minY);
	boundingbox.push_back(maxZ);

	boundingbox.push_back(minX);
	boundingbox.push_back(minY);
	boundingbox.push_back(minZ);


	boundingbox.push_back(maxX);
	boundingbox.push_back(minY);
	boundingbox.push_back(minZ);


	boundingbox.push_back(maxX);
	boundingbox.push_back(maxY);
	boundingbox.push_back(minZ);


	boundingbox.push_back(minX);
	boundingbox.push_back(maxY);
	boundingbox.push_back(minZ);


	boundingbox.push_back(minX);
	boundingbox.push_back(minY);
	boundingbox.push_back(minZ);

	boundingbox.push_back(minX);
	boundingbox.push_back(maxY);
	boundingbox.push_back(maxZ);
	boundingbox.push_back(minX);
	boundingbox.push_back(maxY);
	boundingbox.push_back(minZ);

	boundingbox.push_back(maxX);
	boundingbox.push_back(minY);
	boundingbox.push_back(maxZ);
	boundingbox.push_back(maxX);
	boundingbox.push_back(minY);
	boundingbox.push_back(minZ);

	boundingbox.push_back(maxX);
	boundingbox.push_back(maxY);
	boundingbox.push_back(maxZ);
	boundingbox.push_back(maxX);
	boundingbox.push_back(maxY);
	boundingbox.push_back(minZ);

}
unsigned char*  ROBObject::loadPPM(const char* filename, int& width, int& height)
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
void  ROBObject::loadTexture()
{

	int twidth, theight;   // texture width/height [pixels]
	unsigned char* tdata;  // texture pixel data
	twidth = 512;
	theight = 512;
	tdata = loadPPM(TEXTURE_PATH, twidth, theight);
	if (tdata == NULL) return;

	// Create ID for texture
	glGenTextures(1, &textureID);

	// Set this texture to be the one we are working with
	glBindTexture(GL_TEXTURE_2D, textureID);

	// Generate the texture
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, twidth, theight, 0, GL_RGB, GL_UNSIGNED_BYTE, tdata);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
	// Set bi-linear filtering for both minification and magnification
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);


}

void ROBObject::draw(GLuint shaderProgram, glm::mat4 C)
{

	// Calculate the combination of the model and view (camera inverse) matrices
	glm::mat4 modelview = Window::V * C; // before it was C

	//    modelview = modelview * glm::mat4(1.0f);

	uProjection = glGetUniformLocation(shaderProgram, "projection");
	uModelview = glGetUniformLocation(shaderProgram, "modelview");

	// Now send these values to the shader program
	glUniformMatrix4fv(uProjection, 1, GL_FALSE, &Window::P[0][0]);
	glUniformMatrix4fv(uModelview, 1, GL_FALSE, &modelview[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, &toWorld[0][0]);
	// Now draw the cube. We simply need to bind the VAO associated with it.
	glBindVertexArray(VAO);
	// Tell OpenGL to draw with triangles, using 36 indices, the type of the indices, and the offset to start from
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
	// Unbind the VAO when we're done so we don't accidentally draw extra stuff or tamper with its bound buffers
	glBindVertexArray(0);
}

void ROBObject::drawLines(GLuint shaderProgram, glm::mat4 C)
{
	// Calculate the combination of the model and view (camera inverse) matrices
	glm::mat4 modelview = Window::V * C; // before it was C

	uProjection = glGetUniformLocation(shaderProgram, "projection");
	uModelview = glGetUniformLocation(shaderProgram, "modelview");

	// Now send these values to the shader program
	glUniformMatrix4fv(uProjection, 1, GL_FALSE, &Window::P[0][0]);
	glUniformMatrix4fv(uModelview, 1, GL_FALSE, &modelview[0][0]);

	// Now draw the cube. We simply need to bind the VAO associated with it.
	glBindVertexArray(VAO);
	// Tell OpenGL to draw with triangles, using 36 indices, the type of the indices, and the offset to start from
	   //glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);

	glDrawElements(GL_LINES, (GLsizei)indices.size() * 3, GL_UNSIGNED_INT, 0);

	// Unbind the VAO when we're done so we don't accidentally draw extra stuff or tamper with its bound buffers
	glBindVertexArray(0);
}

void ROBObject::translate(glm::vec3 transVec)
{
	toWorld = glm::translate(glm::mat4(1.0f), transVec) * toWorld;
}
void ROBObject::setPosition(glm::vec3 newPos) {
	this->position = newPos;
}

void ROBObject::draw(GLuint shaderProgram)
{


	glm::mat4 modelview = Window::V * toWorld;


	uProjection = glGetUniformLocation(shaderProgram, "projection");
	uModelview = glGetUniformLocation(shaderProgram, "modelview");
	// Now send these values to the shader program
	glUniformMatrix4fv(uProjection, 1, GL_FALSE, &Window::P[0][0]);
	glUniformMatrix4fv(uModelview, 1, GL_FALSE, &modelview[0][0]);
	// Now draw the cube. We simply need to bind the VAO associated with it.
	glBindVertexArray(VAO);
	// Tell OpenGL to draw with triangles, using indices.size() indices, the type of the indices, and the offset to start from
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
	// Unbind the VAO when we're done so we don't accidentally draw extra stuff or tamper with its bound buffers
	glBindVertexArray(0);

}
void ROBObject::translateX(float xVal) {

	glm::mat4 translateMat = glm::translate(glm::mat4(1.0f), glm::vec3(xVal, 0.0f, 0.0f)); //
	toWorld = translateMat * toWorld;

}
void ROBObject::translateY(float yVal) {
	glm::mat4 translateMat = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, yVal, 0.0f)); //
	toWorld = translateMat * toWorld;
}
void ROBObject::translateZ(float zVal) {
	glm::mat4 translateMat = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, zVal)); //
	toWorld = translateMat * toWorld;
}

void ROBObject::walk() {
	int count = 0;
	toWorld = toWorld * glm::rotate(glm::mat4(1.0f), 1.0f / 180.0f * glm::pi<float>(), glm::vec3(0.0f, 1.0f, 0.0f));
	srand((unsigned)time(0));
	float i,j,w,v;
	i = (rand() % 3) + 1;
	j = (rand() % 3) + 1;
	w = (rand() % 3) + 1;
	v = (rand() % 3) + 1;
	i *= 0.03;
	j *= 0.03;
	w *= 0.03;
	v *= 0.03;
	
	if (count < 600) {
		toWorld*=glm::translate(glm::mat4(1.0f), glm::vec3(i, 0, v));
	}
	else if (count >= 600 && count < 1200) {
		toWorld *= glm::translate(glm::mat4(1.0f), glm::vec3(-w, 0, -j));
	}
	else if (count >= 1200 && count < 1800) {
		toWorld *= glm::translate(glm::mat4(1.0f), glm::vec3(-i, 0, -v));
	}
	else if(count>=1800&&count<2400){
		toWorld *= glm::translate(glm::mat4(1.0f), glm::vec3(w, 0, j));
	}
	else {
		count = 0;
	}
	count++;
}
