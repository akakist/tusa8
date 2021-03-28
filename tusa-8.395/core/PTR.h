#ifndef PTR__H
#define PTR__H
#include "_mutex.h"
#include "errs.h"
#include <stdio.h>
#include <map>
#include <list>
#include "m_var.h"
#ifdef WIN32
#include <time.h>
#endif
#include <string>
#include "mutex_inspector.h"
#include "lockctl.h"

class ptr_error: public cError
{
    ptr_error(const std::string &s):cError(s) {}
};
template <class T>
class PTR_getter;
class base_container
{
    /*base class for PTR_getter class
    implements refcount, last_change_time,last_access_time
    */
    friend class base_container_getter;
    friend class base_container_lock_release;
    friend class PTR_getter<base_container>;
    base_container(const base_container&);         // Not defined  to prevent usage
    base_container& operator=(const base_container&);// Not defined  to prevent usage

public:
    virtual const char* getclassname()const =0;
    _mutex __dataMuteX;
    time_t __last_change_time;
    time_t __last_access_time;
private:
    _mutex __privateMuteX;
    signed short __Ref_Count;

public:
    base_container(const std::string& n);
    signed short __Inc_Ref_Count();
    signed short __Dec_Ref_Count();
    signed short get_ref_count() const {
        return __Ref_Count;
    };
    virtual ~base_container();
};

class
#ifdef WIN32
    __declspec(dllexport)
#endif
    base_container_nm
{
    /*base class for PTR_getter class
    implements refcount, last_change_time,last_access_time
    */
    base_container_nm(const base_container_nm&);         // Not defined  to prevent usage
    base_container_nm& operator=(const base_container_nm&);// Not defined  to prevent usage

public:
    virtual const char* getclassname()const =0;
private:
    signed short __Ref_Count;

public:
    base_container_nm();
    signed short __Inc_Ref_Count();
    signed short __Dec_Ref_Count();
    signed short get_ref_count() const {
        return __Ref_Count;
    };
    virtual ~base_container_nm();
};

template <class T>
class PTR_getter_lock_write;

template <class T>
class PTR_getter
{
    /*	template to operate with refcounted PTRs
    */

protected:
    friend class PTR_getter_lock_write<T>;
public:
    T* ___ptr;
    int operator < (const PTR_getter& l) const
    {
        return ___ptr<l.___ptr;
    }
    operator bool() const
    {
        return ___ptr != NULL;
    }
    bool operator !() const
    {
        return ___ptr == NULL;
    }
    PTR_getter(const T * p):___ptr((T *) p)
    {
        if (___ptr) {
            ___ptr->__Inc_Ref_Count();
        }
    }
    PTR_getter(const PTR_getter < T > &bcg)
    {
        ___ptr = bcg.___ptr;
        if (___ptr) {
            ___ptr->__Inc_Ref_Count();
        }
    }
    ~ PTR_getter()
    {
        int count_val = 0;
        if (___ptr) {
            {
                count_val = ___ptr->__Dec_Ref_Count();
            }
            if (count_val == 0)
            {
                LEAKCTL_REMOVE(___ptr);
                delete ___ptr;
            }
            if (count_val < 0)
                printf("refcount underflow\n");
        }
    }
    int operator==(const PTR_getter & bcg)const
    {
        return ___ptr==bcg.___ptr;
    }
    PTR_getter & operator = (const PTR_getter & bcg)
    {
        if (this != &bcg) {
            if (bcg.___ptr) {
                bcg.___ptr->__Inc_Ref_Count();
            }

            if (___ptr) {
                {
                    int count_val = 0;
                    {
                        count_val = ___ptr->__Dec_Ref_Count();
                    }
                    if (count_val == 0)
                    {
                        LEAKCTL_REMOVE(___ptr);
                        delete ___ptr;
                    }
                    if (count_val < 0)
                        throw cError("refcount underflow");

                }
            }
            ___ptr = bcg.___ptr;
        }
        return *this;
    }
    T* __get__PTR__() {
        return ___ptr;
    }
};

