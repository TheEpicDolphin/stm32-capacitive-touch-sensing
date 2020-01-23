#include "touch_detector.h"

void process_touches(TouchDetector *self){

}

void debounce(Debouncer *self, uint8_t noisy_in, uint32_t cur_time){
	if(noisy_in != self->input){
		self->input = noisy_in;
		self->start_t = cur_time;
	}
	else if(cur_time - self->start_t > DEBOUNCE_TIME){
		self->clean_out = self->input;
	}
}
