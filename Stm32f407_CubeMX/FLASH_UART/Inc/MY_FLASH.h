#include "stm32f4xx_hal.h"

/*Sector address*/
#define SECTORADD_0 		0x08000000
#define SECTORADD_1 		0x08004000
#define SECTORADD_2	 		0x08008000
#define SECTORADD_3 		0x0800C000
#define SECTORADD_4 		0x08010000
#define SECTORADD_5 		0x08020000
#define SECTORADD_6 		0x08040000
#define SECTORADD_7 		0x08060000
#define SECTORADD_8 		0x08080000
#define SECTORADD_9 		0x080A0000
#define SECTORADD_10 		0x080C0000
#define SECTORADD_11 		0x080E0000

//Typedefs
//1. data size
typedef enum
{
	DATA_TYPE_8=0,
	DATA_TYPE_16,
	DATA_TYPE_32,
}DataTypeDef;

//functions prototypes
//1. Erase Sector
static void MY_FLASH_EraseSector(void);

//2. Set Sector Adress
void MY_FLASH_SetSectorAddrs(uint8_t sector, uint32_t addrs);
//3. Write Flash
void MY_FLASH_WriteN(uint32_t idx, void *wrBuf, uint32_t Nsize, DataTypeDef dataType);
//4. Read Flash
void MY_FLASH_ReadN(uint32_t idx, void *rdBuf, uint32_t Nsize, DataTypeDef dataType);
