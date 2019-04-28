#include "CELPEncode.h"

void * CELPEncode::speexState;
SpeexBits CELPEncode::speexBits;

int CELPEncode::init()
{
  if( act ) return;
    
  // create a new narrowband encoder
  speexState = speex_encoder_init(&speex_nb_mode);
  
  // set the encoder quality
  int temp=8;
  int result = speex_encoder_ctl(speexState, SPEEX_SET_QUALITY, &temp);
  
  
  // initialization of speex bits structure
  speex_bits_init(&speexBits);

  // indicate that init() has been called
  act = true;
  
  return result; 
}

bool CELPEncode::available()
{
  return dataReady;
}

int CELPEncode::getNumBytes()
{
  return nbytes;
}

uint8_t *CELPEncode::getDataPointer()
{
  return (uint8_t *)cbits;
  dataReady = false;
}

void CELPEncode::update()
{
  audio_block_t *inBlock;
  uint16_t *inData;
  int inSamples;

  // return if begin() has not been called
  if (!act) return;

  // try receive new audio data and allocate output block
  inBlock =  receiveReadOnly(0);

  if (inBlock == NULL) {
    // no input data, cant update
    return;
  }
  
	// receive data downsampled to roughly 8kHz from 
	// the 44.1kHz that the audio library creates
	inData = (uint16_t *)inBlock->data;

	// how many samples did we receive
  inSamples = inData[0];
  
  // if we need to just buffer up data to fill a frame for speex computation
  if( curSamples+inSamples < FRAME_SIZE ){
	  
	  // append input data to our sampleBuffer
	  for( int i=curSamples; i<curSample+inSamples; i++ ){
		  
		  // +1 offset to account for the first position storing the number of samples 
		  sampleBuffer[i] = inData[i-curSamples+1];
	  }
	  curSamples += inSamples;
  } 
  // otherwise we are receiving enough to fill a frame, possibly more
  else {
	  
	  // append enough of the input data to our sample buffer to fill it
	  for( int i=curSamples; i<FRAME_SIZE; i++ ){
		  
		  // +1 offset to account for the first position storing the number of samples 
		  sampleBuffer[i] = inData[i-curSamples+1];
	  }
	  
	  // now our sample buffer is full, so process it
	  speex_bits_reset(&speexBits);
	  speex_encode_int(speexState, (spx_int16_t*)sampleBuffer, &speexBits);
	  nbytes = speex_bits_write(&speexBits, cbits, 200);
	  dataReady = true;
	  
	  // fill the beginning of the sample buffer with the excess input data
	  int offset = FRAME_SIZE-curSamples;
	  for( int i=0; i<inSamples-offset; i++ ){
		  
		  // +1 offset to account for the first position storing the number of samples 
		  sampleBuffer[i] = inData[offset+i+1];
	  }
	  curSamples = inSamples-offset;
	  dataReady = true;
  }

  release(inBlock);
}

