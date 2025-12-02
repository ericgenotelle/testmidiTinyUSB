// https://github.com/rupraph/esp32-s3-Demo-TinyUSB-MIDI/tree/main?tab=readme-ov-file

#include <Arduino.h>
#include <Adafruit_TinyUSB.h>
#include <MIDI.h>

Adafruit_USBD_MIDI usb_midi;
MIDI_CREATE_INSTANCE(Adafruit_USBD_MIDI, usb_midi, MidiUsb);

void setup() {
  // Serial.begin(115200);     // Serial.begin(31250); //MocoLufa  pour le MIDI ?
  // while (! Serial);
  delay(300);

  TinyUSBDevice.setManufacturerDescriptor("PIO");
  TinyUSBDevice.setProductDescriptor("[MIDI] DIY Handpan");
  MidiUsb.begin(MIDI_CHANNEL_OMNI);
  // Serial.println("Midi USB setup done");
}

void loop() {

  MidiUsb.sendNoteOn(74,127,1);
  delay(500);
  MidiUsb.sendNoteOff(74,0,1);
  delay(500);

  MidiUsb.sendNoteOn(78,127,1);
  delay(500);
  MidiUsb.sendNoteOff(78,0,1);
  delay(500);
}
