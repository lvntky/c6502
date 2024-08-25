#ifndef __C_ADDRESS_MODE_H__
#define __C_ADDRESS_MODE_H__

enum c_address_mode {
	ADDR_MODE_IMMEDIATE = 0, // Immediate mode (e.g., #$12)
	ADDR_MODE_ZERO_PAGE, // Zero Page mode (e.g., $00)
	ADDR_MODE_ZERO_PAGE_X, // Zero Page mode with X register (e.g., $00,X)
	ADDR_MODE_ZERO_PAGE_Y, // Zero Page mode with Y register (e.g., $00,Y)
	ADDR_MODE_ABSOLUTE, // Absolute mode (e.g., $1234)
	ADDR_MODE_ABSOLUTE_X, // Absolute mode with X register (e.g., $1234,X)
	ADDR_MODE_ABSOLUTE_Y, // Absolute mode with Y register (e.g., $1234,Y)
	ADDR_MODE_INDIRECT, // Indirect mode (e.g., ($00))
	ADDR_MODE_INDIRECT_X, // Indexed Indirect mode (e.g., ($00,X))
	ADDR_MODE_INDIRECT_Y, // Indirect Indexed mode (e.g., ($00),Y)
	ADDR_MODE_RELATIVE, // Relative mode (e.g., $12)
	ADDR_MODE_IMPLIED // Implied mode (e.g., no operand)
};

#endif //__C_ADDRESS_MODE_H__