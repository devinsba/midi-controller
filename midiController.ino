#include <Bounce.h>
#include "pot.h"

//#define DEBUG
#ifdef DEBUG
#define print Serial.println
#else
#define print
#endif

#define BUTTONS
#define POTS 4
//#define SLIDES 4

void muxSelect(char addr);

const uint8_t CHANNEL = 1;

const uint8_t MUX_BIT0 = 2;
const uint8_t MUX_BIT1 = 3;
const uint8_t MUX_BIT2 = 4;

const uint8_t MUX1_PIN = 5;
const uint8_t MUX2_PIN = 6;
const uint8_t MUX3_PIN = A0;
const uint8_t MUX4_PIN = A1;

const uint8_t notes[] = {48, 49, 50, 51, 44, 45, 46, 47, 40, 41, 42, 43, 36, 37, 38, 39};
const uint8_t potControls[] = {14, 15, 16, 17, 18, 19, 20, 21};
const uint8_t slideControls[] = {2, 3, 4, 5};

Bounce *buttons;
Pot *pots;
Pot *slides;

char i = 0;

void setup() {
#ifdef DEBUG
	Serial.begin(9600);
#endif
	char j = 0;
	pinMode(MUX_BIT0, OUTPUT);
	pinMode(MUX_BIT1, OUTPUT);
	pinMode(MUX_BIT2, OUTPUT);

#ifdef BUTTONS
	pinMode(MUX1_PIN, INPUT_PULLUP);
	pinMode(MUX2_PIN, INPUT_PULLUP);

	// Setup the buttons
	buttons = (Bounce *) calloc(16, sizeof(Bounce));
	for (j = 0; j < 8; j++) {
		buttons[j] = Bounce(MUX1_PIN, 5);
		buttons[j + 8] = Bounce(MUX2_PIN, 5);
	}
#endif

#ifdef POTS
	pinMode(MUX3_PIN, INPUT_PULLUP);

	pots = (Pot *) calloc(POTS, sizeof(Pot));
	for (j = 0; j < POTS; j++) {
		pots[i] = Pot(MUX3_PIN);
	}
#endif

#ifdef SLIDES
	pinMode(MUX4_PIN, INPUT_PULLUP);

	slides = (Pot *) calloc(SLIDES, sizeof(Pot));
	for (j = 0; j < SLIDES; j++) {
		slides[i] = Pot(MUX4_PIN);
	}
#endif
}

void loop() {
	// Read the buttons
	for (i = 0; i < 8; i++) {
		muxSelect(i);
#ifdef BUTTONS
		buttons[i].update();
		buttons[i + 8].update();
#endif
#ifdef POTS
		if (i < POTS) {
			pots[i].update();
		}
#endif
#ifdef SLIDES
		if (i < SLIDES) {
			slides[i].update();
		}
#endif
	}

#ifdef BUTTONS
	// Handle the buttons
	for (i = 0; i < 16; i++) {
		if (buttons[i].fallingEdge()) {
			print("BUTTON DOWN");
			print(int(notes[i]));
			usbMIDI.sendNoteOn(notes[i], 99, CHANNEL);
		}
		if (buttons[i].risingEdge()) {
			print("BUTTON UP");
			print(int(notes[i]));
			usbMIDI.sendNoteOff(notes[i], 0, CHANNEL);
		}
	}
#endif
#ifdef POTS
	for (i = 0; i < POTS; i++) {
		if (pots[i].hasChanged()) {
			usbMIDI.sendControlChange(
					potControls[i],
					pots[i].getValue(),
					CHANNEL);
		}
	}
#endif
#ifdef SLIDES
	if (pots[i].hasChanged()) {
			usbMIDI.sendControlChange(
					slideControls[i],
					slides[i].getValue(),
					CHANNEL);
		}
#endif
	usbMIDI.send_now();
}

void muxSelect(char addr) {
	digitalWrite(MUX_BIT0, addr & 1);
	digitalWrite(MUX_BIT1, (addr >> 1) & 1);
	digitalWrite(MUX_BIT2, (addr >> 2) & 1);
	delayMicroseconds(15);
}
