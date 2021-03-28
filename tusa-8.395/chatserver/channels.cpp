#include "stdafx.h"
#ifdef _DEBUG
#define New DEBUG_NEW
#endif

#include <map>
#include "_mutex.h"
#include <list>
#include <time.h>
#include <errno.h>
#include <map>
#include "PTR.h"
#include "mutex_inspector.h"

#include "mysql_extens.h"


#include "user.h"
#include "str_lib.h"
#include "message.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include "errs.h"
#include "M_MAP.h"
#include "channels.h"

#include "chat_config.h"
#include "update_cmd.h"
#include "admin.h"
#include "time_limit.h"
__chat_channels chat_channels;
static m_var<bool> channels_loaded(false);




bool add_channel(const string& iname)
{
    CKTIME;
    st_dbh d;
    d.dbh->real_queryQ((QUERY)"insert into channels (irc_name,censored,move,no_stat,ext,read_level,write_level,name,name2) values "
                       "('?','0','0','1','0',0,0,'?','?')"	<< iname<<iname<<iname
                      );
    return true;

}
bool remove_channel(const CH_id& cid)
{
    CKTIME;
    st_dbh d;
    d.dbh->real_queryQ((QUERY)"delete from channels where id='?'"<<CONTAINER(cid));
    return true;

}
void update_channel(const chat_channel_data& cd,const CH_id& cid)
{
    st_update stu;
    CKTIME;
    st_dbh d;
    d.dbh->real_queryQ((QUERY)"update channels set "
                       "censored='?',ext='?',move='?',move_to_channel='?',name='?',name2='?',read_level='?',write_level='?', smiles_allowed='?' "
                       "where id='?'"
                       <<cd.censored
                       <<cd.extended
                       <<cd.move
                       <<CONTAINER(cd.move_to_channel)
                       <<cd.name
                       <<cd.name2
                       <<cd.read_level
                       <<cd.write_level
                       << cd.smiles_allowed
                       <<CONTAINER(cid))
    ;
    __send_update_d3(channel$update$3,cd,cid);

}

map<CH_id,chat_channel_data> load_channels()
{
    CKTIME;
    map<CH_id,chat_channel_data> ret;


    st_dbh d;

    vector<vector<string> > bv;
    bv=d.dbh->exec("select uid,channel,time_ban_end,descr,time_cnt_str from bans");
    map<CH_id,map<DB_id,chat_channel_data::klap > > klaped_logins;
    for (unsigned i=0; i<bv.size(); i++)
    {
        if (bv[i].size()==5)
        {

            DB_id uid;
            CONTAINER(uid)=atoi(bv[i][0]);
            CH_id channel;
            CONTAINER(channel)=atoi(bv[i][1]);
            time_t time_ban_end=atoi(bv[i][2]);
            string descr=bv[i][3];
            string time_cnt_str=bv[i][4];

            chat_channel_data::klap k;
            k.time_ban_end=time_ban_end;
            k.time_cnt_str=time_cnt_str;
            k.za=descr;
            klaped_logins[channel][uid]=k;
        }
    }

    vector< vector<string> > cv;
    cv=d.dbh->exec((string)"select censored,save,move_to_channel,move,no_stat,ext,read_level,write_level,irc_name,name,name2,owner,smiles_allowed,moderated,id from channels order by id");

    for (unsigned int i=0; i<cv.size(); i++)
    {
        if (cv[i].size()!=15) throw cError("select failed in load_channels"+_DMI());
        chat_channel_data cd;
        cd.censored=atoi(cv[i][0]);
        cd.save_messages=atoi(cv[i][1]);
        CONTAINER(cd.move_to_channel)=atoi(cv[i][2]);
        cd.move=atoi(cv[i][3]);

        cd.extended=atoi(cv[i][5]);
        cd.read_level=atoi(cv[i][6]);
        cd.write_level=atoi(cv[i][7]);


        cd.irc_name=cv[i][8];
        cd.name=cv[i][9];
        cd.name2=cv[i][10];
        cd.smiles_allowed=atoi(cv[i][12]);
        cd.moderated=atoi(cv[i][13]);
        CONTAINER(cd.chid)=atoi(cv[i][14]);

        vector<string> v=d.dbh->select_1_columnQ((QUERY)"select uid from moderators where channel='?'"<<CONTAINER(cd.chid));
        for (unsigned j=0; j<v.size(); j++)
        {
            DB_id uid;
            CONTAINER(uid)=0;
            CONTAINER(uid)=atoi(v[j]);
            cd.moderators.insert(uid);
        }
        cd.klaped_logins_stuff=klaped_logins[cd.chid];
        ret[cd.chid]=cd;
    }
    return ret;

}


