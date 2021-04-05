
/**
 * IalIfDef.h
 * IalIfDef Interface Module Definition
 */

#ifndef _IALIFDEF_H_
#define _IALIFDEF_H_

#include "Hal/HalIfDef.h"



/** @name Symbolic Constants
 * Symbolic Constants
 */
///@{

#define IAL_DEFAULT_SIZE			0x1			///< Sync Master

///@}


/** @name HAL Error Cause
 * HAL Error
 */
///@{
typedef enum {
	 IAL_NOERR
	,IAL_ERR_PARAMETER_RANGE
	,IAL_ERR_TXBUFFER_FULL
	,IAL_ERR_RXBUFFER_FULL

	,IAL_ERR_LEGNUM_MISMATCH
	

	,IAL_ERR_NUM

} IAL_ERROR;
///@}



/** @name Type Definition
 * Type Definition
 */
///@{
typedef union uIalBuffer {
	float		*pfBuffer;
	uint32_t	*pulBuffer;
} U_IALBUFFER_TYPE;


typedef union uIalData {
	float		fData;
	uint32_t	ulData;
} U_IALDATA_TYPE;


///@}



/** @name Macro Definition
 * Macro Definition
 */
///@{

///< 레지스트 쓰기 함수

///< 레지스트 읽기 함수

///@}




#endif


