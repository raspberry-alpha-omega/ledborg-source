#include <stdio.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <limits.h>
#include <unistd.h>
#include <sys/time.h>
#include <string.h>
#include "bcm2835_regmap.h"

// Change the following #defines for Rev1 or Rev2 boards
#define PIN_LED_RED								PIN_17
//#define PIN_LED_GREEN							PIN_21	// Rev 1
#define PIN_LED_GREEN							PIN_27	// Rev 2
#define PIN_LED_BLUE							PIN_22


// Constants
#define FALSE									(0)
#define TRUE									(!FALSE)

// Cleanup functions
#define SET_GPIO_REG_FIELD_NAME_PP(a,b,c)		SET_GPIO_REG_FIELD_NAME(a,b,c)
#define SET_GPIO_REG_FIELD_NAME_WO_PP(a,b,c)	SET_GPIO_REG_FIELD_NAME_WO(a,b,c)

typedef unsigned char BOOL;

BOOL initialiseGPIO(void);
void shutdownGPIO(void);

int memFile = NULL_FD;
raw_memory rawMemGPIO = {MAP_FAILED, 0, 0, "GPIO"};

void RegWrite(raw_memory *base, unsigned int address, unsigned int mask, unsigned int value);
unsigned int RegRead(raw_memory *base, unsigned int address);
BOOL MapMemory(raw_memory *base, int device, unsigned int startAddress, size_t size);
void UnmapMemory(raw_memory *base);

BOOL initialiseGPIO(void)
{
	// Enable raw memory access
	if (memFile == NULL_FD) memFile = open("/dev/mem", O_RDWR | O_SYNC); 
	if (memFile == NULL_FD) {
		printf("Failed to open /dev/mem (are we root?) !\n");
		return FALSE;
	}

	// Map GPIO memory
	if (!MapMemory(&rawMemGPIO, memFile, ADDR_GPIO, GPIO_SIZE_BYTES)) {
		printf("Failed to map GPIO device memory (are we root?) !\n");
		return FALSE;
	}

	return TRUE;
}

void shutdownGPIO(void) {
	// Unmap the GPIO memory
	if (rawMemGPIO.accessor != MAP_FAILED) {
		UnmapMemory(&rawMemGPIO);
	}
	
	// Disable raw memory access
	if (memFile != NULL_FD) {
		close(memFile);
		memFile = NULL_FD;
	}
}

int main(int argc, char *argv[]) {
	BOOL printUse = FALSE;

	if (!initialiseGPIO()) {
		shutdownGPIO();
		return 1;
	}

	// Setup pins
	SET_GPIO_REG_FIELD_NAME_PP(FUNCTION_SELECT_1, PIN_LED_RED, OUTPUT);
	SET_GPIO_REG_FIELD_NAME_WO_PP(PIN_CLEAR_0, PIN_LED_RED, CLEAR);
	SET_GPIO_REG_FIELD_NAME_PP(FUNCTION_SELECT_2, PIN_LED_GREEN, OUTPUT);
	SET_GPIO_REG_FIELD_NAME_WO_PP(PIN_CLEAR_0, PIN_LED_GREEN, CLEAR);
	SET_GPIO_REG_FIELD_NAME_PP(FUNCTION_SELECT_2, PIN_LED_BLUE, OUTPUT);
	SET_GPIO_REG_FIELD_NAME_WO_PP(PIN_CLEAR_0, PIN_LED_BLUE, CLEAR);

	// Set values
	if (argc < 2) {
		printf("No values provided!\n");
		printUse = TRUE;
	} else if (strlen(argv[1]) < 3) {
		printf("Not enough values provided!\n");
		printUse = TRUE;
	} else {
		if ((argv[1][0] == '1') || (argv[1][0] == '2')) {
			SET_GPIO_REG_FIELD_NAME_WO_PP(PIN_SET_0, PIN_LED_RED, SET);
		} else if (argv[1][0] == '0') {
			SET_GPIO_REG_FIELD_NAME_WO_PP(PIN_CLEAR_0, PIN_LED_RED, CLEAR);
		} else {
			printf("Unusable red value '%c'\n", argv[1][0]);
			printUse = TRUE;
		}
		if ((argv[1][1] == '1') || (argv[1][1] == '2')) {
			SET_GPIO_REG_FIELD_NAME_WO_PP(PIN_SET_0, PIN_LED_GREEN, SET);
		} else if (argv[1][1] == '0') {
			SET_GPIO_REG_FIELD_NAME_WO_PP(PIN_CLEAR_0, PIN_LED_GREEN, CLEAR);
		} else {
			printf("Unusable blue value '%c'\n", argv[1][1]);
			printUse = TRUE;
		}
		if ((argv[1][2] == '1') || (argv[1][2] == '2')) {
			SET_GPIO_REG_FIELD_NAME_WO_PP(PIN_SET_0, PIN_LED_BLUE, SET);
		} else if (argv[1][2] == '0') {
			SET_GPIO_REG_FIELD_NAME_WO_PP(PIN_CLEAR_0, PIN_LED_BLUE, CLEAR);
		} else {
			printf("Unusable green value '%c'\n", argv[1][2]);
			printUse = TRUE;
		}
	}

	if (printUse) printf("Usage: %s RGB\n Where R is red (0-2), G is green (0-2) and B is blue (0-2).\n", argv[0]);

	shutdownGPIO();

	return 0;
}

///// Raw memory access (dangerous code) /////
void RegWrite(raw_memory *base, unsigned int address, unsigned int mask, unsigned int value) {
	if (base->accessor == MAP_FAILED) {
		printf("RegWrite failed due to an unmapped base (%s)!\n", base->name);
	} else if (address >= base->length) {
		printf("RegWrite failed due to address out of bounds (%s, 0x%08X)!\n", base->name, address);
	} else {
		if (mask != NO_MASK) {
			unsigned int read = RegRead(base, address);
			value &= mask;
			value |= (~mask) & read;
		}
		*((unsigned int *)(base->accessor + base->offset + address)) = value;
	}
}

unsigned int RegRead(raw_memory *base, unsigned int address) {
	if (base->accessor == MAP_FAILED) {
		printf("RegRead failed due to an unmapped base (%s)!\n", base->name);
		return 0;
	} else if (address >= base->length) {
		printf("RegRead failed due to address out of bounds (%s, 0x%08X)!\n", base->name, address);
		return 0;
	} else {
		unsigned int value = *((unsigned int *)(base->accessor  + address));
		return value;
	}
}

BOOL MapMemory(raw_memory *base, int device, unsigned int startAddress, size_t size) {
	// Unmap first if already allocated
	if (base->accessor != MAP_FAILED) {
		UnmapMemory(base);
	}
	// Align start address to the page size (we can only map page aligned blocks)
	base->offset = (unsigned int)(startAddress / getpagesize()) * getpagesize();
	// Compute the necessary size to cover the alignment
	base->length = (unsigned int)size;
	size += (size_t)(startAddress - base->offset);
	// Map the memory from the device to a local address
	base->accessor = mmap(NULL, size, PROT_READ|PROT_WRITE, MAP_SHARED, device, base->offset);
	if (base->accessor == MAP_FAILED) {
		return FALSE;
	} else {
		// Calculate the offset needed by read / write functions to get startAddress as 0
		base->offset = startAddress - base->offset;
		return TRUE;
	}
}

void UnmapMemory(raw_memory *base) {
	if (base->accessor != MAP_FAILED) {
		munmap(base->accessor, base->offset + base->length);
		base->accessor = MAP_FAILED;
	} else {
	}
}
