#include "stdafx.h"
#include "_mutex.h"
#include <list>
#include "mutex_inspector.h"
#include <stdio.h>
#include <map>
#include "user.h"
#include "str_lib.h"
#include "st_stuff.h"
#include "errs.h"
#include "message.h"
#include "chat_def.h"
#include "cutils.h"
#include "web_tools.h"
#include "file_set.h"
#include "web_server_data.h"
#include "tcp_sender.h"
#include "channels.h"
#include "levels.h"
#include "server_config.h"
#include "chat_config.h"


#include "irc_server_config.h"
#include "irc_utils.h"

#include "update_cmd.h"
#include "cm_container.h"
#include "ignore_vec.h"

#include "contact_vec.h"
#include "user_profile.h"

#include "notes.h"
#include "global_state.h"
#include "copy_file.h"
void user_destroy(const PTR_getter<chat_user>&__UU)
{
    MUTEX_INSPECTOR;
    user_pair __U(__UU);
    DB_id session_id;
    CONTAINER(session_id)=0;
    {
        __UC_LOCK;
        session_id=u->db_id;
        u->erased=true;

    }
    {
        __UC_LOCK;
        u->persistent_socket=NULL;
        u->erased=true;
    }

}



void init_internal_data(const user_pair& __U)
{

    c_level clevel=__LEVELS(user$level(__U));
    int inv=0;
    {
        PGLR(user_profile, u,__U.up);
        inv+=u->bought_invisibility;
    }
    inv+=clevel.invisibility;

    //////////////////////////////////////////////////////////////////////////



    int max_msg_len=clevel.max_msg_len;
    //////////////////////////////////////////////////////////////////////////

    {
        PGLW(chat_user,u,__U.cu);
        u->max_msg_len=max_msg_len;
        u->clevel=clevel;
        u->invisibility=inv;
    }


}
int has_inv(const user_pair& __U)
{

    {
        PGLR(chat_user,u,__U.cu);
        return u->invisibility;
    }

    return false;
}


chat_user::chat_user(const CT& ct, const DB_id& uid):base_container("chat_user"),persistent_socket(NULL),
    __user_profile(NULL),_ct(ct),db_id(uid),erased(false),last_change_nick(0),create_time(time(NULL)),invisibility(0),www_offline_status_sent(false),www_inchat(false),
    iframe_last_send_time(time(NULL)),iframe_buffer_size(0)
{



}
string user$login(const DB_id& uid)
{
    PTR_getter<user_profile> __UP=user$profiles.FindByID(CONTAINER(uid));
    if (__UP)
    {
        PGLR(user_profile, u,__UP);
        return u->login;
    }
    return "";
}
string user$login(const user_pair& __U)
{

    {
        PGLR(user_profile, u,__U.up);
        return u->login;
    }
}

int user$gender(const user_pair& __U)
{
    {
        PGLR(user_profile, u,__U.up);
        return u->get_gender();
    }

}

unsigned int user$setting_n_color_pack(const user_pair&__U)
{
    {
        PGLR(user_profile, u,__U.up);
        return u->setting_n_color_pack;
    }

}
string user$irc_ip(const user_pair& __U)
{
    {
        __UC_LOCK;
        return u->irc_ip;
    }
}

bool user__privileges(const user_pair& __U, const string &priv)
{

    MUTEX_INSPECTOR;
    {

        PGLR(user_profile, u,__U.up);
        return u->__privileges.count(priv)>0;
    }

    return false;// privs;
}
set<string> get_user__privileges(const user_pair& __U)
{

    PGLR(user_profile, u,__U.up);
    return u->__privileges;
}

time_t user$get_spent_time(const user_pair& __U)
{
    {
        __UC_LOCK;
        return time(NULL)-u->create_time;
    }
    return 0;

}
DB_id user$id(const user_pair& __U)
{
    {
        PGLR(chat_user,u,__U.cu);
        return u->db_id;
    }

}

unsigned int user$level(const user_pair& __U)
{
    {
        PGLR(user_profile, u,__U.up);
        return u->level;
    }

}
bool is_inv(const DB_id&uid)
{
    user_status status=global_state.get_status(uid);
    if (status.id==st_invisible || status.id==st_offinvisible ) return true;
    return false;
}

