#include "RtAudio.h"
#include <iostream>
#include <cstdlib>

// Two-channel sawtooth wave generator.
int saw( void *outputBuffer, void *inputBuffer, unsigned int nBufferFrames,
         double streamTime, RtAudioStreamStatus status, void *userData )
{
  unsigned int i, j;
  double *buffer = (double *) outputBuffer;
  double *lastValues = (double *) userData;

  if ( status )
    std::cout << "Stream underflow detected!" << std::endl;

  /* Callback de Marek */
  /* Cast data passed through stream to our structure. */ 
  PdGraph *g = (PdGraph*)userData; 
  float *out = (float*)outputBuffer; 
  g->process(input, output); 
  for(int i = 0; i < BLOCK_SIZE; i++) { 
    for(int channel = 0; channel < NUM_OUTPUTS; channel++) { 
      out[i*NUM_OUTPUTS + channel] = output[i + channel*BLOCK_SIZE]; 
    } 
  } 
  return 0; 

  /*
  /*  // Write interleaved audio data.
  for ( i=0; i<nBufferFrames; i++ ) {
    for ( j=0; j<2; j++ ) {
      *buffer++ = lastValues[j];

      lastValues[j] += 0.005 * (j+1+(j*0.1));
      if ( lastValues[j] >= 1.0 ) lastValues[j] -= 2.0;
    }
  }
  
  // Write non interleaved audio data
  for ( i=0; i<nBufferFrames; i++) {
    *buffer++ = lastValues[i];

    lastValues[i] += 0.005 * (i+(i*0.1));
    std::cout<<lastValues[i]<<std::endl;
    if ( lastValues[i] >= 1.0 ) lastValues[i] -= 2.0;
  }
  return 0;
  */
}

int main()
{
  RtAudio dac;
  if ( dac.getDeviceCount() < 1 ) {
    std::cout << "\nNo audio devices found!\n";
    exit( 0 );
  }

  RtAudio::StreamParameters parameters;
  parameters.deviceId = dac.getDefaultOutputDevice();
  parameters.nChannels = 2;
  parameters.firstChannel = 0;
  unsigned int sampleRate = 44100;
  unsigned int bufferFrames = 256; // 256 sample frames
  double data[2];
  RtAudio::StreamOptions options;
  options.flags = RTAUDIO_NONINTERLEAVED;

  try {
    dac.openStream( &parameters, NULL, RTAUDIO_FLOAT64,
                    sampleRate, &bufferFrames, &saw, (void *)&data);
    dac.startStream();
  }
  catch ( RtError& e ) {
    e.printMessage();
    exit( 0 );
  }
  
  char input;
  std::cout << "\nPlaying ... press <enter> to quit.\n";
  std::cin.get( input );

  try {
    // Stop the stream
    dac.stopStream();
  }
  catch (RtError& e) {
    e.printMessage();
  }

  if ( dac.isStreamOpen() ) dac.closeStream();

  return 0;
}
