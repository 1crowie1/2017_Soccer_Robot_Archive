#include <Wire.h>

/*
  IRSeeker.ino - A library/class for the HiTechnic IRSeeker V2 infrared sensor.
  Created by B. Blechschmidt, August 1, 2013.
  Released into the public domain.
*/

struct InfraredResult
{
  byte Direction;
  byte Strength;
};

class InfraredSeeker
{
  public:
    static void Initialize();
    static boolean Test();
    static void ReadACRaw(byte* buffer);
    static void ReadDCRaw(byte* buffer);
    static InfraredResult ReadAC();
    static InfraredResult ReadDC();
    static int DirectionAngle(byte Direction);
  private:
    static InfraredResult PopulateValues(byte* buffer);
    static void ReadValues(byte OffsetAddress, byte* buffer);
    static const int Address = 0x10 / 2; //Divide by two as 8bit-I2C address is provided
};

void InfraredSeeker::Initialize()
{
  Wire.begin();
  Wire.beginTransmission(InfraredSeeker::Address);
  Wire.write(0x00);
  Wire.endTransmission();
  while(Wire.available() > 0)
    Wire.read();
}

boolean InfraredSeeker::Test()
{
  Wire.beginTransmission(InfraredSeeker::Address);
  Wire.write(0x08);
  Wire.endTransmission();
  Wire.requestFrom(InfraredSeeker::Address, 16);
  char Manufacturer_Model[16];
  while(Wire.available() < 16);
  for(byte i=0; i < 16; i++)
  {
    Manufacturer_Model[i] = Wire.read();
  }
  while(Wire.available() > 0)
    Wire.read();
  return strncmp(Manufacturer_Model, "HiTechncNewIRDir", 16)==0;
}

void InfraredSeeker::ReadValues(byte OffsetAddress, byte* buffer)
{
  Wire.beginTransmission(InfraredSeeker::Address);
  Wire.write(OffsetAddress);
  Wire.endTransmission();
  Wire.requestFrom(InfraredSeeker::Address, 6);
  while(Wire.available() < 6);
  for(byte i = 0; i < 6; i++)
  {
    buffer[i] = Wire.read();
  }
  while(Wire.available() > 0)
    Wire.read();
}

void InfraredSeeker::ReadACRaw(byte* buffer)
{
  ReadValues(0x49, buffer);
}

void InfraredSeeker::ReadDCRaw(byte* buffer)
{
  ReadValues(0x42, buffer);
}

InfraredResult InfraredSeeker::PopulateValues(byte* buffer)
{
  InfraredResult Data;
  Data.Direction = buffer[0];
  if(buffer[0] != 0)
  {
    if(buffer[0] % 2 == 0)
    {
      Data.Strength = (buffer[buffer[0] / 2] + buffer[buffer[0] / 2 + 1]) / 2;
    }
    else
    {
      Data.Strength = buffer[buffer[0] / 2 + 1];
    }
  }
  else
  {
    Data.Strength = 0;
  }
  return Data;
}

InfraredResult InfraredSeeker::ReadAC()
{
  byte buffer[6];
  ReadACRaw(buffer);
  return PopulateValues(buffer);
}

InfraredResult InfraredSeeker::ReadDC()
{
  byte buffer[6];
  ReadDCRaw(buffer);
  return PopulateValues(buffer);
}

int DirectionAngle(byte Direction)
{
  return Direction * 30 - 150;
}

// motor front left
static int enA = 4;
static int in1 = 27;
static int in2 = 26;
// motor back left
static int in3 = 29;
static int in4 = 28;
static int enB = 5;
// motor back right
static int enC = 3;
static int in5 = 24;
static int in6 = 25;
// motor front right
static int in7 = 22;
static int in8 = 23;
static int enD = 2;




void drive_forward(int tdelay) {
  //FORWARD
  //fl bw
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  analogWrite(enA, 122);
  //bl bw
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
  analogWrite(enB, 122);
  //br fw
  digitalWrite(in5, HIGH);
  digitalWrite(in6, LOW);
  analogWrite(enC, 100);
  //fr fw
  digitalWrite(in7, HIGH);
  digitalWrite(in8, LOW);
  analogWrite(enD, 122);
  delay(tdelay);
 }

