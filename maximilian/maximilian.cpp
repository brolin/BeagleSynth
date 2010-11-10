
/*
 *  maximilian.cpp
 *  platform independent synthesis library using portaudio or rtaudio
 *
 *  Created by Mick Grierson on 29/12/2009.
 *  Copyright 2009 Mick Grierson & Strangeloop Limited. All rights reserved.
 *	Thanks to the Goldsmiths Creative Computing Team.
 *	Special thanks to Arturo Castro for the PortAudio implementation.
 *
 *	Permission is hereby granted, free of charge, to any person
 *	obtaining a copy of this software and associated documentation
 *	files (the "Software"), to deal in the Software without
 *	restriction, including without limitation the rights to use,
 *	copy, modify, merge, publish, distribute, sublicense, and/or sell
 *	copies of the Software, and to permit persons to whom the
 *	Software is furnished to do so, subject to the following
 *	conditions:
 *	
 *	The above copyright notice and this permission notice shall be
 *	included in all copies or substantial portions of the Software.
 *
 *	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,	
 *	EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 *	OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 *	NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 *	HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 *	WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 *	FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 *	OTHER DEALINGS IN THE SOFTWARE.
 *
 */

#include "maximilian.h"
#include "math.h"

#ifdef MAXIMILIAN_PORTAUDIO
#include "portaudio.h"
//#include "pa_mac_core.h"
#elif defined(MAXIMILIAN_RT_AUDIO)
#include "RtAudio.h"
#endif

extern int channels;
extern int samplerate; 
extern int buffersize;
float chandiv= 1/(float) channels;

