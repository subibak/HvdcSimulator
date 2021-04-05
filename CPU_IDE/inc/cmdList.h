
#ifndef COMMANDLIST_H
#define COMMANDLIST_H



#define     SYS_STATUS_CMD		(0x01)
    #define	SYS_DIAG_STATUS						(0x01) 
    #define	SYS_HISTORY_READ					(0x02) 
    #define	SYS_TAKS_STATUS_READ 				(0x03) 
    #define	SYS_INFODATA_READ	 				(0x04) 
	#define	SYS_DATA_WTITE		 				(0x05)

#define		MODE_ACCESS_CMD		(0x02)
	#define	MODE_SEG_READ						(0x01)
    #define	MODE_SEG_WRITE						(0x02)
    #define	MODE_CONTROL_READ					(0x03)
    #define	MODE_CONTROL_WRITE					(0x04)

#define		CODE_ACCESS_CMD		(0x03)
	#define	CODE_SEG_DOWN						(0x01)
    #define	CODE_FB_DOWN	 					(0x02)
    #define	CODE_SEQTBL_DOWN					(0x03)
    #define	CODE_OUTPUT_READ					(0x04)
    #define	CODE_OUTPUT_WRITE					(0x05)
    #define	CODE_PARA_READ						(0x06)
    #define	CODE_PARA_WRITE						(0x07)

	#define CODE_STATUS_READ					(0x08)
	#define	IO_CONFIG_READ						(0x09)
	
	#define	SFC_INFO_DOWNLOAD					(0x0a)
	#define	SFC_LINK_DOWNLOAD					(0x0b)
	#define	SFC_ACTCODE_DOWNLOAD 				(0x0c)
	#define	SFC_TRSCODE_DOWNLOAD 				(0x0d)
	#define SFC_STATUS_READ						(0x0e)
	#define SFC_STEP_JUMP						(0x0f) 
	
#define	FB_ONLINE_DOWNLOAD_CMD	(0x04)
    #define	FB_ONLINE_DOWNLOAD_ADD			 	(0x01) 	
    #define	FB_ONLINE_DOWNLOAD_DEL			 	(0x02) 	
    #define	FB_ONLINE_DOWNLOAD_SPEC_CHG		 	(0x03) 	
    #define	FB_ONLINE_DOWNLOAD_SEQ_ID_CHG	 	(0x04) 	

    #define	FB_ONLINE_DOWNLOAD_VAR_INFO_CHG	 	(0x05) 	
    #define	FB_ONLINE_DOWNLOAD_TRS_DATA_CHG	 	(0x06) 	

    #define	FB_PARA_MEM_REALLOC				 	(0x07) 	
    #define	FB_ONLINE_LOGIC_UPDATE_OR_CANCEL 	(0x08) 	

	#define VERSION_INFO_WRITE				 	(0x09)
	#define VERSION_INFO_READ				 	(0x0a)
	#define MEMORY_STAUTS_READ				 	(0x0b)
    #define	FB_ONLINE_DOWNLOAD_FBINFO_CHG	 	(0x0c) 	
	#define	IO_ONLINE_DOWNLOAD_SPEC_CHG		 	(0x0d)
	
#define		VAR_ACCESS_CMD		(0x05)
	#define	VAR_BLOCK_READ						(0x01)
    #define	VAR_ITEM_READ						(0x02)
    #define	VAR_BLOCK_WRITE						(0x03)
    #define	VAR_ITEM_WRITE						(0x04)
    #define	VAR_INFO_DOWN						(0x05)
    #define	VAR_ALLMEM_READ						(0x06)
    #define	VAR_ALLMEM_WRITE					(0x07)
    
#define		FRC_ACCESS_CMD		(0x06)
	#define	FRC_FLAG_WRITE						(0x01)
    #define	FRC_STATION_WRITE					(0x02)
    #define	FRC_STATUS_READ						(0x03)
    #define	FRC_TAKS_WRITE						(0x04)



#define		XR_BROADCAST_CMD	(0x07)
	#define	XR_SEND								(0x01)
	
#define		TIME_ACCESS_CMD		(0x08)
	#define	TIME_SYNC_BROAD						(0x01)
    #define	TIME_READ_RCS						(0x02)
    #define	TIME_WRITE_RCS						(0x03)

#define	ETHERNET_DATA_ACC_OP	(0x09)
	#define	ETHERNET_DATA_READ	 				(0x1)
    #define	ETHERNET_DATA_WRITE	 				(0x2)
    #define	ETHERNET_DATA_BROAD	 				(0x3)

#define	ETHERNET_COM_PATH_OP	(0x0a)
	#define	ETHERNET_DEV_STATUS_READ			(0x1)

#define	MIN_PROTOCOL_CMD	SYS_STATUS_CMD 			
#define	MAX_PROTOCOL_CMD	ETHERNET_COM_PATH_OP	     	

#endif 
