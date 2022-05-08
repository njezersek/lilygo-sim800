
#include <Arduino.h>
#include "utilities.h"
#include "relay.h"
#include "gsm.h"


void setup(){
	relay::init();
	gsm::init();
}

void loop(){
	gsm::update();
}