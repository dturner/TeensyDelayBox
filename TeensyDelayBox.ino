/**
 * Teensy Delay Box
 * 
 * Requirements: 
 * - Teensy 3.1/3.2
 * - Memory board add-on (Note: this uses Analog input 0 (pin 14) for SCK, so if you try to use this for something else the memory board will stop working)
 * - Audio shield add-on
 * 
 * Verify that teensy and shields are connected the right way round, see images here: https://forum.pjrc.com/threads/29276-Limits-of-delay-effect-in-audio-library/page3
 * 
 * Based on code also from: https://forum.pjrc.com/threads/29276-Limits-of-delay-effect-in-audio-library/page3
 * 
 * TODO: 
 * 
 * - have feedback amount change input gain as well (how should this work?)
 */

#include <ResponsiveAnalogRead.h> // https://github.com/dxinteractive/ResponsiveAnalogRead
#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>

#define USE_MEMORYBOARD 1


// GUItool: begin automatically generated code
AudioInputI2S            audioIn;        //xy=286,488
AudioMixer4              delayInputMixer;         //xy=460,285

#ifdef USE_MEMORYBOARD
AudioEffectDelayExternal delayNode(AUDIO_MEMORY_MEMORYBOARD); //xy=759,285
#else
AudioEffectDelay         delayNode; //xy=759,285
#endif
AudioMixer4              wetDryMixer;         //xy=800,651
AudioOutputI2S           audioOut;       //xy=907,815
AudioConnection          patchCord1(audioIn, 0, delayInputMixer, 0);
AudioConnection          patchCord2(audioIn, 0, wetDryMixer, 0);
AudioConnection          patchCord3(delayInputMixer, delayNode);
AudioConnection          patchCord4(delayNode, 0, wetDryMixer, 1);
AudioConnection          patchCord5(delayNode, 0, delayInputMixer, 1);
AudioConnection          patchCord6(wetDryMixer, 0, audioOut, 0);
AudioControlSGTL5000     audioShield;    //xy=1177,670
// GUItool: end automatically generated code

#define DELAY_TIME_PIN A3
#define FEEDBACK_PIN A2
#define WET_DRY_MIX_PIN A1

ResponsiveAnalogRead delayPin(DELAY_TIME_PIN, true);
ResponsiveAnalogRead feedbackPin(FEEDBACK_PIN, true);
ResponsiveAnalogRead wetDryPin(WET_DRY_MIX_PIN, true);

void setup() {

  Serial.println("Teensy Delay Box");
  AudioMemory(10);
  audioShield.enable();
  audioShield.volume(0.4);
  audioShield.inputSelect(AUDIO_INPUT_LINEIN);

  // Set the input and output voltage ranges
  //audioShield.lineInLevel(0); 
  //audioShield.lineOutLevel(13); 

  // Set input gain
  delayInputMixer.gain(0, 0.7);

  // Set feedback amount
  //delayInputMixer.gain(1, 0);
  
  // Set delay to 500ms
  delayNode.delay(0, 100);

  // Set wet/dry amount to 50%
  wetDryMixer.gain(0, 0.5);
  wetDryMixer.gain(1, 0.5);

}

void loop() {

  delayPin.update(); 
  feedbackPin.update();
  wetDryPin.update();
  
  if (delayPin.hasChanged()){
    // Set the delay time in milliseconds
    int delayMillis = delayPin.getValue();
    delayNode.delay(0, delayMillis);  
    Serial.print("Delay ");
    Serial.println(delayMillis);
  }

  if (feedbackPin.hasChanged()){
    // Set the feedback amount (allowing for feedback > 100%)
    float feedbackAmount = (float) feedbackPin.getValue() / 512.0f;
    delayInputMixer.gain(1, feedbackAmount);
    Serial.print("Feedback ");
    Serial.println(feedbackAmount);
  }

  if (wetDryPin.hasChanged()){
    float wetAmount = (float) wetDryPin.getValue() / 1023.0f; 
    if (wetAmount > 1) wetAmount = 1;  
    float dryAmount = 1 - wetAmount;
    wetDryMixer.gain(0, dryAmount);
    wetDryMixer.gain(1, wetAmount);
    Serial.print("Wet ");
    Serial.println(wetAmount);
  }

  // We need a delay to avoid continuously setting the delayTime which will create an awful crackling sound
  delay(20);
}
