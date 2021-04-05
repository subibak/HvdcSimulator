#include "ControlMain.h"
#include "Hal/HalTS201.h"
#include "Hal/HalLed.h"

extern uint32_t ulSignalQofManager;
extern uint32_t	ulTimerPeriod;

//extern uint32_t ulMasterSlave;
//extern uint32_t ulPeriod;
//extern uint32_t ulRcvCnt;

uint32_t ulSignalQofControl = SIGNAL_NONE;

CORE_STATE eControlState = STATE_BOOT;
CORE_STATE ePrevControlState = STATE_BOOT;
	
uint32_t ulControlStartCnt	= 0;
uint32_t ulControlEndCnt	= 0;


extern uint32_t		t_ulControlMst;
extern ST_FCOMM_TYPE	t_stFCommTx[HALFCOMM_CH_NUM];
extern ST_FCOMM_TYPE	t_stFCommRx[HALFCOMM_CH_NUM];




void ControlInit(void)
{

//	eControlState		= STATE_INIT;
//	ePrevControlState	= STATE_INIT;

	ulControlStartCnt	= 0;
	ulControlEndCnt		= 0;

//	ClearSignal(&ulSignalQofControl,		0xFFFFFFFF);


	return;
}

uint32_t *pulManagerSignalQ	= &ulSignalQofManager;
uint32_t	pulDebugTemp = 0;

uint32_t	ulS2LedColor = HALLED_LED_BLUE;