double sineBuffer[514]={0,0.012268,0.024536,0.036804,0.049042,0.06131,0.073547,0.085785,0.097992,0.1102,0.12241,0.13455,0.1467,0.15884,0.17093,0.18301,0.19507,0.20709,0.21909,0.23105,0.24295,0.25485,0.26669,0.2785,0.29025,0.30197,0.31366,0.32529,0.33685,0.34839,0.35986,0.37128,0.38266,0.39395,0.40521,0.41641,0.42752,0.4386,0.44958,0.46051,0.47137,0.48215,0.49286,0.50351,0.51407,0.52457,0.53497,0.54529,0.55554,0.5657,0.57578,0.58575,0.59567,0.60547,0.6152,0.62482,0.63437,0.6438,0.65314,0.66238,0.67151,0.68057,0.68951,0.69833,0.70706,0.7157,0.72421,0.7326,0.74091,0.74908,0.75717,0.76514,0.77298,0.7807,0.7883,0.79581,0.80316,0.81042,0.81754,0.82455,0.83142,0.8382,0.84482,0.85132,0.8577,0.86392,0.87006,0.87604,0.88187,0.8876,0.89319,0.89862,0.90396,0.90912,0.91415,0.91907,0.92383,0.92847,0.93295,0.93729,0.9415,0.94556,0.94949,0.95325,0.95691,0.96039,0.96375,0.96692,0.97,0.9729,0.97565,0.97827,0.98074,0.98306,0.98523,0.98724,0.98914,0.99084,0.99243,0.99387,0.99515,0.99628,0.99725,0.99808,0.99875,0.99927,0.99966,0.99988,0.99997,0.99988,0.99966,0.99927,0.99875,0.99808,0.99725,0.99628,0.99515,0.99387,0.99243,0.99084,0.98914,0.98724,0.98523,0.98306,0.98074,0.97827,0.97565,0.9729,0.97,0.96692,0.96375,0.96039,0.95691,0.95325,0.94949,0.94556,0.9415,0.93729,0.93295,0.92847,0.92383,0.91907,0.91415,0.90912,0.90396,0.89862,0.89319,0.8876,0.88187,0.87604,0.87006,0.86392,0.8577,0.85132,0.84482,0.8382,0.83142,0.82455,0.81754,0.81042,0.80316,0.79581,0.7883,0.7807,0.77298,0.76514,0.75717,0.74908,0.74091,0.7326,0.72421,0.7157,0.70706,0.69833,0.68951,0.68057,0.67151,0.66238,0.65314,0.6438,0.63437,0.62482,0.6152,0.60547,0.59567,0.58575,0.57578,0.5657,0.55554,0.54529,0.53497,0.52457,0.51407,0.50351,0.49286,0.48215,0.47137,0.46051,0.44958,0.4386,0.42752,0.41641,0.40521,0.39395,0.38266,0.37128,0.35986,0.34839,0.33685,0.32529,0.31366,0.30197,0.29025,0.2785,0.26669,0.25485,0.24295,0.23105,0.21909,0.20709,0.19507,0.18301,0.17093,0.15884,0.1467,0.13455,0.12241,0.1102,0.097992,0.085785,0.073547,0.06131,0.049042,0.036804,0.024536,0.012268,0,-0.012268,-0.024536,-0.036804,-0.049042,-0.06131,-0.073547,-0.085785,-0.097992,-0.1102,-0.12241,-0.13455,-0.1467,-0.15884,-0.17093,-0.18301,-0.19507,-0.20709,-0.21909,-0.23105,-0.24295,-0.25485,-0.26669,-0.2785,-0.29025,-0.30197,-0.31366,-0.32529,-0.33685,-0.34839,-0.35986,-0.37128,-0.38266,-0.39395,-0.40521,-0.41641,-0.42752,-0.4386,-0.44958,-0.46051,-0.47137,-0.48215,-0.49286,-0.50351,-0.51407,-0.52457,-0.53497,-0.54529,-0.55554,-0.5657,-0.57578,-0.58575,-0.59567,-0.60547,-0.6152,-0.62482,-0.63437,-0.6438,-0.65314,-0.66238,-0.67151,-0.68057,-0.68951,-0.69833,-0.70706,-0.7157,-0.72421,-0.7326,-0.74091,-0.74908,-0.75717,-0.76514,-0.77298,-0.7807,-0.7883,-0.79581,-0.80316,-0.81042,-0.81754,-0.82455,-0.83142,-0.8382,-0.84482,-0.85132,-0.8577,-0.86392,-0.87006,-0.87604,-0.88187,-0.8876,-0.89319,-0.89862,-0.90396,-0.90912,-0.91415,-0.91907,-0.92383,-0.92847,-0.93295,-0.93729,-0.9415,-0.94556,-0.94949,-0.95325,-0.95691,-0.96039,-0.96375,-0.96692,-0.97,-0.9729,-0.97565,-0.97827,-0.98074,-0.98306,-0.98523,-0.98724,-0.98914,-0.99084,-0.99243,-0.99387,-0.99515,-0.99628,-0.99725,-0.99808,-0.99875,-0.99927,-0.99966,-0.99988,-0.99997,-0.99988,-0.99966,-0.99927,-0.99875,-0.99808,-0.99725,-0.99628,-0.99515,-0.99387,-0.99243,-0.99084,-0.98914,-0.98724,-0.98523,-0.98306,-0.98074,-0.97827,-0.97565,-0.9729,-0.97,-0.96692,-0.96375,-0.96039,-0.95691,-0.95325,-0.94949,-0.94556,-0.9415,-0.93729,-0.93295,-0.92847,-0.92383,-0.91907,-0.91415,-0.90912,-0.90396,-0.89862,-0.89319,-0.8876,-0.88187,-0.87604,-0.87006,-0.86392,-0.8577,-0.85132,-0.84482,-0.8382,-0.83142,-0.82455,-0.81754,-0.81042,-0.80316,-0.79581,-0.7883,-0.7807,-0.77298,-0.76514,-0.75717,-0.74908,-0.74091,-0.7326,-0.72421,-0.7157,-0.70706,-0.69833,-0.68951,-0.68057,-0.67151,-0.66238,-0.65314,-0.6438,-0.63437,-0.62482,-0.6152,-0.60547,-0.59567,-0.58575,-0.57578,-0.5657,-0.55554,-0.54529,-0.53497,-0.52457,-0.51407,-0.50351,-0.49286,-0.48215,-0.47137,-0.46051,-0.44958,-0.4386,-0.42752,-0.41641,-0.40521,-0.39395,-0.38266,-0.37128,-0.35986,-0.34839,-0.33685,-0.32529,-0.31366,-0.30197,-0.29025,-0.2785,-0.26669,-0.25485,-0.24295,-0.23105,-0.21909,-0.20709,-0.19507,-0.18301,-0.17093,-0.15884,-0.1467,-0.13455,-0.12241,-0.1102,-0.097992,-0.085785,-0.073547,-0.06131,-0.049042,-0.036804,-0.024536,-0.012268,0,0.012268
};

void setup();//use this to do any initialisation if you want.

void play(double *channels);//run dac! Very very often. Too often in fact. er...

//long counter::count(long start, long end){
//	if (position<start) {
//		position=start;
//	}
//	if ( position >= end ) position = start;
//	position +=2;
//	return(position);
//}


