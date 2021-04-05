
#include	<stdio.h>
#include	<ctype.h>
#include	<string.h>
#include	"usrTypes.h"
#include	"sysDefs.h"
#include	"pcsDefs.h"
#include	"segScheduler.h"



#include	"netProtocol.h"
#include	"usrDefBuf.h"
#include	"sysConfig.h"
#include	"protocolBuf.h"
#include	"usrErrCode.h"
#include	"usrQueData.h"
#include	"osDepend.h"


#define MAXCLUM     (16) 
#ifndef MAXBYTE
#define MAXBYTE     (256)
#endif 

#define	ESC 0x1B


extern int32 gDebugPortFd;


uint32	optionCodeCmdNullFunc(strNewNetProtocolInfo *, strBufInfo *);
#if (SYSTEM_DATA_SYNC_BOARD == ETH3_BOARD) 
	uint32	dualCmdNullFunc(strDualProtocolInfo *, strBufInfo *);
#endif
void	memoryClear ( uint8 *, uint32 );
void	fourBytesDataCopy ( uint32 *, uint32 *, uint32 );
void	memoryCopy ( uint8 *, uint8 *, uint32 );
uint32	timeDataCheck( strTimeDataInfo *);
void	dataDisplay(uint8 *, uint8 *, uint16 );
int32	nodePutOnFirst(   strDoubleLink **,strDoubleLink *);
int32	nodePutOnLast(strDoubleLink **,strDoubleLink *);
strDoubleLink *nodeGetAndUnlink(strDoubleLink **);
int32	nodeUnlink( strDoubleLink **,strDoubleLink *);
int32	nodeFind( strDoubleLink *, strDoubleLink *);
strDoubleLink *nextNodeRef(strDoubleLink *,strDoubleLink *);
int32	nodePutOnMid( strDoubleLink *,strDoubleLink *);
int32	HexToAscii(uint8 *, int32,uint8 *);
void	AsciiToHex(uint8 *, int32, uint8 *);
int32	keyHit(void);
uint32	stringCompare(int8 *, int8 *);
uint32	BcdToHex (void *, void *, int );
void	HexToBcd(int32, uint8 *);
void	cursorPositionSet(void);
void	screenClear(void);
void	lineClear(void);
uint32	stringCompareWithLen( uint8 *, uint8 *, uint32  );
uint32	stringLength(uint8 *);
void	mySwap(void*, void *, uint32);

extern int32   ioSysCtrlFunc(int32 , int32 , int32 );
extern int32   ioWrite(int32 , int8 *, int32 );
extern void setErrorCodeWithVal(uint8 *,uint32,uint8 *,uint32, uint8 *,uint32,
							uint8 *,uint32,uint8 *,uint32);


#ifdef _WIN32
extern	int32	_keyhit(int);
extern	void PortWrite (BYTE,int);
extern	BYTE PortRead (int32);
#endif


uint32	optionCodeCmdNullFunc(
				strNewNetProtocolInfo	*reqMsgPtr,
				strBufInfo *respMsgPtr
					)
{
	uint32	status = NO_ERROR;

    respMsgPtr->dataLen = 0;

    status = L_OPTION_CODE_ERR & 0xffff;

	return(status); 
}


#if (SYSTEM_DATA_SYNC_BOARD == ETH3_BOARD) 
uint32	dualCmdNullFunc(
				strDualProtocolInfo	*reqMsgPtr,
				strBufInfo *respMsgPtr
					)
{
	uint32	status = NO_ERROR;

    respMsgPtr->dataLen = 0;

    status = L_OPTION_CODE_ERR & 0xffff;

	return(status); 
}
#endif


void memoryClear ( uint8 *ptr, uint32 len)
{
	while(len--) *ptr++ = 0;

    return;
}

void fourBytesMemoryClear ( uint32 *ptr, uint32 len)
{
	while(len--) *ptr++ = 0;

    return;
}


