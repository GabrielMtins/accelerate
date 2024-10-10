#include "Util/Math.hpp"

namespace acc {

float smoothstep(float lower, float higher, float x){
	float interpolated = (x - lower) / (higher - lower);

	return smoothstep(interpolated);
}

float smoothstep(float x){
	if(x < 0) return 0;
	if(x > 1) return 1;

	return x * x * (3 - 2 * x);
}

};