double osc::noise() {
	//always the same unless you seed it.
	float r = rand()/(float)RAND_MAX;
	output=r*2-1;
	return(output);
}

double osc::sinewave(double frequency) {
	output=sin (phase*(TWOPI));
	if ( phase >= 1.0 ) phase -= 1.0;
	phase += (1./(samplerate/(frequency*chandiv)));
	return(output);
	
}

double osc::sinebuf4(double frequency) {
	double remainder;
	double a,b,c,d,a1,a2,a3;
	phase += 512./(samplerate/(frequency*chandiv));
	if ( phase >= 511 ) phase -=512;
	remainder = phase - floor(phase);

	if (phase==0) {
		a=sineBuffer[(long) 512];
		b=sineBuffer[(long) phase];
		c=sineBuffer[(long) phase+1];
		d=sineBuffer[(long) phase+2];

	} else {
		a=sineBuffer[(long) phase-1];
		b=sineBuffer[(long) phase];
		c=sineBuffer[(long) phase+1];
		d=sineBuffer[(long) phase+2];

	}
			
		a1 = 0.5f * (c - a);
		a2 = a - 2.5 * b + 2.f * c - 0.5f * d;
		a3 = 0.5f * (d - a) + 1.5f * (b - c);
		output = double (((a3 * remainder + a2) * remainder + a1) * remainder + b);
	return(output);
}

double osc::sinebuf(double frequency) {
	double remainder;
 	phase += 512./(samplerate/(frequency*chandiv));
	if ( phase >= 511 ) phase -=512;
	remainder = phase - floor(phase);
	output = (double) ((1-remainder) * sineBuffer[1+ (long) phase] + remainder * sineBuffer[2+(long) phase]);
	return(output);
}

double osc::coswave(double frequency) {
	output=cos (phase*(TWOPI));
	if ( phase >= 1.0 ) phase -= 1.0;
	phase += (1./(samplerate/(frequency*chandiv)));
	return(output);
	
}

double osc::phasor(double frequency) {
	output=phase;
	if ( phase >= 1.0 ) phase -= 1.0;
	phase += (1./(samplerate/(frequency*chandiv)));
	return(output);
} 

double osc::square(double frequency) {
	if (phase<0.5) output=-1;
	if (phase>0.5) output=1;
	if ( phase >= 1.0 ) phase -= 1.0;
	phase += (1./(samplerate/(frequency*chandiv)));
	return(output);
}

double osc::pulse(double frequency, double duty) {
	if (duty<0.) duty=0;
	if (duty>1.) duty=1;
	if ( phase >= 1.0 ) phase -= 1.0;
	phase += (1./(samplerate/(frequency*chandiv)));
	if (phase<duty) output=-1.;
	if (phase>duty) output=1.;
	return(output);
}

double osc::phasor(double frequency, double startphase, double endphase) {
	output=phase;
	if (phase<startphase) {
		phase=startphase;
	}
	if ( phase >= endphase ) phase = startphase;
	phase += ((endphase-startphase)/(samplerate/(frequency*chandiv)));
	return(output);
}

double osc::triangle(double frequency, double phase) {
	output=tri*2;
	if ( phase >= 1.0 ) phase -= 1.0;
	phase += (1./(samplerate/(frequency*chandiv)));
	if (phase <= 0.5 ) {
	tri = phase;
	} else {	
	tri =(1-phase);
	}
	return(output);
	
} 

double osc::saw(double frequency) {
	
	output=phase;
	if ( phase >= 1.0 ) phase -= 2.0;
	phase += (1./(samplerate/(frequency*chandiv)));
	return(output);
	
} 

double osc::additive (unsigned int waves, double freq1[100]) {
	unsigned int i,j;
	double freqs[waves];
	double phases[waves];
	double sinesum=0;
	for (i=0; i<waves; i++) {
		if ( phases[i] >= 1.0 ) phases[i] -= 1.0;
		phases[i] += (1./(samplerate/(freq1[i])*chandiv));
		freqs[i]=(sin (phases[i]*TWOPI))/waves;
	}
	for (j=0; j<waves; j++) {
		sinesum+=(freqs[j]);
	}
	
	return(sinesum);
}

