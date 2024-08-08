#include "CLI.h"
void setup() 
{
  console_init(9600,50); //(baud,timeout)
}
void loop() 
{
  console_scan();
}
