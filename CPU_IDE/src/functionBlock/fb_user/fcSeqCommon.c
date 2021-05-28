#include	<stdio.h>
#include 	<stdlib.h>

#include	"usrTypes.h"
#include	"usrDef.h"
#include	"fbDef.h"

#include	"fb_code\fb_user\fcSwiYardSeq.h"


void funcSeq0000(strFC0740Info *);
void funcSeq0200(strFC0740Info *);
void funcSeq0210(strFC0740Info *);
void funcSeq0220(strFC0740Info *);
void funcSeq0230(strFC0740Info *);
void funcSeq0240(strFC0740Info *);
void funcSeq0250(strFC0740Info *);
void funcSeq0260(strFC0740Info *);
void funcSeq0320(strFC0740Info *);
void funcSeq0330(strFC0740Info *);
void funcSeq0340(strFC0740Info *);
void funcSeq0350(strFC0740Info *);
void funcSeq0360(strFC0740Info *);
void funcSeq0400(strFC0740Info *);
void funcSeq0410(strFC0740Info *);
void funcSeq0510(strFC0740Info *);
void funcSeq0520(strFC0740Info *);
void funcSeq0600(strFC0740Info *);
void funcSeq0610(strFC0740Info *);
void funcSeq0620(strFC0740Info *);
void funcSeq0630(strFC0740Info *);
void funcSeq0640(strFC0740Info *);
void funcSeq0650(strFC0740Info *);
void funcSeq0660(strFC0740Info *);
void funcSeq0670(strFC0740Info *);
void funcSeq0720(strFC0740Info *);
void funcSeq0730(strFC0740Info *);
void funcSeq0740(strFC0740Info *);
void funcSeq0760(strFC0740Info *);
void funcSeq0770(strFC0740Info *);
void funcSeq0800(strFC0740Info *);


void funcSeq0000(strFC0740Info *fb)
{
	fb->NextStep = Seq0200;
	fb->seqSts1.bit.seq0000 = SEQ_COMM_COMPLETE;
	fb->flagSeqComplete = FLAG_SEQ_COMPLETE;
}

void funcSeq0200(strFC0740Info *fb)
{
	if(fb->sysMode.bit.seqDir == SM_SEQDIR_START)
	{
		// When restarting seq. after operating seq at least one time, reset stop direction led light in HMI
		fb->seqSts1.bit.seq0360 = SEQ_COMM_UNPROCESSED;
		fb->seqSts1.bit.seq0350 = SEQ_COMM_UNPROCESSED;
		fb->seqSts1.bit.seq0340 = SEQ_COMM_UNPROCESSED;
		fb->seqSts1.bit.seq0330 = SEQ_COMM_UNPROCESSED;
		fb->seqSts1.bit.seq0320 = SEQ_COMM_UNPROCESSED;	
		fb->seqSts2.bit.seq0770 = SEQ_COMM_UNPROCESSED;
		fb->seqSts2.bit.seq0760 = SEQ_COMM_UNPROCESSED;
		fb->seqSts2.bit.seq0740 = SEQ_COMM_UNPROCESSED;
		fb->seqSts2.bit.seq0730 = SEQ_COMM_UNPROCESSED;
		fb->seqSts2.bit.seq0720 = SEQ_COMM_UNPROCESSED;
		fb->seqSts2.bit.seq0520 = SEQ_COMM_UNPROCESSED;
		fb->seqSts2.bit.seq0510 = SEQ_COMM_UNPROCESSED;
			
		if(((fb->sysMode.bit.actPwr == SM_ACTPWR_P) && (fb->oSeqSts1.all == STS1_EARTHED_STOPPED_COMPLETE))
			||((fb->sysMode.bit.actPwr == SM_ACTPWR_VDC) && (fb->oSeqSts1.all == STS1_UNDEFINED_COMPLETE)) )
		{
			if(fb->t >=SET_TIME_SEQ_DELAY)
			{
				fb->NextStep = Seq0210;
				fb->seqSts1.bit.seq0200 = SEQ_COMM_COMPLETE;
				fb->flagSeqComplete = FLAG_SEQ_COMPLETE;	
				fb->t = 0.0f;			
			}
			else
			{
				fb->NextStep = Seq0200;
				fb->seqSts1.bit.seq0200 = SEQ_COMM_PROCESSING;
				fb->flagSeqComplete = FLAG_SEQ_UNCOMPLETE;
				fb->t += fb->cycleT;			
			}
		}
		else
		{
			fb->NextStep = Seq0200;
			fb->seqSts1.bit.seq0200 = SEQ_COMM_PROCESSING;
			fb->flagSeqComplete = FLAG_SEQ_UNCOMPLETE;
		}
	}
	else if(fb->sysMode.bit.seqDir == SM_SEQDIR_STOP)
	{
		if((fb->sysMode.bit.actPwr == SM_ACTPWR_P) \
			||((fb->sysMode.bit.actPwr == SM_ACTPWR_VDC) && (fb->oSeqSts2.all == STS2_COUPLED_EARTHED_COMPLETE) && (fb->oSeqSts1.all == STS1_COUPLED_EARTHED_COMPLETE))
			||((fb->sysMode.bit.actPwr == SM_ACTPWR_VDC) && (fb->trip.all > TRIP_BAND)))
		{		
			if(fb->t >=SET_TIME_SEQ_DELAY)
			{	
				fb->NextStep = Seq0200;
				fb->seqSts1.bit.seq0200 = SEQ_COMM_UNPROCESSED;
				fb->flagSeqComplete = FLAG_SEQ_UNCOMPLETE;						
			}
			else
			{
				fb->NextStep = Seq0200;
				fb->seqSts1.bit.seq0200 = SEQ_COMM_PROCESSING;
				fb->flagSeqComplete = FLAG_SEQ_UNCOMPLETE;
				fb->t += fb->cycleT;			
			}			
		}
		else
		{
			fb->NextStep = Seq0200;
			fb->seqSts1.bit.seq0200 = SEQ_COMM_PROCESSING;
			fb->flagSeqComplete = FLAG_SEQ_UNCOMPLETE;
			fb->t = 0.0f;	
		}
	}
	else if(fb->sysMode.bit.seqDir == SM_SEQDIR_NODIR)
	{
		fb->NextStep = Seq0200;
		fb->seqSts1.bit.seq0200 = SEQ_COMM_UNPROCESSED;
		fb->flagSeqComplete = FLAG_SEQ_UNCOMPLETE;
	}
	else
	{
		fb->NextStep = Seq0200;
		fb->seqSts1.bit.seq0200 = SEQ_COMM_UNPROCESSED;
		fb->flagSeqComplete = FLAG_SEQ_UNCOMPLETE;
	}
}


void funcSeq0210(strFC0740Info *fb)
{
	if(fb->sysMode.bit.seqDir == SM_SEQDIR_START)
	{
		if(((fb->sysMode.bit.actPwr == SM_ACTPWR_P) && (fb->oSeqSts1.all == STS1_EARTHED_STOPPED_COMPLETE)) \
			||((fb->sysMode.bit.actPwr == SM_ACTPWR_VDC) && (fb->oSeqSts1.all == STS1_UNDEFINED_COMPLETE)))
		{
			if(	(fb->sysSts.bit.doorIntLock == SS_DOORINTLOCK_READY) && \
				(fb->sysSts.bit.smSrtCirSwi == SS_SMSHORTSWI_OPEN) )
			{
				if(fb->t >=SET_TIME_SEQ_DELAY)
				{	
					fb->NextStep = Seq0220;
					fb->seqSts1.bit.seq0210 = SEQ_COMM_COMPLETE;
					fb->flagSeqComplete = FLAG_SEQ_COMPLETE;					
					fb->t = 0.0f;			
				}
				else
				{
					fb->NextStep = Seq0210;
					fb->seqSts1.bit.seq0210 = SEQ_COMM_PROCESSING;
					fb->flagSeqComplete = FLAG_SEQ_UNCOMPLETE;
					fb->t += fb->cycleT;			
				}			
			}
			else
			{
				fb->NextStep = Seq0210;
				fb->seqSts1.bit.seq0210 = SEQ_COMM_PROCESSING;
				fb->flagSeqComplete = FLAG_SEQ_UNCOMPLETE;
			}
		}
		else
		{
			fb->NextStep = Seq0210;
			fb->seqSts1.bit.seq0210 = SEQ_COMM_PROCESSING;
			fb->flagSeqComplete = FLAG_SEQ_UNCOMPLETE;
		}		
	}
	else if(fb->sysMode.bit.seqDir == SM_SEQDIR_STOP)
	{
		fb->NextStep = Seq0200;
		fb->seqSts1.bit.seq0210 = SEQ_COMM_UNPROCESSED;
		fb->flagSeqComplete = FLAG_SEQ_UNCOMPLETE;
	}
	else if(fb->sysMode.bit.seqDir == SM_SEQDIR_NODIR)
	{
		fb->NextStep = Seq0210;
		fb->seqSts1.bit.seq0210 = SEQ_COMM_UNPROCESSED;
		fb->flagSeqComplete = FLAG_SEQ_UNCOMPLETE;	
	}
	else
	{
		fb->NextStep = Seq0210;
		fb->seqSts1.bit.seq0210 = SEQ_COMM_UNPROCESSED;
		fb->flagSeqComplete = FLAG_SEQ_UNCOMPLETE;
	}
}

void funcSeq0220(strFC0740Info *fb)
{
	if(fb->sysMode.bit.seqDir == SM_SEQDIR_START)
	{
		if(((fb->sysMode.bit.actPwr == SM_ACTPWR_P) && (fb->oSeqSts1.all == STS1_EARTHED_STOPPED_COMPLETE)) \
			||((fb->sysMode.bit.actPwr == SM_ACTPWR_VDC) && (fb->oSeqSts1.all == STS1_UNDEFINED_COMPLETE)))
		{
			fb->yardSwiComm.bit.esx01_02 = YARD_SWITCH_COMM_OPEN;

			if((fb->yardSwiSts.bit.esx01_02 == YARD_SWITCH_STATUS_OPEN))
			{
				if(fb->t >=SET_TIME_SEQ_DELAY)
				{	
					fb->NextStep = Seq0230;
					fb->seqSts1.bit.seq0220 = SEQ_COMM_COMPLETE;
					fb->flagSeqComplete = FLAG_SEQ_COMPLETE;
					fb->seqSts1.bit.seq0320 = SEQ_COMM_UNPROCESSED;
					fb->t = 0.0f;			
				}
				else
				{
					fb->NextStep = Seq0220;
					fb->seqSts1.bit.seq0220 = SEQ_COMM_PROCESSING;
					fb->flagSeqComplete = FLAG_SEQ_UNCOMPLETE;
					fb->t += fb->cycleT;			
				}
			}
			else
			{
				fb->NextStep = Seq0220;
				fb->seqSts1.bit.seq0220 = SEQ_COMM_PROCESSING;
				fb->flagSeqComplete = FLAG_SEQ_UNCOMPLETE;
			}
		}
		else
		{
			fb->NextStep = Seq0220;
			fb->seqSts1.bit.seq0220 = SEQ_COMM_PROCESSING;
			fb->flagSeqComplete = FLAG_SEQ_UNCOMPLETE;
		}
	}
	else if(fb->sysMode.bit.seqDir == SM_SEQDIR_STOP)
	{
		fb->NextStep = Seq0320;
		fb->seqSts1.bit.seq0220 = SEQ_COMM_UNPROCESSED;
		fb->flagSeqComplete = FLAG_SEQ_UNCOMPLETE;
	}
	else if(fb->sysMode.bit.seqDir == SM_SEQDIR_NODIR)
	{
		fb->NextStep = Seq0220;
		fb->seqSts1.bit.seq0220 = SEQ_COMM_UNPROCESSED;	
		fb->flagSeqComplete = FLAG_SEQ_UNCOMPLETE;
	}
	else
	{
		fb->NextStep = Seq0220;
		fb->seqSts1.bit.seq0220 = SEQ_COMM_UNPROCESSED;	
		fb->flagSeqComplete = FLAG_SEQ_UNCOMPLETE;
	}
}

