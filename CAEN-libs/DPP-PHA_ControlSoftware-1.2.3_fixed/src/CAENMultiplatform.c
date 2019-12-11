#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "CAENMultiplatform.h"
#include <CAENLogger.h>

#ifndef WIN32
#include<spawn.h>
//#include<sys/semaphore_wait.h>
#include <sys/wait.h>
#include <sys/types.h>
extern char **environ;
#endif

INIT_C_LOGGER("CAENMultiplatform.log", "CAENMultiplatform.c");

#ifndef WIN32
    static struct termios g_old_kbd_mode;

    /*****************************************************************************/
    static void cooked(void)
    {
	    tcsetattr(0, TCSANOW, &g_old_kbd_mode);
    }

    static void raw(void)
    {
	    static char init;
    /**/
	    struct termios new_kbd_mode;

	    if(init)
		    return;
    /* put keyboard (stdin, actually) in raw, unbuffered mode */
	    tcgetattr(0, &g_old_kbd_mode);
	    memcpy(&new_kbd_mode, &g_old_kbd_mode, sizeof(struct termios));
	    new_kbd_mode.c_lflag &= ~(ICANON | ECHO);
	    new_kbd_mode.c_cc[VTIME] = 0;
	    new_kbd_mode.c_cc[VMIN] = 1;
	    tcsetattr(0, TCSANOW, &new_kbd_mode);
    /* when we exit, go back to normal, "cooked" mode */
	    atexit(cooked);

	    init = 1;
    }

    /*****************************************************************************/
    /*  SLEEP  */
    /*****************************************************************************/
    void Sleep(int t) {
        usleep( t*1000 );
    }

    /*****************************************************************************/
    /*  GETCH  */
    /*****************************************************************************/
    int _getch(void)
    {
	    unsigned char temp;

	    raw();
        /* stdin = fd 0 */
	    if(read(0, &temp, 1) != 1)
		    return 0;
	    return temp;

    }


    /*****************************************************************************/
    /*  KBHIT  */
    /*****************************************************************************/
    int _kbhit()
    {

	    struct timeval timeout;
	    fd_set read_handles;
	    int status;

	    raw();
        /* check stdin (fd 0) for activity */
	    FD_ZERO(&read_handles);
	    FD_SET(0, &read_handles);
	    timeout.tv_sec = timeout.tv_usec = 0;
	    status = select(0 + 1, &read_handles, NULL, NULL, &timeout);
	    if(status < 0)
	    {
		    //printf("select() failed in _kbhit()\n");
		    exit(1);
	    }
        return (status);
    }
#endif

// The variable must be in the form $(VARNAME)
char *getExpandedString(const char *filename) {
    char *result = NULL;
    char *varValue = NULL, *varName=NULL;
    char *p1,*p2;
    size_t varNameLength, resultLength;

    if( strchr(filename, '$') ) {        
        // Get the variable name
        p1 = strchr(filename, '$');
        p2 = strchr(filename, ')');
        if(p2==NULL) {
            result = NULL;
            goto QuitFunction;
        }
        varNameLength = p2-p1-2; // -2 to remove "$("
        varName = (char *)calloc(varNameLength+1, sizeof(char));
        strncpy(varName, p1+2, varNameLength); // +2 to add "$("
        varName[varNameLength] = 0;

        // Get the variable value
        varValue = getenv(varName);
        if(varValue==NULL) {
            result = NULL;
            goto QuitFunction;
        }

        // Build the result
        resultLength = strlen(filename) - (strlen(varName)+3) + strlen(varValue) + 1;
        result = (char*)calloc(resultLength, sizeof(char));
        strncpy(result, filename, p1-filename);
        strcat(result, varValue);
        strcpy(result+strlen(result), p2+1);
        if(strchr(result, '$')) {
            char* tmpResult = getExpandedString(result);
            free(result);
            result = tmpResult;
        }
    }
    else {
        result = (char*)calloc(strlen(filename)+1, sizeof(char));
        strcpy(result, filename);
    }

QuitFunction:
    if(varName!=NULL) {
        free(varName);
    }
    return result;
}

