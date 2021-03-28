
#include "stdafx.h"
#include "_mutex.h"
#include <list>
#include "mutex_inspector.h"
#include <time.h>
#include <map>
#include "mysql_extens.h"
#include "_mutex.h"
#include "user.h"
#include "cutils.h"
#include "str_lib.h"
#include "web_server.h"
#include "message.h"
#include "app_con.h"

#include <algorithm>
#include "web_server_data.h"
#include "file_set.h"
#include "copy_file.h"
#include "admin.h"
#include "channels.h"
#include "server_config.h"
#include "app_def.h"
#include "DB_id.h"

#ifdef __WITH_LICENSE
#include "license.h"
#endif

string _MSG(const string &n);

int do_old_loginban(const DB_id &zuid,const string &descr,time_t t,const DB_id& adm_uid,unsigned int level);
int do_old_ipban(const string &ip_orig,time_t t,const string& za,const DB_id& adm_uid);
void url_admin_old_ipban(const PTR_getter<chat_user> &__UU,app_connection *cc)
{
    MUTEX_INSPECTOR;
    if (!__UU) {
        cc->html_out=cc->redirect();
        return;
    }

    user_pair __U(__UU);
    DB_id cc_user_id=user$id(__U);
    map<string,string> m;
    m["~msg~"]="";
    string cmd=cc->params["cmd"];
    st_dbh d;
    if (cmd=="remove") {
        MUTEX_INSPECTOR;
        vector<string> v=d.dbh->select_1_rowQ((QUERY)"select banned_ip.uid,tbl_users.level from banned_ip,tbl_users where banned_ip.id='?' and tbl_users.id=banned_ip.uid"<<cc->params["id"]);
        if (v.size()!=2) {
            MUTEX_INSPECTOR;
            m["~msg~"]="Запись не найдена!";
        } else {
            MUTEX_INSPECTOR;
            unsigned int ul=user$level(__U)/sub_levels;
            unsigned int bl=atoi(v[1])/sub_levels;
            DB_id id;
            CONTAINER(id)=0;
            CONTAINER(id)=atoi(v[0]);
            if (id==cc_user_id || ul==am_sadmin || bl<=ul) {
                MUTEX_INSPECTOR;
                vector<string> ip=d.dbh->select_1_rowQ((QUERY)"select ip from banned_ip where id=?"<<cc->params["id"]);
                if (ip.size()==1) {
                    MUTEX_INSPECTOR;
                    d.dbh->real_queryQ((QUERY)"delete from banned_ip where id='?'"<<cc->params["id"]);
                    m["~msg~"]=_MSG("ipunbanned");
                    logErr2("%s (%s) unbanned address %s [%s]",
                            get_cnick(__U).c_str(),
                            user$login(__U).c_str(),
                            ip[0].c_str(),
                            cc->peer_ipaddress.c_str());
                }
            } else {
                m["~msg~"]=_MSG("ipnopriv");
            }
        }
    }
    if (cmd=="add") {
        MUTEX_INSPECTOR;
        if (user$level(__U)/sub_levels<am_spy) {
            m["~msg~"]=_MSG("iplowlevel");
        } else {
            MUTEX_INSPECTOR;
            string ip=cc->params["addip"];
            int r=do_old_ipban(ip,atoi(cc->params["how"]),"хз за че",cc_user_id);
            if (r!=0) {
                m["~msg~"]=_MSG("ipalreadybanned");
            } else {
                m["~msg~"]=_MSG("ipbanned");
                logErr2("%s (%s) banned address %s [%s]",get_cnick(__U).c_str(),user$login(__U).c_str(),ip.c_str(),cc->peer_ipaddress.c_str());

            }

        }
    }
    string data;



    vector< vector<string> > v=d.dbh->exec(
                                   //			0								1									2
                                   "select banned_ip.id,banned_ip.ip,UNIX_TIMESTAMP(banned_ip.time_ban),UNIX_TIMESTAMP(banned_ip.time_free),"
                                   //			3					4			5				6
                                   "banned_ip.time_cnt,banned_ip.uid,tbl_users.level,tbl_users.login from banned_ip,tbl_users where tbl_users.id=banned_ip.uid order by banned_ip.ip");
    for (unsigned int i=0; i<v.size(); i++) {
        MUTEX_INSPECTOR;
        if (v[i].size()!=8) {
            throw cError("select() failed. Size different."+_DMI());
        }
        map<string,string>q;

        string id=v[i][0];
        string ip=v[i][1];
        string time_ban=date2rus(atoi(v[i][2]));
        string time_free=date2rus(atoi(v[i][3]));
        string time_cnt=v[i][4];
        DB_id adm_uid;
        CONTAINER(adm_uid)=atoi(v[i][5]);
        string adm_level=v[i][6];
        string adm_login=v[i][7];

        unsigned int ul=user$level(__U)/sub_levels;
        unsigned int bl=atoi(adm_level)/sub_levels;
//        DB_id id;CONTAINER(id)=0;CONTAINER(id)=atoi(v[i][7]);
        //      string ip=v[i][1];

        data+="<tr><td><b>"+ip+"</b></td>";
        data+="<td>"+time_ban+"</td>";		//time ban
        data+="<td>"+time_cnt+"</td>";		//time cnt
        data+="<td>"+time_free+"</td>";		//time free
        data+="<td>"+adm_login+"</td>";		//banned by


        if (cc_user_id==adm_uid || ul==am_sadmin || bl<=ul) {
            data+="<td><input type=button value=\""+_MSG("removeban")+"\" onclick=\"b_remove('"+id+"')\"></td>";
        } else {
            data+="<td></td>";
        }
        data+="</tr>";
    }
    m["~data~"]=data;
    m["~cnt~"]=itostring(v.size());
    cc->html_out=cc->rvfiles(m,"admin/ipban/ipban");
}