void memoryCopy ( uint8 *destPtr, uint8 *srcPtr, uint32 len)
{
	uint32 length = len;
#ifdef _WIN32
	__try{
		while(length--) *destPtr++ = *srcPtr++;
	}
	__except(EXCEPTION_EXECUTE_HANDLER){
		if(GetExceptionCode() == EXCEPTION_ACCESS_VIOLATION ) {

		}
	}
#endif 

#ifdef VXWORKS
	while(length--) *destPtr++ = *srcPtr++;
#endif

    return;
}


void fourBytesDataCopy ( uint32 *destPtr, uint32 *srcPtr, uint32 len)
{
	uint32 length = len;
#ifdef _WIN32
	__try{
		while(length--) *destPtr++ = *srcPtr++;
	}
	__except(EXCEPTION_EXECUTE_HANDLER){
		if(GetExceptionCode() == EXCEPTION_ACCESS_VIOLATION )
			printf("EXCEPTION_ACCESS_VIOLATION : fourBytesDataCopy()\n" );
	}
#endif 
#ifdef VXWORKS
	while(length--) *destPtr++ = *srcPtr++;
#endif
    return;
}


uint32	timeDataCheck( strTimeDataInfo *dataPtr)
{
	uint32	status = NO_ERROR;

	if ( dataPtr->year <= 1900) 
	{
		status = L_TIME_ERR;
		
        setErrorCodeWithVal(__FILE__, __LINE__, __FUNCTION__,status,
							(uint8 *)"Year", dataPtr->year,
							(uint8 *)"Min", 1900,
							(uint8 *)"Not Using", 0
			  		);
	}
    else if ( (dataPtr->month < 1) || (dataPtr->month > 12)) 
    {
		status = L_TIME_ERR;
		
        setErrorCodeWithVal(__FILE__, __LINE__, __FUNCTION__,status,
							(uint8 *)"Month", dataPtr->month,
							(uint8 *)"Min", 1,
							(uint8 *)"Max", 12
			  		);
	}
    else if ( (dataPtr->date <1) || (dataPtr->date > 31)) {
		status = L_TIME_ERR;
		
        setErrorCodeWithVal(__FILE__, __LINE__, __FUNCTION__,status,
							(uint8 *)"Date", dataPtr->date,
							(uint8 *)"Min", 1,
							(uint8 *)"Max", 31
			  		);
	}
    else if (dataPtr->day > 6) {
		status = L_TIME_ERR;
		
        setErrorCodeWithVal(__FILE__, __LINE__, __FUNCTION__,status,
							(uint8 *)"Day", dataPtr->day,
							(uint8 *)"Min", 0,
							(uint8 *)"Max", 6
			  		);
	}
    else if ( dataPtr->hour > 23) {
		status = L_TIME_ERR;
		
        setErrorCodeWithVal(__FILE__, __LINE__, __FUNCTION__,status,
							(uint8 *)"Hour", dataPtr->hour,
							(uint8 *)"Min", 0,
							(uint8 *)"Max", 23
			  		);
	}
    else if ( dataPtr->minute > 59) {
		status = L_TIME_ERR;
		
        setErrorCodeWithVal(__FILE__, __LINE__, __FUNCTION__,status,
							(uint8 *)"Minute", dataPtr->minute,
							(uint8 *)"Min", 0,
							(uint8 *)"Max", 59
			  		);
	}
    else if ( dataPtr->second > 59) {
		status = L_TIME_ERR;
		
        setErrorCodeWithVal(__FILE__, __LINE__, __FUNCTION__,status,
							(uint8 *)"Second", dataPtr->second,
							(uint8 *)"Min", 0,
							(uint8 *)"Max", 59
			  		);
	}
	else if ( dataPtr->mSec > 999) {
		status = L_TIME_ERR;
		
        setErrorCodeWithVal(__FILE__, __LINE__, __FUNCTION__,status,
							(uint8 *)"MiliSecond", dataPtr->mSec,
							(uint8 *)"Min", 0,
							(uint8 *)"Max", 999
			  		);
	}

	return (status);
} 


