

#ifndef _MANAGERMAIN_H_
#define _MANAGERMAIN_H_

#include "Core/Signal.h"

#include "MainApp/ConfigApp/ConfigApp.h"
#include "MainApp/DebugApp/DebugApp.h"
#include "MainApp/DiagApp/DiagApp.h"
#include "MainApp/HmiApp/HmiApp.h"

void IsrManagerTimer0(int lArg);

void SetSyncLost(void);
void PeriodRoutine(void);


#endif


