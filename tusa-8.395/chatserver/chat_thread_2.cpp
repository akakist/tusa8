#include "stdafx.h"
#ifdef _DEBUG
#define New DEBUG_NEW
#endif

#include <string>
#include <set>
using namespace std;
#include "mutex_inspector.h"
#include "oscar_buffer.h"
#include "PTR.h"
#include "DB_id.h"
#include "CH_id.h"
#include "S_id.h"

#include "M_DEQUE.h"
#include "user_status.h"
#include "user_nick.h"
#include "user_state.h"
#include "levels.h"
#include "chat_stuff.h"
#include "message.h"
#include "channels.h"
#include "chat_def.h"
#include "cutils.h"
#include "web_tools.h"
#include "cm_container.h"
#include "irc_server_config.h"
#include "chat_globals.h"
#include "chat_thread_event.h"
#include "chat_config.h"

string get_add_string(const CH_id& channel,const chat_stuff_user &uu,const chat_stuff_user &zz);

void chat_thread_stuff::make_url_users_full_to_file()
{
    MUTEX_INSPECTOR;

    int users_cnt=0;
    map<CH_id, vector<string> > v;
    map<CH_id, string> name2;
    int csize=__channels$.size();
    for (map<CH_id,set<DB_id> >::iterator i=__users_in_channels$.begin(); i!=__users_in_channels$.end(); i++)
    {
        MUTEX_INSPECTOR;
        vector<string> __v;
        v[i->first]=__v;
        for (set<DB_id>::iterator j=i->second.begin(); j!=i->second.end(); j++)
        {
            MUTEX_INSPECTOR;
            map<DB_id,PTR_getter_nm<chat_stuff_user_p > >::iterator U=__users$.find(*j);
            if (U==__users$.end()) continue;
            chat_stuff_user &__z=U->second.___ptr->ud;

            if (__z.cstate.status.id!=st_invisible && __z.cstate.status.id!=st_offinvisible &&__z.cstate.status.id!=user_state::UNDEF)
            {
                MUTEX_INSPECTOR;
                v[i->first].push_back(__z.cstate.nick.name);
                users_cnt++;
            }
        }
        {
            MUTEX_INSPECTOR;
            const chat_channel_data &c=channel_cref(i->first);
            name2[i->first]=c.name2;
        }
    }

    map<string,string> m;
    string u;
    string s;
    map<CH_id,pair<string/*name*/,vector<string>/*userlist*/  > > ch_out;
    {
        MUTEX_INSPECTOR;
        for (map<CH_id, vector<string> > ::iterator i=v.begin(); i!=v.end(); i++) {
            MUTEX_INSPECTOR;
            vector<string> &vs=i->second;
            str_sort(vs);
            for (unsigned int j=0; j<vs.size(); j++) {
                ch_out[i->first].second.push_back(vs[j]);
            }
            ch_out[i->first].first=name2[i->first];
        }
    }
    string uinfo;
    vector<CH_id> chs=chat_channels._keys();
    for (unsigned i=0; i<chs.size(); i++)
    {

        string fn="/tmp/users_online_"+itostring(CONTAINER(chs[i]));
        string body=join(" ",ch_out[chs[i]].second);
        FILE *f=fopen(fn.c_str(),"wb");
        if (!f)
        {
            logErr2("failed fopen %s",fn.c_str());
            continue;
        }
        fwrite(body.data(),1,body.size(),f);
        fclose(f);
        uinfo+=itostring(CONTAINER(chs[i]))+" "+this->__channels$[chs[i]].name2+" "+itostring(ch_out[chs[i]].second.size())+"\n";
    }
    string fn="/tmp/users_online";
    FILE *f=fopen(fn.c_str(),"wb");
    if (!f)
    {
        logErr2("failed fopen %s",fn.c_str());
        return;
    }
    fwrite(uinfo.data(),1,uinfo.size(),f);
    fclose(f);

}









string chat_thread_stuff::LK_get_users$count_for_web()
{
    vector<string>m;
    vector<CH_id>v=chat_channels._keys();
    for (int i=0; i<v.size(); i++)
    {
        if (__channels$.count(v[i]));
        {
            m.push_back(itostring(__users_in_channels$[v[i]].size()));
        }
    }
    string s="<script>parent.rc([" +join(",",m)+"]);</script>\n";
    return s;
}