void dataDisplay(uint8 *name, uint8 *data_ptr, uint16 length)
{
    uint8        buffer[MAXCLUM];
    int16       i = 0, j, k;
    uint8    ch;
    int16	size;	
    uint8	*savePtr;

    printf("\n\t-----------%s[%d]-----------\n", name, length);

	
      if( (length % MAXCLUM ) == 1) {
         size = length+1;
        *(data_ptr+length) = 0; 
    }
    else {size = length;}
	
    savePtr = data_ptr;

    printf("%08lx: ", (long)savePtr);

    for(i = 0; i < size; i++) {
        ch = (uint8)*data_ptr;
        k = i%MAXCLUM;
        if( ((!(i%MAXCLUM)) && (i != 0)) || (i == (size-1)) ) 
		{
            if (i == (size-1)) { 
                buffer[k] = (uint8)((isprint(ch)) ? ch : '.');			
                memset ( (void*)&buffer[k+1], '.', (uint16) (MAXCLUM-k-1) );

                if( (k%8) != 0 ) {
                    if( (length % MAXCLUM) == 1) {	
                        printf(" . ");
					} 
					else {						
                        printf("%02x ", ch);
                }
                } 
				else {
                    printf("- ");
                    printf("%02x ", ch);
                }

                for( j = k+1; j < MAXCLUM; j++) {
                    if ( !(j%8) ) 
                        printf("- ");
                
                    printf(" . ");
                }
            }
            for(j = 0; j < MAXCLUM; j++) 
                printf("%c", buffer[j]);


            printf("\n");
            memset((void *)buffer, '.', (uint16)MAXCLUM);

            if (i == (size-1)) return;

            if(!(i%MAXBYTE)) 
                printf("\n");
			
            printf("%08lx: ", (long)(savePtr+i)); 
        }
        buffer[k] = (uint8)((isprint(ch)) ? ch : '.');
		
        data_ptr++;

        if (!(k%8) && (k != 0) )printf("- ");
        printf("%02x ", ch); 
    }
}


int32    nodePutOnFirst(   	strDoubleLink **headNodePtr,
                        	strDoubleLink *nodePtr
                        )
{
    strDoubleLink     *tailNodePtr;
    int32 status = NO_ERROR;

    if(nodePtr != (strDoubleLink *)NULL) {
        if(*headNodePtr == NULL){
        
            nodePtr->next = nodePtr;
            nodePtr->prev = nodePtr;
        }
        else {
        
            tailNodePtr = (*headNodePtr)->prev;
            nodePtr->next = *headNodePtr;
            nodePtr->prev = tailNodePtr;
            tailNodePtr->next = nodePtr;
            (*headNodePtr)->prev = nodePtr;
        }

        
        *headNodePtr = nodePtr;
    }
    else status = INPUT_NULL_POINT_ERR;

    return (status);
}


int32 nodePutOnLast(	strDoubleLink **headNodePtr,
                    	strDoubleLink *nodePtr
                    )
{
    strDoubleLink *tailNodePtr;
    int32 status = NO_ERROR;

    if(nodePtr != (strDoubleLink *)NULL){
        if(*headNodePtr == NULL) {
            
            nodePtr->next = nodePtr;
            nodePtr->prev = nodePtr;
            *headNodePtr = nodePtr;
        }
        else {
            
            tailNodePtr = (*headNodePtr)->prev;
            tailNodePtr->next = nodePtr;
            nodePtr->prev = tailNodePtr;
            nodePtr->next = *headNodePtr;
            (*headNodePtr)->prev = nodePtr;
        }
    }
    else status = INPUT_NULL_POINT_ERR;

    return(status);
}


strDoubleLink *nodeGetAndUnlink(strDoubleLink **headNodePtr)
{
    strDoubleLink *nodePtr;
    int32 status = NO_ERROR;

    nodePtr = *headNodePtr;

    if(*headNodePtr == NULL) {
        nodePtr = NULL;
    }
    else {
        status = nodeUnlink(headNodePtr, nodePtr);
        if(status != NO_ERROR)
            nodePtr = NULL;
    }
    return(nodePtr);
}


