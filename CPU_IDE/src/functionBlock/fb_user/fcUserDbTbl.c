
#include	<stdio.h>
#include	<stdlib.h>

#include	"usrTypes.h"
#include	"fbDef.h"

#include	"fb_code\fb_user\fcSignal.h"

#include	"fb_code\fb_user\fcZ1_59harm_HDP.h"
#include	"fb_code\fb_user\fcZ1_27_ABUV.h"
#include	"fb_code\fb_user\fcZ1_59_ABOV.h"
#include	"fb_code\fb_user\fcZ1_64_OVG.h"
#include	"fb_code\fb_user\fcZ1_81O_OFP.h"
#include	"fb_code\fb_user\fcZ1_81U_UFP.h"
#include	"fb_code\fb_user\fcZ1_5051_PTOC.h"
#include	"fb_code\fb_user\fcZ1_50N51N.h"
#include	"fb_code\fb_user\fcZ2_5051_TOCP.h"
#include	"fb_code\fb_user\fcZ3_27_UV.h"
#include	"fb_code\fb_user\fcZ2_50N51N_TEOCP.h"
#include	"fb_code\fb_user\fcZ2_87N_REFP.h"
#include	"fb_code\fb_user\fcZ2_26W_TTOLP.h"
#include	"fb_code\fb_user\fcZ2_DcSat_TDSP.h"
#include	"fb_code\fb_user\fcZ2_87_TDP.h"
#include	"fb_code\fb_user\fcZ3_59_OV.h"
#include	"fb_code\fb_user\fcZ3_64_GOV.h"
#include	"fb_code\fb_user\fcZ3_50S51S_OCP.h"
#include	"fb_code\fb_user\fcZ3_49_PROP.h"
#include	"fb_code\fb_user\fcZ3_51G_GOCP.h"
#include	"fb_code\fb_user\fcZ3_87ST_TTDP.h"
#include	"fb_code\fb_user\fcZ3_51Tharm_AHCP.h"
#include	"fb_code\fb_user\fcZ3_60T_PCI.h"
#include	"fb_code\fb_user\fcZ3_87TA_TACDP.h"
#include	"fb_code\fb_user\fcZ4_27D_DUP.h"
#include	"fb_code\fb_user\fcZ4_59D_DOP.h"
#include	"fb_code\fb_user\fcZ4_60VPN_DVDP.h"
#include	"fb_code\fb_user\fcZ4_76A_AOCP.h"
#include	"fb_code\fb_user\fcZ4_76D_DOCP.h"
#include	"fb_code\fb_user\fcZ4_87AD_ADCDP.h"
#include	"fb_code\fb_user\fcZ4_87DPN_DCDP.h"
#include	"fb_code\fb_user\fcZ4_60APN.h"
#include	"fb_code\fb_user\fcZ4_51Aharm.h"
#include	"fb_code\fb_user\fcZ4_51Dharm.h"
#include	"fb_code\fb_user\fcSwiYardSeq.h"

