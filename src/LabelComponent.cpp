#include "Components/LabelComponent.hpp"

namespace acc {
	
LabelComponent::LabelComponent(void){
	this->class_name = "";
	this->own_name = "";
}

LabelComponent::LabelComponent(std::string class_name, std::string own_name){
	this->class_name = class_name;
	this->own_name = own_name;
}

};
