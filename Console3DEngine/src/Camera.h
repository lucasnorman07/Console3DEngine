#pragma once

#include <Windows.h>
#include "Matrix.h"
#include "Vector.h"

#define MOVEMENT_SPEED 0.002

struct Camera {
	vec3 position = vec3(0, 0, 0);
	vec3 lookDirection = vec3(0, 0, 1);

	void move(double translateX, double translateY, double translateZ) {
		position.x += translateX;
		position.y += translateY;
		position.z += translateZ;
	}

	void moveForward(double distance) {
		position = position + (lookDirection * distance);
	};

	void moveRight(double distance) {
		vec3 rightDirection = vec3(0, 1, 0).cross(lookDirection).normalized();
		position = position + (rightDirection * distance);
	};

	void moveUp(double distance) {
		vec3 rightDirection = vec3(0, 1, 0).cross(lookDirection).normalized();
		vec3 upDirection = lookDirection.cross(rightDirection).normalized();
		position = position + (upDirection * distance);
	};

	void rotate(double rotationX, double rotationY, double rotationZ) {
		// create rotation matricies
		Matrix4x4 rotationMatrixX({
			{1, 0, 0, 0},
			{0, cos(rotationX), -sin(rotationX), 0},
			{0, sin(rotationX), cos(rotationX), 0},
			{0, 0, 0, 1}
		});

		Matrix4x4 rotationMatrixY({
			{cos(rotationY), 0, sin(rotationY), 0},
			{0, 1, 0, 0},
			{-sin(rotationY), 0, cos(rotationY), 0},
			{0, 0, 0, 1}
		});
		Matrix4x4 rotationMatrixZ({
			{cos(rotationZ), -sin(rotationZ), 0, 0},
			{sin(rotationZ), cos(rotationZ), 0, 0},
			{0, 0, 1, 0},
			{0, 0, 0, 1}
		});


		// change the look direction
		lookDirection = (rotationMatrixZ * rotationMatrixY * rotationMatrixX)
			* lookDirection;
	}

	void pointAt(vec3 target) {
		lookDirection = (target - position).normalized();
	}

	Matrix4x4 createViewMatrix() {
		vec3 forward = lookDirection;
		vec3 right = vec3(0, 1, 0).cross(forward).normalized();
		vec3 up = forward.cross(right).normalized();

		// create look at matrix
		Matrix4x4 lookAtMatrix({
			{right.x,   right.y,   right.z,   -position.dot(right)},
			{up.x,      up.y,      up.z,      -position.dot(up)},
			{forward.x, forward.y, forward.z, -position.dot(forward)},
			{0, 0, 0, 1}
		});

		return lookAtMatrix;
	}

	void checkInput(int deltaTime) {
		const int KEY_W = 0x57;
		const int KEY_A = 0x41;
		const int KEY_S = 0x53;
		const int KEY_D = 0x44;
		const double movementSpeed = (GetAsyncKeyState(VK_SHIFT) ? MOVEMENT_SPEED * 2 : MOVEMENT_SPEED) * deltaTime;
		if (GetAsyncKeyState(KEY_W)) {
			moveForward(movementSpeed);
		}
		if (GetAsyncKeyState(VK_LEFT)) {
			moveRight(-movementSpeed);
		}
		if (GetAsyncKeyState(KEY_S)) {
			moveForward(-movementSpeed);
		}
		if (GetAsyncKeyState(VK_RIGHT)) {
			moveRight(movementSpeed);
		}
		if (GetAsyncKeyState(VK_UP)) {
			moveUp(movementSpeed);
		}
		if (GetAsyncKeyState(VK_DOWN)) {
			moveUp(-movementSpeed);
		}
		if (GetAsyncKeyState(KEY_A)) {
			rotate(0, movementSpeed / 2.5, 0);
		}
		if (GetAsyncKeyState(KEY_D)) {
			rotate(0, -movementSpeed / 2.5, 0);
		}
	}
};