/*********************************************************************
**				
**	사용자가 정의 한 Function Code
**
*********************************************************************/
strFbExecDbInfo	gUsrDefFbDbTableInfo[MAX_FB_NUM + 1] =
{
/*			fbName, 				fb ID   		INP   				VAR     			OUT  					Init Func    					Run Func  		    	Type Pnt
            ------   				------  		----  				---  	  			---   					-------------       			-------------       	--------*/
	{(uint8 *)"RAMP",				RAMP_CODE,		RAMP_SPEC_NUM,		RAMP_VAR_NUM,		RAMP_OUTPUT_NUM,		rampInitFunc,					rampRunFunc,				NULL},
	
/* FB ID : 700 ~ 1000까지 정의 */

	{(uint8 *)"Z1_59harm_HDP",		FC0700_CODE,	FC0700_SPEC_NUM,	FC0700_VAR_NUM,		FC0700_OUTPUT_NUM,		stdZ1_59harm_HDPInitFunc,		stdZ1_59harm_HDPRunFunc,	NULL},
	{(uint8 *)"Z1_27_ABUV",			FC0701_CODE,	FC0701_SPEC_NUM,	FC0701_VAR_NUM,		FC0701_OUTPUT_NUM,		stdZ1_27_ABUVInitFunc,			stdZ1_27_ABUVRunFunc,		NULL},
	{(uint8 *)"Z1_59_ABOV",			FC0702_CODE,	FC0702_SPEC_NUM,	FC0702_VAR_NUM,		FC0702_OUTPUT_NUM,		stdZ1_59_ABOVInitFunc,			stdZ1_59_ABOVRunFunc,		NULL},
	{(uint8 *)"Z1_64_OVG",			FC0703_CODE,	FC0703_SPEC_NUM,	FC0703_VAR_NUM,		FC0703_OUTPUT_NUM,		stdZ1_64_OVGInitFunc,			stdZ1_64_OVGRunFunc,		NULL},
	{(uint8 *)"Z1_81O_OFP",			FC0704_CODE,	FC0704_SPEC_NUM,	FC0704_VAR_NUM,		FC0704_OUTPUT_NUM,		stdZ1_81O_OFPInitFunc,			stdZ1_81O_OFPRunFunc,		NULL},
	{(uint8 *)"Z1_81U_UFP",			FC0705_CODE,	FC0705_SPEC_NUM,	FC0705_VAR_NUM,		FC0705_OUTPUT_NUM,		stdZ1_81U_UFPInitFunc,			stdZ1_81U_UFPRunFunc,		NULL},
	{(uint8 *)"Z1_5051_PTOC",		FC0706_CODE,	FC0706_SPEC_NUM,	FC0706_VAR_NUM,		FC0706_OUTPUT_NUM,		stdZ1_5051_PTOCInitFunc,		stdZ1_5051_PTOCRunFunc,		NULL},
	{(uint8 *)"Z1_50N51N",			FC0707_CODE,	FC0707_SPEC_NUM,	FC0707_VAR_NUM,		FC0707_OUTPUT_NUM,		stdZ1_50N51NPInitFunc,			stdZ1_50N51NRunFunc,		NULL},
	{(uint8 *)"Z2_5051_TOCP",		FC0708_CODE,	FC0708_SPEC_NUM,	FC0708_VAR_NUM,		FC0708_OUTPUT_NUM,		stdZ2_5051_TOCPInitFunc,		stdZ2_5051_TOCPRunFunc,		NULL},
	{(uint8 *)"Z2_50N51N_TEOCP",	FC0709_CODE,	FC0709_SPEC_NUM,	FC0709_VAR_NUM,		FC0709_OUTPUT_NUM,		stdZ2_50N51N_TEOCPInitFunc,		stdZ2_50N51N_TEOCPRunFunc,	NULL},
	{(uint8 *)"Z2_87N_REFP",		FC0710_CODE,	FC0710_SPEC_NUM,	FC0710_VAR_NUM,		FC0710_OUTPUT_NUM,		stdZ2_87N_REFPInitFunc,			stdZ2_87N_REFPRunFunc,		NULL},
	{(uint8 *)"Z2_26W_TTOLP",		FC0711_CODE,	FC0711_SPEC_NUM,	FC0711_VAR_NUM,		FC0711_OUTPUT_NUM,		stdZ2_26W_TTOLPInitFunc,		stdZ2_26W_TTOLPRunFunc,		NULL},
	{(uint8 *)"Z2_DcSat_TDSP",		FC0712_CODE,	FC0712_SPEC_NUM,	FC0712_VAR_NUM,		FC0712_OUTPUT_NUM,		stdZ2_DcSat_TDSPInitFunc,		stdZ2_DcSat_TDSPRunFunc,	NULL},
	{(uint8 *)"Z2_87_TDP",			FC0713_CODE,	FC0713_SPEC_NUM,	FC0713_VAR_NUM,		FC0713_OUTPUT_NUM,		stdZ2_87_TDPInitFunc,			stdZ2_87_TDPRunFunc,		NULL},
	{(uint8 *)"Z3_27_UV",			FC0714_CODE,	FC0714_SPEC_NUM,	FC0714_VAR_NUM,		FC0714_OUTPUT_NUM,		stdZ3_27_UVInitFunc,			stdZ3_27_UVRunFunc,			NULL},
	{(uint8 *)"Z3_59_OV",			FC0715_CODE,	FC0715_SPEC_NUM,	FC0715_VAR_NUM,		FC0715_OUTPUT_NUM,		stdZ3_59_OVInitFunc,			stdZ3_59_OVRunFunc,			NULL},
	{(uint8 *)"Z3_64_GOV",			FC0716_CODE,	FC0716_SPEC_NUM,	FC0716_VAR_NUM,		FC0716_OUTPUT_NUM,		stdZ3_64_GOVInitFunc,			stdZ3_64_GOVRunFunc,		NULL},
	{(uint8 *)"Z3_50S51S_OCP",		FC0717_CODE,	FC0717_SPEC_NUM,	FC0717_VAR_NUM,		FC0717_OUTPUT_NUM,		stdZ3_50S51S_OCPInitFunc,		stdZ3_50S51S_OCPRunFunc,	NULL},
	{(uint8 *)"Z3_49_PROP",			FC0718_CODE,	FC0718_SPEC_NUM,	FC0718_VAR_NUM,		FC0718_OUTPUT_NUM,		stdZ3_49_PROPInitFunc,			stdZ3_49_PROPRunFunc,		NULL},
	{(uint8 *)"Z3_51G_GOCP",		FC0719_CODE,	FC0719_SPEC_NUM,	FC0719_VAR_NUM,		FC0719_OUTPUT_NUM,		stdZ3_51G_GOCPInitFunc,			stdZ3_51G_GOCPRunFunc,		NULL},
	{(uint8 *)"Z3_87ST_TTDP",		FC0720_CODE,	FC0720_SPEC_NUM,	FC0720_VAR_NUM,		FC0720_OUTPUT_NUM,		stdZ3_87ST_TTDPInitFunc,		stdZ3_87ST_TTDPRunFunc,		NULL},
	{(uint8 *)"Z3_51Tharm_AHCP",	FC0721_CODE,	FC0721_SPEC_NUM,	FC0721_VAR_NUM,		FC0721_OUTPUT_NUM,		stdZ3_51Tharm_AHCPInitFunc,		stdZ3_51Tharm_AHCPRunFunc,	NULL},
	{(uint8 *)"Z3_60T_PCI",			FC0722_CODE,	FC0722_SPEC_NUM,	FC0722_VAR_NUM,		FC0722_OUTPUT_NUM,		stdZ3_60T_PCIInitFunc,			stdZ3_60T_PCIRunFunc,		NULL},
	{(uint8 *)"Z3_87TA_TACDP",		FC0723_CODE,	FC0723_SPEC_NUM,	FC0723_VAR_NUM,		FC0723_OUTPUT_NUM,		stdZ3_87TA_TACDPInitFunc,		stdZ3_87TA_TACDPRunFunc,	NULL},
	{(uint8 *)"Z4_27D_DUP",			FC0724_CODE,	FC0724_SPEC_NUM,	FC0724_VAR_NUM,		FC0724_OUTPUT_NUM,		stdZ4_27D_DUPInitFunc,			stdZ4_27D_DUPRunFunc,		NULL},
	{(uint8 *)"Z4_59D_DOP",			FC0725_CODE,	FC0725_SPEC_NUM,	FC0725_VAR_NUM,		FC0725_OUTPUT_NUM,		stdZ4_59D_DOPInitFunc,			stdZ4_59D_DOPRunFunc,		NULL},
	{(uint8 *)"Z4_60VPN_DVDP",		FC0726_CODE,	FC0726_SPEC_NUM,	FC0726_VAR_NUM,		FC0726_OUTPUT_NUM,		stdZ4_60VPN_DVDPInitFunc,		stdZ4_60VPN_DVDPRunFunc,	NULL},
	{(uint8 *)"Z4_76A_AOCP",		FC0727_CODE,	FC0727_SPEC_NUM,	FC0727_VAR_NUM,		FC0727_OUTPUT_NUM,		stdZ4_76A_AOCPInitFunc,			stdZ4_76A_AOCPRunFunc,		NULL},
	{(uint8 *)"Z4_76D_DOCP",		FC0728_CODE,	FC0728_SPEC_NUM,	FC0728_VAR_NUM,		FC0728_OUTPUT_NUM,		stdZ4_76A_DOCPInitFunc,			stdZ4_76A_DOCPRunFunc,		NULL},
	{(uint8 *)"Z4_87AD_ADCDP",		FC0729_CODE,	FC0729_SPEC_NUM,	FC0729_VAR_NUM,		FC0729_OUTPUT_NUM,		stdZ4_87AD_ADCDPInitFunc,		stdZ4_87AD_ADCDPRunFunc,	NULL},
	{(uint8 *)"Z4_87DPN_DCDP",		FC0730_CODE,	FC0730_SPEC_NUM,	FC0730_VAR_NUM,		FC0730_OUTPUT_NUM,		stdZ4_87DPN_DCDPInitFunc,		stdZ4_87DPN_DCDPRunFunc,	NULL},
	{(uint8 *)"Z4_60APN",			FC0731_CODE,	FC0731_SPEC_NUM,	FC0731_VAR_NUM,		FC0731_OUTPUT_NUM,		stdZ4_60APNInitFunc,			stdZ4_60APNRunFunc,			NULL},
	{(uint8 *)"Z4_51Aharm",			FC0732_CODE,	FC0732_SPEC_NUM,	FC0732_VAR_NUM,		FC0732_OUTPUT_NUM,		stdZ4_51AharmInitFunc,			stdZ4_51AharmRunFunc,		NULL},
	{(uint8 *)"Z4_51Dharm",			FC0733_CODE,	FC0733_SPEC_NUM,	FC0733_VAR_NUM,		FC0733_OUTPUT_NUM,		stdZ4_51DharmInitFunc,			stdZ4_51DharmRunFunc,		NULL},
/* 아래의 FB Code는 입/출력, 내부 변수 갯수 확인요 */
	{(uint8 *)"SwiYardSeq",			FC0740_CODE,	FC0740_SPEC_NUM,	FC0740_VAR_NUM,		FC0740_OUTPUT_NUM,		stdZ4_SwiYardSeqInitFunc,		stdZ4_SwiYardSeqRunFunc,	NULL}
};