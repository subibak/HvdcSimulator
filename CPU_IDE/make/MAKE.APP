# makefile for user application
#
#
#
CPU             = PPC32
##optional soft-float kernel
#TOOL		= e500v2gnu
TOOL		= e500v2diab
CPU_VARIANT	= _ppc85XX_e500v2

USR_SRC_PATH		= D:/HVDC_Simulator/CPU_IDE/src
USR_INC_PATH    	= D:/HVDC_Simulator/CPU_IDE/inc
MBTS_INC_PATH    	= D:/HVDC_Simulator/CPU_IDE/inc/modbustcp
MHCAP_INC_PATH    	= D:/HVDC_Simulator/CPU_IDE/src/MHCAP

WRS_NET_PATH		= $(WIND_COMPONENTS)/$(COMP_IPNET2)/vxcompat/include


# BOARD_TYPE : LKV120_BOARD	(5)
# MAXIOSHELF_CONFIG : MAX8IOSHELF		(2)

ADDED_CFLAGS = 	-DRW_MULTI_THREAD -D_REENTRANT -D"BOARD_TYPE=5" -D"MAXIOSHELF_CONFIG=2" -D_MBTS_PROGRAM 
TGT_DIR = $(WIND_BASE)/target
VSB_DIR = $(TGT_DIR)/lib

include $(TGT_DIR)/h/make/defs.bsp
#include $(TGT_DIR)/h/make/make.$(CPU)$(TOOL)
#include $(TGT_DIR)/h/make/defs.$(WIND_HOST_TYPE)

EXTRA_INCLUDE = -I$(USR_INC_PATH) -I$(WRS_NET_PATH) -I$(MBTS_INC_PATH) -I$(MHCAP_INC_PATH)
#-I$(TGT_DIR)/config/lk11_lkv_120

#사용자가 정의한 INCLUDE 
USR_INC_COMMON_FILE	= $(USR_INC_PATH)/lkv120Board.h $(USR_INC_PATH)/cmdList.h $(USR_INC_PATH)/debugInfo.h \
$(USR_INC_PATH)/fbDef.h $(USR_INC_PATH)/fbMem.h $(USR_INC_PATH)/fbForcing.h \
$(USR_INC_PATH)/usrDefBuf.h $(USR_INC_PATH)/usrErrCode.h $(USR_INC_PATH)/memDef.h\
$(USR_INC_PATH)/msgQue.h $(USR_INC_PATH)/msgQueData.h $(USR_INC_PATH)/netProtocol.h\
$(USR_INC_PATH)/network.h $(USR_INC_PATH)/osDepend.h $(USR_INC_PATH)/pcsDefs.h\
$(USR_INC_PATH)/protocolBuf.h $(USR_INC_PATH)/segScheduler.h $(USR_INC_PATH)/sfcMem.h \
$(USR_INC_PATH)/sysConfig.h $(USR_INC_PATH)/sysDefs.h $(USR_INC_PATH)/sysDual.h\
$(USR_INC_PATH)/systemDiag.h $(USR_INC_PATH)/taskDefs.h $(USR_INC_PATH)/usrQueData.h \
$(USR_INC_PATH)/usrSema.h $(USR_INC_PATH)/usrTypes.h $(USR_INC_PATH)/vmeDef.h \
$(USR_INC_PATH)/pmsUtil.h $(USR_INC_PATH)/usrDef.h

FB_STANDARD_INC_FILE	= $(USR_INC_PATH)/fb_code/fb_standard/fcArithmetic.h \
$(USR_INC_PATH)/fb_code/fb_standard/fcBitshift.h \
$(USR_INC_PATH)/fb_code/fb_standard/fcBitStableEle.h \
$(USR_INC_PATH)/fb_code/fb_standard/fcBitwise.h\
$(USR_INC_PATH)/fb_code/fb_standard/fcComparison.h\
$(USR_INC_PATH)/fb_code/fb_standard/fcComparison.h \
$(USR_INC_PATH)/fb_code/fb_standard/fcCounter.h \
$(USR_INC_PATH)/fb_code/fb_standard/fcEdgeDect.h\
$(USR_INC_PATH)/fb_code/fb_standard/fcExtra.h\
$(USR_INC_PATH)/fb_code/fb_standard/fcMiscellary.h\
$(USR_INC_PATH)/fb_code/fb_standard/fcNumerical.h\
$(USR_INC_PATH)/fb_code/fb_standard/fcSelection.h \
$(USR_INC_PATH)/fb_code/fb_standard/fcTimer.h \
$(USR_INC_PATH)/fb_code/fb_standard/fcTypeConv.h \
$(USR_INC_PATH)/fb_code/fb_standard/fcVmeBrdAcc.h 

