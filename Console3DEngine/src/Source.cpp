#include <chrono>
#include "Engine3D.h"
#include "Mesh.h"

int main() {
	// create the 3d engine
	Engine3D engine("3D Ascii Art!");

	// create the meshes
	Mesh mesh1("3D objects/spaceShip.txt");
	mesh1.move(0, 0, 10);
	Mesh mesh2("3D objects/spaceShip.txt");
	mesh2.move(0, 0, 40);
	Mesh mesh3("3D objects/teaPot.txt");
	mesh3.move(10, 30, -20);
	Mesh mesh4("3D objects/torus.txt");
	mesh4.move(20, -10, 5);
	Mesh mesh5("3D objects/rabbit.txt");
	mesh5.move(-20, 10, 5);
	mesh5.scale(0.1);
	mesh5.rotate(3.14 / 2, 0, 0);

	// add the meshes
	engine.addMesh(mesh1);
	engine.addMesh(mesh2);
	engine.addMesh(mesh3);
	engine.addMesh(mesh4);
	engine.addMesh(mesh5);

	// render loop
	auto previousTime = std::chrono::high_resolution_clock::now();
	while (true) {
		auto now = std::chrono::high_resolution_clock::now();
		int deltaTime = static_cast<int>(
			std::chrono::duration_cast<std::chrono::milliseconds>(now - previousTime).count()
		);
		previousTime = std::chrono::high_resolution_clock::now();
		mesh4.rotate(0.01, 0.03, 0.02);

		engine.render(deltaTime);
	}
}