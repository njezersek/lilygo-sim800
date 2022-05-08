#pragma once
#include <Arduino.h>

namespace gsm {
	void init();
	void restart();
	void update();
	void sendSMS(String number, String message);
	void sendCommand(String command);
}