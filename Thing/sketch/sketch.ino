// sketch.ino

#include <Arduino.h>
#include <Wire.h>
#include <esp_log.h>
#include "Thing.h"

// config ///////////////////////////////////////////////////////////////////

int LABNUM =  10; // which lab exercise number are we doing?
int firmwareVersion = 4; // used to check for updates, see Ex10

// arduino-land entry points ////////////////////////////////////////////////

// initialisation
void setup() {
  switch(LABNUM) {
    case  1: setup01(); break;
    case  2: setup02(); break;
    case  3: setup03(); break;
    case  4: setup04(); break;
    case  5: setup05(); break;
    case  6: setup06(); break;
    case  7: setup07(); break;
    case  8: setup08(); break;
    case  9: setup09(); break;
    case 10: setup10(); break;
    case 11: setup11(); break;
    case 12: setup12(); break;
    default: Serial.println("oops! invalid lab number");
  }
}

// task loop entry point
void loop() {
  switch(LABNUM) {
    case  1: loop01(); break;
    case  2: loop02(); break;
    case  3: loop03(); break;
    case  4: loop04(); break;
    case  5: loop05(); break;
    case  6: loop06(); break;
    case  7: loop07(); break;
    case  8: loop08(); break;
    case  9: loop09(); break;
    case 10: loop10(); break;
    case 11: loop11(); break;
    case 12: loop12(); break;
    default: Serial.println("oops! invalid lab number");
  }
}

// utilities ////////////////////////////////////////////////////////////////

// delay/yield macros
#define WAIT_A_SEC   vTaskDelay(    1000/portTICK_PERIOD_MS); // 1 second
#define WAIT_SECS(n) vTaskDelay((n*1000)/portTICK_PERIOD_MS); // n seconds
#define WAIT_MS(n)   vTaskDelay(       n/portTICK_PERIOD_MS); // n millis

#define ECHECK ESP_ERROR_CHECK_WITHOUT_ABORT

// IDF logging
static const char *TAG = "main";