int32 nodeUnlink( strDoubleLink **headNodePtr,
                  strDoubleLink *nodePtr
                )
{
    if(*headNodePtr == NULL || nodePtr == NULL){
        return(INPUT_NULL_POINT_ERR);
    }

    
    if(nodeFind(*headNodePtr, nodePtr)){
          return(NO_EXIST_NODE_ERR);
    }

    
    if( (nodePtr->next == *headNodePtr) && (nodePtr == *headNodePtr)){
        *headNodePtr = NULL;
    }
    else {
        
        if(nodePtr == *headNodePtr)
            *headNodePtr = nodePtr->next;

        (nodePtr->next)->prev = nodePtr->prev;
        (nodePtr->prev)->next = nodePtr->next;
    }
    
    return(NO_ERROR);
}


int32 nodeFind( 	strDoubleLink *headNodePtr,
                    strDoubleLink *nodePtr
              )
{
    strDoubleLink *tempPtr;
    strDoubleLink *tailNodePtr;
	int32	status = NO_ERROR;
	
    if( (headNodePtr == NULL) || (nodePtr == NULL))
        status = INPUT_NULL_POINT_ERR;
    else {
        tempPtr = headNodePtr;
        tailNodePtr = headNodePtr->prev;

        while( (tempPtr != tailNodePtr) && (tempPtr != nodePtr))
            tempPtr = tempPtr->next;

        if(tempPtr != nodePtr)
            status = NO_EXIST_NODE_ERR;
         else
         	status = NO_ERROR;
    }
    
    return(status);
}


strDoubleLink *nextNodeRef(  strDoubleLink *headNodePtr,
                              	strDoubleLink *curNodePtr
                          	)
{
    strDoubleLink *nextNodePtr = NULL;

    if( (headNodePtr == NULL) || (curNodePtr == NULL))
        nextNodePtr = NULL;
    else if(curNodePtr->next != headNodePtr)
        nextNodePtr = curNodePtr->next;
    else 
        nextNodePtr = NULL;

    return(nextNodePtr);
}


int32 nodePutOnNext( strDoubleLink *curNodePtr,
                     strDoubleLink *newNodePtr
                   )
{
    int32   status = NO_ERROR;
    strDoubleLink *nextNodePtr;

    if( (newNodePtr == NULL) || (curNodePtr == NULL))
        status = INPUT_NULL_POINT_ERR;
    else {
        nextNodePtr 		= curNodePtr->next;
        newNodePtr->next 	= nextNodePtr;
        newNodePtr->prev 	= curNodePtr;
        nextNodePtr->prev 	= newNodePtr;
        curNodePtr->next 	= newNodePtr;
    }
    return(status);
}


int32 nodePutOnPrev( 	strDoubleLink **headNodePtr,
						strDoubleLink *curNodePtr,
                     	strDoubleLink *newNodePtr
                   )
{
    int32   status = NO_ERROR;
    strDoubleLink 	*tailNodePtr;
    strDoubleLink 	*prevNodePtr;

    if( (newNodePtr == NULL) || (curNodePtr == NULL) || (*headNodePtr == NULL))
        status = INPUT_NULL_POINT_ERR;

	
    else if(*headNodePtr == curNodePtr) {
		tailNodePtr 			= (*headNodePtr)->prev;
		
        newNodePtr->next 		= *headNodePtr;
        newNodePtr->prev 		= tailNodePtr;
        
        tailNodePtr->next 		= newNodePtr;
        (*headNodePtr)->prev 	= newNodePtr;

        
        *headNodePtr = newNodePtr;
	}
    else {
        prevNodePtr 		= curNodePtr->prev;
        
        newNodePtr->next 	= curNodePtr;
        newNodePtr->prev 	= prevNodePtr;
        
        prevNodePtr->next 	= newNodePtr;
        curNodePtr->prev 	= newNodePtr;
	}
	
    return(status);
}


