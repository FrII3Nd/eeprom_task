#ifndef CLI
#define CLI

#include <Arduino.h>
#include <string.h>
#include <avr/eeprom.h>
#include <stdlib.h>

#define BUF_SIZE  30 // Max number of symbols taking from user
#define EEPROM_SIZE 1000 // EEPROM memory size


extern char buffer[BUF_SIZE];
extern char* token;
void console_init(int baud, int time);
char console_scan();
void cli_explorer(char* token);
void ERROR(int key, char* data);

#endif