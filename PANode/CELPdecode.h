#ifndef celp_decode
#define celp_decode

#include "Arduino.h"
#include "AudioStream.h"
#include "speex.h"

#define FRAME_SIZE 160

class CELPDecode : public AudioStream
{
public:

	// Constructor
	CELPDecode() : AudioStream(0, NULL),act(false),hasProcessed(true),inputBytes(0),outPos(0),tempPos(0) {};
	
	// Deconstructor
	~CELPDecode(void) {
		speex_bits_destroy(&bits);
		speex_decoder_destroy(dec_state);
	}
	
	// called first to initialize class
	int init(void);
	
	// pass data to buf from outside class, do be decoded
	void feed(char* buf, int len);

	
	// return if the update function has processed the input data supplied with feed()
	// indicating the decoder is ready to be fed more data
	bool consumed(void);
	
	//update called by Audio Library
	virtual void update(void);
	
private:
	
	//speex vars
	SpeexBits bits;
	void *dec_state;
		
	//speex input set by feed
	char input[FRAME_SIZE];
	int inputBytes;	

	// since this size is larger than audio_block_samples, will have to transmit in chunks
	uint16_t outBuffer[FRAME_SIZE];
	uint16_t tempBuffer[FRAME_SIZE];
	int outPos, tempPos;
	
	// initialization and decode state vars
	bool act, hasProcessed;
};
