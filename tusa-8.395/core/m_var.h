
#ifndef m_var_H
#define m_var_H
#include "_mutex.h"
template <class T>
class m_var
{
    T container;
    _mutex Mutex;
public:

    operator T() const
    {
        MUTEX_LOCK kaka(Mutex);
        return container;
    }
    T operator= (const T& v)
    {
        MUTEX_LOCK kaka(Mutex);
        container=v;
        return container;
    }
    T operator++()
    {
        MUTEX_LOCK kaka(Mutex);
        container++;
        return container;

    }
    T operator--()
    {
        MUTEX_LOCK kaka(Mutex);
        container--;
        return container;
    }
    T operator+=(const T& v)
    {
        MUTEX_LOCK kaka(Mutex);
        container+=v;
        return container;

    }
    T operator-=(const T &v)
    {
        MUTEX_LOCK kaka(Mutex);
        container-=v;
        return container;

    }
    T operator|=(const T &v)
    {
        MUTEX_LOCK kaka(Mutex);
        container|=v;
        return container;

    }
    bool operator==(const T &v) const
    {
        MUTEX_LOCK kaka(Mutex);
        return container==v;

    }
    bool operator!=(const T &v) const
    {
        MUTEX_LOCK kaka(Mutex);
        return container!=v;

    }
    int operator>(const T &v) const
    {
        MUTEX_LOCK kaka(Mutex);
        return container>v;

    }
    int operator<(const T &v) const
    {
        MUTEX_LOCK kaka(Mutex);
        return container<v;

    }
    int operator>=(const T &v) const
    {
        MUTEX_LOCK kaka(Mutex);
        return container>=v;

    }
    int operator<=(const T &v) const
    {
        MUTEX_LOCK kaka(Mutex);
        return container<=v;

    }
    m_var():Mutex("m_var") {
    }
    m_var(T v):Mutex("m_var")
    {
        container=v;
    }
    T get() const
    {
        MUTEX_LOCK kaka((const _mutex*)&Mutex);
        return container;
    }
    void set(T i)
    {
        MUTEX_LOCK kaka(Mutex);
        container=i;
    }
    T inc()
    {
        MUTEX_LOCK kaka(Mutex);
        container++;
        return container;
    }
    T add(const T &n)
    {
        MUTEX_LOCK kaka(Mutex);
        container+=n;
        return container;
    }
    int size()
    {
        MUTEX_LOCK kaka(Mutex);
        return container.size();
    }
};

template <class T>
bool  operator == (const T & a, m_var<T> & b) {
    return b==a;
}
template <class T>
bool  operator != (const T & a, m_var<T> & b) {
    return b!=a;
}

template <class T>
int  operator < (const T & a, const m_var<T> & b) {
    return b>a;
}
template <class T>
int  operator > (const T & a, const m_var<T> & b) {
    return b<a;
}
template <class T>
int  operator <= (const T & a, const m_var<T> & b) {
    return b>=a;
}
template <class T>
int  operator >= (const T & a, const m_var<T> & b) {
    return b<=a;
}

template <class T>
bool operator ! (const m_var<T> & b) {
    return !b.get();
}


#endif
