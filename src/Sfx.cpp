#include "Sfx.hpp"

namespace acc {

Sfx::Sfx(const std::string& filename){
	setName(filename);
	
	chunk = Mix_LoadWAV(filename.c_str());

	if(chunk == NULL)
		fprintf(stderr, "Failed to load sound effect: %s\n", filename.c_str());
}

void Sfx::play(int loops, int channel){
	if(chunk == NULL)
		return;

	Mix_PlayChannel(channel, chunk, loops);
}

void Sfx::setVolume(int volume){
	Mix_VolumeChunk(chunk, volume);
}

Sfx::~Sfx(void){
	if(chunk != NULL)
		Mix_FreeChunk(chunk);
}

};
