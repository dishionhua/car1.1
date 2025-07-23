//
// Created by 15350 on 25-7-15.
//

#ifndef CAR_H
#define CAR_H
#include <stdint.h>

extern int Kp,Kd;
void car_straight();
void car_tracking(uint8_t* sensors);

#endif //CAR_H
