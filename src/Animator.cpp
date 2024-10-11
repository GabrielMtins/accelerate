#include "Animator.hpp"

namespace acc {

Animator::Animator(void){
	current_animation = NULL;
}

void Animator::update(uint64_t current_tick){
	if(current_animation == NULL)
		return;

	if(current_tick < next_tick_frame)
		return;

	next_tick_frame = current_tick + frame_duration;
	current_frame++;

	if(current_animation[current_frame] == -1)
		current_frame = 0;
}

void Animator::setAnimation(const int *current_animation, uint64_t frame_duration){
	this->current_animation = current_animation;
	this->frame_duration = frame_duration;

	current_frame = 0;
	next_tick_frame = 0;
}

int Animator::getCurrentId(void){
	return current_animation[current_frame];
}

};
