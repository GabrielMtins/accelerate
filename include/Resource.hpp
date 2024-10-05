#ifndef RESOURCE_HPP
#define RESOURCE_HPP

#include <string>
#include <unordered_map>

namespace acc {

/**
 * Base class for creating resources.
 * If you want to create a custom resource that the Game class can handle it,
 * inherent it from this class. Don't forget to use setName(), so the Game
 * class can store it properly. 
 * On this codebase, I tend to set the name of the resource as their filenames.
 */
class Resource {
	public:
		virtual ~Resource(void) = default;
		/**
		 * Returns a std::string containing the name of the Resource.
	 	 */
		std::string getName(void) {return filename;};
		/**
		 * Sets the name of the resource.
		 */
		void setName(std::string filename) {this->filename = filename;};

	private:
		std::string filename;
	
};

class ResourceManager {
	public:
		ResourceManager(void);
		void loadResource(Resource *resource);
		/**
		 * Returns NULL if it wasn't found a resource with name "filename".
		 */
		Resource * getResource(std::string filename);
		~ResourceManager(void);

	private:
		std::unordered_map<std::string, Resource *> resources;
};

};

#endif
