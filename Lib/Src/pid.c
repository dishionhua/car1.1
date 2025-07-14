#include "pid.h"

#include "get_speed.h"
#include "tim.h"
#include "Debug.h"
#include "usart.h"


Pid_Data angle_ring = {
    .kp = 4.0f,
    .ki = 0.0f,
    .kd = 8.0f,
    .out_max = 100,
    .out_min = -100
  };

Pid_Data speed_ring_l = {
    .kp = 0.3f,
    .ki = 0.04f,
    .kd = 0.0f,
    .out_max = 100,
    .out_min = -100
  };

Pid_Data speed_ring_r = {
    .kp = 0.3f,
    .ki = 0.04f,
    .kd = 0.0f,
    .out_max = 100,
    .out_min = -100
  };

void PID_update(Pid_Data *p) {
    // 计算当前误差
	 p->error1 = p->error0;
	 p->error0 = p->target - p->actual;
	if (angle_ring.error0 > 180) {
		angle_ring.error0 -= 360;
	} else if (angle_ring.error0 < -180) {
		angle_ring.error0 += 360;
	}

    // 累积误差（积分项）
	 if (p->ki != 0) {  // 安全的浮点数比较
	         // 积分累加
	         p->errorint += p->error0;

	         // 积分限幅（使用指针访问成员）
	         if (p->errorint > (p->out_max / p->ki)) {
	             p->errorint = p->out_max / p->ki;
	         }
	         if (p->errorint < (p->out_min / p->ki)) {
	             p->errorint = p->out_min / p->ki;
	         }
	     }
    // PID 输出计算
	p->out = p->kp * p->error0 + p->ki * p->errorint + p->kd * (p->error0 - p->error1);
    // 输出限幅
    if (p->out < p->out_min) {
            p->out = p->out_min;
        }
        if (p->out > p->out_max) {
            p->out = p->out_max;
        }
//        UART_Printf(&huart1, "%.2f,%.2f\r\n", speed_ring_l.error0,speed_ring_l.out);
}





