#ifndef POSIX_CV_H_
#define POSIX_CV_H_

#include <pthread.h>
#include "conditional.h"

typedef union POSIX_Conditional
{
    struct POSIX_Conditional_Class _private * _private vtbl;
    struct
    {
        union Conditional_Cbk Conditional_Cbk;
        pthread_cond_t cv;
    }
    struct Object Object;
}POSIX_Conditional_T;

typedef union POSIX_Conditional_Class
{
      struct Conditional_Cbk_Class Conditional_Cbk;
      struct Class Class;
      bool (* _private wait)(union POSIX_Conditional * const,
            union Conditional * const, IPC_Clock_T const wait_ms);
      bool (* _private post)(union POSIX_Conditional * const, union Conditional * const);
}POSIX_Conditional_Class_;

extern union POSIX_Conditional_Class _private POSIX_Conditional_Class;

extern void Populate_POSIX_Conditional(union POSIX_Conditional * const conditional);

#endif /*POSIX_CV_H_*/