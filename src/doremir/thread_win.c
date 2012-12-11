
#include <Windows.h>
#include <doremir/thread.h>

struct _doremir_thread_t
{
    HANDLE native;
};

struct _doremir_thread_mutex_t
{
    HANDLE native;
};

struct _doremir_thread_condition_t
{
    HANDLE native;
    doremir_thread_mutex_t  mutex;
};

static void doremir_thread_fatal(char* msg, int error);

static const long kJoinInterval = 50;


// --------------------------------------------------------------------------------
// Threads
// --------------------------------------------------------------------------------

static DWORD WINAPI doremir_thread_start(LPVOID x) 
{                
    doremir_thread_runnable_t *runnable = x;
    return runnable->func(runnable->val);
    return 0;
}

doremir_thread_t doremir_thread_create(doremir_thread_runnable_t* runnable)
{
    doremir_thread_t thread = malloc(sizeof(struct _doremir_thread_t));
    
    HANDLE result = CreateThread(NULL, 0, doremir_thread_start, runnable, 0, NULL);

    if (!result)
        doremir_thread_fatal("create", GetLastError());

    thread->native = result;
    return thread;
}

void doremir_thread_sleep(doremir_thread_milli_seconds_t millis)
{
    Sleep(millis);
}

void doremir_thread_join(doremir_thread_t thread)
{         
    BOOL result;
    DWORD exitCode;
    do
    {
        Sleep(kJoinInterval);
        result = GetExitCodeThread(thread->native, &exitCode);

        if (!result)
            doremir_thread_fatal("join", GetLastError());            

    } while (exitCode == STILL_ACTIVE);
    free(thread);                
}

void doremir_thread_detach(doremir_thread_t thread)
{
    BOOL result = CloseHandle(thread->native);
    free(thread);

    if (!result)
        doremir_thread_fatal("detach", GetLastError());
}


// --------------------------------------------------------------------------------
// Mutexes
// --------------------------------------------------------------------------------

/** Create a mutex object.

    Mutexes have single-ownership semantics and must be finalized by passing it
    to a destroy function.
 */
doremir_thread_mutex_t doremir_thread_create_mutex()
{
    doremir_thread_mutex_t mutex = malloc(sizeof(struct _doremir_thread_mutex_t));

    HANDLE result = CreateMutex(NULL, false, NULL);

    if (!result)
        doremir_thread_fatal("create_mutex", GetLastError());
    
    mutex->native = result;
    return mutex;
}

/** Destroy a mutex object.
 */
void doremir_thread_destroy_mutex(doremir_thread_mutex_t mutex)
{
    BOOL result = CloseHandle(mutex->native); // FIXME
    free(mutex);

    if (!result)
        doremir_thread_fatal("destroy_mutex", GetLastError());
}

/** Acquire the lock of a mutex object.
 */
bool doremir_thread_lock(doremir_thread_mutex_t mutex)
{
    // TODO
    // need to do DuplicateHandle or similar
}

/** Try acquiring the lock of a mutex object.
 */
bool doremir_thread_try_lock(doremir_thread_mutex_t mutex)
{
    // TODO
}

/** Release the lock of a mutex object.
 */
bool doremir_thread_unlock(doremir_thread_mutex_t mutex)
{
    // TODO
}







// --------------------------------------------------------------------------------
// Utility
// --------------------------------------------------------------------------------

void doremir_thread_fatal(char* msg, int error)
{
    // TODO log
    printf("Fatal error: Doremir: Thread: %s: %d\n", msg, error);
    exit(error);
}
