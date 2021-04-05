/*
 ******************************************************************************
 *              LK11 MHCAP Project
 *
 *              (c) Copyright 2019 ~ 2020 LK11 Co., Ltd.
 *
 * All rights reserved.  Protected by international copyright laws.
 * Knowledge of the source code may NOT be used to develop a similar product.
 * Please help us continue to provide the Embedded community with the finest
 * software available.  Your honesty is greatly appreciated.
 ******************************************************************************
 */

/*
 ******************************************************************************
 *
 *                  NXP QuoIQ P2020
 *                     on the
 *                  LK11's LKV-120 VME Master Board
 *
 * Filename      : link_lib.c
 * Version       : V1.00
 * Programmer(s) : shjung
 ******************************************************************************
 */

/*
 ******************************************************************************
 REVISION HISTORY :
 ------------------------------------------------------------------------------
 Date          Ver   Name                     Description
 ------------------------------------------------------------------------------
 21/Oct/2019   1.0   SeungHo Jeong (shjung)   Modified from LKV-440(Phase Control Board)
 ******************************************************************************
 */

/*
 ******************************************************************************
 *                             INCLUDE FILES
 ******************************************************************************
 */

#include "project_def.h"

int linkTxReady (int lp_inx);

/*
 ******************************************************************************
 *                             DEFINES
 ******************************************************************************
 */

#define MK_HEADER_DATA(seq_no, len)		(seq_no | (len << 16))
#define GET_SEQ_NO_DATA(h_data)			(h_data & 0xFFFF)
#define GET_LEN_DATA(h_data)			((h_data & 0xFFFF0000) >> 16)
	

/*
 ******************************************************************************
 *                             GLOBAL VARIABLES
 ******************************************************************************
 */

LOCAL uint	local_tx_pack_buf[GTX_TX_MAX_SIZE]; // TX를 위한 임시 버퍼
LOCAL uint	local_rx_pack_buf[GTX_TX_MAX_SIZE]; // RX를 위한 임시 버퍼

SWI_GT_PORT		*gtx_map_data[MAX_LINK_CNT];
LP_ENV_DATA		lp_env_data[MAX_LINK_CNT];


/*
 ******************************************************************************
 *							   FUNCTIONS
 ******************************************************************************
 */

void linkEnable (int lp_inx, int onoff)
{
	int		lp_index		= lp_inx;

	if (lp_inx < 0 || lp_inx > LP_1)	return;
	
	if (onoff == ON) {
		/* Reset 제거 */
		Lkp502SetGtxRstCtrl (lp_inx, FALSE);
		
		/* Tx Interrupt는 사용하지 않음 */
		Lkp502SetTxInt (lp_inx, FALSE);

		/* Rx Interrupt는 사용하지 않음 */
		Lkp502SetRxInt (lp_inx, FALSE);

		/* Tx & Rx 전송 버퍼 크기 결정 */
		Lkp502SetTxSize (lp_inx, TX_1B_MAX_SIZE);
		
		/* Reset 제거 */
		Lkp502SetSfpTxCtrl (lp_inx, TRUE);
	} else {
		/* Reset 설정 */
		Lkp502SetGtxRstCtrl (lp_inx, TRUE);
		
		/* Tx Interrupt는 사용하지 않음 */
		Lkp502SetTxInt (lp_inx, FALSE);

		/* Rx Interrupt는 사용하지 않음 */
		Lkp502SetRxInt (lp_inx, FALSE);

		/* Reset 설정 */
		Lkp502SetGtxRstCtrl (lp_inx, TRUE);
	}
}

/* Check Sum을 구하는 함수 */
uint mkCheckSumData(uint *header, int h_len, uint *data, int d_len)
{
	uint	check_sum = 0;
	int 	max_cnt = LP_TX_BUF_SIZE - 1;
	int 	i;
	
	if (h_len > 0)	for (i = 0; i < h_len; i ++)	check_sum += *header ++;
	if (d_len > 0)	for (i = 0; i < d_len; i ++)	check_sum += *data ++;

	return ~check_sum;	/* 비트단위 NOT 연산 */
}

