#include <iostream>
#include "Json.hpp"
using namespace acc;

int main(void){
	std::cout << "Hello World\n";
	JsonObject *test;

	test = new JsonObject("teste.json");

	//std::cout << *(test->get("twink").data.object_value->get("cu").data.string_value) << '\n';
	//std::cout << test->get("numeros").getObject()->get(3).getString() << '\n';
	
	delete test;

	return 0;
}
