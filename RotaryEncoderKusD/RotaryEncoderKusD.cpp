//Created by Damien Kusters
//Date: 9 nov 2017

//Include the header file
#include "RotaryEncoderKusD.h"

#include <String.h>

//Constructor Needs variable
RotaryEncoderKusD::RotaryEncoderKusD()
{
	encoderBlockKusD = false;
}

void RotaryEncoderKusD::InitializeEncoderKusD(byte _pinData, byte _pinClock, byte _pinSwitch)
{
	//Assign variables with values
	pinDataKusD = _pinData;
	pinClockKusD = _pinClock;
	pinSwitchKusD = _pinSwitch;
	
	pinMode(pinDataKusD, INPUT);
	pinMode(pinClockKusD, INPUT);
	pinMode(pinSwitchKusD, INPUT);
}

bool RotaryEncoderKusD::GetSwitchPressKusD()
{
  if(digitalRead(pinSwitchKusD) == LOW)
  {
	  return true;
  }
  else
  {
	  return false;
  }
}

bool RotaryEncoderKusD::GetDataValueKusD()
{
	return digitalRead(pinDataKusD);
}

bool RotaryEncoderKusD::GetClockValueKusD()
{
	return digitalRead(pinClockKusD);
}

String RotaryEncoderKusD::GetDirectionKusD()
{
	int _data = digitalRead(pinDataKusD);
	int _clock = digitalRead(pinClockKusD);
	
	if(!encoderBlockKusD && (_clock == 0))
	{
		String _result;
		
		if(_data == 0)
		{
		  //Counter Clockwise
		  _result = "ccw";
		}
		else
		{
		  //Clockwise
		  _result = "cw";
		}
		
		encoderBlockKusD = true;
		
		return _result;
	}
	if(_data == 1 && _clock == 1)
	{
		encoderBlockKusD = false;
	}
	
	return "";
}