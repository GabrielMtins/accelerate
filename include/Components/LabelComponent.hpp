#ifndef LABEL_COMPONENT_HPP
#define LABEL_COMPONENT_HPP

#include <string>

namespace acc {

struct LabelComponent {
	/**
	 * The name of the class of the object
	 */
	std::string class_name;

	LabelComponent(void);
	LabelComponent(const std::string& class_name);
};

};

#endif
