#include "maximilian.h"

osc myCounter,mySwitchableOsc;//these oscillators will help us count and make sound.
int CurrentCount;//we're going to put the current count in this variable so that we can use it more easily.
double myOscOutput;//we're going to stick the output here to make it easier to mess with stuff.

extern int channels=2;//stereo-must be supported by hardware
extern int buffersize=256;//should be fine for most things
extern int samplerate=44100;//SR must be supported by card. It's always the default

void setup() {//some inits
	//nothing to go here this time	
}

void play(double *output) {
	
	CurrentCount=myCounter.phasor(1, 1, 9);//phasor can take three arguments; frequency, start value and end value.
	
	if (CurrentCount<5)//simple if statement
	
		myOscOutput=mySwitchableOsc.square(CurrentCount*100);
	
	else if (CurrentCount>=5)//and the 'else' bit.
		
		myOscOutput=mySwitchableOsc.sinewave(CurrentCount*50);//one osc object can produce whichever waveform you want. 
	
	*output=myOscOutput;//point me at your speakers and fire.
}
