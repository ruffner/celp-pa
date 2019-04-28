#include "effect_upsample.h"

AudioEffectUpsample::update()
{
	audio_block_t *inBlock, *outBlock;
	uint16_t *inData, *outData;
	bool flag = false;
	int i, inSamples;
	
	// allocate output
	outBlock = allocate();
	
	// see if there is 8kHz data ready, this will not be the case every time
	inBlock =  receiveReadOnly(0);
	
	// if outblock is null we can't do anything
	if (outBlock == NULL) {
		// no more audiomemory
		return;
	}  else {
		outData = (uint16_t *)outBlock->data;
	}
	
	// if inblock isnt null we can upsample the incoming data
	if (inBlock != NULL) {
		// upsample and buffer the new input data
		
		// cast data to unsigned 16 bit ints
		inData = (uint16_t *)inBlock->data;
				
		// 128 samples at 8kHz = ~706 samples at 44.1kHz
		// so need 6 audio blocks to store the upsampled data
		int inPos = 0;
		bool flag = false;
		
		// iterate through output blocks
		for( int i=0; i<6; i++ ){
			
			for( int j=0; j<AUDIO_BLOCK_SAMPLES; j++ ){
				
				outputBuffer[i][j] = inData[inPos];
				
				if( (j+1) % (flag?6:5)==0 ){
					inPos++;
					flag=!flag;
				}
				
				
			}
			
		}
	}
	// transmit buffered upsampled data
	else {
		

		
	}


	
	
	// outBlock now has fewer samples than inblock, so 
	transmit(outBlock);

	release(inBlock);
}