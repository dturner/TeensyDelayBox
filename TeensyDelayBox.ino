/**
 * Teensy Delay Box
 * 
 * Requirements: 
 * - Teensy 3.1/3.2
 * - Memory board add-on
 * - Audio shield add-on
 * 
 * Verify that teensy and shields are connected the right way round, see images here: https://forum.pjrc.com/threads/29276-Limits-of-delay-effect-in-audio-library/page3
 * 
 * Based on code also from: https://forum.pjrc.com/threads/29276-Limits-of-delay-effect-in-audio-library/page3
 * 
 * TODO: 
 * 
 * - Add scale down and scale up circuit on ns1 
 * 
 */

#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>

// GUItool: begin automatically generated code
AudioInputI2S            audioIn;           //xy=132,452
AudioEffectDelayExternal delayNode(AUDIO_MEMORY_MEMORYBOARD);      //xy=416,282
AudioMixer4              mixer1;         //xy=660,670
AudioOutputI2S           audioOut;           //xy=722,496
AudioConnection          patchCord1(audioIn, 0, delayNode, 0);
AudioConnection          patchCord2(audioIn, 0, mixer1, 0);
AudioConnection          patchCord3(audioIn, 1, audioOut, 1);
AudioConnection          patchCord4(delayNode, 0, mixer1, 1);
AudioConnection          patchCord5(mixer1, 0, audioOut, 0);
AudioControlSGTL5000     audioShield;    //xy=965,601
// GUItool: end automatically generated code


void setup() {

  AudioMemory(10);
  audioShield.enable();
  audioShield.volume(0.4);
  audioShield.inputSelect(AUDIO_INPUT_LINEIN);

  // Set the input and output voltage ranges
  audioShield.lineInLevel(0); 
  audioShield.lineOutLevel(13); 
  
  delayNode.delay(0, 500);

}

void loop() {
  
}