bool load_channels$$$()
{
    CKTIME;
    MUTEX_INSPECTOR;
    if (channels_loaded) return true;
    channels_loaded=true;
    logErr2("Loading channels...");

    map<CH_id,chat_channel_data>v=load_channels();
    for (map<CH_id,chat_channel_data>::iterator i=v.begin(); i!=v.end(); i++)
    {
        chat_channel *m=new chat_channel();
        LEAKCTL_ADD(m);
        m->cd=i->second;
        chat_channels.add(m->cd.chid,m);
    }
    return true;
}

void __chat_channels::add(const CH_id&chan,const PTR_getter<chat_channel > &p)
{

    {
        string ircname;
        {
            PGLR(chat_channel, m,p);
            ircname=m->cd.irc_name;

        }
        M_LOCK(Mutex);
        {
            _container.insert(make_pair(chan,p));
            container_s.insert(make_pair(ircname,p));
            ___keys.push_back(chan);
            ___keys_s.push_back(ircname);


        }
    }
}
PTR_getter<chat_channel> __chat_channels::operator[](const CH_id& n)
{

    PTR_getter<chat_channel > zz(NULL);
    M_LOCK(Mutex);
    map<CH_id,PTR_getter<chat_channel > >::const_iterator i=_container.find(n);

    if (i!=_container.end()) return i->second;

    if (!zz)
        throw cError("operator[]: !zz "+itostring(CONTAINER(n))+ _DMI());
    return zz;
}
CH_id __chat_channels::get_id(const string& irc_name)
{
    PTR_getter<chat_channel > zz(NULL);
    {
        M_LOCK(Mutex);
        map<string,PTR_getter<chat_channel > >::const_iterator i=container_s.find(irc_name);

        if (i==container_s.end())
            throw cError("get_id: i==_container.end()"+irc_name+ _DMI());
        zz=i->second;
    }
    {
        PGLR(chat_channel,c,zz);
        return c->cd.chid;
    }
    throw cError("--- CH not found"+_DMI());
    CH_id id;
    CONTAINER(id)=0;
    return id;
}
PTR_getter<chat_channel> __chat_channels::___get_s(const string& n)
{

    M_LOCK(Mutex);
    map<string,PTR_getter<chat_channel > >::const_iterator i=container_s.find(n);

    if (i==container_s.end())
        return NULL;
    return i->second;
}

PTR_getter<chat_channel> __chat_channels::___get(const CH_id& n)
{

    PTR_getter<chat_channel > zz(NULL);
    M_LOCK(Mutex);
    map<CH_id,PTR_getter<chat_channel > >::const_iterator i=_container.find(n);

    if (i!=_container.end()) return i->second;
    return NULL;
}


void __chat_channels::clear() {
    _container.clear();
    container_s.clear();
    this->___keys.clear();
    this->___keys_s.clear();
}
vector<CH_id> __chat_channels::_keys()
{
    M_LOCK(Mutex);
    return ___keys;

}
size_t __chat_channels::count(const CH_id& id)
{
    M_LOCK(Mutex);
    return _container.count(id);
}

out_oscar_buffer & operator<<(out_oscar_buffer&b, const chat_channel_data::klap &z)
{
    b<<z.time_ban_end<<z.time_cnt_str<<z.za;
    return b;
}
oscar_buffer & operator>>(oscar_buffer&b, chat_channel_data::klap &z)
{
    b>>z.time_ban_end>>z.time_cnt_str>>z.za;
    return b;
}

out_oscar_buffer & operator<<(out_oscar_buffer&b, const chat_channel_data &n)
{

    b<<n.name;
    b<<n.name2;
    b<<CONTAINER(n.chid);
    b<<n.move_to_channel;
    b<<n.move;
    b<<n.save_messages;
    b<<n.censored;
    b<<n.topic;
    b<<n.set_by;
    b<<n.topic_time;
    b<<n.read_level;
    b<<n.write_level;
    b<<n.extended;
    b<<n.moderated;


    b<<n.moderators;
    b<<n.klaped_logins_stuff;
    b<<n.smiles_allowed;

    return b;

}
oscar_buffer & operator>>(oscar_buffer&b, chat_channel_data &n)
{

    b>>n.name;
    b>>n.name2;
    b>>n.chid;
    b>>n.move_to_channel;
    b>>n.move;
    b>>n.save_messages;
    b>>n.censored;
    b>>n.topic;
    b>>n.set_by;
    b>>n.topic_time;
    b>>n.read_level;
    b>>n.write_level;
    b>>n.extended;
    b>>n.moderated;


    b>>n.moderators;
    b>>n.klaped_logins_stuff;
    b>>n.smiles_allowed;

    return b;

}





