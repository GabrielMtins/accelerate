#ifndef MUSIC_HPP
#define MUSIC_HPP

#include "Context.hpp"
#include "Resource.hpp"

namespace acc {

/**
 * This class is used to manage the loading and playing of music files.
 */
class Music : public Resource{
	public:
		/**
		 * Loads an music file from given filename.
		 */
		Music(const std::string& filename);
		/**
		 * Plays the music for a given number of loops. If loops = -1,
		 * the music will play until halted.
		 */
		void play(int loops);
		/**
		 * Sets the volume of the music.
		 */
		static void setVolume(int volume);
		/**
		 * Fades out the music in a given amount of milliseconds.
		 */
		static void fadeOutMusic(int ms);
		static void haltMusic(void);
		~Music(void);
	
	private:
		Mix_Music *music;
};
	
};

#endif
