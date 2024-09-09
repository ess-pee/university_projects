// Ex02.ino/.cpp
// blinking an external LED; reading from a switch
// changes from ESP32 Feather to ESP32S3: pin 32 becomes 6, 14 becomes 5

#include "Thing.h"

bool useInternalLED = true; // which LED to blink

void setup02() {
  setup01(); // include previous setup for serial, and for the internal LED
  Serial.printf("\nsetup02...\nESP32 MAC = %s\n", MAC_ADDRESS); // ESP's "ID"

  // set up GPIO pin for an external LED
  pinMode(6, OUTPUT); // set up pin 6 as a digital output

  // set up GPIO pin for a switch
  pinMode(5, INPUT_PULLUP); // pin 5: digital input, built-in pullup resistor
}

void loop02() {
  if(digitalRead(5) == LOW) { // switch pressed
    Serial.printf("switch is pressed...\n");
    useInternalLED = ! useInternalLED;
  }

  if(useInternalLED) {
    Serial.printf("blinking internal LED...\n");
    blink(1, 500); // using the method from Ex01
  } else {
    Serial.printf("setting 6 HIGH...\n");
    digitalWrite(6, HIGH);       // on...
    delay(500); // take a rest (but: what happens if switch pressed now?!)

    Serial.printf("setting 6 LOW...\n");
    digitalWrite(6, LOW);        // off...
    delay(500); // take a rest (but: what happens if switch pressed now?!)
  }
}