void remove_moderator(const DB_id& uid)
{

    st_update stu;
    __send_update_d2(moderator$remove$2,uid);
    st_dbh d;
    d.dbh->real_queryQ((QUERY)"delete from moderators where uid='?'"<<CONTAINER(uid));
}


void add_moderator(const DB_id& uid,const CH_id& channel)
{
    st_update stu;
    CKTIME;
    __send_update_d3(moderator$add$3,uid,channel);
    st_dbh d;
    d.dbh->real_queryQ((QUERY)"insert into moderators (uid,channel) values ('?','?')"<<CONTAINER(uid)<<CONTAINER(channel));
}


map<DB_id,vector<CH_id> > get_moderators()
{

    map<DB_id,vector<CH_id> > ret;
    st_dbh d;
    vector<vector<string> > v=d.dbh->execQ((QUERY)"select uid,channel from moderators");
    for (unsigned i=0; i<v.size(); i++)
    {
        if (v[i].size()!=2) continue;
        DB_id uid;
        CONTAINER(uid)=0;
        CONTAINER(uid)=atoi(v[i][0]);
        CH_id ch;
        CONTAINER(ch)=atoi(v[i][1]);
        ret[uid].push_back(ch);
    }
    return ret;
}
void load_bans()
{
    CKTIME;

    vector<klap_item> ret;
    st_dbh d;
    vector<vector<string> > v=d.dbh->execQ((QUERY)
                                           "select id,uid,descr,UNIX_TIMESTAMP(time_ban_start),UNIX_TIMESTAMP(time_ban_end),time_cnt_str,channel,adm_uid,adm_name,user_name,alevel,ulevel from bans where time_ban_end>NOW()");
    for (unsigned i=0; i<v.size(); i++)
    {
        if (v[i].size()!=12)
        {
            logErr2("if(v[i].size()!=14)");
            continue;
        }

        DB_id uid,adm_uid;
        CONTAINER(uid)=0;
        CONTAINER(adm_uid)=0;
        int id=				atoi(v[i][0]);
        CONTAINER(uid)=			atoi(v[i][1]);
        string descr=				v[i][2];
        time_t timeban_start=		atoi(v[i][3]);
        time_t time_ban_end=		atoi(v[i][4]);
        string time_cnt_str=			v[i][5];
        CH_id channel;
        CONTAINER(channel)=0;
        CONTAINER(channel)=				atoi(v[i][6]);
        CONTAINER(adm_uid)=		 atoi(v[i][7]);
        string adm_name		=		v[i][8];;
        string user_name	=		v[i][9];;
        int alevel			=	atoi(v[i][10]);
        int ulevel			=	atoi(v[i][11]);;


        klap_item bi;
        CONTAINER(bi.id)=id;
        bi.uid=uid;
        bi.descr=descr;
        bi.time_ban_start=timeban_start;
        bi.time_ban_end=time_ban_end;
        bi.time_cnt_str=time_cnt_str;
        bi.channel=channel;
        bi.adm_uid=adm_uid;
        bi.adm_name=adm_name;
        bi.user_name=user_name;
        bi.alevel=alevel;
        bi.ulevel=ulevel;
        ret.push_back(bi);
    }
    _klap_stuff.clear();
    for (unsigned i=0; i<ret.size(); i++)
        _klap_stuff.add__klap(ret[i]);
}


string make_select(const string &n,map<CH_id,string> &p,const CH_id &def,const string &add)
{
    string s;
    s="<SELECT name=\""+n+"\" "+add+">\r\n";
    for (map<CH_id,string>::const_iterator i=p.begin(); i!=p.end(); i++) {
        s+="<OPTION value=";
        s+="\""+itostring(CONTAINER((*i).first))+"\"";
        if ((*i).first==def) {
            s+=" SELECTED";
        }
        s+=">";
        s+=(*i).second;
        s+="\r\n";
    }
    s+="</SELECT>\r\n";
    return s;
}

string make_select(const string &n,vector<pair<CH_id,string> > &p,const CH_id &def,const string &add)
{
    string s;
    s="<SELECT name=\""+n+"\" "+add+">\r\n";
    for (vector<pair<CH_id,string> >::const_iterator  i=p.begin(); i!=p.end(); i++) {
        s+="<OPTION value=";
        s+="\""+itostring(CONTAINER((*i).first))+"\"";
        if ((*i).first==def) {
            s+=" SELECTED";
        }
        s+=">";
        s+=(*i).second;
        s+="\r\n";
    }
    s+="</SELECT>\r\n";
    return s;
}
