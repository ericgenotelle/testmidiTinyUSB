// https://github.com/rupraph/esp32-s3-Demo-TinyUSB-MIDI/tree/main?tab=readme-ov-file

#include <Arduino.h>

#include <EEPROM.h>
#include "ShiftRegister74HC595.h"
#include "CD74HC4067.h"

#include <Adafruit_TinyUSB.h>
#include <MIDI.h>

#include "NetMan.h"
#include <ElegantOTA.h>

#define PIN_MUX_A       15          // PIN A to pilot MUX CD4067B
#define PIN_MUX_B       16          // PIN B to pilot MUX CD4067B
#define PIN_MUX_C       17          // PIN C to pilot MUX CD4067B
#define PIN_MUX_D       18          // PIN D to pilot MUX CD4067B

#define PIN_SR_DATA     12          // PIN for Shift Register : DATA
#define PIN_SR_CLOCK    13          // PIN for Shift Register : CLOCK  
#define PIN_SR_LATCH    14          // PIN for Shift Register : LATCH

#define EEPROM_SIZE                     512         // EEPROM_SIZE 

#define PAN_WIFI_AP_SSID            "PAN"                   //SSID prefix in WIFI AP mode
#define PAN_WIFI_AP_PASSWORD        "12345678"              //Password related to SSID prefix in WIFI AP mode
#define PAN_WIFI_STA_SSID           "BoxGnl_5GHz"           //SSID in WIFI STA mode
#define PAN_WIFI_IP                 "10.0.0.1"              //IP address in WIFI STA mode 
#define PAN_WIFI_MASQUE             "255.255.255.0"  
#define PAN_WIFI_STA_PASSWORD       "wifiGenotelle"         //@brief Password related to SSID in WIFI STA mode


/// @brief Shift Register to manage LED.
ShiftRegister74HC595<4> sr(PIN_SR_DATA, PIN_SR_CLOCK, PIN_SR_LATCH);

/// @brief MUC to manage PAD
CD74HC4067 mux(PIN_MUX_A, PIN_MUX_B, PIN_MUX_C, PIN_MUX_D);  // (S0, S1, S2, S3)


Adafruit_USBD_MIDI usb_midi;
MIDI_CREATE_INSTANCE(Adafruit_USBD_MIDI, usb_midi, MidiUsb);

NetMan nm;

void _handleRoot(AsyncWebServerRequest *request) 
{
  nm._request = request;
  nm.clear();
  nm.add("Test");
  nm.sendHTML();
}

void setup() {
  // Serial.begin(115200);     // Serial.begin(31250); //MocoLufa  pour le MIDI ?
  // while (! Serial);
  delay(300);

  bool ret = EEPROM.begin(EEPROM_SIZE) ;
  sr.setAllLow(); // set all pins LOW
  mux.channel(1); 

  TinyUSBDevice.setManufacturerDescriptor("PIO");
  TinyUSBDevice.setProductDescriptor("[MIDI] DIY Handpan2");
  MidiUsb.begin(MIDI_CHANNEL_OMNI);
  // Serial.println("Midi USB setup done");

  IPAddress ip;
  IPAddress masqueip;

  ip.fromString(PAN_WIFI_IP);
  masqueip.fromString(PAN_WIFI_MASQUE);

  ret = nm.connectWifiSTA(PAN_WIFI_STA_SSID,PAN_WIFI_STA_PASSWORD,ip,ip,masqueip);
  nm.on("/",_handleRoot);
  nm.begin();
  ElegantOTA.begin(nm._server); 

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

  ElegantOTA.loop();

}
