#include <CH455.h>

CH455 display(PB6, PB7, CH455::COMMON_CATHODE);

void setup() {
  Serial.begin(9600);

  display.begin(8);
  display.displayNumber(0);

  Serial.println("CH455 Key Scan Test");
}

void loop() {
  uint8_t key = display.readKey();

  if (key != 0xFF && key != 0x00) {
    Serial.print("Key code: 0x");
    Serial.println(key, HEX);

    display.displayNumber(key);
  }

  delay(100);
}
