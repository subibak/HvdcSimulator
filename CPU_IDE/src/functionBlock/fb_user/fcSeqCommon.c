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
}

void funcSeq0200(strFC0740Info *fb)
{
	if(fb->sysMode.bit.seqDir == SM_SEQDIR_START)
	{
		fb->NextStep = Seq0210;
		fb->seqSts1.bit.seq0200 = SEQ_COMM_COMPLETE;
	}
	else if(fb->sysMode.bit.seqDir == SM_SEQDIR_STOP)
	{
		fb->NextStep = Seq0200;
		fb->seqSts1.bit.seq0200 = SEQ_COMM_COMPLETE;
	}
	else if(fb->sysMode.bit.seqDir == SM_SEQDIR_NODIR)
	{
		fb->NextStep = Seq0200;
		fb->seqSts1.bit.seq0200 = SEQ_COMM_UNPROCESSED;
	}
	else
	{
		fb->NextStep = Seq0200;
		fb->seqSts1.bit.seq0200 = SEQ_COMM_UNPROCESSED;
	}
}


void funcSeq0210(strFC0740Info *fb)
{
	if(fb->sysMode.bit.seqDir == SM_SEQDIR_START)
	{
		if(	(fb->sysSts.bit.doorIntLock == SS_DOORINTLOCK_READY) | \
			(fb->sysSts.bit.smSrtCirSwi == SS_SMSHORTSWI_OPEN) )
		{
			fb->NextStep = Seq0220;
			fb->seqSts1.bit.seq0210 = SEQ_COMM_COMPLETE;
		}
		else
		{
			fb->NextStep = Seq0210;
			fb->seqSts1.bit.seq0210 = SEQ_COMM_PROCESSING;
		}
	}
	else if(fb->sysMode.bit.seqDir == SM_SEQDIR_STOP)
	{
		fb->NextStep = Seq0200;
		fb->seqSts1.bit.seq0210 = SEQ_COMM_UNPROCESSED;
	}
	else if(fb->sysMode.bit.seqDir == SM_SEQDIR_NODIR)
	{
		fb->NextStep = Seq0210;
		fb->seqSts1.bit.seq0210 = SEQ_COMM_UNPROCESSED;
	}
	else
	{
		fb->NextStep = Seq0210;
		fb->seqSts1.bit.seq0210 = SEQ_COMM_UNPROCESSED;
	}
}

void funcSeq0220(strFC0740Info *fb)
{
	if(fb->sysMode.bit.seqDir == SM_SEQDIR_START)
	{
		fb->yardSwiComm.bit.esx01_02 = YARD_SWITCH_COMM_OPEN;

		if((fb->yardSwiSts.bit.esx01_02 == YARD_SWITCH_STATUS_OPEN))
		{
			fb->NextStep = Seq0230;
			fb->seqSts1.bit.seq0220 = SEQ_COMM_COMPLETE;
		}
		else
		{
			fb->NextStep = Seq0220;
			fb->seqSts1.bit.seq0220 = SEQ_COMM_PROCESSING;
		}
	}
	else if(fb->sysMode.bit.seqDir == SM_SEQDIR_STOP)
	{
		fb->NextStep = Seq0320;
		fb->seqSts1.bit.seq0220 = SEQ_COMM_UNPROCESSED;
	}
	else if(fb->sysMode.bit.seqDir == SM_SEQDIR_NODIR)
	{
		fb->NextStep = Seq0220;
		fb->seqSts1.bit.seq0220 = SEQ_COMM_UNPROCESSED;	
	}
	else
	{
		fb->NextStep = Seq0220;
		fb->seqSts1.bit.seq0220 = SEQ_COMM_UNPROCESSED;	
	}
}

void funcSeq0230(strFC0740Info *fb)
{
	if(fb->sysMode.bit.seqDir == SM_SEQDIR_START)
	{
		fb->yardSwiComm.bit.esx11_21 = YARD_SWITCH_COMM_OPEN;

		if((fb->yardSwiSts.bit.esx11_21 == YARD_SWITCH_STATUS_OPEN))
		{
			fb->NextStep = Seq0240;
			fb->seqSts1.bit.seq0230 = SEQ_COMM_COMPLETE;
		}
		else
		{
			fb->NextStep = Seq0230;
			fb->seqSts1.bit.seq0230 = SEQ_COMM_PROCESSING;

		}
	}
	else if(fb->sysMode.bit.seqDir == SM_SEQDIR_STOP)
	{
		fb->NextStep = Seq0330;
		fb->seqSts1.bit.seq0230 = SEQ_COMM_UNPROCESSED;
	}
	else if(fb->sysMode.bit.seqDir == SM_SEQDIR_NODIR)
	{
		fb->NextStep = Seq0230;
		fb->seqSts1.bit.seq0230 = SEQ_COMM_UNPROCESSED;
	}
	else
	{
		fb->NextStep = Seq0230;
		fb->seqSts1.bit.seq0230 = SEQ_COMM_UNPROCESSED;
	}	
}


