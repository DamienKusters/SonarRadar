//Created by Damien Kusters
//10-11-2017

#include <RotaryEncoderKusD.h>

RotaryEncoderKusD rotEnc;

void setup()
{
  //Initialize encoder. parameters:(byte button, byte clock, byte data)
  rotEnc.InitializeEncoderKusD(10,11,7);
  
  //Print the data to the serial monitor.
  Serial.begin(9600);
}

void loop()
{
  //rotEnc.GetClockValueKusD();//Returns the current value of the clock pin.
  //rotEnc.GetDataValueKusD();//Returns the current value of the data pin.
	
  //TIP: to make sure the library doesn't slow down your code: put all your code regarding the encoder direction in this statement: if(_encoderDirection != "")
  String _encoderDirection = rotEnc.GetDirectionKusD();//Returns (cw)Clockwise/(ccw)Counter Clockwise/()No String
  if(_encoderDirection != "")
  {
    Serial.println(_encoderDirection);
  }

  //Gets value of the button
  if(rotEnc.GetSwitchPressKusD())//Returns true/false
  {
    Serial.println("Button Pressed");
  }
}