void data_push_back_www(const PTR_getter<chat_user>& __U, const string& s)
{

    MUTEX_INSPECTOR;
    if (!__U) return;
    PTR_getter<c_socket> psocket(NULL);
    {
        PGLR(chat_user,u,__U);
        if (u->_ct!=T_WWW) throw cError("data_push_back_www: if(u->_ct!=T_WWW) "+_DMI());
        psocket=u->persistent_socket;

    }
    if (!psocket) {
        return;
    }
    {
        string push_buffer;
        if (!psocket) {
            PGLW(chat_user,u,__U);
            u->iframe_push_buffer+=s;
            return;
        }
        else
        {
            PGLR(chat_user,u,__U);
            push_buffer=u->iframe_push_buffer;

        }
        string ss;
        if (push_buffer.size() )
        {
            ss+=push_buffer;
            PGLW(chat_user,u,__U);
            u->iframe_push_buffer="";
        }
        ss += s;
        {
            PGLW(chat_user,u,__U);
            if (u->iframe_buffer_size > chat_config.iframe_buffer_max_size.get()) {
                u->iframe_buffer_size = 0;
                ss += RVfiles("scripts/reload_on_buffer_too_much");
            }
            u->iframe_buffer_size+=s.size();
            u->iframe_last_send_time=time(NULL);
        }
        write_sender_data(psocket,ss);

    }
}
void data_push_back_www(const user_pair& __U, const string& s)
{

    data_push_back_www(__U.cu,s);
}

void data_push_back_irc(const PTR_getter<chat_user>& __U, const string& s)
{
    MUTEX_INSPECTOR;
    if (!__U) return;
    PTR_getter<c_socket> psocket(NULL);
    {
        PGLR(chat_user,u,__U);
        if (u->_ct!=T_IRC) throw cError("data_push_back_irc: if(u->_ct!=T_IRC) "+_DMI());
        psocket=u->persistent_socket;
    }
    if (!psocket) {
        return;
    }
    {
        write_sender_data(psocket,s);

    }
    return;
}
void data_push_back_irc(const user_pair& __U, const string& s)
{
    data_push_back_irc(__U.cu,s);
}

bool can_add_nick(const user_pair &__U)
{
    if (__LEVELS(user$level(__U))._max_nicks_count>user$nicks_size(user$id(__U))) return true;
    return false;
}

user_pair::user_pair(const PTR_getter<chat_user> &CU): cu(CU),up(NULL)
{
    {
        if (!CU) throw cError("if(!CU)"+_DMI());
        PGLR(chat_user, c,CU);
        if (!c->__user_profile) throw cError("!c->__user_profile"+_DMI());
        up=c->__user_profile;
    }

}


chat_user::~chat_user()
{
}
vector<pair<DB_id,CT> > chat_users_container::keys$()
{
    vector<pair<DB_id,CT> > keys;
    {
        M_LOCK(Mutex);
        for (map<DB_id,map<CT,PTR_getter<chat_user> > > ::const_iterator i=container.begin(); i!=container.end(); i++)
        {
            for (map<CT,PTR_getter<chat_user> > ::const_iterator j=i->second.begin(); j!=i->second.end(); j++)
            {
                keys.push_back(make_pair(i->first,j->first));
            }

        }

    }
    return keys;
}
void chat_users_container::__clean$()
{
    MUTEX_INSPECTOR;

    vector<pair<DB_id,CT> > keys;
    {
        M_LOCK(Mutex);
        for (map<DB_id,map<CT,PTR_getter<chat_user> > > ::const_iterator i=container.begin(); i!=container.end(); i++)
        {
            for (map<CT,PTR_getter<chat_user> > ::const_iterator j=i->second.begin(); j!=i->second.end(); j++)
            {
                keys.push_back(make_pair(i->first,j->first));
            }

        }
    }

    for (unsigned i=0; i<keys.size(); i++)
    {
        bool need_clean=false;
        PTR_getter<chat_user> __Z=get$(keys[i]);
        DB_id db_id;
        CONTAINER(db_id)=0;
        if (__Z)
        {
            PGLR(chat_user, z,__Z);
            db_id=z->db_id;
            if (z->erased)
                need_clean=true;
        }
        if (need_clean)
        {
            M_LOCK(Mutex);
            map<DB_id,map<CT,PTR_getter<chat_user> > >::iterator it=container.find(keys[i].first);
            if (it!=container.end())
            {
                it->second.erase(keys[i].second);
                if (it->second.size()==0)
                {
                    container.erase(keys[i].first);
                }
            }
        }

    }
}







size_t chat_users_container::size$()
{

    M_LOCK(Mutex);
    return container.size();
}


vector<PTR_getter<chat_user> > chat_users_container::values$()
{

    M_LOCK(Mutex);
    vector<PTR_getter<chat_user> > r;
    for (map<DB_id,map<CT,PTR_getter<chat_user> > >::const_iterator i=container.begin(); i!=container.end(); i++)
    {
        for (map<CT,PTR_getter<chat_user> >::const_iterator j=i->second.begin(); j!=i->second.end(); j++)
        {
            r.push_back(j->second);
        }
    }
    return r;
}