void funcSeq0240(strFC0740Info *fb)
{
	if(fb->sysMode.bit.seqDir == SM_SEQDIR_START)
	{
		fb->yardSwiComm.bit.esx12_22 = YARD_SWITCH_COMM_OPEN;

		if((fb->yardSwiSts.bit.esx12_22 == YARD_SWITCH_STATUS_OPEN))
		{
			fb->NextStep = Seq0250;
			fb->seqSts1.bit.seq0240 = SEQ_COMM_COMPLETE;
		}
		else
		{
			fb->NextStep = Seq0240;
			fb->seqSts1.bit.seq0240 = SEQ_COMM_PROCESSING;
		}
	}
	else if(fb->sysMode.bit.seqDir == SM_SEQDIR_STOP)
	{
		fb->NextStep = Seq0340;
		fb->seqSts1.bit.seq0240 = SEQ_COMM_UNPROCESSED;
	}
	else if(fb->sysMode.bit.seqDir == SM_SEQDIR_NODIR)
	{
		fb->NextStep = Seq0240;
		fb->seqSts1.bit.seq0240 = SEQ_COMM_UNPROCESSED;
	}
	else
	{
		fb->NextStep = Seq0240;
		fb->seqSts1.bit.seq0240 = SEQ_COMM_UNPROCESSED;
	}		
}

void funcSeq0250(strFC0740Info *fb)
{
	if(fb->sysMode.bit.seqDir == SM_SEQDIR_START)
	{
		//Zig-Zag TR contributes to the system in Vdc mode
		if(fb->sysMode.bit.actPwr == SM_ACTPWR_VDC)
		{
			fb->yardSwiComm.bit.dsx01 = YARD_SWITCH_COMM_CLOSE;

			if((fb->yardSwiSts.bit.dsx01 == YARD_SWITCH_STATUS_CLOSE))
			{
				fb->NextStep = Seq0260;
				fb->seqSts1.bit.seq0250 = SEQ_COMM_COMPLETE;
			}
			else
			{
				fb->NextStep = Seq0250;			
				fb->seqSts1.bit.seq0250 = SEQ_COMM_PROCESSING;
			}
		}
		else
		{
			fb->NextStep = Seq0260;
			fb->seqSts1.bit.seq0250 = SEQ_COMM_PROCESSING;
		}

	}
	else if(fb->sysMode.bit.seqDir == SM_SEQDIR_STOP)
	{
		fb->NextStep = Seq0350;
		fb->seqSts1.bit.seq0250 = SEQ_COMM_UNPROCESSED;
	}
	else if(fb->sysMode.bit.seqDir == SM_SEQDIR_NODIR)
	{
		fb->NextStep = Seq0250;
		fb->seqSts1.bit.seq0250 = SEQ_COMM_UNPROCESSED;
	}
	else
	{
		fb->NextStep = Seq0250;
		fb->seqSts1.bit.seq0250 = SEQ_COMM_UNPROCESSED;
	}	
}
void funcSeq0260(strFC0740Info *fb)
{
	if(fb->sysMode.bit.seqDir == SM_SEQDIR_START)
	{
		fb->yardSwiComm.bit.dsx02 = YARD_SWITCH_COMM_CLOSE;

		if((fb->yardSwiSts.bit.dsx02 == YARD_SWITCH_STATUS_CLOSE))
		{
			fb->NextStep = Seq0400;
			fb->seqSts1.bit.seq0260 = SEQ_COMM_COMPLETE;
		}
		else
		{
			fb->NextStep = Seq0260;
			fb->seqSts1.bit.seq0260 = SEQ_COMM_PROCESSING;
		}
	}
	else if(fb->sysMode.bit.seqDir == SM_SEQDIR_STOP)
	{
		fb->NextStep = Seq0360;
		fb->seqSts1.bit.seq0260 = SEQ_COMM_UNPROCESSED;
	}
	else if(fb->sysMode.bit.seqDir == SM_SEQDIR_NODIR)
	{
		fb->NextStep = Seq0260;
		fb->seqSts1.bit.seq0260 = SEQ_COMM_UNPROCESSED;
	}
	else
	{
		fb->NextStep = Seq0260;
		fb->seqSts1.bit.seq0260 = SEQ_COMM_UNPROCESSED;
	}	
}

