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

/**
 * This struct represents a generic type for json.
 * The type can be a: number, boolean, null, string, objects and arrays.
 * Objects and Arrays are represented by the same class JsonObject.
 */
struct JsonType {
	/**
	 * Creates a NULL type.
	 */
	JsonType(void);
	/**
	 * Creates a number type.
	 */
	JsonType(double number_value);
	/**
	 * Creates a boolean type.
	 */
	JsonType(bool boolean_value);
	/**
	 * Creates a string type.
	 */
	JsonType(std::string string_value);
	/**
	 * Creates a object type.
	 */
	JsonType(JsonObject *object_value);

	/**
	 * Returns a number as a double. It doesn't do casting.
	 */
	double getNumber(void);
	/**
	 * Returns a boolean value. It doesn't do casting.
	 */
	bool getBoolean(void);
	/**
	 * Returns a string value. It doesn't do casting.
	 */
	std::string getString(void);
	/**
	 * Returns a object value. It can be either an object or an array.
	 * It doesn't do casting.
	 */
	JsonObject * getObject(void);

	/**
	 * Returns true if the type is a number.
	 */
	bool isNumber(void);
	/**
	 * Returns true if the type is a boolean.
	 */
	bool isBoolean(void);
	/**
	 * Returns bool if the type is a string.
	 */
	bool isString(void);
	/**
	 * Returns bool if the type is an object or an array.
	 */
	bool isObject(void);

	/**
	 * A union data to manage the differents json types.
	 */
	union {
		double number_value;
		bool boolean_value;
		std::string *string_value;
		JsonObject *object_value;
	} data;

	/**
	 * The type of the JsonType.
	 */
	int type;
};

class JsonObject : public Resource {
	public:
		/**
		 * Creates an empty JsonObject.
		 */
		JsonObject(void);
		/**
		 * Loads a JsonObject from a given file name.
		 */
		JsonObject(std::string filename);
		~JsonObject(void);
		/**
		 * Parses a given file. Returns false if any error.
		 */
		bool parseFile(std::string filename);

		/**
		 * Returns the type of a given key. It works when
		 * it is an object.
		 */
		JsonType& get(std::string key);

		/**
		 * Returns the type for a given index. It works
		 * when it is an array.
		 */
		JsonType& get(size_t i);

		/**
		 * Returns true if the object has that key.
		 */
		bool has(std::string key);
		/**
		 * Returns true if the object has that key.
		 */
		bool has(size_t i);

		void set(std::string key, std::string value);
		void set(std::string key, bool value);
		void set(std::string key, JsonObject *object);
		void set(std::string key, double number);

		void pushArray(std::string value);
		void pushArray(bool value);
		void pushArray(JsonObject *object);
		void pushArray(double number);

		/**
		 * Returns true if the object is an array.
		 */
		bool isArray(void);
		void setAsArray(void);
		/**
		 * Returns the number of elements inside
		 * the object.
		 */
		size_t size(void);
	
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