void chat_users_container::insert$(const PTR_getter<chat_user> &__UU)
{



    DB_id uid;
    CONTAINER(uid)=0;
    CONTAINER(uid)=0;
    string cookie;
    CT ct;
    {
        PGLR(chat_user, u,__UU);
        uid=u->db_id;
        ct=u->_ct;
    }
    {
        M_LOCK(Mutex);
        container[uid].insert(make_pair(ct,__UU));
    }
    string login;
    string cnick;
    vector<string> nicks;
    unsigned int level=0;

    {
        PGLR(user_profile,u, user$profiles.FindByID(CONTAINER(uid)));
        login=u->login;
        cnick=u->last_nick;
        level=u->level;
    }

    {
        PGLR(nick_vec,u,nick_vecs.find_by_id_create(CONTAINER(uid)));
        for (map<unsigned int,user_nick> ::const_iterator i=u->u_nicks.begin(); i!=u->u_nicks.end(); i++)
        {
            nicks.push_back(i->second.name);
        }
    }


}
int chat_users_container::count$(const DB_id& id, const CT& ct)
{

    M_LOCK(Mutex);
    map<DB_id,map<CT,PTR_getter<chat_user> > >::const_iterator i=container.find(id);
    if (i!=container.end())
        return i->second.count(ct);
    return 0;
}
int chat_users_container::count$(const PTR_getter<chat_user> &__U)
{
    pair<DB_id,CT> s;
    if (!__U) return 0;
    {
        PGLR(chat_user, u,__U);
        s=make_pair(u->db_id,u->_ct);
    }
    return count$(s.first,s.second);
}

PTR_getter<chat_user>  chat_users_container::get$(const DB_id& id,const CT& ct)
{

    M_LOCK(Mutex);
    PTR_getter<chat_user>  __R(NULL);
    map<DB_id,map<CT,PTR_getter<chat_user> > >::const_iterator i=container.find(id);
    if (i!=container.end())
    {
        map<CT,PTR_getter<chat_user> >::const_iterator j=i->second.find(ct);
        if (j!=i->second.end())
        {
            __R=j->second;
        }
    }


    return __R;
}
PTR_getter<chat_user>  chat_users_container::get$(const pair<DB_id,CT>& z)
{
    return get$(z.first,z.second);

}
vector<PTR_getter<chat_user> >  chat_users_container::get$(const DB_id& id)
{

    vector<PTR_getter<chat_user> >  ret;
    M_LOCK(Mutex);
    map<DB_id,map<CT,PTR_getter<chat_user> > >::const_iterator i=container.find(id);
    if (i!=container.end())
    {
        for (map<CT,PTR_getter<chat_user> >::const_iterator j=i->second.begin(); j!=i->second.end(); j++)
            ret.push_back(j->second);
        return ret;
    }

    return ret;
}

void chat_users_container::erase$(const DB_id& id,const CT& ct)
{
    MUTEX_INSPECTOR;
    PTR_getter<chat_user> __U(get$(id,ct));
    if (__U)
    {

        DB_id dbid;
        CONTAINER(dbid)=0;
        string cookie;
        {
            PGLR(chat_user,u,__U);
            dbid=u->db_id;

        }
        {
            M_LOCK(Mutex);
            map<DB_id,map<CT,PTR_getter<chat_user> > >::iterator i=container.find(id);
            if (i!=container.end())
            {
                i->second.erase(ct);
                if (i->second.size()==0)
                    container.erase(id);
            }


        }

    }

}





unsigned int user$nicks_size(const DB_id& uid)
{

    PGLR(nick_vec, u,nick_vecs.find_by_id_create(CONTAINER(uid)));
    return u->u_nicks.size();
}


user_status get_user_status_by_id(const user_pair&__U, int id)
{
    MUTEX_INSPECTOR;

    user_status s;

    {
        PGLW(user_profile, u,__U.up);
        map<unsigned int,user_status>  &st=u->status;
        if (st.count(id))
            return st.find(id)->second;
        for (map<unsigned int,user_status>::iterator i=st.begin(); i!=st.end(); i++)
            throw cError("Error: cannot find status by id "+itostring(id)+" "+_DMI());
    }
    return s;
}
user_status get_user_status_by_id(const DB_id&  uid , int status_id)
{
    MUTEX_INSPECTOR;
    user_status s;
    {
        PTR_getter<user_profile> __UP=user$profiles.FindByID(CONTAINER(uid));
        if (__UP)
        {
            PGLW(user_profile, u,__UP);
            map<unsigned int,user_status>  &st=u->status;
            if (st.count(status_id))
                return st.find(status_id)->second;
            throw cError("Error: cannot find status by id "+itostring(status_id)+" "+_DMI());

        }
    }
    return s;
}


bool is_inv(const user_state & s)
{
    if (s.status.id==st_invisible|| s.status.id==st_offinvisible) return true;
    return false;
}

DB_id user$id(const PTR_getter<chat_user> &__U)
{

    {
        PGLR(chat_user,u,__U);
        return u->db_id;
    }
    DB_id d;
    CONTAINER(d)=0;
    return d;
}
