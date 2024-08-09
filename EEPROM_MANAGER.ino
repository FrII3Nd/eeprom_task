/*
  CLI to control eeprom Arduino Nano
  Use Serial Monitor or PuTTY console
*/

#include "CLI.h"

void setup() {
  console_init();
}
void loop() {
  console_scan();
}
