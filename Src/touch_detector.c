#include "touch_detector.h"

void process_touches(TouchDetector *self, Debouncer *dbs, uint32_t cur_time, uint8_t cap_sensor){
    if(self->touch_state == Idle){
    	if(dbs[cap_sensor].clean_out){
    		self->touch_state = OnePressed;
    		self->touches[0].cap_idx = cap_sensor;
    		self->touches[0].start_t = cur_time;
    	}
    }
    else if(self->touch_state == OnePressed){
    	if(dbs[cap_sensor].clean_out && self->touches[0].cap_idx != cap_sensor){
    		self->touch_state = TwoPressed;
    		self->touches[1].cap_idx = cap_sensor;
    		self->touches[1].start_t = cur_time;
    	}
    	else if(!dbs[cap_sensor].clean_out && self->touches[0].cap_idx == cap_sensor){
    		self->touch_state = OneReleasedNoPressed;
    		self->touches[0].end_t = cur_time;

    	}
    }
    else if(self->touch_state == TwoPressed){
    	if(!dbs[cap_sensor].clean_out){
    		self->touch_state = OneReleasedOnePressed;
    		if(self->touches[0].cap_idx == cap_sensor){
    			self->touches[0].end_t = cur_time;
    		}
    		else{
    			self->touches[1].end_t = cur_time;
    		}
    	}
    }
    else if(self->touch_state == OneReleasedNoPressed){
    	if(dbs[cap_sensor].clean_out){
    		self->touch_state = OneReleasedOnePressed;
    		self->touches[1].cap_idx = cap_sensor;
    		self->touches[1].start_t = cur_time;
    	}
    	else if(cur_time - self->touches[0].end_t > MAX_SWIPE_GAP){
    		uint32_t touch_duration = self->touches[0].end_t - self->touches[0].start_t;
			if(TAP_MIN_THRESHOLD < touch_duration && touch_duration < TAP_MAX_THRESHOLD){
				//Register a tap
				enum TouchType action = Tap;

			}
			self->touch_state = Idle;
    	}
    }
    else if(self->touch_state == OneReleasedOnePressed){
    	if(!dbs[cap_sensor].clean_out){
    		self->touch_state = Idle;
    		if(self->touches[0].cap_idx == cap_sensor){
    			self->touches[0].end_t = cur_time;
    		}
    		else{
    			self->touches[1].end_t = cur_time;
    		}

    		enum TouchType action = None;
    		uint32_t touches_start_diff = self->touches[1].start_t - self->touches[0].start_t;
    		uint32_t touches_end_diff;
    		uint32_t touch_duration;
    		if(self->touches[1].end_t >= self->touches[0].end_t){
    			touches_end_diff = self->touches[1].end_t - self->touches[0].end_t;
    			touch_duration = self->touches[1].end_t - self->touches[0].start_t;
    		}
    		else{
    			touches_end_diff = self->touches[0].end_t - self->touches[1].end_t;
    			touch_duration = self->touches[0].end_t - self->touches[0].start_t;
    		}

    		if((touches_start_diff <= TAP_MIN_THRESHOLD) && (touches_end_diff <= TAP_MIN_THRESHOLD) &&
    				(TAP_MIN_THRESHOLD < touch_duration) && (touch_duration < TAP_MAX_THRESHOLD)){
    			//Register a tap
    			action = Tap;
    		}
    		else if(touches_start_diff > TAP_MIN_THRESHOLD){
    			//Checks if the touches started on one sensor and ended on the other, generating a swipe
    			if(self->touches[1].end_t >= self->touches[0].end_t){
    				if(touches_end_diff > TAP_MIN_THRESHOLD){
    					//Register a swipe
    					action = (self->touches[0].cap_idx << 1) | self->touches[1].cap_idx;
    				}
    			}
    		}
    		self->touch_state = Idle;
    	}
    }
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