template <class T>
class PTR_getter_nm
{
    /*	template to operate with refcounted PTRs
    */

protected:
public:
    T* ___ptr;
    int operator < (const PTR_getter_nm& l) const
    {
        return ___ptr<l.___ptr;
    }
    operator bool() const
    {
        return ___ptr != NULL;
    }
    bool operator !() const
    {
        return ___ptr == NULL;
    }
    PTR_getter_nm(const T * p):___ptr((T *) p)
    {
        if (___ptr) {
            ___ptr->__Inc_Ref_Count();
        }
    }
    PTR_getter_nm(const PTR_getter_nm < T > &bcg)
    {
        ___ptr = bcg.___ptr;
        if (___ptr) {
            ___ptr->__Inc_Ref_Count();
        }
    }
    ~ PTR_getter_nm()
    {
        int count_val = 0;
        if (___ptr) {
            {
                count_val = ___ptr->__Dec_Ref_Count();
            }
            if (count_val == 0)
            {
                LEAKCTL_REMOVE(___ptr);
                delete ___ptr;
            }
            if (count_val < 0)
                printf("refcount underflow\n");
        }
    }
    int operator==(const PTR_getter_nm & bcg)const
    {
        return ___ptr==bcg.___ptr;
    }
    PTR_getter_nm & operator = (const PTR_getter_nm & bcg)
    {
        if (this != &bcg) {
            if (bcg.___ptr) {
                bcg.___ptr->__Inc_Ref_Count();
            }

            if (___ptr) {
                {
                    int count_val = 0;
                    {
                        count_val = ___ptr->__Dec_Ref_Count();
                    }
                    if (count_val == 0)
                    {
                        LEAKCTL_REMOVE(___ptr);
                        delete ___ptr;
                    }
                    if (count_val < 0)
                        throw cError("refcount underflow");

                }
            }
            ___ptr = bcg.___ptr;
        }
        return *this;
    }
    T* __get__PTR__() {
        return ___ptr;
    }
};

template <class T>
class PTR_getter_lock_write
{
    /*
    	template class write accessor to PTR_getters
    	thread safe and shared safe
    */
private:
    PTR_getter_lock_write(const PTR_getter_lock_write& p);// Not defined  to prevent usage
    PTR_getter_lock_write& operator=(const PTR_getter_lock_write& p);// Not defined  to prevent usage
public:
    PTR_getter<T> gt;
public:
    PTR_getter_lock_write(const PTR_getter<T> &p):gt((PTR_getter<T>)p)
    {
        if(!gt.___ptr) throw cError ("if(!gt.___ptr) "+_DMI());
        gt.___ptr->__dataMuteX.lock();
        time_t t=time(NULL);
        gt.___ptr->__last_change_time=t;
        gt.___ptr->__last_access_time=t;
    }
    ~PTR_getter_lock_write()
    {
        gt.___ptr->__dataMuteX.unlock();
    }
    T* operator->()
    {
        if(!gt.___ptr) throw cError ("if(!gt.___ptr) "+_DMI());
        return (T*)gt.___ptr;
    }
    operator bool () {
        return gt.___ptr!=NULL;
    }
    bool operator ! () {
        return gt.___ptr==NULL;
    }
};


template <class T>
class PTR_getter_lock_write_no_lockctl
{

private:
    PTR_getter_lock_write_no_lockctl(const PTR_getter_lock_write_no_lockctl& p);// Not defined  to prevent usage
    PTR_getter_lock_write_no_lockctl& operator=(const PTR_getter_lock_write_no_lockctl& p);// Not defined  to prevent usage
#ifdef DEBUG

#endif
public:
    PTR_getter<T> gt;
public:
    PTR_getter_lock_write_no_lockctl(const PTR_getter<T> &p):gt((PTR_getter<T>)p)
    {
        if(!gt.___ptr) throw cError ("if(!gt.___ptr) "+_DMI());
        gt.___ptr->__dataMuteX.lock();
        time_t t=time(NULL);
        gt.___ptr->__last_change_time=t;
        gt.___ptr->__last_access_time=t;
    }
    ~PTR_getter_lock_write_no_lockctl()
    {
        gt.___ptr->__dataMuteX.unlock();
    }
    T* operator->()
    {
        if(gt.___ptr==NULL) throw cError("if(gt.___ptr==NULL)");
        return (T*)gt.___ptr;
    }
    operator bool () {
        return gt.___ptr!=NULL;
    }
    bool operator ! () {
        return gt.___ptr==NULL;
    }
};


