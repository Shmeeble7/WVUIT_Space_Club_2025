#define XBee Serial1

#include <stdint.h>
#include <Wire.h>
#include <Adafruit_MMC56x3.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <utility/imumaths.h>

Adafruit_MMC5603 mag = Adafruit_MMC5603(6789);
uint16_t BNO055_SAMPLERATE_DELAY_MS = 100;
Adafruit_BNO055 bno = Adafruit_BNO055(55, 0x28, &Wire);

const int lightpin = 13;
static int sentPackets = 0;

void setup() 
{
  Serial.begin(9600);
  XBee.begin(9600);

  XBee.setTimeout(5);
  
  if (!mag.begin(MMC56X3_DEFAULT_ADDRESS, &Wire))
  {  
    // There was a problem detecting the MMC5603 ... check your connections
    //Serial.println("Ooops, no MMC5603 detected ... Check your wiring!");
    while (1) delay(10);
  }

  if (!bno.begin())
  {
    //There was a problem detecting the BNO055 ... check your connections
    //Serial.print("Ooops, no BNO055 detected ... Check your wiring or I2C ADDR!");
    while (1) delay(10);
  }

  
  
  //Serial.print("Sender configured");
  delay(10000);
}

void loop() 
{
  sendMagnetometer();
  delay(20);
  sendIMU();
  delay(100);
  
}

void antennaSend(const char* message)
{
  uint8_t checksum = 0;
  char packet[256];
  
  // Calculate the checksum
  for (int i = 0; message[i] != '\0'; i++)
  {
    checksum += message[i];
  }

  snprintf(packet, sizeof(packet),"<%s|%d>", message, checksum);
  XBee.print(packet);

  /*
   * XBee.print("<");
     XBee.print(message);
     XBee.print("|");
     XBee.print(checksum);
     XBee.print(">");
   *
   */

}

void sendMagnetometer()
{
  sensors_event_t eventM;
  mag.getEvent(&eventM);

  char message[256];

  snprintf(message, sizeof(message),
           "MAG,%.3f,%.3f,%.3f,%.2f,%d",
           eventM.magnetic.x,
           eventM.magnetic.y,
           eventM.magnetic.z,
           millis()/1000.0,
           sentPackets++);

  antennaSend(message);
}

void sendIMU()
{
  sensors_event_t angVelocityData, accelerometerData;
  bno.getEvent(&angVelocityData, Adafruit_BNO055::VECTOR_GYROSCOPE);
  bno.getEvent(&accelerometerData, Adafruit_BNO055::VECTOR_ACCELEROMETER);
  
  char message[256];

  snprintf(message, sizeof(message),
    "IMU,%.3f,%.3f,%.3f,%.3f,%.3f,%.3f,%.2f,%d",
    angVelocityData.gyro.x,
    angVelocityData.gyro.y,
    angVelocityData.gyro.z,
    accelerometerData.acceleration.x,
    accelerometerData.acceleration.y,
    accelerometerData.acceleration.z,
    millis()/1000.0,
    sentPackets++);

  antennaSend(message);
  
}
