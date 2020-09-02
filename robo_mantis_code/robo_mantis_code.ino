#include <Wire.h>
byte outputArray[10];

#include <Servo.h>

Servo myservo1;//lift
Servo myservo2;//rotate claw
Servo myservo3;//claw
Servo myservo4;//main lift

byte servoByte1 = 100;
byte servoByte2 = 90;
byte servoByte3 = 50;
byte servoByte4 = 50;

#define PI_ENABLE_PIN 34
#define PI_SHARED_PIN 35

#define M1_PWM  6
#define M1_A  22
#define M1_B  23
#define M1_CURR_SENSE 56

#define M2_PWM  7
#define M2_A  25
#define M2_B  24
#define M2_CURR_SENSE 57

#define M3_PWM  8
#define M3_A  26
#define M3_B  27
#define M3_CURR_SENSE 58

#define M4_PWM  9
#define M4_A  29
#define M4_B  28
#define M4_CURR_SENSE 59

#define M5_PWM  10
#define M5_A  30
#define M5_B  31
#define M5_CURR_SENSE 60

#define M6_PWM  11
#define M6_A  33
#define M6_B  32
#define M6_CURR_SENSE 61


void setup() {

  // Set up I2C communication
  Wire.begin(9);
  Wire.onReceive(receiveData);
  Wire.onRequest(sendData);
  Wire.setClock(1000000);


  myservo1.attach(2);
  myservo2.attach(3);
  myservo3.attach(4);
  myservo4.attach(5);

  myservo1.write(100);//lift
  myservo2.write(90);//rotate claw
  myservo3.write(50);//claw
  myservo4.write(50);//main lift

  //initiate all the pins for the motors
  pinMode(M1_PWM, OUTPUT);
  pinMode(M1_A, OUTPUT);
  pinMode(M1_B, OUTPUT);

  pinMode(M2_PWM, OUTPUT);
  pinMode(M2_A, OUTPUT);
  pinMode(M2_B, OUTPUT);

  pinMode(M3_PWM, OUTPUT);
  pinMode(M3_A, OUTPUT);
  pinMode(M3_B, OUTPUT);

  pinMode(M4_PWM, OUTPUT);
  pinMode(M4_A, OUTPUT);
  pinMode(M4_B, OUTPUT);

  pinMode(M5_PWM, OUTPUT);
  pinMode(M5_A, OUTPUT);
  pinMode(M5_B, OUTPUT);

  pinMode(M6_PWM, OUTPUT);
  pinMode(M6_A, OUTPUT);
  pinMode(M6_B, OUTPUT);

  pinMode(PI_ENABLE_PIN, OUTPUT);

  //Allow the pi to continue
  digitalWrite(PI_ENABLE_PIN, LOW);

  Serial.begin(115200);
  Serial.println("Starting Robo-Mantis");
}

void motorStop()
{
  motorMove(1, 0);
  motorMove(2, 0);
  motorMove(3, 0);
  motorMove(4, 0);
  motorMove(5, 0);
  motorMove(6, 0);
}

void motorMove(byte motorNumber, int set_speed)
{
  //set speed can be a number between -255 to 255
  byte pwmPin, maPin, mbPin;

  switch (motorNumber)
  {
    case 1:
      pwmPin = M1_PWM;
      maPin = M1_A;
      mbPin = M1_B;
      break;
    case 2:
      pwmPin = M2_PWM;
      maPin = M2_A;
      mbPin = M2_B;
      break;
    case 3:
      pwmPin = M3_PWM;
      maPin = M3_A;
      mbPin = M3_B;
      break;
    case 4:
      pwmPin = M4_PWM;
      maPin = M4_A;
      mbPin = M4_B;
      break;
    case 5:
      pwmPin = M5_PWM;
      maPin = M5_A;
      mbPin = M5_B;
      break;
    case 6:
      pwmPin = M6_PWM;
      maPin = M6_A;
      mbPin = M6_B;
      break;

    default:
      break;
  }

  if (set_speed >= 0)
  {
    digitalWrite(mbPin, LOW);
    digitalWrite(maPin, HIGH);

  }
  else if (set_speed < 0)
  {
    digitalWrite(maPin, LOW);
    digitalWrite(mbPin, HIGH);
  }

  analogWrite(pwmPin, abs(set_speed));

}

