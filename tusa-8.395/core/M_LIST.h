#ifndef M_LIST__HH2
#define M_LIST__HH2
#include "_mutex.h"
#include <list>
template <class T1>
class M_LIST
{
    std::list<T1> container;
    _mutex Mutex;
public:
    M_LIST():Mutex("M_LIST") {}
    std::list<T1> get_container() const
    {
        MUTEX_LOCK kaka(Mutex);
        return container;
    }
    void assign(const std::list<T1>& m)
    {
        MUTEX_LOCK kaka(Mutex);
        container=m;
    }
    void push_back(const T1& m)
    {
        MUTEX_LOCK kaka(Mutex);
        container.push_back(m);
    }
    T1 pop_front()
    {

        MUTEX_LOCK kaka(Mutex);
        T1 ret;
        if(container.size())
        {
            auto i=container.begin();
            ret=*i;
            container.erase(i);
        }
        return ret;
    }
    size_t size() const
    {
        MUTEX_LOCK kaka(Mutex);
        return container.size();
    }
    void clear()
    {
        MUTEX_LOCK kaka(Mutex);
        container.clear();
    }
    bool extract_first(T1 & conn)
    {
        MUTEX_LOCK kaka(Mutex);
        if (container.size() > 0) {
            conn = *container.begin();
            container.erase(container.begin());
            return true;
        }
        else {
            return false;
        }
        return false;
    }
#ifndef WIN32
    bool try_extract_first(T1 & conn)
    {
        MUTEX_TRY_LOCK kaka(Mutex);
        if(!kaka.result)
        {
            if (container.size() > 0) {
                conn = *container.begin();
                container.erase(container.begin());
                return true;
            }
            else {
                return false;
            }
        }
        return false;
    }
#endif

};
#endif