FB_USER_INC_FILE	= $(USR_INC_PATH)/fb_code/fb_user/fcSignal.h\
$(USR_INC_PATH)/fb_code/fb_user/fcPrtCommon.h\
$(USR_INC_PATH)/fb_code/fb_user/fcZ1_59harm_HDP.h\
$(USR_INC_PATH)/fb_code/fb_user/fcZ1_27_ABUV.h\
$(USR_INC_PATH)/fb_code/fb_user/fcZ1_59_ABOV.h\
$(USR_INC_PATH)/fb_code/fb_user/fcZ1_64_OVG.h\
$(USR_INC_PATH)/fb_code/fb_user/fcZ1_81O_OFP.h\
$(USR_INC_PATH)/fb_code/fb_user/fcZ1_81U_UFP.h\
$(USR_INC_PATH)/fb_code/fb_user/fcZ1_5051_PTOC.h\
$(USR_INC_PATH)/fb_code/fb_user/fcZ1_50N51N.h\
$(USR_INC_PATH)/fb_code/fb_user/fcZ2_5051_TOCP.h\
$(USR_INC_PATH)/fb_code/fb_user/fcZ2_50N51N_TEOCP.h\
$(USR_INC_PATH)/fb_code/fb_user/fcZ2_87N_REFP.h\
$(USR_INC_PATH)/fb_code/fb_user/fcZ2_26W_TTOLP.h\
$(USR_INC_PATH)/fb_code/fb_user/fcZ2_DcSat_TDSP.h\
$(USR_INC_PATH)/fb_code/fb_user/fcZ3_27_UV.h\
$(USR_INC_PATH)/fb_code/fb_user/fcZ3_59_OV.h\
$(USR_INC_PATH)/fb_code/fb_user/fcZ3_64_GOV.h\
$(USR_INC_PATH)/fb_code/fb_user/fcZ3_49_PROP.h\
$(USR_INC_PATH)/fb_code/fb_user/fcZ3_51G_GOCP.h\
$(USR_INC_PATH)/fb_code/fb_user/fcZ3_87ST_TTDP.h\
$(USR_INC_PATH)/fb_code/fb_user/fcZ3_51Tharm_AHCP.h\
$(USR_INC_PATH)/fb_code/fb_user/fcZ3_60T_PCI.h\
$(USR_INC_PATH)/fb_code/fb_user/fcZ3_87TA_TACDP.h\
$(USR_INC_PATH)/fb_code/fb_user/fcZ4_27D_DUP.h\
$(USR_INC_PATH)/fb_code/fb_user/fcZ4_59D_DOP.h\
$(USR_INC_PATH)/fb_code/fb_user/fcZ4_60VPN_DVDP.h\
$(USR_INC_PATH)/fb_code/fb_user/fcZ4_76A_AOCP.h\
$(USR_INC_PATH)/fb_code/fb_user/fcZ4_76D_DOCP.h\
$(USR_INC_PATH)/fb_code/fb_user/fcZ4_87AD_ADCDP.h\
$(USR_INC_PATH)/fb_code/fb_user/fcZ4_87DPN_DCDP.h\
$(USR_INC_PATH)/fb_code/fb_user/fcZ4_60APN.h\
$(USR_INC_PATH)/fb_code/fb_user/fcZ4_51Aharm.h\
$(USR_INC_PATH)/fb_code/fb_user/fcZ4_51Dharm.h\
$(USR_INC_PATH)/fb_code/fb_user/fcSwiYardSeq.h

FB_COMMON_INC_FILE	= $(USR_INC_PATH)/fb_code/fb_common/fbRunningErrCode.h 

USR_INC_RMT_COM_FILE = $(USR_INC_PATH)/rmt_Comm/rmtDevUtil.h $(USR_INC_PATH)/rmt_Comm/ethModbus.h

USR_MHCAP_FILE	=  $(USR_SRC_PATH)/MHCAP/link_lib.h \
$(USR_SRC_PATH)/MHCAP/lkp502.h \
$(USR_SRC_PATH)/MHCAP/mhcap_inc.h \
$(USR_SRC_PATH)/MHCAP/mhcap_types.h \
$(USR_SRC_PATH)/MHCAP/project_def.h \
$(USR_SRC_PATH)/MHCAP/vme.h