void funcSeq0230(strFC0740Info *fb)
{
	if(fb->sysMode.bit.seqDir == SM_SEQDIR_START)
	{
		if(((fb->sysMode.bit.actPwr == SM_ACTPWR_P) && (fb->oSeqSts1.all == STS1_EARTHED_STOPPED_COMPLETE)) \
			||((fb->sysMode.bit.actPwr == SM_ACTPWR_VDC) && (fb->oSeqSts1.all == STS1_UNDEFINED_COMPLETE)))
		{
			fb->yardSwiComm.bit.esx11_21 = YARD_SWITCH_COMM_OPEN;

			if((fb->yardSwiSts.bit.esx11_21 == YARD_SWITCH_STATUS_OPEN))
			{
				if(fb->t >=SET_TIME_SEQ_DELAY)
				{	
					fb->NextStep = Seq0240;
					fb->seqSts1.bit.seq0230 = SEQ_COMM_COMPLETE;
					fb->flagSeqComplete = FLAG_SEQ_COMPLETE;
					fb->seqSts1.bit.seq0330 = SEQ_COMM_UNPROCESSED;
					fb->t = 0.0f;			
				}
				else
				{
					fb->NextStep = Seq0230;
					fb->seqSts1.bit.seq0230 = SEQ_COMM_PROCESSING;
					fb->flagSeqComplete = FLAG_SEQ_UNCOMPLETE;
					fb->t += fb->cycleT;			
				}				
			}
			else
			{
				fb->NextStep = Seq0230;
				fb->seqSts1.bit.seq0230 = SEQ_COMM_PROCESSING;
				fb->flagSeqComplete = FLAG_SEQ_UNCOMPLETE;
			}
		}
		else
		{
			fb->NextStep = Seq0230;
			fb->seqSts1.bit.seq0230 = SEQ_COMM_PROCESSING;
			fb->flagSeqComplete = FLAG_SEQ_UNCOMPLETE;
		}
	}
	else if(fb->sysMode.bit.seqDir == SM_SEQDIR_STOP)
	{
		fb->NextStep = Seq0330;
		fb->seqSts1.bit.seq0230 = SEQ_COMM_UNPROCESSED;
		fb->flagSeqComplete = FLAG_SEQ_UNCOMPLETE;
	}
	else if(fb->sysMode.bit.seqDir == SM_SEQDIR_NODIR)
	{
		fb->NextStep = Seq0230;
		fb->seqSts1.bit.seq0230 = SEQ_COMM_UNPROCESSED;
		fb->flagSeqComplete = FLAG_SEQ_UNCOMPLETE;
	}
	else
	{
		fb->NextStep = Seq0230;
		fb->seqSts1.bit.seq0230 = SEQ_COMM_UNPROCESSED;
		fb->flagSeqComplete = FLAG_SEQ_UNCOMPLETE;
	}	
}


void funcSeq0240(strFC0740Info *fb)
{
	if(fb->sysMode.bit.seqDir == SM_SEQDIR_START)
	{
		if(((fb->sysMode.bit.actPwr == SM_ACTPWR_P) && (fb->oSeqSts1.all == STS1_EARTHED_STOPPED_COMPLETE)) \
			||((fb->sysMode.bit.actPwr == SM_ACTPWR_VDC) && (fb->oSeqSts1.all == STS1_UNDEFINED_COMPLETE)))
		{
			fb->yardSwiComm.bit.esx12_22 = YARD_SWITCH_COMM_OPEN;

			if((fb->yardSwiSts.bit.esx12_22 == YARD_SWITCH_STATUS_OPEN))
			{
				if(fb->t >=SET_TIME_SEQ_DELAY)
				{	
					fb->NextStep = Seq0250;
					fb->seqSts1.bit.seq0240 = SEQ_COMM_COMPLETE;
					fb->flagSeqComplete = FLAG_SEQ_COMPLETE;
					fb->seqSts1.bit.seq0340 = SEQ_COMM_UNPROCESSED;
					fb->t = 0.0f;			
				}
				else
				{
					fb->NextStep = Seq0240;
					fb->seqSts1.bit.seq0240 = SEQ_COMM_PROCESSING;
					fb->flagSeqComplete = FLAG_SEQ_UNCOMPLETE;
					fb->t += fb->cycleT;			
				}
			}
			else
			{
				fb->NextStep = Seq0240;
				fb->seqSts1.bit.seq0240 = SEQ_COMM_PROCESSING;
				fb->flagSeqComplete = FLAG_SEQ_UNCOMPLETE;
			}
		}
		else
		{
			fb->NextStep = Seq0240;
			fb->seqSts1.bit.seq0240 = SEQ_COMM_PROCESSING;
			fb->flagSeqComplete = FLAG_SEQ_UNCOMPLETE;
		}		
	}
	else if(fb->sysMode.bit.seqDir == SM_SEQDIR_STOP)
	{
		fb->NextStep = Seq0340;
		fb->seqSts1.bit.seq0240 = SEQ_COMM_UNPROCESSED;
		fb->flagSeqComplete = FLAG_SEQ_UNCOMPLETE;
	}
	else if(fb->sysMode.bit.seqDir == SM_SEQDIR_NODIR)
	{
		fb->NextStep = Seq0240;
		fb->seqSts1.bit.seq0240 = SEQ_COMM_UNPROCESSED;
		fb->flagSeqComplete = FLAG_SEQ_UNCOMPLETE;
	}
	else
	{
		fb->NextStep = Seq0240;
		fb->seqSts1.bit.seq0240 = SEQ_COMM_UNPROCESSED;
		fb->flagSeqComplete = FLAG_SEQ_UNCOMPLETE;
	}		
}

void funcSeq0250(strFC0740Info *fb)
{
	if(fb->sysMode.bit.seqDir == SM_SEQDIR_START)
	{
		//Zig-Zag TR contributes to the system in Vdc mode
		if((fb->sysMode.bit.actPwr == SM_ACTPWR_P) && (fb->oSeqSts1.all == STS1_EARTHED_STOPPED_COMPLETE))
		{
			if(fb->t >=SET_TIME_SEQ_DELAY)
			{	
				fb->NextStep = Seq0260;
				fb->seqSts1.bit.seq0250 = SEQ_COMM_COMPLETE;
				fb->flagSeqComplete = FLAG_SEQ_COMPLETE;
				fb->seqSts1.bit.seq0350 = SEQ_COMM_UNPROCESSED;					
				fb->t = 0.0f;			
			}
			else
			{
				fb->NextStep = Seq0250;			
				fb->seqSts1.bit.seq0250 = SEQ_COMM_PROCESSING;
				fb->flagSeqComplete = FLAG_SEQ_UNCOMPLETE;
				fb->t += fb->cycleT;			
			}			
		}
		else if((fb->sysMode.bit.actPwr == SM_ACTPWR_VDC) && (fb->oSeqSts1.all == STS1_UNDEFINED_COMPLETE)
		     ||((fb->sysMode.bit.actPwr == SM_ACTPWR_VDC) && (fb->trip.all > TRIP_BAND) && (fb->oSeqSts1.all == TRIP_S1_UNDEFINED_COMPLETE)))
		{
			fb->yardSwiComm.bit.dsx02 = YARD_SWITCH_COMM_CLOSE;

			if((fb->yardSwiSts.bit.dsx02 == YARD_SWITCH_STATUS_CLOSE))
			{
				if(fb->t >=SET_TIME_SEQ_DELAY)
				{	
					fb->NextStep = Seq0260;
					fb->seqSts1.bit.seq0250 = SEQ_COMM_COMPLETE;
					fb->flagSeqComplete = FLAG_SEQ_COMPLETE;
					fb->seqSts1.bit.seq0350 = SEQ_COMM_UNPROCESSED;					
					fb->t = 0.0f;			
				}
				else
				{
					fb->NextStep = Seq0250;			
					fb->seqSts1.bit.seq0250 = SEQ_COMM_PROCESSING;
					fb->flagSeqComplete = FLAG_SEQ_UNCOMPLETE;
					fb->t += fb->cycleT;			
				}
			}
			else
			{
				fb->NextStep = Seq0250;			
				fb->seqSts1.bit.seq0250 = SEQ_COMM_PROCESSING;
				fb->flagSeqComplete = FLAG_SEQ_UNCOMPLETE;
			}			
		}
		else
		{
			fb->NextStep = Seq0250;			
			fb->seqSts1.bit.seq0250 = SEQ_COMM_PROCESSING;
			fb->flagSeqComplete = FLAG_SEQ_UNCOMPLETE;
		}		
	}
	else if(fb->sysMode.bit.seqDir == SM_SEQDIR_STOP)
	{
		fb->NextStep = Seq0350;
		fb->seqSts1.bit.seq0250 = SEQ_COMM_UNPROCESSED;
		fb->flagSeqComplete = FLAG_SEQ_UNCOMPLETE;
	}
	else if(fb->sysMode.bit.seqDir == SM_SEQDIR_NODIR)
	{
		fb->NextStep = Seq0250;
		fb->seqSts1.bit.seq0250 = SEQ_COMM_UNPROCESSED;
		fb->flagSeqComplete = FLAG_SEQ_UNCOMPLETE;
	}
	else
	{
		fb->NextStep = Seq0250;
		fb->seqSts1.bit.seq0250 = SEQ_COMM_UNPROCESSED;
		fb->flagSeqComplete = FLAG_SEQ_UNCOMPLETE;
	}	
}
void funcSeq0260(strFC0740Info *fb)
{
	if(fb->sysMode.bit.seqDir == SM_SEQDIR_START)
	{
		if(((fb->sysMode.bit.actPwr == SM_ACTPWR_P) && (fb->oSeqSts1.all == STS1_EARTHED_STOPPED_COMPLETE)) \
			||((fb->sysMode.bit.actPwr == SM_ACTPWR_VDC) && (fb->oSeqSts1.all == STS1_UNDEFINED_COMPLETE)))
		{
			fb->yardSwiComm.bit.dsx01 = YARD_SWITCH_COMM_CLOSE;

			if((fb->yardSwiSts.bit.dsx01 == YARD_SWITCH_STATUS_CLOSE))
			{
				if(fb->t >=SET_TIME_SEQ_DELAY)
				{	
					fb->NextStep = Seq0400;
					fb->seqSts1.bit.seq0260 = SEQ_COMM_COMPLETE;
					fb->flagSeqComplete = FLAG_SEQ_COMPLETE;
					fb->seqSts1.bit.seq0360 = SEQ_COMM_UNPROCESSED;
					fb->t = 0.0f;			
				}
				else
				{
					fb->NextStep = Seq0260;
					fb->seqSts1.bit.seq0260 = SEQ_COMM_PROCESSING;
					fb->flagSeqComplete = FLAG_SEQ_UNCOMPLETE;
					fb->t += fb->cycleT;			
				}
			}
			else
			{
				fb->NextStep = Seq0260;
				fb->seqSts1.bit.seq0260 = SEQ_COMM_PROCESSING;
				fb->flagSeqComplete = FLAG_SEQ_UNCOMPLETE;
			}
		}
		else
		{
			fb->NextStep = Seq0260;
			fb->seqSts1.bit.seq0260 = SEQ_COMM_PROCESSING;
			fb->flagSeqComplete = FLAG_SEQ_UNCOMPLETE;
		}		

	}
	else if(fb->sysMode.bit.seqDir == SM_SEQDIR_STOP)
	{
		fb->NextStep = Seq0360;
		fb->seqSts1.bit.seq0260 = SEQ_COMM_UNPROCESSED;
		fb->flagSeqComplete = FLAG_SEQ_UNCOMPLETE;
	}
	else if(fb->sysMode.bit.seqDir == SM_SEQDIR_NODIR)
	{
		fb->NextStep = Seq0260;
		fb->seqSts1.bit.seq0260 = SEQ_COMM_UNPROCESSED;
		fb->flagSeqComplete = FLAG_SEQ_UNCOMPLETE;
	}
	else
	{
		fb->NextStep = Seq0260;
		fb->seqSts1.bit.seq0260 = SEQ_COMM_UNPROCESSED;
		fb->flagSeqComplete = FLAG_SEQ_UNCOMPLETE;
	}	
}

void funcSeq0320(strFC0740Info *fb)
{
	if(fb->sysMode.bit.seqDir == SM_SEQDIR_START)
	{
		fb->NextStep = Seq0320;
		fb->seqSts1.bit.seq0320 = SEQ_COMM_UNPROCESSED;
		fb->flagSeqComplete = FLAG_SEQ_UNCOMPLETE;
	}
	else if(fb->sysMode.bit.seqDir == SM_SEQDIR_STOP)
	{
		if(((fb->sysMode.bit.actPwr == SM_ACTPWR_P) && (fb->oSeqSts2.all == STS2_COUPLED_STOPPED_COMPLETE) && (fb->oSeqSts1.all == STS1_COUPLED_STOPPED_COMPLETE))
			||((fb->sysMode.bit.actPwr == SM_ACTPWR_P) && (fb->trip.all > TRIP_BAND) && (fb->oSeqSts2.bit.seq0510 == SEQ_COMM_PROCESSING))
			||((fb->sysMode.bit.actPwr == SM_ACTPWR_VDC) && (fb->oSeqSts2.all == STS2_COUPLED_EARTHED_COMPLETE) && (fb->oSeqSts1.all == STS1_COUPLED_EARTHED_COMPLETE))
			||((fb->sysMode.bit.actPwr == SM_ACTPWR_VDC) && (fb->trip.all > TRIP_BAND)))
		{
			fb->yardSwiComm.bit.esx01_02 = YARD_SWITCH_COMM_CLOSE;

			if((fb->yardSwiSts.bit.esx01_02 == YARD_SWITCH_STATUS_CLOSE))
			{
				if(fb->t >=SET_TIME_SEQ_DELAY)
				{	
					fb->NextStep = Seq0200;
					fb->seqSts1.bit.seq0320 = SEQ_COMM_COMPLETE;
					fb->flagSeqComplete = FLAG_SEQ_COMPLETE;
					fb->seqSts1.bit.seq0220 = SEQ_COMM_UNPROCESSED;
					fb->seqSts1.bit.seq0210 = SEQ_COMM_UNPROCESSED;					
					fb->t = 0.0f;			
				}
				else
				{
					fb->NextStep = Seq0320;
					fb->seqSts1.bit.seq0320 = SEQ_COMM_PROCESSING;
					fb->flagSeqComplete = FLAG_SEQ_UNCOMPLETE;
					fb->t += fb->cycleT;			
				}				
			}
			else
			{
				fb->NextStep = Seq0320;
				fb->seqSts1.bit.seq0320 = SEQ_COMM_PROCESSING;
				fb->flagSeqComplete = FLAG_SEQ_UNCOMPLETE;
			}
		}
		else
		{
			fb->NextStep = Seq0320;
			fb->seqSts1.bit.seq0320 = SEQ_COMM_PROCESSING;
			fb->flagSeqComplete = FLAG_SEQ_UNCOMPLETE;
		}		
	}
	else if(fb->sysMode.bit.seqDir == SM_SEQDIR_NODIR)
	{
		fb->NextStep = Seq0320;
		fb->seqSts1.bit.seq0320 = SEQ_COMM_UNPROCESSED;
		fb->flagSeqComplete = FLAG_SEQ_UNCOMPLETE;
	}
	else
	{
		fb->NextStep = Seq0320;
		fb->seqSts1.bit.seq0320 = SEQ_COMM_UNPROCESSED;
		fb->flagSeqComplete = FLAG_SEQ_UNCOMPLETE;
	}		
}

