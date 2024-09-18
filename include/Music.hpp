#ifndef MUSIC_HPP
#define MUSIC_HPP

#include "Context.hpp"
#include "Resource.hpp"

namespace acc {

class Music : public Resource{
	public:
		Music(std::string filename);
		void play(int loops);
		static void setVolume(int volume);
		static void fadeOutMusic(int ms);
		~Music(void);
	
	private:
		Mix_Music *music;
};
	
};

#endif