void url_admin_old_loginban(const PTR_getter<chat_user> &__UU,app_connection *cc)
{
//	logErr2("url_admin_old_loginban");
    MUTEX_INSPECTOR;
    if (!__UU) {
        cc->html_out=cc->redirect();
        return;
    }
    user_pair __U(__UU);
//    if (user$level(__U)/sub_levels<am_guard){
    //      return;
//    }
    DB_id cc_user_id=user$id(__U);
    map<string,string> m;
    m["~msg~"]="";
    if (cc->params.find("sort")!=cc->params.end()) {
        m["~sort~"]=cc->params["sort"];
    } else {
        m["~sort~"]="login";
    }
    string cmd=cc->params["cmd"];
    st_dbh d;
    if (cmd=="remove" && user$level(__U)/sub_levels>=am_guard) {
        MUTEX_INSPECTOR;
        DB_id uid;
        CONTAINER(uid)=atoi(cc->params["id"]);
        DB_id adm_uid;
        CONTAINER(adm_uid)=0;
        vector<string> v=d.dbh->select_1_columnQ((QUERY)"select adm_uid from banned_login where uid=?"<<CONTAINER(uid));
        if (v.size()==1) CONTAINER(adm_uid)=atoi(v[0]);
        unsigned int adm_level=0;
        string zlogin;
        {
            MUTEX_INSPECTOR;
            PGLR(user_profile,u,user$profiles.FindByID(CONTAINER(adm_uid)));
            adm_level=u->level;
        }
        {
            MUTEX_INSPECTOR;
            PGLR(user_profile,u,user$profiles.FindByID(CONTAINER(uid)));
            zlogin=u->login;
        }

        if (adm_level/sub_levels>=user$level(__U)/sub_levels && adm_uid!=cc_user_id)
        {
            m["~msg~"]=_MSG("loginnopriv");
        }
        else
        {
            MUTEX_INSPECTOR;
            d.dbh->real_queryQ((QUERY)"delete from banned_login where uid=?"<<CONTAINER(uid));
            m["~msg~"]=_MSG("loginunbanned");
            logErr2("%s (%s) unbanned login %s [%s]",get_cnick(__U).c_str(),user$login(__U).c_str(),zlogin.c_str(),cc->peer_ipaddress.c_str());
        }


    }
    if (cmd=="add") {
        MUTEX_INSPECTOR;
        string login;
        DB_id uid;
        CONTAINER(uid)=0;
        bool found=false;
        if (cc->params["addnick"].size()) {
            login=cc->params["addnick"];
            vector<string> z=d.dbh->select_1_rowQ((QUERY)"select uid from nicks where str_nick='?'"<<str_nick(login));
            if (z.size()==0) {
                m["~msg~"]="Ник не найден.";

            } else {
                CONTAINER(uid)=atoi(z[0]);
                found=true;
            }
        }
        if (cc->params["addlogin"].size()) {
            MUTEX_INSPECTOR;
            login=cc->params["addlogin"];
            vector<string> z=d.dbh->select_1_rowQ((QUERY)"select id from tbl_users where login='?'"<<cc->params["addlogin"]);
            if (z.size()==0) {
                m["~msg~"]=_MSG("loginnotfound");
            }
            else
            {
                CONTAINER(uid)=atoi(z[0]);
                found=true;
            }
        }
        if (found) {
            MUTEX_INSPECTOR;
            int r=do_old_loginban(uid,cc->params["descr"],atoi(cc->params["how"]),cc_user_id,user$level(__U)/sub_levels);
            if (r==-1) {
                m["~msg~"]=_MSG("loginnopriv");
            }
            if (r==0) {
                m["~msg~"]=_MSG("loginbanned");
                logErr2("%s (%s) banned login %s [%s]",get_cnick(__U).c_str(),user$login(__U).c_str(),login.c_str(),cc->peer_ipaddress.c_str());
            }
            if (r==1) {
                m["~msg~"]=_MSG("loginalreadybanned");
            }
        }
    }
    string sort=cc->params["sort"];
    map<string,string>mout;

    vector< vector<string> > v=d.dbh->exec("select uid,adm_uid,UNIX_TIMESTAMP(time_ban),descr,time_cnt from banned_login");
    for (unsigned int i=0; i<v.size(); i++) {
        MUTEX_INSPECTOR;
        if (v[i].size()!=5) {
            throw cError("select() failed. Size different."+_DMI());
        }
        DB_id zid;
        CONTAINER(zid)=atoi(v[i][0]);
        DB_id adm_uid;
        CONTAINER(adm_uid)=atoi(v[i][1]);
        time_t time_ban=atoi(v[i][2]);
        string descr=v[i][3];
        string time_cnt=v[i][4];
        string zlogin,adm_login;
        unsigned int zlevel=0,adm_level=0;
        string mkey;
        string znick,adm_nick;
        {
            MUTEX_INSPECTOR;
            PTR_getter<user_profile> __P=user$profiles.FindByID(CONTAINER(zid));
            if (__P)
            {
                PGLR(user_profile,u,__P);
                zlogin=u->login;
                zlevel=u->level;
                znick=u->last_nick;
            }
        }
        {
            MUTEX_INSPECTOR;
            PTR_getter<user_profile> __P=user$profiles.FindByID(CONTAINER(adm_uid));
            if (__P)
            {
                PGLR(user_profile,u,__P);
                adm_login=u->login;
                adm_level=u->level;
                adm_nick=u->last_nick;

            }
        }
        string row;
        row+="<tr><td><b>";		//login
        if (user$level(__U)/sub_levels<am_admin) {
            row+=znick +" ("+zlogin+")";
        } else {
            row+="<a class=nick href=\"#\" onclick=\"return login_inf('"+zlogin+"')\">"+znick +" ("+zlogin+")"+"</a>";
        }
        row+="</b></td>";
        mkey=znick +" "+zlogin;

        row+="<td>"+datetime2rus(time_ban)+"</td>";		//time ban
        if (sort=="date")mkey=itostring(time_ban);

        row+="<td>"+time_cnt+"</td>";		//time cnt
        if (sort=="how")mkey=time_cnt+itostring(i);

        unsigned int ul=user$level(__U)/sub_levels;
        row+="<td>"+adm_nick +" ("+adm_login+")"+"</td>";		//banned by
        if (sort=="who")mkey=adm_nick +" ("+adm_login+")"+itostring(i);

        row+="<td>"+descr+"</td>";
        if (cc_user_id==adm_uid || ul==am_sadmin || adm_level/sub_levels<=ul) {
            row+="<td><input type=button value=\""+_MSG("removeban")+"\" onclick=\"b_remove('"+itostring(CONTAINER(zid))+"')\"></td>";
        } else {
            row+="<td></td>";
        }
        row+="</tr>";
        mout[mkey]=row;
    }
    string data;
    for (map<string,string>::iterator i=mout.begin(); i!=mout.end(); i++)
    {
        MUTEX_INSPECTOR;
        data+=i->second+"\n";
    }
    m["~data~"]=data;
    m["~cnt~"]=itostring(v.size());
    cc->html_out=cc->rvfiles(m,"admin/loginban/loginban");
}



