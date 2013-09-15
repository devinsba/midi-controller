#pragma once
#ifndef _POT_H_
#define _POT_H_

#include <Arduino.h>

class Pot {
private:
	uint8_t myPin;
	uint16_t fullValue;
	uint8_t lastValue;
	bool changed;

	Pot() {}

public:
	Pot(uint8_t pin);

	bool update();
	bool hasChanged();
	uint8_t getValue();
};

#endif
