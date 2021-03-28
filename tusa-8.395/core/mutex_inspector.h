#ifndef _MINSTPKETR_HJ
#define _MINSTPKETR_HJ
#ifndef WIN32
#include <pthread.h>
#endif
#include <string>
#include <map>
#include <list>
#include <deque>
#include <stack>
#include "_mutex.h"
#include "errs.h"
#include "lockctl.h"
#ifdef MUTEX_INSPECTOR_DBUG
class mutex_inspector
{
public:
    mutex_inspector(const char* ff, int ll);
    mutex_inspector(const char* ff, int ll,const char *ss);
    ~mutex_inspector();
};
#define MUTEX_INSPECTOR  mutex_inspector kall12344(__FL__);
#define MUTEX_INSPECTORS(a)  mutex_inspector kall123444(__FL__,a);
#define _DMI() dump_mutex_inspector(pthread_self())
std::string dump_mutex_inspectors();
std::string dump_mutex_inspector(pthread_t pt);
#else
#define MUTEX_INSPECTOR
#define MUTEX_INSPECTORS(a)
std::string itostring(int i);
#define _DMI() std::string((string )__FILE__ " "+itostring(__LINE__))
#endif


#ifdef LOCKCTL_DBUG
class lockctl {
public:
    lockctl(const char* f,int l);
    lockctl();
    ~lockctl();
};
#endif

void log_start_thread(const std::string &thread_name);
#ifdef LEAKCTLDEBUG
void add_leakctlitem(void *_p,const char *_name, const char *_file,int _line);
void add_leakctlitem(void *_p,const char *_file,int _line);
void remove_leakctlitem(void *p,const char *name, const char*file, int line);
void remove_leakctlitem(void *p, const char*file, int line);
string report_leaks();
#define LEAKCTL_REMOVE(a) remove_leakctlitem(a,a->getclassname(),__FILE__,__LINE__)
#define LEAKCTL_ADD(a) add_leakctlitem(a,a->getclassname(),__FILE__,__LINE__)
#define LEAKCTL_REMOVENS(a) remove_leakctlitem(a,__FILE__,__LINE__)
#define LEAKCTL_ADDNS(a) add_leakctlitem(a,__FILE__,__LINE__)
#else
#define LEAKCTL_REMOVE(a)
#define LEAKCTL_ADD(a)
#define LEAKCTL_REMOVENS(a)
#define LEAKCTL_ADDNS(a)
#endif



#endif
