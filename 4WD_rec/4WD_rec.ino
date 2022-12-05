/*
  Author: Advait Thale
  *******************************************************************
  *                     4WD Receiver(Bluetooth Car)                 *    
  *******************************************************************

  4WD with DC motors controlled by L293D Motor driver implemented on Arduino Nano 
  and consoled via HC-05 (Bluetooth 4.0). The 4WD is controlled by self made handheld
  direction control console via dual-axis potentiometer also having HC-05 and Arduino 
  Nano as MCU. As D-A pot is moved the 4WD moves. The implemented direction movements 
  on Dual-axis pot. are:
  -----------------------------------------------------------------------------------
                                     Forward 
                               
                           Turn Left         Turn Right     
                                            
                                     Backward                                               
  -----------------------------------------------------------------------------------
*/

#include <SoftwareSerial.h>

#define EN1 2 // PWM pin A 
#define EN2 7 // PWM pin B 
#define IN1 3 // Input pin 1 
#define IN2 4 // Input pin 2
#define IN3 5 // Input pin 3
#define IN4 6 // Input pin 4


int pwm = 255;
int x , y , x1, y1;
int motorSpeedA = 0;
int motorSpeedB = 0;
uint16_t valx, valy;
SoftwareSerial bt(10, 11); //TX, RX 

void setup()
 {
  bt.begin(38400);
  Serial.begin(38400);
  pinMode(EN1, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(EN2, OUTPUT);

}

void loop()
{
  if (bt.available() >= 0)
  {
    if (bt.available() >= 8)
    {
      byte val = bt.read();
      if (val == 0xE7) //x start
      {
        byte xh = bt.read();
        byte xl = bt.read();
        byte yh = bt.read();
        byte yl = bt.read();
        bt.flush();
        valx = ((xh << 3) | xl);
        valy = ((yh << 3) | yl);
      }
    }
    x = valx - x1;
    y = valy - y1;
  
    Serial.print("\tvalx: ");
    Serial.print(x);
    Serial.print("\tvaly: ");
    Serial.println(y);

    if (y < 512)
   {
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);
    digitalWrite(IN3, HIGH);
    digitalWrite(IN4, LOW);
    motorSpeedA = map(y, 512, 0, 0, 255);
    motorSpeedB = map(y, 512, 0, 0, 255);
   }
   else if (y > 512)
   {
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, HIGH);
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, HIGH);
    motorSpeedA = map(y, 512, 1023, 0, 255);
    motorSpeedB = map(y, 512, 1023, 0, 255);
   }
    else
    {
      motorSpeedA = 0;
      motorSpeedB = 0;
    }
    if (x < 512)
    {
      // Convert the declining X-axis readings from 470 to 0 into increasing 0 to 255 value
      int xMapped = map(x, 512, 0, 0, 255);
      motorSpeedA = motorSpeedA - xMapped;
      motorSpeedB = motorSpeedB + xMapped;
      if (motorSpeedA < 0) 
      {
        motorSpeedA = 0;
      }
      if (motorSpeedB > 255) 
      {
        motorSpeedB = 255;
      }
    }
    if (x > 512) 
    {
      int xMapped = map(x, 512, 1023, 0, 255);
      motorSpeedA = motorSpeedA + xMapped;
      motorSpeedB = motorSpeedB - xMapped;
      if (motorSpeedA > 255) 
      {
        motorSpeedA = 255;
      }
      if (motorSpeedB < 0) 
      {
        motorSpeedB = 0;
      }
    }
    if (motorSpeedA < 70)
    {
      motorSpeedA = 0;
    }
    if (motorSpeedB < 70) 
    {
      motorSpeedB = 0;
    }
    analogWrite(EN1, motorSpeedA);
    analogWrite(EN2, motorSpeedB);
  }
  bt.flush();
}