string get_irc_name(const CH_id& channel)
{

    string irc_name;
    {
        __CH_LOCK(chat_channels[channel]);
        irc_name="#"+CH->cd.irc_name;
    }
    return irc_name;
}
void __set_user_state(int cmd,const DB_id&uid, const out_oscar_buffer &s)
{
    MUTEX_INSPECTOR;
    chat_event_SET_user_state *e=new chat_event_SET_user_state;
    LEAKCTL_ADD(e);
    e->msg=s.as_string();
    e->cmd=cmd;
    e->uid=uid;
    if (CONTAINER(uid)==0)
    {
        logErr2("set_user_state uid==0 %s",_DMI().c_str());
    }
    else
        send_chat_event_push_back(e);
}











void chat_event_CHROOM::pack(out_oscar_buffer &b)
{
    b<<uid<<oldchan<<newchan;
}
void chat_event_CHROOM::unpack(oscar_buffer &b)
{
    b>>uid>>oldchan>>newchan;
}

void chat_event_FLUSH_USER_DATA::pack(out_oscar_buffer &b)
{
    b<<uid;
}
void chat_event_FLUSH_USER_DATA::unpack(oscar_buffer &b)
{
    b>>uid;
}

void chat_event_CHMODE::pack(out_oscar_buffer &b)
{
    b<<nick;
    b<<channel;
    b<<uid;
    b<<mode;
}
void chat_event_CHMODE::unpack(oscar_buffer &b)
{
    b>>nick;
    b>>channel;
    b>>uid;
    b>>mode;
}

void chat_event_SEND_MESSAGE::pack(out_oscar_buffer &b)
{
    b<<from_uid;
    b<<msg;
    b<<tonick;
    b<<priv;
    b<<sys;
    b<<makeit;
    b<<channel;

}
void chat_event_SEND_MESSAGE::unpack(oscar_buffer &b)
{
    b>>from_uid;
    b>>msg;
    b>>tonick;
    b>>priv;
    b>>sys;
    b>>makeit;
    b>>channel;

}


void chat_event_INVITE::pack(out_oscar_buffer &b)
{
    b<<uid;
    b<<zid;
    b<<msg;
}
void chat_event_INVITE::unpack(oscar_buffer &b)
{
    b>>uid;
    b>>zid;
    b>>msg;

}

void chat_event_CHVOICE::pack(out_oscar_buffer &b)
{
    b<<zid;
    b<<channel;
    b<<uid;
    b<<voice;

}
void chat_event_CHVOICE::unpack(oscar_buffer &b)
{
    b>>zid;
    b>>channel;
    b>>uid;
    b>>voice;

}
void chat_event_SET_user_state::pack(out_oscar_buffer &b)
{
    b<<cmd<<uid<<ct;
    b.Pack(msg.data(),msg.size());

}
void chat_event_SET_user_state::unpack(oscar_buffer &b)
{
    b>>cmd>>uid>>ct;
    b.Unpack(msg,b.remains());

}


void chat_event_CHTOPIC::pack(out_oscar_buffer &b)
{
    b<<uid;
    b<<channel;
    b<<top;

}
void chat_event_CHTOPIC::unpack(oscar_buffer &b)
{
    b>>uid;
    b>>channel;
    b>>top;

}


out_oscar_buffer & operator<<(out_oscar_buffer&b, const CT &n)
{
    int nn=n;
    b<<nn;
    return b;
}
oscar_buffer & operator>>(oscar_buffer&b, CT &n)
{
    int nn;
    b>>nn;
    n=(CT )nn;
    return b;
}
bool chat_thread_stuff::check_klaped(chat_channel_data& __c,const DB_id & uid, string& timecnt,string&za,time_t& end)
{

    time_t ct=time(NULL);
    map<DB_id,chat_channel_data::klap>::const_iterator ibl=__c.klaped_logins_stuff.find(uid);
    if (ibl!=__c.klaped_logins_stuff.end())
    {
        const chat_channel_data::klap &k=ibl->second;
        if (k.time_ban_end>ct)
        {
            timecnt=k.time_cnt_str;
            za=k.za;
            end=k.time_ban_end;
            return true;
        }
        else __c.klaped_logins_stuff.erase(uid);
    }
    return false;
}

