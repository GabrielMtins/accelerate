#include "Vec3.hpp"
#include <cmath>

namespace acc {

Vec3::Vec3(void){
	x = y = z = 0.0f;
}

Vec3::Vec3(float x, float y){
	this->x = x;
	this->y = y;
	this->z = 0;
}

Vec3::Vec3(float x, float y, float z){
	this->x = x;
	this->y = y;
	this->z = z;
}

Vec3 Vec3::operator+(const Vec3& obj){
	return Vec3(x + obj.x, y + obj.y, z + obj.z);
}

Vec3 Vec3::operator-(const Vec3& obj){
	return Vec3(x - obj.x, y - obj.y, z - obj.z);
}

Vec3 Vec3::operator*(const Vec3& obj){
	return Vec3(x * obj.x, y * obj.y, z * obj.z);
}

Vec3 Vec3::operator*(const float& r){
	return Vec3(x * r, y * r, z * r);
}

Vec3 Vec3::operator/(const float& r){
	return Vec3(x / r, y / r, z / r);
}

Vec3& Vec3::operator+=(const Vec3& obj){
	this->x += obj.x;
	this->y += obj.y;
	this->z += obj.z;

	return *this;
}

Vec3& Vec3::operator-=(const Vec3& obj){
	this->x -= obj.x;
	this->y -= obj.y;
	this->z -= obj.z;

	return *this;
}

Vec3& Vec3::operator*=(const Vec3& obj){
	this->x *= obj.x;
	this->y *= obj.y;
	this->z *= obj.z;

	return *this;
}

Vec3& Vec3::operator*=(const float& r){
	this->x *= r;
	this->y *= r;
	this->z *= r;

	return *this;
}

Vec3& Vec3::operator/=(const float& r){
	this->x /= r;
	this->y /= r;
	this->z /= r;

	return *this;
}

int Vec3::compareLength(float length_compared){
	float current_length = lengthSqr();

	if(current_length < length_compared)
		return -1;
	if(current_length > length_compared)
		return 1;

	return 0;
}

Vec3 Vec3::normalize(void){
	float l = length();

	return Vec3(x / l, y / l, z / l);
}

Vec3 Vec3::sign(void){
	Vec3 other = *this;

	if(other.x < 0) other.x = -1;
	if(other.y < 0) other.y = -1;
	if(other.z < 0) other.z = -1;

	if(other.x > 0) other.x = 1;
	if(other.y > 0) other.y = 1;
	if(other.z > 0) other.z = 1;
	
	return other;
}

Vec3 Vec3::clamp(float new_size){
	return normalize() * new_size;
}

float Vec3::lengthSqr(void){
	return x * x + y * y + z * z;
}

float Vec3::length(void){
	return sqrt(lengthSqr());
}

float Vec3::dotProduct(Vec3 a, Vec3 b){
	return a.x * b.x + a.y * b.y + a.z * b.z;
}

};
