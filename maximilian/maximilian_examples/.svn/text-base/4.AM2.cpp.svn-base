#include "maximilian.h"

//This shows how to use maximilian to do basic amplitude modulation

osc mySine,myOtherSine,myPhasor;//Three oscillators. They can be called anything. They can be any of the available waveforms.


extern int channels=2;//stereo-must be supported by hardware
extern int buffersize=256;//should be fine for most things
extern int samplerate=44100;//SR must be supported by card. It's always the default

void setup() {//some inits
	//nothing to go here this time	
}

void play(double *output) {
	
	*output=mySine.sinewave(440)*myOtherSine.sinewave(myPhasor.phasor(0.1,0,440));
	
}