LP_ENV_DATA *getLpEnvData(int lp_inx)
{
	if (lp_inx < LP_0 || lp_inx > LP_1) 	return NULL;
	return &(lp_env_data[lp_inx]);
}

void initLpEnvData (void) // main()에서 1번만 호출
{
	LP_ENV_DATA	*lp_env;
	uint		*h_data;
	int			lp_inx;
	SWI_GT_PORT	*gtx_port = pGtPort;

	/* Set manual mode to control Tx/Rx LED */
	Lkp502SetCLedAutoMode (FALSE);

	for (lp_inx = 0; lp_inx < MODULE_MAX_LINK; lp_inx ++) {
		gtx_map_data[lp_inx] = gtx_port;

		lp_env = getLpEnvData (lp_inx);
		lp_env->lrx_set_env 	= TRUE; /* Link Rx Mode 환경 설정 */
		lp_env->rx_cnt			= 0;    /* Rx 발생 갯수 */
		lp_env->rx_ok_cnt		= 0;    /* 정상 Rx 횟수 */
		lp_env->rx_overflow 	= 0;    /* Rx로 수신한 데이터를 처리하지 못한 상태에서 데이터 수신한 숫자 */
		lp_env->rx_check_sum_err= 0;    /* Rx 수신시 check sum 오류 횟수 */
#if 1
		lp_env->rx_seq_no_err   = 0;    /* Rx sequence number 오류 횟수 */
#endif

		lp_env->ltx_set_env 	= TRUE; /* Link Tx Mode 환경 설정 */
		lp_env->tx_cnt			= 0;    /* Tx 시도 횟수 */
		lp_env->tx_ok_cnt		= 0;    /* 정상 Tx 횟수 */
		lp_env->tx_err_cnt		= 0;    /* 오류 Tx 횟수 */

		lp_env->rx_int_proc_flag= FALSE;    /* Rx Int에서 설정 */
		lp_env->rx_read_cnt 	= lp_env->tx_ok_cnt;    /* rx_buf로 부터 데이터를 읽은 횟수 */

		Lkp502SetTxSize (lp_inx, TX_1B_MAX_SIZE);

		/* sequence number는 현재 GTX Buffer에 저장된 정보로 초기화한다 */
		lp_env->tx_seq_num = GET_SEQ_NO_DATA(gtx_map_data[lp_inx]->pTxSpace);
		lp_env->rx_seq_num = GET_SEQ_NO_DATA(gtx_map_data[lp_inx]->pRxSpace);

		/* Turn Off LED */
		Lkp502SetTxLed (lp_inx, OFF);
		Lkp502SetRxLed (lp_inx, OFF);

		/* Enable link port */

		linkEnable (lp_inx, ON);

		gtx_port ++;
	}
}

/* Tx가 가능한가를 확인하는 함수
 * TRUE : OK
 * ERROR : ERROR
*/
int linkTxReady (int lp_inx)
{
	LP_ENV_DATA *lp_env_data;
	uint32 		rx_fail;
	lp_env_data = getLpEnvData (lp_inx);

	if (lp_env_data->ltx_set_env != TRUE)	return ERROR;

	/* GT Status #5 Register : 0xnnnn8014 */
	rx_fail = (uint32)swiInLong(gtx_map_data[lp_inx]->pRoStatReg) & CHECK_LP_RX_FAIL;
	if (!rx_fail)	return TRUE;

	return ERROR;
}

/* Link Port의 rx pack buf의 값을 반환함
 * Link Port를 통해 읽은 데이터 값이 저장되는 버퍼임
 */
uint *getLpEnvPackBuf (int lp_inx)
{
	LP_ENV_DATA *lp_env;

	if (lp_inx < LP_0 || lp_inx > LP_1) return NULL;

	lp_env = getLpEnvData (lp_inx);
	return (lp_env->rx_pack_buf);
}