int do_old_ipban(const string &ip_orig,time_t t,const string& za,const DB_id& adm_uid)
{
    MUTEX_INSPECTOR;

    string ip=remove_spaces(ip_orig);
    st_dbh d;
    vector<string> v=d.dbh->select_1_rowQ((QUERY)"select id from banned_ip where ip='?'" << ip );
    if (v.size()>0) return 1;

    time_t t_remove=time(NULL)+t;
    d.dbh->real_queryQ((QUERY)
                       "insert into banned_ip "
                       "(ip,time_ban,time_free,time_cnt,uid,za) values "
                       "('?',NOW(),FROM_UNIXTIME(?),'?',?,'?')"
                       <<ip<<t_remove<<get_name_of_ban_time(t)<<CONTAINER(adm_uid)<<za
                      );
    return 0;
}
int do_old_loginban(const DB_id &zuid,const string &descr,time_t t,const DB_id& adm_uid,unsigned int adm_level)
{
    MUTEX_INSPECTOR;
    st_dbh d;
    vector<string> v=d.dbh->select_1_rowQ((QUERY)"select uid from banned_login where uid='?'"<<CONTAINER(zuid));
    if (v.size()==0) {
        unsigned int l=atoi(d.dbh->select_1Q((QUERY)"select level from tbl_users where id='?'"<<CONTAINER(zuid)))/sub_levels;
        if (adm_level<=l) {
            return -1;
        }
        time_t t_remove=time(NULL)+t;
        d.dbh->real_queryQ((QUERY)
                           "insert into banned_login "
                           "(uid,time_ban,time_free,time_cnt,adm_uid,descr) values "
                           "('?',NOW(),FROM_UNIXTIME(?),'?','?','?')"
                           <<CONTAINER(zuid)<<t_remove<<get_name_of_ban_time(t)<<CONTAINER(adm_uid)<<descr
                          );
        return 0;
    } else {
        return 1;
    }
    return 0;
}
int do_nickban(const string &nick_orig,const DB_id& adm_uid,unsigned int adm_level)
{
    MUTEX_INSPECTOR;

    string nick=remove_spaces(nick_orig);
    unsigned int uid;
    st_dbh d;
    vector<string> ___z=d.dbh->select_1_rowQ((QUERY)"select id,banned,uid from nicks where str_nick like '?'"<<str_nick(nick));

    if (___z.size()==0) {
        MUTEX_INSPECTOR;
        d.dbh->real_queryQ((QUERY)
                           "insert DELAYED into nicks (nick,str_nick,banned,uid,bby) values('?','?',1,0,?)"
                           <<nick<<str_nick(nick)<<CONTAINER(adm_uid)
                          );
        return 2;
    }


    if ( ___z.size()!=3) {
        throw cError("select() failed. Size different in first select."+_DMI());
    }
    unsigned int bnid=atoi(___z[0]);
    DB_id b_uid;
    CONTAINER(b_uid)=atoi(___z[2]);
    bool banned=atoi(___z[1]);

    if (banned) {
        MUTEX_INSPECTOR
        return 1;
    }
    else {
        MUTEX_INSPECTOR
        vector<string> __q=d.dbh->select_1_rowQ((QUERY)"select level from tbl_users where id=?"<<CONTAINER(b_uid));
        if (__q.size()!=1) {
            throw cError("select failed. size different."+_DMI());
        }
        unsigned int bl=atoi(__q[0])/sub_levels;
        if (adm_level>bl) {
            MUTEX_INSPECTOR;
            d.dbh->real_queryQ((QUERY)"update nicks set banned='1',bby=? where id=? and uid=?"
                               <<CONTAINER(adm_uid)<<bnid<<CONTAINER(b_uid)
                              );
            {
                MUTEX_INSPECTOR;
                PTR_getter<nick_vec> NV=nick_vecs.find_by_id_create(CONTAINER(b_uid));
                if (NV)
                {
                    MUTEX_INSPECTOR;
                    PGLW(nick_vec,n,NV);
                    n->u_nicks.erase(bnid);
                }
                {
                    MUTEX_INSPECTOR;
                    vector<PTR_getter<chat_user> > VU=local_users.get$(b_uid);
                    if (VU.size())
                        kick_user(b_uid,_MSG("zanick"));
                }
            }

            return 0;
        } else {
            return -1;
        }
    }
}