void funcSeq0320(strFC0740Info *fb)
{
	if(fb->sysMode.bit.seqDir == SM_SEQDIR_START)
	{
		fb->NextStep = Seq0320;
		fb->seqSts1.bit.seq0320 = SEQ_COMM_UNPROCESSED;
	}
	else if(fb->sysMode.bit.seqDir == SM_SEQDIR_STOP)
	{
		fb->yardSwiComm.bit.esx01_02 = YARD_SWITCH_COMM_CLOSE;

		if((fb->yardSwiSts.bit.esx01_02 == YARD_SWITCH_STATUS_CLOSE))
		{
			fb->NextStep = Seq0200;
			fb->seqSts1.bit.seq0320 = SEQ_COMM_COMPLETE;
		}
		else
		{
			fb->NextStep = Seq0320;
			fb->seqSts1.bit.seq0320 = SEQ_COMM_PROCESSING;
		}
	}
	else if(fb->sysMode.bit.seqDir == SM_SEQDIR_NODIR)
	{
		fb->NextStep = Seq0320;
		fb->seqSts1.bit.seq0320 = SEQ_COMM_UNPROCESSED;
	}
	else
	{
		fb->NextStep = Seq0320;
		fb->seqSts1.bit.seq0320 = SEQ_COMM_UNPROCESSED;
	}		
}

void funcSeq0330(strFC0740Info *fb)
{
	if(fb->sysMode.bit.seqDir == SM_SEQDIR_START)
	{
		fb->NextStep = Seq0330;
		fb->seqSts1.bit.seq0330 = SEQ_COMM_UNPROCESSED;
	}
	else if(fb->sysMode.bit.seqDir == SM_SEQDIR_STOP)
	{
		fb->yardSwiComm.bit.esx11_21 = YARD_SWITCH_COMM_CLOSE;

		if((fb->yardSwiSts.bit.esx11_21 == YARD_SWITCH_STATUS_CLOSE))
		{
			fb->NextStep = Seq0320;
			fb->seqSts1.bit.seq0330 = SEQ_COMM_COMPLETE;
		}
		else
		{
			fb->NextStep = Seq0330;
			fb->seqSts1.bit.seq0330 = SEQ_COMM_PROCESSING;
		}
	}
	else if(fb->sysMode.bit.seqDir == SM_SEQDIR_NODIR)
	{
		fb->NextStep = Seq0330;
		fb->seqSts1.bit.seq0330 = SEQ_COMM_UNPROCESSED;
	}
	else
	{
		fb->NextStep = Seq0330;
		fb->seqSts1.bit.seq0330 = SEQ_COMM_UNPROCESSED;
	}	
}

void funcSeq0340(strFC0740Info *fb)
{
	if(fb->sysMode.bit.seqDir == SM_SEQDIR_START)
	{
		fb->NextStep = Seq0340;
		fb->seqSts1.bit.seq0340 = SEQ_COMM_UNPROCESSED;
	}
	else if(fb->sysMode.bit.seqDir == SM_SEQDIR_STOP)
	{
		fb->yardSwiComm.bit.esx12_22 = YARD_SWITCH_COMM_CLOSE;

		if((fb->yardSwiSts.bit.esx12_22 == YARD_SWITCH_STATUS_CLOSE))
		{
			fb->NextStep = Seq0330;
			fb->seqSts1.bit.seq0340 = SEQ_COMM_COMPLETE;
		}
		else
		{
			fb->NextStep = Seq0340;
			fb->seqSts1.bit.seq0340 = SEQ_COMM_PROCESSING;
		}
	}
	else if(fb->sysMode.bit.seqDir == SM_SEQDIR_NODIR)
	{
		fb->NextStep = Seq0340;
		fb->seqSts1.bit.seq0340 = SEQ_COMM_UNPROCESSED;
	}
	else
	{
		fb->NextStep = Seq0340;
		fb->seqSts1.bit.seq0340 = SEQ_COMM_UNPROCESSED;
	}	
}

void funcSeq0350(strFC0740Info *fb)
{
	if(fb->sysMode.bit.seqDir == SM_SEQDIR_START)
	{
		fb->NextStep = Seq0350;
		fb->seqSts1.bit.seq0350 = SEQ_COMM_UNPROCESSED;
	}
	else if(fb->sysMode.bit.seqDir == SM_SEQDIR_STOP)
	{
		//Zig-Zag TR contributes to the system in Vdc mode
		if(fb->sysMode.bit.actPwr == SM_ACTPWR_VDC)
		{
			fb->yardSwiComm.bit.dsx01 = YARD_SWITCH_COMM_OPEN;

			if((fb->yardSwiSts.bit.dsx01 == YARD_SWITCH_STATUS_OPEN))
			{
				fb->NextStep = Seq0340;
				fb->seqSts1.bit.seq0350 = SEQ_COMM_COMPLETE;
			}
			else
			{
				fb->NextStep = Seq0350;
				fb->seqSts1.bit.seq0350 = SEQ_COMM_PROCESSING;
			}
		}
		else
			fb->NextStep = Seq0340;
	}
	else if(fb->sysMode.bit.seqDir == SM_SEQDIR_NODIR)
	{
		fb->NextStep = Seq0350;
		fb->seqSts1.bit.seq0350 = SEQ_COMM_UNPROCESSED;
	}
	else
	{
		fb->NextStep = Seq0350;
		fb->seqSts1.bit.seq0350 = SEQ_COMM_UNPROCESSED;
	}	
}

