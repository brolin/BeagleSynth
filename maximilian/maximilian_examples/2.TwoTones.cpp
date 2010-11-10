#include "maximilian.h"

osc mySine,myOtherSine;//Two oscillators with names.

extern int channels=2;//stereo-must be supported by hardware
extern int buffersize=256;//should be fine for most things
extern int samplerate=44100;//SR must be supported by card. It's always the default

void setup() {//some inits
	//nothing to go here this time	
}

void play(double *output) {//this is where the magic happens. Very slow magic.
	
	*output=mySine.sinewave(440)+myOtherSine.sinewave(441);//these two sines will beat together. They're now a bit too loud though..
	
}

