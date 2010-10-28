/*--------------------------------------------------------------------------- ------------------------- 
 * 
 * Marek Bereza 13/03/10 
 * 
 */ 
#include <stdlib.h> 
#include <stdio.h> 
#include <unistd.h> 
#include <string.h> 
#include "portaudio.h" 
#include "PdGraph.h" 
#include "StaticUtils.h" 
// settings 
#define BLOCK_SIZE 1024 
#define NUM_INPUTS  0 
#define NUM_OUTPUTS 2 
#define SAMPLERATE (44100.0f) 
// interleaved io buffers 
float output[BLOCK_SIZE*NUM_OUTPUTS]; 
float input[BLOCK_SIZE*NUM_INPUTS]; 
/* This routine will be called by the PortAudio engine when audio is 
needed. 
** It may called at interrupt level on some machines so don't do 
anything 
** that could mess up the system like calling malloc() or free(). 
*/ 
static int pdCallback( const void *inputBuffer, void *outputBuffer, 
		       unsigned long framesPerBuffer, 
		       const PaStreamCallbackTimeInfo* timeInfo, 
		       PaStreamCallbackFlags statusFlags, 
		       void *userData ) 
{ 
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
} 

void getDirAndFile(char *path, char *outDir, char *outFile) { 
  char *lastSlash = strrchr(path, '/'); 
  sprintf(outDir, ""); 
  if(lastSlash==NULL) { 
    sprintf(outFile, "%s", path); 
  } else { 
    strncpy(outDir, path, 1+1+lastSlash-path); 
    outDir[1+lastSlash-path] = '\0'; 
    strcpy(outFile, lastSlash+1); 
  } 
} 

int main(int argc, char * const argv[]) { 
  char dir [256]; 
  char file[256]; 
  char buf [256]; 
  PaStream *stream; 
  PaError err; 
  PdGraph *graph; 
  printf("Pd+Portaudio example\n"); 
  if(argc!=2) { 
    printf("Usage: \n\t%s file.pd\n\n", argv[0]); 
    return 1; 
  } 
  // relative path 
  if(argv[1][0]!='/' && argv[1][0]!='~') { 
    char *cwd = getcwd(NULL, 128); 
    sprintf(buf, "%s/%s", cwd, argv[1]); 
    free(cwd); 
  } else { // absolute path 
    sprintf(buf, argv[1]); 
    printf("Opening: %s\n", buf); 
  } 
  getDirAndFile(buf, dir, file); 
  graph = PdGraph::newInstance(dir, file, "", 
			       BLOCK_SIZE, 
			       NUM_INPUTS, 
			       NUM_OUTPUTS, 
			       SAMPLERATE, 
			       NULL); 
  err = Pa_Initialize(); 
  if( err != paNoError ) goto error; 
  err = Pa_OpenDefaultStream( &stream, 
			      0,          /* no input channels */ 
			      2,          /* stereo output */ 
			      paFloat32,  /* 32 bit floating point output */ 
			      SAMPLERATE, 
			      BLOCK_SIZE,        /* frames per buffer */ 
			      pdCallback, 
			      graph); 
  if( err != paNoError ) goto error; 
  err = Pa_StartStream( stream ); 
  if( err != paNoError ) goto error; 
  printf("\nHit ENTER to stop.\n\n"); 
  getchar(); 
  err = Pa_StopStream( stream ); 
  if( err != paNoError ) goto error; 
  err = Pa_CloseStream( stream ); 
  if( err != paNoError ) goto error; 
  Pa_Terminate(); 
  delete graph; 
  return err; 
 error: 
  Pa_Terminate(); 
  fprintf( stderr, "An error occured while using the portaudio stream 
\n" ); 
  fprintf( stderr, "Error number: %d\n", err ); 
  fprintf( stderr, "Error message: %s\n", Pa_GetErrorText( err ) ); 
  delete graph; 
  return err; 
  return 0; 
}
