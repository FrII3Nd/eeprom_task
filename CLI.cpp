#include "CLI.h"

char buffer[BUF_SIZE];
char* token;
void console_init() {
  Serial.begin(baud);
  Serial.setTimeout(timeout);
  Serial.println("\n---------------CLI enable---------------");
}
void console_scan() {
  while (Serial.available() > 0) {
    Serial.readBytes((byte*)&buffer, sizeof(buffer));
    Serial.print("\n>>");
    Serial.println(buffer);
    token = buffer;
    cli_explorer(token);
    memset(buffer, 0, sizeof(buffer));
  }
}
void cli_explorer(char* token) {
  char* subtoken = strtok(token, " ");
  if (strcmp(subtoken, "eeprom") == 0)  //Check command
  {
    subtoken = strtok('\0', " ");

    //__________WRITE__________

    if (strcmp(subtoken, "-w") == 0)  //Check key1 -w
    {
      subtoken = strtok('\0', " ");
      if (strcmp(subtoken, "-a") == 0)  // Check key2 -a
      {
        subtoken = strtok('\0', " ");
        int addr = atoi(subtoken);
        if (addr > EEPROM_SIZE || addr < 0 || !digit_check(subtoken))  //Check address
        {
          ERROR(3, subtoken);  // Incorrect address
          return 0;
        }
        subtoken = strtok('\0', " ");
        if (strcmp(subtoken, "-v") == 0)  // Check key3 -v
        {
          subtoken = strtok('\0', " ");
          if (atoi(subtoken) > 255 || atoi(subtoken) < 0 || !digit_check(subtoken) || subtoken == '\0')  //Check value
          {
            ERROR(4, subtoken);  // Incorrect value
            return 0;
          }
          Serial.println("Writing to eeprom...");
          eeprom_write_byte(addr, atoi(subtoken));  //Write byte to eeprom
          Serial.print("Writing success: ");
          Serial.print(subtoken);
          Serial.print(" --> ");
          Serial.print("(");
          Serial.print(addr);
          Serial.println(")");


          return 0;
        }
      }
    }

    //__________READ__________

    if ((strcmp(subtoken, "-r") == 0))  // Check key1 -r
    {
      subtoken = strtok('\0', " ");
      if (strcmp(subtoken, "-a") == 0)  // Check key2 -a
      {
        Serial.println("Reading from eeprom...");
        subtoken = strtok('\0', " ");
        if (atoi(subtoken) > EEPROM_SIZE || atoi(subtoken) < 0 || !digit_check(subtoken))  //Check address
        {
          ERROR(3, subtoken);
          return 0;
        }
        Serial.print("Reading success: (");
        Serial.print(subtoken);
        Serial.print(") : ");
        Serial.println(eeprom_read_byte(atoi(subtoken)));  // Print byte from eeprom
        return 0;
      }
    }

    //__________CLEAR__________

    if ((strcmp(subtoken, "-e") == 0))  // Check key1 -e
    {
      subtoken = strtok('\0', " ");
      if (strcmp(subtoken, "-a") == 0)  // Check key2 -a
      {
        subtoken = strtok('\0', " ");
        Serial.println("Clearing eeprom cell...");
        if (atoi(subtoken) > EEPROM_SIZE || atoi(subtoken) < 0 || !digit_check(subtoken))  //Check address
        {
          ERROR(3, subtoken);  //Incorrect address
          return 0;
        }
        eeprom_write_byte(atoi(subtoken), 0);  // Write 0 to eeprom
        Serial.println("Clearing success!");
        return 0;
      }
    }

    //__________DUMP__________

    if ((strcmp(subtoken, "-d") == 0))  // Check key1 -d
    {
      int num = 0;  // Cell address info (0000 0008...)
      int cnt = 0;  // Current cell address
      char fill[4];
      char data[2];

      for (int i = 0; i < EEPROM_SIZE / 8; i++) {
        sprintf(fill, "%04d", num);
        Serial.print(fill);
        Serial.print(" : ");
        for (int j = 0; j < 8; j++) {
          sprintf(data, "%02X", eeprom_read_byte(cnt));
          Serial.print(data);
          Serial.print(" ");
          cnt++;
        }
        num += 8;
        Serial.print("\n");
      }
      return 0;
    }

    //__________HELP__________

    if ((strcmp(subtoken, "-h") == 0))  // Check key1 -h
    {
      Serial.print("                  EEPROM MANUAL                  \n\n     Syntax:  eeprom [key1] [key2] [address] [key3] [value]\n");
      Serial.print("     key1:  -w : Write to EEPROM. Example: \"eeprom -w [key2] [address] [key3] [value]\"\n            -r : Read from EEPROM. Example: \"eeprom -r [key2] [address]\"\n");
      Serial.print("            -e : Clear EEPROM cell. Example: \"eeprom -e [key2] [address]\"\n            -d : Output a memory dump. Example: \"eeprom -d\"\n            -h : Open manual. Example: \"eeprom -h\"\n");
      Serial.print("     key2:  -a : Key for selecting memory cell address. Example: \"eeprom [key1] -a 10\"\n");
      Serial.print("     key3:  -v : Key for entering the value. Example: \"eeprom [key1] [key2] [address] -v 55\"\n");
      return 0;
    }
    ERROR(2, subtoken);
    return 0;
  } else {
    ERROR(1, subtoken);
    return 0;
  }
}
void ERROR(int key, char* data) {
  switch (key) {
    case 1:  // Command checkout fail
      Serial.print("Command \"");
      Serial.print(data);
      Serial.println("\" not found");
      return 0;
    case 2:  // Key сheckout fail
      Serial.println("Error: Incorrect syntax! (use \"eeprom -h\" for more information)");
      return 0;
    case 3:  // Address checkout fail
      Serial.println("Error: Incorrect address!");
      return 0;
    case 4:  // Value checkout fail
      Serial.println("Error: Incorrect value! (Use 0 - 255)");
      return 0;
    default: return 0;
  }
}
bool digit_check(char* str) {
  char* string = str;
  for (int i = 0; i < strlen(string); i++) {
    if (isdigit(string[i]) == 0) {
      return false;
    }
  }
  return true;
}