void funcSeq0330(strFC0740Info *fb)
{
	if(fb->sysMode.bit.seqDir == SM_SEQDIR_START)
	{
		fb->NextStep = Seq0330;
		fb->seqSts1.bit.seq0330 = SEQ_COMM_UNPROCESSED;
		fb->flagSeqComplete = FLAG_SEQ_UNCOMPLETE;
	}
	else if(fb->sysMode.bit.seqDir == SM_SEQDIR_STOP)
	{
		if(((fb->sysMode.bit.actPwr == SM_ACTPWR_P) && (fb->oSeqSts2.all == STS2_COUPLED_STOPPED_COMPLETE) && (fb->oSeqSts1.all == STS1_COUPLED_STOPPED_COMPLETE))
			||((fb->sysMode.bit.actPwr == SM_ACTPWR_P) && (fb->trip.all > TRIP_BAND) && (fb->oSeqSts2.bit.seq0510 == SEQ_COMM_PROCESSING))
			||((fb->sysMode.bit.actPwr == SM_ACTPWR_VDC) && (fb->oSeqSts2.all == STS2_COUPLED_EARTHED_COMPLETE) && (fb->oSeqSts1.all == STS1_COUPLED_EARTHED_COMPLETE))
			||((fb->sysMode.bit.actPwr == SM_ACTPWR_VDC) && (fb->trip.all > TRIP_BAND)))
		{
			fb->yardSwiComm.bit.esx11_21 = YARD_SWITCH_COMM_CLOSE;

			if((fb->yardSwiSts.bit.esx11_21 == YARD_SWITCH_STATUS_CLOSE))
			{
				if(fb->t >=SET_TIME_SEQ_DELAY)
				{	
					fb->NextStep = Seq0320;
					fb->seqSts1.bit.seq0330 = SEQ_COMM_COMPLETE;
					fb->flagSeqComplete = FLAG_SEQ_COMPLETE;
					fb->seqSts1.bit.seq0230 = SEQ_COMM_UNPROCESSED;					
					fb->t = 0.0f;			
				}
				else
				{
					fb->NextStep = Seq0330;
					fb->seqSts1.bit.seq0330 = SEQ_COMM_PROCESSING;
					fb->flagSeqComplete = FLAG_SEQ_UNCOMPLETE;
					fb->t += fb->cycleT;			
				}				
			}
			else
			{
				fb->NextStep = Seq0330;
				fb->seqSts1.bit.seq0330 = SEQ_COMM_PROCESSING;
				fb->flagSeqComplete = FLAG_SEQ_UNCOMPLETE;
			}
		}
		else
		{
			fb->NextStep = Seq0330;
			fb->seqSts1.bit.seq0330 = SEQ_COMM_PROCESSING;
			fb->flagSeqComplete = FLAG_SEQ_UNCOMPLETE;
		}
	}
	else if(fb->sysMode.bit.seqDir == SM_SEQDIR_NODIR)
	{
		fb->NextStep = Seq0330;
		fb->seqSts1.bit.seq0330 = SEQ_COMM_UNPROCESSED;
		fb->flagSeqComplete = FLAG_SEQ_UNCOMPLETE;
	}
	else
	{
		fb->NextStep = Seq0330;
		fb->seqSts1.bit.seq0330 = SEQ_COMM_UNPROCESSED;
		fb->flagSeqComplete = FLAG_SEQ_UNCOMPLETE;
	}	
}

void funcSeq0340(strFC0740Info *fb)
{
	if(fb->sysMode.bit.seqDir == SM_SEQDIR_START)
	{
		fb->NextStep = Seq0340;
		fb->seqSts1.bit.seq0340 = SEQ_COMM_UNPROCESSED;
		fb->flagSeqComplete = FLAG_SEQ_UNCOMPLETE;
	}
	else if(fb->sysMode.bit.seqDir == SM_SEQDIR_STOP)
	{
		if(((fb->sysMode.bit.actPwr == SM_ACTPWR_P) && (fb->oSeqSts2.all == STS2_COUPLED_STOPPED_COMPLETE) && (fb->oSeqSts1.all == STS1_COUPLED_STOPPED_COMPLETE))
			||((fb->sysMode.bit.actPwr == SM_ACTPWR_P) && (fb->trip.all > TRIP_BAND) && (fb->oSeqSts2.bit.seq0510 == SEQ_COMM_PROCESSING))
			||((fb->sysMode.bit.actPwr == SM_ACTPWR_VDC) && (fb->oSeqSts2.all == STS2_COUPLED_EARTHED_COMPLETE) && (fb->oSeqSts1.all == STS1_COUPLED_EARTHED_COMPLETE))
			||((fb->sysMode.bit.actPwr == SM_ACTPWR_VDC) && (fb->trip.all > TRIP_BAND)))
		{
			fb->yardSwiComm.bit.esx12_22 = YARD_SWITCH_COMM_CLOSE;

			if((fb->yardSwiSts.bit.esx12_22 == YARD_SWITCH_STATUS_CLOSE))
			{
				if(fb->t >=SET_TIME_SEQ_DELAY)
				{	
					fb->NextStep = Seq0330;
					fb->seqSts1.bit.seq0340 = SEQ_COMM_COMPLETE;
					fb->flagSeqComplete = FLAG_SEQ_COMPLETE;
					fb->seqSts1.bit.seq0240 = SEQ_COMM_UNPROCESSED;					
					fb->t = 0.0f;			
				}
				else
				{
					fb->NextStep = Seq0340;
					fb->seqSts1.bit.seq0340 = SEQ_COMM_PROCESSING;
					fb->flagSeqComplete = FLAG_SEQ_UNCOMPLETE;
					fb->t += fb->cycleT;			
				}				
			}
			else
			{
				fb->NextStep = Seq0340;
				fb->seqSts1.bit.seq0340 = SEQ_COMM_PROCESSING;
				fb->flagSeqComplete = FLAG_SEQ_UNCOMPLETE;
			}
		}
		else
		{
			fb->NextStep = Seq0340;
			fb->seqSts1.bit.seq0340 = SEQ_COMM_PROCESSING;
			fb->flagSeqComplete = FLAG_SEQ_UNCOMPLETE;
		}		
	}
	else if(fb->sysMode.bit.seqDir == SM_SEQDIR_NODIR)
	{
		fb->NextStep = Seq0340;
		fb->seqSts1.bit.seq0340 = SEQ_COMM_UNPROCESSED;
		fb->flagSeqComplete = FLAG_SEQ_UNCOMPLETE;
	}
	else
	{
		fb->NextStep = Seq0340;
		fb->seqSts1.bit.seq0340 = SEQ_COMM_UNPROCESSED;
		fb->flagSeqComplete = FLAG_SEQ_UNCOMPLETE;
	}	
}

void funcSeq0350(strFC0740Info *fb)
{
	if(fb->sysMode.bit.seqDir == SM_SEQDIR_START)
	{
		fb->NextStep = Seq0350;
		fb->seqSts1.bit.seq0350 = SEQ_COMM_UNPROCESSED;
		fb->flagSeqComplete = FLAG_SEQ_UNCOMPLETE;
	}
	else if(fb->sysMode.bit.seqDir == SM_SEQDIR_STOP)
	{	
		//Zig-Zag TR contributes to the system in Vdc mode		
		if((fb->sysMode.bit.actPwr == SM_ACTPWR_P) && (fb->oSeqSts2.all == STS2_COUPLED_STOPPED_COMPLETE) && (fb->oSeqSts1.all == STS1_COUPLED_STOPPED_COMPLETE)
		||((fb->sysMode.bit.actPwr == SM_ACTPWR_P) && (fb->trip.all > TRIP_BAND) && (fb->oSeqSts2.bit.seq0510 == SEQ_COMM_PROCESSING)))
		{
			if(fb->t >=SET_TIME_SEQ_DELAY)
			{	
				fb->NextStep = Seq0340;
				fb->seqSts1.bit.seq0350 = SEQ_COMM_COMPLETE;
				fb->flagSeqComplete = FLAG_SEQ_COMPLETE;
				fb->seqSts1.bit.seq0250 = SEQ_COMM_UNPROCESSED;				
				fb->t = 0.0f;			
			}
			else
			{
				fb->NextStep = Seq0350;
				fb->seqSts1.bit.seq0350 = SEQ_COMM_PROCESSING;
				fb->flagSeqComplete = FLAG_SEQ_UNCOMPLETE;
				fb->t += fb->cycleT;			
			}			
		}
		else if((fb->sysMode.bit.actPwr == SM_ACTPWR_VDC) && (fb->oSeqSts2.all == STS2_COUPLED_EARTHED_COMPLETE) && (fb->oSeqSts1.all == STS1_COUPLED_EARTHED_COMPLETE)
			 ||((fb->sysMode.bit.actPwr == SM_ACTPWR_VDC) && (fb->trip.all > TRIP_BAND)))
		{
			fb->yardSwiComm.bit.dsx02 = YARD_SWITCH_COMM_OPEN;

			if((fb->yardSwiSts.bit.dsx02 == YARD_SWITCH_STATUS_OPEN))
			{
				if(fb->t >=SET_TIME_SEQ_DELAY)
				{	
					fb->NextStep = Seq0340;
					fb->seqSts1.bit.seq0350 = SEQ_COMM_COMPLETE;
					fb->flagSeqComplete = FLAG_SEQ_COMPLETE;
					fb->seqSts1.bit.seq0250 = SEQ_COMM_UNPROCESSED;					
					fb->t = 0.0f;			
				}
				else
				{
					fb->NextStep = Seq0350;
					fb->seqSts1.bit.seq0350 = SEQ_COMM_PROCESSING;
					fb->flagSeqComplete = FLAG_SEQ_UNCOMPLETE;
					fb->t += fb->cycleT;			
				}				
			}
			else
			{
				fb->NextStep = Seq0350;
				fb->seqSts1.bit.seq0350 = SEQ_COMM_PROCESSING;
				fb->flagSeqComplete = FLAG_SEQ_UNCOMPLETE;
			}
		}
		else
		{
			fb->NextStep = Seq0350;
			fb->seqSts1.bit.seq0350 = SEQ_COMM_PROCESSING;
			fb->flagSeqComplete = FLAG_SEQ_UNCOMPLETE;
		}
	}
	else if(fb->sysMode.bit.seqDir == SM_SEQDIR_NODIR)
	{
		fb->NextStep = Seq0350;
		fb->seqSts1.bit.seq0350 = SEQ_COMM_UNPROCESSED;
		fb->flagSeqComplete = FLAG_SEQ_UNCOMPLETE;
	}
	else
	{
		fb->NextStep = Seq0350;
		fb->seqSts1.bit.seq0350 = SEQ_COMM_UNPROCESSED;
		fb->flagSeqComplete = FLAG_SEQ_UNCOMPLETE;
	}	
}

