#pragma once

#define TOUCH_TIMEOUT	1000
#define TAP_MAX_THRESHOLD	150
#define TAP_MIN_THRESHOLD 50

enum TouchType{
	Tap = 0,
	SwipeForward = 1,
	SwipeBackward = 2
};

enum TouchState{
	Idle = 0,
	OnePress = 1,
	TwoPresses = 2,
	OneReleaseNoPress = 3,
	OneReleaseOnePress = 4,
	TwoReleases = 5
};

enum TouchState{
	NoTouch = 0,
	FirstTouch = 1,
	SecondTouch = 2
};

typedef struct TouchDetector{
	Touch touches[2];
	uint8_t touch_state;
	uint8_t in_touch;
} TouchDetector;

typedef struct Touch{
	uint8_t cap_index;
	uint8_t in_action;
	uint32_t start_t;	//ms
	uint32_t end_t;	//ms
} CapSensor;

void process_touches(TouchDetector *self);

