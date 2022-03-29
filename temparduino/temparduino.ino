#define USE_ARDUINO_INTERRUPTS true // Set-up low-level interrupts for most acurate BPM math.
#include <PulseSensorPlayground.h> // Includes the PulseSensorPlayground Library.
#include <SoftwareSerial.h>

int tempReading;
int tempPin = 0;
const int PulseWire = 1; // PulseSensor PURPLE WIRE connected to ANALOG PIN 0
const int LED13 = 13; // The on-board Arduino LED, close to PIN 13.
int Threshold = 550;


PulseSensorPlayground pulseSensor;

SoftwareSerial espSerial(5, 6);
char floatString[5];
char str[75];

void setup()
{
  Serial.begin(115200);
  pulseSensor.analogInput(PulseWire);
  pulseSensor.blinkOnPulse(LED13); //auto-magically blink Arduino's LED with heartbeat.
  pulseSensor.setThreshold(Threshold);
  pulseSensor.begin();
  espSerial.begin(115200);
}

void loop()
{
  int myBPM = pulseSensor.getBeatsPerMinute(); // Calls function on our pulseSensor object that returns BPM as an "int".
  // "myBPM" hold this BPM value now.
  if (pulseSensor.sawStartOfBeat()) { // Constantly test to see if "a beat happened".

    Serial.println("♥ A HeartBeat Happened ! "); // If test is "true", print a message "a heartbeat happened".
    Serial.print("BPM: "); // Print phrase "BPM: "
    Serial.println(myBPM); // Print the value inside of myBPM.

    tempReading = analogRead(tempPin);
    float mv = ( tempReading / 1024.0) * 5000;
    float cel = mv / 10;
    
    Serial.print("TEMPRATURE = ");
    Serial.print(cel);
    Serial.print("*C");
    Serial.println();
    
//    str = String("coming from arduino: ") + String("BPM= ") + String(myBPM) + String("T= ") + String(cel);
    dtostrf(cel,4,2,floatString);
    snprintf (str, 35, "{\"temperature\": %s, \"bpm\": %i}\n", floatString, myBPM);
    espSerial.println(str);
  }

  delay(2000);


}