void funcSeq0360(strFC0740Info *fb)
{
	if(fb->sysMode.bit.seqDir == SM_SEQDIR_START)
	{
		fb->NextStep = Seq0360;
		fb->seqSts1.bit.seq0360 = SEQ_COMM_UNPROCESSED;
		fb->flagSeqComplete = FLAG_SEQ_UNCOMPLETE;
	}
	else if(fb->sysMode.bit.seqDir == SM_SEQDIR_STOP)
	{
		if(((fb->sysMode.bit.actPwr == SM_ACTPWR_P) && (fb->oSeqSts2.all == STS2_COUPLED_STOPPED_COMPLETE) && (fb->oSeqSts1.all == STS1_COUPLED_STOPPED_COMPLETE))
			||((fb->sysMode.bit.actPwr == SM_ACTPWR_P) && (fb->trip.all > TRIP_BAND) && (fb->oSeqSts2.bit.seq0510 == SEQ_COMM_PROCESSING))
			||((fb->sysMode.bit.actPwr == SM_ACTPWR_VDC) && (fb->oSeqSts2.all == STS2_COUPLED_EARTHED_COMPLETE) && (fb->oSeqSts1.all == STS1_COUPLED_EARTHED_COMPLETE))
			||((fb->sysMode.bit.actPwr == SM_ACTPWR_VDC) && (fb->trip.all > TRIP_BAND)))
		{
			fb->yardSwiComm.bit.dsx01 = YARD_SWITCH_COMM_OPEN;

			if((fb->yardSwiSts.bit.dsx01 == YARD_SWITCH_STATUS_OPEN))
			{
				if(fb->t >=SET_TIME_SEQ_DELAY)
				{
					fb->NextStep = Seq0350;
					fb->seqSts1.bit.seq0360 = SEQ_COMM_COMPLETE;
					fb->flagSeqComplete = FLAG_SEQ_COMPLETE;
					fb->seqSts1.bit.seq0260 = SEQ_COMM_UNPROCESSED;						
					fb->t = 0.0f;			
				}
				else
				{
					fb->NextStep = Seq0360;
					fb->seqSts1.bit.seq0360 = SEQ_COMM_PROCESSING;
					fb->flagSeqComplete = FLAG_SEQ_UNCOMPLETE;
					fb->t += fb->cycleT;			
				}
			}
			else
			{
				fb->NextStep = Seq0360;
				fb->seqSts1.bit.seq0360 = SEQ_COMM_PROCESSING;
				fb->flagSeqComplete = FLAG_SEQ_UNCOMPLETE;
			}
		}
		else
		{
			fb->NextStep = Seq0360;
			fb->seqSts1.bit.seq0360 = SEQ_COMM_PROCESSING;
			fb->flagSeqComplete = FLAG_SEQ_UNCOMPLETE;
		}		
	}
	else if(fb->sysMode.bit.seqDir == SM_SEQDIR_NODIR)
	{
		fb->NextStep = Seq0360;
		fb->seqSts1.bit.seq0360 = SEQ_COMM_UNPROCESSED;
		fb->flagSeqComplete = FLAG_SEQ_UNCOMPLETE;
	}
	else
	{
		fb->NextStep = Seq0360;
		fb->seqSts1.bit.seq0360 = SEQ_COMM_UNPROCESSED;
		fb->flagSeqComplete = FLAG_SEQ_UNCOMPLETE;
	}	
}

void funcSeq0400(strFC0740Info *fb)
{
	if(fb->sysMode.bit.seqDir == SM_SEQDIR_START)
	{
		if(((fb->sysMode.bit.actPwr == SM_ACTPWR_P) && (fb->oSeqSts1.all == STS1_EARTHED_STOPPED_COMPLETE)) \
			||((fb->sysMode.bit.actPwr == SM_ACTPWR_VDC) && (fb->oSeqSts1.all == STS1_UNDEFINED_COMPLETE)) )
		{
			if(fb->t >=SET_TIME_SEQ_DELAY)
			{	
				fb->NextStep = Seq0410;
				fb->seqSts1.bit.seq0400 = SEQ_COMM_COMPLETE;
				fb->flagSeqComplete = FLAG_SEQ_COMPLETE;
				fb->t = 0.0f;			
			}
			else
			{
				fb->NextStep = Seq0400;
				fb->seqSts1.bit.seq0400 = SEQ_COMM_PROCESSING;
				fb->flagSeqComplete = FLAG_SEQ_COMPLETE;
				fb->t += fb->cycleT;			
			}			
		}
		else
		{
			fb->NextStep = Seq0400;
			fb->seqSts1.bit.seq0400 = SEQ_COMM_PROCESSING;
			fb->flagSeqComplete = FLAG_SEQ_COMPLETE;			
		}	
	}
	else if(fb->sysMode.bit.seqDir == SM_SEQDIR_STOP)
	{
		if(((fb->sysMode.bit.actPwr == SM_ACTPWR_P) && (fb->oSeqSts2.all == STS2_COUPLED_STANDBY_COMPLETE)) \
			||((fb->sysMode.bit.actPwr == SM_ACTPWR_VDC) && (fb->oSeqSts2.all == STS2_COUPLED_STOPPED_COMPLETE) && (fb->oSeqSts1.all == STS1_COUPLED_STOPPED_COMPLETE))
			||((fb->sysMode.bit.actPwr == SM_ACTPWR_VDC) && (fb->trip.all > TRIP_BAND) && (fb->oSeqSts1.all == TRIP_S1_UNDEFINED_COMPLETE)))
		{
			if(fb->t >=SET_TIME_SEQ_DELAY)
			{	
				fb->NextStep = Seq0360;
				fb->seqSts1.bit.seq0400 = SEQ_COMM_UNPROCESSED;
				fb->flagSeqComplete = FLAG_SEQ_UNCOMPLETE;				
				fb->t = 0.0f;			
			}
			else
			{
				fb->NextStep = Seq0400;
				fb->seqSts1.bit.seq0400 = SEQ_COMM_PROCESSING;
				fb->flagSeqComplete = FLAG_SEQ_COMPLETE;	
				fb->t += fb->cycleT;			
			}			
		}
		else
		{
			fb->NextStep = Seq0400;
			fb->seqSts1.bit.seq0400 = SEQ_COMM_PROCESSING;
			fb->flagSeqComplete = FLAG_SEQ_COMPLETE;			
		}			

	}
	else if(fb->sysMode.bit.seqDir == SM_SEQDIR_NODIR)
	{
		fb->NextStep = Seq0400;
		fb->seqSts1.bit.seq0400 = SEQ_COMM_UNPROCESSED;
		fb->flagSeqComplete = FLAG_SEQ_UNCOMPLETE;
	}
	else
	{
		fb->NextStep = Seq0400;
		fb->seqSts1.bit.seq0400 = SEQ_COMM_UNPROCESSED;
		fb->flagSeqComplete = FLAG_SEQ_UNCOMPLETE;
	}
}

void funcSeq0410(strFC0740Info *fb)
{
	if(fb->sysMode.bit.seqDir == SM_SEQDIR_START)
	{
		if((fb->sysMode.bit.actPwr == SM_ACTPWR_P) && (fb->oSeqSts1.all == STS1_EARTHED_STANDBY_COMPLETE) && (fb->oSeqSts2.all == STS2_EARTHED_STANDBY_COMPLETE))
		{
			if(fb->t >=SET_TIME_SEQ_DELAY)
			{	
				fb->NextStep = Seq0600;
				fb->seqSts1.bit.seq0410 = SEQ_COMM_COMPLETE;
				fb->flagSeqComplete = FLAG_SEQ_COMPLETE;
				fb->t = 0.0f;			
			}
			else
			{
				fb->NextStep = Seq0410;
				fb->seqSts1.bit.seq0410 = SEQ_COMM_PROCESSING;
				fb->flagSeqComplete = FLAG_SEQ_UNCOMPLETE;
				fb->t += fb->cycleT;			
			}			
		}
		else if((fb->sysMode.bit.actPwr == SM_ACTPWR_VDC) && (fb->oSeqSts1.all == STS1_EARTHED_STOPPED_COMPLETE))
		{
			fb->yardSwiComm.bit.ds3x1 = YARD_SWITCH_COMM_CLOSE;

			if((fb->yardSwiSts.bit.ds3x1 == YARD_SWITCH_STATUS_CLOSE))
			{
				if(fb->t >=SET_TIME_SEQ_DELAY)
				{	
					fb->NextStep = Seq0600;
					fb->seqSts1.bit.seq0410 = SEQ_COMM_COMPLETE;
					fb->flagSeqComplete = FLAG_SEQ_COMPLETE;
					fb->seqSts2.bit.seq0510 = SEQ_COMM_UNPROCESSED;
					fb->seqSts2.bit.seq0520 = SEQ_COMM_UNPROCESSED;	
					fb->t = 0.0f;			
				}
				else
				{
					fb->NextStep = Seq0410;
					fb->seqSts1.bit.seq0410 = SEQ_COMM_PROCESSING;
					fb->flagSeqComplete = FLAG_SEQ_UNCOMPLETE;
					fb->t += fb->cycleT;			
				}				
			}
			else
			{
				fb->NextStep = Seq0410;
				fb->seqSts1.bit.seq0410 = SEQ_COMM_PROCESSING;
				fb->flagSeqComplete = FLAG_SEQ_UNCOMPLETE;
			}
		}
		else
		{
			fb->NextStep = Seq0410;
			fb->seqSts1.bit.seq0410 = SEQ_COMM_PROCESSING;
			fb->flagSeqComplete = FLAG_SEQ_UNCOMPLETE;
		}
	}
	else if(fb->sysMode.bit.seqDir == SM_SEQDIR_STOP)
	{
		fb->NextStep = Seq0510;
		fb->seqSts1.bit.seq0410 = SEQ_COMM_UNPROCESSED;
		fb->flagSeqComplete = FLAG_SEQ_UNCOMPLETE;	
	}
	else if(fb->sysMode.bit.seqDir == SM_SEQDIR_NODIR)
	{
		fb->NextStep = Seq0410;
		fb->seqSts1.bit.seq0410 = SEQ_COMM_UNPROCESSED;
		fb->flagSeqComplete = FLAG_SEQ_UNCOMPLETE;
	}
	else
	{
		fb->NextStep = Seq0410;
		fb->seqSts1.bit.seq0410 = SEQ_COMM_UNPROCESSED;
		fb->flagSeqComplete = FLAG_SEQ_UNCOMPLETE;
	}
}

void funcSeq0510(strFC0740Info *fb)
{
	if(fb->sysMode.bit.seqDir == SM_SEQDIR_START)
	{
		fb->NextStep = Seq0510;
		fb->seqSts2.bit.seq0510 = SEQ_COMM_UNPROCESSED;
		fb->flagSeqComplete = FLAG_SEQ_UNCOMPLETE;
	}
	else if(fb->sysMode.bit.seqDir == SM_SEQDIR_STOP)
	{
		if((fb->sysMode.bit.actPwr == SM_ACTPWR_P) && (fb->oSeqSts2.all == STS2_COUPLED_STANDBY_COMPLETE)
		||((fb->sysMode.bit.actPwr == SM_ACTPWR_P) && (fb->trip.all > TRIP_BAND) && (fb->oSeqSts2.bit.seq0510 == SEQ_COMM_PROCESSING)))
		{
			if(fb->t >=SET_TIME_SEQ_DELAY)
			{	
				fb->NextStep = Seq0400;
				fb->seqSts2.bit.seq0510 = SEQ_COMM_COMPLETE;
				fb->flagSeqComplete = FLAG_SEQ_UNCOMPLETE;	
				fb->seqSts1.bit.seq0410 = SEQ_COMM_UNPROCESSED;	
				fb->t = 0.0f;			
			}
			else
			{
				fb->NextStep = Seq0510;
				fb->seqSts2.bit.seq0510 = SEQ_COMM_PROCESSING;
				fb->flagSeqComplete = FLAG_SEQ_UNCOMPLETE;
				fb->t += fb->cycleT;			
			}				
		}
		else if((fb->sysMode.bit.actPwr == SM_ACTPWR_VDC) && (fb->oSeqSts2.all == STS2_COUPLED_STOPPED_COMPLETE) &  (fb->oSeqSts1.all == STS1_COUPLED_STOPPED_COMPLETE)
			 ||((fb->sysMode.bit.actPwr == SM_ACTPWR_VDC) && (fb->trip.all > TRIP_BAND) && (fb->oSeqSts1.bit.seq0200 == SEQ_COMM_UNPROCESSED)))
		{
			fb->yardSwiComm.bit.ds3x1 = YARD_SWITCH_COMM_OPEN;

			if((fb->yardSwiSts.bit.ds3x1 == YARD_SWITCH_STATUS_OPEN))
			{
				if(fb->t >=SET_TIME_SEQ_DELAY)
				{					
					fb->NextStep = Seq0400;
					fb->seqSts2.bit.seq0510 = SEQ_COMM_COMPLETE;
					fb->flagSeqComplete = FLAG_SEQ_UNCOMPLETE;
					fb->seqSts1.bit.seq0410 = SEQ_COMM_UNPROCESSED;
					fb->t = 0.0f;			
				}
				else
				{
					fb->NextStep = Seq0510;
					fb->seqSts2.bit.seq0510 = SEQ_COMM_PROCESSING;
					fb->flagSeqComplete = FLAG_SEQ_UNCOMPLETE;
					fb->t += fb->cycleT;			
				}
			}
			else
			{
				fb->NextStep = Seq0510;
				fb->seqSts2.bit.seq0510 = SEQ_COMM_PROCESSING;
				fb->flagSeqComplete = FLAG_SEQ_UNCOMPLETE;
			}
		}
		else
		{
			fb->NextStep = Seq0510;
			fb->seqSts2.bit.seq0510 = SEQ_COMM_PROCESSING;
			fb->flagSeqComplete = FLAG_SEQ_UNCOMPLETE;
		}
	}
	else if(fb->sysMode.bit.seqDir == SM_SEQDIR_NODIR)
	{
		fb->NextStep = Seq0510;
		fb->seqSts2.bit.seq0510 = SEQ_COMM_UNPROCESSED;
		fb->flagSeqComplete = FLAG_SEQ_UNCOMPLETE;
	}
	else
	{
		fb->NextStep = Seq0510;
		fb->seqSts2.bit.seq0510 = SEQ_COMM_UNPROCESSED;
		fb->flagSeqComplete = FLAG_SEQ_UNCOMPLETE;
	}	
}

