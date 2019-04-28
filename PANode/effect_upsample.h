#ifndef audio_upsample
#define audio_upsample

#include "Arduino.h"
#include "AudioStream.h"

class AudioEffectUpsample : public AudioStream
{
public:
	AudioEffectUpsample() : AudioStream(1, inputQueueArray),rxCount(0) {};
	void init(void);
	virtual void update(void);
private:
	int rxCount;
	int txPos;
	uint16_t inputBuffer[7][AUDIO_BLOCK_SAMPLES];
	uint16_t outputBuffer[6][AUDIO_BLOCK_SAMPLES];
	audio_block_t *inputQueueArray[1];
};

#endif