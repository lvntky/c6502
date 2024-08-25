#ifndef __C_REGISTER_H__
#define __C_REGISTER_H__

#include <stdint.h>

#define FLAG_CARRY 0x01 // Carry Flag
#define FLAG_ZERO 0x02 // Zero Flag
#define FLAG_INTERRUPT 0x04 // Interrupt Disable
#define FLAG_DECIMAL 0x08 // Decimal Mode
#define FLAG_BREAK 0x10 // Break Command
#define FLAG_UNUSED 0x20 // Unused (always set to 1 on the real 6502)
#define FLAG_OVERFLOW 0x40 // Overflow Flag
#define FLAG_NEGATIVE 0x80 // Negative Flag

/**
 * Bitwise macros for processor status flags
 */
// Set a flag in the status register
#define SET_FLAG(reg, flag) ((reg).status |= (flag))

// Clear a flag in the status register
#define CLEAR_FLAG(reg, flag) ((reg).status &= ~(flag))

// Check if a flag is set in the status register
#define IS_FLAG_SET(reg, flag) (((reg).status & (flag)) != 0)

// Check if a flag is clear in the status register
#define IS_FLAG_CLEAR(reg, flag) (((reg).status & (flag)) == 0)

typedef struct c_register {
	uint16_t pc;
	uint8_t sp;
	uint8_t acc;
	uint8_t x;
	uint8_t y;
	uint8_t status;
} c_register_t;

#endif //__C_REGISTER_H__