/*int getBaseName(char *_dest, const char *_source) {
    char *tmpRes = (char*)calloc(strlen(_source), sizeof(char));
    char *last = NULL, *p1 = NULL, *p2 = NULL;

    if(!tmpRes)
        return -1;

    logMsg(c_logger_Severity_DEBUG, "_source is: %s", _source);
    //strcpy(tmpRes, _source);
    sprintf(tmpRes, "%s", _source);
    tmpRes=_source;
    logMsg(c_logger_Severity_DEBUG, "tmpRes is: %s", tmpRes);
    p1 = strrchr(tmpRes, '/');
#ifdef WIN32
    // in windows paths may also have '\'!
    p2 = strrchr(tmpRes, '\\'); // NOTE double \ = single \ !!
#endif
    last = p1>p2?p1:p2;
    logMsg(c_logger_Severity_DEBUG, "last index is: %d, value: %c", last-tmpRes, *last);

    // last may still be NULL (if _source don't contain '\' or '/')
    if(last == NULL)
        //strcpy(_dest, "");
        sprintf(_dest, "%s", "");
    else {
        last[1]=0; // setting last[1]=0 means truncate tmpRes to the last '\' or '/'
        //strcpy(_dest, tmpRes);
        sprintf(_dest, "%s", tmpRes);
    }
    logMsg(c_logger_Severity_DEBUG, "_dest is: %s", _dest);
    free(tmpRes);

    return 0;
}*/

int getBaseName(char *_dest, const char *_source) {
    char *last = NULL, *p1 = NULL, *p2 = NULL;

    logMsg(c_logger_Severity_DEBUG, "_source is: %s", _source);
    if(_dest!=_source)
        strcpy(_dest, _source);
    p1 = strrchr(_dest, '/');
#ifdef WIN32
    // in windows paths may also have '\'!
    p2 = strrchr(_dest, '\\'); // NOTE double \ = single \ !!
#endif
    last = p1>p2?p1:p2;
    logMsg(c_logger_Severity_DEBUG, "last index is: %d, value: %c", last-_dest, *last);

    // last may still be NULL (if _source don't contain '\' or '/')
    if(last == NULL)
        //strcpy(_dest, "");
        sprintf(_dest, "%s", "");
    else
        last[1]=0; // setting last[1]=0 means truncate _dest to the last '\' or '/'
    logMsg(c_logger_Severity_DEBUG, "_dest is: %s", _dest);

    return 0;
}

int _fillWrites(char *filename, char *key, Register_t *regs, int *num) {
    int cnt=0;
    FILE *f;
    char str[20];
    char *commentBegin = NULL;

    *num = 0;
    if(filename == NULL) {
        return -1;
    }
    f=fopen(filename, "r");
    if(!f) {
        return -2;
    }

    while(!feof(f)) {
        int tmp_fscnfres; // to avoid compiler warnings

        tmp_fscnfres = fscanf(f, "%s", str);
        commentBegin = strchr(str, '#');
        if(commentBegin != NULL) {
            char line[1000];
            char *tmp; // to avoid compiler warnings

            *commentBegin = 0;
            tmp = fgets(line, 1000, f);
        }
        if(strcmp(str, key) == 0) {
            if(cnt>=MAX_FW_NUM) {
                // Too much free writes!
                logMsg(c_logger_Severity_WARNING, "Maximum Number of free writes (%d) reached", MAX_FW_NUM);
                break;
            }
            strcpy(str, "");
            tmp_fscnfres = fscanf(f, "%x", &regs[cnt].address);
            tmp_fscnfres = fscanf(f, "%x", &regs[cnt].data);
            tmp_fscnfres = fscanf(f, "%x", &regs[cnt].mask);
            cnt++;
        }
    }
    *num = cnt;

    fclose(f);
    return 0;
}

int getFreeWritesFromFile(char *filename, Register_t *regs, int *num) {
    int ret = 0;
    ret = _fillWrites(filename, "GENERIC_WRITE", regs, num);
    return ret;
}

int getBroadcastWritesFromFile(char *filename, Register_t *regs, int *num) {
    // here we suppose that the address is of type 0xY0YY
    // we will fill the broadcast write struct in a sigle entry
    // it must be care of the user to manage the writes
    int ret = 0;
    ret = _fillWrites(filename, "BROADCAST_WRITE", regs, num);
    return ret;
}

#ifdef _ENABLE_THREADS_
int _isProcValid(c_Process_t *proc) {
    if(proc != NULL)
        return 1;
    else
        return 0;
}

c_Process_t *c_newProc() {
    c_Process_t *proc = (c_Process_t *)calloc(1, sizeof(c_Process_t));
    if(proc == NULL)
        goto QuitFunction;

    proc->argc = 0;
    proc->execName = NULL;
    proc->argv[proc->argc]=NULL;

QuitFunction:
    return proc;
}

int c_setProcExec(char *exec, c_Process_t *proc) {
    if(!_isProcValid(proc))
        return -1;
    if((proc->execName = (char*)calloc(strlen(exec)+1, sizeof(char)))==NULL)
        return -2;

    strcpy(proc->execName, exec);
    if(proc->argv[0] == NULL)
        c_addProcArg(proc->execName, proc);
    else {
        free(proc->argv[0]);
        if((proc->argv[0] = (char*)calloc(strlen(proc->execName)+1, sizeof(char)))==NULL)
            return -3;
        strcpy(proc->argv[0], proc->execName);
    }
    return 0;
}

