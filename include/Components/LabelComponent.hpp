#ifndef LABEL_COMPONENT_HPP
#define LABEL_COMPONENT_HPP

#include <string>

namespace acc {

struct LabelComponent {
	std::string class_name; /* the name of the class of object */

	LabelComponent(void);
	LabelComponent(std::string class_name);
};

};

#endif