/* Packet Buffer로 구성된 데이터를 복사하는 함수 */
void copyPackBuffer (uint *in_fr_pack_buf, uint *in_to_pack_buf)
{
	PACK_INFO	*p_info;
	uint		*fr_pack, *to_pack;
	int 		max, p, i, len;

	p_info = (PACK_INFO *) in_fr_pack_buf;
	if (p_info->packet_init != PACKET_INIT_DATA) {
		/* source가 초기화가 안되어 있음
		 * PACK_INFO 영역만 초기화하고 return
		 */
		memset(in_to_pack_buf, 0x00, PACKET_OFFSET(1));
		return;
	}
	
	max = p_info->packet_cnt;
	for (p = 0; p < max; p++) {
		to_pack = in_to_pack_buf + PACKET_OFFSET(p);
		fr_pack = in_fr_pack_buf + PACKET_OFFSET(p);
		len = p_info->packet_len[p];
		memset(to_pack, 0x00, PACKET_OFFSET(1));
		for (i = 0; i < len; i ++) {
			*to_pack++ = *fr_pack;
			fr_pack ++;
		}
	}
}

/* Packet Buffer에서 초기 PACK_INFO 데이터를 만드는 함수 */
void makePackInfo (uint *pack_buf)
{
	PACK_INFO	*p_info;

	memset((void *)pack_buf, 0x00, sizeof(PACK_INFO));
	p_info					= (PACK_INFO *)pack_buf;
	p_info->packet_cnt		= 1;
	p_info->packet_len[0]	= sizeof(PACK_INFO);
	p_info->packet_init 	= PACKET_INIT_DATA;
}

/* fr_buf 데이터를 packet_inx로 지정되는 packet으로 append하는 함수
 * Retrun : 비워져 있는 다음 packet_index
 */
int appendLinkPacket (int packet_inx, uint *to_pack_buf, uint *fr_buf, int len)
{
	PACK_INFO	*p_info;
	uint		*to_buf;
	int i;

	p_info = (PACK_INFO *)to_pack_buf;
	to_buf = to_pack_buf + PACKET_OFFSET(packet_inx);

	for (i = 0; i < len; i ++) {
		*to_buf = *fr_buf ++;
		to_buf ++;
	}
	
	p_info->packet_len[packet_inx] = len;
	packet_inx ++;
	p_info->packet_cnt = packet_inx;

	return packet_inx;
}

/* fr_packet_buf에 있는 Packet을 packet_inx로 지정되는 packet으로
 * 모두 append하는 함수
 * Retrun : 비워저 있는 다음 packet_index
 * fr_buf(super_block + p#1...p#n)을 to_buf로 append하는 함수
 */
int appendLinkPackBuf (int packet_inx, uint *to_pack_buf, uint *fr_pack_buf)
{
	PACK_INFO	*fr_p_info = (PACK_INFO *) fr_pack_buf;
	uint		*fr_buf;
	int 		p, i, len;

	if (fr_p_info->packet_init != PACKET_INIT_DATA ||	/* 초기화가 안되었거나 */
		fr_p_info->packet_cnt == 1) 				/* PACKET DATA만 있을 경우 */
		return packet_inx;

	fr_buf = fr_pack_buf + PACKET_OFFSET(1);

	for (p = 1; p < fr_p_info->packet_cnt; p++) {
		len = fr_p_info->packet_len[p];
		packet_inx = appendLinkPacket(packet_inx, to_pack_buf, fr_buf, len);

		fr_buf += PACKET_OFFSET(1);
	}
	return packet_inx;
}

/* Link port에 tx_pack_buf(super_block+p#1+...+p#n)을 출력하는 함수
 * Return Value : OK or ERROR
 */
