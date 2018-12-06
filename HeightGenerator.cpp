#include "HeightGenerator.h"
#include "Window.h"
#include <iostream>
#include <fstream>
#include <math.h>
#include <random>

# define M_PI           3.14159265358979323846 

HeightGenerator::HeightGenerator()
{
 seed = randomInt(1, 1000000000);
}

HeightGenerator::HeightGenerator(int gridX, int gridZ, int vertexCount, int seed) {
	this->seed = seed;
	xOffset = gridX * (vertexCount - 1);
	zOffset = gridZ * (vertexCount - 1);
}

GLfloat HeightGenerator::genHeight(int x, int z) {
	GLfloat total = 0;
	GLfloat t = 16.0f;
	float d = (float)pow(2, OCTAVES - 1);
	for (int i = 0; i < OCTAVES; i++) {
		float freq = (float)(pow(2, i) / d);
		float amp = (float)pow(ROUGHNESS, i) * AMP;
		total += getInterpolatedNoise(((float)(x + xOffset)*freq), ((float)(z + zOffset)*freq)) * amp;
	}
	return total;
}

GLfloat HeightGenerator::getInterpolatedNoise(float x, float z){
	int intX = (int)x;
	int intZ = (int)z;
	float fracX = x - intX;
	float fracZ = z - intZ;

	float v1 = getSmoothNoise(intX, intZ);
	float v2 = getSmoothNoise(intX + 1, intZ);
	float v3 = getSmoothNoise(intX, intZ + 1);
	float v4 = getSmoothNoise(intX + 1, intZ + 1);
	float i1 = interpolate(v1, v2, fracX);
	float i2 = interpolate(v3, v4, fracX);
	return interpolate(i1, i2, fracZ);
}

float HeightGenerator::interpolate(float a, float b, float blend) {
	double theta = blend *M_PI;
	float f = (float)(1.0f - cos(theta)) * 0.5f;
	return a * (1.0f - f) + b * f;
}

float HeightGenerator::getSmoothNoise(int x, int z) {
	float corners = (getNoise(x - 1, z - 1) + getNoise(x + 1, z - 1) + getNoise(x - 1, z + 1)
		+ getNoise(x + 1, z + 1)) / 100.0f;
	float sides = (getNoise(x - 1, z) + getNoise(x + 1, z) + getNoise(x, z - 1)
		+ getNoise(x, z + 1)) / 80.0f;
	float center = getNoise(x, z) / 900.0f;
	return corners + sides + center;
}

float HeightGenerator::getNoise(int x, int z) {
	seed = x * 49654 + z * 324176 + seed;
	return ((double)rand() / (RAND_MAX)) * 2.0f - 1.0f;
}