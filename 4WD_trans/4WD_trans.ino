/*
  Author: Advait Thale
  *******************************************************************
                        4WD Receiver(Bluetooth Car)
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

#include<SoftwareSerial.h>

SoftwareSerial bt(2, 3);// (Tx, Rx)

byte start = 0xE7;    //start byte for x axis
byte starty = 0xE5;   //start byte for y axis
byte dip = 0xE6;
int SW = 5;
int NRx = A0;
int NRy = A1;
int x, y, state;
bool flag;

void setup()
{
  pinMode(SW, INPUT_PULLUP);
  Serial.begin(38400);
  bt.begin(38400);
  Serial.println("Init");
}
void loop()
{
  NRx = analogRead(A0);
  NRy = analogRead(A1);

  byte xh = ((NRx & 0b1110000000) >> 3);
  delay(70);
  byte xl = NRx & 0x7F;
  byte yh = ((NRy & 0b1110000000) >> 3);
  delay(70);
  byte yl = NRy & 0x7F;

  if (digitalRead(SW == LOW))
  {
    byte SW = 0b10000000;
    byte switchState = switchState | SW;
    bt.write(switchState);
  }
  else
  {
    byte switchState = switchState & 0x00;
    bt.write(switchState);
  }

  bt.write(start);
  delay(100);
  bt.write(xh);
  delay(100);
  bt.write(xl);
  delay(100);
  bt.write(state);
  delay(100);
  bt.write(yh);
  delay(100);
  bt.write(yl);
  delay(100);
  bt.flush();
  delay(10);
  Serial.print("\nrx: ");
  Serial.print(NRx);
  Serial.print("\tstart: ");
  Serial.print(start);
  Serial.print("\txh: ");
  Serial.print(xh);
  Serial.print("\txl: ");
  Serial.println(xl);

}
