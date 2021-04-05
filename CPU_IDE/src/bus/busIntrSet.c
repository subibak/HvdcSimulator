
#include	"usrTypes.h"
#include	"sysDefs.h"
#include	"sysConfig.h"
#include	"lkv120Board.h"
#include	"vmeDef.h"
#include	"usrErrCode.h"
#include	"osDepend.h"

#ifdef VXWORKS

#define	VME_IRQ_LVL1	(1)
#define	VME_IRQ_LVL2	(2)
#define	PCI_INTR_STAT_VIRQ_OFFSET 	0x304
#define	VME_BUS_INTR_STAT_OFFSET	0x314


uint32	gVmeBusErrOccur = 0;


uint32 systemIntrSet(void);

static uint32 vmeBusErrIntrSet(void);
static void vmeBusErrIntrFunc(int32);

void brdAccFailErrIntrFunc(int32 );
static uint32 brdAccFailErrIntrSet(void);

void pciLocalBusErrIntrFunc(int32 );
static uint32 pciLocalBusErrIntrSet(void);

static void exceptionHandlingIntrSet(void);
static int32 excIntrFunc();

#if (BOARD_TYPE == MVME2100_BOARD)
	extern int32 sysUnivIntDisable(int32);
	extern int32 sysUnivIntEnable(int32);
	extern int32 sysUnivIntConnect ( int32 , voidfunc_ptr, int32);
	extern void sysPciOutLong(uint32,uint32);
	extern uint32 sysPciInLong(uint32);
#endif

extern int32 errRetFunc();
extern int32 	sysHwIntrSet(int32 , voidfunc_ptr, int32);
extern void debugMsgDisplayInISR(	uint8 *, uint8 *, uint8 *, uint32, uint32, uint32,uint32,uint32);


#if (BOARD_TYPE == MVME2100_BOARD)
	extern uint32	univBaseAdrs;
#endif


uint32 systemIntrSet(void)
{
	uint32	status = NO_ERROR;

	int32 oldLevel;

    oldLevel = intLock();

    
	exceptionHandlingIntrSet();

    
	status = vmeBusErrIntrSet();
    if(status != NO_ERROR)
    	goto MODULE_END;

	
	status = brdAccFailErrIntrSet();
    if(status != NO_ERROR)
    	goto MODULE_END;

	
	status = pciLocalBusErrIntrSet();
    if(status != NO_ERROR)
    	goto MODULE_END;

MODULE_END:
	intUnlock(oldLevel);

    return(status);
}


static uint32 vmeBusErrIntrSet(void)
{
	uint32	status = NO_ERROR;
	int32	intrArg;

    intrArg = 0;

#if (BOARD_TYPE == MVME2100_BOARD)

   	status = sysUnivIntDisable(VME_BUS_ERR);
    if(status != NO_ERROR){
 		return(status);
	}

	status = (uint32)sysUnivIntConnect (	VME_BUS_ERR,
    										vmeBusErrIntrFunc,
    										intrArg                                        
										);
	if(status != NO_ERROR)
    	status = HW_INTERRUPT_SET_ERR;
	else {
		
    	status = sysUnivIntEnable((VME_BUS_ERR));
	}
	
#elif ((BOARD_TYPE == MVME3100_BOARD)||(BOARD_TYPE == MVME4100_BOARD)||(BOARD_TYPE == MVME2500_BOARD))
	status = TSI148_INT_DISABLE(VME_BUS_ERR);
	if(status !=  NO_ERROR) return(status);

    status = sysHwIntrSet(VME_BUS_ERR, vmeBusErrIntrFunc, 0); 

	if(status != NO_ERROR)
    	status = HW_INTERRUPT_SET_ERR;
	else 
    	status = TSI148_INT_ENABLE(VME_BUS_ERR);
#endif

    return(status);
}


static void vmeBusErrIntrFunc(int32 intrArg)
{
	
	
	gVmeBusErrOccur = 1;
	
	debugMsgDisplayInISR(	"vmeBusErrIntrFunc", 
							"VME BUS ERR",
   	        			    __FILE__,__LINE__,0,0,0,0
						);

	return;  
}