//I like this.
double envelope::line(int numberofsegments,double segments[1000]) {
	period=channels/(segments[valindex+1]*0.004);
	nextval=segments[valindex+2];
	currentval=segments[valindex];
	if (currentval-amplitude > 0.0000001 && valindex < numberofsegments) {
		amplitude += ((currentval-startval)/(samplerate/period));
	} else if (currentval-amplitude < -0.0000001 && valindex < numberofsegments) {
		amplitude -= (((currentval-startval)*(-1))/(samplerate/period));
	} else if (valindex >numberofsegments-1) {
		valindex=numberofsegments-2;
	} else {
		valindex=valindex+2;
		startval=currentval;
	}
	output=amplitude;
	return(output);	
}

//and this
void envelope::trigger(int index,double amp) {
	valindex=index;
	amplitude=amp;
	
}

//and this
double delayline::dl(double input, int size, double feedback)  {
	if ( phase >=size ) phase = 0;
	memory[phase]=(memory[phase]*feedback)+(input*feedback)*chandiv;
	phase+=1;
	output=memory[phase+1];
	return(output);
	
}

double delayline::dl(double input, int size, double feedback, int position)  {
	if ( phase >=size ) phase = 0;
	if ( position >=size ) position = 0;
	memory[phase]=(memory[phase]*feedback)+(input*feedback)*chandiv;
	phase+=1;
	output=memory[position];
	return(output);
	
}

//I particularly like these. cutoff between 0 and 1
double filter::lopass(double input, double cutoff) {
	output=outputs[0] + cutoff*(input-outputs[0]);
	outputs[0]=output;
	return(output);
}
//as above
double filter::hipass(double input, double cutoff) {
	output=input-(outputs[0] + cutoff*(input-outputs[0]));
	outputs[0]=output;
	return(output);
}
//awesome. cuttof is freq in hz. res is between 1 and whatever. Watch out!
double filter::lores(double input,double cutoff1, double resonance) {
	cutoff=cutoff1*0.5;
	if (cutoff>(samplerate*0.25)) cutoff=(samplerate*0.25);
	if (resonance<1.) resonance = 1.;
	z=cos(TWOPI*cutoff/samplerate);
	c=2-2*z;
	double r=(sqrt(2.0)*sqrt(-pow((z-1.0),3.0))+resonance*(z-1))/(resonance*(z-1));
	x=x+(input-y)*c;
	y=y+x;
	x=x*r;
	output=y;
	return(output);
}

//this is just stupid. Kindof works.
double filter::hires(double input,double cutoff1, double resonance) {
	cutoff=cutoff1*0.5;
	if (cutoff>(samplerate*0.25)) cutoff=(samplerate*0.25);
	if (resonance<1.) resonance = 1.;
	z=cos(TWOPI*cutoff/samplerate);
	c=2-2*z;
	double r=(sqrt(2.0)*sqrt(-pow((z-1.0),3.0))+resonance*(z-1))/(resonance*(z-1));
	x=x+(input-y)*c;
	y=y+x;
	x=x*r;
	output=input-y;
	return(output);
}

//This works a bit. Needs attention.
double filter::bandpass(double input,double cutoff1, double resonance) {
	cutoff=cutoff1;
	if (cutoff>(samplerate*0.5)) cutoff=(samplerate*0.5);
	if (resonance>=1.) resonance=0.999999;
	z=cos(TWOPI*cutoff/samplerate);
	inputs[0] = (1-resonance)*(sqrt(resonance*(resonance-4*pow(z,2)+2)+1));
	inputs[1] = 2*z*resonance;
	inputs[2] = pow((resonance*-1),2);
	
	output=inputs[0]*input+inputs[1]*outputs[1]+inputs[2]*outputs[2];
	outputs[2]=outputs[1];
	outputs[1]=output;
	return(output);
}

//stereo bus
double *mix::stereo(double input,double two[2],double x) {
	if (x>1) x=1;
	if (x<0) x=0;
	two[0]=input*sqrt(1.0-x);
	two[1]=input*sqrt(x);
	return(two);
} 

//quad bus
double *mix::quad(double input,double four[4],double x,double y) {
	if (x>1) x=1;
	if (x<0) x=0;
	if (y>1) y=1;
	if (y<0) y=0;
	four[0]=input*sqrt((1.0-x)*y);
	four[1]=input*sqrt((1.0-x)*(1.0-y));
	four[2]=input*sqrt(x*y);
	four[3]=input*sqrt(x*(1.0-y));
	return(four);
}

