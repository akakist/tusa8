#ifndef M_SETH__hh
#define M_SETH__hh
#include <set>
#include "_mutex.h"
using namespace std;

template <class T1>
class M_SET
{
    set<T1> container;
    _mutex Mutex;
public:
    M_SET():Mutex("M_SET") {}
    set<T1> get_container()
    {
        MUTEX_LOCK kaka(Mutex);
        return container;
    }
    set<T1> extract_all()
    {
        set<T1> r;
        {
            MUTEX_LOCK kaka(Mutex);
            r=container;
            container.clear();
        }
        return r;
    }
    void assign(const set<T1>& m)
    {
        MUTEX_LOCK kaka(Mutex);
        container=m;
    }
    int count(const T1&key)
    {
        MUTEX_LOCK kaka(Mutex);
        return container.count(key);
    }
    void insert(const T1& key)
    {
        MUTEX_LOCK kaka(Mutex);
        container.insert(key);
    }
    void erase(const T1& key)
    {
        MUTEX_LOCK kaka(Mutex);
        container.erase(key);
    }
    void clear()
    {
        MUTEX_LOCK kaka(Mutex);
        container.clear();
    }
    size_t size()
    {
        MUTEX_LOCK kaka(Mutex);
        return container.size();
    }

};

#endif
