#ifndef OBJOBJECT_H
#define OBJOBJECT_H

#ifdef __APPLE__
#include <OpenGL/gl3.h>
#include <OpenGL/glext.h>
#include <OpenGL/gl.h> // Remove this line in future projects
#else
#include <GL/glew.h>
#endif

#include <GLFW/glfw3.h>
#include <GL/glut.h>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>

class OBJObject
{
private:
std::vector<unsigned int> indices;
std::vector<glm::vec3> vertices;
std::vector<glm::vec3> normals;
std::vector<glm::vec3> random;


public:
	GLfloat size;
	GLfloat angle;
	glm::vec3 position;
	//std::vector<GLint> faces;
	glm::mat4 toWorld;
	GLfloat newX;
	GLfloat newY;
	GLfloat newZ;
	GLfloat scaleX, scaleY, scaleZ;
	OBJObject(const char* filepath);
	OBJObject::~OBJObject();
	// These variables are needed for the shader program
	GLuint VBO, VAO, EBO, Norms,TO;
	GLuint uProjection, uModelview;
	std::vector< unsigned int > vertexIndices, uvIndices, normalIndices;
	std::vector<glm::vec3> a_vertices;
	std::vector<glm::vec3> a_normals;
	std::vector<glm::vec2> a_uv;
	std::vector<glm::vec2> tex;
	//for texture
	GLint twidth = 512;
	GLint theight = 512;   // texture width/height [pixels]
	bool objRotation;
	//lights rotations
	bool dirRot;
	bool ptRot;
	bool sptRot;
	//camera rotation
	bool camRot=true;
	//if light source
	bool ifDirSource;
	bool ifPtSource;
	bool ifSptSource;
	//for material
	glm::mat3 material;
	GLfloat shiness;
	//for lighting
	glm::vec3 dirLightDir;
	glm::vec3 ptLightDir;
	glm::vec3 sptLightDir;
	GLint dirOn, pointOn, spotOn;
	GLint normalColor;
	GLint isLight=0;
	GLfloat sptCutOff,sptCutOff2;
	//lights positions
	glm::vec3 ptPos,sptPos;
	//lights color
	glm::vec3 dirColor = { 1.0f,1.0f,1.0f };
	glm::vec3 pointLightColors[4] = {

		glm::vec3(1.0f, 0.9f, 1.0f),
		glm::vec3(0.8f, 0.8f, 0.8f),
		glm::vec3(1.0f, 1.0f, 1.0f),
		glm::vec3(0.4f, 0.7f, 0.1f)
	};
	glm::vec3 sptColor= { 1.0f,1.0f,0.8f };
	//trackBall color
	glm::vec3 dirTColor;
	glm::vec3 ptTColor;
	glm::vec3 sptTColor;
	//for texture
	std::vector<std::string> faces;

	void parse(const char* filepath);
	void draw(GLuint);
	void draw(GLuint shaderProgram, glm::mat4 C);
	void drawFrame(GLuint shaderProgram, glm::mat4 C);
	void update();
	void spin(float);
	void colorChange(GLint m);
	void center();
	void scale(bool);
	void translate(glm::vec3);
	void scaleSize(GLfloat);
	unsigned int loadCubemap(std::vector<std::string> faces);
	GLfloat randomFloat(GLfloat a, GLfloat b) {
		GLfloat random = ((GLfloat)rand()) / (GLfloat)RAND_MAX;
		GLfloat diff = b - a;
		GLfloat r = random * diff;
		return a + r;
	}
	unsigned char* loadPPM(const char* filename, int& width, int& height);
	void loadTexture(unsigned char* tdata);
};

#endif