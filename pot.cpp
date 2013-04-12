#include "pot.h"

Pot::Pot(uint8_t pin) {
	myPin = pin;
}

bool Pot::update() {
	uint8_t temp = analogRead(myPin) / 8;
	if (temp != lastValue) {
		lastValue = temp;
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
