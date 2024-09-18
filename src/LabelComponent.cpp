#include "Components/LabelComponent.hpp"

namespace acc {
	
LabelComponent::LabelComponent(void){
	this->class_name = "";
}

LabelComponent::LabelComponent(std::string class_name){
	this->class_name = class_name;
}

};
