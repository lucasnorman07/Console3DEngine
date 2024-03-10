#pragma once

#include <array>
#include <vector>
#include <fstream>
#include <sstream>
#include "Matrix.h"
#include "Vector.h"
#include "Camera.h"
#include "TriangleClipping.h"
#include "Engine3D.h"

#define LIGHT_DIRECTION vec3(0, -1, 2).normalized()

struct Mesh {
	bool fileLoadedSuccessfully = false;
	vec3 translation;
	vec3 rotation;
	double scaleFactor = 1;
	std::vector<vec3> vertexBuffer;
	std::vector<int> indexBuffer;

	Mesh() {}
	Mesh(const std::string& filename) {
		fileLoadedSuccessfully = loadFromObjFile(filename);
	}

	bool loadFromObjFile(const std::string& filename) {
		std::ifstream file(filename);
		if (file.fail()) {
			std::cout << "File could not be opened: " << filename << "\n";
			// error
			return false;
		}
		std::string line;
		while (std::getline(file, line)) {
			if (line[0] == 'v') {
				std::stringstream s;
				s << line;
				char junk;
				double x, y, z;
				s >> junk >> x >> y >> z;
				vertexBuffer.push_back({ x, y, z });
			}
			else if (line[0] == 'f') {
				std::stringstream s;
				s << line;
				char junk;
				int v0, v1, v2;
				s >> junk >> v0 >> v1 >> v2;
				indexBuffer.push_back(v0 - 1);
				indexBuffer.push_back(v1 - 1);
				indexBuffer.push_back(v2 - 1);
			}
		}
		// success
		return true;
	}

	void move(double translateX, double translateY, double translateZ) {
		translation.x += translateX;
		translation.y += translateY;
		translation.z += translateZ;
	}

	void rotate(double rotationX, double rotationY, double rotationZ) {
		rotation.x += rotationX;
		rotation.y += rotationY;
		rotation.z += rotationZ;
	}

	void scale(double scaleFactor) {
		this->scaleFactor = scaleFactor;
	}

	Matrix4x4 createTransformationMatrix() const {
		// create translation matrix
		Matrix4x4 translationMatrix({
			{1 * scaleFactor, 0, 0, translation.x},
			{0, 1 * scaleFactor, 0, translation.y},
			{0, 0, 1 * scaleFactor, translation.z},
			{0, 0, 0, 1}
		});

		// create rotation matricies
		Matrix4x4 rotationMatrixX({
			{1, 0, 0, 0},
			{0, cos(rotation.x), -sin(rotation.x), 0},
			{0, sin(rotation.x), cos(rotation.x), 0},
			{0, 0, 0, 1}
		});

		Matrix4x4 rotationMatrixY({
			{cos(rotation.y), 0, sin(rotation.y), 0},
			{0, 1, 0, 0},
			{-sin(rotation.y), 0, cos(rotation.y), 0},
			{0, 0, 0, 1}
		});

		Matrix4x4 rotationMatrixZ({
			{cos(rotation.z), -sin(rotation.z), 0, 0},
			{sin(rotation.z), cos(rotation.z), 0, 0},
			{0, 0, 1, 0},
			{0, 0, 0, 1}
		});

		return translationMatrix * rotationMatrixZ * rotationMatrixY * rotationMatrixX;
	}

	void render(char* screenBuffer, double* depthBuffer, int width, int height, Camera& camera, Matrix4x4& projectionMatrix, int deltaTime) const {
		// if the mesh has not loaded
		if (!fileLoadedSuccessfully) return;

		int size = width * height;

		// listen for key input and move the camera
		camera.checkInput(deltaTime);

		// create transformation matrix for the mesh
		Matrix4x4 transformationMatrix = createTransformationMatrix();

		// create view matrix for the camera
		Matrix4x4 viewMatrix = camera.createViewMatrix();

		// draw triangle to screen
		for (int i = 0; i < indexBuffer.size(); i += 3) {
			vec3 point1 = vertexBuffer[indexBuffer[i]];
			vec3 point2 = vertexBuffer[indexBuffer[i + 1]];
			vec3 point3 = vertexBuffer[indexBuffer[i + 2]];

			// transform all of the points
			vec3 transformedPoint1 = transformationMatrix * point1;
			vec3 transformedPoint2 = transformationMatrix * point2;
			vec3 transformedPoint3 = transformationMatrix * point3;

			// get the surface normal
			vec3 surfaceNormal = (transformedPoint3 - transformedPoint1).cross(transformedPoint2 - transformedPoint1).normalized();

			// face culling
			if ((transformedPoint1 - camera.position).dot(surfaceNormal) < 0) continue;

			// add camera translation
			vec3 viewedPoint1 = viewMatrix * transformedPoint1;
			vec3 viewedPoint2 = viewMatrix * transformedPoint2;
			vec3 viewedPoint3 = viewMatrix * transformedPoint3;

			// clip the triangle
			Triangle trianglesToRaster[2];
			Triangle triangle = {viewedPoint1, viewedPoint2, viewedPoint3};
			int triangleCount = clipTriangle(triangle, trianglesToRaster[0], trianglesToRaster[1], vec3(0, 0, 1), vec3(0, 0, 0.1));

			for (int j = 0; j < triangleCount; j++) {
				vec3 p1 = trianglesToRaster[j][0];
				vec3 p2 = trianglesToRaster[j][1];
				vec3 p3 = trianglesToRaster[j][2];

				// project the points
				p1 = projectionMatrix * p1;
				p2 = projectionMatrix * p2;
				p3 = projectionMatrix * p3;
				// perspective divide
				p1 = p1 / p1.w;
				p2 = p2 / p2.w;
				p3 = p3 / p3.w;

				fillTriangle(
					p1,
					p2,
					p3,
					screenBuffer, depthBuffer, width, height,
					surfaceNormal.dot(LIGHT_DIRECTION)
				);

				/*drawPoint({ p1.x, p1.y }, '0' + (indexBuffer[i]), screenBuffer, width, height);
				drawPoint({ p2.x, p2.y }, '0' + (indexBuffer[i + 1]), screenBuffer, width, height);
				drawPoint({ p3.x, p3.y }, '0' + (indexBuffer[i + 2]), screenBuffer, width, height);*/
			}

		}
	}

	//std::vector<vec3> vertexBuffer = {
	//	{ -1.0,  1.0, -1.0 },
	//	{  1.0,  1.0, -1.0 },
	//	{  1.0, -1.0, -1.0 },
	//	{ -1.0, -1.0, -1.0 },
	//	{ -1.0,  1.0,  1.0 },
	//	{  1.0,  1.0,  1.0 },
	//	{  1.0, -1.0,  1.0 },
	//	{ -1.0, -1.0,  1.0 }
	//};

 //   std::vector<int> indexBuffer = {
	//	// front
 //       3, 0, 2,
	//	0, 1, 2,
	//	// back
	//	7, 6, 4,
	//	4, 6, 5,
	//	// top
	//	7, 3, 6,
	//	3, 2, 6,
	//	// bottom
	//	0, 4, 5,
	//	1, 0, 5,
	//	// left
	//	7, 4, 3,
	//	4, 0, 3,
	//	// right
	//	2, 1, 6,
	//	1, 5, 6
 //   };

};