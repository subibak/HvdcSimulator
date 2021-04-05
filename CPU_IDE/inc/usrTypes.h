
#ifndef USRTYPES_H
#define USRTYPES_H

#ifndef	VXWORKS
	#define	VXWORKS
#endif

#if defined(VXWORKS)
#include	"types/vxTypesOld.h"
#endif

typedef unsigned char	uint8;
typedef char			int8;

typedef unsigned short	uint16;
typedef	signed short	int16;

typedef	unsigned int	uint32;
typedef	signed int		int32;

#ifdef VXWORKS
	typedef	unsigned long long int uint64;
	typedef	signed long long int   int64;
#endif 

#ifdef _WIN32
	typedef	unsigned __int64 uint64;
	typedef	signed __int64	int64;

	#if ( (!defined(NO_ERROR)) || (NO_ERROR != 0) )
		#undef	NO_ERROR
		#define	NO_ERROR	(0)
	#endif
#endif 



#ifndef VXWORKS
	typedef	unsigned int	uint;
	typedef	int	TASK_ID;
	typedef	int	SEM_ID;
	typedef	int	PCI_DEV;
	typedef	int	VXB_DEVICE_ID;
	typedef	int	STATUS;

	#define	IMPORT extern

#endif 

typedef int (*func_ptr)(int32, int8 *, int32);
typedef void (*voidfunc_ptr)();

#define	func_ref(func)	func


#endif 
