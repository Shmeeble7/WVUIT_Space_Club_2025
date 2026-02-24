#define XBee Serial1

void setup() 
{
  Serial1.begin(9600);
  while (!Serial1)
    delay(10);
 
  XBee.begin(9600);
  
}

void loop() 
{
  antennaSend("Big Space Data!!");
  delay(2000);
  
}

void antennaSend(const char* message)
{
  uint8_t checksum = 0;

  // Calculate the checksum
  for (int i = 0; message[i] != '\0'; i++)
  {
    checksum += message[i];
  }

  XBee.print("<");
  XBee.print(message);
  XBee.print("|");
  XBee.print(checksum);
  XBee.print(">");

}
