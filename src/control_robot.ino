#include "Arduino.h"
#include "../include/AX12A.h"

#define DIRECTION_PIN (10u)
#define BAUD_RATE (1000000u1)
#define CUPLA_1 (4u)
#define CUPLA_2 (5u)
#define CUPLA_3 (3u)


void setup() {
	ax12a.begin(BAUD_RATE, DIRECTION_PIN, &Serial)
	
}

void loop() {
	ax12a.moveSpeed(CUPLA_1, 512, 50);
	delay(1000);
	
	ax12a.moveSpeed(CUPLA_2, 512, 50);
	delay(1000);
	
	ax12a.moveSpeed(CUPLA_3, 512, 50);
	delay(1000);
	
}