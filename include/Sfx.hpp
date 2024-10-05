#ifndef SFX_HPP
#define SFX_HPP

#include "Context.hpp"
#include "Resource.hpp"

namespace acc {

/**
 * This class loads a sound effect. It should be used
 * for small sound effects, for other types of sounds like music
 * prefer to use the Music class.
 */
class Sfx : public Resource {
	public:
		Sfx(std::string filename);
		void play(int loops, int channel);
		~Sfx(void);

	private:
		Mix_Chunk *chunk;
};

};

#endif
