#ifndef RESOURCE_HPP
#define RESOURCE_HPP

#include <string>
#include <unordered_map>

namespace acc {

class Resource {
	public:
		virtual ~Resource(void) = default;
		std::string getName(void) {return filename;};
		void setName(std::string filename) {this->filename = filename;};

	private:
		std::string filename;
	
};

class ResourceManager {
	public:
		ResourceManager(void);
		void loadResource(Resource *resource);
		Resource * getResource(std::string filename);
		~ResourceManager(void);

	private:
		std::unordered_map<std::string, Resource *> resources;
};

};

#endif