int linkSendProc (int lp_inx, uint *tx_pack_buf)
{
	LP_ENV_DATA 	*lp_env_data;
	PACK_INFO		*p_info;
	uint			seq_no;
	vuint			*snd_buf, *rcv_buf;
//	uint			*data;
	int 			i, len;
	uint			h_data, check_sum;
	int 			p, ret = ERROR;
		
	if (lp_inx < LP_0 && lp_inx > LP_1) 	return ERROR;

	lp_env_data = getLpEnvData (lp_inx);
	lp_env_data->tx_cnt ++;
	
	if (linkTxReady(lp_inx) == TRUE) {
		Lkp502SetTxLed (lp_inx, ON);
		seq_no = lp_env_data->tx_seq_num;
		seq_no ++;
		if (seq_no > 0xFFFF)	seq_no = 0;
		lp_env_data->tx_seq_num = seq_no;

		p_info	= (PACK_INFO *) tx_pack_buf;
		p		= p_info->packet_cnt - 1;
		for (; p >= 0; p --) {
			snd_buf = (vint32 *)gtx_map_data[lp_inx]->pTxSpace;

			snd_buf += PACKET_OFFSET(p);
			rcv_buf  = tx_pack_buf;
			rcv_buf += PACKET_OFFSET(p);

			len = p_info->packet_len[p];
			h_data		= MK_HEADER_DATA(seq_no, len);
			check_sum	= mkCheckSumData(&h_data, 1, rcv_buf, len);

			*snd_buf ++ = h_data;
			for (i = 1; i <= len; i ++) {
				*snd_buf ++ = *rcv_buf;
				rcv_buf ++;
			}
			if (i < (LP_TX_BUF_SIZE-1)) {
				for (; i < (LP_TX_BUF_SIZE-1); i ++)
					*snd_buf++ = 0x00;
			}
			*snd_buf = check_sum;
		}
	
		ret = OK;
		Lkp502SetTxLed (lp_inx, OFF);
	}

	if (ret == OK) {
		lp_env_data->tx_ok_cnt ++;
		return OK;
	} 
	lp_env_data->tx_err_cnt ++;

	return ERROR;
}
	
/* Link port에 tx_pack_buf(super_block+p#1+...+p#n)을 출력하는 함수
 * Return Value : OK or ERROR
 */
int linkWriteProc (int lp_index, uint *tx_pack_buf)
{
	int stat;
	if (lp_index == LP_0 || lp_index == LP_1) {
#if 0
		if (linkTxReady (lp_index) != TRUE)
			return ERROR;
#endif
		stat = linkSendProc (lp_index, tx_pack_buf);
	}
	return stat;
}

/* tx_lp_inx로 inc_lp_index로 정의된 port로 읽은 데이터와 in_data를 통합하여 
 * 전송하는 함수
 * Return Value : OK or ERROR
 */
int linkWrite (int tx_lp_index, uint *in_data, int len, int *inc_lp_index)
{
	int 	i, packet_inx, *lp_index;
	int 	stat;
	uint	*my_tx_pack_buf;
	uint	*my_rcv_pack_buf[2];

	if (tx_lp_index < LP_0 || tx_lp_index > LP_1)	return ERROR;

	my_tx_pack_buf = local_tx_pack_buf;
	
	makePackInfo (my_tx_pack_buf);

	packet_inx = 1;
	if (inc_lp_index != NULL) {
		my_rcv_pack_buf[0]	= getLpEnvPackBuf (LP_0);
		my_rcv_pack_buf[1]	= getLpEnvPackBuf (LP_1);

		lp_index = inc_lp_index;
		for (i = 0; i < MAX_LINK_CNT; i ++, lp_index ++) {
			if (*lp_index == ERROR)
				continue;
			switch(*lp_index) {
				case LP_0 :
					packet_inx = appendLinkPackBuf (packet_inx, my_tx_pack_buf, my_rcv_pack_buf[0]);
					break;
				case LP_1 :
					packet_inx = appendLinkPackBuf (packet_inx, my_tx_pack_buf, my_rcv_pack_buf[1]);
					break;
				default :	break;
			}
		}
	}

	appendLinkPacket (packet_inx, my_tx_pack_buf, in_data, len);

	stat = linkWriteProc (tx_lp_index, my_tx_pack_buf);
	return stat;
}

/* Link port(fr_gtx_buf)로 부터 packet을 읽어 in_to_pack_buf로 저장하는 함수
 * Return Value : OK or ERROR
 */
