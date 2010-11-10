#include "maximilian.h"

osc myCounter,mySquare;//these oscillators will help us count and play sound
int CurrentCount;//we're going to put the current count in this variable so that we can use it more easily.


extern int channels=2;//stereo-must be supported by hardware
extern int buffersize=256;//should be fine for most things
extern int samplerate=44100;//SR must be supported by card. It's always the default

void setup() {//some inits
	//nothing to go here this time	
}

void play(double *output) {
	
	CurrentCount=myCounter.phasor(1, 1, 9);//phasor can take three arguments; frequency, start value and end value.
	*output=mySquare.square(CurrentCount*100);
}
