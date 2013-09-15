#include "pot.h"

#include <stdint.h>

Pot::Pot(uint8_t pin) {
	changed = false;
	myPin = pin;
}

bool Pot::update() {
	uint16_t temp = analogRead(myPin);
	uint8_t small = 0;
	if (temp < fullValue - 4 || temp > fullValue + 4) {
		small = map(temp, 0, 1023, 0, 127);
		fullValue = temp;
		lastValue = small;
		changed = true;
	}
	return changed;
}

bool Pot::hasChanged() {
	return changed;
}

uint8_t Pot::getValue() {
	changed = false;
	return lastValue;
}
