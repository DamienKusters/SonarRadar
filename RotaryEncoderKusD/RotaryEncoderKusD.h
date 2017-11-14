//Created by Damien Kusters
//Date: 9 nov 2017

//Header guard, prevents double defenition
//If not defined: define, else end file
#ifndef RotaryEncoderKusD_h
#define RotaryEncoderKusD_h

//Include arduino
#include <Arduino.h>
#include <String.h>

class RotaryEncoderKusD
{
  public:
    RotaryEncoderKusD();//Nothing in front = Constructor
	
	void InitializeEncoderKusD(byte _pinData, byte _pinClock, byte _pinSwitch);
    bool GetSwitchPressKusD();//Method/function
    bool GetDataValueKusD();
	bool GetClockValueKusD();
	String GetDirectionKusD();
  private:
    byte pinDataKusD;
	byte pinClockKusD;
	byte pinSwitchKusD;
	bool encoderBlockKusD;
};//.h Needs ';' after classes

#endif
