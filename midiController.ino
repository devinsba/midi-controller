#include <Bounce.h>

void muxSelect(char addr);

const uint8_t CHANNEL = 1;

const uint8_t MUX_BIT0 = 2;
const uint8_t MUX_BIT1 = 3;
const uint8_t MUX_BIT2 = 4;

const uint8_t MUX1_PIN = 5;
const uint8_t MUX2_PIN = 6;
const uint8_t MUX3_PIN = A0;
const uint8_t MUX4_PIN = A1;

const uint8_t notes[] = {60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75};

uint8_t pots[8];
const uint8_t potControls[] = {80, 81, 82, 83, 84, 85, 86, 87};

uint8_t slides[4];
const uint8_t slideControls[] = {88, 89, 90, 91};

Bounce *buttons;

void setup() {
	int i = 0;
	pinMode(MUX_BIT0, OUTPUT);
	pinMode(MUX_BIT1, OUTPUT);
	pinMode(MUX_BIT2, OUTPUT);

	pinMode(MUX1_PIN, INPUT_PULLUP);
	pinMode(MUX2_PIN, INPUT_PULLUP);

	// Setup the buttons
	buttons = (Bounce *) calloc(16, sizeof(Bounce));
	for (i = 0; i < 8; i++) {
		buttons[i] = Bounce(MUX1_PIN, 5);
		buttons[i + 8] = Bounce(MUX2_PIN, 5);
	}
}

void loop() {
	char i = 0;
	
	// Read the buttons
	for (i = 0; i < 8; i++) {
		muxSelect(i);
		buttons[i].update();
		buttons[i + 8].update();
		pots[i] = analogRead(MUX3_PIN);
		if (i < 4) {
			slides[i] = analogRead(MUX4_PIN);
		}
	}

	// Handle the buttons
	for (i = 0; i < 16; i++) {
		if (buttons[i].fallingEdge()) {
			usbMIDI.sendNoteOn(notes[i], 99, CHANNEL);
		}
		if (buttons[i].risingEdge()) {
			usbMIDI.sendNoteOff(notes[i], 0, CHANNEL);
		}
	}
	for (i = 0; i < 8; i++) {
		usbMIDI.sendControlChange(potControls[i], pots[i], CHANNEL);
	}
	for (i = 0; i < 4; i++) {
		usbMIDI.sendControlChange(slideControls[i], slides[i], CHANNEL);
	}
}

void muxSelect(char addr) {
	digitalWrite(MUX_BIT0, addr & 1);
	digitalWrite(MUX_BIT1, (addr >> 1) & 1);
	digitalWrite(MUX_BIT2, (addr >> 2) & 1);
	delayMicroseconds(5);
}