#if 0
int gtxGetPackData (ADDRESS fr_gtx_buf, uint *in_to_pack_buf, int rx_seq_no)
#else
int gtxGetPackData (int lp_inx, ADDRESS fr_gtx_buf, uint *in_to_pack_buf, int rx_seq_no)
#endif
{
	LP_ENV_DATA *lp_env_data;
	uint	*to_pack_buf, check_sum, h_data, rcv_check_sum;
	int 	i, len;

#if 1
	lp_env_data = getLpEnvData (lp_inx);
#endif

	h_data = *fr_gtx_buf ++;
	len = GET_LEN_DATA(h_data);
	if (GET_SEQ_NO_DATA(h_data) != rx_seq_no) {
#if 1
		lp_env_data->rx_seq_no_err ++;
#endif
		/*
		printf ("%s : SEQ_NO ERROR : req(0x%X) rcv(0x%X)\n",
				__FUNCTION__, rx_seq_no, GET_SEQ_NO_DATA(h_data));
		*/
		return ERROR;
	}

	to_pack_buf = in_to_pack_buf;
	/* 최소한 한번씩은 Tx된 데이터를 읽어야 함.
	 * 안그러면 인터럽트가 정상 동작하지 않음
	 */
	for (i = 0; i < LP_TX_REAL_LEN; i ++) {
		*to_pack_buf = *fr_gtx_buf ++;
		to_pack_buf ++;
	}

	rcv_check_sum = *fr_gtx_buf;
	if (len > LP_TX_REAL_LEN) {
		/* printf ("%s : LEN TOO LONG : len = %d\n", __FUNCTION__, len); */
#if 1
		lp_env_data->rx_overflow ++;
#endif
		return ERROR;	/* data over flow or crack */
	}

	/* printf ("%s : len = %d rcv_check_sum = 0x%X\n",
			__FUNCTION__, len, rcv_check_sum); */

	check_sum = mkCheckSumData(&h_data, 1, in_to_pack_buf, len);

	if (check_sum != rcv_check_sum) {
#if 1
		lp_env_data->rx_check_sum_err ++;
#endif
		/* printf ("%s : CHECK SUM ERROR : make(0x%X) rcv(0x%X)\n",
				__FUNCTION__, check_sum, rcv_check_sum); */
		return ERROR;
	}

	return OK;
}

/* Link port로 부터 packet buffer 데이터를 읽는 함수
 * Return Value : ERROR
 *				  FALSE : 신규 데이터 없음
 *				  TRUE	: 신규 데이터 있음
 */