int32 HexToAscii(uint8 *hex_ptr, int32 len, uint8 *buf)
{
    int32     i, j;
    uint8    high_nibble[512];	
    uint8    low_nibble[512];	
    uint8    tmpbuf[512];	
    uint8    data;

    for(i = 0; i < len; i++, hex_ptr++){
        data = *hex_ptr;
        high_nibble[i] =((data>>4) & 0x0f);
        low_nibble[i] = (data & 0x0f);
    }
	i = 0;
	for(j = 0; j < len; j++) {
        tmpbuf[i] = high_nibble[j];
        tmpbuf[i+1] = low_nibble[j];
        i += 2;
    }
    for(i = 0; i < len * 2; i++, buf++) {
       
        if( tmpbuf[i] <= 0x09 )
            *buf = tmpbuf[i] + 0x30;
        else *buf = tmpbuf[i] + 0x37; 
    }
    return(len * 2);
}


void AsciiToHex(uint8 *ascii_ptr, int32 len, uint8 *buf)
{
    int32     i, j;
    uint8    high_nibble[512];	
    uint8    low_nibble[512];	
    uint8    tmpbuf[512];	
    uint8    data;
    uint8    *ptr;

    ptr = ascii_ptr;

    for(i = 0; i < len; i++, ptr++) 
    {
        data = *ptr;
        
        if( (data >= 0x30) && ( data <=0x39 ) ) 
            tmpbuf[i] = data - 0x30;
        else 
        	tmpbuf[i] = data - 0x37;
    }
    
    for(i = 0, j = 0; i < len; i = i+2, j++) 
    {
		high_nibble[j] = tmpbuf[i];
		low_nibble[j] = tmpbuf[i+1];
    }
    
    for(i = 0; i < len/2; i++)
    {
        *buf = (uint8) ((high_nibble[i] << 4) | low_nibble[i]);
        buf++;
	}
}


int32   keyHit(void)
{
    int32  byteRead = 0;

#ifdef VXWORKS
	(void)ioSysCtrlFunc(gDebugPortFd, MYIONREAD , (int32)&byteRead);
#else
	byteRead = _keyhit(gDebugPortFd);

	if(byteRead) 
	{
#ifdef _WIN32
		(void)PortRead(gDebugPortFd);
#endif
		PortWrite ('\b',gDebugPortFd);
	}
#endif
    return(byteRead);
}


uint32  stringCompare(int8 *s1, int8 *s2)
{
    uint32 len = 0, compare = 0;

    
    if( strlen((const char*)s1) != strlen((const char*)s2))
        compare = 1;
    
    else {
        len  = strlen((const char*)s1);
        while(len--){
            if(*s1++ != *s2++){
                compare = 1;
                break;
            }
        }
    }

    return(compare);
}


uint32 BcdToHex (void *bcdPtr, void *hexPtr, int byteLen)
{
    int8    c_data, *c_ptr;
    uint16  s_data, *s_ptr;
    uint32  l_data, *l_ptr;
    uint32  status = NO_ERROR;

    
    c_data = 0; s_data = 0; l_data = 0;
    c_ptr = NULL; s_ptr = NULL; l_ptr = NULL;

    switch(byteLen) {
        case 1:
            c_ptr = (int8 *)bcdPtr;

            c_data = ( ( (int8)*c_ptr ) & 0xf0) >> 4;
            c_data *= 10;
            c_data += ( (int8)*c_ptr) & 0xf;

            memoryCopy ( (uint8 *)hexPtr, (uint8 *)&c_data, 1);
        break;
        case 2: 
            s_ptr = (uint16 *)bcdPtr;

            s_data = ( (((uint16)*s_ptr) & 0xf000) >> 12 ) * 1000 +
                     ( (((uint16)*s_ptr) & 0x0f00) >> 8  ) * 100 +
                     ( (((uint16)*s_ptr) & 0x00f0) >> 4  ) * 10 +
                     ( (((uint16)*s_ptr) & 0x000f) >> 0  ) * 1;

            memoryCopy( (uint8 *) hexPtr, (uint8 *) &s_data, 2);
        break;
        case 4: 
            l_ptr= (uint32 *)bcdPtr;

            l_data = ( (((uint32)*l_ptr) & 0xf0000000) >> 28 ) * 10000000 +
                     ( (((uint32)*l_ptr) & 0x0f000000) >> 24 ) * 1000000 +
                     ( (((uint32)*l_ptr) & 0x00f00000) >> 20 ) * 100000 +
                     ( (((uint32)*l_ptr) & 0x000f0000) >> 16 ) * 10000 +
                     ( (((uint32)*l_ptr) & 0x0000f000) >> 12 ) * 1000 +
                     ( (((uint32)*l_ptr) & 0x00000f00) >> 8  ) * 100 +
                     ( (((uint32)*l_ptr) & 0x000000f0) >> 4  ) * 10 +
                     ( (((uint32)*l_ptr) & 0x0000000f) >> 0  ) * 1;

            memoryCopy( (uint8 *) hexPtr, (uint8 *) &l_data, 4);
        break;
        default:
            status = BCD_CONV_LEN_ERR;
        break;
    }
    return(status);
}


