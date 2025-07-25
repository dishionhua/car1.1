#include "Debug.h"
#include "string.h"
#include "stdio.h"
#include <stdarg.h>
#include "usart.h"
#include <stdlib.h>
#include <stdbool.h>

#include "car.h"
#include "pid.h"
#include "oled.h"
#include "font.h"
#include "jy61p.h"
#include "motor.h"

uint8_t sofa_data[50];
uint8_t camera_data[50];


void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size){
        if (huart==&huart1){
            if (Size==10){
                if (camera_data[0]==0xA5 && camera_data[1]==0xA6 &&camera_data[9]==0x5B){
                    //取出camera_data中的7个信号位数据
                    uint8_t sensors[7];
                    memcpy(sensors, &camera_data[2], 7);
                    int data1=camera_data[2]+camera_data[3]+camera_data[4]+camera_data[5];
                    int data2=camera_data[5]+camera_data[6]+camera_data[7]+camera_data[8];
                    //第一题是否到达黑线
                    if (state == 0) {
                        if (data1 + data2 != 0) {
                            Motor_stop();
                            stop_flag = 1;
                        }
                    }
                    //第二题是否到达黑线
                    if (state == 1) {
                        if (data1 + data2 != 0) state = 2;
                    }
                    //判断循迹是否走完
                    if (state == 2) {
                        //执行循迹
                        car_tracking(sensors);
                        if (data1 + data2 == 0 && (Yaw < -170 || Yaw > 170)) {
                            // state = 1;
                            // angle_ring.target = 180;
                            Motor_stop();
                        }
                        // if (data1 + data2 == 0 && (10 > Yaw > -10)) {
                        //     Motor_stop();
                        // }
                    }
                }
            }
            HAL_UARTEx_ReceiveToIdle_DMA(&huart1, camera_data, sizeof(camera_data));
        }
        if (huart == &huart4) {
            uint8_t first = sofa_data[0];
            uint8_t second = sofa_data[1];
            uint8_t third = sofa_data[2];
            float value;
            if (first == 'p') {
                Kp = (second - '0') * 10 + (third - '0');
            }
            else if (first == 'd') {
                Kd = (second - '0') * 10 + (third - '0');
            }
            else if (first == 's') {
                base_speed = (second - '0') * 10 + (third - '0');
            }
            else if (first == '4') {
                state = 4;
            }
            else if (first == '3') {
                state = 3;
            }
            else if (first == '2') {
                state = 2;
            }
            else if (first == '1') {
                state = 1;
            }
            else if (first == '0') {
                state = 0;
            }

            if(second == 'P' && third == '1')
            {
                value = parse_float_from_uart(sofa_data,(uint16_t)strlen((char*)sofa_data));
                angle_ring.kp = value;
                // speed_ring_r.kp = value;
            }
            else if(second == 'I' && third == '1')
            {
                value = parse_float_from_uart(sofa_data,(uint16_t)strlen((char*)sofa_data));
                angle_ring.ki = value;
                // speed_ring_r.ki = value;
            }
            else if(second == 'D' && third == '1')
            {
                value = parse_float_from_uart(sofa_data,(uint16_t)strlen((char*)sofa_data));
                angle_ring.kd = value;
                // speed_ring_r.kd = value;
            }
            HAL_UARTEx_ReceiveToIdle_DMA(&huart4, sofa_data, sizeof(sofa_data));
        }
}


float parse_float_from_uart(const uint8_t *data, uint16_t length) {
    // 最小有效数据长度检查 (P1= + 1个数字字符 + !#+ = 8字节)
    if (length < 8) return 0.0f / 0.0f; // 返回NaN表示错误

    // 验证起始标记
    if (strncmp((char *) data, "@P1=", 4) != 0 && strncmp((char *) data, "@I1=", 4) != 0 && strncmp(
            (char *) data, "@D1=", 4) != 0) {
        return 0.0f / 0.0f; // 起始标记错误，返回NaN
    }

    // 查找结束标记
    const uint8_t *end_ptr = data + 4; // 从数字部分开始搜索
    bool end_marker_found = false;

    // 在剩余数据中搜索结束标记
    for (uint16_t i = 4; i < length - 2; i++) {
        if (data[i] == '!' && data[i + 1] == '#' && data[i + 2] == '+') {
            end_ptr = &data[i];
            end_marker_found = true;
            break;
        }
    }

    if (!end_marker_found) return 0.0f / 0.0f; // 结束标记未找到

    // 计算数字部分长度
    uint16_t num_len = end_ptr - (data + 4);
    if (num_len == 0) return 0.0f / 0.0f; // 无数字内容

    // 创建临时缓冲区存放数字字符串
    char num_str[32]; // 足够存放长浮点数
    if (num_len >= sizeof(num_str)) {
        return 0.0f / 0.0f; // 数字部分过长
    }

    memcpy(num_str, data + 4, num_len);
    num_str[num_len] = '\0'; // 添加终止符

    // 验证数字格式 (可选但推荐)
    for (uint16_t i = 0; i < num_len; i++) {
        char c = num_str[i];
        if (!((c >= '0' && c <= '9') || c == '.' || c == '-' || c == '+')) {
            return 0.0f / 0.0f; // 非法字符
        }
    }

    // 转换为浮点数
    char *end_parse;
    float value = strtof(num_str, &end_parse);

    // 检查是否整个字符串都被转换
    if (end_parse != num_str + num_len) {
        return 0.0f / 0.0f; // 转换不完全
    }

    return value;
}


void UART_Printf(UART_HandleTypeDef *huart, const char *format, ...) {
    va_list args;
    va_start(args, format);
    char buffer[64]; // 减小缓冲区
    int len = vsnprintf(buffer, sizeof(buffer), format, args);
    va_end(args);

    if (len > 0 && len < sizeof(buffer)) {
        HAL_UART_Transmit(huart, (uint8_t *) buffer, len, 100);
    }
}