void funcSeq0520(strFC0740Info *fb)
{
	if(fb->sysMode.bit.seqDir == SM_SEQDIR_START)
	{
		fb->NextStep = Seq0600;
		fb->seqSts2.bit.seq0520 = SEQ_COMM_UNPROCESSED;
		fb->flagSeqComplete = FLAG_SEQ_UNCOMPLETE;
	}
	else if(fb->sysMode.bit.seqDir == SM_SEQDIR_STOP)
	{
		if(((fb->sysMode.bit.actPwr == SM_ACTPWR_P) && (fb->oSeqSts2.all == STS2_COUPLED_STANDBY_COMPLETE)) \
			||((fb->sysMode.bit.actPwr == SM_ACTPWR_VDC) && (fb->oSeqSts2.all == STS2_COUPLED_STOPPED_COMPLETE) &  (fb->oSeqSts1.all == STS1_COUPLED_STOPPED_COMPLETE))
			||((fb->sysMode.bit.actPwr == SM_ACTPWR_VDC) && (fb->trip.all > TRIP_BAND) && (fb->oSeqSts2.bit.seq0510 == SEQ_COMM_PROCESSING)))
		{
			if((fb->sysSts.bit.convDischg == SS_CONVDISCHG_EXPIRED))
			{
				if(fb->t >=SET_TIME_SEQ_DELAY)
				{	
					fb->NextStep = Seq0510;
					fb->seqSts2.bit.seq0520 = SEQ_COMM_COMPLETE;
					fb->flagSeqComplete = FLAG_SEQ_UNCOMPLETE;					
					fb->t = 0.0f;			
				}
				else
				{
					fb->NextStep = Seq0520;
					fb->seqSts2.bit.seq0520 = SEQ_COMM_PROCESSING;
					fb->flagSeqComplete = FLAG_SEQ_UNCOMPLETE;
					fb->t += fb->cycleT;			
				}				
			}
			else
			{
				fb->NextStep = Seq0520;
				fb->seqSts2.bit.seq0520 = SEQ_COMM_PROCESSING;
				fb->flagSeqComplete = FLAG_SEQ_UNCOMPLETE;
			}
		}
		else
		{
			fb->NextStep = Seq0520;
			fb->seqSts2.bit.seq0520 = SEQ_COMM_PROCESSING;
			fb->flagSeqComplete = FLAG_SEQ_UNCOMPLETE;
		}
	}
	else if(fb->sysMode.bit.seqDir == SM_SEQDIR_NODIR)
	{
		fb->NextStep = Seq0520;
		fb->seqSts2.bit.seq0520 = SEQ_COMM_UNPROCESSED;
		fb->flagSeqComplete = FLAG_SEQ_UNCOMPLETE;
	}
	else
	{
		fb->NextStep = Seq0520;
		fb->seqSts2.bit.seq0520 = SEQ_COMM_UNPROCESSED;
		fb->flagSeqComplete = FLAG_SEQ_UNCOMPLETE;
	}	
}

void funcSeq0600(strFC0740Info *fb)
{
	if(fb->sysMode.bit.seqDir == SM_SEQDIR_START)
	{
		if(((fb->sysMode.bit.actPwr == SM_ACTPWR_P) && (fb->oSeqSts1.all == STS1_EARTHED_STANDBY_COMPLETE) && (fb->oSeqSts2.all == STS2_EARTHED_STANDBY_COMPLETE)) \
			|| ((fb->sysMode.bit.actPwr == SM_ACTPWR_VDC) && (fb->oSeqSts1.all == STS1_EARTHED_STOPPED_COMPLETE)))
		{
			if(fb->t >=SET_TIME_SEQ_DELAY)
			{	
				fb->NextStep = Seq0610;
				fb->seqSts2.bit.seq0600 = SEQ_COMM_COMPLETE;
				fb->flagSeqComplete = FLAG_SEQ_COMPLETE;
				fb->t = 0.0f;			
			}
			else
			{
				fb->NextStep = Seq0600;
				fb->seqSts2.bit.seq0600 = SEQ_COMM_PROCESSING;
				fb->flagSeqComplete = FLAG_SEQ_COMPLETE;
				fb->t += fb->cycleT;			
			}			

		}
		else
		{
			fb->NextStep = Seq0600;
			fb->seqSts2.bit.seq0600 = SEQ_COMM_PROCESSING;
			fb->flagSeqComplete = FLAG_SEQ_COMPLETE;			
		}	
	}
	else if(fb->sysMode.bit.seqDir == SM_SEQDIR_STOP)
	{
		if(((fb->sysMode.bit.actPwr == SM_ACTPWR_P) && (fb->oSeqSts2.all == STS2_STOP_EARTHED_COUPLED)) \
			||((fb->sysMode.bit.actPwr == SM_ACTPWR_VDC) && (fb->oSeqSts2.all == STS2_COUPLED_STANDBY_COMPLETE))
			||((fb->sysMode.bit.actPwr == SM_ACTPWR_VDC) && (fb->trip.all > TRIP_BAND) && (fb->oSeqSts2.bit.seq0510 == SEQ_COMM_PROCESSING)))
		{
			if(fb->t >=SET_TIME_SEQ_DELAY)
			{	
				fb->NextStep = Seq0520;
				fb->seqSts2.bit.seq0600 = SEQ_COMM_UNPROCESSED;
				fb->flagSeqComplete = FLAG_SEQ_UNCOMPLETE;				
				fb->t = 0.0f;			
			}
			else
			{
				fb->NextStep = Seq0600;
				fb->seqSts2.bit.seq0600 = SEQ_COMM_PROCESSING;
				fb->flagSeqComplete = FLAG_SEQ_COMPLETE;	
				fb->t += fb->cycleT;			
			}			
		}
		else
		{
			fb->NextStep = Seq0600;
			fb->seqSts2.bit.seq0600 = SEQ_COMM_PROCESSING;
			fb->flagSeqComplete = FLAG_SEQ_COMPLETE;	
		}
	}
	else if(fb->sysMode.bit.seqDir == SM_SEQDIR_NODIR)
	{
		fb->NextStep = Seq0600;
		fb->seqSts2.bit.seq0600 = SEQ_COMM_UNPROCESSED;
		fb->flagSeqComplete = FLAG_SEQ_UNCOMPLETE;
	}
	else
	{
		fb->NextStep = Seq0600;
		fb->seqSts2.bit.seq0600 = SEQ_COMM_UNPROCESSED;
		fb->flagSeqComplete = FLAG_SEQ_UNCOMPLETE;
	}
}

void funcSeq0610(strFC0740Info *fb)
{
	uint32 subSts;

	subSts = SS_INTLCK_UNCOMPLETE;
/*
	fb->sysSts.bit.trCooling == SS_TRCOOLING_ON 				// sysSts.2
	fb->sysSts.bit.cpSys == SS_CPSYS_READY 						// sysSts.4
	fb->yardSwiSts.bit.cbx91 == YARD_SWITCH_STATUS_OPEN 		// DI.0
	fb->yardSwiSts.bit.cbx01 == YARD_SWITCH_STATUS_OPEN 		// DI.1
	fb->yardSwiSts.bit.ds3x1 == YARD_SWITCH_STATUS_CLOSE 		// DI.4
	fb->sysSts.bit.tapChgPos == SS_TAPCHSPOS_INPOS				// syssts.5
	fb->sysSts.bit.noEmy == SS_NOEMY_ACTIVE 					// sysSts.6
	fb->sysSts.bit.convDischg == SS_CONVDISCHG_EXPIRED			// sysSts.7
*/		
		
	//Zig-Zag TR contributes to the system in Vdc mode
	if(fb->sysMode.bit.actPwr == SM_ACTPWR_VDC)
	{
		if((fb->sysSts.bit.trCooling == SS_TRCOOLING_ON)
			&& (fb->sysSts.bit.cpSys == SS_CPSYS_READY)						
			&& (fb->yardSwiSts.bit.cbx91 == YARD_SWITCH_STATUS_OPEN) 			
			&& (fb->yardSwiSts.bit.cbx01 == YARD_SWITCH_STATUS_OPEN) 			
			&& (fb->yardSwiSts.bit.ds3x1 == YARD_SWITCH_STATUS_CLOSE) 			
			&& (fb->sysSts.bit.tapChgPos == SS_TAPCHSPOS_INPOS)				
			&& (fb->sysSts.bit.noEmy == SS_NOEMY_ACTIVE) 						
			&& (fb->sysSts.bit.convDischg == SS_CONVDISCHG_EXPIRED)
			&& (fb->yardSwiSts.bit.dsx01 == YARD_SWITCH_STATUS_CLOSE))			
			subSts = SS_INTLCK_COMPLETE;
		else
			subSts = SS_INTLCK_UNCOMPLETE;				
	}
	else
	{
		if((fb->sysSts.bit.trCooling == SS_TRCOOLING_ON)
			&& (fb->sysSts.bit.cpSys == SS_CPSYS_READY)						
			&& (fb->yardSwiSts.bit.cbx91 == YARD_SWITCH_STATUS_OPEN) 			
			&& (fb->yardSwiSts.bit.cbx01 == YARD_SWITCH_STATUS_OPEN) 			
			&& (fb->sysSts.bit.tapChgPos == SS_TAPCHSPOS_INPOS)				
			&& (fb->sysSts.bit.noEmy == SS_NOEMY_ACTIVE) 						
			&& (fb->sysSts.bit.convDischg == SS_CONVDISCHG_EXPIRED)			
			&& (fb->yardSwiSts.bit.dsx01 == YARD_SWITCH_STATUS_CLOSE))				
			subSts = SS_INTLCK_COMPLETE;
		else
			subSts = SS_INTLCK_UNCOMPLETE;			
	}
		

	if(fb->sysMode.bit.seqDir == SM_SEQDIR_START)
	{
		if(((fb->sysMode.bit.actPwr == SM_ACTPWR_P) && (fb->oSeqSts1.all == STS1_EARTHED_COUPLED_COMPLETE) && (fb->oSeqSts2.all == STS2_EARTHED_COUPLED_COMPLETE)) \
			||((fb->sysMode.bit.actPwr == SM_ACTPWR_VDC) && (fb->oSeqSts1.all == STS1_EARTHED_STANDBY_COMPLETE) && (fb->oSeqSts2.all == STS2_EARTHED_STANDBY_COMPLETE)))
		{
			if((subSts == SS_INTLCK_COMPLETE))
			{
				if(fb->t >=SET_TIME_SEQ_DELAY)
				{	
					fb->NextStep = Seq0620;
					fb->seqSts2.bit.seq0610 = SEQ_COMM_COMPLETE;
					fb->flagSeqComplete = FLAG_SEQ_COMPLETE;					
					fb->t = 0.0f;			
				}
				else
				{
					fb->NextStep = Seq0610;
					fb->seqSts2.bit.seq0610 = SEQ_COMM_PROCESSING;
					fb->flagSeqComplete = FLAG_SEQ_UNCOMPLETE;
					fb->t += fb->cycleT;			
				}				
			}
			else
			{
				fb->NextStep = Seq0610;
				fb->seqSts2.bit.seq0610 = SEQ_COMM_PROCESSING;
				fb->flagSeqComplete = FLAG_SEQ_UNCOMPLETE;
			}
		}
		else
		{
			fb->NextStep = Seq0610;
			fb->seqSts2.bit.seq0610 = SEQ_COMM_PROCESSING;
			fb->flagSeqComplete = FLAG_SEQ_UNCOMPLETE;
		}
	}
	else if(fb->sysMode.bit.seqDir == SM_SEQDIR_STOP)
	{
		fb->NextStep = Seq0600;
		fb->seqSts2.bit.seq0610 = SEQ_COMM_UNPROCESSED;
		fb->flagSeqComplete = FLAG_SEQ_UNCOMPLETE;
	}
	else if(fb->sysMode.bit.seqDir == SM_SEQDIR_NODIR)
	{
		fb->NextStep = Seq0610;
		fb->seqSts2.bit.seq0610 = SEQ_COMM_UNPROCESSED;
		fb->flagSeqComplete = FLAG_SEQ_UNCOMPLETE;
	}
	else
	{
		fb->NextStep = Seq0610;
		fb->seqSts2.bit.seq0610 = SEQ_COMM_UNPROCESSED;
		fb->flagSeqComplete = FLAG_SEQ_UNCOMPLETE;
	}
}

