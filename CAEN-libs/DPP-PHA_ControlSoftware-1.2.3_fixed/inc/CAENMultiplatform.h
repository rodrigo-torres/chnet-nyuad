#ifndef __CAENMULTIPLATFORM
#define __CAENMULTIPLATFORM

#ifdef __cplusplus
extern "C" {
#endif

#ifdef WIN32
    #include <windows.h>
    #include <conio.h>
	#define CLEARSCR "cls"
#else
    #include <pthread.h>
    #include <semaphore.h>
    #include <sys/types.h>
    #include <sys/socket.h>
    #include <netdb.h>
    #include <arpa/inet.h>
    #include <sys/time.h> /* struct timeval, select() */
    #include <termios.h> /* tcgetattr(), tcsetattr() */
    #include <stdlib.h> /* atexit(), exit() */
    #include <unistd.h> /* read() */
    #include <stdio.h> /* printf() */
    #include <string.h> /* memcpy() */
    #include <errno.h>
    //#include <sched.h>
	#define CLEARSCR "clear"

    /*****************************************************************************/
    /*  SLEEP  */
    /*****************************************************************************/
    void Sleep(int t);

    /*****************************************************************************/
    /*  GETCH  */
    /*****************************************************************************/
    int _getch(void);

    /*****************************************************************************/
    /*  KBHIT  */
    /*****************************************************************************/
    int _kbhit();
#endif

#include <CAENTypes.h>

#define MAX_FW_NUM 100
#define MAX_PROCARG_LEN 10
#define MAX_PROC_NARGS 10

#ifndef max
	#define max( a, b ) ( ((a) > (b)) ? (a) : (b) )
#endif

#ifndef min
	#define min( a, b ) ( ((a) < (b)) ? (a) : (b) )
#endif

#ifdef WIN32
    #define popen  _popen    /* redefine POSIX 'deprecated' popen as _popen */
    #define pclose  _pclose  /* redefine POSIX 'deprecated' pclose as _pclose */
#else
    #define ZeroMemory(DEST, SIZE) memset(DEST, 0, SIZE)
#endif

#ifdef _ENABLE_THREADS_
    #ifdef WIN32 // Windows
        // Mutexs
	    /*#define mutex_t					CRITICAL_SECTION
	    #define lock(m)					EnterCriticalSection(m)
	    #define unlock(m)				LeaveCriticalSection(m)
	    #define trylock(m)				TryEnterCriticalSection(m)
	    #define initmutex(m)			InitializeCriticalSection(m)*/
        typedef HANDLE                  mutex_t;
	    #define lock(m)					WaitForSingleObject(m, INFINITE)
	    #define unlock(m)				ReleaseMutex(m)
	    #define trylock(m)				WaitForSingleObject(m, 1)
	    #define initmutex(m)			(*m = CreateMutex(NULL, FALSE, NULL))==NULL?GetLastError():0
        #define destroymutex(m)         (*m)!=NULL ? (ReleaseMutex(*m)==NULL?NULL:GetLastError()) : NULL
	    
        // Semaphores
        typedef HANDLE                  semaphore_t;
        #define semaphore_wait(s,t)		WaitForSingleObject(*s,t)
	    #define semaphore_post(s)		ReleaseSemaphore(*s,1,NULL)
        #define semaphore_init(s)       (int)((*s = CreateSemaphore(NULL,0,1,NULL))==NULL)
        #define semaphore_destroy(s)    if(*s)((int)(CloseHandle(*s))==TRUE)
        #define c_sleep(nms)            Sleep(nms)
        /*typedef DWORD                   c_thread_t;
        typedef HANDLE                  c_tret_t;*/
        typedef HANDLE                  c_thread_t;
        typedef DWORD                   c_tret_t;
        #define thread_start(r, h, t, arg) *h = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)t, arg, 0, (LPDWORD)&r);
        #define thread_end(r, h)        r = WaitForSingleObject(h,INFINITE);
    #else // LINUX
        //#define _GNU_SOURCE
        //#include <sched.h>
        #include <sys/resource.h>
        
        // Mutexs
	    #define mutex_t					pthread_mutex_t
	    #define lock(m)					pthread_mutex_lock(m)
	    #define trylock(m)				pthread_mutex_trylock(m)
	    #define unlock(m)				pthread_mutex_unlock(m)
	    #define initmutex(m)			pthread_mutex_init(m,NULL)
	    #define destroymutex(m)         pthread_mutex_destroy(m)
	    
        // Semaphores
        #define semaphore_t				sem_t
        #define semaphore_wait(s,t)		sem_wait(s)
	    #define semaphore_post(s)		sem_post(s)
	    #define semaphore_init(s)		sem_init(s,0,0)
        #define semaphore_destroy(s)    sem_destroy(s)
	    #define INFINITE				0
        #define c_sleep(nms)            usleep((int)(nms/1000.0))

        typedef pthread_t c_thread_t;
        typedef int c_tret_t;
        #define thread_start(r, h, t, arg) r = pthread_create(h, NULL, (void * (*)(void *))&t, arg)
        #define thread_end(r, h)        r = pthread_join(h,NULL);
    #endif
    
    typedef enum {
        CAENPROC_RetCode_Success = 0L,
        CAENPROC_RetCode_ProcessFail = -1L
    } CAENPROC_RetCode_t;

    typedef struct {
	    char *execName;
        char *argv[MAX_PROC_NARGS+1];
        int  argc;

    #ifdef WIN32
        PROCESS_INFORMATION procInfo; 
        DWORD lastErr;
        HANDLE jobObject;
    #else
        pid_t pid;
    #endif
    } c_Process_t;

    c_Process_t *c_newProc();
    c_Process_t *c_freeProc(c_Process_t *proc);
    int c_setProcExec(char *exec, c_Process_t *proc);
    int c_addProcArg(char *arg, c_Process_t *proc);
    int c_startProcess(c_Process_t *proc);
    int c_endProcess(c_Process_t *proc);
    int c_getNCPU();
    int c_setThreadAffinity(c_thread_t *thr, uint64_t mask);
    c_thread_t c_getCurrentThread();
    //int c_isMutexLocked(mutex_t *m);
#endif

#ifdef _ENABLE_SOCKETS_
    #ifndef WIN32 // Linux
        typedef int SOCKET;
        #define INVALID_SOCKET -1
        #define SOCKET_ERROR   -1
        #define closesocket(s) close(s);
    #endif

    int c_sockInit();
    int c_sockCleanUp();
#endif

typedef struct {
	uint32_t address;
    uint32_t mask;
	uint32_t data;
} Register_t;

char *getExpandedString(const char *filename);
int getBaseName(char *_dest, const char *_source);
int getFreeWritesFromFile(char *filename, Register_t *regs, int *num);
int getBroadcastWritesFromFile(char *filename, Register_t *regs, int *num);
int c_pause();

#ifdef __cplusplus
}
#endif

#endif