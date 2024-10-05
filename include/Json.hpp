#ifndef JSON_HPP
#define JSON_HPP

#include <cstddef>
#include <unordered_map>
#include <vector>
#include <stack>
#include <string>
#include <cstdio>

#include "Resource.hpp"

namespace acc {

class JsonObject;

enum JSON_TYPE_ENUM {
	JSON_TYPE_NUMBER = 0,
	JSON_TYPE_BOOLEAN,
	JSON_TYPE_STRING,
	JSON_TYPE_NULL,
	JSON_TYPE_OBJECT
};

struct JsonType {
	JsonType(void);
	JsonType(double number_value);
	JsonType(bool boolean_value);
	JsonType(std::string string_value);
	JsonType(JsonObject *object_value);

	double getNumber(void);
	bool getBoolean(void);
	std::string getString(void);
	JsonObject * getObject(void);

	bool isNumber(void);
	bool isBoolean(void);
	bool isString(void);
	bool isObject(void);

	union {
		double number_value;
		bool boolean_value;
		std::string *string_value;
		JsonObject *object_value;
	} data;

	int type;
};

class JsonObject : public Resource {
	public:
		JsonObject(void);
		JsonObject(std::string filename);
		~JsonObject(void);
		bool parseFile(std::string filename);

		JsonType& get(std::string key);
		JsonType& get(size_t i);

		void set(std::string key, std::string value);
		void set(std::string key, bool value);
		void set(std::string key, JsonObject *object);
		void set(std::string key, double number);

		void pushArray(std::string value);
		void pushArray(bool value);
		void pushArray(JsonObject *object);
		void pushArray(double number);

		bool isArray(void);
		void setAsArray(void);
	
	private:
		bool stateMachine(const std::string& line, size_t& pos);
		bool isWhiteSpace(char c);

		bool is_array;
		int state, next_state;
		int token_type;

		std::string current_token;
		std::string current_key;

		std::unordered_map<std::string, JsonType> dictionary;
		std::vector<JsonType> json_array;

		std::stack<JsonObject *> object_stack;
};

};

#endif
