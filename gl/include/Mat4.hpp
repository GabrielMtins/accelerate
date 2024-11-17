#ifndef MAT4_HPP
#define MAT4_HPP

#include "Vec3.hpp"

namespace acc {

struct Mat4 {
	float arr[16];

	Mat4(void);
	Mat4 operator+(const Mat4& obj) const;
	Mat4 operator-(const Mat4& obj) const;
	Mat4 operator*(const Mat4& obj) const;

	static Mat4 Identity(void);
	static Mat4 Transform(float x, float y, float z);
	static Mat4 Transform(const acc::Vec3& vec);
	static Mat4 Scale(float x, float y, float z);
	static Mat4 RotateZ(float angle);
};

};

#endif
