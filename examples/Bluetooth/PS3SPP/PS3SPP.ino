/*
 Example sketch for the Bluetooth library - developed by Kristian Lauszus
 For more information visit my blog: http://blog.tkjelectronics.dk/ or 
 send me an e-mail:  kristianl@tkjelectronics.com
 */
 
/* 
 Note: 
 You will need a Arduino Mega 1280/2560 to run this sketch,
 As a normal Arduino (Uno, Duemilanove etc.) doesn't have enough SRAM and FLASH
 */

#include <PS3BT.h>
#include <SPP.h>
USB Usb;
BTD Btd(&Usb); // You have to create the Bluetooth Dongle instance like so

/* You can create the instances of the bluetooth services in two ways */
SPP SerialBT(&Btd); // This will set the name to the defaults: "Arduino" and the pin to "1234"
//SPP SerialBTBT(&Btd,"Lauszus's Arduino","0000"); // You can also set the name and pin like so
PS3BT PS3(&Btd); // This will just create the instance
//PS3BT PS3(&Btd,0x00,0x15,0x83,0x3D,0x0A,0x57); // This will also store the bluetooth address - this can be obtained from the dongle when running the sketch

boolean firstMessage = true;

String analogOutput; // We will store the data in these string so we doesn't overflow the dongle
String digitalOutput;

void setup() {
  Serial.begin(115200); // This wil lprint the debugging from the libraries
  if (Usb.Init() == -1) {
    Serial.print(F("\r\nOSC did not start"));
    while(1); //halt
  }
  Serial.print(F("\r\nBluetooth Library Started"));
}
void loop() {
  Usb.Task();

  if(SerialBT.connected) {
    if(firstMessage) {
      firstMessage = false;
      SerialBT.println(F("Hello from Arduino")); // Send welcome message
    }
    if(Serial.available())
      SerialBT.print(Serial.read());
    if(SerialBT.available())
      Serial.write(SerialBT.read());
  } 
  else 
    firstMessage = true;

  if(PS3.PS3Connected || PS3.PS3NavigationConnected) {    
    analogOutput = ""; // Reset analog output string
    if(PS3.getAnalogHat(LeftHatX) > 137 || PS3.getAnalogHat(LeftHatX) < 117 || PS3.getAnalogHat(LeftHatY) > 137 || PS3.getAnalogHat(LeftHatY) < 117 || PS3.getAnalogHat(RightHatX) > 137 || PS3.getAnalogHat(RightHatX) < 117 || PS3.getAnalogHat(RightHatY) > 137 || PS3.getAnalogHat(RightHatY) < 117) {
      if(PS3.getAnalogHat(LeftHatX) > 137 || PS3.getAnalogHat(LeftHatX) < 117) {
        analogOutput += "LeftHatX: ";
        analogOutput += PS3.getAnalogHat(LeftHatX);
        analogOutput += "\t";
      }
      if(PS3.getAnalogHat(LeftHatY) > 137 || PS3.getAnalogHat(LeftHatY) < 117) {   
        analogOutput += "LeftHatY: ";
        analogOutput += PS3.getAnalogHat(LeftHatY);
        analogOutput += "\t";
      } 
      if(PS3.getAnalogHat(RightHatX) > 137 || PS3.getAnalogHat(RightHatX) < 117) {
        analogOutput += "RightHatX: ";
        analogOutput += PS3.getAnalogHat(RightHatX);
        analogOutput += "\t";
      } 
      if(PS3.getAnalogHat(RightHatY) > 137 || PS3.getAnalogHat(RightHatY) < 117) {
        analogOutput += "RightHatY: ";
        analogOutput += PS3.getAnalogHat(RightHatY);
        analogOutput += "\t";
      }         
    }
    //Analog button values can be read from almost all buttons
    if(PS3.getAnalogButton(L2_ANALOG) || PS3.getAnalogButton(R2_ANALOG)) {
      if(analogOutput != "")
        analogOutput += "\r\n";
      if(PS3.getAnalogButton(L2_ANALOG)) {
        analogOutput += "L2: "; 
        analogOutput += PS3.getAnalogButton(L2_ANALOG);
        analogOutput += "\t";
      } 
      if(PS3.getAnalogButton(R2_ANALOG)) {
        analogOutput += "R2: "; 
        analogOutput += PS3.getAnalogButton(R2_ANALOG);
        analogOutput += "\t";
      }
    }
    if(analogOutput != "") {      
      Serial.println(analogOutput);
      if(SerialBT.connected)
        SerialBT.println(analogOutput);
    }

    if(PS3.buttonPressed) {
      digitalOutput = "PS3 Controller";
      if(PS3.getButton(PS)) {
        digitalOutput += " - PS";
        PS3.disconnect();
      } 
      else {
        if(PS3.getButton(TRIANGLE))
          digitalOutput += " - Traingle";
        if(PS3.getButton(CIRCLE))
          digitalOutput += " - Circle";
        if(PS3.getButton(CROSS))
          digitalOutput += " - Cross";
        if(PS3.getButton(SQUARE))
          digitalOutput += " - Square";

        if(PS3.getButton(UP)) {
          digitalOutput += " - UP";
          if(PS3.PS3Connected) {
            PS3.setAllOff();
            PS3.setLedOn(LED4);
          }
        } 
        if(PS3.getButton(RIGHT)) {
          digitalOutput += " - Right";
          if(PS3.PS3Connected) {
            PS3.setAllOff();
            PS3.setLedOn(LED1); 
          }         
        } 
        if(PS3.getButton(DOWN)) {
          digitalOutput += " - Down";          
          if(PS3.PS3Connected) {
            PS3.setAllOff();
            PS3.setLedOn(LED2);          
          }
        } 
        if(PS3.getButton(LEFT)) {  
          digitalOutput += " - Left";         
          if(PS3.PS3Connected) {
            PS3.setAllOff();         
            PS3.setLedOn(LED3);            
          }         
        } 

        if(PS3.getButton(L1))
          digitalOutput += " - L1";
        if(PS3.getButton(L2))
          digitalOutput += " - L2";
        if(PS3.getButton(L3))
          digitalOutput += " - L3";        
        if(PS3.getButton(R1))
          digitalOutput += " - R1";               
        if(PS3.getButton(R2))
          digitalOutput += " - R2";               
        if(PS3.getButton(R3))
          digitalOutput += " - R3";               

        if(PS3.getButton(SELECT))
          digitalOutput += " - Select";
        if(PS3.getButton(START))
          digitalOutput += " - Start";  
        
        Serial.println(digitalOutput);
        if(SerialBT.connected)
          SerialBT.println(digitalOutput);
      }             
    }
  }
}
