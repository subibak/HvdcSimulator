
#ifndef PCSDEF_H
#define PCSDEF_H


#define     PCS_AUTO_MODE		(0x01)
#define     PCS_MANUAL_MODE		(0x02)

#define     PCS_NO_MODE			(0x00)
#define     PCS_INIT			(0x01)
#define     PCS_DOWNLOAD		(0x02)
#define     PCS_DOWNLOAD_DONE	(0x03)
#define     PCS_STOP			(0x04)
#define     PCS_RUN				(0x05)
#define     PCS_DEBUG			(0x06)
#define     PCS_ERR				(0x07)

#define	MIN_STATION_ID			(1)
#define	MAX_STATION_ID			(64)


#define	ALL_ID		(0x00)

#ifndef	LOGIC_ID
	#define	LOGIC_ID	(0x10)
#endif

#define	NET_ID		(0x50)

#define	AUTO_VAR	(0x01)
#define	MMEM_VAR	(0x02)
#define	SMEM_VAR	(0x03)


#define	REQUEST		(0x1)
#define	RESPONSE	(0x2)
#define	BROADCAST	(0x3)
#define	MULTICAST	(0x4)

#if  (SYSTEM_DATA_SYNC_BOARD	== ETH3_BOARD)
	#define RMTREQST	(0x5)
	#define RMTRESPS	(0x6)
	#define RMTCAST		(0x7)
	#define RMTREAD		(0x8)
#endif

#define	MAX_IO_POINT_NUM	(32)
#define	MAX_IO_CARD_NUM		(12)

#if (MAXIOSHELF_CONFIG == MAX16IOSHELF)
	#define	MAX_IO_SHELF_NUM	(16)
#elif (MAXIOSHELF_CONFIG == MAX8IOSHELF)
	#define	MAX_IO_SHELF_NUM	(8)
#elif (MAXIOSHELF_CONFIG == MAX12IOSHELF)
	#define	MAX_IO_SHELF_NUM	(12)
#else 
	#error "You Should Define MAXIOSHELF_CONFIG(8 or 16)!!!!"
#endif

#define	MAX_FIELDBUS_SLAVE_NUM	(124)


#define	TMTC_MAX_CHANNEL_NUM	15


#define	MAX_DIU_SEG_ID 		(MAX_IO_SHELF_NUM)
#define	MAX_TMTC_SEG_ID 	(TMTC_MAX_CHANNEL_NUM)


#define FORCING_DISABLE		(0)
#define FORCING_ENABLE		(1)


#define	DATA_SYNC_DISABLE	(0)
#define	DATA_SYNC_ENABLE	(1)

#define	LOCAL_RAW_OFFSET	((float)(682.5))
#define	LOCAL_RAW_FACTOR	((float)(2730.0))


#define	OLD_LOCAL_RAW_OFFSET	((float)(400.0))
#define	OLD_LOCAL_RAW_FACTOR	((float)(1600.0))

#define	LOCAL_ENG_OFFSET	((float)(-819.0))
#define	LOCAL_ENG_FACTOR	((float)(32.76))
#define	LOCAL_ENG_LIMIT		((float)(3276.0))
#define	MAX_RAW_DATA		(4095)
#define	MIN_RAW_DATA		(0)

#define	OLD_LOCAL_ENG_OFFSET	((float)(-400.0))
#define	OLD_LOCAL_ENG_FACTOR	((float)(16.0))
#define	OLD_LOCAL_ENG_LIMIT		((float)(1600.0))

#define	OLD_MAX_RAW_DATA		(2047)
#define	OLD_MIN_RAW_DATA		(0)



#define	BIT16_LOCAL_RAW_OFFSET	((float)(10922.5))
#define	BIT16_LOCAL_RAW_FACTOR	((float)(43690.0))

#define	TCM16_LOCAL_RAW_OFFSET	((float)(0.0))
#define	TCM16_LOCAL_RAW_FACTOR	((float)(65535.0))


#define	BIT16_LOCAL_ENG_OFFSET	((float)(-13107.0))
#define	BIT16_LOCAL_ENG_FACTOR	((float)(524.28))
#define	BIT16_LOCAL_ENG_LIMIT	((float)(52428.0))
#define	BIT16_MAX_RAW_DATA		(65535)
#define	BIT16_MIN_RAW_DATA		(0)


#define	MAX_PI_INPUT_COUNT 65535


#define	TOTAL_DWL	(0)
#define	SEG_DWL		(1)


#define	MAX_SEG_EXEC_COUNT	(0x7fffffff)


#define	XR_GEN_MAX_TIME		60 


#define	NO_RETAIN	(0)
#define	EN_RETAIN	(1)
#ifdef _WIN32
#define	NO_RETAIN_CHECK (0)
#endif
#define EN_RETAIN_CHECK (1)
#define NO_SAVE			(0)
#define EN_SAVE		    (1)


#endif 
