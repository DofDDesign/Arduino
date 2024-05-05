#include <dht11.h>
#include <CapacitiveSensor.h>
#define DHT11PIN 4
int DHTLoop;
#define POT1 A0
#define POT2 A1
#define CapLED 6
//Initialize the capacitive sensor. 8-send 9-receive
CapacitiveSensor Sensor = CapacitiveSensor(8, 9);
long val;
int CapLEDState;
dht11 DHT11;

void setup()
{
  Serial.begin(9600);
  pinMode(CapLED, OUTPUT);
  DHTLoop = 0;
  CapLEDState = 0;
}

void loop()
{
  //Increment the temperature sensor loop
  DHTLoop ++;
  //Read the current data from the potentiometers
  int data1 = analogRead(POT1);
  int data2 = analogRead(POT2);
  //Shift the max value of the potentiometers from 1023 to 255
  data1 = data1 >> 2;
  data2 = data2 >> 2;
  //Map the value of the potentiometers from 0-255 to 1-150
  data1 = map(data1, 0, 255, 1, 150);
  data2 = map(data2, 0, 255, 1, 150);
  //Print the value of the potentiometers
  Serial.print(F("Potentiometer 1: "));
  Serial.println(data1);
  Serial.print(F("Potentiometer 2: "));
  Serial.println(data2);
  //Read the current data from the capacitive sensor
  val = Sensor.capacitiveSensor(data1);
  //Constrain the value of the capacitive sensor to 0-150
  val = constrain(val, 0, 150);
  //Print the value of the capacitive sensor
  Serial.print(F("Capacitive Sensor: "));
  Serial.println(val);
  //If the value of the capacitive sensor is greater than or equal to the value of pot 2 and capstate is 1, dim the lights
  if (val >= data2 && CapLEDState == 1) {
    Serial.println(F("LEDs OFF"));
    CapLEDState = 0;
    for (int i = 255; i > 0; i--) {
      analogWrite(CapLED, i);
      delay(1);
    };
    analogWrite(CapLED, 0);
    delay(50);
  }
  //If the value of the capacitive sensor is greater than or equal to the value of pot 2 and capstate is 0, brighten the lights
  else if (val >= data2 && CapLEDState == 0) {
    CapLEDState = 1;
    Serial.println(F("LEDs ON"));
    for (int i = 0; i < 255; i++) {
      analogWrite(CapLED, i);
      delay(1);
    };
    analogWrite(CapLED, 255);
    delay(50);
  };
  //If we're on loop 10, more than a second has passed. read the value of the thermometer.
  if (DHTLoop == 10){
    DHTLoop = 0;
    int chk = DHT11.read(DHT11PIN);
    Serial.print(F("Humidity (%): "));
    Serial.println((float)DHT11.humidity, 2);
    Serial.print(F("Temperature  (F): "));
    Serial.println(((float)DHT11.temperature * 9/5) + 32, 2);
  };
  delay(100);

}