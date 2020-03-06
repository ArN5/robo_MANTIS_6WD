
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

  Serial.begin(115200);
   

  for(int i=0;i<255;i++)
  {
 motorMove(6,i);
 motorMove(5,i);
 motorMove(4,i);
 motorMove(3,i);
 motorMove(2,i);
 motorMove(1,i); 
 delay(10);   
  }

delay(1000); 
  
    for(int i=0;i<510;i++)
  {
 motorMove(6,255-i);
 motorMove(5,255-i);
 motorMove(4,255-i);
 motorMove(3,255-i);
 motorMove(2,255-i);
 motorMove(1,255-i); 
 delay(10);   
  }

    for(int i=0;i<255;i++)
  {
 motorMove(6,-255+i);
 motorMove(5,-255+i);
 motorMove(4,-255+i);
 motorMove(3,-255+i);
 motorMove(2,-255+i);
 motorMove(1,-255+i); 
 delay(10);   
  }

 motorMove(6,100);
 motorMove(5,100);
 motorMove(4,100);
 motorMove(3,100);
 motorMove(2,100);
 motorMove(1,100); 

}

void motorMove(byte motorNumber,int set_speed)
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

  if(set_speed >= 0)
  {
  digitalWrite(mbPin, LOW);
  digitalWrite(maPin, HIGH);
  
  }
  else if(set_speed < 0)
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

  //motorTest(5,1);

  
//  for (int i = 0; i < 6; i++)
//  {
//    motorTest(i+1,20);
//  }
}