void funcSeq0360(strFC0740Info *fb)
{
	if(fb->sysMode.bit.seqDir == SM_SEQDIR_START)
	{
		fb->NextStep = Seq0360;
		fb->seqSts1.bit.seq0360 = SEQ_COMM_UNPROCESSED;
	}
	else if(fb->sysMode.bit.seqDir == SM_SEQDIR_STOP)
	{
		fb->yardSwiComm.bit.dsx02 = YARD_SWITCH_COMM_OPEN;

		if((fb->yardSwiSts.bit.dsx02 == YARD_SWITCH_STATUS_OPEN))
		{
			fb->NextStep = Seq0350;
			fb->seqSts1.bit.seq0360 = SEQ_COMM_COMPLETE;
		}
		else
		{
			fb->NextStep = Seq0360;
			fb->seqSts1.bit.seq0360 = SEQ_COMM_PROCESSING;
		}
	}
	else if(fb->sysMode.bit.seqDir == SM_SEQDIR_NODIR)
	{
		fb->NextStep = Seq0360;
		fb->seqSts1.bit.seq0360 = SEQ_COMM_UNPROCESSED;
	}
	else
	{
		fb->NextStep = Seq0360;
		fb->seqSts1.bit.seq0360 = SEQ_COMM_UNPROCESSED;
	}	
}

void funcSeq0400(strFC0740Info *fb)
{
	if(fb->sysMode.bit.seqDir == SM_SEQDIR_START)
	{
		fb->NextStep = Seq0410;
		fb->seqSts1.bit.seq0400 = SEQ_COMM_COMPLETE;
	}
	else if(fb->sysMode.bit.seqDir == SM_SEQDIR_STOP)
	{
		fb->NextStep = Seq0360;
		fb->seqSts1.bit.seq0400 = SEQ_COMM_COMPLETE;

	}
	else if(fb->sysMode.bit.seqDir == SM_SEQDIR_NODIR)
	{
		fb->NextStep = Seq0400;
		fb->seqSts1.bit.seq0400 = SEQ_COMM_UNPROCESSED;
	}
	else
	{
		fb->NextStep = Seq0400;
		fb->seqSts1.bit.seq0400 = SEQ_COMM_UNPROCESSED;
	}
}

void funcSeq0410(strFC0740Info *fb)
{
	if(fb->sysMode.bit.seqDir == SM_SEQDIR_START)
	{
		fb->yardSwiComm.bit.ds3x1 = YARD_SWITCH_COMM_CLOSE;

		if((fb->yardSwiSts.bit.ds3x1 == YARD_SWITCH_STATUS_CLOSE))
		{
			fb->NextStep = Seq0600;
			fb->seqSts1.bit.seq0410 = SEQ_COMM_COMPLETE;
		}
		else
		{
			fb->NextStep = Seq0410;
			fb->seqSts1.bit.seq0410 = SEQ_COMM_PROCESSING;
		}
	}
	else if(fb->sysMode.bit.seqDir == SM_SEQDIR_STOP)
	{
		fb->NextStep = Seq0510;
		fb->seqSts1.bit.seq0410 = SEQ_COMM_UNPROCESSED;
	}
	else if(fb->sysMode.bit.seqDir == SM_SEQDIR_NODIR)
	{
		fb->NextStep = Seq0410;
		fb->seqSts1.bit.seq0410 = SEQ_COMM_UNPROCESSED;
	}
	else
	{
		fb->NextStep = Seq0410;
		fb->seqSts1.bit.seq0410 = SEQ_COMM_UNPROCESSED;
	}
}

void funcSeq0510(strFC0740Info *fb)
{
	if(fb->sysMode.bit.seqDir == SM_SEQDIR_START)
	{
		fb->NextStep = Seq0510;
		fb->seqSts1.bit.seq0510 = SEQ_COMM_UNPROCESSED;
	}
	else if(fb->sysMode.bit.seqDir == SM_SEQDIR_STOP)
	{
		fb->yardSwiComm.bit.ds3x1 = YARD_SWITCH_COMM_OPEN;

		if((fb->yardSwiSts.bit.ds3x1 == YARD_SWITCH_STATUS_OPEN))
		{
			fb->NextStep = Seq0400;
			fb->seqSts1.bit.seq0510 = SEQ_COMM_COMPLETE;
		}
		else
		{
			fb->NextStep = Seq0510;
			fb->seqSts1.bit.seq0510 = SEQ_COMM_PROCESSING;
		}
	}
	else if(fb->sysMode.bit.seqDir == SM_SEQDIR_NODIR)
	{
		fb->NextStep = Seq0510;
		fb->seqSts1.bit.seq0510 = SEQ_COMM_UNPROCESSED;
	}
	else
	{
		fb->NextStep = Seq0510;
		fb->seqSts1.bit.seq0510 = SEQ_COMM_UNPROCESSED;
	}	
}

