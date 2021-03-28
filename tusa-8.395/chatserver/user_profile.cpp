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
#include "cm_container.h"
#include "notes.h"
user_profile::user_profile()
{
    id=0;
    CONTAINER(last_channel)=0;
    gender=0;
    bought_invisibility=0;
    level=0;
    setting_neye=-1;
    setting_n_color_pack=0;
    contact_options=CONTACT_ADD_IN_PRIV|CONTACT_ADD_OUT_PRIV;
    notes_sort=u_notes::sort_date;
    notes_sort_desc=true;
    notes_save_copy=true;
    notes_msg_per_page=20;
    ul_mode_contacts_only=false;
    ul_mode_hide_male=false;
    ul_mode_hide_female=false;
    unreged=false;
    birth_date=0;
}
user_profile::~user_profile() {}



int user_profile::load_from_db(const string& table_name)  // 0 - no error
{
    MUTEX_INSPECTOR;
    unsigned int uid=id;
    if (uid==0) {
        logErr2("uid==0 (%d) %s",uid,_DMI().c_str());
        throw cError("if(get_db_row_id().as_int()==0) "+_DMI());
    }
    st_dbh d;
    vector<vector<string> > v;
    v=d.dbh->execQ((QUERY)"select id,name,pic,disable_invite from user_status where uid=?"<<id);
    status.clear();
    for (unsigned i=0; i<v.size(); i++)
    {
        if (v[i].size()==4)
        {
            user_status s;
            s.id=atoi(v[i][0]);
            s.name=v[i][1];
            s.pic=atoi(v[i][2]);
            s.disable_invite=atoi(v[i][3]);
            status[s.id]=s;
        }
    }
    for (unsigned int i=0; i<msgs_defstatus.size(); i++) {
        user_status c;
        c.id=i;
        c.name=msgs_defstatus.get(gender,i);
        c.pic=i-2;
        c.disable_invite=false;
        status[i]=c;
    }
    status[0].pic=1000;
    status[1].pic=998;
    status[2].pic=999;
    status[7].disable_invite=true;


    {
        vector<string> v=d.dbh->select_1_rowQ((QUERY)	"select "
                                              "login,"  						//0
                                              "gender," 						//1
                                              "bought_invisibility," 			//2
                                              "level,"							//3
                                              "setting_neye,"					//4
                                              "setting_n_color_pack,"			//5
                                              "last_nick_id,"					//6
                                              "last_nick,"						//7
                                              "last_channel,"					//8
                                              "last_status_id,"					//9
                                              "contact_options,"				//10
                                              "notes_msg_per_page,"				//11
                                              "notes_save_copy,"				//12
                                              "notes_sort,"						//13
                                              "notes_sort_desc,"				//14
                                              "setting_show_eye,"				//15
                                              "ul_mode_contacts_only,"			//16
                                              "ul_mode_hide_male,"				//17
                                              "ul_mode_hide_female,"				//18
                                              "unreged,"						//19
                                              "birth_date"					//20

                                              " from tbl_users "
                                              "where id=?"<<id);
        if (v.size()!=21) {
            logErr2("v.size()!=21 %d  %s",v.size(),_DMI().c_str()) ;
            return -1;
        }

        login			=			v[0];
        set_gender(atoi(			v[1]));
        bought_invisibility=atoi(	v[2]);
        level=atoi(					v[3]);
        setting_neye		=atoi(	v[4]);
        setting_n_color_pack=atoi(	v[5]);
        last_nick_id	=atoi(		v[6]);
        last_nick		=			v[7];
        CONTAINER(last_channel)=atoi(v[8]);
        last_status_id	=atoi(		v[9]);
        contact_options	=atoi(		v[10]);
        notes_msg_per_page	=atoi(	v[11]);
        notes_save_copy	=atoi(		v[12]);
        notes_sort		=atoi(		v[13]);
        notes_sort_desc	=atoi(		v[14]);
        setting_show_eye	=atoi(	v[15]);
        ul_mode_contacts_only=atoi(	v[16]);
        ul_mode_hide_male	=atoi(	v[17]);
        ul_mode_hide_female	=atoi(	v[18]);
        unreged				=atoi(	v[19]);
        birth_date			=atoi(	v[20]);


        vector<string> vv=d.dbh->select_1_columnQ((QUERY)"select priv from tbl_privs where uid=?"<<id);

        for (unsigned i=0; i<vv.size(); i++)
        {
            __privileges.insert(vv[i]);
        }


    }
    return 0;
}
unsigned char user_profile::get_gender()const {
    return gender;
}
void user_profile::set_gender(unsigned char g)
{
    gender=g;
    for (unsigned int i=0; i<msgs_defstatus.size(); i++) {
        user_status c;
        c.id=i;
        c.name=msgs_defstatus.get(gender,i);
        c.pic=i-2;
        c.disable_invite=false;
        status[i]=c;
    }
    status[0].pic=1000;
    status[1].pic=998;
    status[2].pic=999;
    status[7].disable_invite=true;


}

void user_profile::on_id_not_exists(const string &tbname, unsigned int _id)
{
    MUTEX_INSPECTOR;
    logErr2("invalid call on_id_not_exists %s %d %s",tbname.c_str(),_id,_DMI().c_str());
    throw cError("invalid call on_id_not_exists %s %d %s",tbname.c_str(),_id,_DMI().c_str());
}

