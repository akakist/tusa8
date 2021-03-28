#ifndef DB_VAR_BX_BANADMIN_H__
#define DB_VAR_BX_BANADMIN_H__
#include <vector>
#include <string>
#include "str_lib.h"
#include "PTR.h"
#include <time.h>

#include "mysql_extens.h"
#include "_QUERY.h"

#include "oscar_buffer.h"
#include <map>


inline bool is_empty(const unsigned int &n) {
    return n==0;
}
inline bool is_empty(const string& n) {
    return n.size()==0;
}
inline string ToString(const unsigned int &n) {
    return itostring(n);
}
inline string ToString(const string &n) {
    return n;
}
class db_row: public base_container
{
private:
public:
    db_row():
        __last_update_time(time(NULL)),base_container("db_row")
    {
    }
    time_t __last_update_time;

};





template <class T1,class T2>
class db_map
{
    map < T1, PTR_getter < T2 > >container;
    time_t last_drop_time;


public:
    _mutex Mutex;
    string table_name;
    db_map(const string & tablen)
        : table_name(tablen),last_drop_time(time(NULL))
    {
    }
    ~db_map()
    {
    }
    void clear()
    {
        M_LOCK(Mutex);
        container.clear();
    }
    void clear_key(const T1& k)
    {
        M_LOCK(Mutex);
        container.erase(k);
    }
    void init(const T1& id,oscar_buffer &b)
    {
        MUTEX_INSPECTOR;
        T2 *bb = new T2;
        LEAKCTL_ADD(bb);
        if (!bb)throw cError("__@if(!bb)");
        bb->unpack(b);
        {
            MUTEX_INSPECTOR;
            PTR_getter < T2 > bbb(bb);
            {
                M_LOCK(Mutex);
                container.insert(pair < T1, PTR_getter < T2 > >(id, bbb));
            }
        }
    }
    PTR_getter < T2 > find_by_id_create(const T1 & id)
    {
        MUTEX_INSPECTOR;
        PTR_getter < T2 > b(NULL);
#ifdef DEBUG
        if (!is_empty(id))
#endif
        {
            MUTEX_INSPECTOR;
            M_LOCK(Mutex);
            b=get_NM__(id);

        }
        if(b) return b;
        T2 *bb = new T2;
        LEAKCTL_ADD(bb);
        if (!bb)
            throw cError(table_name + " !bb" + _DMI());
        bb->id=id;
        if (bb->load_from_db(table_name)) {
            bb->on_id_not_exists(table_name, id);

        }
        PTR_getter < T2 > bbb(bb);
        {
            M_LOCK(Mutex);
            container.insert(pair < T1, PTR_getter < T2 > >(id, bb));

        }
        return bbb;
    }

    PTR_getter < T2 > get_NM__(const T1 & id)
    {
        MUTEX_INSPECTOR;



#ifdef DEBUG
        if (!is_empty(id))
#endif
        {
#ifdef WIN32
            map < T1, PTR_getter < T2 > >::const_iterator
#else
            auto 
#endif
            i= container.find(id);
            if (i != container.end())
            {
                return i->second;

            }



        }
        return NULL;
    }

    PTR_getter < T2 > get(const T1 & id)
    {
        MUTEX_INSPECTOR;


        PTR_getter < T2 > b(NULL);

#ifdef DEBUG
        if (!is_empty(id))
#endif
        {
            M_LOCK(Mutex);
            b=get_NM__(id);
            return b;

        }
        return NULL;
    }

    PTR_getter < T2 > FindByID(const T1 & id)
    {
        MUTEX_INSPECTOR;


        PTR_getter < T2 > b(NULL);
#ifdef DEBUG
        if (!is_empty(id))
#endif
        {
            M_LOCK(Mutex);
            b=get_NM__(id);
        }
        if (b)return b;

        if (!b) {
            T2 *bb = new T2;
            LEAKCTL_ADD(bb);
            if (!bb)
                throw cError(table_name + " !bb" + _DMI());
            bb->id=id;
            if (bb->load_from_db(table_name)) {
                LEAKCTL_REMOVE(bb);
                delete bb;
                return NULL;
            }

            PTR_getter < T2 > bbb(bb);
            {
                M_LOCK(Mutex);
                container.insert(pair < T1, PTR_getter < T2 > >(id, bbb));
            }
            return bbb;
        }
        return NULL;
    }
    void purge(const time_t &drop_timeout)
    {
        MUTEX_INSPECTOR;
        time_t t = time(NULL);
        if (t-last_drop_time<=drop_timeout) return;

        last_drop_time=time(NULL);

        vector < T1 > vdel;
        vector<T1> c_copy;
        {
            M_LOCK(Mutex);
#ifdef WIN32
//            map < T1, PTR_getter < T2 > >::const_iterator i;
#else
//            auto i;
#endif
            for (auto i = container.begin(); i != container.end(); i++)
            {
                c_copy.push_back(i->first);
            }
        }

        {
            M_LOCK(Mutex);
#ifdef WIN32
            for (map < T1, PTR_getter < T2 > >::const_iterator i = container.begin(); i != container.end(); i++)
#else
            for (auto i = container.begin(); i != container.end(); i++)
#endif
            {
                PTR_getter < T2 > b = i->second;

                if (b) {
                    bool cond;
                    {
                        PTR_getter_lock_read_no_access < T2 > br(b);
                        LOCKCTL;
                        cond=br->__last_access_time && (t - br->__last_access_time > drop_timeout)
                             && br->get_ref_count() == 3;
                    }

                    if (cond) {
                        vdel.push_back(i->first);
                    }
                }
            }
            for (size_t i=0; i<vdel.size(); i++)
            {
                container.erase(vdel[i]);
            }
        }
    }
    size_t __xx_size()
    {
        M_LOCK(Mutex);
        return container.size();
    }

};

#endif
