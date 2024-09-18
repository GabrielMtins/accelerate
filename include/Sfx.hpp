#ifndef SFX_HPP
#define SFX_HPP

#include "Context.hpp"
#include "Resource.hpp"

namespace acc {

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
