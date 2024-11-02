#include "Json.hpp"

#include <iostream>
#include <fstream>
#include <cctype>

namespace acc {

enum JSON_STATE {
	SEARCH_OPEN_BRACKET = 0,
	SEARCH_FOR_END,
	SEARCH_OPEN_SQUARE,
	SEARCH_COLON,
	SEARCH_VALUE,
	STRING_SEARCH_QUOTE,
	STRING_READ_ON,
	NUMBER_READ,
};

enum TOKEN_TYPE {
	TOKEN_KEY = 0,
	TOKEN_STRING,
};

JsonType::JsonType(void){
	type = JSON_TYPE_NULL;
}

JsonType::JsonType(double number_value){
	type = JSON_TYPE_NUMBER;
	data.number_value = number_value;
}

JsonType::JsonType(bool boolean_value){
	type = JSON_TYPE_BOOLEAN;
	data.boolean_value = boolean_value;
}

JsonType::JsonType(const std::string& string_value){
	type = JSON_TYPE_STRING;
	data.string_value = new std::string(string_value);
}

JsonType::JsonType(JsonObject *object_value){
	type = JSON_TYPE_OBJECT;
	data.object_value = object_value;
}

double JsonType::getNumber(void){
	return data.number_value;
}

bool JsonType::getBoolean(void){
	return data.boolean_value;
}

std::string JsonType::getString(void){
	return std::string(*data.string_value);
}

JsonObject * JsonType::getObject(void){
	return data.object_value;
}

bool JsonType::isNumber(void){
	return type == JSON_TYPE_NUMBER;
}

bool JsonType::isBoolean(void){
	return type == JSON_TYPE_BOOLEAN;
}

bool JsonType::isString(void){
	return type == JSON_TYPE_STRING;
}

bool JsonType::isObject(void){
	return type == JSON_TYPE_OBJECT;
}

JsonObject::JsonObject(void){
	is_array = false;
}

JsonObject::JsonObject(const std::string& filename){
	is_array = false;
	setName(filename);
	parseFile(filename);
}

JsonObject::~JsonObject(void){
	for(auto& i : dictionary){
		if(i.second.type == JSON_TYPE_STRING)
			delete i.second.data.string_value;
		if(i.second.type == JSON_TYPE_OBJECT)
			delete i.second.data.object_value;
	}

	for(auto& i : json_array){
		if(i.type == JSON_TYPE_STRING)
			delete i.data.string_value;
		if(i.type == JSON_TYPE_OBJECT)
			delete i.data.object_value;
	}
}

bool JsonObject::parseFile(const std::string& filename){
	std::ifstream file(filename);
	std::string line;
	size_t line_id = 1;

	object_stack.push(this);

	state = next_state = SEARCH_OPEN_BRACKET;

	if(!file.is_open())
		return false;

	while(std::getline(file, line)){
		size_t pos = 0;

		while(pos < line.size()){
			if(!stateMachine(line, pos)){
				file.close();
				fprintf(stderr, "Error reading line number %ld\n", line_id);

				return false;
			}
		}

		line_id++;
	}

	file.close();

	if(next_state != SEARCH_FOR_END){
		fprintf(stderr, "Error processing the end of the file.\n");
		return false;
	}

	return true;
}

bool JsonObject::saveFile(const std::string& filename){
	std::ofstream file(filename);

	if(!file.is_open())
		return false;

	writeJsonObject(file, this, "");

	file.close();

	return true;
}

JsonType& JsonObject::get(const std::string& key){
	return dictionary[key];
}

JsonType& JsonObject::get(size_t i){
	return json_array[i];
}

const std::unordered_set<std::string>& JsonObject::getKeys(void){
	return keys;
}

bool JsonObject::has(const std::string& key){
	return dictionary.find(key) != dictionary.end();
}

bool JsonObject::has(size_t i){
	return i < json_array.size();
}

void JsonObject::set(const char *key, const char *value){
	dictionary[std::string(key)] = JsonType(std::string(value));
	keys.insert(std::string(key));
}

void JsonObject::set(const std::string& key, std::string value){
	dictionary[key] = JsonType(value);
	keys.insert(key);
}

void JsonObject::set(const std::string& key, bool value){
	dictionary[key] = JsonType(value);
	keys.insert(key);
}

void JsonObject::set(const std::string& key, JsonObject *object){
	dictionary[key] = JsonType(object);
	keys.insert(key);
}

void JsonObject::set(const std::string& key, double number){
	dictionary[key] = JsonType(number);
	keys.insert(key);
}

void JsonObject::pushArray(const char *value){
	json_array.push_back(JsonType(std::string(value)));
}

void JsonObject::pushArray(const std::string& value){
	json_array.push_back(JsonType(value));
}

void JsonObject::pushArray(bool value){
	json_array.push_back(JsonType(value));
}

void JsonObject::pushArray(JsonObject *object){
	json_array.push_back(JsonType(object));
}

void JsonObject::pushArray(double number){
	json_array.push_back(JsonType(number));
}

bool JsonObject::isArray(void){
	return is_array;
}

void JsonObject::setAsArray(void){
	is_array = true;
}

size_t JsonObject::size(void){
	if(isArray())
		return json_array.size();
	else
		return keys.size();
}

bool JsonObject::writeJsonObject(std::ofstream& stream, JsonObject *object, const std::string& tabs){
	if(object->isArray())
		stream << "[\n";
	else
		stream << "{\n";

	if(!object->isArray()){
		size_t index = 0;

		for(const auto& i : object->getKeys()){
			writeJsonType(stream, i, object->get(i), tabs + "\t");

			index++;

			if(index != object->size())
				stream << ',';

			stream << "\n";
		}
	}
	else{
		for(size_t i = 0; i < object->size(); i++){
			writeJsonType(stream, "", object->get(i), tabs + "\t");

			if(i != object->size() - 1)
				stream << ",";

			stream << "\n";
		}
	}

	if(object->isArray())
		stream << tabs << "]";
	else
		stream << tabs << "}";

	return true;
}

bool JsonObject::writeJsonType(std::ofstream& stream, const std::string& key, JsonType& type, const std::string& tabs){
	std::string output;

	stream << tabs;

	if(key != ""){
		stream << '"' << key << '"' << ": ";
	}

	switch(type.type){
		case JSON_TYPE_NUMBER:
			stream << type.getNumber();
			break;

		case JSON_TYPE_BOOLEAN:
			if(type.getBoolean())
				stream << "true";
			else
				stream << "false";

			break;

		case JSON_TYPE_STRING:
			stream << '"' << type.getString() << '"';
			break;

		case JSON_TYPE_NULL:
			stream << "null";
			break;

		case JSON_TYPE_OBJECT:
			writeJsonObject(stream, type.getObject(), tabs + '\t');
				
			break;
	}

	return true;
}

bool JsonObject::stateMachine(const std::string& line, size_t& pos){
	char c = line[pos];

	switch(state){
		case SEARCH_OPEN_BRACKET:
			if(isWhiteSpace(c)){
				pos++;
			}
			else if(c == '{'){
				next_state = STRING_SEARCH_QUOTE;
				current_key = "";
				token_type = TOKEN_KEY;
				pos++;
			}
			else{
				fprintf(stderr, "Expected character '{' at line: %s\n", line.c_str());
				return false;
			}

			break;

		case SEARCH_OPEN_SQUARE:
			if(isWhiteSpace(c)){
				pos++;
			}
			else if(c == '['){
				next_state = SEARCH_VALUE;
				pos++;
			}
			else{
				fprintf(stderr, "Expected character '[' at line: %s\n", line.c_str());
				return false;
			}

			break;

		case SEARCH_FOR_END:
			if(isWhiteSpace(c)){
				pos++;
			}
			else if(c == '}'){
				if(object_stack.top()->isArray()){
					fprintf(stderr, "Expected character ']' at line: %s\n", line.c_str());
					return false;
				}

				object_stack.pop();
				pos++;
			}
			else if(c == ']'){
				if(!object_stack.top()->isArray()){
					fprintf(stderr, "Expected character '}' at line: %s\n", line.c_str());
					return false;
				}

				object_stack.pop();
				pos++;
			}
			else if(c == ','){
				next_state = STRING_SEARCH_QUOTE;

				if(object_stack.top()->isArray()){
					token_type = TOKEN_STRING;
					next_state = SEARCH_VALUE;
				}
				else{
					token_type = TOKEN_KEY;
					next_state = STRING_SEARCH_QUOTE;
				}

				pos++;
			}
			else{
				if(object_stack.top()->isArray())
					fprintf(stderr, "Expected character ',' or character ']' at line: %s\n", line.c_str());
				else
					fprintf(stderr, "Expected character ',' or character '}' at line: %s\n", line.c_str());

				return false;
			}

			break;

		case SEARCH_COLON:
			if(isWhiteSpace(c)){
				pos++;
			}
			else if(c == ':'){
				next_state = SEARCH_VALUE;
				pos++;
			}
			else{
				fprintf(stderr, "Expected character ':' at line: %s\n", line.c_str());
				return false;
			}
			break;

		case SEARCH_VALUE:
			if(isWhiteSpace(c)){
				pos++;
			}
			else if(c == '\"'){
				token_type = TOKEN_STRING;
				next_state = STRING_SEARCH_QUOTE;
			}
			else if(c == '{'){
				JsonObject *object = new JsonObject();

				if(object_stack.top()->isArray())
					object_stack.top()->pushArray(object);
				else
					object_stack.top()->set(current_key, object);

				object_stack.push(object);

				next_state = SEARCH_OPEN_BRACKET;
			}
			else if(c == '['){
				JsonObject *object = new JsonObject();
				object->setAsArray();

				if(object_stack.top()->isArray())
					object_stack.top()->pushArray(object);
				else
					object_stack.top()->set(current_key, object);

				object_stack.push(object);

				next_state = SEARCH_OPEN_SQUARE;
			}
			else if(c == ']' && object_stack.top()->isArray()){
				next_state = SEARCH_FOR_END;
			}
			else if(isdigit(c) || c == '-'){
				current_token = "";
				next_state = NUMBER_READ;
			}
			else if(line.substr(pos, 4) == "true"){
				if(object_stack.top()->isArray())
					object_stack.top()->pushArray(true);
				else
					object_stack.top()->set(current_key, true);

				next_state = SEARCH_FOR_END;
				pos += 4;
			}
			else if(line.substr(pos, 5) == "false"){
				if(object_stack.top()->isArray())
					object_stack.top()->pushArray(false);
				else
					object_stack.top()->set(current_key, false);

				next_state = SEARCH_FOR_END;
				pos += 5;
			}
			else if(line.substr(pos, 4) == "null"){
				next_state = SEARCH_FOR_END;
				pos += 4;
			}
			else{
				fprintf(stderr, "Couldn't find any value at line: %s\n", line.c_str());
				return false;
			}

			break;

		case STRING_SEARCH_QUOTE:
			if(isWhiteSpace(c)){
				pos++;
			}
			else if(c == '\"'){
				next_state = STRING_READ_ON;
				current_token = "";
				pos++;
			}
			else if(c == '}'){
				next_state = SEARCH_FOR_END;
			}
			else{
				std::cout << "Expected character '\"' at line: " << line << '\n';
				return false;
			}

			break;

		case STRING_READ_ON:
			if(c == '\"'){
				if(token_type == TOKEN_KEY){
					current_key = current_token;
					next_state = SEARCH_COLON;
				}
				else if(token_type == TOKEN_STRING){
					if(object_stack.top()->isArray())
						object_stack.top()->pushArray(current_token);
					else
						object_stack.top()->set(current_key, current_token);

					next_state = SEARCH_FOR_END;
				}
			}
			else{
				current_token += c;
			}

			pos++;

			break;

		case NUMBER_READ:
			if(isdigit(c) || c == '-' || c == '.' || c == 'E' || c == 'e' || c == '+'){
				current_token += c;
				pos++;
			}
			else{
				next_state = SEARCH_FOR_END;

				if(object_stack.top()->isArray())
					object_stack.top()->pushArray(std::stod(current_token));
				else
					object_stack.top()->set(current_key, std::stod(current_token));
			}

			break;
	}

	state = next_state;
	
	return true;
}

bool JsonObject::isWhiteSpace(char c){
	return (isspace(c) || c == '\n' || c == '\t' || c == '\r');
}

};
