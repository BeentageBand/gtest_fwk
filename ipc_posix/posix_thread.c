#define COBJECT_IMPLEMENTATION
#include "posix_thread.h"


typedef pthread_t PThread_T;

static void * posix_thread_routine(void * thread);

static void posix_thread_delete(struct Object * const obj);
static bool posix_thread_register_thread(union Thread_Cbk * const cbk, union Thread * const thread );
static bool posix_thread_run_thread(union Thread_Cbk * const cbk, union Thread * const thread );
static bool posix_thread_join_thread(union Thread_Cbk * const cbk, union Thread * const thread );
static bool posix_thread_unregister_thread(union Thread_Cbk * const cbk, union Thread * const thread );

union POSIX_Thread_Class _private POSIX_Thread_Class = 
{
    {
        {posix_thread_delete, NULL},
        posix_thread_register_thread,
        posix_thread_run_thread,
        posix_thread_join_thread,
        posix_thread_unregister_thread
    }
};

static union POSIX_Thread POSIX_Thread = {NULL};
static CMap_PThread_IPC_TID_T POSIX_Thread_PThread_Pool = {NULL};
static pthread_attr_t POSIX_Thread_Attr;
static pthread_mutexattr_t POSIX_Mux_Attr;

void * posix_thread_routine(void * thread)
{
   union Thread * const this = _cast(Thread, (union Thread *)thread);

   Isnt_Nullptr(this, NULL);

   Dbg_Info("%s for thread = %d", __func__, this->tid);

   if(this->vtbl && this->vtbl->runnable)
   {
      this->vtbl->runnable(this);
   }
   else
   {
      Dbg_Fault("%s:Unable to run thread %d", __func__, this->tid);
   }

   pthread_exit(NULL);
   return NULL;
}

void posix_thread_delete(struct Object * const obj)
{
    union POSIX_Thread * const this = (union POSIX_Thread *)Object_Cast(&POSIX_Thread_Class, obj);
    Isnt_Nullptr(this, );
    pthread_cancel(this->pthread, NULL);
}

IPC_TID_T ipc_posix_self_thread(union IPC_Helper * const helper)
{ 
    Pair_PThread_IPC_TID_T  pair = {pthread_self(), 0};

    Pair_PThread_IPC_TID_T * const found = POSIX_Thread_PThread_Pool.vtbl->find(&POSIX_Thread_PThread_Pool, pair);
    IPC_TID_T tid = IPC_MAX_TID;

    if(POSIX_Thread_PThread_Pool.vtbl->end(&POSIX_Thread_PThread_Pool) != found)
    {
        tid  = found->obj;
    }
    return tid;
}

bool posix_thread_register_thread(union Thread_Cbk * const cbk, union Thread * const thread)
{
    union POSIX_Thread * const this = _cast(POSIX_Thread, cbk);
    Isnt_Nullptr(this, false);
    IPC_Helper * const ipc_helper = IPC_get_instance();
    Isnt_Nullptr(ipc_helper, false);
    union Mutex * mux = ipc_helper->single_mux;
    Isnt_Nullptr(mux, false);

    if(!mux->vtbl->lock(mux, 200)) return false;
    CSet_Thread_Ptr_T * const thread_set = ipc_helper->rthreads;
    thread_set->vtbl->insert(thread);
    mux->vtbl->unlock(mux);
    return NULL != IPC_Helper_find_thread(thread->tid);
}

bool posix_thread_run_thread(union Thread_Cbk * const cbk, union Thread * const thread)
{
    bool rc = false;
    if(-1 == this->pthread)
    {
        rc = 0 == pthread_create(this->pthread,
                                &POSIX_Thread_Attr,
                                posix_thread_routine,
                                (void*)thread);
        POSIX_Thread_PThread_Pool.vbtl->insert(&POSIX_Thread_PThread_Pool, 
        CMap_PThread_IPC_TID_make_pair(this->pthread, thread->tid));
    }
    return rc;
}

bool posix_thread_join_thread(union Thread_Cbk * const cbk, union Thread * const thread)
{
    union POSIX_Thread * const this = _cast(POSIX_Thread, cbk);
    Isnt_Nullptr(this, false);
    bool rc = 0 == pthread_join(this->pthread);

    POSIX_Thread_PThread_Pool.vbtl->erase(&POSIX_Thread_PThread_Pool, 
     CMap_PThread_IPC_TID_make_pair(this->pthread, thread->tid));
    return rc;
}

bool posix_thread_unregister_thread(union Thread_Cbk * const cbk, union Thread * const thread)
{
    union POSIX_Thread * const this = _cast(POSIX_Thread, cbk);
    Isnt_Nullptr(this, false);
    IPC_Helper * const ipc_helper = IPC_get_instance();
    Isnt_Nullptr(ipc_helper, false);
    union Mutex * mux = ipc_helper->single_mux;
    Isnt_Nullptr(mux, false);
    union Thread * t_found = IPC_Helper_find_mailbox(thread->tid)
    Isnt_Nullptr(t_found, true);

    if(!mux->vtbl->lock(mux, 200)) return false;
    CSet_Thread_Ptr_T * const thread_set = ipc_helper->rthreads;
    thread_set->vtbl->erase(thread);
    mux->vtbl->unlock(mux);
    return NULL == IPC_Helper_find_thread(thread->tid);
}

void Populate_POSIX_Thread(union Thread_Cbk * const this)
{
    if(NULL == POSIX_Thread.vbtl)
    {
        POSIX_Thread.Thread_Cbk.vtbl = &Thread_Cbk_Class;
        Object_Init(&POSIX_Thread.Object,
        POSIX_Thread_Class.Class,
        sizeof(POSIX_Thread_Class.Thread_Cbk));
        pthread_attr_init(&POSIX_Thread_Attr);
        POSIX_Thread.pthread = -1;
    }
    _clone(this, POSIX_Thread)
    pthread_init(&this->pthread, &POSIX_Thread_Attr);
}