
#ifndef INC_MOTOR_H_
#define INC_MOTOR_H_
#include "main.h"

// 设置右侧电机的PWM值
void Set_Pwmr(int motor_right);

// 设置左侧电机的PWM值
void Set_Pwml(int motor_left);

// 控制右侧电机
void Motor_Right(int a);

// 控制左侧电机
void Motor_Left(int a);

//刹车
void Motor_stop(void);


#endif //MOTOR_H