void funcSeq0620(strFC0740Info *fb)
{
	if(fb->sysMode.bit.seqDir == SM_SEQDIR_START)
	{
		if(((fb->sysMode.bit.actPwr == SM_ACTPWR_P) && (fb->oSeqSts1.all == STS1_EARTHED_COUPLED_COMPLETE) && (fb->oSeqSts2.all == STS2_EARTHED_COUPLED_COMPLETE)) \
			||((fb->sysMode.bit.actPwr == SM_ACTPWR_VDC) && (fb->oSeqSts1.all == STS1_EARTHED_STANDBY_COMPLETE) && (fb->oSeqSts2.all == STS2_EARTHED_STANDBY_COMPLETE)))
		{
			fb->yardSwiComm.bit.dsx01 = YARD_SWITCH_COMM_OPEN;

			if((fb->yardSwiSts.bit.dsx01 == YARD_SWITCH_STATUS_OPEN))
			{
				if(fb->t >=SET_TIME_SEQ_DELAY)
				{	
					fb->NextStep = Seq0630;
					fb->seqSts2.bit.seq0620 = SEQ_COMM_COMPLETE;
					fb->flagSeqComplete = FLAG_SEQ_COMPLETE;
					fb->seqSts2.bit.seq0720 = SEQ_COMM_UNPROCESSED;
					fb->t = 0.0f;			
				}
				else
				{
					fb->NextStep = Seq0620;
					fb->seqSts2.bit.seq0620 = SEQ_COMM_PROCESSING;
					fb->flagSeqComplete = FLAG_SEQ_UNCOMPLETE;
					fb->t += fb->cycleT;			
				}
			}
			else
			{
				fb->NextStep = Seq0620;
				fb->seqSts2.bit.seq0620 = SEQ_COMM_PROCESSING;
				fb->flagSeqComplete = FLAG_SEQ_UNCOMPLETE;
			}
		}
		else
		{
			fb->NextStep = Seq0620;
			fb->seqSts2.bit.seq0620 = SEQ_COMM_PROCESSING;
			fb->flagSeqComplete = FLAG_SEQ_UNCOMPLETE;
		}		
	}
	else if(fb->sysMode.bit.seqDir == SM_SEQDIR_STOP)
	{
		fb->NextStep = Seq0720;
		fb->seqSts2.bit.seq0620 = SEQ_COMM_UNPROCESSED;
		fb->flagSeqComplete = FLAG_SEQ_UNCOMPLETE;
	}
	else if(fb->sysMode.bit.seqDir == SM_SEQDIR_NODIR)
	{
		fb->NextStep = Seq0620;
		fb->seqSts2.bit.seq0620 = SEQ_COMM_UNPROCESSED;
		fb->flagSeqComplete = FLAG_SEQ_UNCOMPLETE;
	}
	else
	{
		fb->NextStep = Seq0620;
		fb->seqSts2.bit.seq0620 = SEQ_COMM_UNPROCESSED;
		fb->flagSeqComplete = FLAG_SEQ_UNCOMPLETE;
	}
}

void funcSeq0630(strFC0740Info *fb)
{
	if(fb->sysMode.bit.seqDir == SM_SEQDIR_START)
	{
		if(((fb->sysMode.bit.actPwr == SM_ACTPWR_P) && (fb->oSeqSts1.all == STS1_EARTHED_COUPLED_COMPLETE) && (fb->oSeqSts2.all == STS2_EARTHED_COUPLED_COMPLETE)) \
			||((fb->sysMode.bit.actPwr == SM_ACTPWR_VDC) && (fb->oSeqSts1.all == STS1_EARTHED_STANDBY_COMPLETE) && (fb->oSeqSts2.all == STS2_EARTHED_STANDBY_COMPLETE)))
		{
			fb->yardSwiComm.bit.cbx91 = YARD_SWITCH_COMM_CLOSE;
			
			if((fb->yardSwiSts.bit.cbx91 == YARD_SWITCH_STATUS_CLOSE))
			{
				if(fb->t >=SET_TIME_SEQ_DELAY)
				{	
					fb->NextStep = Seq0640;
					fb->seqSts2.bit.seq0630 = SEQ_COMM_COMPLETE;
					fb->flagSeqComplete = FLAG_SEQ_COMPLETE;
					fb->seqSts2.bit.seq0730 = SEQ_COMM_UNPROCESSED;
					fb->t = 0.0f;			
				}
				else
				{
					fb->NextStep = Seq0630;
					fb->seqSts2.bit.seq0630 = SEQ_COMM_PROCESSING;
					fb->flagSeqComplete = FLAG_SEQ_UNCOMPLETE;
					fb->t += fb->cycleT;			
				}				
			}
			else
			{
				fb->NextStep = Seq0630;
				fb->seqSts2.bit.seq0630 = SEQ_COMM_PROCESSING;
				fb->flagSeqComplete = FLAG_SEQ_UNCOMPLETE;
			}
		}
		else
		{
			fb->NextStep = Seq0630;
			fb->seqSts2.bit.seq0630 = SEQ_COMM_PROCESSING;
			fb->flagSeqComplete = FLAG_SEQ_UNCOMPLETE;
		}
	}
	else if(fb->sysMode.bit.seqDir == SM_SEQDIR_STOP)
	{
		fb->NextStep = Seq0730;
		fb->seqSts2.bit.seq0630 = SEQ_COMM_UNPROCESSED;
		fb->flagSeqComplete = FLAG_SEQ_UNCOMPLETE;
	}
	else if(fb->sysMode.bit.seqDir == SM_SEQDIR_NODIR)
	{
		fb->NextStep = Seq0630;
		fb->seqSts2.bit.seq0630 = SEQ_COMM_UNPROCESSED;
		fb->flagSeqComplete = FLAG_SEQ_UNCOMPLETE;
	}
	else
	{
		fb->NextStep = Seq0630;
		fb->seqSts2.bit.seq0630 = SEQ_COMM_UNPROCESSED;
		fb->flagSeqComplete = FLAG_SEQ_UNCOMPLETE;
	}
}

void funcSeq0640(strFC0740Info *fb)
{
	if(fb->sysMode.bit.seqDir == SM_SEQDIR_START)
	{
		if(((fb->sysMode.bit.actPwr == SM_ACTPWR_P) && (fb->oSeqSts1.all == STS1_EARTHED_COUPLED_COMPLETE) && (fb->oSeqSts2.all == STS2_EARTHED_COUPLED_COMPLETE)) \
			||((fb->sysMode.bit.actPwr == SM_ACTPWR_VDC) && (fb->oSeqSts1.all == STS1_EARTHED_STANDBY_COMPLETE) && (fb->oSeqSts2.all == STS2_EARTHED_STANDBY_COMPLETE)))
		{
			fb->yardSwiComm.bit.cbx01 = YARD_SWITCH_COMM_CLOSE;

			if((fb->yardSwiSts.bit.cbx01 == YARD_SWITCH_STATUS_CLOSE))
			{
				if(fb->t >=SET_TIME_SEQ_DELAY)
				{	
					fb->NextStep = Seq0650;
					fb->seqSts2.bit.seq0640 = SEQ_COMM_COMPLETE;
					fb->flagSeqComplete = FLAG_SEQ_COMPLETE;
					fb->seqSts2.bit.seq0740 = SEQ_COMM_UNPROCESSED;					
					fb->t = 0.0f;			
				}
				else
				{
					fb->NextStep = Seq0640;
					fb->seqSts2.bit.seq0640 = SEQ_COMM_PROCESSING;
					fb->flagSeqComplete = FLAG_SEQ_UNCOMPLETE;
					fb->t += fb->cycleT;			
				}				
			}
			else
			{
				fb->NextStep = Seq0640;
				fb->seqSts2.bit.seq0640 = SEQ_COMM_PROCESSING;
				fb->flagSeqComplete = FLAG_SEQ_UNCOMPLETE;
			}
		}
		else
		{
			fb->NextStep = Seq0640;
			fb->seqSts2.bit.seq0640 = SEQ_COMM_PROCESSING;
			fb->flagSeqComplete = FLAG_SEQ_UNCOMPLETE;	
		}
	}
	else if(fb->sysMode.bit.seqDir == SM_SEQDIR_STOP)
	{
		fb->NextStep = Seq0740;
		fb->seqSts2.bit.seq0640 = SEQ_COMM_UNPROCESSED;
		fb->flagSeqComplete = FLAG_SEQ_UNCOMPLETE;
	}
	else if(fb->sysMode.bit.seqDir == SM_SEQDIR_NODIR)
	{
		fb->NextStep = Seq0640;
		fb->seqSts2.bit.seq0640 = SEQ_COMM_UNPROCESSED;
		fb->flagSeqComplete = FLAG_SEQ_UNCOMPLETE;
	}
	else
	{
		fb->NextStep = Seq0640;
		fb->seqSts2.bit.seq0640 = SEQ_COMM_UNPROCESSED;
		fb->flagSeqComplete = FLAG_SEQ_UNCOMPLETE;
	}
}


void funcSeq0650(strFC0740Info *fb)
{
	if(fb->sysMode.bit.seqDir == SM_SEQDIR_START)
	{
		if(((fb->sysMode.bit.actPwr == SM_ACTPWR_P) && (fb->oSeqSts1.all == STS1_EARTHED_COUPLED_COMPLETE) && (fb->oSeqSts2.all == STS2_EARTHED_COUPLED_COMPLETE)) \
			||((fb->sysMode.bit.actPwr == SM_ACTPWR_VDC) && (fb->oSeqSts1.all == STS1_EARTHED_STANDBY_COMPLETE) && (fb->oSeqSts2.all == STS2_EARTHED_STANDBY_COMPLETE)))
		{
			if((fb->sysSts.bit.passiveChg == SS_PASSIVECHG_COMPLETE))
			{
				if(fb->t >=SET_TIME_SEQ_DELAY)
				{	
					fb->NextStep = Seq0660;
					fb->seqSts2.bit.seq0650 = SEQ_COMM_COMPLETE;
					fb->flagSeqComplete = FLAG_SEQ_COMPLETE;					
					fb->t = 0.0f;			
				}
				else
				{
					fb->NextStep = Seq0650;
					fb->seqSts2.bit.seq0650 = SEQ_COMM_PROCESSING;
					fb->flagSeqComplete = FLAG_SEQ_UNCOMPLETE;
					fb->t += fb->cycleT;			
				}				
			}
			else
			{
				fb->NextStep = Seq0650;
				fb->seqSts2.bit.seq0650 = SEQ_COMM_PROCESSING;
				fb->flagSeqComplete = FLAG_SEQ_UNCOMPLETE;
			}
		}
		else
		{
			fb->NextStep = Seq0650;
			fb->seqSts2.bit.seq0650 = SEQ_COMM_PROCESSING;
			fb->flagSeqComplete = FLAG_SEQ_UNCOMPLETE;
		}
	}
	else if(fb->sysMode.bit.seqDir == SM_SEQDIR_STOP)
	{
		fb->NextStep = Seq0740;
		fb->seqSts2.bit.seq0650 = SEQ_COMM_UNPROCESSED;
		fb->flagSeqComplete = FLAG_SEQ_UNCOMPLETE;
	}
	else if(fb->sysMode.bit.seqDir == SM_SEQDIR_NODIR)
	{
		fb->NextStep = Seq0650;
		fb->seqSts2.bit.seq0650 = SEQ_COMM_UNPROCESSED;
		fb->flagSeqComplete = FLAG_SEQ_UNCOMPLETE;
	}
	else
	{
		fb->NextStep = Seq0650;
		fb->seqSts2.bit.seq0650 = SEQ_COMM_UNPROCESSED;
		fb->flagSeqComplete = FLAG_SEQ_UNCOMPLETE;
	}
}