void funcSeq0520(strFC0740Info *fb)
{
	if(fb->sysMode.bit.seqDir == SM_SEQDIR_START)
	{
		fb->NextStep = Seq0600;
		fb->seqSts1.bit.seq0520 = SEQ_COMM_UNPROCESSED;
	}
	else if(fb->sysMode.bit.seqDir == SM_SEQDIR_STOP)
	{
		if((fb->sysSts.bit.convDischg == SS_CONVDISCHG_EXPIRED))
		{
			fb->NextStep = Seq0510;
			fb->seqSts1.bit.seq0520 = SEQ_COMM_PROCESSING;
		}
		else
		{
			fb->NextStep = Seq0520;
			fb->seqSts1.bit.seq0520 = SEQ_COMM_COMPLETE;
		}
	}
	else if(fb->sysMode.bit.seqDir == SM_SEQDIR_NODIR)
	{
		fb->NextStep = Seq0520;
		fb->seqSts1.bit.seq0520 = SEQ_COMM_UNPROCESSED;
	}
	else
	{
		fb->NextStep = Seq0520;
		fb->seqSts1.bit.seq0520 = SEQ_COMM_UNPROCESSED;
	}	
}

void funcSeq0600(strFC0740Info *fb)
{
	if(fb->sysMode.bit.seqDir == SM_SEQDIR_START)
	{
		fb->NextStep = Seq0610;
		fb->seqSts1.bit.seq0600 = SEQ_COMM_COMPLETE;
	}
	else if(fb->sysMode.bit.seqDir == SM_SEQDIR_STOP)
	{
		fb->NextStep = Seq0520;
		fb->seqSts1.bit.seq0600 = SEQ_COMM_UNPROCESSED;
	}
	else if(fb->sysMode.bit.seqDir == SM_SEQDIR_NODIR)
	{
		fb->NextStep = Seq0600;
		fb->seqSts1.bit.seq0600 = SEQ_COMM_UNPROCESSED;
	}
	else
	{
		fb->NextStep = Seq0600;
		fb->seqSts1.bit.seq0600 = SEQ_COMM_UNPROCESSED;
	}
}

void funcSeq0610(strFC0740Info *fb)
{
	uint32 subSts;
	uint32 intLockCon;
	uint32 condition;

	subSts 		= 0x00000003 & (fb->sysSts.bit.trCooling | fb->sysSts.bit.trCooling | fb->sysSts.bit.cpSys);

	//Zig-Zag TR contributes to the system in Vdc mode
	if(fb->sysMode.bit.actPwr == SM_ACTPWR_VDC)
	{
		intLockCon 	= 0x00000001 & (fb->yardSwiSts.bit.cbx91 | fb->yardSwiSts.bit.cbx01 | fb->yardSwiSts.bit.esx12_22 | fb->yardSwiSts.bit.dsx01);
		intLockCon 	= 0x00000003 & intLockCon & (fb->sysSts.bit.tapChgPos | fb->sysSts.bit.noEmy | fb->sysSts.bit.convDischg);
	}
	else
	{
		intLockCon 	= 0x00000001 & (fb->yardSwiSts.bit.cbx91 | fb->yardSwiSts.bit.cbx01 | fb->yardSwiSts.bit.esx12_22);
		intLockCon 	= 0x00000003 & intLockCon & (fb->sysSts.bit.tapChgPos | fb->sysSts.bit.noEmy | fb->sysSts.bit.convDischg);	
	}

	condition 	= subSts | intLockCon;

	if(fb->sysMode.bit.seqDir == SM_SEQDIR_START)
	{
		if((condition == SS_INTLCK_COMPLETE))
		{
			fb->NextStep = Seq0620;
			fb->seqSts1.bit.seq0610 = SEQ_COMM_COMPLETE;
		}
		else
		{
			fb->NextStep = Seq0610;
			fb->seqSts1.bit.seq0610 = SEQ_COMM_PROCESSING;
		}
	}
	else if(fb->sysMode.bit.seqDir == SM_SEQDIR_STOP)
	{
		fb->NextStep = Seq0600;
		fb->seqSts1.bit.seq0610 = SEQ_COMM_UNPROCESSED;
	}
	else if(fb->sysMode.bit.seqDir == SM_SEQDIR_NODIR)
	{
		fb->NextStep = Seq0610;
		fb->seqSts1.bit.seq0610 = SEQ_COMM_UNPROCESSED;
	}
	else
	{
		fb->NextStep = Seq0610;
		fb->seqSts1.bit.seq0610 = SEQ_COMM_UNPROCESSED;
	}
}

