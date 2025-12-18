#include <CH455.h>

/*
STM32F103：
  SCL -> PB6
  SDA -> PB7
*/

CH455 display(PB6, PB7, CH455::COMMON_CATHODE);

void setup() {
  display.begin(8);

  display.displayNumber(1234);
}

void loop() {

}
