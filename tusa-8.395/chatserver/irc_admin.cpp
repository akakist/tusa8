

#include "stdafx.h"
#ifdef _DEBUG
#define New DEBUG_NEW
#endif

#include "_mutex.h"
#include <list>
#include "mutex_inspector.h"
#include <stdio.h>
#include <map>
#include <set>
#include "user.h"
#include "str_lib.h"
#include "st_stuff.h"
#include "errs.h"
#include "message.h"
#include "chat_def.h"
#include "cutils.h"
#include "irc_server.h"
#include "file_set.h"
#include "web_server_data.h"

#include "channels.h"
#include "irc_server_config.h"
#include "irc_utils.h"
#include "update_cmd.h"
#include "admin.h"
#include "chat_globals.h"
#include "app_def.h"
#include "user.h"
#include "user_profile.h"
#include <sys/timeb.h>

string get_name_of_ban_time(time_t _tt);

void irc_cmd_MODE(const PTR_getter<chat_user> &__UU,irc_command &cmd);
void irc_cmd_KICK(const PTR_getter<chat_user> &__UU,irc_command &cmd);
void irc_cmd_EYE(const PTR_getter<chat_user> &__UU,irc_command &cmd);
void irc_cmd_EYEN(const PTR_getter<chat_user> &__UU,irc_command &cmd);
int do_nickban(const string &nick_orig,const DB_id& id,unsigned int level);

