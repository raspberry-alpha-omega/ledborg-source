/*
 * File:   bcm2835_regmap.h
 * Author: Arron Churchill
 * Freeburn Robotics Limited
 */
#ifndef _BCM2835_REGMAP_H_
#define _BCM2835_REGMAP_H_

/***** GENERAL DEFINITIONS *****/
#define NO_MASK													(UINT_MAX)
#define NULL_FD													((int)0)
#define MAP_NAME_LEN											(20)

/***** MEMORY LOCATIONS *****/
typedef struct raw_memory {
	void *accessor;			// Base location for memory access
	unsigned int offset;	// Offset into base for the starting point
	unsigned int length;
	char name[MAP_NAME_LEN + 1];
} raw_memory;

extern raw_memory rawMemI2C;
extern raw_memory rawMemGPIO;
extern raw_memory rawMemPWM;
extern raw_memory rawMemCLOCK;

/***** MEMORY MACROS *****/
// General purpose
#define SET_REGISTER_FIELD(base, name, field, value)			RegWrite(base, name, (name##_##field##_MASK) << (name##_##field##_SHIFT), (value) << (name##_##field##_SHIFT))
#define SET_REGISTER_FIELD_WO(base, name, field, value)			RegWrite(base, name, NO_MASK, (value) << (name##_##field##_SHIFT))
#define GET_REGISTER_FIELD(base, name, field)					((RegRead(base, name >> (name##_##field##_SHIFT)) & (name##_##field##_MASK))
#define CHECK_REGISTER_FIELD(base, name, field)					(((RegRead(base, name >> (name##_##field##_SHIFT)) & (name##_##field##_MASK)) == (value))
#define SET_REGISTER_FIELD_NAME(base, name, field, value)		RegWrite(base, name, (name##_##field##_MASK) << (name##_##field##_SHIFT), (name##_##field##_##value) << (name##_##field##_SHIFT))
#define SET_REGISTER_FIELD_NAME_WO(base, name, field, value)	RegWrite(base, name, NO_MASK, (name##_##field##_##value) << (name##_##field##_SHIFT))
#define CHECK_REGISTER_FIELD_NAME(base, name, field, value)		(((RegRead(base, name) >> (name##_##field##_SHIFT)) & (name##_##field##_MASK)) == (name##_##field##_##value))

// I2C
#define SET_I2C_REG_FIELD(name, field, value)					RegWrite(&rawMemI2C, I2C_##name, (I2C_##name##_##field##_MASK) << (I2C_##name##_##field##_SHIFT), (value) << (I2C_##name##_##field##_SHIFT))
#define SET_I2C_REG_FIELD_WO(name, field, value)				RegWrite(&rawMemI2C, I2C_##name, NO_MASK, (value) << (I2C_##name##_##field##_SHIFT))
#define GET_I2C_REG_FIELD(name, field)							((RegRead(&rawMemI2C, I2C_##name) >> (I2C_##name##_##field##_SHIFT)) & (I2C_##name##_##field##_MASK))
#define CHECK_I2C_REG_FIELD(name, field, value)					(((RegRead(&rawMemI2C, I2C_##name) >> (I2C_##name##_##field##_SHIFT)) & (I2C_##name##_##field##_MASK)) == (value))
#define SET_I2C_REG_FIELD_NAME(name, field, value)				RegWrite(&rawMemI2C, I2C_##name, (I2C_##name##_##field##_MASK) << (I2C_##name##_##field##_SHIFT), (I2C_##name##_##field##_##value) << (I2C_##name##_##field##_SHIFT))
#define SET_I2C_REG_FIELD_NAME_WO(name, field, value)			RegWrite(&rawMemI2C, I2C_##name, NO_MASK, (I2C_##name##_##field##_##value) << (I2C_##name##_##field##_SHIFT))
#define CHECK_I2C_REG_FIELD_NAME(name, field, value)			(((RegRead(&rawMemI2C, I2C_##name) >> (I2C_##name##_##field##_SHIFT)) & (I2C_##name##_##field##_MASK)) == (I2C_##name##_##field##_##value))

// GPIO
#define SET_GPIO_REG_FIELD(name, field, value)					RegWrite(&rawMemGPIO, GPIO_##name, (GPIO_##name##_##field##_MASK) << (GPIO_##name##_##field##_SHIFT), (value) << (GPIO_##name##_##field##_SHIFT))
#define SET_GPIO_REG_FIELD_WO(name, field, value)				RegWrite(&rawMemGPIO, GPIO_##name, NO_MASK, (value) << (GPIO_##name##_##field##_SHIFT))
#define GET_GPIO_REG_FIELD(name, field)							((RegRead(&rawMemGPIO, GPIO_##name) >> (GPIO_##name##_##field##_SHIFT)) & (GPIO_##name##_##field##_MASK))
#define CHECK_GPIO_REG_FIELD(name, field, value)				(((RegRead(&rawMemGPIO, GPIO_##name) >> (GPIO_##name##_##field##_SHIFT)) & (GPIO_##name##_##field##_MASK)) == (value))
#define SET_GPIO_REG_FIELD_NAME(name, field, value)				RegWrite(&rawMemGPIO, GPIO_##name, (GPIO_##name##_##field##_MASK) << (GPIO_##name##_##field##_SHIFT), (GPIO_##name##_##field##_##value) << (GPIO_##name##_##field##_SHIFT))
#define SET_GPIO_REG_FIELD_NAME_WO(name, field, value)			RegWrite(&rawMemGPIO, GPIO_##name, NO_MASK, (GPIO_##name##_##field##_##value) << (GPIO_##name##_##field##_SHIFT))
#define CHECK_GPIO_REG_FIELD_NAME(name, field, value)			(((RegRead(&rawMemGPIO, GPIO_##name) >> (GPIO_##name##_##field##_SHIFT)) & (GPIO_##name##_##field##_MASK)) == (GPIO_##name##_##field##_##value))