void drive_backwards(int tdelay) {
  //BACKWARDS
  //fl fw
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
  analogWrite(enA, 122);
  //bl fw
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);
  analogWrite(enB, 122);
  //br bw
  digitalWrite(in5, LOW);
  digitalWrite(in6, HIGH);
  analogWrite(enC, 100);
  //fr bw
  digitalWrite(in7, LOW);
  digitalWrite(in8, HIGH);
  analogWrite(enD, 122);
  delay(tdelay);
}

void drive_right(int tdelay) {
  //RIGHT
  //fl bw
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  analogWrite(enA, 142);
  //bl fw
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);
  analogWrite(enB, 122);
  //br fw
  digitalWrite(in5, HIGH);
  digitalWrite(in6, LOW);
  analogWrite(enC, 100);
  //fr bw
  digitalWrite(in7, LOW);
  digitalWrite(in8, HIGH);
  analogWrite(enD, 122);
  delay(tdelay);
}

void drive_left(int tdelay) {
  //LEFT
  //fl fw
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
  analogWrite(enA, 142);
  //bl bw
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
  analogWrite(enB, 122);
  //br bw
  digitalWrite(in5, LOW);
  digitalWrite(in6, HIGH);
  analogWrite(enC, 100);
  //fr fw
  digitalWrite(in7, HIGH);
  digitalWrite(in8, LOW);
  analogWrite(enD, 122);
  delay(tdelay);
}

void drive_diag_left(int tdelay) {
  //DIAG FORWARDS LEFT
  //fl fw
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  analogWrite(enA, 0);
  //bl fw
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
  analogWrite(enB, 122);
  //br bw
  digitalWrite(in5, HIGH);
  digitalWrite(in6, LOW);
  analogWrite(enC, 0);
  //fr bw
  digitalWrite(in7, HIGH);
  digitalWrite(in8, LOW);
  analogWrite(enD, 122);
  delay(tdelay);
}

void drive_diag_right(int tdelay) {
  //DIAG FORWARDS RIGHT
  //fl fw
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  analogWrite(enA, 122);
  //bl fw
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);
  analogWrite(enB, 0);
  //br bw
  digitalWrite(in5, HIGH);
  digitalWrite(in6, LOW);
  analogWrite(enC, 100);
  //fr bw
  digitalWrite(in7, LOW);
  digitalWrite(in8, HIGH);
  analogWrite(enD, 0);
  delay(tdelay);
}

void drive_diag_bl(int tdelay) {
  //DIAG BACKWARDS LEFT
  //fl fw
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
  analogWrite(enA, 0);
  //bl fw
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);
  analogWrite(enB, 122);
  //br bw
  digitalWrite(in5, LOW);
  digitalWrite(in6, HIGH);
  analogWrite(enC, 0);
  //fr bw
  digitalWrite(in7, LOW);
  digitalWrite(in8, HIGH);
  analogWrite(enD, 122);
  delay(tdelay);
}

void drive_diag_br(int tdelay) {
  //DIAG BACKWARDS RIGHT
  //fl fw
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
  analogWrite(enA, 122);
  //bl fw
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);
  analogWrite(enB, 0);
  //br bw
  digitalWrite(in5, LOW);
  digitalWrite(in6, HIGH);
  analogWrite(enC, 100);
  //fr bw
  digitalWrite(in7, LOW);
  digitalWrite(in8, HIGH);
  analogWrite(enD, 0);
  delay(tdelay);
}

void turn_left(int tdelay) {
  //left
  //fl bw
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
  analogWrite(enA, 72);
  //bl bw
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);
  analogWrite(enB, 72);
  //br fw
  digitalWrite(in5, HIGH);
  digitalWrite(in6, LOW);
  analogWrite(enC, 72);
  //fr fw
  digitalWrite(in7, HIGH);
  digitalWrite(in8, LOW);
  analogWrite(enD, 72);
  delay(tdelay);
}

void turn_right(int tdelay) {
  //right
  //fl bw
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  analogWrite(enA, 72);
  //bl bw
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
  analogWrite(enB, 72);
  //br fw
  digitalWrite(in5, LOW);
  digitalWrite(in6, HIGH);
  analogWrite(enC, 72);
  //fr fw
  digitalWrite(in7, LOW);
  digitalWrite(in8, HIGH);
  analogWrite(enD, 72);
  delay(tdelay);
}




void backside(int tdelay) {
  //ir motor code and factor in light sensor once andy has finished it
}

#define CompassAddress 0x1E // i2c address
#define CompassMode 0x02 // angle
#define CompassReadMode 0x00 // cotinuous scanning 
#define CompassRegister 0x03
#define CompassByteLength 6

