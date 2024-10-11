#ifndef ANIMATOR_HPP
#define ANIMATOR_HPP

#include <cstddef>
#include <cstdint>

namespace acc {

class Animator {
	public:
		Animator(void);
		void update(uint64_t current_tick);
		void setAnimation(const int *current_animation, uint64_t frame_duration);
		int getCurrentId(void);

	private:
		uint64_t next_tick_frame;
		uint64_t frame_duration;

		int current_frame;
		const int *current_animation;
};

};

#endif
