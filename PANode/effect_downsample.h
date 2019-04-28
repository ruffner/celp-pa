#ifndef audio_downsample
#define audio_downsample

#include "Arduino.h"
#include "AudioStream.h"

class AudioEffectDownsample : public AudioStream
{
public:
	AudioEffectDownsample() : AudioStream(1, inputQueueArray),outCount(0) {};
	virtual void update(void);
	int getOutCount();
private:
	int outCount;
	audio_block_t *inputQueueArray[1];
};

#endif