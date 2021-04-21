/**************************************************************************
** Switch Yard Sequence(FB740)
**************************************************************************/
#ifndef FCSWIYARDSEQ_H
#define FCSWIYARDSEQ_H

/*************************************************************************
**	Function  Prototype
**************************************************************************/
extern uint32	stdZ4_SwiYardSeqInitFunc(uint32 *,uint32 *,uint32 *,strFbDefInfo *);
extern uint32	stdZ4_SwiYardSeqRunFunc(uint32, uint32);

/*********************************************************************
** Switch Yard Sequence FB 	
**********************************************************************/
#define	FC0740_INIT					0
#define	FC0740_RUN					1

#define	FC0740_CODE					740

/* Spec, Var, Output ¡Æ©ö¨ùo E¢çAI */
#define	FC0740_SPEC_NUM				6
#define	FC0740_VAR_NUM				3	
#define	FC0740_OUTPUT_NUM			3


// Input Variable
//UNION NMAE_STRUCTURE NAME_STATE 
#define SM_STS_ERROR				2
#define SM_STS_READY				1
#define SM_STS_UNREADY				0
#define SM_OP_AUTOMATIC				1
#define SM_OP_MANUAL				0
#define SM_SEQOP_HOLD				4
#define SM_SEQOP_START				2
#define SM_SEQOP_STOP				1
#define SM_ACTPWR_P					2
#define SM_ACTPWR_VDC				1
#define SM_FREQ_EN					1
#define SM_FREQ_DEN					0
#define SM_THIRD_EN					1
#define SM_THIRD_DEN				0
#define SM_RCTPWR_Q					2
#define SM_RCTPWR_VAC				1
#define SM_SEQDIR_START				4
#define SM_SEQDIR_STOP				2
#define SM_SEQDIR_NODIR				1

union UN_SYS_MODE
{
	uint32				all;
	struct {
		uint32			rsvd1:10;		// Reserved
		uint32			seqDir:3;		// Sequence Direction(4:Start Direction, 2:Stop direction, 0:Stop direction)	
		uint32			hmiEmst:1;		// HMI Emergency Stop(1: On, 0: Off)
		uint32			convReset:1;	// Converter Reset(1: Reset, 0: None)	
		uint32			allTripRest:1;	// All Trip Reset(1: Reset, 0: None)	
		uint32			convdDblk:1;	// Converter Block(1: Start, 0: Stop)	
		uint32			convOp:1;		// Converter Operation(1:Start, 0: Stop)	
		uint32			actChg:1;		// Active Charging(1: Start, 0: Stop)
		uint32			rctPwr:2;		// Reactive Power Mode(2:Q Mode, 1:Vac Mode)
		uint32			third:1;		// Third Harmonic Mode(1:Enable, 0:Disable)	
		uint32			freq:1;			// Frequency Mode(1:Enable, 0:Disable)	
		uint32			actPwr:2;		// Active Power Mode(2:P Mode, 1:Vdc Mode)	
		uint32			seqOp:3;		// Sequence Operation(4:Hold, 2:Start, Stop:1)	
		uint32			rsvd0:1;		// Reserved
		uint32			operation:1;	// System Operation Mode(1:Automatic, 0:Manual)	
		uint32			status:2;		// System Status(2:Error, 1:Ready, 0:Unready)	
	}	bit;
};

#define SS_DOORINTLOCK_ERROR		2 
#define SS_DOORINTLOCK_READY		1 
#define SS_DOORINTLOCK_UNREADY		0 
#define SS_SMSHORTSWI_ERROR			2
#define SS_SMSHORTSWI_OPEN			1
#define SS_SMSHORTSWI_CLOSE			0
#define SS_CONVDISCHG_ERROR			2
#define SS_CONVDISCHG_EXPIRED		1
#define SS_CONVDISCHG_UNREADY		0
#define SS_INTLCK_ERROR				2
#define SS_INTLCK_COMPLETE			1
#define SS_INTLCK_UNCOMPLETE		0
#define SS_PASSIVECHG_ERROR			2
#define SS_PASSIVECHG_COMPLETE		1
#define SS_PASSIVECHG_UNCOMPLETE	0
#define SS_ACTCHGSEQ_ERROR			2
#define SS_ACTCHGSEQ_COMPLETE		1
#define SS_ACTCHGSEQ_UNCOMPLETE		0
#define SS_CONVBLKSEQ_ERROR			2
#define SS_CONVBLKSEQ_COMPLETE		1
#define SS_CONVBLKSEQ_UNCOMPLETE	0

