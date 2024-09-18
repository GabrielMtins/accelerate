#ifndef VEC3_HPP
#define VEC3_HPP

namespace acc {

struct Vec3 {
	float x, y, z;

	Vec3(void);
	Vec3(float x, float y);
	Vec3(float x, float y, float z);

	Vec3 operator+(const Vec3& obj);
	Vec3 operator-(const Vec3& obj);
	Vec3 operator*(const Vec3& obj);
	Vec3 operator*(const float& r);
	Vec3 operator/(const float& r);

	Vec3& operator+=(const Vec3& obj);
	Vec3& operator-=(const Vec3& obj);
	Vec3& operator*=(const Vec3& obj);
	Vec3& operator*=(const float& r);
	Vec3& operator/=(const float& r);

	int compareLength(float length_compared);

	Vec3 normalize(void);
	Vec3 sign(void);
	Vec3 clamp(float new_size);

	float lengthSqr(void);
	float length(void);

	void print(void);

	static float dotProduct(Vec3 a, Vec3 b);
};

};

#endif
