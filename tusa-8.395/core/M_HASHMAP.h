#ifndef M_HASHMAPH__hh
#define M_HASHMAPH__hh
#include <map>
#ifndef WIN32
#include <map>
#endif
#include "_mutex.h"
#include <vector>


template <class T1, class T2>
class M_HASHMAP
{
    std::map<T1,T2> container;
    _mutex Mutex;
public:
    M_HASHMAP():Mutex("M_HASHMAP") {}
    std::map<T1,T2> get_container()
    {
        MUTEX_LOCK kaka(Mutex);
        return container;
    }
    void assign(const std::map<T1,T2>& m)
    {
        MUTEX_LOCK kaka(Mutex);
        container=m;
    }
    T2 get(const T1&key)
    {
        MUTEX_LOCK kaka(Mutex);
        return container[key];
    }
    T2 get_or_null(const T1&key)
    {
        MUTEX_LOCK kaka(Mutex);
        if(container.find(key)==container.end())return NULL;
        return container[key];
    }
    bool is_key_exists(const T1& key)
    {
        MUTEX_LOCK kaka(Mutex);
        if(container.find(key)==container.end())return false;
        else return true;
    }
    size_t  count(const T1& id)
    {
        MUTEX_LOCK kaka(Mutex);
        return container.count(id);
    }
    void insert(const T1& key, const T2& val)
    {
        MUTEX_LOCK kaka(Mutex);
        container.insert(std::pair<T1,T2>(key,val));
    }
    void set(const T1& key, const T2& val)
    {
        MUTEX_LOCK kaka(Mutex);
        container[key]=val;
    }
    void erase_key(const T1& key)
    {
        MUTEX_LOCK kaka(Mutex);
#ifdef WIN32
        map<T1,T2>::iterator i=container.find(key);
#else
        auto i=container.find(key);
#endif
        if(i!=container.end())container.erase(i);
    }
    std::vector<T1> keys()
    {
        MUTEX_LOCK kaka(Mutex);
        std::vector<T1> v;
        for(auto i=container.begin(); i!=container.end(); i++)
        {
            v.push_back((*i).first);
        }
        return v;
    }
    std::vector<T2> values()
    {
        MUTEX_LOCK kaka(Mutex);
        std::vector<T2> v;
        for(auto i=container.begin(); i!=container.end(); i++)
        {
            v.push_back(i->second);
        }
        return v;
    }
    void cleanup( std::vector<T1>(*fn)(const std::map<T1,T2>&),void (*del_value)(T2 &val))
    {
        MUTEX_LOCK kaka(Mutex);
        std::vector<T1> v=fn(container);
        for(unsigned int i=0; i<v.size(); i++)
        {
            auto it=container.find(v[i]);
            if(it!=container.end())
            {
                T2 val=it->second;
                container.erase(it);
                del_value(val);
            }
        }
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
    void inc(T1 key)
    {
        MUTEX_LOCK kaka(Mutex);
        container[key]++;
    }
    void add(T1 key,T2 val)
    {
        MUTEX_LOCK kaka(Mutex);
        container[key]+=val;
    }
    T2 operator[](const T1& s)
    {
        MUTEX_LOCK lala(Mutex);
        return container[s];
    }
    std::map<T1,T2> extract_all()
    {
        MUTEX_LOCK kaka(Mutex);
        std::map<T1,T2> ret=container;
        container.clear();
        return ret;

    }

};

#endif
