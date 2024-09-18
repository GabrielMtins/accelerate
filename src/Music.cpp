#include "Music.hpp"

namespace acc {

Music::Music(std::string filename){
	setName(filename);

	music = Mix_LoadMUS(filename.c_str());

	if(music == NULL)
		fprintf(stderr, "Failed to load music: %s\n", filename.c_str());
}

void Music::play(int loops){
	if(music == NULL) return;

	Mix_PlayMusic(music, loops);
}

void Music::setVolume(int volume){
	Mix_VolumeMusic(volume);
}

void Music::fadeOutMusic(int ms){
	Mix_FadeOutMusic(ms);
}

Music::~Music(void){
	if(music != NULL)
		Mix_FreeMusic(music);
}

};
