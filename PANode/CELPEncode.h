#ifndef celp_encode_h
#define celp_encode_h

#include "Arduino.h"
#include "AudioStream.h"
#include "DMAChannel.h"

#include "speex.h"

#define FRAME_SIZE 160

class CELPEncode : public AudioStream 
{
public:
  CELPEncode() : AudioStream(1, inputQueueArray),act(false),dataReady(false),nbytes(0), curSamples(0) {};
  virtual void update(void);
  int init();
  bool available();
  int getNumBytes();
  uint8_t *getDataPointer();
  
private:
  static void *speexState;
  static SpeexBits speexBits;
  char cbits[200];
  bool act = false;
  int curSamples;
  uint16_t sampleBuffer[FRAME_SIZE];
  int nbytes;
  volatile bool dataReady = false;
  audio_block_t *inputQueueArray[1];
};

#endif

