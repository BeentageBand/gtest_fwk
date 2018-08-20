#ifndef POSIX_TMR_H_
#define POSIX_TMR_H_

#include <time.h>
#include "tmr.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef union POSIX_Timer
{
    union POSIX_Timer_Class _private * _private vtbl;
    struct
    {
        union Timer_Cbk Timer_Cbk;
        timer_t tmr;
    };
    struct Object Object;
}POSIX_Timer_T;

typedef union POSIX_Timer_Class
{
    struct Class Class;
    struct Timer_Cbk_Class Timer_Cbk;
}POSIX_Timer_Class_T;

extern union POSIX_Timer_Class _private POSIX_Timer_Class;

extern void Populate_POSIX_Timer(union POSIX_Timer * const cbk);

#ifdef __cplusplus
}
#endif

#endif /*POSIX_TMR_H_*/