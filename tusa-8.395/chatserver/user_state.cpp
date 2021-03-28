#include "stdafx.h"
#ifdef _DEBUG
#define New DEBUG_NEW
#endif

#include <string>
using namespace std;
#include "user.h"
#include "cutils.h"
#include "db_var.h"
#include "user_profile.h"




out_oscar_buffer & operator<<(out_oscar_buffer&b, const user_state& n)
{
    b<<n.status<<n.nick<<n.voice<<n.__channel_us<<n.ip;
    return b;
}
oscar_buffer & operator>>(oscar_buffer&b, user_state& n)
{

    b>>n.status>>n.nick>>n.voice>>n.__channel_us>>n.ip;
    return b;
}


void user_state::channel_add(const CT& _ct,const CH_id& ch)
{
    __channel_us[ch].insert(_ct);
}



void user_state::export_channels(set<CH_id>&s) const
{
    MUTEX_INSPECTOR;

    for (map<CH_id,set<CT> >::const_iterator i=__channel_us.begin(); i!=__channel_us.end(); i++)
    {
        s.insert(i->first);
    }
}


bool user_state::channel_us_count_usr(const CH_id& ch,const CT &p) const
{
    map<CH_id,set<CT> >::const_iterator i=__channel_us.find(ch);
    if (i!=__channel_us.end())
    {
        return i->second.count(p);
    }
    return false;
}
size_t user_state::channel_us_size() const
{
    return __channel_us.size();
}
void user_state::www_channel_replace(const CH_id &chan)
{
    MUTEX_INSPECTOR;
    vector<CH_id> rm;
    for (map<CH_id,set<CT> >::iterator i=__channel_us.begin(); i!=__channel_us.end(); i++)
    {
        if (i->second.count(T_WWW))
            i->second.erase(T_WWW);
        if (i->second.size()==0)
            rm.push_back(i->first);
    }
    for (unsigned i=0; i<rm.size(); i++)
        __channel_us.erase(rm[i]);
    __channel_us[chan].insert(T_WWW);
}

set<CH_id> user_state::part_channels(const set<CH_id>&chans,const CT& _ct)
{
    MUTEX_INSPECTOR;
    set<CH_id> rm;
    for (map<CH_id,set<CT> >::iterator i=__channel_us.begin(); i!=__channel_us.end(); i++)
    {
        if (chans.size()==0 || chans.count(i->first))
        {
            if (i->second.size())
            {
                i->second.erase(_ct);
                if (i->second.size()==0)
                {
                    rm.insert(i->first);
                }
            }
        }
    }
    for (set<CH_id>::const_iterator i=rm.begin(); i!=rm.end(); i++)
    {
        __channel_us.erase(*i);
    }
    return rm;
}