static uint32 brdAccFailErrIntrSet(void)
{
	uint32	status = NO_ERROR;
	int32	intrArg;

    intrArg = 0;

#if (BOARD_TYPE == MVME2100_BOARD)
   	status = sysUnivIntDisable(ACC_FAIL_ERR);
    if(status != NO_ERROR) return(status);

	status = (uint32)sysUnivIntConnect (	ACC_FAIL_ERR,
    										brdAccFailErrIntrFunc,
    										intrArg                                        
										);
	if(status != NO_ERROR)
    	status = HW_INTERRUPT_SET_ERR;
	else
    	status = sysUnivIntEnable(ACC_FAIL_ERR);

#elif ((BOARD_TYPE == MVME3100_BOARD)||(BOARD_TYPE == MVME4100_BOARD)||(BOARD_TYPE == MVME2500_BOARD))
	
	status = TSI148_INT_DISABLE(ACC_FAIL_ERR);
	if(status !=  NO_ERROR) return(status);

    status = sysHwIntrSet(ACC_FAIL_ERR, brdAccFailErrIntrFunc, 0); 

	if(status != NO_ERROR)
    	status = HW_INTERRUPT_SET_ERR;
	else 
    	status = TSI148_INT_ENABLE(ACC_FAIL_ERR);

#endif

    return(status);
}


void brdAccFailErrIntrFunc(int32 intrArg)
{
	debugMsgDisplayInISR(	"brdAccFailErrIntrFunc", 
							"ACCESS FAIL ERROR",
   	        			    __FILE__,__LINE__,0,0,0,0
						);
	return;  
}


static uint32 pciLocalBusErrIntrSet(void)
{
	uint32	status = NO_ERROR;
	int32	intrArg;
   
   	intrArg = 0;

#if (BOARD_TYPE == MVME2100_BOARD)
 
   	status = sysUnivIntDisable(LOCAL_BUS_ERR);
    if(status != NO_ERROR) return(status);

	status = (uint32)sysUnivIntConnect (	LOCAL_BUS_ERR,
    										pciLocalBusErrIntrFunc,
    										intrArg                                        
										);
	if(status != NO_ERROR)
    	status = HW_INTERRUPT_SET_ERR;
	else
    	status = sysUnivIntEnable(LOCAL_BUS_ERR);

#elif ((BOARD_TYPE == MVME3100_BOARD)||(BOARD_TYPE == MVME4100_BOARD)||(BOARD_TYPE == MVME2500_BOARD))
	
	status = TSI148_INT_DISABLE(LOCAL_BUS_ERR);
	if(status !=  NO_ERROR) return(status);

    status = sysHwIntrSet(LOCAL_BUS_ERR, pciLocalBusErrIntrFunc, 0); 

	if(status != NO_ERROR)
    	status = HW_INTERRUPT_SET_ERR;
	else 
    	status = TSI148_INT_ENABLE(LOCAL_BUS_ERR);

#endif

    return(status);
}


void pciLocalBusErrIntrFunc(int32 intrArg)
{
	debugMsgDisplayInISR(	"pciLocalBusErrIntrFunc", 
							"PCI LOCAL BUS ERROR",
   	        			    __FILE__,__LINE__,0,0,0,0
						);

	return;  
}


static void exceptionHandlingIntrSet(void)
{
    excVecSet ((func_ptr *) DATA_EXP_VECTOR_OFFSET, func_ref(excIntrFunc));
    excVecSet ((func_ptr *) MCH_EXP_VECTOR_OFFSET, func_ref(excIntrFunc));
    excVecSet ((func_ptr *) ALN_EXP_VECTOR_OFFSET, func_ref(excIntrFunc));
}


static int32 excIntrFunc
    (
    ESFPPC *    pEsf		
    )
{
    REG_SET *pRegSet = &pEsf->regSet;

   	pRegSet->pc = (_RType)errRetFunc;	

	debugMsgDisplayInISR(	"excIntrFunc", 
							"EXCEPTION ERROR",
   	        			    __FILE__,__LINE__,0,0,0,0
						);
    return (0);
}



#endif