template <class T>
class PTR_getter_lock_write_no_access
{
private:
    PTR_getter_lock_write_no_access(const PTR_getter_lock_write_no_access& p);// Not defined  to prevent usage
    PTR_getter_lock_write_no_access& operator=(const PTR_getter_lock_write_no_access& p);// Not defined  to prevent usage
#ifdef DEBUG

#endif


public:
    PTR_getter<T> gt;
public:
    PTR_getter_lock_write_no_access(const PTR_getter<T> &p):gt((PTR_getter<T>)p)
    {
        if(!gt.___ptr) throw cError("if(!gt.___ptr) "+_DMI());
        gt.___ptr->__dataMuteX.lock();
    }
    ~PTR_getter_lock_write_no_access()
    {
        gt.___ptr->__dataMuteX.unlock();
    }
    T* operator->()
    {
        if(gt.___ptr==NULL) throw cError("if(gt.___ptr==NULL)");
        return (T*)gt.___ptr;
    }
    operator bool () {
        return gt.___ptr!=NULL;
    }
    bool operator ! () {
        return gt.___ptr==NULL;
    }
};


template <class T>
class PTR_getter_lock_read
{


private:
    PTR_getter_lock_read(const PTR_getter_lock_read& p);// Not defined  to prevent usage
    PTR_getter_lock_read& operator=(const PTR_getter_lock_read& p);// Not defined  to prevent usage

public:
    PTR_getter<T> gt;
public:
    PTR_getter_lock_read(const PTR_getter<T> &p):gt((PTR_getter<T>)p)
    {
        if(!gt.___ptr) throw cError("if(!gt.___ptr) "+_DMI());
        gt.___ptr->__dataMuteX.lock();
        gt.___ptr->__last_access_time=time(NULL);
    }
    ~PTR_getter_lock_read()
    {
        gt.___ptr->__dataMuteX.unlock();
    }
    const T* operator->() const
    {
        if(gt.___ptr==NULL) throw cError("if(gt.___ptr==NULL)");
        return (T*)gt.___ptr;
    }
    operator bool () {
        return gt.___ptr!=NULL;
    }
    bool operator ! () {
        return gt.___ptr==NULL;
    }
};


template <class T>
class PTR_getter_lock_read_no_lockctl
{


private:
    PTR_getter_lock_read_no_lockctl(const 		     PTR_getter_lock_read_no_lockctl& p);// Not defined  to prevent usage
    PTR_getter_lock_read_no_lockctl& operator=(const PTR_getter_lock_read_no_lockctl& p);// Not defined  to prevent usage

public:
    PTR_getter<T> gt;
public:
    PTR_getter_lock_read_no_lockctl(const PTR_getter<T> &p):gt((PTR_getter<T>)p)
    {
        if(!gt.___ptr) throw cError("if(!gt.___ptr) "+_DMI());
        gt.___ptr->__dataMuteX.lock();
        gt.___ptr->__last_access_time=time(NULL);
    }
    ~PTR_getter_lock_read_no_lockctl()
    {
        gt.___ptr->__dataMuteX.unlock();
    }
    const T* operator->() const
    {
        if(gt.___ptr==NULL) throw cError("if(gt.___ptr==NULL)");
        return (T*)gt.___ptr;
    }
    operator bool () {
        return gt.___ptr!=NULL;
    }
    bool operator ! () {
        return gt.___ptr==NULL;
    }
};