//System Status
union UN_SS_STS
{
	uint32				all;
	struct {
		uint32			rsvd:11;		// Reserved
		uint32			RampVacInit:1	// Ramp Vac Init(1:Complete,:Uncomplete)
		uint32			RampQInit:1		// Ramp Q Init(1:Complete,:Uncomplete)
		uint32			RampVdcInit:1	// Ramp Vdc Init(1:Complete,:Uncomplete)
		uint32			RampPInit:1		// Ramp P Init(1:Complete,:Uncomplete)		
		uint32			RampVacRated:1	// Ramp Vac Rated(1:Complete,:Uncomplete)
		uint32			RampQRated:1	// Ramp Q Rated(1:Complete,:Uncomplete)
		uint32			RampVdcRated:1	// Ramp Vdc Rated(1:Complete,:Uncomplete)
		uint32			RampPRated:1	// Ramp P Rated(1:Complete,:Uncomplete)
		uint32			convBlkSeq:2;	// Converter Block Sequence(3:Error, 2:Complete, 1:Processing, 0:Unprocessed)
		uint32			convActchgSeq:2;// Converter Active Charging Sequence(3:Error, 2:Complete, 1:Processing, 0:Unprocessed)
		uint32			passiveChg:1;	// Passive Charge(2:Error, 1:Complete, 0:Uncomplete)
		uint32			convDischg:1;	// Converter Discharge Time Expired(2: Error, 1: Expried, 0:Unready) 
		uint32			noEmy:1;		// No Emergency Switch Active(1:Active, 0:Inactive)
		uint32			tapChgPos:1;	// Tap Changer in Start Position(2:Fault, 1:In-position, 0:Out-position)
		uint32			cpSys:1;		// C&P System Ready(2:Fault, 1:Ready, 0:Unready)
		uint32			convCooling:1;	// Converter Cooling On(2:Fault, 1:Ready, 0:Unready)
		uint32			trCooling:1;	// TR. Cooling On(2:Fault, 1:ON, 0:OFF)
		uint32			smSrtCirSwi:1;	// Submodule Short Circuit Switch Open(2:Fault, 1:Close, 0:Open)	
		uint32			doorIntLock:1;	// Door and Interlocking fulfilled(2:Error, 1:Ready, 0:Unready)	
	} bit;
};

// Yard Switch Status & Command
#define YARD_SWITCH_STATUS_OPEN		0
#define YARD_SWITCH_STATUS_CLOSE	1
#define YARD_SWITCH_STATUS_FAULT	2

#define YARD_SWITCH_COMM_ALL_OPEN	0
#define YARD_SWITCH_COMM_OPEN		0
#define YARD_SWITCH_COMM_CLOSE		1

union UN_YS { 
	struct {
		uint32			rsvd:24;		// Reserved
		uint32			esx12_22:1;		// DC Busbar ES, ES112&122 or ES212&222 Commnad or Status(1:Close, 0:Open), (1:Close, 0:Open)			
		uint32			esx11_21:1;		// Converter Hall ES, ES111&121 or ES211&221 Commnad or Status(1:Close, 0:Open), (1:Close, 0:Open)	
		uint32			esx01_02:1;		// AC ES, ES101&102 or ES201&202 Commnad or Status(1:Close, 0:Open), (1:Close, 0:Open)	
		uint32			ds3x1:1;		// Busbar DS, DS311 or DS321 Commnad or Status(1:Close, 0:Open), (1:Close, 0:Open)	
		uint32			dsx02:1;		// Resistor Bypass Switch, DS102 or DS202 Commnad or Status(:Close, 0:Open), (1:Close, 0:Open)		
		uint32			dsx01:1;		// Zigzag TR DS, DS101 or DS201 Commnad or Status(1:Close, 0:Open), (1:Close, 0:Open)	
		uint32			cbx91:1;		// TR Primary CB, CB191 or CB291 Commnad or Status(1:Close, 0:Open), (1:Close, 0:Open)		
		uint32			cbx01:1;		// TR Secondary CB, CB101 or CB201 Commnad or Status(1:Close, 0:Open), (1:Close, 0:Open)		
	} bit;
};

