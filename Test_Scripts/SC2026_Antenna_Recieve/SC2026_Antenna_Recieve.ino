#define SERIAL_RX_BUFFER_SIZE 256
#define SERIAL_TX_BUFFER_SIZE 256
#define MAX_PACKET_SIZE 256

#define XBee Serial1 

#include <SD.h>
#include <SoftwareSerial.h>
#include <stdint.h>
#include <Wire.h>

int sd = BUILTIN_SDCARD;
File AntennaRecieveData;

const int lightpin = 13;

void setup() 
{
  Serial.begin(9600);
  XBee.begin(9600);

  pinMode(lightpin, OUTPUT);
  
  if (!SD.begin(sd))
  {
    //This will blink the built-in LED once every second if the SD card fails to initialize
    while (1)
    {
      digitalWrite(lightpin, HIGH);
      delay(1000);
      digitalWrite(lightpin, LOW);
      delay(1000);
    }
  }
  else
  {
    //Create the file if it doesn't exist
    Serial.println("SD card looks good my G");
    AntennaRecieveData = SD.open("AntennaRecieveData.txt", FILE_WRITE);
    if (AntennaRecieveData)
    {
      AntennaRecieveData.println("------- Starting new run -------");
      AntennaRecieveData.println();
      AntennaRecieveData.close();
    }
  }

  AntennaRecieveData = SD.open("AntennaRecieveData.txt", FILE_WRITE);
  Serial.print("Reciever configured");

}

void loop() 
{
  antennaReceive();

}

void antennaReceive()
{
  static char buffer[MAX_PACKET_SIZE];
  static uint8_t index = 0;
  static bool receiving = false;

  while (XBee.available())
  {
    char c = XBee.read();

    if (c == '<')
    {
      receiving = true;
      index = 0;
      memset(buffer, 0, MAX_PACKET_SIZE);
    }
    else if (c == '>' && receiving)
    {
      buffer[index] = '\0';
      receiving = false;

      processPacket(buffer);
    }
    else if (receiving)
    {
      if (index < MAX_PACKET_SIZE - 1)
      {
        buffer[index++] = c;
      }
      else
      {
        receiving = false;
        index = 0;
      }
    }
  }
}

void processPacket(char* packet)
{
  char* separator = strchr(packet, '|');

  if (separator == NULL)
  {
    Serial.print("Invalid packet format: ");
    Serial.println(packet);
    return;
  }

  *separator = '\0';
  char* message = packet;
  char* checksumStr = separator + 1;

  uint8_t receivedChecksum = atoi(checksumStr);

  uint8_t calculatedChecksum = 0;
  for (int i = 0; message[i] != '\0'; i++)
  {
    calculatedChecksum += message[i];
  }

  if (receivedChecksum == calculatedChecksum)
  {
    Serial.print("Valid packet received: ");
    Serial.println(message);
  
    if (AntennaRecieveData)
    {
      AntennaRecieveData.print(millis()/1000.0);
      AntennaRecieveData.print(" ");
      AntennaRecieveData.println(message);
      AntennaRecieveData.flush();
    }
  }
  else
  {
    Serial.print("Checksum failed: ");
    Serial.println(packet);
  }
}
