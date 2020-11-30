#include <Servo.h>

#define pinServo 3
#define left_butt 5
#define right_butt 4
  
Servo myservo;
int angle = 90;


void setup()
{
  myservo.attach(pinServo);
  
  pinMode(left_butt, INPUT);
  pinMode(right_butt, INPUT);
}

void loop()
{
  if(digitalRead(left_butt) == 1)
    if (angle != 0)
    	angle += 1;
  if(digitalRead(right_butt) == 1)
    if (angle != 180)
    	angle -= 1;
  
  myservo.write(angle);
  delay(100);
}