void funcSeq0660(strFC0740Info *fb)
{
	if(fb->sysMode.bit.seqDir == SM_SEQDIR_START)
	{
		if(((fb->sysMode.bit.actPwr == SM_ACTPWR_P) && (fb->oSeqSts1.all == STS1_EARTHED_COUPLED_COMPLETE) && (fb->oSeqSts2.all == STS2_EARTHED_COUPLED_COMPLETE)) \
			||((fb->sysMode.bit.actPwr == SM_ACTPWR_VDC) && (fb->oSeqSts1.all == STS1_EARTHED_STANDBY_COMPLETE) && (fb->oSeqSts2.all == STS2_EARTHED_STANDBY_COMPLETE)))
		{
			fb->yardSwiComm.bit.dsx01 = YARD_SWITCH_COMM_CLOSE;

			if((fb->yardSwiSts.bit.dsx01 == YARD_SWITCH_STATUS_CLOSE))
			{
				if(fb->t >=SET_TIME_SEQ_DELAY)
				{	
					fb->NextStep = Seq0670;
					fb->seqSts2.bit.seq0660 = SEQ_COMM_COMPLETE;
					fb->flagSeqComplete = FLAG_SEQ_COMPLETE;
					fb->seqSts2.bit.seq0760 = SEQ_COMM_UNPROCESSED;
					fb->t = 0.0f;			
				}
				else
				{
					fb->NextStep = Seq0660;
					fb->seqSts2.bit.seq0660 = SEQ_COMM_PROCESSING;
					fb->flagSeqComplete = FLAG_SEQ_UNCOMPLETE;					
					fb->t += fb->cycleT;			
				}				
			}
			else
			{
				fb->NextStep = Seq0660;
				fb->seqSts2.bit.seq0660 = SEQ_COMM_PROCESSING;
				fb->flagSeqComplete = FLAG_SEQ_UNCOMPLETE;
			}
		}
		else
		{
			fb->NextStep = Seq0660;
			fb->seqSts2.bit.seq0660 = SEQ_COMM_PROCESSING;
			fb->flagSeqComplete = FLAG_SEQ_UNCOMPLETE;
		}
	}
	else if(fb->sysMode.bit.seqDir == SM_SEQDIR_STOP)
	{
		fb->NextStep = Seq0760;
		fb->seqSts2.bit.seq0660 = SEQ_COMM_UNPROCESSED;
		fb->flagSeqComplete = FLAG_SEQ_UNCOMPLETE;
	}
	else if(fb->sysMode.bit.seqDir == SM_SEQDIR_NODIR)
	{
		fb->NextStep = Seq0660;
		fb->seqSts2.bit.seq0660 = SEQ_COMM_UNPROCESSED;
		fb->flagSeqComplete = FLAG_SEQ_UNCOMPLETE;
	}
	else
	{
		fb->NextStep = Seq0660;
		fb->seqSts2.bit.seq0660 = SEQ_COMM_UNPROCESSED;
		fb->flagSeqComplete = FLAG_SEQ_UNCOMPLETE;
	}
}

void funcSeq0670(strFC0740Info *fb)
{
	if(fb->sysMode.bit.seqDir == SM_SEQDIR_START)
	{
		if(((fb->sysMode.bit.actPwr == SM_ACTPWR_P) && (fb->oSeqSts1.all == STS1_EARTHED_COUPLED_COMPLETE) && (fb->oSeqSts2.all == STS2_EARTHED_COUPLED_COMPLETE)) \
			||((fb->sysMode.bit.actPwr == SM_ACTPWR_VDC) && (fb->oSeqSts1.all == STS1_EARTHED_STANDBY_COMPLETE) && (fb->oSeqSts2.all == STS2_EARTHED_STANDBY_COMPLETE)))
		{
			if((fb->sysSts.bit.convActchgSeq == SS_ACTCHGSEQ_COMPLETE))
			{
				if(fb->t >=SET_TIME_SEQ_DELAY)
				{	
					fb->NextStep = Seq0800;
					fb->seqSts2.bit.seq0670 = SEQ_COMM_COMPLETE;
					fb->flagSeqComplete = FLAG_SEQ_COMPLETE;
					fb->seqSts2.bit.seq0770 = SEQ_COMM_UNPROCESSED;					
					fb->t = 0.0f;			
				}
				else
				{
					fb->NextStep = Seq0670;
					fb->seqSts2.bit.seq0670 = SEQ_COMM_PROCESSING;
					fb->flagSeqComplete = FLAG_SEQ_UNCOMPLETE;
					fb->t += fb->cycleT;			
				}
			}
			else
			{
				fb->NextStep = Seq0670;
				fb->seqSts2.bit.seq0670 = SEQ_COMM_PROCESSING;
				fb->flagSeqComplete = FLAG_SEQ_UNCOMPLETE;
			}
		}
		else
		{
			fb->NextStep = Seq0670;
			fb->seqSts2.bit.seq0670 = SEQ_COMM_PROCESSING;
			fb->flagSeqComplete = FLAG_SEQ_UNCOMPLETE;
		}		
	}
	else if(fb->sysMode.bit.seqDir == SM_SEQDIR_STOP)
	{
		fb->NextStep = Seq0770;
		fb->seqSts2.bit.seq0670 = SEQ_COMM_UNPROCESSED;
		fb->flagSeqComplete = FLAG_SEQ_UNCOMPLETE;
	}
	else if(fb->sysMode.bit.seqDir == SM_SEQDIR_NODIR)
	{
		fb->NextStep = Seq0670;
		fb->seqSts2.bit.seq0670 = SEQ_COMM_UNPROCESSED;
		fb->flagSeqComplete = FLAG_SEQ_UNCOMPLETE;
	}
	else
	{
		fb->NextStep = Seq0670;
		fb->seqSts2.bit.seq0670 = SEQ_COMM_UNPROCESSED;
		fb->flagSeqComplete = FLAG_SEQ_UNCOMPLETE;
	}
}

void funcSeq0720(strFC0740Info *fb)
{
	if(fb->sysMode.bit.seqDir == SM_SEQDIR_START)
	{
		fb->NextStep = Seq0720;
		fb->seqSts2.bit.seq0720 = SEQ_COMM_UNPROCESSED;
		fb->flagSeqComplete = FLAG_SEQ_UNCOMPLETE;
	}
	else if(fb->sysMode.bit.seqDir == SM_SEQDIR_STOP)
	{
		if(((fb->sysMode.bit.actPwr == SM_ACTPWR_P) && (fb->oSeqSts2.all == STS2_STOP_EARTHED_COUPLED)) \
			||((fb->sysMode.bit.actPwr == SM_ACTPWR_VDC) && (fb->oSeqSts2.all == STS2_COUPLED_STANDBY_COMPLETE)) 
			||((fb->sysMode.bit.actPwr == SM_ACTPWR_VDC) && (fb->trip.all > TRIP_BAND) && (fb->oSeqSts2.bit.seq0720 == SEQ_COMM_PROCESSING)))
		{
			fb->yardSwiComm.bit.dsx01 = YARD_SWITCH_COMM_CLOSE;

			if((fb->yardSwiSts.bit.dsx01 == YARD_SWITCH_STATUS_CLOSE))
			{
				if(fb->t >=SET_TIME_SEQ_DELAY)
				{	
					fb->NextStep = Seq0600;
					fb->seqSts2.bit.seq0720 = SEQ_COMM_COMPLETE;
					fb->flagSeqComplete = FLAG_SEQ_COMPLETE;
					fb->seqSts2.bit.seq0620 = SEQ_COMM_UNPROCESSED;
					fb->seqSts2.bit.seq0610 = SEQ_COMM_UNPROCESSED;
					fb->t = 0.0f;			
				}
				else
				{
					fb->NextStep = Seq0720;
					fb->seqSts2.bit.seq0720 = SEQ_COMM_PROCESSING;
					fb->flagSeqComplete = FLAG_SEQ_UNCOMPLETE;
					fb->t += fb->cycleT;			
				}
			}
			else
			{
				fb->NextStep = Seq0720;
				fb->seqSts2.bit.seq0720 = SEQ_COMM_PROCESSING;
				fb->flagSeqComplete = FLAG_SEQ_UNCOMPLETE;
			}
		}
		else
		{
			fb->NextStep = Seq0720;
			fb->seqSts2.bit.seq0720 = SEQ_COMM_PROCESSING;
			fb->flagSeqComplete = FLAG_SEQ_UNCOMPLETE;
		}
	}
	else if(fb->sysMode.bit.seqDir == SM_SEQDIR_NODIR)
	{
		fb->NextStep = Seq0720;
		fb->seqSts2.bit.seq0720 = SEQ_COMM_UNPROCESSED;
		fb->flagSeqComplete = FLAG_SEQ_UNCOMPLETE;
	}
	else
	{
		fb->NextStep = Seq0720;
		fb->seqSts2.bit.seq0720 = SEQ_COMM_UNPROCESSED;
		fb->flagSeqComplete = FLAG_SEQ_UNCOMPLETE;
	}	
}

void funcSeq0730(strFC0740Info *fb)
{
	if(fb->sysMode.bit.seqDir == SM_SEQDIR_START)
	{
		fb->NextStep = Seq0730;
		fb->seqSts2.bit.seq0730 = SEQ_COMM_UNPROCESSED;
		fb->flagSeqComplete = FLAG_SEQ_UNCOMPLETE;
	}
	else if(fb->sysMode.bit.seqDir == SM_SEQDIR_STOP)
	{
		if(((fb->sysMode.bit.actPwr == SM_ACTPWR_P) && (fb->oSeqSts2.all == STS2_STOP_EARTHED_COUPLED)) \
			||((fb->sysMode.bit.actPwr == SM_ACTPWR_VDC) && (fb->oSeqSts2.all == STS2_COUPLED_STANDBY_COMPLETE))
			||((fb->sysMode.bit.actPwr == SM_ACTPWR_VDC) && (fb->trip.all > TRIP_BAND) && (fb->oSeqSts2.bit.seq0720 == SEQ_COMM_PROCESSING)))
		{
			fb->yardSwiComm.bit.cbx91 = YARD_SWITCH_COMM_OPEN;	
			
			if((fb->yardSwiSts.bit.cbx91 == YARD_SWITCH_STATUS_OPEN))
			{
				if(fb->t >=SET_TIME_SEQ_DELAY)
				{	
					fb->NextStep = Seq0720;
					fb->seqSts2.bit.seq0730 = SEQ_COMM_COMPLETE;
					fb->flagSeqComplete = FLAG_SEQ_COMPLETE;
					fb->seqSts2.bit.seq0630 = SEQ_COMM_UNPROCESSED;				
					fb->t = 0.0f;			
				}
				else
				{
					fb->NextStep = Seq0730;
					fb->seqSts2.bit.seq0730 = SEQ_COMM_PROCESSING;
					fb->flagSeqComplete = FLAG_SEQ_UNCOMPLETE;
					fb->t += fb->cycleT;			
				}
			}
			else
			{			
				fb->NextStep = Seq0730;
				fb->seqSts2.bit.seq0730 = SEQ_COMM_PROCESSING;
				fb->flagSeqComplete = FLAG_SEQ_UNCOMPLETE;
			}			
		}
		else
		{
			fb->NextStep = Seq0730;
			fb->seqSts2.bit.seq0730 = SEQ_COMM_PROCESSING;
			fb->flagSeqComplete = FLAG_SEQ_UNCOMPLETE;			
		}	
	}
	else if(fb->sysMode.bit.seqDir == SM_SEQDIR_NODIR)
	{
		fb->NextStep = Seq0730;
		fb->seqSts2.bit.seq0730 = SEQ_COMM_UNPROCESSED;
		fb->flagSeqComplete = FLAG_SEQ_UNCOMPLETE;
	}
	else
	{
		fb->NextStep = Seq0730;
		fb->seqSts2.bit.seq0730 = SEQ_COMM_UNPROCESSED;
		fb->flagSeqComplete = FLAG_SEQ_UNCOMPLETE;
	}	
}

void funcSeq0740(strFC0740Info *fb)
{
	if(fb->sysMode.bit.seqDir == SM_SEQDIR_START)
	{
		fb->NextStep = Seq0740;
		fb->seqSts2.bit.seq0740 = SEQ_COMM_UNPROCESSED;
		fb->flagSeqComplete = FLAG_SEQ_UNCOMPLETE;
	}
	else if(fb->sysMode.bit.seqDir == SM_SEQDIR_STOP)
	{
		if(((fb->sysMode.bit.actPwr == SM_ACTPWR_P) && (fb->oSeqSts2.all == STS2_STOP_EARTHED_COUPLED)) \
			||((fb->sysMode.bit.actPwr == SM_ACTPWR_VDC) && (fb->oSeqSts2.all == STS2_COUPLED_STANDBY_COMPLETE))
			||((fb->sysMode.bit.actPwr == SM_ACTPWR_VDC) && (fb->trip.all > TRIP_BAND) && (fb->oSeqSts2.bit.seq0720 == SEQ_COMM_PROCESSING)))
		{
			fb->yardSwiComm.bit.cbx01 = YARD_SWITCH_COMM_OPEN;		

			if((fb->yardSwiSts.bit.cbx01 == YARD_SWITCH_STATUS_OPEN))
			{
				if(fb->t >=SET_TIME_SEQ_DELAY)
				{	
					fb->NextStep = Seq0730;
					fb->seqSts2.bit.seq0740 = SEQ_COMM_COMPLETE;
					fb->flagSeqComplete = FLAG_SEQ_COMPLETE;
					fb->seqSts2.bit.seq0640 = SEQ_COMM_UNPROCESSED;					
					fb->t = 0.0f;			
				}
				else
				{
					fb->NextStep = Seq0740;
					fb->seqSts2.bit.seq0740 = SEQ_COMM_PROCESSING;
					fb->flagSeqComplete = FLAG_SEQ_UNCOMPLETE;
					fb->t += fb->cycleT;			
				}				
			}
			else
			{
				fb->NextStep = Seq0740;
				fb->seqSts2.bit.seq0740 = SEQ_COMM_PROCESSING;
				fb->flagSeqComplete = FLAG_SEQ_UNCOMPLETE;
			}
		}
		else
		{
			fb->NextStep = Seq0740;
			fb->seqSts2.bit.seq0740 = SEQ_COMM_PROCESSING;
			fb->flagSeqComplete = FLAG_SEQ_UNCOMPLETE;
		}

	}
	else if(fb->sysMode.bit.seqDir == SM_SEQDIR_NODIR)
	{
		fb->NextStep = Seq0740;
		fb->seqSts2.bit.seq0740 = SEQ_COMM_UNPROCESSED;
		fb->flagSeqComplete = FLAG_SEQ_UNCOMPLETE;
	}
	else
	{
		fb->NextStep = Seq0740;
		fb->seqSts2.bit.seq0740 = SEQ_COMM_UNPROCESSED;
		fb->flagSeqComplete = FLAG_SEQ_UNCOMPLETE;
	}	
}

