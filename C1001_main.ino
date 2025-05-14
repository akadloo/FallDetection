#include "DFRobot_HumanDetection.h"


#define PRESENCE_OUTPUT_PIN 10  // D10 


DFRobot_HumanDetection hu(&Serial1);


void setup() {
 Serial.begin(115200);


 #if defined(ESP32)
 Serial1.begin(115200, SERIAL_8N1, /*rx =*/D3, /*tx =*/D2);
 #else
 Serial1.begin(115200);
 #endif


 pinMode(PRESENCE_OUTPUT_PIN, OUTPUT);       // Set D10 as output
 digitalWrite(PRESENCE_OUTPUT_PIN, LOW);     // Initialize to LOW


 Serial.println("Start initialization");
 while (hu.begin() != 0) {
   Serial.println("init error!!!");
   delay(1000);
 }
 Serial.println("Initialization successful");


 Serial.println("Start switching work mode");
 while (hu.configWorkMode(hu.eFallingMode) != 0) {
   Serial.println("error!!!");
   delay(1000);
 }
 Serial.println("Work mode switch successful");


 hu.configLEDLight(hu.eFALLLed, 1);
 hu.configLEDLight(hu.eHPLed, 1);
 hu.dmInstallHeight(270);
 hu.dmFallTime(5);
 hu.dmUnmannedTime(1);
 hu.dmFallConfig(hu.eResidenceTime, 200);
 hu.dmFallConfig(hu.eFallSensitivityC, 3);
 hu.sensorRet();


 Serial.print("Current work mode:");
 switch (hu.getWorkMode()) {
   case 1:
     Serial.println("Fall detection mode");
     break;
   case 2:
     Serial.println("Sleep detection mode");
     break;
   default:
     Serial.println("Read error");
 }
}


void loop() {
 int presence = hu.smHumanData(hu.eHumanPresence);
 Serial.print("Existing information:");
 switch (presence) {
   case 0:
     Serial.println("No one is present");
     break;
   case 1:
     Serial.println("Someone is present");
     break;
   default:
     Serial.println("Read error");
 }


 Serial.print("Motion information:");
 switch (hu.smHumanData(hu.eHumanMovement)) {
   case 0:
     Serial.println("None");
     break;
   case 1:
     Serial.println("Still");
     break;
   case 2:
     Serial.println("Active");
     break;
   default:
     Serial.println("Read error");
 }


 Serial.print("Body movement parameters: ");
 Serial.print( hu.smHumanData(hu.eHumanMovingRange));


 Serial.print("Fall status:");
 switch (hu.getFallData(hu.eFallState)) {
   case 0:
     Serial.println("Not fallen");
     digitalWrite(PRESENCE_OUTPUT_PIN, LOW);  // Set D10 LOW
     break;
   case 1:
     Serial.println("Fallen");
     digitalWrite(PRESENCE_OUTPUT_PIN, HIGH); // Set D10 HIGH
     break;
   default:
     Serial.println("Read error");
     digitalWrite(PRESENCE_OUTPUT_PIN, LOW);  // Set D10 LOW


 }


 Serial.print("Stationary dwell status: ");
 switch (hu.getFallData(hu.estaticResidencyState)) {
   case 0:
     Serial.println("No stationary dwell");
     break;
   case 1:
     Serial.println("Stationary dwell present");
     break;
   default:
     Serial.println("Read error");
 }
 Serial.println();
 delay(1000);
}
