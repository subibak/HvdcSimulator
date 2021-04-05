
#include	"usrTypes.h"
#include	"sysDefs.h"
#include	"sysConfig.h"
#include	"lkv120Board.h"
#include	"vmeDef.h"
#include	"sysDual.h"
#include	"usrErrCode.h"

#include	"osDepend.h"

#ifdef VXWORKS

#if (BOARD_TYPE == MVME2100_BOARD)
	#define CNFG_PCI_HOST_BRDG		0x80000000 
	#define KAHLUA_CFG_STATUS		0x06	
	
	#define KAHLUA_PCI_RCV_MSTR_ABORT  0x2000  
	#define KAHLUA_PCI_RCV_TGT_ABORT   0x1000  
	
	#define PCI_MSTR_MEM_LOCAL    0x80000000		
	
	#define	VME_LM_MSTR_SIZE	0x00010000
	#define	VME_LM_MSTR_BUS		PCI_MSTR_MEM_LOCAL + 0x7b000000
	
	#define	VME_A32_MSTR_SIZE	0x08000000
	#define	VME_A32_MSTR_LOCAL	PCI_MSTR_MEM_LOCAL
	
	#define	VME_A24_MSTR_SIZE	0x01000000
	#define	VME_A24_MSTR_LOCAL	(PCI_MSTR_MEM_LOCAL + 0x70000000)
	
	#define	VME_A16_MSTR_SIZE	0x00010000
	#define	VME_A16_MSTR_LOCAL	(PCI_MSTR_MEM_LOCAL + 0x71ff0000)

#elif ((BOARD_TYPE == MVME3100_BOARD)||(BOARD_TYPE == MVME4100_BOARD)||(BOARD_TYPE == MVME2500_BOARD)||(BOARD_TYPE == LKV120_BOARD))

	#define TSI148_VME_BUS_ERR_REG			0x268
	#define TSI148_VME_BUS_ERR_MASK			(1<<31)
	#define TSI148_VME_BUS_ERR_CLE_MASK		(1<<29)

	#define TSI148_READ32(base,reg)  *(uint32 *)(base+reg)

	#define TSI148_WRITE32(base,reg,val) \
		 { *(uint32 *)(base+reg)=val; \
		   (void)sysInLong(base+reg); }

#endif


#if (BOARD_TYPE == MVME2100_BOARD)
	extern uint32	univBaseAdrs;
#elif ((BOARD_TYPE == MVME3100_BOARD)||(BOARD_TYPE == MVME4100_BOARD)||(BOARD_TYPE == MVME2500_BOARD)||(BOARD_TYPE == LKV120_BOARD))
	extern uint32 gTempeBaseAdrs;
#endif

#if (SYSTEM_DATA_SYNC_BOARD == ETH3_BOARD)
extern  strSysDualInfo gRmtShelfDualInfo;
#endif 

extern uint32 gVmeBusErrOccur;


#if (BOARD_TYPE == LKV120_BOARD)   
	extern STATUS VmeAccess (char *adrs, int mode, int length, char *pVal);
	extern void sysProbeErrClr (void);
#endif

extern void setErrorCode(uint8 *,uint32,uint8 *,uint32);


extern int32 vmeDataAcc(int32, int8 *, int8 *);
extern  void memoryCopy (uint8 *, uint8 *, uint32);
extern void fourBytesDataCopy ( uint32 *, uint32 *, uint32 );
extern void logMessageSet (int8 *, uint32, int8 *, uint32, uint32 );

#if (BOARD_TYPE == MVME2100_BOARD)
	extern  uint16 sysPciConfigInWord (uint16 *);
	extern  void   sysPciConfigOutWord (uint16 *, uint16);
	extern  uint32 sysPciInLong (uint32);
	extern  void   sysPciOutLong (uint32, uint32);
	extern void sysUnivVERRClr (void);
#endif