int CompassX;
int CompassY;
int CompassZ;
int CompassStartHeadingZ;

void CompassInit() {
  Wire.begin();
  Wire.beginTransmission(CompassAddress);
  Wire.write(CompassMode);
  Wire.write(CompassReadMode);
  Wire.endTransmission();
  Wire.beginTransmission(CompassAddress);
  Wire.write(CompassRegister);
  Wire.endTransmission();

  
 Wire.requestFrom(CompassAddress, CompassByteLength);

 if(CompassByteLength<=Wire.available()) {
  CompassStartHeadingZ = Wire.read()<<8;
  CompassStartHeadingZ |= Wire.read();
 }

 Serial.print("Original heading: ");
 Serial.print(CompassStartHeadingZ);
 
 Serial.println();
 
}

void getCompassData() {
 Wire.beginTransmission(CompassAddress);
 Wire.write(CompassRegister);
 Wire.endTransmission();
  
 Wire.requestFrom(CompassAddress, CompassByteLength);

 if(CompassByteLength<=Wire.available()) {
  CompassX = Wire.read()<<8;
  CompassX |= Wire.read();
  
  CompassZ = Wire.read()<<8;
  CompassZ |= Wire.read();
  
  CompassY = Wire.read()<<8;
  CompassY |= Wire.read();
 }
}



void setup()
{
  Serial.begin(9600);
  InfraredSeeker::Initialize();
  CompassInit();
  Serial.println("HiTechnic IRSeeker V2");
  Serial.println();
  Serial.println();
  Serial.println("Dir\tAngle\tStrength");
  Serial.println();

  //motor fl
  pinMode(enA, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  //motor bl
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);
  pinMode(enB, OUTPUT);
  //motor br
  pinMode(enC, OUTPUT);
  pinMode(in5, OUTPUT);
  pinMode(in6, OUTPUT);
  //motor fl
  pinMode(in7, OUTPUT);
  pinMode(in8, OUTPUT);
  pinMode(enD, OUTPUT);


}

void loop()
{  
  InfraredResult InfraredBall = InfraredSeeker::ReadAC();
  getCompassData();
  Serial.print(InfraredBall.Direction);
  Serial.print("\t");
  Serial.print(DirectionAngle(InfraredBall.Direction));
  Serial.print("\t");
  Serial.print(InfraredBall.Strength);
  Serial.println(); 
  Serial.println(CompassX);
  delay(10); //optional






  if (CompassX >= (CompassStartHeadingZ + 80)) {
    turn_left(5);
    Serial.println("Turning Left");
  }
  else if (CompassX <= (CompassStartHeadingZ - 50)) {
    turn_right(5);
    Serial.println("Turning Right");
  }
  else if (InfraredBall.Direction == 5) {
    Serial.print(InfraredBall.Direction);
    Serial.print("\t");
    drive_forward(5);
  }
  else if (InfraredBall.Direction == 6) {
    Serial.print(InfraredBall.Direction);
    Serial.print("\t");
    drive_diag_right(5);
  }
  else if (InfraredBall.Direction == 4) {
    Serial.print(InfraredBall.Direction);
    Serial.print("\t");
    drive_diag_left(5);
  }
  else if (InfraredBall.Direction == 3) {
    Serial.print(InfraredBall.Direction);
    Serial.print("\t");
    drive_left(5);
  }
  else if (InfraredBall.Direction == 7) {
    Serial.print(InfraredBall.Direction);
    Serial.print("\t");
    drive_right(5);
  }
  else if (InfraredBall.Direction == 8) {
    Serial.print(InfraredBall.Direction);
    Serial.print("\t");
    drive_backwards(5);
  }
  else if (InfraredBall.Direction == 9) {
    Serial.print(InfraredBall.Direction);
    Serial.print("\t");
    drive_diag_bl(5);
  }
  else if (InfraredBall.Direction == 1) {
    Serial.print(InfraredBall.Direction);
    Serial.print("\t");
    drive_diag_br(5);
  }
  else if (InfraredBall.Direction == 2) {
    Serial.print(InfraredBall.Direction);
    Serial.print("\t");
    drive_backwards(5);
  }
  else if (InfraredBall.Direction == 0) {
    Serial.print(InfraredBall.Direction);
    Serial.print("\t");
    drive_backwards(5);
  }

  
}
