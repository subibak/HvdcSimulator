
#ifndef USRDEF_H
#define USRDEF_H

#ifndef NULL
	#define	NULL	0x0
#endif

#ifndef TRUE
	#define 	TRUE		1
#endif

#ifndef FALSE
	#define 	FALSE       0
#endif

#define BYTE_SIZE		1
#define WORD_SIZE		2
#define DOUBLE_SIZE		4
#define LONG_SIZE		8


#define	INT_TYPE	(0x1)		
#define	UINT_TYPE	(0x2)
#define	LINT_TYPE	(0x3)
#define	ULINT_TYPE	(0x4)
#define	REAL_TYPE	(0x5)
#define	LREAL_TYPE	(0x6)		

#define TIME32_TYPE	(0x7)								  
#define	DATE32_TYPE	(0x8)								 
#define	TOD32_TYPE	(0x9)								 
#define	DT64_TYPE	(0xa)								 

#define	BOOL_TYPE	((0x2) | (0x9 << 4))		
#define	WORD_TYPE	((0x2) | (0xa << 4))		
#define	LWORD_TYPE	((0x4) | (0x9 << 4))		

#define	MAX_NOANY_TYPE_NUM		13
#define	ANY_TYPE		((MAX_NOANY_TYPE_NUM+1) << 16)			
#define	ANY_BIT_TYPE	((MAX_NOANY_TYPE_NUM+2) << 16)			
#define	ANY_NUM_TYPE	((MAX_NOANY_TYPE_NUM+3) << 16)			
#define	ANY_INT_TYPE	((MAX_NOANY_TYPE_NUM+4) << 16)			
#define	ANY_REAL_TYPE	((MAX_NOANY_TYPE_NUM+5) << 16)			

#define SIZE01_TYPE	(0x1 << 12)
#define SIZE08_TYPE	(0x2 << 12)
#define SIZE16_TYPE	(0x3 << 12)
#define SIZE32_TYPE	(0x4 << 12)
#define	SIZE64_TYPE (0x5 << 12) 

#define	GET_TYPE_SIZE(data)			((((data)&0xf000) > SIZE32_TYPE) ? SIZE64_TYPE : SIZE32_TYPE)
#define	GET_TYPE_DATA_SIZE(data)	((data)&0xf000)
#define	GET_TYPE_INDEX(data)		(((data)&0xf) + (((data)&0xf0)>>4))
#define	GET_ANY_TYPE_INDEX(data)	(((data)&0x1f0000)>>16 ) 

#define	GET_DATA_TYPE(data)				((data)&0xff)
#define	GET_BASIC_DATA_TYPE(data)		((data)&0xf)

#define	CHK_IS_ANY_TYPE(data)		(((data)&0x1f0000) != 0 ? 1 : 0)

#define	CHK_IS_BIT_TYPE(data)		( ( (((data)&0xf0) != 0 ) || (((data)&0x1f0000) == 0x0f0000) ) ? 1 :0  )

#define	GET_BASIC_SIZE(BT) (( ((BT)==LINT_TYPE) || ((BT)==ULINT_TYPE)||((BT)==LREAL_TYPE) || ((BT)==DT64_TYPE)) ? SIZE64_TYPE : SIZE32_TYPE)
#define GET_NON_TUNNABLE_TYPE(data)  ((data)&0x400)

#define NOT_USED    0

#ifndef ENDOFSTR
    #define ENDOFSTR    '\0'
#endif

#if (defined(VXWORKS))
#define ALIGN	__attribute__((packed)) 
#elif (defined(_WIN32))
#define ALIGN
#endif 


#ifdef _WIN32
extern void		fdprintf (int fd, const char *pFormat, ...);
#endif 

/* 64bit Access는 사용 안함 */
#undef USED_BIT64

#endif 
