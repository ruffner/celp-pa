#ifndef input_speex_h
#define input_speex_h

#include "Arduino.h"
#include "AudioStream.h"
#include "DMAChannel.h"

#include "speex.h"


class AudioInputSpeex : public AudioStream 
{
public:
  AudioInputSpeex(): AudioStream(0, NULL){
    init(A2);  
  }
  virtual void update(void);
  friend void dma_ch9_isr(void);
private:
  static void *speexState;
  static SpeexBits speexBits;

  static audio_block_t *block_left;
  static uint16_t block_offset;
  static int32_t hpf_y1;
  static int32_t hpf_x1;
  
  static bool update_responsibility;
  static DMAChannel dma;
  static void isr(void);
  static void init(uint8_t pin);
};

#endif

