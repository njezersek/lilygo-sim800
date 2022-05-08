#pragma once
#include <Arduino.h>

namespace relay {
	extern uint8_t pins[];
	extern bool state[];
	void init();
	void update();
	void set(int i, bool state);
}