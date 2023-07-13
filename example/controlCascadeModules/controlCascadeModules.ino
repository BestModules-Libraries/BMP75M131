/*************************************************
File:             controlCascadeModules.ino
Description:      Multiple relay modules are cascaded through IIC control, 
                  which are respectively used as a switch of led to control 
                  the light on and off of LED for flow light display.
Note:               
**************************************************/

#include <BMP75M131.h>
//BMP75M131 myRelay(&Wire);//intPin,Please comment out this line of code if you don't use Wire
BMP75M131 myRelay(&Wire1);//Please uncomment out this line of code if you use Wire1 on BMduino
//BMP75M131 myRelay(&Wire2);//intPin,Please comment out this line of code if you don't use Wire
uint8_t  moduleNumber;

void setup() 
{
  Serial.begin(9600);   //Serial debug output
  
  myRelay.begin();
  moduleNumber = myRelay.getNumber();
  if(moduleNumber == 0)
  {
    Serial.print("Get module number error.");
  }
  else
  {
    Serial.print("The modules number is: ");
    Serial.println(moduleNumber);
  }
}

void loop()
{ 
//  uint8_t  index;
//  for(index = 1; index <= moduleNumber; index++)
//  {
//    myRelay.setRelaySwitch(index, 1);
//    delay(1000);
//    myRelay.setRelaySwitch(index, 0);
//  }

    uint8_t tt[22]={};
    myRelay.setRelaySwitch(1, 1);
    delay(100);
    myRelay.setRelaySwitch(2, 1);
    delay(100);
    myRelay.setRelaySwitch(3, 1);
    delay(100);
    
    myRelay.getAllRelayStatus(tt);
    Serial.print("The modules AllRelayStatus is: ");
    for(uint8_t i = 0; i<moduleNumber;i++)
    {
      Serial.print(tt[i]);
    }
    Serial.println("");
Serial.print("The module 3 RelayStatus is: ");
    Serial.println(myRelay.getRelayStatus(3));
}
