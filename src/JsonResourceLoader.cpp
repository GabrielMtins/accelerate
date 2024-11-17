#include "Util/JsonResourceLoader.hpp"

namespace acc {

bool JsonResourceLoader::load(Game *game, std::string filename){
	JsonObject *main_object;
	bool sucess;
	
	main_object = new JsonObject();

	sucess = main_object->parseFile(filename);

	if(!sucess){
		delete main_object;
		return false;
	}

	loadTextures(game, main_object);
	loadCanvases(game, main_object);
	loadSfxs(game, main_object);
	loadMusics(game, main_object);
	loadFonts(game, main_object);

	delete main_object;

	return true;
}

bool JsonResourceLoader::loadTextures(Game *game, JsonObject *main_object){
	JsonObject *texture_array, *current_texture;
	Context *context = game->getContext();
	std::string filename;
	int cell_width, cell_height;

	if(!main_object->has("textures"))
		return false;

	texture_array = main_object->get("textures").getObject();

	for(size_t i = 0; i < texture_array->size(); i++){
		if(!texture_array->get(i).isObject())
			continue;

		current_texture = texture_array->get(i).getObject();
		filename = "";
		cell_width = -1;
		cell_height = -1;

		if(current_texture->has("filename") && current_texture->get("filename").isString()){
			filename = current_texture->get("filename").getString();
		}

		if(current_texture->has("cell_width") && current_texture->get("cell_width").isNumber()){
			cell_width = (int) current_texture->get("cell_width").getNumber();
		}

		if(current_texture->has("cell_height") && current_texture->get("cell_height").isNumber()){
			cell_height = (int) current_texture->get("cell_height").getNumber();
		}

		if(filename != ""){
			game->loadResource((Resource *) context->getRenderer()->loadTexture(context, filename, cell_width, cell_height));
		}
	}

	return true;
}

bool JsonResourceLoader::loadCanvases(Game *game, JsonObject *main_object){
	JsonObject *canvas_array;
	std::string filename;

	if(!main_object->has("canvases"))
		return false;

	canvas_array = main_object->get("canvases").getObject();

	for(size_t i = 0; i < canvas_array->size(); i++){
		if(!canvas_array->get(i).isString())
			continue;

		game->loadResource((Resource *) new Canvas(canvas_array->get(i).getString()));
	}

	return true;
}

bool JsonResourceLoader::loadSfxs(Game *game, JsonObject *main_object){
	JsonObject *array;
	std::string filename;

	if(!main_object->has("sfxs"))
		return false;

	array = main_object->get("sfxs").getObject();

	for(size_t i = 0; i < array->size(); i++){
		if(!array->get(i).isString())
			continue;

		game->loadResource((Resource *) new Sfx(array->get(i).getString()));
	}

	return true;
}

bool JsonResourceLoader::loadMusics(Game *game, JsonObject *main_object){
	JsonObject *array;
	std::string filename;

	if(!main_object->has("musics"))
		return false;

	array = main_object->get("musics").getObject();

	for(size_t i = 0; i < array->size(); i++){
		if(!array->get(i).isString())
			continue;

		game->loadResource((Resource *) new Music(array->get(i).getString()));
	}

	return true;
}

bool JsonResourceLoader::loadFonts(Game *game, JsonObject *main_object){
	JsonObject *array, *current;
	std::string filename;
	size_t font_size;

	if(!main_object->has("fonts"))
		return false;

	array = main_object->get("fonts").getObject();

	for(size_t i = 0; i < array->size(); i++){
		if(!array->get(i).isObject())
			continue;

		current = array->get(i).getObject();
		filename = "";
		font_size = 0;

		if(current->has("filename") && current->get("filename").isString()){
			filename = current->get("filename").getString();
		}

		if(current->has("font_size") && current->get("font_size").isNumber()){
			font_size = (size_t) current->get("font_size").getNumber();
		}

		if(filename != "" && font_size != 0)
			game->loadResource((Resource *) new Font(filename, font_size));
	}

	return true;
}


};
