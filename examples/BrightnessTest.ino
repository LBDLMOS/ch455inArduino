#include <CH455.h>

CH455 display(PB6, PB7, CH455::COMMON_CATHODE);

uint8_t brightness = 1;
unsigned long lastChange = 0;

void setup() {
  display.begin(brightness);
  display.displayNumber(888);
}

void loop() {
  if (millis() - lastChange > 1000) {
    lastChange = millis();

    brightness++;
    if (brightness > 8) brightness = 1;

    display.setBrightness(brightness);
    display.displayNumber(brightness * 111); 
  }
}
