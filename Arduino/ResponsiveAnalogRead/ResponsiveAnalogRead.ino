#include "MIDIUSB.h"


// include the ResponsiveAnalogRead library
#include <ResponsiveAnalogRead.h>

// define the pin you want to use
const int ANALOG_PIN = A0;

// make a ResponsiveAnalogRead object, pass in the pin, and either true or false depending on if you want sleep enabled
// enabling sleep will cause values to take less time to stop changing and potentially stop changing more abruptly,
//   where as disabling sleep will cause values to ease into their correct position smoothly and more accurately
ResponsiveAnalogRead analog(ANALOG_PIN, true);

// the next optional argument is snapMultiplier, which is set to 0.01 by default
// you can pass it a value from 0 to 1 that controls the amount of easing
// increase this to lessen the amount of easing (such as 0.1) and make the responsive values more responsive
// but doing so may cause more noise to seep through if sleep is not enabled

void noteOn(byte channel, byte pitch, byte velocity) {
  midiEventPacket_t noteOn = {0x09, 0x90 | channel, pitch, velocity};
  MidiUSB.sendMIDI(noteOn);
}

void noteOff(byte channel, byte pitch, byte velocity) {
  midiEventPacket_t noteOff = {0x08, 0x80 | channel, pitch, velocity};
  MidiUSB.sendMIDI(noteOff);
}

void controlChange(byte channel, byte control, byte value) {
  midiEventPacket_t event = {0x0B, 0xB0 | channel, control, value};
  MidiUSB.sendMIDI(event);
}

void setup() {
  // begin serial so we can see analog read values through the serial monitor
  Serial.begin(9600);
}


void loop() {
  // update the ResponsiveAnalogRead object every loop
  analog.update();

  
  
  // if the repsonsive value has change, print out 'changed'
  if(analog.hasChanged()) {
    controlChange(0, 48, analog.getValue()/8);   // Channel 0, middle C, normal velocity
  MidiUSB.flush();
  //delay(500);
 Serial.println("Sending note off");
  
   Serial.println(analog.getRawValue()/8);
  Serial.print("\t");
  Serial.println(analog.getValue()/8);
  Serial.print("\tchanged");
  }
  
  //Serial.println("");
  
}
