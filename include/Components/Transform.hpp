#ifndef TRANSFORM_COMPONENT_HPP
#define TRANSFORM_COMPONENT_HPP

#include "Vec3.hpp"

namespace acc {

struct TransformComponent {
	/**
	 * The position for the entity.
	 */
	Vec3 position;

	TransformComponent(void);
};

};

#endif
