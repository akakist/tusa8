#ifndef update_cmd__h
#define update_cmd__h
#include "CH_id.h"
enum UCMD {
    ban$add$2,
    ban$remove$2,
    channel$topic$5,
    channel$update$3,
    contact$add$3,
    contact$remove$3,
    ignore$add$3,
    ignore$remove$3,
    moderator$add$3,
    moderator$remove$2,
    nick$add$3,
    nick$update_nick$4,
    nick$remove$3,
    nick$unmark_for_sale$3,
    note$filter_in_delete$4,
    note$filter_in_insert$4,
    note$filter_out_delete$4,
    note$filter_out_insert$4,
    note$folder_create$4,
    note$folder_remove$3,
    note$folder_rename$4,
    note$move_message$5,
    note$remove$3,
    note$set_attachment$2,
    note$set_head$4,

    note$set_status$4,
    note$set_unread$3,
    note$set_msg$4,
    user_profile$contact_options$3,
    user_profile$privs$3,
    user_profile$set_nick$3,
    chat_thread$set_status$3,
    user_profile$last_channel$3,
    user_profile$setting_neye$3,
    user_profile$notes_msg_per_page$3,
    user_profile$notes_save_copy$3,
    user_profile$notes_sort$3,
    user_profile$notes_sort_desc$3,
    user_profile$setting_show_eye$3,
    user_profile$ul_mode_contacts_only$3,
    user_profile$ul_mode_hide_female$3,
    user_profile$ul_mode_hide_male$3,
    user_profile$add_stat_t_count$3,

    user_profile$inc_stat_m_count$3,
    user_profile$stat_last_ip$3,
};
struct user_status;


void process_update_command_local(const string& cmd);
void process_update_command(const string& cmd);

struct st_update
{
    vector<string> container;
    void __process_update_command(const string&s)
    {
        container.push_back(s);
    }
    ~st_update()
    {
        for (unsigned i=0; i<container.size(); i++)
        {
            process_update_command(container[i]);
        }
    }

};

#define __send_update_d3(a,b, c)\
{\
	out_oscar_buffer __$o;\
	__$o<<a<<b<<c;\
	stu.__process_update_command(__$o.as_string());\
}
#define __send_update_d4(a,b, c,d)\
{\
	out_oscar_buffer __$o;\
	__$o<<a<<b<<c<<d;\
	stu.__process_update_command(__$o.as_string());\
}
#define  __send_update_d2(a,b)\
{\
	out_oscar_buffer __$o;\
	__$o<<a<<b;\
	stu.__process_update_command(__$o.as_string());\
}




#define  __send_update_d5(a,b, c, d,e)\
{\
	out_oscar_buffer __$o;\
	__$o<<a<<b<<c<<d<<e;\
	stu.__process_update_command(__$o.as_string());\
}


struct st_dbh;
#endif