void funcSeq0620(strFC0740Info *fb)
{
	if(fb->sysMode.bit.seqDir == SM_SEQDIR_START)
	{
		fb->yardSwiComm.bit.dsx02 = YARD_SWITCH_COMM_OPEN;

		if((fb->yardSwiSts.bit.dsx02 == YARD_SWITCH_STATUS_OPEN))
		{
			fb->NextStep = Seq0630;
			fb->seqSts1.bit.seq0620 = SEQ_COMM_COMPLETE;
		}
		else
		{
			fb->NextStep = Seq0620;
			fb->seqSts1.bit.seq0620 = SEQ_COMM_PROCESSING;
		}
	}
	else if(fb->sysMode.bit.seqDir == SM_SEQDIR_STOP)
	{
		fb->NextStep = Seq0720;
		fb->seqSts1.bit.seq0620 = SEQ_COMM_UNPROCESSED;
	}
	else if(fb->sysMode.bit.seqDir == SM_SEQDIR_NODIR)
	{
		fb->NextStep = Seq0620;
		fb->seqSts1.bit.seq0620 = SEQ_COMM_UNPROCESSED;
	}
	else
	{
		fb->NextStep = Seq0620;
		fb->seqSts1.bit.seq0620 = SEQ_COMM_UNPROCESSED;
	}
}

void funcSeq0630(strFC0740Info *fb)
{
	if(fb->sysMode.bit.seqDir == SM_SEQDIR_START)
	{
		if((fb->yardSwiSts.bit.cbx91 == YARD_SWITCH_STATUS_CLOSE))
		{
			fb->NextStep = Seq0640;
			fb->seqSts1.bit.seq0630 = SEQ_COMM_COMPLETE;
		}
		else
		{
			fb->NextStep = Seq0630;
			fb->seqSts1.bit.seq0630 = SEQ_COMM_PROCESSING;
		}
	}
	else if(fb->sysMode.bit.seqDir == SM_SEQDIR_STOP)
	{
		fb->NextStep = Seq0730;
		fb->seqSts1.bit.seq0630 = SEQ_COMM_UNPROCESSED;
	}
	else if(fb->sysMode.bit.seqDir == SM_SEQDIR_NODIR)
	{
		fb->NextStep = Seq0630;
		fb->seqSts1.bit.seq0630 = SEQ_COMM_UNPROCESSED;
	}
	else
	{
		fb->NextStep = Seq0630;
		fb->seqSts1.bit.seq0630 = SEQ_COMM_UNPROCESSED;
	}
}

void funcSeq0640(strFC0740Info *fb)
{
	if(fb->sysMode.bit.seqDir == SM_SEQDIR_START)
	{
		fb->yardSwiComm.bit.cbx01 = YARD_SWITCH_COMM_CLOSE;

		if((fb->yardSwiSts.bit.cbx01 == YARD_SWITCH_STATUS_CLOSE))
		{
			fb->NextStep = Seq0650;
			fb->seqSts1.bit.seq0640 = SEQ_COMM_COMPLETE;
		}
		else
		{
			fb->NextStep = Seq0640;
			fb->seqSts1.bit.seq0640 = SEQ_COMM_PROCESSING;
		}
	}
	else if(fb->sysMode.bit.seqDir == SM_SEQDIR_STOP)
	{
		fb->NextStep = Seq0740;
		fb->seqSts1.bit.seq0640 = SEQ_COMM_UNPROCESSED;
	}
	else if(fb->sysMode.bit.seqDir == SM_SEQDIR_NODIR)
	{
		fb->NextStep = Seq0640;
		fb->seqSts1.bit.seq0640 = SEQ_COMM_UNPROCESSED;
	}
	else
	{
		fb->NextStep = Seq0640;
		fb->seqSts1.bit.seq0640 = SEQ_COMM_UNPROCESSED;
	}
}


void funcSeq0650(strFC0740Info *fb)
{
	if(fb->sysMode.bit.seqDir == SM_SEQDIR_START)
	{
		if((fb->sysSts.bit.passiveChg == SS_PASSIVECHG_COMPLETE))
		{
			fb->NextStep = Seq0660;
			fb->seqSts1.bit.seq0650 = SEQ_COMM_COMPLETE;
		}
		else
		{
			fb->NextStep = Seq0650;
			fb->seqSts1.bit.seq0650 = SEQ_COMM_PROCESSING;
		}
	}
	else if(fb->sysMode.bit.seqDir == SM_SEQDIR_STOP)
	{
		fb->NextStep = Seq0740;
		fb->seqSts1.bit.seq0650 = SEQ_COMM_UNPROCESSED;
	}
	else if(fb->sysMode.bit.seqDir == SM_SEQDIR_NODIR)
	{
		fb->NextStep = Seq0650;
		fb->seqSts1.bit.seq0650 = SEQ_COMM_UNPROCESSED;
	}
	else
	{
		fb->NextStep = Seq0650;
		fb->seqSts1.bit.seq0650 = SEQ_COMM_UNPROCESSED;
	}
}