int linkReadProc (int lp_inx, uint *rcv_pack_buf)
{
	LP_ENV_DATA *lp_env_data;
	PACK_INFO	*fr_p_info, *to_p_info;
	vint32		*gtx_rx_buf;
	uint		*to_pack_buf;
	uint		hd_data, rx_seq_no;
	int 		p, p_cnt, i, len;
	int 		stat;

	lp_env_data = getLpEnvData (lp_inx);
	if (lp_env_data->lrx_set_env != TRUE) {
	    return ERROR;
	}
	
	gtx_rx_buf	= (vint32 *)gtx_map_data[lp_inx]->pRxSpace;
	
	hd_data 	= *gtx_rx_buf;
	fr_p_info	= (PACK_INFO *) (gtx_rx_buf + 1);
	rx_seq_no	= GET_SEQ_NO_DATA(hd_data);

	if (lp_env_data->rx_seq_num == rx_seq_no) {
		return FALSE; /* 예전 데이터임 */
	}
	
	if (
	    fr_p_info->packet_init != PACKET_INIT_DATA ||	/* 초기화 안된 데이터 */
		fr_p_info->packet_cnt == 1 ||					/* Data Packet 없음 */
		lp_env_data->rx_seq_num == rx_seq_no) { 		/* 예전 데이터임 */
		return FALSE;
	}
	

	Lkp502SetRxLed (lp_inx, ON);

	to_pack_buf = local_rx_pack_buf;
	lp_env_data->rx_cnt ++;

	stat = 1;
	p_cnt = fr_p_info->packet_cnt;
	for (p = 0; p < p_cnt; p ++) {
#if 0
		if (gtxGetPackData (gtx_rx_buf, to_pack_buf, rx_seq_no) == ERROR) {
#else
		if (gtxGetPackData (lp_inx, gtx_rx_buf, to_pack_buf, rx_seq_no) == ERROR) {
#endif
			stat = 0;
			break;
		}
		gtx_rx_buf	+= PACKET_OFFSET(1);
		to_pack_buf += PACKET_OFFSET(1);
	}

	Lkp502SetRxLed (lp_inx, OFF);

	to_p_info = (PACK_INFO *) rcv_pack_buf;
	if (stat) {
		lp_env_data->rx_ok_cnt ++;
		lp_env_data->rx_seq_num = rx_seq_no;
		copyPackBuffer (local_rx_pack_buf, rcv_pack_buf);
		to_p_info->err_status = 0;
		return TRUE;
	}
	lp_env_data->rx_check_sum_err ++;
	to_p_info->err_status = 1;

	return ERROR;
}

/* Link port에서 데이터를 읽는 함수
 * Return Value : ERROR or 읽은 Packet 갯수
 */
int linkRead (int lp_index)
{
	uint	*rcv_pack_buf;
	PACK_INFO	*p_info;
	int 	r_stat;

	if (lp_index == LP_0 || lp_index == LP_1) {
		rcv_pack_buf = getLpEnvPackBuf (lp_index);
		r_stat = linkReadProc (lp_index, rcv_pack_buf);
		if (r_stat != TRUE) 	{
		    return r_stat;
		}
	} else {
		return ERROR;
	}
	
	p_info = (PACK_INFO *)rcv_pack_buf;
	if (p_info->packet_init != PACKET_INIT_DATA)	{
	    return 0;
	}
	
	return p_info->packet_cnt - 1;
}

/* link read로 부터 읽은 데이터에서 packet을 추출하는 함수
 * Return Value : packet의 길이임
 */
int linkReadPacket(int lp_index, int packet_inx, uint *buf)
{
	PACK_INFO	*p_info;
	uint		*rcv_pack_buf;
	int 		i, len;

	if (lp_index == LP_0 || lp_index == LP_1) {
		rcv_pack_buf = getLpEnvPackBuf(lp_index);
	} else {
		printf ("linkReadPacket : lp = %d p_inx = %d : LP ERR \n", lp_index, packet_inx);
		return ERROR;
	}
	p_info = (PACK_INFO *) rcv_pack_buf;

	if (p_info->err_status) {
		printf ("linkReadPacket : lp = %d p_inx = %d : ERR_STAT ERR \n", lp_index, packet_inx);
		return ERROR;
	}
	if (packet_inx >= p_info->packet_cnt) {
		printf ("linkReadPacket : lp = %d p_inx = %d : PACET_INX ERR \n", lp_index, packet_inx);
		return ERROR;
	}

	len = p_info->packet_len[packet_inx];
	rcv_pack_buf += PACKET_OFFSET(packet_inx);
	for (i = 0; i < len; i++) {
		*buf++ = *rcv_pack_buf;
		rcv_pack_buf++;
	}
	return len;
}

void showGtxData (uint *in_gtx_buf)
{
	PACK_INFO	*p_info;
	uint		*gtx_buf;
	int 		p, i;

	p_info = (PACK_INFO *) (in_gtx_buf + 1);	/* skip header */

	printf ("PACKET_CNT = %d ERR_STATUS = %d\n", p_info->packet_cnt, p_info->err_status);
	for (p = 0; p < p_info->packet_cnt; p ++) {
		printf ("   Packet#%d : %d\n", p, p_info->packet_len[p]);
	}
	for (p = 1; p < p_info->packet_cnt; p ++) {
		gtx_buf = in_gtx_buf + PACKET_OFFSET(p);
		gtx_buf ++;		/* skip header */
		printf ("   Packet#%d : %d\n", p, p_info->packet_len[p]);
		printf ("        ");
		for (i = 0; i < p_info->packet_len[p]; ) {
			printf ("%02X ", *gtx_buf++);
			i++;
			if ((i % 16) == 0)	printf ("\n        ");
		}
		printf ("\n");
	}
}

