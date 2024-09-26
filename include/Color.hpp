#ifndef COLOR_HPP
#define COLOR_HPP

#include <cstdint>

namespace acc {

struct Color {
	uint8_t r, g, b, a;

	Color(void);
	Color(uint8_t r, uint8_t g, uint8_t b);
	Color(uint8_t r, uint8_t g, uint8_t b, uint8_t a);
};

};

#endif
