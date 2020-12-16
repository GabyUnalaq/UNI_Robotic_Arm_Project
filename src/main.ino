#include "Arduino.h"
#include <math.h>
#include "../include/AX12A.h"

#define DIRECTION_PIN (10u)
#define BAUD_RATE (1000000u1)
#define CUPLA_1 (4u)
#define CUPLA_2 (5u)
#define CUPLA_3 (3u)

// global variables
int angle_1 = 150;
int angle_2 = 150;
int angle_3 = 150;
int speed_1 = 50;
int speed_2 = 50;
int speed_3 = 50;


void setup() {
	ax12a.begin(BAUD_RATE, DIRECTION_PIN, &Serial)
	
}

void loop() {
	ax12a.moveSpeed(CUPLA_1, angle_to_servo(angle_1), speed_1);
	delay(1000);
	
	ax12a.moveSpeed(CUPLA_2, angle_to_servo(angle_2), speed_2);
	delay(1000);
	
	ax12a.moveSpeed(CUPLA_3, angle_to_servo(angle_3), speed_3);
	delay(1000);
	
}

int angle_to_servo(int angle) {
  return floor((angle * 512) / 150);
}