//ambisonic bus
double *mix::ambisonic(double input,double eight[8],double x,double y,double z) {
	if (x>1) x=1;
	if (x<0) x=0;
	if (y>1) y=1;
	if (y<0) y=0;
	if (z>1) y=1;
	if (z<0) y=0;
	eight[0]=input*(sqrt((1.0-x)*y)*1.0-z);
	eight[1]=input*(sqrt((1.0-x)*(1.0-y))*1.0-z);
	eight[2]=input*(sqrt(x*y)*1.0-z);
	eight[3]=input*(sqrt(x*(1.0-y))*1.0-z);
	eight[4]=input*(sqrt((1.0-x)*y)*z);
	eight[5]=input*(sqrt((1.0-x)*(1.0-y))*z);
	eight[6]=input*sqrt((x*y)*z);
	eight[7]=input*sqrt((x*(1.0-y))*z);
	return(eight);
}

void sample::load(char* tmpPath) {
	myPath = new char[200];
	strcpy(myPath, tmpPath);
	read();
}

double sample::play() {
	speed=1;
	long length=myDataSize*0.5;
	double remainder;
	short* buffer = (short *)myData;
	position=position+(((speed)*chandiv*myChannels)/(samplerate/mySampleRate));
	remainder = position - (long) position;
	if ((long) position>length) position=0;
	output = (double) ((1-remainder) * buffer[1+ (long) position] + remainder * buffer[2+(long) position])/32767;//linear interpolation
	return(output);
}

double sample::play(double speed) {
	double remainder;
	long length=myDataSize*0.5;	
	short* buffer = (short *)myData;
	position=position+(((speed)*chandiv*myChannels)/(samplerate/mySampleRate));
	if (speed >=0) {
		if ((long) position>length) position=1;
		remainder = position - (long) position;
		output = (double) ((1-remainder) * buffer[1+ (long) position] + remainder * buffer[2+(long) position])/32767;//linear interpolation
	} else {
		if ((long) position<0) position=length;
		remainder = position - (long) (position-1);
		output = (double) ((-1-remainder) * buffer[((long) position) -1] + remainder * buffer[((long) position)-2])/32767;//linear interpolation
	}	
	return(output);
}

double sample::play(double frequency, double start, double end) {
	double remainder;
	short* buffer = (short *)myData;
	if (frequency >0.) {
		if (position<start) {
			position=start;
		}
		if ( position >= end ) position = start;
		position += 0.5*((end-start)/(samplerate/(frequency*chandiv)));
		remainder = position - (long) position;
		output = (double) ((1-remainder) * buffer[1+ (long) position] +
						   remainder * buffer[2+(long) position])/32767;//linear interpolation
	} else {
		frequency=frequency-(frequency+frequency);
		if ( position <= start ) position = end;
		position -= 0.5*((end-start)/(samplerate/(frequency*chandiv)));
		remainder = position - (long) (position);
		output = (double) ((-1-remainder) * buffer[((long) position)-1] +
						   remainder * buffer[((long) position)-2])/32767;//linear interpolation
		
	}
	
	return(output);
}

//Cubic interpolation. Cobbled together from various.
double sample::play4(double frequency, double start, double end) {
	double remainder;
	double a,b,c,d,a1,a2,a3;
	short* buffer = (short*)myData;
	if (frequency >0.) {
		if (position<start) {
			position=start;
		}
		if ( position >= end-1 ) position = start;
		position += 0.5*((end-start)/(samplerate/(frequency*chandiv)));
		remainder = position - (long) position;
		if (position>0) {
			a=buffer[(long) position-1];

		} else {
			a=buffer[0];
			
		}
		
		b=buffer[(long) position];
		if (position<end-2) {
			c=buffer[(long) position+1];

		} else {
			c=buffer[0];

		}
		if (position<end-3) {
			d=buffer[(long) position+2];

		} else {
			d=buffer[0];
		}
		a1 = 0.5f * (c - a);
		a2 = a - 2.5 * b + 2.f * c - 0.5f * d;
		a3 = 0.5f * (d - a) + 1.5f * (b - c);
		output = (double) (((a3 * remainder + a2) * remainder + a1) * remainder + b) / 32767;
		
	} else {
		frequency=frequency-(frequency+frequency);
		if ( position <= start ) position = end;
		position -= 0.5*((end-start)/(samplerate/(frequency*chandiv)));
		remainder = position - (long) (position-1);
		if (position>start && position < end-1) {
			a=buffer[(long) position+1];
			
		} else {
			a=buffer[0];
			
		}
		
		b=buffer[(long) position];
		if (position>start) {
			c=buffer[(long) position-1];
			
		} else {
			c=buffer[0];
			
		}
		if (position>start+1) {
			d=buffer[(long) position-2];
			
		} else {
			d=buffer[0];
		}
		a1 = 0.5f * (c - a);
		a2 = a - 2.5 * b + 2.f * c - 0.5f * d;
		a3 = 0.5f * (d - a) + 1.5f * (b - c);
		output = (double) (((a3 * remainder + a2) * -remainder + a1) * -remainder + b) / 32767;
		
	}
	
	return(output);
}
	

