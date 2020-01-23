#pragma once
#include "stm32f3xx_hal.h"

#define TOUCH_TIMEOUT	1000
#define TAP_MAX_THRESHOLD	150
#define TAP_MIN_THRESHOLD	50
#define DEBOUNCE_TIME	30

enum TouchType{
	None = 0,
	SwipeForward = 1,
	SwipeBackward = 2,
	Tap = 3
};

enum TouchState{
	Idle = 0,
	OnePressed = 1,
	TwoPressed = 2,
	OneReleasedNoPressed = 3,
	OneReleasedOnePressed = 4,
	TwoReleased = 5
};

typedef struct Touch{
	uint8_t cap_idx;
	uint32_t start_t;	//ms
	uint32_t end_t;	//ms
} Touch;

typedef struct TouchDetector{
	Touch touches[2];
	uint8_t touch_state;
} TouchDetector;

typedef struct Debouncer{
	uint32_t start_t;
	uint8_t input;
	uint8_t clean_out;
} Debouncer;

void process_touches(TouchDetector *self);
void debounce(Debouncer *self, uint8_t noisy_in, uint32_t cur_time);