// PWM
#define SET_PWM_REG_FIELD(name, field, value)					RegWrite(&rawMemPWM, PWM_##name, (PWM_##name##_##field##_MASK) << (PWM_##name##_##field##_SHIFT), (value) << (PWM_##name##_##field##_SHIFT))
#define SET_PWM_REG_FIELD_WO(name, field, value)				RegWrite(&rawMemPWM, PWM_##name, NO_MASK, (value) << (PWM_##name##_##field##_SHIFT))
#define GET_PWM_REG_FIELD(name, field)							((RegRead(&rawMemPWM, PWM_##name) >> (PWM_##name##_##field##_SHIFT)) & (PWM_##name##_##field##_MASK))
#define CHECK_PWM_REG_FIELD(name, field, value)					(((RegRead(&rawMemPWM, PWM_##name) >> (PWM_##name##_##field##_SHIFT)) & (PWM_##name##_##field##_MASK)) == (value))
#define SET_PWM_REG_FIELD_NAME(name, field, value)				RegWrite(&rawMemPWM, PWM_##name, (PWM_##name##_##field##_MASK) << (PWM_##name##_##field##_SHIFT), (PWM_##name##_##field##_##value) << (PWM_##name##_##field##_SHIFT))
#define SET_PWM_REG_FIELD_NAME_WO(name, field, value)			RegWrite(&rawMemPWM, PWM_##name, NO_MASK, (PWM_##name##_##field##_##value) << (PWM_##name##_##field##_SHIFT))
#define CHECK_PWM_REG_FIELD_NAME(name, field, value)			(((RegRead(&rawMemPWM, PWM_##name) >> (PWM_##name##_##field##_SHIFT)) & (PWM_##name##_##field##_MASK)) == (PWM_##name##_##field##_##value))

// CLOCK
#define SET_CLOCK_REG_FIELD(name, field, value)					ClockRegWrite(&rawMemCLOCK, CLOCK_##name, (CLOCK_X_##field##_MASK) << (CLOCK_X_##field##_SHIFT), (value) << (CLOCK_X_##field##_SHIFT))
#define GET_CLOCK_REG_FIELD(name, field)						((RegRead(&rawMemCLOCK, CLOCK_##name) >> (CLOCK_X_##field##_SHIFT)) & (CLOCK_X_##field##_MASK))
#define CHECK_CLOCK_REG_FIELD(name, field, value)				(((RegRead(&rawMemCLOCK, CLOCK_##name) >> (CLOCK_X_##field##_SHIFT)) & (CLOCK_X_##field##_MASK)) == (value))
#define SET_CLOCK_REG_FIELD_NAME(name, field, value)			ClockRegWrite(&rawMemCLOCK, CLOCK_##name, (CLOCK_X_##field##_MASK) << (CLOCK_X_##field##_SHIFT), (CLOCK_X_##field##_##value) << (CLOCK_X_##field##_SHIFT))
#define CHECK_CLOCK_REG_FIELD_NAME(name, field, value)			(((RegRead(&rawMemCLOCK, CLOCK_##name) >> (CLOCK_X_##field##_SHIFT)) & (CLOCK_X_##field##_MASK)) == (CLOCK_X_##field##_##value))

/***** MEMORY MAP *****/
// GPIO control (see 6.1-2 p90)
#define ADDR_GPIO												(0x20200000) //(0x7E200000)
#define GPIO_SIZE_BYTES											(180)

#include "bcm2835_gpioreg.h"

// BSC / I2C Serial controllers (see 3.1 p28 and 1.2.2-4 p6, 0x20nnnnnn for physical addresses)
#define ADDR_BSC0												(0x20205000) //(0x7E205000)
#define ADDR_BSC1												(0x20804000) //(0x7E804000)
#define ADDR_BSC2												(0x20805000) //(0x7E805000)	// Used by HDMI

#define I2C_SIZE_BYTES											(32)

//#include "bcm2835_i2creg.h"

// PWM control (see 9 p138)
#define ADDR_PWM												(0x2020C000) //(0x7E20C000)
#define PWM_SIZE_BYTES											(40)

//#include "bcm2835_pwmreg.h"

// Clock control
#define ADDR_CLOCK												(0x20101000) //(0x7E101000)
#define CLOCK_SIZE_BYTES										(1024)

//#include "bcm2835_clockreg.h"

#endif
