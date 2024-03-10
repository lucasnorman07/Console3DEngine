#pragma once

#include "Vector.h"

struct Matrix4x4 {
	double data[4][4]{};

	Matrix4x4() {
		for (int i = 0; i < 4; i++) {
			data[i][i] = 1;
		}
	};
	Matrix4x4(std::initializer_list<std::initializer_list<double>> init_list) {
		int y = 0;
		for (const auto& row : init_list) {
			int x = 0;
			for (const auto& value : row) {
				data[y][x] = value;
				x++;
			}
			y++;
		}
	}

	Matrix4x4 multiplyMatrix(const Matrix4x4& matrix) const {
		Matrix4x4 result;
		for (int y = 0; y < 4; y++) {
			for (int x = 0; x < 4; x++) {
				double sum = 0;
				for (int i = 0; i < 4; i++) {
					sum += data[y][i] * matrix.data[x][i];
				}
				result.data[y][x] = sum;
			}
		}
		return result;
	}

	vec3 multiplyVector(const vec3& vector) const {
		return {
			vector.x * data[0][0] + vector.y * data[0][1] + vector.z * data[0][2] + vector.w * data[0][3],
			vector.x * data[1][0] + vector.y * data[1][1] + vector.z * data[1][2] + vector.w * data[1][3],
			vector.x * data[2][0] + vector.y * data[2][1] + vector.z * data[2][2] + vector.w * data[2][3],
			vector.x * data[3][0] + vector.y * data[3][1] + vector.z * data[3][2] + vector.w * data[3][3]
		};
	}

	Matrix4x4 operator*(const Matrix4x4& matrix) const {
		return multiplyMatrix(matrix);
	}

	vec3 operator*(const vec3& vector) const {
		return multiplyVector(vector);
	}
};