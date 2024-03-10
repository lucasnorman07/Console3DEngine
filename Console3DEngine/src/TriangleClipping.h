#pragma once

#include "Vector.h"

struct Triangle {
	vec3 points[3]{};

	Triangle(vec3& p1, vec3& p2, vec3& p3) : points{ p1, p2, p3 } {}
	Triangle() {}

	vec3& operator[](int index) {
		if (index < 0 || index > 2) throw std::out_of_range("Index is out of bounds");
		return points[index];
	}
};

vec3 findIntersection(vec3 point1, vec3 point2, vec3 planeNormal, vec3 planePoint) {
	vec3 line = point2 - point1;
	double t = (planePoint - point1).dot(planeNormal) / line.dot(planeNormal);
	vec3 intersection = point1 + line * t;
	return intersection;
}

int clipTriangle(Triangle& triangle, Triangle& outputTriangle1, Triangle& outputTriangle2, vec3 planeNormal, vec3 planePoint) {
	int insideCount = 0;
	int outsideCount = 0;
	vec3 insidePoints[3];
	vec3 outsidePoints[3];
	for (int i = 0; i < 3; i++) {
		if ((triangle[i] - planePoint).dot(planeNormal) > 0) {
			insidePoints[insideCount++] = triangle[i];
		} else {
			outsidePoints[outsideCount++] = triangle[i];
		}
	}
		
	if (insideCount == 0) {
		 return 0;
	} else if (insideCount == 3) {
		outputTriangle1[0] = triangle[0];
		outputTriangle1[1] = triangle[1];
		outputTriangle1[2] = triangle[2];
		return 1;
	} else if (insideCount == 1) {
		outputTriangle1[0] = insidePoints[0];
		outputTriangle1[2] = findIntersection(insidePoints[0], outsidePoints[1], planeNormal, planePoint);
		outputTriangle1[1] = findIntersection(insidePoints[0], outsidePoints[0], planeNormal, planePoint);
		return 1;
	} else if (insideCount == 2) {
		outputTriangle1[0] = insidePoints[0];
		outputTriangle1[2] = findIntersection(insidePoints[0], outsidePoints[0], planeNormal, planePoint);
		outputTriangle1[1] = insidePoints[1];

		outputTriangle2[0] = insidePoints[1];
		outputTriangle2[2] = findIntersection(insidePoints[0], outsidePoints[0], planeNormal, planePoint);
		outputTriangle2[1] = findIntersection(insidePoints[1], outsidePoints[0], planeNormal, planePoint);
		return 2;
	}
}