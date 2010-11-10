#include "maximilian.h"

osc myCounter,mySwitchableOsc;//
int CurrentCount;//
double myOscOutput,myFilteredOutput;//
double myEnvelopeData[6] = {500,0,1000,500,0,500};//this data will be used to make an envelope. Value and time to value in ms.
envelope myEnvelope;
filter myFilter;

extern int channels=2;//stereo-must be supported by hardware
extern int buffersize=256;//should be fine for most things
extern int samplerate=44100;//SR must be supported by card. It's always the default

void setup() {//some inits
	myEnvelope.amplitude=myEnvelopeData[0]; //initialise the envelope
}

void play(double *output) {
	
	CurrentCount=myCounter.phasor(1, 1, 9);//phasor can take three arguments; frequency, start value and end value.
	
	if (CurrentCount<5)//simple if statement
		
		myOscOutput=mySwitchableOsc.square(CurrentCount*100);
	
	else if (CurrentCount>=5)//and the 'else' bit.
		
		myOscOutput=mySwitchableOsc.saw(CurrentCount*50);//one osc object can produce whichever waveform you want. 
	
	if (CurrentCount==1) 
		
		myEnvelope.trigger(0,myEnvelopeData[0]); //trigger the envelope
	
	myFilteredOutput=myFilter.lores(myOscOutput,(myEnvelope.line(6, myEnvelopeData)),10);//lores takes an audio input, a frequency and a resonance factor (1-100)
	
	*output=myFilteredOutput;//point me at your speakers and fire.
}