int c_addProcArg(char *arg, c_Process_t *proc) {
    if(!_isProcValid(proc))
        return -1;
    if(proc->argc >= MAX_PROC_NARGS)
        return -2;
    if((proc->argv[proc->argc] = (char*)calloc(strlen(arg)+1, sizeof(char)))==NULL)
        return -3;

    strcpy(proc->argv[proc->argc], arg);
    proc->argv[proc->argc+1]=NULL;
    proc->argc++;
    return 0;
}

#ifdef WIN32
int __uninitHandlers(c_Process_t *proc) {
    proc->procInfo.dwProcessId=0;
    proc->procInfo.dwThreadId=0;
    proc->procInfo.hProcess=NULL;
    proc->procInfo.hThread=NULL;
    proc->jobObject=NULL;
    return 0;
}

int __startProcessWin(c_Process_t *proc) {
    int res=CAENPROC_RetCode_Success, ok=1, i;
    size_t stringlen=0;
    char *CommandString = NULL;
    JOBOBJECT_EXTENDED_LIMIT_INFORMATION jeli = { 0 };
    STARTUPINFOA                         si   = { 0 };
    char fName[50];
    
    if(!_isProcValid(proc))
        return -1;
    if(proc->execName==NULL)
        return -2;
    if(strlen(proc->execName)==0)
        return -2;

    __uninitHandlers(proc);

    stringlen=strlen(proc->execName)+1;
    for(i=1; i<proc->argc; i++) {
        if(proc->argv[i]!=NULL)
            stringlen+=strlen(proc->argv[i])+1;
        else
            break;
    }
    if( (CommandString = (char*)calloc(stringlen+5, sizeof(char)))==NULL )
        return -3;
    strcpy(CommandString, proc->execName);
    for(i=1; i<proc->argc; i++) {
        if(proc->argv[i]!=NULL) {
            strcat(CommandString, " ");
            strcat(CommandString, proc->argv[i]);
        }
        else
            break;
    }

    proc->jobObject = CreateJobObject(NULL, NULL);
    if(proc->jobObject==NULL) {
        proc->lastErr = GetLastError();
        strcpy(fName, "CreateJobObject");
        res = CAENPROC_RetCode_ProcessFail;
        goto QuitFunction;
    }
    jeli.BasicLimitInformation.LimitFlags = JOB_OBJECT_LIMIT_KILL_ON_JOB_CLOSE;
    if(SetInformationJobObject(proc->jobObject, JobObjectExtendedLimitInformation, &jeli, sizeof(jeli))==FALSE) {
        proc->lastErr = GetLastError();
        strcpy(fName, "SetInformationJobObject");
        res = CAENPROC_RetCode_ProcessFail;
        goto QuitFunction;
    }
    //proc->startInfo.cb = sizeof(proc->startInfo);
    si.cb = sizeof(si);

    ok = CreateProcessA(NULL,
                        (LPSTR)CommandString,
						NULL,              // process security attributes 
						NULL,              // primary thread security attributes 
						FALSE,              // handles are inherited 
						CREATE_SUSPENDED+CREATE_BREAKAWAY_FROM_JOB+CREATE_NO_WINDOW,  // creation flags 
						NULL,              // use parent's environment 
						NULL,              // use parent's current directory 
						&si,  // STARTUPINFO pointer 
						&proc->procInfo);  // receives PROCESS_INFORMATION
    if (!ok) {
        proc->lastErr = GetLastError();
        strcpy(fName, "CreateProcess");
        res = CAENPROC_RetCode_ProcessFail;
        goto QuitFunction;
    }

    if(AssignProcessToJobObject(proc->jobObject, proc->procInfo.hProcess)==FALSE) {
        proc->lastErr = GetLastError();
        strcpy(fName, "AssignProcessToJobObject");
        res = CAENPROC_RetCode_ProcessFail;
        goto QuitFunction;
    }

    ResumeThread(proc->procInfo.hThread);

QuitFunction:
    if(res!=CAENPROC_RetCode_Success) {
        logMsg(c_logger_Severity_DEBUG, "Error in __startProcessWin: Function %s exited with code %d", fName, proc->lastErr);
        c_endProcess(proc);
    }
    return res;
}

