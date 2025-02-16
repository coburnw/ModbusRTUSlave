/*
  ModbusRTUSlave Library - Coil_RS-485

  This sketch demonstrates how to setup a ModbusRTUSlave to control an LED using Modbus RTU over RS-485.
  It is assumed that you know how to set up RS-485 hardware, and have an appropriate master device.
  
  Circuit:
  - RO pin of RS-485 transceiver to pin 10
  - DI pin of RS-485 transceiver to pin 11
  - DE and #RE pins of RS-485 transceiver to pin 12

  Created: 2022-06-25
  By: C. M. Bulliner
  Modified: 2022-06-30
  By: C. M. Bulliner
  Modified: 2022-10-18
  By: C. M. Bulliner

*/

// ModbusRTUSlave is not dependant on SoftwareSerial.
// It is included here so that the Serial port can be kept free for debugging.
#include <SoftwareSerial.h>
#include <ModbusRTUSlave.h>

// Here are some constants used elsewhere in the program.
const byte ledPin = LED_BUILTIN, rxPin = 10, txPin = 11, dePin = 12, id = 1;
const word bufSize = 256, numCoils = 1, ledAddress = 0;
const unsigned long baud = 38400, responseDelay = 10;

// This is the buffer for the ModbusRTUSlave object.
// It is used to store the Modbus messages.
// A size of 256 bytes is recommended, but sizes as low as 8 bytes can be used.
byte buf[bufSize];

// Initilize a SoftwareSerial port.
SoftwareSerial mySerial(rxPin, txPin);

// Initilize a ModbusRTUSlave with DE pin and response delay.
ModbusRTUSlave modbus(mySerial, buf, bufSize, dePin, responseDelay);

// This is a function that will be passed to the ModbusRTUSlave for reading coils.
char coilRead(word address) {
  if (address == ledAddress) return digitalRead(ledPin);
  else return false;
}

// This is a function that will be passed to the ModbusRTUSlave for writing to coils.
boolean coilWrite(word address, boolean value) {
  if (address == ledAddress) digitalWrite(ledPin, value);
  return true;
}

void setup() {
  // Setup the LED pin.
  pinMode(ledPin, OUTPUT);

  // Setup the SoftwareSerial port.
  mySerial.begin(baud);

  // Setup the ModbusRTUSlave
  modbus.begin(id, baud);

  // Configure the coil(s).
  modbus.configureCoils(numCoils, coilRead, coilWrite);
}

void loop() {
  // Poll for Modbus RTU requests from the master device.
  // This will automatically run the coilRead or coilWrite functions as needed.
  modbus.poll();
}