int do_nickban(const string &nick_orig,const DB_id& id,unsigned int level);

void url_admin_old_nickban(const PTR_getter<chat_user> &__UU,app_connection *cc)
{
    MUTEX_INSPECTOR;
    LICENSE_CK(39);
    if (!__UU) {
        cc->html_out=cc->redirect();
        return;
    }
    user_pair __U(__UU);
//    if (user$level(__U)/sub_levels<am_guard){
    //      return;
//    }
    DB_id cc_user_id=user$id(__U);
    map<string,string> m;
    m["~msg~"]="";
    if (cc->params.find("sort")!=cc->params.end()) {
        m["~sort~"]=cc->params["sort"];
    } else {
        m["~sort~"]="nick";
    }
    string cmd=cc->params["cmd"];
    st_dbh d;
    if (cmd=="remove" /*&& user$level(__U)/sub_levels>=am_guard*/) {
        vector<string>vid=splitTOK(".",cc->params["id"]);
        if (vid.size()!=2)throw cError("invalid argument format"+_DMI());
        unsigned int uid=atoi(vid[0]);
        unsigned int nid=atoi(vid[1]);
        vector<string> v=d.dbh->select_1_rowQ((QUERY)"select nicks.uid,nicks.bby,tbl_users.level,nicks.nick from nicks,tbl_users where nicks.id='?' and nicks.uid='?' and tbl_users.id=nicks.bby"<<nid<<uid);
        if (v.size()!=4) {
            m["~msg~"]="Запись не найдена!";
        } else {
            DB_id id;
            CONTAINER(id)=0;
            CONTAINER(id)=atoi(v[1]);
            unsigned int bl=atoi(v[2])/sub_levels;
            unsigned int ul=user$level(__U)/sub_levels;
            if (id==cc_user_id || ul==am_sadmin || bl<=ul) {
                if (v[0]=="0") {
                    d.dbh->real_queryQ((QUERY)"delete from nicks where id='?' and uid='?'"<<nid<<uid);
                } else {
                    d.dbh->real_queryQ((QUERY)"update nicks set banned=0 where id='?' and uid=?"
                                       <<nid<<uid);
                }
                m["~msg~"]=_MSG("nickunbanned");
                logErr("%s (%s) unbanned nick %s [%s]",get_cnick(__U).c_str(),user$login(__U).c_str(),v[3].c_str(),cc->peer_ipaddress.c_str());
            } else {
                m["~msg~"]=_MSG("nicknopriv");
            }
        }
    }
    if (cmd=="add") {
        string nick;
        if (remove_spaces(cc->params["addnick"])!="") {
            nick=cc->params["addnick"];
            int r=do_nickban(nick,cc_user_id,user$level(__U)/sub_levels);
            if (r==2) {
                m["~msg~"]=_MSG("nickbannedforsystem");
                logErr("%s (%s) banned nick %s [%s]",get_cnick(__U).c_str(),user$login(__U).c_str(),nick.c_str(),cc->peer_ipaddress.c_str());
            }
            if (r==1) {
                m["~msg~"]=_MSG("nickalreadybanned");
            }
            if (r==0) {
                m["~msg~"]=_MSG("nickbanned");
                logErr("%s (%s) banned nick %s [%s]",get_cnick(__U).c_str(),user$login(__U).c_str(),nick.c_str(),cc->peer_ipaddress.c_str());
            }
            if (r==-1) {
                m["~msg~"]=_MSG("nicknopriv");
            }
        }
    }
    string data;

    string orderby="a.nick";
    if (cc->params["sort"]=="nick") orderby="a.str_nick";
    if (cc->params["sort"]=="who") orderby="5";

    string wh;
    string snick;
    if (cc->params["snick"].size()) {
        snick=cc->params["snick"];
        if (snick.size()>16) snick=snick.substr(0,16);
        string wnick=MESL(str_nick(snick));
        wh="and a.str_nick like '"+wnick+"%' ";
    }
    m["~snick~"]=cc->params["snick"];

    int cnt=atoi(d.dbh->select_1("select count(*) from nicks a where a.banned=1 "+wh));
    int start=atoi(cc->params["l"]);
    int end=start+100;
    if (end>cnt) end=cnt;
    m["~numb~"]=itostring(start);
    string l;
    for (int i=0; i<=cnt; i+=100) {
        if (i==start) {
            l+="["+itostring(i)+"] ";
        } else {
            l+="<a href=\"?l="+itostring(i)+"&sort="+cc->params["sort"];
            if (wh.size()) l+="&snick="+snick;
            l+="\" class=nick>["+itostring(i)+"]</a> ";
        }
    }
    m["~list~"]=l;
    m["~start~"]=itostring(start);
    m["~end~"]=itostring(end);


    vector< vector<string> > v=d.dbh->exec("select a.id,a.nick,b.login,a.bby,b.login,b.level,a.uid from nicks a,tbl_users b where a.banned=1 and b.id=a.bby  "+wh+"order by "+orderby+" limit "+itostring(start)+",100");

    {
        for (unsigned int i=0; i<v.size(); i++) {
            if (v[i].size()!=7) {
                throw cError("select() failed. Size different."+_DMI());
            }
            data+="<tr><td><b>"+v[i][1]+"</b></td>";		//nick
            DB_id id;
            CONTAINER(id)=0;
            CONTAINER(id)=atoi(v[i][3]);
            data+="<td>"+v[i][4]+"</td>";
            unsigned int ul=user$level(__U)/sub_levels;
            unsigned int bl=atoi(v[i][5])/sub_levels;
            if (ul==am_sadmin || id==cc_user_id || bl<=ul) {
                data+="<td><input type=button value=\""+_MSG("removeban")+"\" onclick=\"b_remove('"+v[i][6]+"."+v[i][0]+"')\"></td>";
            } else {
                data+="<td></td>";
            }
            data+="</tr>";
        }
    }
    m["~data~"]=data;
    m["~cnt~"]=itostring(cnt);
    cc->html_out=cc->rvfiles(m,"admin/nickban/nickban");
}