uint32	myVmeDataAcc(	uint8 *fileName, int32 lineNum,
						int32	accDataType, int32 mode, 
						uint8 *vmeAddrPtr, uint8 *bufPtr
					)
{
	uint32	status = NO_ERROR;
	uint8	*srcPtr = NULL, *destPtr = NULL;

#if (BOARD_TYPE == MVME2100_BOARD)
	uint16	temp;
	uint32	pciSlv1Ctl = 0, lsiCtlReg = 0, vmeAddr;
    strSysConfigInfo configData;

    
    if( (accDataType != BUS_BYTE_ACC) && (accDataType != BUS_WORD_ACC) &&
        (accDataType != BUS_LONG_ACC) 
	){
		status = BUS_DATA_ACC_TYPE_ERR;
        return(status);
    }

    if( (mode != BUS_DATA_READ) && (mode != BUS_DATA_WRITE)
	){
		status = BUS_ACC_TYPE_ERR;
        return(status);
    }

	systemCnfgInfoRead((strSysConfigInfo *)&configData);
#if (BOARD_TYPE == LKV120_BOARD)
	if((configData.memoryBoardType == MEM_BOARD_NONE)||(configData.memoryBoardType == MEM_4MB_MRAM)) {
#else
	if(configData.memoryBoardType == MEM_BOARD_NONE) {
#endif
	
	      
	    if(mode == BUS_DATA_READ) {
			srcPtr 	= vmeAddrPtr;
	        destPtr	= bufPtr;
	    }
	    else if(mode == BUS_DATA_WRITE){
			srcPtr 	= bufPtr;
	        destPtr = vmeAddrPtr;
	    }
		memoryCopy((uint8*)destPtr,(uint8*)srcPtr,accDataType);
		return (status);
	}
	
    

    sysPciConfigOutWord((uint16 *)(CNFG_PCI_HOST_BRDG + KAHLUA_CFG_STATUS),
                        (KAHLUA_PCI_RCV_MSTR_ABORT | KAHLUA_PCI_RCV_TGT_ABORT));

    
    if(mode == BUS_DATA_READ) {
		srcPtr 	= vmeAddrPtr;
        destPtr	= bufPtr;
    }
    else if(mode == BUS_DATA_WRITE){
		srcPtr 	= bufPtr;
        destPtr = vmeAddrPtr;

    	
        vmeAddr  = (uint32)vmeAddrPtr;
		lsiCtlReg = universeCtrlRegAdrGet(vmeAddr);

		if(lsiCtlReg == 0) return( PCI_ADDR_ERR);

    	pciSlv1Ctl = sysPciInLong (lsiCtlReg);
    	sysPciOutLong (lsiCtlReg, (pciSlv1Ctl & ~LSI_CTL_WP));
    }

	status = vmeDataAcc(accDataType, srcPtr, destPtr);
	
	if(status != NO_ERROR) {
    	status = BUS_DATA_ACC_ERR;
		setErrorCode( __FILE__,
						__LINE__,
						"myVmeDataAcc",
						status
			   		   );			
	}
    
    temp = sysPciConfigInWord((uint16 *)(CNFG_PCI_HOST_BRDG + KAHLUA_CFG_STATUS));
    if (temp & (KAHLUA_PCI_RCV_MSTR_ABORT | KAHLUA_PCI_RCV_TGT_ABORT)) {
        status = BUS_DATA_ACC_ERR;
		setErrorCode( __FILE__,
						__LINE__,
						"myVmeDataAcc",
						status
			   		   );			
	}

    
    sysPciConfigOutWord((uint16 *)(CNFG_PCI_HOST_BRDG + KAHLUA_CFG_STATUS), 
			(KAHLUA_PCI_RCV_MSTR_ABORT | KAHLUA_PCI_RCV_TGT_ABORT));

    

    if ((mode == BUS_DATA_WRITE) && (pciSlv1Ctl & LSI_CTL_WP))
    {
        sysPciOutLong (lsiCtlReg, pciSlv1Ctl);
    }

	
	if ((gVmeBusErrOccur) || (status != NO_ERROR)) {
		
		vmeBusErrClear ();
		
		fatalErrMsgSet(	(int8*)fileName, 
						lineNum,
						"myVmeDataAcc", 
						BUS_DATA_ACC_ERR, 
						(uint32)vmeAddrPtr
					);
	}
#elif ((BOARD_TYPE == MVME3100_BOARD) || (BOARD_TYPE == MVME4100_BOARD) || (BOARD_TYPE == MVME2500_BOARD))
    uint32 tsi148Reg;
    strSysConfigInfo configData;
    
    
    if( (accDataType != BUS_BYTE_ACC) && (accDataType != BUS_WORD_ACC) &&
        (accDataType != BUS_LONG_ACC) 
	){
		status = BUS_DATA_ACC_TYPE_ERR;
        return(status);
    }

    if( (mode != BUS_DATA_READ) && (mode != BUS_DATA_WRITE)
	){
		status = BUS_ACC_TYPE_ERR;
        return(status);
    }

	systemCnfgInfoRead((strSysConfigInfo *)&configData);
#if (BOARD_TYPE == LKV120_BOARD)
	if((configData.memoryBoardType == MEM_BOARD_NONE)||(configData.memoryBoardType == MEM_4MB_MRAM)) {
#else
	if(configData.memoryBoardType == MEM_BOARD_NONE) {
#endif
	
	      
	    if(mode == BUS_DATA_READ) {
			srcPtr 	= vmeAddrPtr;
	        destPtr	= bufPtr;
	    }
	    else if(mode == BUS_DATA_WRITE){
			srcPtr 	= bufPtr;
	        destPtr = vmeAddrPtr;
	    }
		memoryCopy((uint8*)destPtr,(uint8*)srcPtr,accDataType);
		return (status);
	}
	
    
    if(mode == BUS_DATA_READ) {
		srcPtr 	= vmeAddrPtr;
        destPtr	= bufPtr;
    }
    else if(mode == BUS_DATA_WRITE){
		srcPtr 	= bufPtr;
        destPtr = vmeAddrPtr;

    }

	status = vmeDataAcc(accDataType, srcPtr, destPtr);
	if(status != NO_ERROR) 
		setErrorCode( __FILE__,
						__LINE__,
						"myVmeDataAcc",
						status
			   		   );			

	
	
	if ((gVmeBusErrOccur) || (status != NO_ERROR)) {
		
		
	    tsi148Reg = TSI148_READ32 (gTempeBaseAdrs, TSI148_VME_BUS_ERR_REG);

	    
	    if ((tsi148Reg & TSI148_VME_BUS_ERR_MASK) != NO_ERROR) {
	    	status = BUS_DATA_ACC_ERR;
			setErrorCode( __FILE__,
							__LINE__,
							"myVmeDataAcc",
							status
				   		   );			
		}

		
		if(status)
			fatalErrMsgSet(	(int8*)fileName, 
							lineNum,
							"myVmeDataAcc", 
							BUS_DATA_ACC_ERR, 
							(uint32)vmeAddrPtr
						);

		
		vmeBusErrClear();
	}
#elif (BOARD_TYPE == LKV120_BOARD)

    uint32 tsi148Reg;
    int vme_mode;
    int len;
    strSysConfigInfo configData;

    
    
    if( (accDataType != BUS_BYTE_ACC) && (accDataType != BUS_WORD_ACC) &&
        (accDataType != BUS_LONG_ACC) 
	){
		status = BUS_DATA_ACC_TYPE_ERR;
        return(status);
    }

    switch (accDataType) {
    case BUS_BYTE_ACC:
        len = sizeof(int8);
        break;
    case BUS_WORD_ACC:
        len = sizeof(int16);
        break;
    case BUS_LONG_ACC:
        len = sizeof(int32);
        break;
    default:
        len = sizeof(int32);
        break;
    }

    if( (mode != BUS_DATA_READ) && (mode != BUS_DATA_WRITE)
	){
		status = BUS_ACC_TYPE_ERR;
        return(status);
    }

	systemCnfgInfoRead((strSysConfigInfo *)&configData);

#if (BOARD_TYPE == LKV120_BOARD)
	if((configData.memoryBoardType == MEM_BOARD_NONE)||(configData.memoryBoardType == MEM_4MB_MRAM)) {
#else
	if(configData.memoryBoardType == MEM_BOARD_NONE) {
#endif
	
	      
	    if(mode == BUS_DATA_READ) {
			srcPtr 	= vmeAddrPtr;
	        destPtr	= bufPtr;
	    }
	    else if(mode == BUS_DATA_WRITE){
			srcPtr 	= bufPtr;
	        destPtr = vmeAddrPtr;
	    }
		memoryCopy((uint8*)destPtr,(uint8*)srcPtr,accDataType);
		return (status);
	}
	
    
    if(mode == BUS_DATA_READ) {
        vme_mode =  VX_READ;
        
    }
    else if(mode == BUS_DATA_WRITE){
        vme_mode =  VX_WRITE;
    }
    

    status = VmeAccess(vmeAddrPtr, vme_mode,len,bufPtr);
	
#if 0
    status = NO_ERROR;
#endif
	
	if(status != NO_ERROR)
    {
        printf("%s %d,%s,%d\n", __FILE__,
                        __LINE__,
                        "myVmeDataAcc",
                        status
                       );
		setErrorCode( __FILE__,
                        __LINE__,
                        "myVmeDataAcc",
                        status
                       );			
    }
	

#endif

    return(status);
}
#endif

