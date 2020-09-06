#include <Wire.h>
#include <Servo.h>

Servo myservo1;//lift
Servo myservo2;//rotate claw
Servo myservo3;//claw
Servo myservo4;//main lift

#define ZERO_POS1 100
#define ZERO_POS2 90
#define ZERO_POS3 50
#define ZERO_POS4 50

//maximum amount of milliamps before shutting motors down
#define MAX_CURRENT_DRAW 5000

#define OUT_ARR_LENGTH 10

#define SERVO1_PIN 2
#define SERVO2_PIN 3
#define SERVO3_PIN 4
#define SERVO4_PIN 5

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

#define SLAVE_I2C_ADDRESS 9
#define I2C_FREQ 1000000
#define BAUD_RATE 115200

byte servoByte1 = ZERO_POS1;
byte servoByte2 = ZERO_POS2;
byte servoByte3 = ZERO_POS3;
byte servoByte4 = ZERO_POS4;

byte speedByteM1 = 0;
byte speedByteM2 = 0;
byte speedByteM3 = 0;
byte speedByteM4 = 0;
byte speedByteM5 = 0;
byte speedByteM6 = 0;

float motor1milliAmps = 0;
float motor2milliAmps = 0;
float motor3milliAmps = 0;
float motor4milliAmps = 0;
float motor5milliAmps = 0;
float motor6milliAmps = 0;
float motorTotalmilliAmps = 0;

byte outputArray[OUT_ARR_LENGTH];
bool motors_enabled = true;
long vccMilliVolts = 0;
void setup() {

  // Set up I2C communication
  Wire.begin(SLAVE_I2C_ADDRESS);
  Wire.onReceive(receiveData);
  Wire.onRequest(sendData);
  Wire.setClock(I2C_FREQ);

  myservo1.attach(SERVO1_PIN);
  myservo2.attach(SERVO2_PIN);
  myservo3.attach(SERVO3_PIN);
  myservo4.attach(SERVO4_PIN);

  myservo1.write(ZERO_POS1);//lift
  myservo2.write(ZERO_POS2);//rotate claw
  myservo3.write(ZERO_POS3);//claw
  myservo4.write(ZERO_POS4);//main lift

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

  Serial.begin(BAUD_RATE);
  //Serial.println("09/05/2020 Starting Robo-Mantis");
  vccMilliVolts = readVccMilliVolts();
}

long readVccMilliVolts()
{
  long result;
  // Read 1.1V reference against AVcc
  ADMUX = _BV(REFS0) | _BV(MUX3) | _BV(MUX2) | _BV(MUX1);
  delay(2);             // Wait for Vref to settle
  ADCSRA |= _BV(ADSC);  // Convert
  while (bit_is_set(ADCSRA, ADSC));
  result = ADCL;
  result |= ADCH << 8;
  result = 1126400L / result; // Back-calculate AVcc in mV
  if (result > 2000 || result < 200)
  {
    return readVccMilliVolts();
  }
  return result;
}

double checkInputVCC()
{
  //If the Vcc rail = 5v then this value should equal 5 volts
  return (vccMilliVolts * 0.001);
}

void updateCurrentSense()
{
  //the current will only be positive
  //IC it cant measure negative currents
  //pololu site https://www.pololu.com/product/708
  //For the VNH2SP30 version, the current sense (CS)
  //pins will output approximately 0.13 volts per
  //amp of output current.

  double val1 = analogRead(M1_CURR_SENSE) /  ( 1023.0 / (vccMilliVolts * 0.001));
  double val2 = analogRead(M2_CURR_SENSE) /  ( 1023.0 / (vccMilliVolts * 0.001));
  double val3 = analogRead(M3_CURR_SENSE) /  ( 1023.0 / (vccMilliVolts * 0.001));
  double val4 = analogRead(M4_CURR_SENSE) /  ( 1023.0 / (vccMilliVolts * 0.001));
  double val5 = analogRead(M5_CURR_SENSE) /  ( 1023.0 / (vccMilliVolts * 0.001));
  double val6 = analogRead(M6_CURR_SENSE) /  ( 1023.0 / (vccMilliVolts * 0.001));

  motor1milliAmps = val1 * 37.03;
  motor2milliAmps = val2 * 37.03;
  motor3milliAmps = val3 * 37.03;
  motor4milliAmps = val4 * 37.03;
  motor5milliAmps = val5 * 37.03;
  motor6milliAmps = val6 * 37.03;
  motorTotalmilliAmps = motor1milliAmps + motor2milliAmps +
                        motor3milliAmps + motor4milliAmps +
                        motor5milliAmps + motor6milliAmps ;

  //reduce resolution from 1024 to 256 by dividing by 4
  //in order to save to byte in output array to send over I2C
  //TODO: NEED TO FIX truncating to zero
  outputArray[0] = val1 / 4;
  outputArray[1] = val2 / 4;
  outputArray[2] = val3 / 4;
  outputArray[3] = val4 / 4;
  outputArray[4] = val5 / 4;
  outputArray[5] = val6 / 4;
}