USR_INC_FILE 	= $(USR_INC_COMMON_FILE) $(FB_STANDARD_INC_FILE) $(FB_USER_INC_FILE)\
$(FB_COMMON_INC_FILE) $(USR_INC_RMT_COM_FILE)  $(USR_MHCAP_FILE)
				  
# 사용자 응용 프로그램
APP_TASK_OBJ	= logicTask.o sysDiagTask.o sysDualTask.o usrAppInitTask.o \
			  usrComTask.o  usrDbgTask.o usrTaskCreate.o  

BUS_OBJ		= busDataAcc.o busIntrSet.o 

DEBUG_OBJ	= appDbg.o msgLogging.o rtErrDisplay.o 


DIAGNOSIS_OBJ	= systemDiag.o 

FB_STANDARD_OBJ	= fcArithmetic.o fcBitshift.o fcBitStableEle.o\
			  fcBitwise.o fcComparison.o\
			  fcCounter.o fcEdgeDect.o fcExtra.o\
			  fcMiscellary.o fcNumerical.o fcSelection.o\
			  fcTimer.o fcTypeConv.o fcVmeBrdAcc.o \
			  fcStandardDbTbl.o

FB_USER_OBJ	= fcSignal.o fcUserDbTbl.o\
			  fcPrtCommon.o\
			  fcZ1_59harm_HDP.o\
			  fcZ1_27_ABUV.o\
			  fcZ1_59_ABOV.o\
			  fcZ1_64_OVG.o\
			  fcZ1_81O_OFP.o\
			  fcZ1_81U_UFP.o\
			  fcZ1_5051_PTOC.o\
			  fcZ1_50N51N.o\
			  fcZ2_5051_TOCP.o\
			  fcZ2_50N51N_TEOCP.o\
			  fcZ2_87N_REFP.o\
			  fcZ2_26W_TTOLP.o\
			  fcZ2_DcSat_TDSP.o\
			  fcZ2_87_TDP.o\
			  fcZ3_27_UV.o\
			  fcZ3_59_OV.o\
			  fcZ3_64_GOV.o\
			  fcZ3_49_PROP.o\
			  fcZ3_51G_GOCP.o\
			  fcZ3_87ST_TTDP.o\
			  fcZ3_51Tharm_AHCP.o\
			  fcZ3_60T_PCI.o\
			  fcZ3_87TA_TACDP.o\
			  fcZ3_50S51S_OCP.o\
			  fcZ4_27D_DUP.o\
			  fcZ4_59D_DOP.o\
			  fcZ4_60VPN_DVDP.o\
			  fcZ4_76A_AOCP.o\
			  fcZ4_76D_DOCP.o\
			  fcZ4_87AD_ADCDP.o\
			  fcZ4_87DPN_DCDP.o\
			  fcZ4_60APN.o\
			  fcZ4_51Aharm.o\
			  fcZ4_51Dharm.o\
			  fcSeqCommon.o\
			  fcSwiYardSeq.o

FB_UTILITY_OBJ	= fbCommon.o fbForcing.o fbLogicExec.o fbMemAcc.o fbOnlineDwl.o fbVarAcc.o sfcMemAcc.o 

MHCAP_OBJ = common.o link_lib.o lkp502.o mhcap_task.o vme.o vme_disp.o 

MODBUSTCP_OBJ = mbtServerProc.o mbtServerData.o mbtServerDebug.o mbtCodec.o mbtDiag.o

NET_COMMAND_OBJ		= fbAccessCmd.o fbForcingCmd.o fbOnlineDwlCmd.o\
			  fbVarAccessCmd.o sysControlCmd.o sysDataExhgCmd.o\
			  sysDiagnosisCmd.o sysStatusExhgCmd.o sysTimeCmd.o 

NETWORK_OBJ	= netDualMsg.o netUsrLib.o rmtComUtil.o 

PMS_OBJ		= pmsUtil.o  pmsCommon.o

SYS_DUAL_OBJ	= sysDual.o sysDualAccCmd.o

SYSTEM_OBJ	= logicTaskMngr.o sysUsrLib.o sysUtility.o 

TARGET_OBJ	= bootInit.o lkv120Board.o 
		  
UTILITY_OBJ	= dataChkSum.o usrAppLib.o 

VERSION_OBJ	= swVer.o

OBJS_ALL =  $(APP_TASK_OBJ) $(BUS_OBJ) $(DEBUG_OBJ)  \
			$(DIAGNOSIS_OBJ) $(FB_STANDARD_OBJ) \
			$(FB_UTILITY_OBJ) $(MHCAP_OBJ) $(MODBUSTCP_OBJ)\
			$(NET_COMMAND_OBJ) $(NETWORK_OBJ) $(PMS_OBJ)\
			$(SYS_DUAL_OBJ) $(SYSTEM_OBJ) $(TARGET_OBJ)\
			$(UTILITY_OBJ) $(VERSION_OBJ)\
			$(FB_USER_OBJ) 
     
