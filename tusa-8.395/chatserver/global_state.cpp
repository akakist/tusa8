#include "stdafx.h"
#ifdef _DEBUG
#define New DEBUG_NEW
#endif

#include "_mutex.h"
#include <string>
#include <set>
#include <map>
#include "mutex_inspector.h"
#include "DB_id.h"
#include "CH_id.h"
#include "user_status.h"
#include "user_nick.h"
#include "user_state.h"
#include "global_state.h"
using namespace std;

__global_state global_state;

void __global_state::set_ip(const DB_id& uid,const string& ip)
{
    M_LOCK(Mutex);
    user_states[uid].ip=ip;
}
string __global_state::get_ip(const DB_id& uid)
{
    M_LOCK(Mutex);
    return user_states[uid].ip;
}

void __global_state::set_data(const string & packet)
{
    M_LOCK(Mutex);
    oscar_buffer b(packet.data(),packet.size());
    b>>user_states;
}
string __global_state::get_data()
{
    M_LOCK(Mutex);
    out_oscar_buffer b;
    b<<user_states;
    return b.as_string();
}
void __global_state::set_user_state(const DB_id& uid, const user_state& st)
{
    M_LOCK(Mutex);
    user_states[uid]=st;
    tmpc__.erase(uid);
}
user_state __global_state::get_user_state(const DB_id& uid)
{
    M_LOCK(Mutex);
    return user_states[uid];
}

map<DB_id,user_state> __global_state::get_user_states()
{
    M_LOCK(Mutex);
    return user_states;
}
void __global_state::pack(out_oscar_buffer &b)
{
    M_LOCK(Mutex);
    b<<user_states;
}
void __global_state::unpack(oscar_buffer &b)
{
    M_LOCK(Mutex);
    b>>user_states;
}






void __global_state::set_nick(const DB_id& uid ,const user_nick & n)
{
    M_LOCK(Mutex);
    user_states[uid].nick=n;
}
size_t __global_state::channel_size(const DB_id& uid)
{
    M_LOCK(Mutex);
    user_state &z=user_states[uid];
    return z.channel_us_size();
}
user_status __global_state::get_status(const DB_id& uid)
{
    M_LOCK(Mutex);
    return user_states[uid].status;
}



set<CH_id> __global_state::part_channels(const set<CH_id>&ch,const CT& _ct, const DB_id&uid)
{
    set<CH_id> ret;
    M_LOCK(Mutex);
    tmpc__.erase(uid);
    if (!user_states.count(uid))return ret;
    return user_states[uid].part_channels(ch,_ct);
}

void __global_state::remove(const DB_id& uid)
{
    M_LOCK(Mutex);
    user_states.erase(uid);
    tmpc__.erase(uid);
}

void __global_state::set_status(const DB_id& uid, const user_status& st)
{
    M_LOCK(Mutex);
    user_states[uid].status=st;
}
string __global_state::dump()
{
    M_LOCK(Mutex);
    out_oscar_buffer b;
    b<<user_states;
    return b.as_string();
}
bool __global_state::inchat$www(const DB_id &uid)
{
    M_LOCK(Mutex);
    map<DB_id, map<CT,set<CH_id > >  > ::const_iterator iA=tmpc__.find(uid);
    if (iA!=tmpc__.end())
    {
        map<CT,set<CH_id > > ::const_iterator iB=iA->second.find(T_WWW);
        if (iB!=iA->second.end())
        {
            return iB->second.size();
        }
    }

    set<CH_id> &r=tmpc__[uid][T_WWW];

    user_state &s=user_states[uid];
    foreach_channel_us(i,s)
    {
        if (i->second.count(T_WWW)) r.insert(i->first);
    }
    return r.size();
}
bool __global_state::inchat$irc(const DB_id &uid)
{
    M_LOCK(Mutex);
    map<DB_id, map<CT,set<CH_id > >  > ::const_iterator iA=tmpc__.find(uid);
    if (iA!=tmpc__.end())
    {
        map<CT,set<CH_id > > ::const_iterator iB=iA->second.find(T_IRC);
        if (iB!=iA->second.end())
        {
            return iB->second.size();
        }
    }
    set<CH_id> &r=tmpc__[uid][T_IRC];

    user_state &s=user_states[uid];
    foreach_channel_us(i,s)
    {
        if (i->second.count(T_IRC)) r.insert(i->first);
    }
    return r.size();
}
set<CH_id> __global_state::get_user_channel(const CT& _ct,const DB_id& uid)
{
    M_LOCK(Mutex);
    map<DB_id, map<CT,set<CH_id > >  > ::const_iterator iA=tmpc__.find(uid);
    if (iA!=tmpc__.end())
    {
        map<CT,set<CH_id > > ::const_iterator iB=iA->second.find(_ct);
        if (iB!=iA->second.end())
        {
            return iB->second;
        }
    }

    set<CH_id> &r=tmpc__[uid][_ct];
    user_state &s=user_states[uid];
    foreach_channel_us(i,s)
    {
        if (i->second.count(_ct)) r.insert(i->first);
    }
    return r;
}
int __global_state::get_users$count()
{
    int n=0;
    M_LOCK(Mutex);
    for (map<DB_id,user_state>::iterator i=user_states.begin(); i!=user_states.end(); i++)
    {
        if (i->second.__channel_us.size()) n++;
    }
    return n;
}
size_t  __global_state::size()
{
    M_LOCK(Mutex);
    return user_states.size();
}
map<DB_id,user_state> __global_state::get_container()
{
    M_LOCK(Mutex);
    return user_states;
}
