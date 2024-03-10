#pragma once

#include <iostream>

struct vec3 {
	double x, y, z, w;

	vec3() : x(0), y(0), z(0), w(1) {}
	vec3(double x, double y, double z, double w = 1) : x(x), y(y), z(z), w(w) {}

	vec3 add(const vec3& other) const {
		return { x + other.x, y + other.y, z + other.z, w };
	}

	vec3 subtract(const vec3& other) const {
		return { x - other.x, y - other.y, z - other.z, w };
	}

	vec3 multiply(double factor) const {
		return { x * factor, y * factor, z * factor, w };
	}

	vec3 divide(double factor) const {
		return { x / factor, y / factor, z / factor, w };
	}

	double dot(const vec3& other) const {
		return x * other.x + y * other.y + z * other.z;
	}

	vec3 cross(const vec3& other) const {
		return {
			y * other.z - z * other.y,
			z * other.x - x * other.z,
			x * other.y - y * other.x,
			w
		};
	}

	vec3 normalized() {
		double length = std::sqrt(x * x + y * y + z * z);
		if (length != 0) {
			return { x / length, y / length, z / length, w };
		} else {
			return { 0.0, 0.0, 0.0 };
		}
	}

	vec3 operator+(const vec3& other) const {
		return add(other);
	}

	vec3 operator-(const vec3& other) const {
		return subtract(other);
	}

	vec3 operator*(double other) const {
		return multiply(other);
	}

	vec3 operator/(double other) const {
		return divide(other);
	}
};

std::ostream& operator<<(std::ostream& o, const vec3& v) {
	o << "{" << v.x << ", " << v.y << ", " << v.z << "}\n";
	return o;
}

struct vec2 {
	double x, y;

	vec2(double x, double y) : x(x), y(y) {}
	vec2(int x, int y) : x((double)x), y((double)y) {}
	vec2(vec3 vector) : x(vector.x), y(vector.y) {}

	vec2 add(const vec2& other) const {
		return { x + other.x, y + other.y };
	}

	vec2 subtract(const vec2& other) const {
		return { x - other.x, y - other.y };
	}

	vec2 multiply(double factor) const {
		return { x * factor, y * factor };
	}

	vec2 divide(double factor) const {
		return { x / factor, y / factor };
	}

	double dot(const vec2& other) const {
		return x * other.x + y * other.y;
	}

	double determinant(const vec2& other) const {
		return x * other.y - y * other.x;
	}

	vec2 operator+(const vec2& other) const {
		return add(other);
	}

	vec2 operator-(const vec2& other) const {
		return subtract(other);
	}

	vec2 operator*(double other) const {
		return multiply(other);
	}

	vec2 operator/(double other) const {
		return divide(other);
	}
};