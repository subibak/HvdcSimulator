

/*!
 * Queue.h
 * Queue Module Definition
 */



#ifndef _QUEUE_H_
#define _QUEUE_H_

#include "Core.h"

/** @name System Configuration
 * System Configuration
 */
///@{
///@}

/** @name System Error Cause
 * System Error Cause
 */
///@{
///@}


/** @name Type Definition
 * Type Definition
 */
///@{

typedef enum {
	NO_QERROR
	,OVERFLOW_QERROR
	,UNDERFLOW_QERROR
	,EMPTY_QERROR
	
} QRESULT;


typedef union uBufferBody {
	uint8_t		*p8Buffer;
	uint32_t	*p32Buffer;
	float		*pfBuffer;
	char		*pcBuffer;
} U_BUFFER_BODY;


typedef struct stBufferType			///< Buffer Type Á¤ÀÇ
{
	uint32_t		ulHead;
	uint32_t		ulTail;
	uint32_t		ulSize;
	uint32_t		ulUnusedSize;
	uint32_t		ulUsedSize;
	U_BUFFER_BODY	uBody;
} ST_BUFFER_TYPE;

//typedef struct stBufferType ST_BUFFER_TYPE;


///@}




QRESULT QWrite(ST_BUFFER_TYPE *pBuffer, uint8_t *pbData, uint32_t ulLen);
QRESULT QRead(ST_BUFFER_TYPE *pBuffer, uint8_t *pbData, uint32_t ulLen);
QRESULT QFlush(ST_BUFFER_TYPE *pBuffer);
QRESULT QRollBack(ST_BUFFER_TYPE *pBuffer);



#endif /* _CORE_H_ */