void checkOverCurrent()
{
//check if the motors are drawing to much current
  if (motors_enabled = true && motorTotalmilliAmps > MAX_CURRENT_DRAW)
  {
    motors_enabled = false;
    motorStop();
  }
}

void printMotorMilliAmps()
{
  //total current fed to all the motors
  Serial.print(motor1milliAmps);
  Serial.print(", ");
  Serial.print(motor2milliAmps);
  Serial.print(", ");
  Serial.print(motor3milliAmps);
  Serial.print(", ");
  Serial.print(motor4milliAmps);
  Serial.print(", ");
  Serial.print(motor5milliAmps);
  Serial.print(", ");
  Serial.print(motor6milliAmps);
  Serial.println("");
}

void printOutputArray()
{
  for (int i = 0; i < OUT_ARR_LENGTH; i++) {
    Serial.print(outputArray[i]);
    Serial.print(" ,");
  }
  Serial.println("");
}

void motorStop()
{
  digitalWrite(M1_PWM, LOW);
  digitalWrite(M1_A, LOW);
  digitalWrite(M1_B, LOW);
  
  digitalWrite(M2_PWM, LOW);
  digitalWrite(M2_A, LOW);
  digitalWrite(M2_B, LOW);

  digitalWrite(M3_PWM, LOW);
  digitalWrite(M3_A, LOW);
  digitalWrite(M3_B, LOW);

  digitalWrite(M4_PWM, LOW);
  digitalWrite(M4_A, LOW);
  digitalWrite(M4_B, LOW);

  digitalWrite(M5_PWM, LOW);
  digitalWrite(M5_A, LOW);
  digitalWrite(M5_B, LOW);

  digitalWrite(M6_PWM, LOW);
  digitalWrite(M6_A, LOW);
  digitalWrite(M6_B, LOW);

}

void motorMove(byte motorNumber, int set_speed)
{
  //bulding block for all motor commands
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

  if (motors_enabled == true)
  {
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
  else{
    //if motors are disabled
   motorStop();
  }

}

void motorTest(byte motorNumber, unsigned int delay_time)
{
  //Performs a motor test
  //motor number lets you select one of 6 motors
  //input range 1,2,3,4,5,6
  //delay time lets you input the
  //delay time between each step

  for (int i = 0; i < 255; i++)
  {
    Serial.println(i);
    motorMove(motorNumber, i);
    delay(delay_time);
  }
  for (int i = 255; i > 0; i--)
  {
    Serial.println(i);
    motorMove(motorNumber, i);
    delay(delay_time);
  }
}

// the loop routine runs over and over again forever:
void loop() {
  receiveData();
  updateCurrentSense();
  
  Serial.println(checkInputVCC());
  //printMotorMilliAmps();
  //printOutputArray();
  //Serial.println(readVccMilliVolts());
  vccMilliVolts = readVccMilliVolts();
  //delay(100);
}


void receiveData() {
  //Commands are given as a single byte, where the bits are parsed as shown:
  // XXYYYZZZ 1 BYTE

  if (Wire.available())
  {
    byte receivedByte = Wire.read();
    //Serial.println(receivedByte);
    switch (receivedByte)
    {
      case 0:
        {
          motorStop();
          break;
        }
      case 1:
        {
          byte speedByteM1 = Wire.read();
          byte speedByteM2 = Wire.read();
          byte speedByteM3 = Wire.read();
          byte speedByteM4 = Wire.read();
          byte speedByteM5 = Wire.read();
          byte speedByteM6 = Wire.read();

          //        Serial.print(" ");
          //        Serial.print(speedByteM1);
          //        Serial.print(" ");
          //        Serial.print(speedByteM2);
          //        Serial.print(" ");
          //        Serial.print(speedByteM3);
          //        Serial.print(" ");
          //        Serial.print(speedByteM4);
          //        Serial.print(" ");
          //        Serial.print(speedByteM5);
          //        Serial.print(" ");
          //        Serial.print(speedByteM6);

          motorMove(1, 2 * (speedByteM1 - 127));
          motorMove(2, 2 * (speedByteM2 - 127));
          motorMove(3, 2 * (speedByteM3 - 127));
          motorMove(4, 2 * (speedByteM4 - 127));
          motorMove(5, 2 * (speedByteM5 - 127));
          motorMove(6, 2 * (speedByteM6 - 127));
          break;
        }
      default:
        {
          break;
        }
    }
    if (receivedByte == 2)
    {
      //set the servo position
      servoByte1 = Wire.read();
      servoByte2 = Wire.read();
      servoByte3 = Wire.read();
      servoByte4 = Wire.read();

      //        Serial.print(servoByte1);
      //        Serial.print(servoByte2);
      //        Serial.print(servoByte3);
      //        Serial.print(servoByte4);

      myservo1.write(servoByte1);
      myservo2.write(servoByte2);
      myservo3.write(servoByte3);
      myservo4.write(servoByte4);
    }
    //    Serial.print(" end\n");
  }
}

void sendData() {
  Wire.write(outputArray, OUT_ARR_LENGTH);  // Send current sense data
}
