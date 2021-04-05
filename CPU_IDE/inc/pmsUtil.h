
#include "sysDefs.h"





#define MMEM_DWORD(index) (*(volatile uint32*)(MPU_FB_MEMORY_START_BUS_ADDR + M_MEMORY_AREA_START_OFFSET + ((index)*4)))
#define MMEM_WORD(index) (*(volatile uint16*)(MPU_FB_MEMORY_START_BUS_ADDR + M_MEMORY_AREA_START_OFFSET + ((index)*2)))

#define	MIN_MB_BO_ADDR	    (0)
#define	MIN_MB_BI_ADDR	(10000)
#define	MIN_MB_AI_ADDR	(30000)
#define	MIN_MB_AO_ADDR	(40000)

#if (MB_SLAVE_MAP_TYPE != MB_SLAVE_MAP_SAMSUNG)
#define	MAX_MB_BO_ADDR	 (3199)
#define	MAX_MB_BI_ADDR	(13199)
#define	MAX_MB_AI_ADDR	(31999)
#define	MAX_MB_AO_ADDR	(41999)
#elif  (MB_SLAVE_MAP_TYPE == MB_SLAVE_MAP_SAMSUNG)
#define	MAX_MB_BO_ADDR	(9999)  
#define	MAX_MB_BI_ADDR	(19999) 
#define	MAX_MB_AI_ADDR	(39999) 
#define	MAX_MB_AO_ADDR	(49999) 
#endif

#define	MODBUS_BO	(0x10)
#define	MODBUS_BI	(0x11)
#define	MODBUS_AI	(0x13)
#define	MODBUS_AO	(0x14)



	


#define SMEM_WORD(index) (*(volatile uint16*)(MPU_FB_MEMORY_START_BUS_ADDR + S_MEMORY_AREA_START_OFFSET + ((index)*2)))


#if (MB_SLAVE_MAP_TYPE != MB_SLAVE_MAP_SAMSUNG)
#define	MODBUS_BO_MMEM_START_INDEX	(0)			
#define	MODBUS_BI_MMEM_START_INDEX	(1000)
#define	MODBUS_AI_MMEM_START_INDEX	(3000)	
#define	MODBUS_AO_MMEM_START_INDEX	(4000)	

#elif  (MB_SLAVE_MAP_TYPE == MB_SLAVE_MAP_SAMSUNG)

#define	MODBUS_BO_MMEM_START_INDEX	(0)			
#define	MODBUS_BI_MMEM_START_INDEX	(500)		
#define	MODBUS_AI_MMEM_START_INDEX	(5000)		
#define	MODBUS_AO_MMEM_START_INDEX	(10000)		

#endif

#define DNP_BO_SWMEM_START_INDEX	(0)		
#define DNP_BOR_SWMEM_START_INDEX	(512)
#define DNP_BI_SWMEM_START_INDEX	(1024)
#define DNP_AI_SWMEM_START_INDEX	(2048)
#define DNP_AO_SWMEM_START_INDEX	(3052)
#define DNP_AOR_SWMEM_START_INDEX	(3564)