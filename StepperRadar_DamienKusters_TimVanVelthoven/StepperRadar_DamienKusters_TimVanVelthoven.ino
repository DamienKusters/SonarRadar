//Made by Damien Kusters
//And Tim van Velthoven

#include <EEPROM.h>
#include <RotaryEncoderKusD.h>
#include <Timer.h>

RotaryEncoderKusD rotEncDkTv;
Timer tmrSonarDkTv;

const byte doSaveLedDkTv = 12;
const byte doBaseLedDkTv = 6;

//Define stepper controls
//const can't change
const byte doStepperStepDkTv = 9;
const byte doStepperDirDkTv = 8;

//Define sensor I/O
const byte doSonarOutputDkTv = 4;
const byte diSonarInputDkTv = 5;
long sonarDurationDkTv;

//Rotary encoder
const byte diRencButtonDkTv = 7;
const byte diRencDataDkTv = 10;
const byte diRencClickDkTv = 11;
bool encoderBlock = false;

//EEPROM variables and addresses
byte maxEepromValuesDkTv = 3;
byte addrAngleMaxDkTv = 0;
byte addrAngleBaseDkTv = 1;
byte addrAngleCurrentDkTv = 2;

//Save these values
int angleMaxDkTv = 1000;//Load value from rotary encoder and save position / Get value from C# application
int angleBaseDkTv = 0;//default: 0 (is 0 point) below 0 is an underflow, we don't want that
int angleCurrentDkTv = 0;//Current value of the angle
int dirBackwardDkTv = false;//Bool that toggles wich direction the radar is going

//#define debugMode

void setup()
{
  //Attach Stepper
  pinMode(doStepperDirDkTv, OUTPUT);
  pinMode(doStepperStepDkTv, OUTPUT);

  pinMode(doSaveLedDkTv, OUTPUT);
  pinMode(doBaseLedDkTv, OUTPUT);

  //Init Stepper
  digitalWrite(doStepperDirDkTv, LOW);
  digitalWrite(doStepperStepDkTv, LOW);

  //Set sensor I/O
  pinMode(doSonarOutputDkTv, OUTPUT);
  pinMode(diSonarInputDkTv, INPUT);

  //Init timer based sonar
  tmrSonarDkTv.every(30, CalculateDistanceDkTv);

  //Init encoder
  rotEncDkTv.InitializeEncoderKusD(diRencDataDkTv,diRencClickDkTv,diRencButtonDkTv);
  
  //Start serial monitor
  Serial.begin(9600);

  InitVariablesDkTv();

  //Following code is for debug testing
  #ifdef debugMode
    Serial.println("--Debug Mode--");

    
    Serial.println("init finished..");
  #endif
}

void InitVariablesDkTv()
{
  angleCurrentDkTv = ReadValueDkTv(addrAngleCurrentDkTv);
  angleMaxDkTv = ReadValueDkTv(addrAngleMaxDkTv);

  #ifdef debugMode
    Serial.print("Current: ");
    Serial.println(angleCurrentDkTv);
    Serial.print("max: ");
    Serial.println(angleMaxDkTv);
  #endif
  
  if(angleMaxDkTv == 0)
  {
    digitalWrite(doBaseLedDkTv, HIGH);
  }
}

void loop()
{
  //Move motor
  MotorTickDkTv(2000);

  //Check/change direction of motor
  MotorCheckDirectionDkTv();

  //Fetch encoder values and change variables based on input
  EncoderCheckDkTv();

  #ifndef debugMode
    tmrSonarDkTv.update();//Distance what the sonar sees
  #endif
  
  //Following code is for debug testing
  #ifdef debugMode
    
    //TEMP code for testing saving to EEPROM
    //SaveValueDkTv(addrAngleMaxDkTv, angleMaxDkTv);
    //SaveValueDkTv(addrAngleBaseDkTv, angleBaseDkTv);
    //SaveValueDkTv(addrAngleCurrentDkTv, angleCurrentDkTv);
    
    //Serial.print("angleCurrentDkTv : ");
    //Serial.println(ReadValueDkTv(addrAngleCurrentDkTv));
  #endif
}

void SaveValueDkTv(byte _addr, int _val)
{
  //Thanks to Jake van Hout
  //for explaining the concept of bitmasking/bitshifting
  
  byte _lsbVal;//Least Significant Byte 
  byte _msbVal;//Most  ''

  _lsbVal = (_val >> 8);//Left chunk of bits
  _msbVal = _val;//Right chunk of bits

  EEPROM.write(_addr, _lsbVal);
  EEPROM.write(_addr + maxEepromValuesDkTv, _msbVal);
}

int ReadValueDkTv(byte _addr)
{
  return (int)EEPROM.read(_addr) << 8 | (int)EEPROM.read(_addr + maxEepromValuesDkTv);
}

void MotorTickDkTv(int _tick)
{
  digitalWrite(doStepperStepDkTv, HIGH);
  delayMicroseconds(_tick); 
  digitalWrite(doStepperStepDkTv, LOW);
  delayMicroseconds(_tick);
}

void MotorCheckDirectionDkTv()
{
  if(angleCurrentDkTv >= angleMaxDkTv)
  {
    digitalWrite(doStepperDirDkTv, HIGH);
    dirBackwardDkTv = true;
  }
  if(angleCurrentDkTv <= angleBaseDkTv)
  {
    digitalWrite(doStepperDirDkTv, LOW);
    dirBackwardDkTv = false;
  }

  if(dirBackwardDkTv)
  {
    angleCurrentDkTv--;
  }
  else
  {
    angleCurrentDkTv++;
  }
}

void EncoderCheckDkTv()
{
  //This function uses my own RotaryEncoderKusD library.
  
  //Gets value of the button
  if(rotEncDkTv.GetSwitchPressKusD())//Returns true/false
  {
    digitalWrite(doSaveLedDkTv, HIGH);
    
    //Save the new location
    SaveValueDkTv(addrAngleCurrentDkTv, angleCurrentDkTv);

    //Save the new angle max value to the EEPROM
    SaveValueDkTv(addrAngleMaxDkTv, angleMaxDkTv);
  }
  else
  {
    digitalWrite(doSaveLedDkTv, LOW);
  }
  
  //Returns (cw)Clockwise/(ccw)Counter Clockwise/()No String
  String _encoderDirection = rotEncDkTv.GetDirectionKusD();
  if(_encoderDirection != "")
  {
    if(_encoderDirection == "cw")
    {
      angleMaxDkTv = angleMaxDkTv + 100;
    }
    else
    {
      angleMaxDkTv = angleMaxDkTv - 100;
    }

    if(angleMaxDkTv <= 0)
    {
      digitalWrite(doBaseLedDkTv, HIGH);
      angleMaxDkTv = 0;
    }
    else
    {
      digitalWrite(doBaseLedDkTv, LOW);
    }
    if(angleMaxDkTv > 1800)
    {
      angleMaxDkTv = 1800;
    }
  
    #ifdef debugMode
      Serial.print("Max: ");
      Serial.println(angleMaxDkTv);
    #endif
  }
}

void CalculateDistanceDkTv()
{
  digitalWrite(doSonarOutputDkTv, LOW);
  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(doSonarOutputDkTv, HIGH); 
  digitalWrite(doSonarOutputDkTv, LOW);
  sonarDurationDkTv = pulseIn(diSonarInputDkTv, HIGH); // Reads the echoPin, returns the sound wave travel time in microseconds
  #ifndef debugMode
    Serial.print("sonar:  ");
    Serial.println((int)sonarDurationDkTv*0.034/2);
  #endif
}
