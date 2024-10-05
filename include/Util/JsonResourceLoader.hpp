#ifndef JSON_RESOURCE_LOADER_HPP
#define JSON_RESOURCE_LOADER_HPP

#include "Game.hpp"
#include "Texture.hpp"
#include "Canvas.hpp"
#include "Sfx.hpp"
#include "Music.hpp"
#include "Font.hpp"
#include "Json.hpp"

namespace acc {

class JsonResourceLoader {
	public:
		static bool load(Game *game, std::string filename);

	private:
		static bool loadTextures(Game *game, JsonObject *main_object);
		static bool loadCanvases(Game *game, JsonObject *main_object);
		static bool loadSfxs(Game *game, JsonObject *main_object);
		static bool loadMusics(Game *game, JsonObject *main_object);
		static bool loadFonts(Game *game, JsonObject *main_object);
};

};

#endif
