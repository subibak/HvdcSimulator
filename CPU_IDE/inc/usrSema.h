
#ifndef USRSEMA_H
#define USRSEMA_H


#define BINARY_SEMA     (1)
#define COUNT_SEMA      (2)
#define MUTEX_SEMA      (3) 


typedef struct {
    int32   semaType;
    int32   semaOptions;
    int32   semaInitVal;
#ifdef _WIN32
	int32	semaMaxVal;
	char	*semaName;
#endif
}strSemaInfo;

#endif 