long sample::length() {
	long length;
	length=myDataSize*0.5;	
	return(length);
}


#ifdef MAXIMILIAN_PORTAUDIO
int routing(const void *inputBuffer,
			void *outputBuffer,
			unsigned long nBufferFrames,
			const PaStreamCallbackTimeInfo* timeInfo,
			PaStreamCallbackFlags status,
			void *userData ){
#elif defined(MAXIMILIAN_RT_AUDIO)
	int routing	(void *outputBuffer, void *inputBuffer, unsigned int nBufferFrames,
				 double streamTime, RtAudioStreamStatus status, void *userData ) {
#endif
		
		unsigned int i, j;
		
#ifdef MAXIMILIAN_PORTAUDIO
		float *buffer = (float *) outputBuffer;
#elif defined(MAXIMILIAN_RT_AUDIO)
		double *buffer = (double *) outputBuffer;
#endif
		double *lastValues = (double *) userData;
		//	double currentTime = (double) streamTime; Might come in handy for control
		if ( status )
			std::cout << "Stream underflow detected!" << std::endl;
		for ( i=0; i<nBufferFrames; i++ ) {	
		}
		// Write interleaved audio data.
		for ( i=0; i<nBufferFrames; i++ ) {
			for ( j=0; j<channels; j++ ) {
				*buffer++=lastValues[j];
				play(lastValues);			
			}
		}
		return 0;
	}
	
	//This is main()
	int main()
	{
		setup();
		
#ifdef MAXIMILIAN_PORTAUDIO
		PaStream *stream;
		PaError err;
		err = Pa_Initialize();
		if( err != paNoError )
			std::cout <<   "PortAudio error: " << Pa_GetErrorText( err ) << std::endl;
		
		double data[channels];
		
		err = Pa_OpenDefaultStream( &stream,
								   0,          /* no input channels */
								   channels,          /* stereo output */
								   paFloat32,  /* 64 bit floating point output */
								   samplerate,
								   buffersize,        /* frames per buffer, i.e. the number
	                                                   of sample frames that PortAudio will
	                                                   request from the callback. Many apps
	                                                   may want to use
	                                                   paFramesPerBufferUnspecified, which
	                                                   tells PortAudio to pick the best,
	                                                   possibly changing, buffer size.*/
								   &routing, /* this is your callback function */
								   &data ); /*This is a pointer that will be passed to
											 your callback*/
		
		//PaAlsa_EnableRealtimeScheduling(stream,true);
		
		err = Pa_StartStream( stream );
		if( err != paNoError )
			std::cout <<   "PortAudio error: " << Pa_GetErrorText( err ) << std::endl;
		
		
		char input;
		std::cout << "\nMaximilian is playing ... press <enter> to quit.\n";
		std::cin.get( input );
		
		
		
		err = Pa_Terminate();
		if( err != paNoError )
			std::cout <<  "PortAudio error: "<< Pa_GetErrorText( err ) << std::endl;
		
#elif defined(MAXIMILIAN_RT_AUDIO)
		RtAudio dac;
		if ( dac.getDeviceCount() < 1 ) {
			std::cout << "\nNo audio devices found!\n";
			exit( 0 );
		}
		
		RtAudio::StreamParameters parameters;
		parameters.deviceId = dac.getDefaultOutputDevice();
		parameters.nChannels = channels;
		parameters.firstChannel = 0;
		unsigned int sampleRate = samplerate;
		unsigned int bufferFrames = buffersize; // 256 sample frames
		double data[parameters.nChannels];
		
		try {
			dac.openStream( &parameters, NULL, RTAUDIO_FLOAT64,
						   sampleRate, &bufferFrames, &routing, (void *)&data);
			
			dac.startStream();
		}
		catch ( RtError& e ) {
			e.printMessage();
			exit( 0 );
		}
		
		char input;
		std::cout << "\nMaximilian is playing ... press <enter> to quit.\n";
		std::cin.get( input );
		
		try {
			// Stop the stream
			dac.stopStream();
		}
		catch (RtError& e) {
			e.printMessage();
		}
		
		if ( dac.isStreamOpen() ) dac.closeStream();
#endif
		
		return 0;
	}