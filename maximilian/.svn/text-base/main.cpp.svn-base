#include "maximilian.h"
#include <cstdlib>


double outputs[2],moreoutputs[2]; //some track outputs
double filtered, voices,temp;
int counter;
osc countme;
sample beats,beat;
delayline myDelay;
mix mymix,bobbins;//some panning busses

extern int channels=2;//stereo-must be supported by hardware
extern int buffersize=512;//should be fine for most things
extern int samplerate=44100;//SR must be supported by card. It's always the default

void setup() {//some inits
	beats.load("/Users/mick/Desktop/beat2.wav");
	beat.load("/Users/mick/Desktop/remoteatmos.wav");

}

void play(double *output) {//this is where the magic happens. Very slow magic.


	temp=beats.play(1.,0,beats.length());
	filtered=beat.play(1.);

	
//now we send the sounds to some stereo busses.
//	mymix.stereo(more+mixed+delayed, outputs, 1-pan);
	bobbins.stereo(temp+filtered, moreoutputs, 0.5);//invert the pan
		
	
	//mixing
	
	output[0]=moreoutputs[0];//stick it in the out!!
	output[1]=moreoutputs[1];
	
}