void funcSeq0760(strFC0740Info *fb)
{
	if(fb->sysMode.bit.seqDir == SM_SEQDIR_START)
	{
		fb->NextStep = Seq0760;
		fb->seqSts2.bit.seq0760 = SEQ_COMM_UNPROCESSED; 
		fb->flagSeqComplete = FLAG_SEQ_UNCOMPLETE;
	}
	else if(fb->sysMode.bit.seqDir == SM_SEQDIR_STOP)
	{
		if(((fb->sysMode.bit.actPwr == SM_ACTPWR_P) && (fb->oSeqSts2.all == STS2_STOP_EARTHED_COUPLED)) \
			||((fb->sysMode.bit.actPwr == SM_ACTPWR_VDC) && (fb->oSeqSts2.all == STS2_COUPLED_STANDBY_COMPLETE))
			||((fb->sysMode.bit.actPwr == SM_ACTPWR_VDC) && (fb->trip.all > TRIP_BAND) && (fb->oSeqSts2.bit.seq0720 == SEQ_COMM_PROCESSING)))
		{
			fb->yardSwiComm.bit.dsx01 = YARD_SWITCH_COMM_OPEN;		

			if((fb->yardSwiSts.bit.dsx01 == YARD_SWITCH_STATUS_OPEN))
			{
				if(fb->t >=SET_TIME_SEQ_DELAY)
				{	
					fb->NextStep = Seq0740;
					fb->seqSts2.bit.seq0760 = SEQ_COMM_COMPLETE; 
					fb->flagSeqComplete = FLAG_SEQ_COMPLETE;
					fb->seqSts2.bit.seq0660 = SEQ_COMM_UNPROCESSED;
					fb->seqSts2.bit.seq0650 = SEQ_COMM_UNPROCESSED;					
					fb->t = 0.0f;			
				}
				else
				{
					fb->NextStep = Seq0760;
					fb->seqSts2.bit.seq0760 = SEQ_COMM_PROCESSING; 
					fb->flagSeqComplete = FLAG_SEQ_UNCOMPLETE;
					fb->t += fb->cycleT;			
				}				
			}
			else
			{
				fb->NextStep = Seq0760;
				fb->seqSts2.bit.seq0760 = SEQ_COMM_PROCESSING; 
				fb->flagSeqComplete = FLAG_SEQ_UNCOMPLETE;
			}
		}
		else
		{
			fb->NextStep = Seq0760;
			fb->seqSts2.bit.seq0760 = SEQ_COMM_PROCESSING; 
			fb->flagSeqComplete = FLAG_SEQ_UNCOMPLETE;
		}		
	}
	else if(fb->sysMode.bit.seqDir == SM_SEQDIR_NODIR)
	{
		fb->NextStep = Seq0760;
		fb->seqSts2.bit.seq0760 = SEQ_COMM_UNPROCESSED; 
		fb->flagSeqComplete = FLAG_SEQ_UNCOMPLETE;
	}
	else
	{
		fb->NextStep = Seq0760;
		fb->seqSts2.bit.seq0760 = SEQ_COMM_UNPROCESSED; 
		fb->flagSeqComplete = FLAG_SEQ_UNCOMPLETE;
	}	
}

void funcSeq0770(strFC0740Info *fb)
{
	if(fb->sysMode.bit.seqDir == SM_SEQDIR_START)
	{
		fb->NextStep = Seq0770;
		fb->seqSts2.bit.seq0770 = SEQ_COMM_UNPROCESSED;
		fb->flagSeqComplete = FLAG_SEQ_UNCOMPLETE;
	}
	else if(fb->sysMode.bit.seqDir == SM_SEQDIR_STOP)
	{
		if(((fb->sysMode.bit.actPwr == SM_ACTPWR_P) && (fb->oSeqSts2.all == STS2_STOP_EARTHED_COUPLED)) \
			||((fb->sysMode.bit.actPwr == SM_ACTPWR_VDC) && (fb->oSeqSts2.all == STS2_COUPLED_STANDBY_COMPLETE))
			||((fb->sysMode.bit.actPwr == SM_ACTPWR_VDC) && (fb->trip.all > TRIP_BAND) && (fb->oSeqSts2.bit.seq0720 == SEQ_COMM_PROCESSING)))
		{
			if((fb->sysSts.bit.convBlkSeq == SS_CONVBLKSEQ_COMPLETE))
			{
				if(fb->t >=SET_TIME_SEQ_DELAY)
				{	
					fb->NextStep = Seq0760;
					fb->seqSts2.bit.seq0770 = SEQ_COMM_COMPLETE;
					fb->flagSeqComplete = FLAG_SEQ_COMPLETE;
					fb->seqSts2.bit.seq0670 = SEQ_COMM_UNPROCESSED;					
					fb->t = 0.0f;			
				}
				else
				{
					fb->NextStep = Seq0770;
					fb->seqSts2.bit.seq0770 = SEQ_COMM_PROCESSING;
					fb->flagSeqComplete = FLAG_SEQ_UNCOMPLETE;	
					fb->t += fb->cycleT;			
				}				
			}
			else
			{
				fb->NextStep = Seq0770;
				fb->seqSts2.bit.seq0770 = SEQ_COMM_PROCESSING;
				fb->flagSeqComplete = FLAG_SEQ_UNCOMPLETE;
			}
		}
		else
		{
			fb->NextStep = Seq0770;
			fb->seqSts2.bit.seq0770 = SEQ_COMM_PROCESSING;
			fb->flagSeqComplete = FLAG_SEQ_UNCOMPLETE;
		}		
	}
	else if(fb->sysMode.bit.seqDir == SM_SEQDIR_NODIR)
	{
		fb->NextStep = Seq0770;
		fb->seqSts2.bit.seq0770 = SEQ_COMM_UNPROCESSED;
		fb->flagSeqComplete = FLAG_SEQ_UNCOMPLETE;
	}
	else
	{
		fb->NextStep = Seq0770;
		fb->seqSts2.bit.seq0770 = SEQ_COMM_UNPROCESSED;
		fb->flagSeqComplete = FLAG_SEQ_UNCOMPLETE;
	}	
}

void funcSeq0800(strFC0740Info *fb)
{
	if(fb->sysMode.bit.seqDir == SM_SEQDIR_START)
	{
		if((fb->sysMode.bit.actPwr == SM_ACTPWR_P) && (fb->sysSts.bit.oRampVdcRated == 1) \
			|| (fb->sysMode.bit.actPwr == SM_ACTPWR_VDC))
		{
			if(fb->t >=SET_TIME_SEQ_DELAY)
			{	
				fb->NextStep = Seq0800;
				fb->seqSts2.bit.seq0800 = SEQ_COMM_COMPLETE;
				fb->flagSeqComplete = FLAG_SEQ_COMPLETE;							
			}
			else
			{
				fb->NextStep = Seq0800;
				fb->seqSts2.bit.seq0800 = SEQ_COMM_PROCESSING;
				fb->flagSeqComplete = FLAG_SEQ_UNCOMPLETE;
				fb->t += fb->cycleT;			
			}			
		}
		else
		{
			fb->NextStep = Seq0800;
			fb->seqSts2.bit.seq0800 = SEQ_COMM_PROCESSING;
			fb->flagSeqComplete = FLAG_SEQ_UNCOMPLETE;
			fb->t = 0.0f;
		}
	}
	else if(fb->sysMode.bit.seqDir == SM_SEQDIR_STOP)
	{
		if(((fb->sysMode.bit.actPwr == SM_ACTPWR_P) && (fb->sysSts.bit.RampPInit == 1) && (fb->oSeqSts2.all == STS2_STOP_EARTHED_COUPLED))\
			|| ((fb->sysMode.bit.actPwr == SM_ACTPWR_VDC) && (fb->sysSts.bit.RampVdcInit == 1) && (fb->oSeqSts2.all == STS2_COUPLED_STANDBY_COMPLETE)))
		{
			if(fb->t >=SET_TIME_SEQ_DELAY)
			{
				fb->NextStep = Seq0770;
				fb->seqSts2.bit.seq0800 = SEQ_COMM_UNPROCESSED;
				fb->flagSeqComplete = FLAG_SEQ_UNCOMPLETE;					
				fb->t = 0.0f;			
			}
			else
			{
				fb->NextStep = Seq0800;
				fb->seqSts2.bit.seq0800 = SEQ_COMM_PROCESSING;
				fb->flagSeqComplete = FLAG_SEQ_UNCOMPLETE;	
				fb->t += fb->cycleT;			
			}
		}
		else
		{
			fb->NextStep = Seq0800;
			fb->seqSts2.bit.seq0800 = SEQ_COMM_PROCESSING;
			fb->flagSeqComplete = FLAG_SEQ_UNCOMPLETE;			
		}

	}
	else if(fb->sysMode.bit.seqDir == SM_SEQDIR_NODIR)
	{
		fb->NextStep = Seq0800;
		fb->seqSts2.bit.seq0800 = SEQ_COMM_UNPROCESSED;
		fb->flagSeqComplete = FLAG_SEQ_UNCOMPLETE;
/*		
		//fault case
		if(fb->sysMode.bit.actPwr == SM_ACTPWR_VDC)
		{
			fb->seqSts2.all = STS2_STOP_EARTHED_COUPLED;
		}		
	*/
	}
	else
	{
		fb->NextStep = Seq0800;
		fb->seqSts2.bit.seq0800 = SEQ_COMM_UNPROCESSED;
		fb->flagSeqComplete = FLAG_SEQ_UNCOMPLETE;
	}
}


/*
		if(((fb->sysMode.bit.actPwr == SM_ACTPWR_P) && (fb->oSeqSts1.all == STS1_EARTHED_STOPPED_COMPLETE)) \
			||((fb->sysMode.bit.actPwr == SM_ACTPWR_VDC) && (fb->oSeqSts1.all == STS1_UNDEFINED_COMPLETE)) )
		{

		}
		else
		{

		}

		if(((fb->sysMode.bit.actPwr == SM_ACTPWR_P) && (fb->oSeqSts1.all == STS1_EARTHED_STANDBY_COMPLETE) && (fb->oSeqSts2.all == STS2_EARTHED_STANDBY_COMPLETE)) \
			||((fb->sysMode.bit.actPwr == SM_ACTPWR_VDC) && (fb->oSeqSts1.all == STS1_EARTHED_STOPPED_COMPLETE)))
		{

		}
		else
		{

		}

		if(((fb->sysMode.bit.actPwr == SM_ACTPWR_P) && (fb->oSeqSts1.all == STS1_EARTHED_STANDBY_COMPLETE) && (fb->oSeqSts2.all == STS2_EARTHED_COUPLED_COMPLETE)) \
			||((fb->sysMode.bit.actPwr == SM_ACTPWR_VDC) && (fb->oSeqSts1.all == STS1_EARTHED_STANDBY_COMPLETE) && (fb->oSeqSts2.all == STS2_EARTHED_STANDBY_COMPLETE)))
		{

		}
		else
		{

		}

///////////////////////////////
		if(((fb->sysMode.bit.actPwr == SM_ACTPWR_P) && (fb->oSeqSts2.all == STS2_EARTHED_COUPLED_COMPLETE)) \
			||((fb->sysMode.bit.actPwr == SM_ACTPWR_VDC) && (fb->oSeqSts2.all == STS2_COUPLED_STANDBY_COMPLETE)) )
		{

		}
		else
		{

		}

		if(((fb->sysMode.bit.actPwr == SM_ACTPWR_P) && (fb->oSeqSts2.all == STS2_COUPLED_STANDBY_COMPLETE)) \
			||((fb->sysMode.bit.actPwr == SM_ACTPWR_VDC) && (fb->oSeqSts2.all == STS2_COUPLED_STOPPED_COMPLETE) &  (fb->oSeqSts1.all == STS1_COUPLED_STOPPED_COMPLETE)))
		{

		}
		else
		{

		}

		if(((fb->sysMode.bit.actPwr == SM_ACTPWR_P) && (fb->oSeqSts2.all == STS2_COUPLED_STOPPED_COMPLETE) && (fb->oSeqSts1.all == STS1_COUPLED_STOPPED_COMPLETE)) \
			||((fb->sysMode.bit.actPwr == SM_ACTPWR_VDC) && (fb->oSeqSts2.all == STS2_COUPLED_EARTHED_COMPLETE) && (fb->oSeqSts1.all == STS1_COUPLED_EARTHED_COMPLETE)))
		{

		}
		else
		{

		}
*/