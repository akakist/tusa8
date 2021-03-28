/*
core
(c) belialov.com
*/
#ifndef __MUTEX__H
#define __MUTEX__H
#ifndef WIN32
#include <pthread.h>
#endif
#include <string>

#ifdef WIN32
/*
win32 wrappers
*/
typedef HANDLE pthread_t;
typedef unsigned long pid_t;
typedef void pthread_attr_t;
pthread_t pthread_self();
int pthread_create(pthread_t *,pthread_attr_t*, void*(f)(void *),void*);
void usleep(unsigned long);
void sleep(unsigned long);
/* end of win32 wrappers*/
#endif
		class _mutex
		{
		private:
			_mutex(const _mutex&);             // Not defined to prevent usage
			_mutex& operator=(const _mutex&);  // Not defined to prevent usage
#ifdef WIN32
			CRITICAL_SECTION m_;
#else
			pthread_mutex_t m_;
#endif
		public:
			_mutex();~_mutex(); void lock(); void unlock();
#ifndef WIN32
			int trylock(); 
#endif			
		};


	class MUTEX_LOCK
	{
	/*
		stack object to lock mutex
		in constructor - locks
		in destructor unlocks
	*/
	public:
		explicit MUTEX_LOCK(const _mutex& m) : m_((_mutex*)&m) {m_->lock();}
		explicit MUTEX_LOCK(const _mutex* m) : m_((_mutex*)m) {m_->lock();}
		~MUTEX_LOCK() {m_->unlock();}
	private:
		_mutex* const m_;
		MUTEX_LOCK(const MUTEX_LOCK&);             // Not defined
		MUTEX_LOCK& operator=(const MUTEX_LOCK&);  // Not defined
	};
#ifndef WIN32
	class MUTEX_TRY_LOCK
	{
	/*
		stack object to try lock mutex
		result - indicates success of lock
		if locked result==0
		else result !=0
	*/
	public:
		explicit MUTEX_TRY_LOCK(_mutex& m) : m_(&m) {result=m_->trylock();}
		explicit MUTEX_TRY_LOCK(_mutex* m) : m_(m) {result=m_->trylock();}
		~MUTEX_TRY_LOCK() {if(!result)m_->unlock();}
		int result;
	private:
		_mutex* m_;
		MUTEX_TRY_LOCK(const MUTEX_TRY_LOCK&);             // Not defined
		MUTEX_TRY_LOCK& operator=(const MUTEX_TRY_LOCK&);  // Not defined
	};
#endif	

#endif
