#pragma once

#include "Vector.h"
#include <algorithm>

const std::string SHADES = ".,-~:;=!*#$@";
//const char* SHADES = "0123456789";

void drawPoint(vec2 point, char value, char* screenBuffer, int width, int height) {
	point.x = (point.x + 1) * width / 2;
	point.y = (point.y + 1) * height / 2;

	int x = static_cast<int>(std::round(point.x));
	int y = static_cast<int>(std::round(point.y));
	if (x >= 0 && x < width && y >= 0 && y < height)
		screenBuffer[(int)(x + y * width)] = value;
}

void fillTriangle(const vec3& vector1, const vec3& vector2, const vec3& vector3, char* screenBuffer, double* depthBuffer, int width, int height, double lightValue) {
	vec2 point1 = { vector1.x, vector1.y };
	vec2 point2 = { vector2.x, vector2.y };
	vec2 point3 = { vector3.x, vector3.y };

	// get the depth values for each vector
	double z1 = vector1.z;
	double z2 = vector2.z;
	double z3 = vector3.z;

	// scale the x and y coordinates to screen
	point1.x = (point1.x + 1) * width / 2;
	point1.y = (point1.y + 1) * height / 2;
	point2.x = (point2.x + 1) * width / 2;
	point2.y = (point2.y + 1) * height / 2;
	point3.x = (point3.x + 1) * width / 2;
	point3.y = (point3.y + 1) * height / 2;

	// check for clockwise winding order
	if ((point3 - point1).determinant(point2 - point1) < 0) {
		// swap winding order
		std::swap(point2, point3);
		std::swap(z2, z3);
	}

	int minX = (int)std::min((double)width  - 1 , std::max(0.0,  std::min(std::min(point1.x, point2.x), point3.x) ) );
	int maxX = (int)std::min((double)width  - 1 , std::max(0.0,  std::max(std::max(point1.x, point2.x), point3.x) ) );
	int minY = (int)std::min((double)height - 1 , std::max(0.0,  std::min(std::min(point1.y, point2.y), point3.y) ) );
	int maxY = (int)std::min((double)height - 1 , std::max(0.0,  std::max(std::max(point1.y, point2.y), point3.y) ) );

	vec2 edge1 = point2 - point1;
	vec2 edge2 = point3 - point2;
	vec2 edge3 = point1 - point3;
	for (int x = minX; x <= maxX; x++) {
		for (int y = minY; y <= maxY; y++) {
			vec2 point = { x, y };
			vec2 pointEdge1 = point - point1;
			vec2 pointEdge2 = point - point2;
			vec2 pointEdge3 = point - point3;

			double determinant1 = pointEdge1.determinant(edge1);
			double determinant2 = pointEdge2.determinant(edge2);
			double determinant3 = pointEdge3.determinant(edge3);

			double length = edge2.determinant(edge1);
			double a = determinant2 / length;
			double b = determinant3 / length;
			double c = determinant1 / length;

			if (determinant1 >= 0 &&
				determinant2 >= 0 &&
				determinant3 >= 0) {
				// flip the depth value
				double depth = a * z1 + b * z2 + c * z3;
				int pixelIndex = x + y * width;
				if (depth < depthBuffer[pixelIndex]) {
					screenBuffer[pixelIndex] = SHADES[(int)std::clamp((lightValue * 0.9 + 0.1) * SHADES.length(), 0.0, SHADES.length() - 1.0)];
					depthBuffer[pixelIndex] = depth;
				}
			};
		}
	}
}