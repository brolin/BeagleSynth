#include "maximilian.h"

osc mySine;//let's create an oscillator and give it a name.

extern int channels=2;//stereo-must be supported by hardware
extern int buffersize=256;//should be fine for most things
extern int samplerate=44100;//SR must be supported by card. It's always the default

void setup() {//some inits
	//nothing to go here this time	
}

void play(double *output) {//this is where the magic happens. Very slow magic.
	
	*output=mySine.sinewave(440);//simple as that! 

}