void funcSeq0660(strFC0740Info *fb)
{
	if(fb->sysMode.bit.seqDir == SM_SEQDIR_START)
	{
		fb->yardSwiComm.bit.dsx02 = YARD_SWITCH_COMM_CLOSE;

		if((fb->yardSwiSts.bit.dsx02 == YARD_SWITCH_STATUS_CLOSE))
		{
			fb->NextStep = Seq0670;
			fb->seqSts1.bit.seq0660 = SEQ_COMM_COMPLETE;
		}
		else
		{
			fb->NextStep = Seq0660;
			fb->seqSts1.bit.seq0660 = SEQ_COMM_PROCESSING;
	}
	else if(fb->sysMode.bit.seqDir == SM_SEQDIR_STOP)
	{
		fb->NextStep = Seq0760;
		fb->seqSts1.bit.seq0660 = SEQ_COMM_UNPROCESSED;
	}
	else if(fb->sysMode.bit.seqDir == SM_SEQDIR_NODIR)
	{
		fb->NextStep = Seq0660;
		fb->seqSts1.bit.seq0660 = SEQ_COMM_UNPROCESSED;
	}
	else
	{
		fb->NextStep = Seq0660;
		fb->seqSts1.bit.seq0660 = SEQ_COMM_UNPROCESSED;
	}
}

void funcSeq0670(strFC0740Info *fb)
{
	if(fb->sysMode.bit.seqDir == SM_SEQDIR_START)
	{
		if((fb->sysSts.bit.convActchgSeq == SS_ACTCHGSEQ_COMPLETE))
		{
			fb->NextStep = Seq0800;
			fb->seqSts1.bit.seq0670 = SEQ_COMM_COMPLETE;
		}
		else
		{
			fb->NextStep = Seq0670;
			fb->seqSts1.bit.seq0670 = SEQ_COMM_PROCESSING;
		}
	}
	else if(fb->sysMode.bit.seqDir == SM_SEQDIR_STOP)
	{
		fb->NextStep = Seq0770;
		fb->seqSts1.bit.seq0670 = SEQ_COMM_UNPROCESSED;
	}
	else if(fb->sysMode.bit.seqDir == SM_SEQDIR_NODIR)
	{
		fb->NextStep = Seq0670;
		fb->seqSts1.bit.seq0670 = SEQ_COMM_UNPROCESSED;
	}
	else
	{
		fb->NextStep = Seq0670;
		fb->seqSts1.bit.seq0670 = SEQ_COMM_UNPROCESSED;
	}
}

void funcSeq0720(strFC0740Info *fb)
{
	if(fb->sysMode.bit.seqDir == SM_SEQDIR_START)
	{
		fb->NextStep = Seq0720;
		fb->seqSts1.bit.seq0720 = SEQ_COMM_UNPROCESSED;
	}
	else if(fb->sysMode.bit.seqDir == SM_SEQDIR_STOP)
	{
		fb->yardSwiComm.bit.dsx02 = YARD_SWITCH_COMM_CLOSE;

		if((fb->yardSwiSts.bit.ds3x1 == YARD_SWITCH_STATUS_CLOSE))
		{
			fb->NextStep = Seq0600;
			fb->seqSts1.bit.seq0720 = SEQ_COMM_COMPLETE;
		}
		else
		{
			fb->NextStep = Seq0720;
			fb->seqSts1.bit.seq0720 = SEQ_COMM_PROCESSING;
		}
	}
	else if(fb->sysMode.bit.seqDir == SM_SEQDIR_NODIR)
	{
		fb->NextStep = Seq0720;
		fb->seqSts1.bit.seq0720 = SEQ_COMM_UNPROCESSED;
	}
	else
	{
		fb->NextStep = Seq0720;
		fb->seqSts1.bit.seq0720 = SEQ_COMM_UNPROCESSED;
	}	
}

void funcSeq0730(strFC0740Info *fb)
{
	if(fb->sysMode.bit.seqDir == SM_SEQDIR_START)
	{
		fb->NextStep = Seq0730;
		fb->seqSts1.bit.seq0730 = SEQ_COMM_UNPROCESSED;
	}
	else if(fb->sysMode.bit.seqDir == SM_SEQDIR_STOP)
	{
		if((fb->yardSwiSts.bit.cbx91 == YARD_SWITCH_STATUS_OPEN))
		{
			fb->NextStep = Seq0720;
			fb->seqSts1.bit.seq0730 = SEQ_COMM_COMPLETE;
		}
		else
		{			
			fb->NextStep = Seq0730;
			fb->seqSts1.bit.seq0730 = SEQ_COMM_PROCESSING;
		}
	}
	else if(fb->sysMode.bit.seqDir == SM_SEQDIR_NODIR)
	{
		fb->NextStep = Seq0730;
		fb->seqSts1.bit.seq0730 = SEQ_COMM_UNPROCESSED;
	}
	else
	{
		fb->NextStep = Seq0730;
		fb->seqSts1.bit.seq0730 = SEQ_COMM_UNPROCESSED;
	}	
}

void funcSeq0740(strFC0740Info *fb)
{
	if(fb->sysMode.bit.seqDir == SM_SEQDIR_START)
	{
		fb->NextStep = Seq0740;
		fb->seqSts1.bit.seq0740 = SEQ_COMM_UNPROCESSED;
	}
	else if(fb->sysMode.bit.seqDir == SM_SEQDIR_STOP)
	{
		fb->yardSwiComm.bit.cbx01 = YARD_SWITCH_COMM_OPEN;		

		if((fb->yardSwiSts.bit.cbx01 == YARD_SWITCH_STATUS_OPEN))
		{
			fb->NextStep = Seq0730;
			fb->seqSts1.bit.seq0740 = SEQ_COMM_COMPLETE;
		}
		else
		{
			fb->NextStep = Seq0740;
			fb->seqSts1.bit.seq0740 = SEQ_COMM_PROCESSING;
		}
	}
	else if(fb->sysMode.bit.seqDir == SM_SEQDIR_NODIR)
	{
		fb->NextStep = Seq0740;
		fb->seqSts1.bit.seq0740 = SEQ_COMM_UNPROCESSED;
	}
	else
	{
		fb->NextStep = Seq0740;
		fb->seqSts1.bit.seq0740 = SEQ_COMM_UNPROCESSED;
	}	
}

void funcSeq0760(strFC0740Info *fb)
{
	if(fb->sysMode.bit.seqDir == SM_SEQDIR_START)
	{
		fb->NextStep = Seq0760;
		fb->seqSts1.bit.seq0760 = SEQ_COMM_UNPROCESSED; 
	}
	else if(fb->sysMode.bit.seqDir == SM_SEQDIR_STOP)
	{
		fb->yardSwiComm.bit.dsx02 = YARD_SWITCH_COMM_OPEN;		

		if((fb->yardSwiSts.bit.dsx02 == YARD_SWITCH_STATUS_OPEN))
		{
			fb->NextStep = Seq0740;
			fb->seqSts1.bit.seq0760 = SEQ_COMM_COMPLETE; 
		}
		else
		{
			fb->NextStep = Seq0760;
			fb->seqSts1.bit.seq0760 = SEQ_COMM_PROCESSING; 
		}
	}
	else if(fb->sysMode.bit.seqDir == SM_SEQDIR_NODIR)
	{
		fb->NextStep = Seq0760;
		fb->seqSts1.bit.seq0760 = SEQ_COMM_UNPROCESSED; 
	}
	else
	{
		fb->NextStep = Seq0760;
		fb->seqSts1.bit.seq0760 = SEQ_COMM_UNPROCESSED; 
	}	
}

void funcSeq0770(strFC0740Info *fb)
{
	if(fb->sysMode.bit.seqDir == SM_SEQDIR_START)
	{
		fb->NextStep = Seq0770;
		fb->seqSts1.bit.seq0770 = SEQ_COMM_UNPROCESSED;
	}
	else if(fb->sysMode.bit.seqDir == SM_SEQDIR_STOP)
	{
		if((fb->sysSts.bit.convBlkSeq == SS_CONVBLKSEQ_COMPLETE))
		{
			fb->NextStep = Seq0760;
			fb->seqSts1.bit.seq0770 = SEQ_COMM_COMPLETE;
		}
		else
		{
			fb->NextStep = Seq0770;
			fb->seqSts1.bit.seq0770 = SEQ_COMM_PROCESSING;
		}
	}
	else if(fb->sysMode.bit.seqDir == SM_SEQDIR_NODIR)
	{
		fb->NextStep = Seq0770;
		fb->seqSts1.bit.seq0770 = SEQ_COMM_UNPROCESSED;
	}
	else
	{
		fb->NextStep = Seq0770;
		fb->seqSts1.bit.seq0770 = SEQ_COMM_UNPROCESSED;
	}	
}

void funcSeq0800(strFC0740Info *fb)
{
	if(fb->sysMode.bit.seqDir == SM_SEQDIR_START)
	{
		fb->NextStep = Seq0800;
		fb->seqSts1.bit.seq0800 = SEQ_COMM_COMPLETE;
	}
	else if(fb->sysMode.bit.seqDir == SM_SEQDIR_STOP)
	{
		fb->NextStep = Seq0770;
		fb->seqSts1.bit.seq0800 = SEQ_COMM_UNPROCESSED;
	}
	else if(fb->sysMode.bit.seqDir == SM_SEQDIR_NODIR)
	{
		fb->NextStep = Seq0800;
		fb->seqSts1.bit.seq0800 = SEQ_COMM_UNPROCESSED;
	}
	else
	{
		fb->NextStep = Seq0800;
		fb->seqSts1.bit.seq0800 = SEQ_COMM_UNPROCESSED;
	}
}