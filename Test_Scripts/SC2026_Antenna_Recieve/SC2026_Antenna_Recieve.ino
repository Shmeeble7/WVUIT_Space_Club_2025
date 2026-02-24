
#define MAX_PACKET_SIZE 64
#define XBee Serial1 

void setup() 
{
  Serial.begin(9600);
  Serial1.begin(9600);
  while (!Serial1)
    delay(10);
 
  // Start XBee serial communication
  XBee.begin(9600);
  

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
      index = 0;
      receiving = true;
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
    }
  }
}

void processPacket(char* packet)
{
  char* separator = strchr(packet, '|');

  if (separator == NULL)
  {
    Serial.println("Invalid packet format");
    return;
  }

  *separator = '\0';   // Split string
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
  }
  else
  {
    Serial.println("Checksum failed");
  }
}
