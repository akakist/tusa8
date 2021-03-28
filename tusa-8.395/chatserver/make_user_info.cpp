#include "stdafx.h"
#ifdef _DEBUG
#define New DEBUG_NEW
#endif

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
#include "irc_server.h"
#include "html_out.h"
#include "chat_globals.h"

#include "contact_vec.h"
#include "update_cmd.h"
#include "user_credits.h"
#include "web_tools.h"
#include "chat_thread_event.h"
#include "app_con.h"
#include "mysql_extens.h"
#include "app_def.h"
#ifdef __WITH_LICENSE
#include "license.h"
#endif


inline void __ui_fio(bool admin, bool hide_name, const string &name,string& out_irc)
{
    string n;

    {
        if (name!="" && (( !hide_name) || admin)) {
            n=name;
            if (admin && hide_name) {
                n<<" "<<"(скрыто)";
            }
        }
    }
}

inline void __ui_nicks(const DB_id& zid, bool setting_show_nicks, const string& nick_name, bool logined, int gender, const string& cnick, const string& irc_host, string& out_irc)
{

    vector<string> nk_irc;
    map<unsigned int,user_nick> vn;
    {
        PGLR(nick_vec, u,nick_vecs.find_by_id_create(CONTAINER(zid)));
        vn=u->u_nicks;
    }
    for (map<unsigned int,user_nick>::const_iterator i=vn.begin(); i!=vn.end(); i++) {
        if (setting_show_nicks)
        {
            if (str_nick(i->second.name)!=str_nick(nick_name))
            {
                nk_irc.push_back(i->second.name);
            }
        }
    }
    if (setting_show_nicks) {
        string ss=nick_name;
        ss<<" "<<"так же как"<<": "<<join(", ",nk_irc);
        out_irc+=irc_gen_reply2(316,ss,irc_host,cnick);
    }

}
inline void __ui_nicks2(vector<string>& ilist,const DB_id& zid, bool setting_show_nicks, const string& nick_name,  int gender, bool admin)
{
    vector<string> nk_www;
    vector<string> nk_irc;
    map<unsigned int,user_nick> vn;
    {
        PGLR(nick_vec, u,nick_vecs.find_by_id_create(CONTAINER(zid)));
        vn=u->u_nicks;
    }

    for (map<unsigned int,user_nick>::const_iterator i=vn.begin(); i!=vn.end(); i++) {
        if (setting_show_nicks )
        {
            if (str_nick(i->second.name)!=str_nick(nick_name))
            {
                nk_irc.push_back(i->second.name);
                {
                    nk_www.push_back(i->second.name);
                }
            }
        }
    }
    if (nk_www.size())
    {
        string n;
        if (gender==0) {
            n="Известно";
        }
        else if (gender==1) {
            n="Известен";
        }
        else if (gender==2) {
            n="Известна";
        }
        ilist.push_back(n);
        ilist.push_back(join(", ",nk_www));
    }

}
inline void __ui_gender(int gender,const string& cnick,const string& nick_name,const string& irc_host,string& out_irc)
{
    string n;
    if (gender==0) {
        n="Неизвестен";
    }
    else if (gender==1) {
        n="Мужской";
    }
    else if (gender==2) {
        n="Женский";
    }
    string ss;
    ss<<nick_name<<" "<<"Пол"<<": "<<n;
    out_irc<<irc_gen_reply2(316,ss,irc_host,cnick);


}



void make_user_info(const DB_id& zid,const DB_id& uid, bool admin,const string & irc_host,const string &cnick, bool logined,string& out_irc)
{
    MUTEX_INSPECTOR;

    string n;
    string nick_name;
    unsigned int nick_id;
    PTR_getter<user_profile> __PZ=user$profiles.FindByID(CONTAINER(zid));
    if (__PZ) {
        PGLR(user_profile, u,__PZ);
        nick_name=u->last_nick;
        nick_id=u->last_nick_id;
    }
    else
    {
        logErr2("err load __P %s %d %s",__FILE__,__LINE__,_DMI().c_str());
        return;
    }

    int gender;


    unsigned int level;
    {
        PGLR(user_profile, u,__PZ);
        level=u->level;
    }
    {
        PGLR(user_profile, u,__PZ);
        gender=u->get_gender();
    }
    st_dbh d;
    vector<string> v=d.dbh->select_1_rowQ((QUERY)"select name,hide_name,setting_show_level,setting_show_credits,credit_summa,info from tbl_users where id=?"<<CONTAINER(zid));
    if (v.size()!=6)throw cError("if(v.size()!=6)");
    string	name=v[0];

    bool	hide_name=atoi(v[1]),
            setting_show_level=atoi(v[2]),
            setting_show_credits=atoi(v[3]);
    double credit_summa =atof(v[4].c_str());
    string info=v[5];
    //int gender=atoi(v)
    __ui_fio(admin,hide_name,name,out_irc);


    __ui_nicks(zid, false, nick_name,logined, gender, cnick,irc_host,out_irc);

    //gender
    __ui_gender(gender,cnick,nick_name,irc_host,
                out_irc);

    ///level
    if (setting_show_level || admin) {
        if (admin && !setting_show_level) {

            string ss;
            ss<<nick_name<<" "<<"Уровень"<<": "<<LEVEL_NAME(level,gender)<<" "<<"(скрыто)";
            out_irc+=irc_gen_reply2(316,ss,irc_host,cnick);

        } else {

            string ss;
            ss<<nick_name<<" "<<"Уровень"<<": "<<LEVEL_NAME(level,gender);
            out_irc+=irc_gen_reply2(316,ss,irc_host,cnick);

        }
    }
    //credits
    if (setting_show_credits || admin ) {
        if (admin && !setting_show_credits) {
            string ss;
            ss<<nick_name<<" "<<"Кредиты"<<": "<<get_f22(credit_summa)<<" "<<"(скрыто)";
            out_irc+=irc_gen_reply2(316,ss,irc_host,cnick);
        } else {
            string ss;
            ss<<nick_name<<" "<<"Кредиты"<<": "<<get_f22(credit_summa);
            out_irc+=irc_gen_reply2(316,ss,irc_host,cnick);
        }
    }

    out_irc+=irc_gen_reply2(316,nick_name+" info available from http://"+irc_host+"/chat_userinfo2.php?id="+itostring(CONTAINER(zid)),irc_host,cnick);


    if (info.size()) {
        info=str_replace("\r","",info);
        info=str_replace("\n"," ",info);
        out_irc+=irc_gen_reply2(316,nick_name+" says: "+info,irc_host,cnick);
    }


}