void motorTest(byte motorNumber, unsigned int delay_time)
{
  byte pwmPin, maPin, mbPin;

  switch (motorNumber)
  {
    case 1:
      pwmPin = M1_PWM;
      maPin = M1_A;
      mbPin = M1_B;
      break;

    case 2:
      pwmPin = M2_PWM;
      maPin = M2_A;
      mbPin = M2_B;
      break;

    case 3:
      pwmPin = M3_PWM;
      maPin = M3_A;
      mbPin = M3_B;
      break;

    case 4:
      pwmPin = M4_PWM;
      maPin = M4_A;
      mbPin = M4_B;
      break;

    case 5:
      pwmPin = M5_PWM;
      maPin = M5_A;
      mbPin = M5_B;
      break;

    case 6:
      pwmPin = M6_PWM;
      maPin = M6_A;
      mbPin = M6_B;
      break;

    default:
      break;
  }


  digitalWrite(maPin, HIGH);
  digitalWrite(mbPin, LOW);

  for (int i = 0; i < 255; i++)
  {
    Serial.println(i);
    analogWrite(pwmPin, i);
    delay(delay_time);
  }
  for (int i = 255; i > 0; i--)
  {
    Serial.println(i);
    analogWrite(pwmPin, i);
    delay(delay_time);
  }

  delay(1000);

  digitalWrite(maPin, LOW);
  digitalWrite(mbPin, HIGH);

  for (int i = 0; i < 255; i++)
  {
    Serial.println(i);
    analogWrite(pwmPin, i);
    delay(delay_time);
  }
  for (int i = 255; i > 0; i--)
  {
    Serial.println(i);
    analogWrite(pwmPin, i);
    delay(delay_time);
  }
  delay(1000);

}

// the loop routine runs over and over again forever:
void loop() {
  receiveData();

  //motorTest(5,1);

//    for (int i = 0; i < 6; i++)
//    {
//      motorTest(i+1,1);
//    }
}


void receiveData() {
  //Commands are given as a single byte, where the bits are parsed as shown:
  // XXYYYZZZ 1 BYTE

  if (Wire.available())
  {
    byte receivedByte = Wire.read();
    Serial.print(receivedByte);
    switch (receivedByte)
    {
      case 0:
        motorStop();
        break;
      case 1:
        byte speedByteM1 = Wire.read();
        byte speedByteM2 = Wire.read();
        byte speedByteM3 = Wire.read();
        byte speedByteM4 = Wire.read();
        byte speedByteM5 = Wire.read();
        byte speedByteM6 = Wire.read();
        Serial.print(" ");
        Serial.print(speedByteM1);
        Serial.print(" ");
        Serial.print(speedByteM2);
        Serial.print(" ");
        Serial.print(speedByteM3);
        Serial.print(" ");
        Serial.print(speedByteM4);
        Serial.print(" ");
        Serial.print(speedByteM5);
        Serial.print(" ");
        Serial.print(speedByteM6);

        motorMove(1, 2 * (speedByteM1 - 127));
        motorMove(2, 2 * (speedByteM2 - 127));
        motorMove(3, 2 * (speedByteM3 - 127));
        motorMove(4, 2 * (speedByteM4 - 127));
        motorMove(5, 2 * (speedByteM5 - 127));
        motorMove(6, 2 * (speedByteM6 - 127));

        break;
      case 2:
        servoByte1 = Wire.read();
//        servoByte2 = Wire.read();
//        servoByte3 = Wire.read();
//        servoByte4 = Wire.read();
        Serial.print(" ");
        Serial.print(servoByte1);
//        Serial.print(" ");
//        Serial.print(servoByte2);
//        Serial.print(" ");
//        Serial.print(servoByte3);
//        Serial.print(" ");
//        Serial.print(servoByte4);
        myservo1.write(servoByte1);
//        myservo2.write(servoByte2);
//        myservo3.write(servoByte3);
//        myservo4.write(servoByte4);

        break;
      case 3:
        break;
      default:
        break;
    }
    Serial.print(" end\n");
  }
}

void sendData() {
  Wire.write(outputArray, 10);  // Send IR sensor data and current batt voltage in one block
}