void ControlMain(void)
{

	uint32_t *pulMySignalQ 		= &ulSignalQofControl;
	uint32_t *pulOtherSignalQ	= pulManagerSignalQ;

	CORE_STATE *pePrevState = &ePrevControlState;
	CORE_STATE *peState		= &eControlState;
	CORE_STATE eNextState;

	switch(eControlState)
	{
		case STATE_BOOT:
			InitTS201();
			//DelayUs(1000);		
			WriteReg(0x30000001, CORE_BOOTUP_READYCODE);
			SetState(pePrevState, peState, STATE_INIT);
			ClearSignal(pulMySignalQ, 0xFFFFFFFF);
			break;

		case STATE_INIT:

			ControlInit();
				
			//SetSignal(pulOtherSignalQ,	SIGNAL_CORE_RESET_DONE);
			if(CheckSignal(pulMySignalQ, SIGNAL_DO_INIT) == false)
			{
				;
			}
			else
			{
				/* Initialize System Variable */
				ControlAppInit();

				HalTS0201MPEnable();
				ClearSignal(pulMySignalQ,	SIGNAL_DO_INIT);
				SetSignal(pulOtherSignalQ,	SIGNAL_INIT_DONE);

				if(t_ulControlMst == SYNC_MASTER)
				{
					SetState(pePrevState, peState, STATE_MASTER_NORMAL);
					ulS2LedColor = HALLED_LED_BLUE;
				}
				else
				{
					SetState(pePrevState, peState, STATE_SLAVE_NORMAL);
					ulS2LedColor = HALLED_LED_GREEN;
				}
			}
			break;
		case STATE_MASTER_NORMAL:
		case STATE_SLAVE_NORMAL:
			/*****************************************************************************/
			/* Stage 1: Wait Sync														 */
			/*****************************************************************************/
			if((*pulMySignalQ & SIGNAL_SYNC_DETECTED) == SIGNAL_SYNC_DETECTED)
			{
				ulControlStartCnt++;
				ClearSignal(pulMySignalQ, SIGNAL_SYNC_DETECTED);
				HalLedCtrlSet(HALLED_STA2LED, HALLED_ON, ulS2LedColor);
				
#if 0
				ControlAppIOHandle();
				HalLedCtrlSet(HALLED_STA2LED, HALLED_OFF, ulS2LedColor);

				SetSignal(pulOtherSignalQ, SIGNAL_RELEASE_BUS);					

				/*****************************************************************************/
				/* Stage 3: Main Algorithm													 */
				/*****************************************************************************/
				ControlAppRun();

#else
				/*****************************************************************************/
				/* Stage 2: Send Data and Read DAta										 */
				/*****************************************************************************/
				switch(ulSystemCfg)
				{
					case SYSTEM_MES:
						IalControlSend();
						IalControlReceive();
						HalLedCtrlSet(HALLED_STA2LED, HALLED_OFF, ulS2LedColor);
						SetSignal(pulOtherSignalQ, SIGNAL_RELEASE_BUS);					
						MesAppRun();
						break;
					case SYSTEM_CCS:
						IalControlSend();
						IalControlReceive();
						HalLedCtrlSet(HALLED_STA2LED, HALLED_OFF, ulS2LedColor);
						SetSignal(pulOtherSignalQ, SIGNAL_RELEASE_BUS);					
						CcsAppRun();
						break;
					case SYSTEM_VBC:
						IalControlSend();
						IalControlReceive();
						HalLedCtrlSet(HALLED_STA2LED, HALLED_OFF, ulS2LedColor);
						SetSignal(pulOtherSignalQ, SIGNAL_RELEASE_BUS);					
						VbcAppRun();
						break;
					default:
						break;
				}
#endif

//					HalLedCtrlSet(HALLED_STA2LED, HALLED_OFF, ulS2LedColor);
//				SetSignal(pulOtherSignalQ, SIGNAL_RELEASE_BUS);					
				ulControlEndCnt++;

			}
			else
			{
				eNextState = ProcessSignal(pulMySignalQ, *peState);

				if(eNextState != *peState)
				{
					SetState(pePrevState, peState, eNextState);
				}
				
			}
			break;
#if 0

		case STATE_SLAVE_NORMAL:
			/*****************************************************************************/
			/* Stage 1: Wait Sync														 */
			/*****************************************************************************/
			if((*pulMySignalQ & SIGNAL_SYNC_DETECTED) == SIGNAL_SYNC_DETECTED)
			{
				ulControlStartCnt++;
				ClearSignal(pulMySignalQ, SIGNAL_SYNC_DETECTED);

				/*****************************************************************************/
				/* Stage 2: Send Data and Read DAta										 */
				/*****************************************************************************/
				ControlAppIOHandle();

				SetSignal(pulOtherSignalQ, SIGNAL_RELEASE_BUS);					

				/*****************************************************************************/
				/* Stage 3: Main Algorithm													 */
				/*****************************************************************************/

 				memcpy(t_stFCommTx[HALFCOMM_CH_2].stBuffer.uBody.p32Buffer, t_stFCommRx[HALFCOMM_CH_1].stBuffer.uBody.p32Buffer, t_stFCommTx[HALFCOMM_CH_2].stBuffer.ulSize);
				memcpy(t_stFCommTx[HALFCOMM_CH_3].stBuffer.uBody.p32Buffer, t_stFCommRx[HALFCOMM_CH_1].stBuffer.uBody.p32Buffer, t_stFCommTx[HALFCOMM_CH_3].stBuffer.ulSize);


				ulControlEndCnt++;

			}
			else
			{
				eNextState = ProcessSignal(pulMySignalQ, *peState);

				if(eNextState != *peState)
				{
					SetState(pePrevState, peState, eNextState);
				}
				
			}
			break;
#endif
		case STATE_FAULT:
			eNextState = ProcessSignal(pulMySignalQ, *peState);

			if(eNextState != *peState)
			{
				SetState(pePrevState, peState, eNextState);
			}
			break;
				
		default:
			break;
	}

}


volatile uint32_t ulDummy=0;

void IsrExtIrq(int lDummy)
{
	ulDummy++;

}

char		cDummy[12];

void main(void)
{
	bool blLoop = true;
	
	strcpy((char *)cDummy, "DUMMY");

	interrupt(SIGIRQ3, IsrExtIrq);			// uart rx isr
	HalTS201ExtInt();

	while(blLoop)
	{
		ControlMain();
	}
	
}

