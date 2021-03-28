#ifndef __MUTEX__H1111
#define __MUTEX__H1111
#ifndef WIN32
#include <pthread.h>
#endif



class _mutex1
{
private:
    _mutex1(const _mutex1&);             // Not defined to prevent usage
    _mutex1& operator=(const _mutex1&);  // Not defined to prevent usage
#ifdef WIN32
    CRITICAL_SECTION m_;//[6];
#else
    pthread_mutex_t m_;
#endif
public:
    _mutex1();
    ~_mutex1();
    void lock();
    void unlock();
#ifndef WIN32
    int trylock();
#endif
};
class MUTEX_LOCK1
{
public:
    explicit MUTEX_LOCK1(const _mutex1& m) : m_((_mutex1*)&m) {
        m_->lock();
    }
    explicit MUTEX_LOCK1(const _mutex1* m) : m_((_mutex1*)m) {
        m_->lock();
    }
    ~MUTEX_LOCK1() {
        m_->unlock();
    }
private:
    _mutex1* const m_;
    MUTEX_LOCK1(const MUTEX_LOCK1&);             // Not defined
    MUTEX_LOCK1& operator=(const MUTEX_LOCK1&);  // Not defined
};

#endif
