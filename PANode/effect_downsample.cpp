#include "effect_downsample.h"

AudioEffectDownsample::getOutCount()
{
	return outCount;
}

AudioEffectDownsample::update()
{
	audio_block_t *inBlock, *outBlock;
	uint16_t *inData, *outData;
	bool flag = false;
	int i;
	
	// try to receive 44.1kHz audio data and allocate output block
	inBlock =  receiveReadOnly(0);
	outBlock = allocate();
	if (inBlock == NULL) {
		// no input data, cant update
		return;
	}
	if (outBlock == NULL) {
		// no more audiomemory
		return;
	}
	
	// cast data to unsigned 16 bit ints
	inData = (uint16_t *)inBlock->data;
	outData = (uint16_t *)outBlock->data;
	
	// save position 0 for the number of bytes in the output
	outCount = 1;
	
	// alternating 5 and 6 point moving average filter to downsample
	// to roughly 8kHz from 44.1k
	for( i=0; i<AUDIO_BLOCK_SAMPLES; i+=flag?6:5){
		// i tops out at 126 so we just disregard the last two samples
		if( i+5 >= AUDIO_BLOCK_SAMPLES ){
			break;
		}
		uint32_t sum = inData[i]+inData[i+1]+inData[i+2]+inData[i+3]+inData[i+4];
		if( flag ){
			// add the 6th point to the filter
			sum += inData[i+5];
			sum = sum / 6;
		} else {
			sum = sum / 5;
		}
		outData[outCount] = sum;
		outCount++;
		flag = !flag;
	}
	
	// assign the first position in outBlock to represent how many samples 
	// are contained in the audio block
	outData[0] = outCount;

	// outBlock now has fewer samples than inblock, should only be used by input expecting this
	transmit(outBlock);

	release(inBlock);
}