int __endProcessWin(c_Process_t *proc) {
    uint32_t ret=0;
    if(!_isProcValid(proc))
        return -1;

    if(proc->procInfo.hProcess!=NULL) {
        TerminateProcess(proc->procInfo.hProcess, 0);
	    CloseHandle(proc->procInfo.hProcess);
    }
    if(proc->procInfo.hThread!=NULL)
        CloseHandle(proc->procInfo.hThread);
    if(proc->jobObject!=NULL) {
        TerminateJobObject(proc->jobObject, ret);
        CloseHandle(proc->jobObject);
    }

    __uninitHandlers(proc);

    return 0;
}

int __getNCPUWin() {
    SYSTEM_INFO info;
    GetSystemInfo(&info);
    return info.dwNumberOfProcessors;
}

int __setThreadAffinityWin(c_thread_t *thr, uint64_t mask) {
    DWORD_PTR MP;
    if(SetThreadAffinityMask(*thr, mask)==0)
        return GetLastError();
    else
        return CAENPROC_RetCode_Success;
}

/*int __isMutexLockedWin(mutex_t *m) {
    return (m->LockCount > -1);
}*/

#else
// WARNING, for some reason code definition of _GNU_SOURCE doesn't work, one have to give it to gcc in the Makefile
//#define _GNU_SOURCE 1
int __startProcessLin(c_Process_t *proc) {
    int res;
    res = posix_spawn(&proc->pid, proc->execName, NULL, NULL, proc->argv, environ);
    return res;
}

int __endProcessLin(c_Process_t *proc) {
    int status;

    if(!_isProcValid(proc))
        return -1;

    waitpid(proc->pid, &status, 0);
    return 0;
}

int __getNCPULin() {
    int ret = -1;
#ifdef _SC_NPROCESSORS_ONLN
    ret = sysconf(_SC_NPROCESSORS_ONLN);
#else
    ret = -1;
#endif
    return ret;
}

int __setThreadAffinityLin(c_thread_t *thr, uint64_t mask) {
    cpu_set_t cpuset;
    int i;
    int s;

    CPU_ZERO(&cpuset);
    for(i=0; i<sizeof(mask)*8; i++)
        if((mask>>i)&0x1)
            CPU_SET(i, &cpuset);
    s = pthread_setaffinity_np(*thr, sizeof(cpu_set_t), &cpuset);
    if(s==0)
        return CAENPROC_RetCode_Success;
    else
        return CAENPROC_RetCode_ProcessFail;
}

/*int __isMutexLockedLin(mutex_t *m) {
    int ret;
    if(ret = pthread_mutex_trylock(m) == 0) {
        unlock(m);
    }
    return ret;
}*/
#endif


int c_startProcess(c_Process_t *proc) {
#ifdef WIN32
    return __startProcessWin(proc);
#else
    return __startProcessLin(proc);
#endif
}

int c_endProcess(c_Process_t *proc) {
    int ret=CAENPROC_RetCode_Success;
#ifdef WIN32
    ret = __endProcessWin(proc);
#else
    ret = __endProcessLin(proc);
#endif
    return ret;
}

c_Process_t *c_freeProc(c_Process_t *proc) {
    int i;

    if(proc == NULL)
        return NULL;

    for(i=0; i<proc->argc; i++) {
        if(proc->argv[i]!=NULL)
            free(proc->argv[i]);
        else
            break;
    }
    if(proc->execName != NULL)
        free(proc->execName);
    proc->argc=0;

#ifdef WIN32
    __uninitHandlers(proc);
#endif

    return NULL;
}

int c_getNCPU() {
    int ret=CAENPROC_RetCode_Success;
#ifdef WIN32
    ret = __getNCPUWin();
#else
    ret = __getNCPULin();
#endif
    return ret;
}

int c_setThreadAffinity(c_thread_t *thr, uint64_t mask) {
    int ret=CAENPROC_RetCode_Success;
#ifdef WIN32
    ret = __setThreadAffinityWin(thr, mask);
#else
    ret = __setThreadAffinityLin(thr, mask);
#endif
    return ret;
}

c_thread_t c_getCurrentThread() {
#ifdef WIN32
    return GetCurrentThread();
#else
    return pthread_self();
#endif
}

/*int c_isMutexLocked(mutex_t *m) {
#ifdef WIN32
    return __isMutexLockedWin(m);
#else
    return __isMutexLockedLin(m);
#endif
}*/
#endif

#ifdef _ENABLE_SOCKETS_
#ifdef WIN32
WSADATA _G_wsaData;
#endif

int c_sockInit() {
    int res = 0;

#ifdef WIN32
    res = WSAStartup(MAKEWORD(2,2), &_G_wsaData);
#endif
    return res;
}

int c_sockCleanUp() {
#ifdef WIN32
    WSACleanup();
#endif
    return 0;
}
#endif

int c_pause() {
    //printf("Press any key to continue\n");
    _getch();
    return 0;
}
