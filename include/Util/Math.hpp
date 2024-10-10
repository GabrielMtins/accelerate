#ifndef ACC_MATH_HPP
#define ACC_MATH_HPP

namespace acc {

float smoothstep(float lower, float higher, float x);

float smoothstep(float x);

template <typename T>
T interpolate(const T& start, const T& end, float lower, float higher, float x){
	return start + (x - lower) / (higher - lower) * (end - start);
}

};

#endif
