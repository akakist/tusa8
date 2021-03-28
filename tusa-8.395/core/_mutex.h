
#ifndef __MUTEX__H123321
#define __MUTEX__H123321

#ifdef WITH_MUTEXES
#else
#	ifdef NO_MUTEXES
#	else
#	error you must use WITH_MUTEXES  or NO_MUTEXES
#	endif
#endif

#ifdef WIN32
#endif


#ifndef WIN32
#include <pthread.h>
#endif
#include <string>

#ifdef WIN32

#include <string>
#include <windows.h>



typedef HANDLE pthread_t;
typedef void pthread_attr_t;
pthread_t pthread_self();
int pthread_create(pthread_t *,pthread_attr_t*, void*(f)(void *),void*);
void usleep(unsigned long);
void sleep(unsigned long);
/* end of win32 wrappers*/
#endif
#ifndef _WIN32
#include <semaphore.h>
#endif

class _mutex
{
private:
    _mutex(const _mutex&);             // Not defined to prevent usage
    _mutex& operator=(const _mutex&);  // Not defined to prevent usage
#ifdef WITH_MUTEXES
//			const std::string& name;
#ifdef WIN32
    CRITICAL_SECTION m_;
#else
    pthread_mutex_t m_;
#endif
#endif
public:
    _mutex(const std::string& n="");
//			_mutex(){}
    ~_mutex();
    void lock();
    void unlock();
#ifndef WIN32
    int trylock();
#endif
};


class MUTEX_LOCK
{
    /*  stack object to lock mutex
    	in constructor - locks
    	in destructor unlocks
    */
public:
    explicit MUTEX_LOCK(const _mutex& m)
#ifdef WITH_MUTEXES
        : m_((_mutex*)&m)
#endif
    {
#ifdef WITH_MUTEXES
        m_->lock();
#endif
    }
    explicit MUTEX_LOCK(const _mutex* m)
#ifdef WITH_MUTEXES
        : m_((_mutex*)m)
#endif
    {
#ifdef WITH_MUTEXES
        m_->lock();
#endif
    }
    ~MUTEX_LOCK() {
#ifdef WITH_MUTEXES
        m_->unlock();
#endif
    }
private:
#ifdef WITH_MUTEXES
    _mutex* const m_;
#endif
    MUTEX_LOCK(const MUTEX_LOCK&);             // Not defined
    MUTEX_LOCK& operator=(const MUTEX_LOCK&);  // Not defined
};
#ifndef WIN32
class MUTEX_TRY_LOCK
{
    /*  stack object to try lock mutex
    	result - indicates success of lock
    	if locked result==0
    	else result !=0
    */
public:
    explicit MUTEX_TRY_LOCK(_mutex& m)
#ifdef WITH_MUTEXES
        : m_(&m)
#endif
    {
#ifdef WITH_MUTEXES
        result=m_->trylock();
#endif
    }
    explicit MUTEX_TRY_LOCK(_mutex* m)
#ifdef WITH_MUTEXES
        : m_(m)
#endif
    {
#ifdef WITH_MUTEXES
        result=m_->trylock();
#endif
    }
    ~MUTEX_TRY_LOCK() {
#ifdef WITH_MUTEXES
        if(!result)m_->unlock();
#endif
    }
#ifdef WITH_MUTEXES
    int result;
#endif
private:
#ifdef WITH_MUTEXES
    _mutex* m_;
#endif
    MUTEX_TRY_LOCK(const MUTEX_TRY_LOCK&);             // Not defined
    MUTEX_TRY_LOCK& operator=(const MUTEX_TRY_LOCK&);  // Not defined
};
#endif

#define M_LOCK(a) MUTEX_LOCK kall(a);

#ifdef DEBUG
#define CONTAINER(a) a.container
#else
#define CONTAINER(a) a
#endif



void free_memory();
void add_free_handler(void(*p)());

#endif