void HexToBcd(int32 hexData, uint8 *bcdPtr)
{
    uint8    bcdVal;

    bcdVal = hexData%10;

    hexData /= 10;
    hexData *= 0x10;

    bcdVal += hexData;
	
    *bcdPtr = bcdVal;
}


void screenClear(void)
{
	int8 data[5]={ESC,'[','2','J',0x00};
#ifdef _WINNT
		system("cls");
#else
	(void)ioWrite(gDebugPortFd, data, 5);
#endif
}


void lineClear(void)
{
#ifdef _WINNT
	printf("                                                 \r");
#else
	int8 data[5]={ESC,'[','2','K',0x00};
    (void)ioWrite(gDebugPortFd, data, 5);
#endif 
}


void cursorPositionSet(void)
{
	int8 data[9]={ESC,'[',0x00,0x00,';',0x00,0x00,'H',0x00};
	int8 Line =1, Column =1;
	
#ifdef _WINNT
	COORD Pos = {0, 0};
		SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), Pos);
#else

 	data[2]=(Line/10)+0x30;
 	data[3]=(Line%10)+0x30;
 	data[5]=(Column/10)+0x30;
 	data[6]=(Column%10)+0x30;

	(void)ioWrite(gDebugPortFd, data, 9); 
#endif

}


uint32 stringCompareWithLen(uint8 *dest_ptr, uint8 *src_ptr, uint32 len)
{
    uint32 i, status = NO_ERROR;

    for(i = 0; i < len; i++) {
        if(*dest_ptr++ != *src_ptr++){
            status = 1;
            break;
        } 
        else status = 0;
    }
    return(status);
}


uint32 stringLength(uint8 *string)
{
   uint32 len = 0;

   while(*string++ != 0x00)
       len++;

   return(len); 
}


void mySwap(void *a, void *b, uint32 n)
{
    uint8 *t;
    t = (uint8*)malloc(n);
    memcpy(t, a, n);
    memcpy(a, b, n);
    memcpy(b, t, n);
 
    free(t);
}

void upperToLowerCaseConversion(int8 *strPtr, uint32 strLen)
{
	uint32 	iLoop;
	int8	chData;
	
	for(iLoop = 0; iLoop < strLen; iLoop++, strPtr++)
	{
		chData = *strPtr;
		
		if( (chData >= 'A') && (chData <= 'Z'))
		{
			chData = 'a' + (chData - 'A');
			
			*strPtr = chData;	
		}	
	}
	
	return;
}

int32 isleap (int32 month, int32 year)
{
	int32	retVal;
	
	if( (month == 2) 
			&& 
		(
			((year % 400) == 0)
			   || 
			(((year % 4) == 0) && ((year % 100) != 0))
		)
	){ 
		retVal = 1;
	}
	else 
	{
		retVal = 0;	
	}
	
	return(retVal);
}

