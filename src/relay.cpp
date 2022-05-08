#include "relay.h"

#define RELAY1_PIN GPIO_NUM_15
#define RELAY2_PIN GPIO_NUM_2
#define RELAY3_PIN GPIO_NUM_18
#define RELAY4_PIN GPIO_NUM_19

uint8_t relay::pins[4] = {RELAY1_PIN, RELAY2_PIN, RELAY3_PIN, RELAY4_PIN};
bool relay::state[4] = {0, 0, 0, 0};

void relay::init() {
	for (int i = 0; i < 4; i++) {
		pinMode(pins[i], OUTPUT);
		digitalWrite(pins[i], !state[i]);
	}
}

void relay::update() {
	for (int i = 0; i < 4; i++) {
		digitalWrite(pins[i], !state[i]);
	}
}

void relay::set(int key, bool val) {
	if(key < 0 || key > 3) return;
	state[key] = val;
	update();
}