exe	:$(OBJS_ALL) 
	$(LD) $(LDFLAGS) -r -o romApp.o $^

$(TARGET_OBJ)	: %.o : $(USR_SRC_PATH)/target/%.c $(USR_INC_FILE) 
	$ $(RM) $@
	$(CC) $(CFLAGS) $< -c -o $@

$(DIAGNOSIS_OBJ)	: %.o : $(USR_SRC_PATH)/diagnosis/%.c $(USR_INC_FILE) 
	$ $(RM) $@
	$(CC) $(CFLAGS) $< -c -o $@

$(NET_COMMAND_OBJ)	: %.o : $(USR_SRC_PATH)/net_command/%.c $(USR_INC_FILE) 
	$ $(RM) $@
	$(CC) $(CFLAGS) $< -c -o $@

$(UTILITY_OBJ)	: %.o : $(USR_SRC_PATH)/utility/%.c $(USR_INC_FILE) 
	$ $(RM) $@
	$(CC) $(CFLAGS) $< -c -o $@
$(DEBUG_OBJ)	: %.o : $(USR_SRC_PATH)/debug/%.c $(USR_INC_FILE) 
	$ $(RM) $@
	$(CC) $(CFLAGS) $< -c -o $@

$(SYS_DUAL_OBJ)	: %.o : $(USR_SRC_PATH)/sys_dual/%.c $(USR_INC_FILE) 
	$ $(RM) $@
	$(CC) $(CFLAGS) $< -c -o $@

$(FB_UTILITY_OBJ)	: %.o : $(USR_SRC_PATH)/functionBlock/fb_utility/%.c $(USR_INC_FILE) 
	$ $(RM) $@
	$(CC) $(CFLAGS) $< -c -o $@

$(FB_STANDARD_OBJ)	: %.o : $(USR_SRC_PATH)/functionBlock/fb_standard/%.c $(USR_INC_FILE) 
	$ $(RM) $@
	$(CC) $(CFLAGS) $< -c -o $@

$(FB_USER_OBJ)	: %.o : $(USR_SRC_PATH)/functionBlock/fb_user/%.c $(USR_INC_FILE) 
	$ $(RM) $@
	$(CC) $(CFLAGS) $< -c -o $@
	
$(FB_FMC_OBJ)	: %.o : $(USR_SRC_PATH)/fb_code/fieldbus/%.c $(USR_INC_FILE) 
	$ $(RM) $@
	$(CC) $(CFLAGS) $< -c -o $@

$(NETWORK_OBJ)	: %.o : $(USR_SRC_PATH)/network/%.c $(USR_INC_FILE) 
	$ $(RM) $@
	$(CC) $(CFLAGS) $< -c -o $@

$(SYSTEM_OBJ)	: %.o : $(USR_SRC_PATH)/system/%.c $(USR_INC_FILE) 
	$ $(RM) $@
	$(CC) $(CFLAGS) $< -c -o $@

$(APP_TASK_OBJ)	: %.o : $(USR_SRC_PATH)/app_task/%.c $(USR_INC_FILE) 
	$ $(RM) $@
	$(CC) $(CFLAGS) $< -c -o $@

$(VERSION_OBJ)	: %.o : $(USR_SRC_PATH)/version/%.c $(USR_INC_FILE) 
	$ $(RM) $@
	$(CC) $(CFLAGS) $< -c -o $@

$(BUS_OBJ)	: %.o : $(USR_SRC_PATH)/bus/%.c $(USR_INC_FILE) 
	$ $(RM) $@
	$(CC) $(CFLAGS) $< -c -o $@
	
$(PMS_OBJ)	: %.o : $(USR_SRC_PATH)/pms/%.c $(USR_INC_FILE) 
	$ $(RM) $@
	$(CC) $(CFLAGS) $< -c -o $@

$(MODBUSTCP_OBJ)	: %.o : $(USR_SRC_PATH)/modbustcp/%.c $(USR_INC_FILE) 
	$ $(RM) $@
	$(CC) $(CFLAGS) $< -c -o $@

$(MHCAP_OBJ): %.o : $(USR_SRC_PATH)/MHCAP/%.c $(USR_INC_FILE) 
	$ $(RM) $@
	$(CC) $(CFLAGS) $< -c -o $@
	
clean :
	$ $(RM) *.o

