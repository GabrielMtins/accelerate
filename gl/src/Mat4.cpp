#include "Mat4.hpp"
#include <cmath>

namespace acc {

Mat4::Mat4(void){
	for(int i = 0; i < 16; i++){
		arr[i] = 0;
	}
}

Mat4 Mat4::operator+(const Mat4& obj) const{
	Mat4 other;

	for(int i = 0; i < 16; i++){
		other.arr[i] = arr[i] + obj.arr[i];
	}

	return other;
}

Mat4 Mat4::operator-(const Mat4& obj) const{
	Mat4 other;

	for(int i = 0; i < 16; i++){
		other.arr[i] = arr[i] - obj.arr[i];
	}

	return other;
}

Mat4 Mat4::operator*(const Mat4& obj) const{
	Mat4 other;

	for(int j = 0; j < 4; j++){
		for(int i = 0; i < 4; i++){
			float res = 0.0f;

			for(int k = 0; k < 4; k++){
				res += arr[k + j * 4] * obj.arr[i + 4 * k];
			}

			other.arr[i + j * 4] = res;
		}
	}

	return other;
}

Vec3 Mat4::operator*(const Vec3& obj) const{
	Vec3 ret;

	ret.x = arr[0] * obj.x + arr[1] * obj.y + arr[2] * obj.z + arr[3];
	ret.y = arr[4] * obj.x + arr[5] * obj.y + arr[6] * obj.z + arr[7];
	ret.z = arr[8] * obj.x + arr[9] * obj.y + arr[10] * obj.z + arr[11];

	return ret;
}

Mat4 Mat4::Identity(void){
	Mat4 identity;

	for(int i = 0; i < 16; i++){
		identity.arr[i] = 0;
	}

	for(int i = 0; i < 4; i++){
		identity.arr[i * 4 + i] = 1;
	}

	return identity;
}

Mat4 Mat4::Transform(float x, float y, float z){
	Mat4 transform = Mat4::Identity();

	transform.arr[3] = x;
	transform.arr[7] = y;
	transform.arr[11] = z;

	return transform;
}

Mat4 Mat4::Transform(const acc::Vec3& vec){
	return Mat4::Transform(vec.x, vec.y, vec.z);
}

Mat4 Mat4::Scale(float x, float y, float z){
	Mat4 scale;

	scale.arr[0] = x;
	scale.arr[5] = y;
	scale.arr[10] = z;
	scale.arr[15] = 1.0f;
	
	return scale;
}

Mat4 Mat4::RotateX(float angle){
	Mat4 rotate_x;

	float cos_angle = cosf(angle);
	float sin_angle = sinf(angle);

	rotate_x.arr[0] = 1.0f;
	rotate_x.arr[15] = 1.0f;

	rotate_x.arr[5] = cos_angle;
	rotate_x.arr[6] = -sin_angle;
	rotate_x.arr[9] = sin_angle;
	rotate_x.arr[10] = cos_angle;

	return rotate_x;
}

Mat4 Mat4::RotateY(float angle){
	Mat4 rotate_y;

	float cos_angle = cosf(angle);
	float sin_angle = sinf(angle);

	rotate_y.arr[0] = cos_angle;
	rotate_y.arr[2] = sin_angle;
	rotate_y.arr[5] = 1.0f;
	rotate_y.arr[8] = -sin_angle;
	rotate_y.arr[10] = cos_angle;

	rotate_y.arr[15] = 1.0f;
	
	return rotate_y;
}

Mat4 Mat4::RotateZ(float angle){
	Mat4 rotate_z;
	float cos_angle = cosf(angle);
	float sin_angle = sinf(angle);

	rotate_z.arr[0] = cos_angle;
	rotate_z.arr[1] = -sin_angle;
	rotate_z.arr[4] = sin_angle;
	rotate_z.arr[5] = cos_angle;
	rotate_z.arr[10] = 1.0f;
	rotate_z.arr[15] = 1.0f;

	return rotate_z;
}

Mat4 Mat4::PerspectiveProjection(float aspect_ratio, float fov, float far, float near){
	Mat4 perspective_projection;

	float s = 1.0f / tanf(0.5f * fov);
	float a1 = -(far + near)/ (far - near);
	float a2 = - 2.0f * far * near / (far - near);

	perspective_projection.arr[0] = s / aspect_ratio;
	perspective_projection.arr[5] = s;
	perspective_projection.arr[10] = a1;
	perspective_projection.arr[11] = a2;
	perspective_projection.arr[14] = -1.0f;
	
	return perspective_projection;
}

};
