#include "maximilian.h"

osc mySine,myOtherSine,myLastSine,myPhasor;//Three oscillators


extern int channels=2;//stereo-must be supported by hardware
extern int buffersize=256;//should be fine for most things
extern int samplerate=44100;//SR must be supported by card. It's always the default

void setup() {//some inits
	//nothing to go here this time	
}

void play(double *output) {
	
	*output=mySine.sinewave(myOtherSine.sinewave(myLastSine.sinewave(0.1)*30)*440);//awesome bassline
	
}