void irc_cmd_MODE(const PTR_getter<chat_user> &__UU,irc_command &cmd)
{
    MUTEX_INSPECTOR;
    if (!__UU) {
        return;
    }
    user_pair __U(__UU);
    deque<string> vc=splitTOKd(" ",cmd.data);
    if (vc.size()==0)
    {
        irc_send_reply(__UU,403,"Bad command.");
        return;
    }
    string ircchan=vc[0];
    if (ircchan.size())
    {
        if (ircchan[0]=='#')
            ircchan=ircchan.substr(1,ircchan.size()-1);
    }

    PTR_getter<chat_channel> ML=chat_channels.___get_s(ircchan);
    if (!ML)
    {
        irc_send_reply(__UU,403,"Unknown channel.");
        logErr2("Unknown channel %s (%s)",ircchan.c_str(),cmd.data.c_str());
        return;
    }



    if (vc.size()==1) // NO DATA. MODE REQUEST
    {
        bool moderated=false;
        {
            __CH_LOCK(ML);
            moderated=CH->cd.moderated;
        }
        string opt;
        {
            user_pair __U(__UU);
            if (user$level(__U)/sub_levels<am_vip && !user__privileges(__U,"$topic")) opt="+t";
            if (moderated) {
                if (opt.size()) opt+="m";
                else opt="+m";
            }
        }
        string sss;
        sss<<":"<<string(irc_server_config.host)<<" 324 "<<get_cnick(__U)<<" #"<<ircchan<<" "<<opt<<"\r\n";
        data_push_back_irc(__UU,sss);
        return;

    }

    bool moderated=false;
    CH_id ch_id;
    CONTAINER(ch_id)=0;
    {
        __CH_LOCK(ML);
        moderated=CH->cd.moderated;
        ch_id=CH->cd.chid;
    }
    string dop;
    if (vc.size()>1) {
        dop=strupper(vc[1]);
    }
    if (!dop.size())
    {
        string opt;
        {
            user_pair __U(__UU);
            if (user$level(__U)/sub_levels<am_vip && !user__privileges(__U,"$topic")) opt="+t";
            if (moderated) {
                if (opt.size()) opt+="m";
                else opt="+m";
            }
        }
        user_pair __U(__UU);
        string cnick=get_cnick(__U);
        {

            string sss;
            sss<<":"<<string(irc_server_config.host)<<" 324 "<<cnick<<" #"<<ircchan<<" "<<opt<<"\r\n";
            data_push_back_irc(__UU,sss);
        }
    }
    if (dop=="+E") {		//get excepts list
        irc_send_reply(__UU,368,ircchan,"End of exception list");
        return;
    }
    if (dop=="+B") {	//get ban list
        irc_send_reply(__UU,368,ircchan,"End of ban list");
        return;
    }
    if (dop=="+I") {	//get invite list
        irc_send_reply(__UU,347,ircchan,"End of invite list");
        return;
    }
    if (dop=="+V" || dop=="-V" )
    {
        if (vc.size()!=3)
        {
            irc_send_reply(__UU,403,"Bad syntax.");
            return;
        }
        string n=vc[2];
        DB_id zid=all_user_nick_2_DB_id(n);
        if (CONTAINER(zid)==0)
        {
            irc_send_reply(__UU,401,"No such nick/channel.");
            return;
        }
        string znick;
        {
            PGLR(user_profile,z,user$profiles.FindByID(CONTAINER(zid)));
            znick=z->last_nick;
        }
        bool state=false;
        if (dop=="+V") state=true;
        if (!do_change_voice(ch_id,user$id(__UU),zid,state)) {
            irc_send_reply(__UU,481,"Operation not permitted.");
        }
        return;
    }
    if (dop=="+M" || dop=="-M")
    {
        bool state=false;
        if (dop=="+M") state=true;
        if (!do_change_mode(ML,__UU,state)) {
            irc_send_reply(__UU,481,"Operation not permitted.");
        }
        return;

    }
    if (dop=="+BN" &&  user__privileges(__U,"$oldban"))
    {
        if (vc.size()>2)
        {
            bool r=do_nickban(vc[2],user$id(__U),user$level(__U)/sub_levels);

            irc_send_reply(__UU,481,(string)"Ban MODE: "+(r?"success":"failure")+" +BN "+vc[2]);

        }
    }
    if (dop=="+BI" || dop=="+BL"|| dop=="+BLI")
    {
        // #channel +BL nick timesec pri4ina
        // #channel +BI ipaddress timesec pri4ina
        // #channel +BLI nick timesec pri4ina
        // #channel +BN nick
        if (vc.size()<4)
        {
            if (dop=="+BL")
                irc_send_reply(__UU,403,"Bad syntax. syntax: mode #channel +BL nick timesecond description");
            if (dop=="+BI")
                irc_send_reply(__UU,403,"Bad syntax. syntax: mode #channel +BI IP timesecond description");
            if (dop=="+BLI")
                irc_send_reply(__UU,403,"Bad syntax. syntax: mode #channel +BLI nick timesecond description");
            return;
        }
        string obj=vc[2];
        time_t timeban=atoi(vc[3]);
        vc.pop_front();
        vc.pop_front();
        vc.pop_front();
        vc.pop_front();
        string descr=join(" ",vc);
        if (dop=="+BL"||dop=="+BI"||dop=="+BLI"||dop=="+BIL")
        {
            klap_item bi;
            bi.id=get_next_klap_id();
            bi.descr=remove_tags(descr);
            bi.setcurtime(timeban);
            bi.channel=ch_id;
            bi.adm_uid=user$id(__UU);
            if (dop=="+BL")
            {

                bi.uid=all_user_nick_2_DB_id(obj);

                bi.user_name=get_klap_uname(bi.uid);
                bi.adm_name=get_klap_uname(user$id(__UU));
                bi.alevel=get_klap_ulevel(user$id(__UU));
                bi.ulevel=get_klap_ulevel(bi.uid);

                string res=add_klap$$(bi);
                irc_send_reply(__UU,481,"Ban MODE: "+res);
            }
            if (dop=="+BI")
            {

                CONTAINER(bi.uid)=0;

                bi.adm_name=get_klap_uname(user$id(__UU));
                bi.alevel=get_klap_ulevel(user$id(__UU));


                string res=add_klap$$(bi);
                irc_send_reply(__UU,481,"Ban MODE: "+res);
            }
            if (dop=="+BLI") {

                bi.uid=all_user_nick_2_DB_id(obj);

                st_dbh d;
                bi.user_name=get_klap_uname(bi.uid);
                bi.adm_name=get_klap_uname(user$id(__UU));
                bi.alevel=get_klap_ulevel(user$id(__UU));
                bi.ulevel=get_klap_ulevel(bi.uid);


                string res=add_klap$$(bi);
                irc_send_reply(__UU,481,"Ban MODE: "+res);
            }
        }


    }


}
void irc_cmd_KICK(const PTR_getter<chat_user> &__UU,irc_command &cmd)
{
    MUTEX_INSPECTOR;
    st_update stu;

    user_pair __U(__UU);
    {
        string cnick;
        string login;
        unsigned int levelsub_levels;
        string kmsg;
        string nick;
        string ip;
        cnick=get_cnick(__U);
        login=user$login(__U);
        levelsub_levels=user$level(__U)/sub_levels;
        {
            __UC_LOCK;
            ip=u->irc_ip;
        }
        size_t pz=cmd.data.find(" ",0);
        size_t ppz=cmd.data.find(":",pz);
        if (pz>0) {
            if (ppz!=-1) {
                nick=remove_spaces(cmd.data.substr(pz,ppz-pz));
                kmsg=cmd.data.substr(ppz+1,cmd.data.size()-ppz-1);
            }
            if (ppz==-1) nick=remove_spaces(cmd.data.substr(pz,cmd.data.size()-pz));
        }
        if (!nick.size()) {
            irc_send_reply(__U,401,"No such nick/channel");
            return;
        }
        bool noprivs=false;
        if (levelsub_levels<am_killer) noprivs=true;
        bool done=false;
        if (!noprivs) {
            DB_id zid=all_user_nick_2_DB_id(nick);
            if (CONTAINER(zid))
            {

                unsigned int zlevel;
                string znick,zlogin;

                PTR_getter<user_profile> ZP=user$profiles.FindByID(CONTAINER(zid));
                if (ZP)
                {
                    PGLR(user_profile,z,ZP);
                    zlogin=z->login;
                    zlevel=z->level;
                    znick=z->last_nick;
                }

                int u_kick_ability=__LEVELS(user$level(__U)).kick_ability;
                int z_kick_shield=__LEVELS(zlevel).kick_shield;
                if (u_kick_ability>z_kick_shield)
                {
                    kick_user(zid,kmsg);


                    logErrN("users","User %s (%s) was kicked by %s (%s) - %s [%s]",znick.c_str(),zlogin.c_str(),cnick.c_str(),login.c_str(),kmsg.c_str(),ip.c_str());
                    done=true;
                    st_dbh d;
                    d.dbh->real_queryQ((QUERY)"update tbl_users set kick_count=kick_count+1 where id='?'"<<CONTAINER(user$id(__UU)));
                    d.dbh->real_queryQ((QUERY)"update tbl_users set kicked_count=kicked_count+1 where id='?'"<<CONTAINER(zid));

                } else {
                    irc_send_reply(__U,481,"Permission denied. Not enough kick ability.");
                    noprivs=true;
                    return;
                }
            }
        }
        if (noprivs) {
            irc_send_reply(__U,481,"Permission denied. You not an IRC operator, or you have few privileges.");
            return;
        }
        if (!done) {
            irc_send_reply(__U,401,"No such nick/channel");
            return;
        }
    }

}
void irc_cmd_EYE(const PTR_getter<chat_user> &__UU,irc_command &cmd)
{
    MUTEX_INSPECTOR;
    st_update stu;
    user_pair __U(__UU);
    {
        int ulevel;
        ulevel=user$level(__U);
        if (ulevel/sub_levels<am_admin) {
            irc_send_reply(__U,481,"Permission denied. You not an IRC operator, or you have few privileges.");
            return;
        }
        DB_id id=user$id(__UU);
        if (strupper(cmd.data)=="ON") {
            DB_id id=user$id(__UU);
            {
                PGLW(user_profile, u,__U.up);
                u->setting_show_eye=true;
                __send_update_d3(user_profile$setting_show_eye$3,CONTAINER(id),u->setting_show_eye);
            }
            irc_send_reply(__U,300,"Eye state is on ON");
        } else if (strupper(cmd.data)=="OFF") {
            {
                PGLW(user_profile, u,__U.up);
                u->setting_show_eye=false;
                __send_update_d3(user_profile$setting_show_eye$3,CONTAINER(id),u->setting_show_eye);
            }
            irc_send_reply(__U,300,"Eye state is on OFF");
        } else {
            irc_send_reply(__U,421,cmd.command+": not enough arguments. Syntax: EYE on|off");
        }
    }
}
void irc_cmd_EYEN(const PTR_getter<chat_user> &__UU,irc_command &cmd)
{
    MUTEX_INSPECTOR;
    st_update stu;
    user_pair __U(__UU);
    DB_id id=user$id(__UU);
    {
        PGLW(user_profile, u,user$profiles.FindByID(__U.up));
        u->setting_neye=atoi(cmd.data);
        __send_update_d3(user_profile$setting_neye$3,CONTAINER(id),u->setting_neye);
    }
    irc_send_reply(__U,300,"Eye number set to "+itostring(atoi(cmd.data)));
}
