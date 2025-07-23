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

int Kp = 8, Kd = 0;//循迹系数

void car_straight() {
    angle_ring.actual = Yaw;
    //目标角度归一化
    if (angle_ring.target > 180) {
        angle_ring.target -= 360;
    } else if (angle_ring.target < -180) {
        angle_ring.target += 360;
    }
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

int final_error = 0;
int last_error = 0;
void car_tracking(uint8_t* sensors) {
    // base_speed = 50;
    last_error = final_error;
    // 计算偏差
    int weights[7] = {-3, -2, -1, 0, 1, 2, 3};
    int error = 0,p = 0;
    for (int i = 0; i < 7; i++) {
        error += sensors[i] * weights[i];
        p += sensors[i];
    }

    if (p != 0) {
        final_error = error*10 / p;
    } else {
        final_error = 0; // 防止除零错误
    }

    // PID控制
    speed_ring_l.target = base_speed + Kp*final_error/10;//+ Kd*(final_error - (float)last_error);
    speed_ring_r.target = base_speed - Kp*final_error/10;//- Kd*((float)final_error - (float)last_error);
    // //限幅
    // if (speed_ring_l.target < 0) speed_ring_l.target = 0;
    // if (speed_ring_r.target < 0) speed_ring_r.target = 0;
    speed_ring_l.actual = (float) get_speed(&htim4);
    speed_ring_r.actual = (float) get_speed(&htim5);


    PID_update(&speed_ring_l);
    PID_update(&speed_ring_r);
    Set_Pwml((int) speed_ring_l.out);
    Set_Pwmr((int) speed_ring_r.out);
}