// Trip Signals
union UN_TRIP
{
	uint32				all;
	struct {
		uint32			rsvd:28;		// Reserved		
		uint32			nmst:1;			// Normal Stop from Protection
		uint32			emst:1;			// Emergency Stop from Protection		
		uint32			conBlk:1;		// Converter Block from Protection	
		uint32			alarm:1;		// Alarm from Protection(1:ON)	
	} bit;
};

/* Inner Variables */
// Start/Stop Sequence 
typedef enum {
    Seq0000     	= 0,        	// Undefined
    SeqBypass		= 1,			// Sequence Bypass(No operation of Sequence in State Machine)
    Step_offset 	= 10,       	// Step Offset
    Dir_offset  	= 100,      	// Direction Offset
    Seq0200     	= 200,      	// Earthed
    Seq0210     	= 210,      	// Check Subsystem Status
    Seq0220     	= 220,      	// Open Converter AC Earthing Switch
    Seq0230     	= 230,      	// Open Converter Hall Earthing Switch
    Seq0240     	= 240,      	// Open DC Busbar Earthing Switch
    Seq0250     	= 250,      	// Close ZigZag Transformer Disconnector
    Seq0260     	= 260,      	// Close Resistor Bypass Switch
    Seq0320     	= 320,      	// Close Converter AC Earthing Switch
    Seq0330     	= 330,      	// Close Converter Hall Earthing Switch
    Seq0340     	= 340,      	// Close DC Busbar Earthing Switch
    Seq0350     	= 350,      	// Open ZigZag Transformer Disconnector
    Seq0360     	= 360,      	// Open Resistor Bypass Switch
    Seq0400     	= 400,      	// Stopped
    Seq0410     	= 410,      	// Close Busbar Disconnector
    Seq0510     	= 510,      	// Open Busbar Disconnector
    Seq0520     	= 520,      	// Converter Discharge Time Expired
    Seq0600     	= 600,      	// Standby
    Seq0610     	= 610,      	// Check Subsystem Status and Interlock Conditons
    Seq0620     	= 620,      	// Open Resistor Bypass Switch
    Seq0630     	= 630,      	// Check Tr. Primary Circuit Breaker Close
    Seq0640     	= 640,      	// Close Tr. Secondary Circuit Breaker
    Seq0650     	= 650,      	// Check Passive Charge Complete
    Seq0660     	= 660,      	// Close Resistor Bypass Switch
    Seq0670     	= 670,      	// Converter Active Charge Sequence
    Seq0720     	= 720,     	 	// Close Resistor Bypass Switch
    Seq0730     	= 730,      	// Check Tr. Primary Circuit Breaker Open
    Seq0740     	= 740,  	    // Open Tr. Secondary Circuit Breaker
    Seq0760     	= 760,	      	// Open Resistor Bypass Switch
    Seq0770     	= 770,      	// Converter Block Sequence
    Seq0800     	= 800,  	    // In-Service or Coupled
} SEQ_STEP;

#define FLAG_SEQ_COMPLETE			1
#define FLAG_SEQ_UNCOMPLETE			0

#define FLAG_SEQ_START				1
#define FLAG_SEQ_STOP				0

/* Output Variables */
#define SEQ_COMM_ALL_RESET			0 
#define SEQ_COMM_ERROR				3
#define SEQ_COMM_COMPLETE			2
#define SEQ_COMM_PROCESSING			1
#define SEQ_COMM_UNPROCESSED		0

