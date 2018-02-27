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
 */

#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>

AudioSynthWaveformSine     sine;
AudioEffectEnvelope        env;
AudioEffectDelayExternal   dly(AUDIO_MEMORY_MEMORYBOARD);
AudioMixer4                mix;
AudioOutputI2S             headphones;

AudioConnection            patchCord1(sine, env);
AudioConnection            patchCord2(env, dly);
AudioConnection            patchCord3(dly, 0, mix, 0);
AudioConnection            patchCord4(dly, 1, mix, 1);
AudioConnection            patchCord5(env, 0, headphones, 0);
AudioConnection            patchCord6(mix, 0, headphones, 1);
AudioControlSGTL5000       audioShield;

void setup() {
        
        AudioMemory(10);
        audioShield.enable();
        audioShield.volume(0.3);
        sine.amplitude(0.9);
        sine.frequency(800);
        dly.delay(0, 500);
        dly.delay(1, 1500);
}

void loop() {
        env.noteOn();
        delay(450);
        env.noteOff();
        delay(2000);
        Serial.print("CPU: ");
        Serial.println(AudioProcessorUsageMax());
        AudioProcessorUsageMaxReset();
}
