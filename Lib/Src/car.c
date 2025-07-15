//
// Created by 15350 on 25-7-15.
//

#include "../Inc/car.h"

#include <math.h>
#include <string.h>

#include "Debug.h"
#include "get_speed.h"
#include "jy61p.h"
#include "main.h"
#include "motor.h"
#include "pid.h"
#include "tim.h"

int k = 40;//循迹系数

void car_straight() {
    angle_ring.actual = Yaw;
    PID_update(&angle_ring);
    if (fabsf(angle_ring.target - angle_ring.actual) < 3) angle_ring.out = 0;
    speed_ring_l.target =base_speed - angle_ring.out;
    speed_ring_r.target =base_speed + angle_ring.out;
    speed_ring_l.actual = (float) get_speed(&htim4);;
    speed_ring_r.actual = (float) get_speed(&htim5);
    PID_update(&speed_ring_l);
    PID_update(&speed_ring_r);
    Set_Pwml((int) speed_ring_l.out);
    Set_Pwmr((int) speed_ring_r.out);
}

void car_tracking(uint8_t* sensors) {
    // 计算偏差
    int weights[7] = {-3, -2, -1, 0, 1, 2, 3};
    int error = 0,p = 0;
    for (int i = 0; i < 7; i++) {
        error += sensors[i] * weights[i];
        p += sensors[i];
    }
    int last_error = error / p;
    // PID控制
    speed_ring_l.target = base_speed + k*last_error;
    speed_ring_r.target = base_speed - k*last_error;
    //限幅
    if (speed_ring_l.target < 0) speed_ring_l.target = 0;
    if (speed_ring_r.target < 0) speed_ring_r.target = 0;
    speed_ring_l.actual = (float) get_speed(&htim4);;
    speed_ring_r.actual = (float) get_speed(&htim5);
    PID_update(&speed_ring_l);
    PID_update(&speed_ring_r);
    Set_Pwml((int) speed_ring_l.out);
    Set_Pwmr((int) speed_ring_r.out);
}
