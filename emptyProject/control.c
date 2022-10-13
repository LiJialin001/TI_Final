/*
 * control.c
 *
 *  Created on: 2022年10月13日
 *      Author: Administrator
 */

#include "include.h"
#include "control.h"
#include "motor.h"
#include "math.h"

#define MOTOR1      motor[0]
#define MOTOR2      motor[1]
#define MOTOR3      motor[2]
#define MOTOR4      motor[3]
#define ClearMotor  memset(motor, 0, sizeof(int16_t) * 4)

//Extern引用
extern int16_t motor[4];



void MotorControl(void)
{


    UpdateMotor(MOTOR4, MOTOR2, MOTOR1, MOTOR3);
}