template <class T>
class PTR_getter_lock_read_no_access
{

private:
    PTR_getter_lock_read_no_access(const PTR_getter_lock_read_no_access& p);// Not defined  to prevent usage
    PTR_getter_lock_read_no_access& operator=(const PTR_getter_lock_read_no_access& p);// Not defined  to prevent usage

public:
    PTR_getter<T> gt;
public:
    PTR_getter_lock_read_no_access(const PTR_getter<T> &p):gt((PTR_getter<T>)p)
    {
        if(!gt.___ptr) throw cError("if(!gt.___ptr) "+_DMI());
        gt.___ptr->__dataMuteX.lock();
    }
    ~PTR_getter_lock_read_no_access()
    {
        gt.___ptr->__dataMuteX.unlock();
    }
    const T* operator->() const
    {
        if(gt.___ptr==NULL) throw cError("if(gt.___ptr==NULL)");
        return (T*)gt.___ptr;
    }
    operator bool () {
        return gt.___ptr!=NULL;
    }
    bool operator ! () {
        return gt.___ptr==NULL;
    }
};
template <class T1,class T2>
class PTR_getter_lock_read2
{

private:
    PTR_getter_lock_read2(const PTR_getter_lock_read2& p);// Not defined  to prevent usage
    PTR_getter_lock_read2& operator=(const PTR_getter_lock_read2& p);// Not defined  to prevent usage


public:
    PTR_getter<T1> gt;
public:
    PTR_getter_lock_read2(const PTR_getter<T1> &p):gt((PTR_getter<T1>)p)
    {
        if(!gt.___ptr) throw cError("if(!gt.___ptr) "+_DMI());
        gt.___ptr->__dataMuteX.lock();
        time_t t=time(NULL);
        gt.___ptr->__last_change_time=t;
        gt.___ptr->__last_access_time=t;
    }
    ~PTR_getter_lock_read2()
    {
        gt.___ptr->__dataMuteX.unlock();
    }
    const T2* operator->() const
    {
        if(gt.___ptr==NULL) throw cError("if(gt.___ptr==NULL)");
        return (T2*)gt.___ptr;
    }
    operator bool () {
        return gt.___ptr!=NULL;
    }
    bool operator ! () {
        return gt.___ptr==NULL;
    }
    operator T2* () {
        return (T2*)gt.___ptr;
    }

};
template <class T1,class T2>
class PTR_getter_lock_write2
{

private:
    PTR_getter_lock_write2(const PTR_getter_lock_write2& p);// Not defined  to prevent usage
    PTR_getter_lock_write2& operator=(const PTR_getter_lock_write2& p);// Not defined  to prevent usage


public:
    PTR_getter<T1> gt;
public:
    PTR_getter_lock_write2(const PTR_getter<T1> &p):gt((PTR_getter<T1>)p)
    {
        if(!gt.___ptr) throw cError("if(!gt.___ptr) "+_DMI());
        gt.___ptr->__dataMuteX.lock();
        time_t t=time(NULL);
        gt.___ptr->__last_change_time=t;
        gt.___ptr->__last_access_time=t;
    }
    ~PTR_getter_lock_write2()
    {
        gt.___ptr->__dataMuteX.unlock();
    }
    T2* operator->() const
    {
        if(gt.___ptr==NULL) throw cError("if(gt.___ptr==NULL)");
        return (T2*)gt.___ptr;
    }
    operator bool () {
        return gt.___ptr!=NULL;
    }
    bool operator ! () {
        return gt.___ptr==NULL;
    }
    operator T2* () {
        return (T2*)gt.___ptr;
    }

};


#define PGLR(a,b,c) if(!c)throw cError("!PGLR %s %d",__FILE__,__LINE__); PTR_getter_lock_read<a> b(c); LOCKCTL;
#define PGLW(a,b,c) if(!c)throw cError("!PGLR %s %d",__FILE__,__LINE__); PTR_getter_lock_write<a> b(c); LOCKCTL;
#define NOTPTR(S) else logErr2(#S "==NULL %s %d\n%s",__FILE__,__LINE__,_DMI().c_str());

#endif