// HMI Status Command
union UN_SEQSTS1{
	uint32				all;
	struct {
		uint32			rsvd:2;			// Reserved
		uint32			seq0410:2;		// (3:Error, 2:Complete, 1:Processing, 0:Unprocessed)
		uint32			seq0400:2;		// (3:Error, 2:Complete, 1:Processing, 0:Unprocessed)
		uint32			seq0360:2;		// (3:Error, 2:Complete, 1:Processing, 0:Unprocessed)
		uint32			seq0350:2;		// (3:Error, 2:Complete, 1:Processing, 0:Unprocessed)
		uint32			seq0340:2;		// (3:Error, 2:Complete, 1:Processing, 0:Unprocessed)
		uint32			seq0330:2;		// (3:Error, 2:Complete, 1:Processing, 0:Unprocessed)
		uint32			seq0320:2;		// (3:Error, 2:Complete, 1:Processing, 0:Unprocessed)
		uint32			seq0260:2;		// (3:Error, 2:Complete, 1:Processing, 0:Unprocessed)
		uint32			seq0250:2;		// (3:Error, 2:Complete, 1:Processing, 0:Unprocessed)
		uint32			seq0240:2;		// (3:Error, 2:Complete, 1:Processing, 0:Unprocessed)
		uint32			seq0230:2;		// (3:Error, 2:Complete, 1:Processing, 0:Unprocessed)
		uint32			seq0220:2;		// (3:Error, 2:Complete, 1:Processing, 0:Unprocessed)
		uint32			seq0210:2;		// (3:Error, 2:Complete, 1:Processing, 0:Unprocessed)
		uint32			seq0200:2;		// (3:Error, 2:Complete, 1:Processing, 0:Unprocessed)
		uint32			seq0000:2;		// (3:Error, 2:Complete, 1:Processing, 0:Unprocessed)	
	} bit;
};

union UN_SEQSTS2{
	uint32				all;
	struct {
		uint32			seq0800:2;		// (3:Error, 2:Complete, 1:Processing, 0:Unprocessed)
		uint32			seq0770:2;		// (3:Error, 2:Complete, 1:Processing, 0:Unprocessed)
		uint32			seq0760:2;		// (3:Error, 2:Complete, 1:Processing, 0:Unprocessed)
		uint32			seq0740:2;		// (3:Error, 2:Complete, 1:Processing, 0:Unprocessed)
		uint32			seq0730:2;		// (3:Error, 2:Complete, 1:Processing, 0:Unprocessed)
		uint32			seq0720:2;		// (3:Error, 2:Complete, 1:Processing, 0:Unprocessed)		
		uint32			seq0670:2;		// (3:Error, 2:Complete, 1:Processing, 0:Unprocessed)
		uint32			seq0660:2;		// (3:Error, 2:Complete, 1:Processing, 0:Unprocessed)
		uint32			seq0650:2;		// (3:Error, 2:Complete, 1:Processing, 0:Unprocessed)
		uint32			seq0640:2;		// (3:Error, 2:Complete, 1:Processing, 0:Unprocessed)
		uint32			seq0630:2;		// (3:Error, 2:Complete, 1:Processing, 0:Unprocessed)
		uint32			seq0620:2;		// (3:Error, 2:Complete, 1:Processing, 0:Unprocessed)
		uint32			seq0610:2;		// (3:Error, 2:Complete, 1:Processing, 0:Unprocessed)
		uint32			seq0600:2;		// (3:Error, 2:Complete, 1:Processing, 0:Unprocessed)
		uint32			seq0520:2;		// (3:Error, 2:Complete, 1:Processing, 0:Unprocessed)	
		uint32			seq0510:2;		// (3:Error, 2:Complete, 1:Processing, 0:Unprocessed)	
	} bit;
};


#define ACT_CHARGE_COMM_STOP		0
#define ACT_CHARGE_COMM_START		1
#define ACT_CHARGE_COMM_HOLD		2

#define CONV_BLK_COMM_STOP			0
#define CONV_BLK_COMM_START			1
#define CONV_BLK_COMM_HOLD			2

//FB Variable 
typedef struct {
    // Input
    union UN_SYS_MODE	sysMode;		// Operation Mode
	union UN_SS_STS		sysSts;			// Subsystem Status
	union UN_YS			yardSwiSts;		// Yard Switch Status
	union UN_SEQSTS1	oSeqSts1;		// Other Station Sequence Status1
	union UN_SEQSTS2	oSeqSts2;		// Other StationSequence Status2
	union UN_TRIP		trip;			// Trips from Protection 		
		
    // Inner variable
	SEQ_STEP			PrevStep;		// Previous Step
	SEQ_STEP			CurrStep;		// Current Step
	SEQ_STEP			NextStep;		// Next Step	
	uint32				flagSeqComlete;	// Results of Sequence Process(1:Complete, 0:Uncomplete)
    
    // Output
	union UN_SEQSTS1	seqSts1;		// Sequence Status1
	union UN_SEQSTS2	seqSts2;		// Sequence Status2
    union UN_YS 		yardSwiComm;	// Yard Switch Command
} strFC0740Info;

#endif
