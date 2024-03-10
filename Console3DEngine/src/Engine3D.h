#pragma once

#include <cstdio>
#include <iostream>
#include <thread>
#include "Renderer.h"
#include "Mesh.h"
#include "Matrix.h"
#include "Camera.h"

#define PI 3.14159265359
// small screen
static const int width = 100 / 1.6;
static const int height = 50 / 1.6;
// big screen
//static const int width = 200;
//static const int height = 50;
static const int size = width * height;

class Engine3D {
private:
	char* screenBuffer;
	double* depthBuffer;
	Camera camera;
	std::vector<Mesh*> meshes;
	Matrix4x4 projectionMatrix;
	std::string title;

public:
	Engine3D(std::string title) {
		this->title = title;
		screenBuffer = new char[size];
		depthBuffer = new double[size];

		double FOV = 90.0 * PI / 180;
		double zFar = 1000.0;
		double zNear = 0.1;
		double f = 1.0 / std::tan(FOV / 2.0);
		double q = zFar / (zFar - zNear);

		// the height/width ration of each charachter
		double charHeightWidthRatio = 2.0 / 1.0;
		// the height/width ration of the screen
		double aspectRatio = height / (double)width;
		projectionMatrix = Matrix4x4({
			{f * aspectRatio * charHeightWidthRatio, 0, 0, 0},
			{0, f, 0, 0},
			{0, 0, q, q * -zNear},
			{0, 0, 1, 0}
		});
	}

	~Engine3D() {
		delete[] screenBuffer;
		delete[] depthBuffer;
	}

	void addMesh(Mesh& mesh) {
		meshes.push_back(&mesh);
	}

	void render(int deltaTime/*in milliseconds*/) {
		// clear the depthBuffer
		std::fill(depthBuffer, depthBuffer + size, std::numeric_limits<double>::infinity());

		// clear screenBuffer
		memset(screenBuffer, ' ', size * sizeof(char));

		for (const Mesh *mesh : meshes) {
			// render the mesh to the screen buffer
			mesh->render(screenBuffer, depthBuffer, width, height, camera, projectionMatrix, deltaTime);
		}

		// clear console
		std::system("cls");

		// create status text
		std::string statusText = "Title: " + title +
			", deltaTime: " + std::to_string(deltaTime) +
			", FPS: " + (deltaTime > 0 ? std::to_string(1000 / deltaTime) : "0");
		for (int i = 0; i < statusText.length(); i++) {
			screenBuffer[i + (height - 1) * width] = statusText[i];
		}

		// draw screen (loop from end to start because the y should flip)
		for (int y = height - 1; y >= 0; y--) {
			std::cout.write(screenBuffer + sizeof(char) * width * y, sizeof(char) * width);
			std::putchar('\n');
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(16));
	}
};