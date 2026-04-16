
//This is the sketch for the "Pathfinder" board included in the WVUIT Space Club 2026 project 

#include <SD.h>
#include <SoftwareSerial.h>
#include <stdint.h>
#include <Wire.h>
#include <Adafruit_MMC56x3.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <utility/imumaths.h>

Adafruit_MMC5603 mag = Adafruit_MMC5603(12345);

uint16_t BNO055_SAMPLERATE_DELAY_MS = 100;
Adafruit_BNO055 bno = Adafruit_BNO055(55, 0x28, &Wire);

int sd = BUILTIN_SDCARD;
const int lightpin = 13;

File SensorData;


void setup()
{
  // Start serial communication with computer
  Serial.begin(9600);
  while (!Serial)
    delay(10);

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
      delay(10);
    }
  }
  else
  {
    //Serial.println("SD card looks good my G");
  }

  
  // I2C mode
  if (!mag.begin(MMC56X3_DEFAULT_ADDRESS, &Wire))
  {  
    // There was a problem detecting the MMC5603 ... check your connections
    //Serial.println("Ooops, no MMC5603 detected ... Check your wiring!");
    while (1)
    {
      digitalWrite(lightpin, HIGH);
      delay(1000);
      digitalWrite(lightpin, LOW);
      delay(1000);
      delay(10);
    }
  }

  if (!bno.begin())
  {
    /* There was a problem detecting the BNO055 ... check your connections */
    //Serial.print("Ooops, no BNO055 detected ... Check your wiring or I2C ADDR!");
    while (1)
    {
      digitalWrite(lightpin, HIGH);
      delay(1000);
      digitalWrite(lightpin, LOW);
      delay(1000);
      delay(10);
    }
  }

  SensorData = SD.open("IMUData.txt", FILE_WRITE);
  if(SensorData)
  {
    SensorData.println("Orientation, Angular Velocity, Linear Acceleration, Magnetometer, Acceleration, Gravitation");
    SensorData.close();
  }
  
}

void loop()
{
  
  IMUIntegrate();
  magnetometerTest();
  digitalWrite(lightpin, HIGH);
}

void magnetometerTest()
{
  SensorData = SD.open("MagnetometerData.txt", FILE_WRITE);
  sensors_event_t eventM;
  
  if(SensorData)
  {
    mag.getEvent(&eventM);
    SensorData.print("X: "); SensorData.print(eventM.magnetic.x); SensorData.print("  ");
    SensorData.print("Y: "); SensorData.print(eventM.magnetic.y); SensorData.print("  ");
    SensorData.print("Z: "); SensorData.print(eventM.magnetic.z); SensorData.print("  ");
    SensorData.print("uT ");
    SensorData.print("--");
    SensorData.println(millis()/1000.0, 4);
    SensorData.flush();
    SensorData.close();
  }
}

void IMUIntegrate()
{
  sensors_event_t orientationData , angVelocityData , linearAccelData, magnetometerData, accelerometerData, gravityData;
  
  SensorData = SD.open("IMUData.txt", FILE_WRITE);

  if(SensorData)
  {
    sensors_event_t orientationData , angVelocityData , linearAccelData, magnetometerData, accelerometerData, gravityData;
    bno.getEvent(&orientationData, Adafruit_BNO055::VECTOR_EULER);
    bno.getEvent(&angVelocityData, Adafruit_BNO055::VECTOR_GYROSCOPE);
    bno.getEvent(&linearAccelData, Adafruit_BNO055::VECTOR_LINEARACCEL);
    bno.getEvent(&magnetometerData, Adafruit_BNO055::VECTOR_MAGNETOMETER);
    bno.getEvent(&accelerometerData, Adafruit_BNO055::VECTOR_ACCELEROMETER);
    bno.getEvent(&gravityData, Adafruit_BNO055::VECTOR_GRAVITY);
    
    printEvent(&orientationData, SensorData);
    printEvent(&angVelocityData, SensorData);
    printEvent(&linearAccelData, SensorData);
    printEvent(&magnetometerData, SensorData);
    printEvent(&accelerometerData, SensorData);
    printEvent(&gravityData, SensorData);

    SensorData.print("--");
    SensorData.print(" Time: ");
    SensorData.println(millis()/1000.0, 3);
    SensorData.flush();
    SensorData.close();
    
    delay(BNO055_SAMPLERATE_DELAY_MS);
    }
  else
  {
    //Serial.println("Big yikers!!!!");
  }

  
}

void printEvent(sensors_event_t* event, File sd) {
  double x = -1000000, y = -1000000 , z = -1000000; //dumb values, easy to spot problem
  if (event->type == SENSOR_TYPE_ACCELEROMETER) {
    //sd.print("Accl:");
    x = event->acceleration.x;
    y = event->acceleration.y;
    z = event->acceleration.z;
  }
  else if (event->type == SENSOR_TYPE_ORIENTATION) {
    //sd.print("Orient:");
    x = event->orientation.x;
    y = event->orientation.y;
    z = event->orientation.z;
  }
  else if (event->type == SENSOR_TYPE_MAGNETIC_FIELD) {
    //sd.print("Mag:");
    x = event->magnetic.x;
    y = event->magnetic.y;
    z = event->magnetic.z;
  }
  else if (event->type == SENSOR_TYPE_GYROSCOPE) {
    //sd.print("Gyro:");
    x = event->gyro.x;
    y = event->gyro.y;
    z = event->gyro.z;
  }
  else if (event->type == SENSOR_TYPE_ROTATION_VECTOR) {
    //sd.print("Rot:");
    x = event->gyro.x;
    y = event->gyro.y;
    z = event->gyro.z;
  }
  else if (event->type == SENSOR_TYPE_LINEAR_ACCELERATION) {
    //sd.print("Linear:");
    x = event->acceleration.x;
    y = event->acceleration.y;
    z = event->acceleration.z;
  }
  else if (event->type == SENSOR_TYPE_GRAVITY) {
    //sd.print("Gravity:");
    x = event->acceleration.x;
    y = event->acceleration.y;
    z = event->acceleration.z;
  }
  else {
    sd.print("Unk:");
  }

  //sd.print("\tx= ");
  sd.print(x);
  sd.print(", ");
  //sd.print(" |\ty= ");
  sd.print(y);
  sd.print(", ");
  //sd.print(" |\tz= ");
  sd.println(z);
}
