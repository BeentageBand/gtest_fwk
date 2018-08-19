#define COBJECT_IMPLEMENTATION
#include "ipc_posix.h"
#include "posix_tmr.h"

static void posix_timer_delete(struct Object * const obj);
static bool posix_timer_wait(union POSIX_Timer * const, union Timer * const, IPC_Clock_T const wait_ms);
static bool posix_timer_post(union POSIX_Timer * const, union Timer * const);

static union POSIX_Timer POSIX_Timer = {NULL};

union POSIX_Timer_Class POSIX_Timer_Class = 
{{
    {posix_timer_delete, NULL},
    posix_timer_wait,
    posix_timer_post
}};

void posix_timer_delete(struct Object * const obj)
{
    union POSIX_Timer * const this = (union POSIX_Timer *)Object_Cast(&POSIX_Timer_Class, obj);
    Isnt_Nullptr(this, );
    tmr_destroy(&this->tmr);
}

bool posix_timer_wait(union POSIX_Timer * const this, union Timer * const timer, IPC_Clock_T const wait_ms)
{
    struct timespec wait_ts;
    ipc_posix_make_timespec(&wait_ts, wait_ms);
    return 0 == tmr_timedwait(&this->tmr, &wait_ts);
}

bool posix_timer_post(union POSIX_Timer * const this, union Timer * const tmr)
{
    return 0 == tmr_post(&this->tmr);
}

void Populate_POSIX_Timer(union POSIX_Timer * const this)
{
    if(NULL == POSIX_Timer.vtbl)
    {
        POSIX_Timer.Timer_Cbk.vtbl = &Timer_Cbk_Class;
        Object_Init(&POSIX_Timer.Object,
                    &POSIX_Timer_Class.Class,
                    sizeof(POSIX_Timer_Class.Timer_Cbk));
    }
    _clone(this, POSIX_Timer);
    
}
