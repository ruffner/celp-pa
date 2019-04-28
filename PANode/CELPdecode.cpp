#include "CELPdecode.h"

// called first - initialized speex decoder
int CELPDecode::init(void)
{
	if( act ) return;
   /*Create a new decoder state in narrowband mode*/
   state = speex_decoder_init(&speex_nb_mode);

   /*Set the perceptual enhancement on*/
   tmp=1;
   int result = speex_decoder_ctl(state, SPEEX_SET_ENH, &tmp);

   /*Initialization of the structure that holds the bits*/
   speex_bits_init(&bits);
   
   act = true;
   
   return result;
}

//set_buf populates buf frorm bufin to be used in the deocde process
void CELPDecode::feed(char* bufin, uint16_t len)
{
	__disable_irq();
	for(int i = 0; i<len; i++){
		input[i] = bufin[i];
	}
	inputBytes = len;
	hasProcessed = false;
	__enable_irq();
}

// returns true when data supplied with feed() has been processed, indicating more data
// is ready to be fed
bool CELPDecode::consumed()
{
	return hasProcessed;
}

//update is run every 2.9 ms or so. this will read allocate a sound 
//block, decode the CELP data, the transmit the reult to the DAC
void CELPDecode::update(void)
{
	audio_block_t *outBlock;
	outBlock = allocate();

	// no where to store output data
	if(outBlock == NULL) return;
	
	outData = (int16_t*)(outBlock->data);

	// dont run if not initialized
	if( !act ) return;

	// if we have new data to process, do it
	if( !hasProcessed && inputBytes){
	
		// read input bytes to speex bits struct
		speex_bits_read_from(&bits, input, inputBytes);

		// decode speex bits struct
		speex_decode_int(state, &bits, outBuffer);
		
		hasProcessed = true;
	}
		
	// if we have remaining data, fill output block with combination of old and new data
	if( tempPos ) {
		// we have old data that needs to be filled in first
		for( int i=0; i<tempPos; i++ ){
			outData[i] = tempBuffer[i];
		}
		outPos = tempPos;
		
		// now fill in remaining space in outData with freshly decoded data
		for( int i=outPos; i<AUDIO_BLOCK_SAMPLES; i++ ){
			outData[i] = outBuffer[i-outPos];
		}
		
		// now store the rest of the newly decoded data into tempBuffer
		tempPos = AUDIO_BLOCK_SAMPLES-outPos;
		for( int i=tempPos; i<FRAME_SIZE; i++ ){
			tempBuffer[i-tempPos] = outBuffer[i];
		}
	} 
	// otherwise just copy in the initial data 
	else {
		// fill output block with initial data
		for( int i=0; i<AUDIO_BLOCK_SAMPLES; i++ ){
			outData[i] = outBuffer[i];
		}
		
		// now store the rest of the newly decoded data into tempBuffer
		tempPos = AUDIO_BLOCK_SAMPLES;
		for( int i=tempPos; i<FRAME_SIZE; i++ ){
			tempBuffer[i-tempPos] = outBuffer[i];
		}
	}
	
	
	transmit(outBlock);
	release(outBlock);
}
