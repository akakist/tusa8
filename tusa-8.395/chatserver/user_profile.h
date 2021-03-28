#ifndef USER_PROFILE___H
#define USER_PROFILE___H

#include <string>
#include <map>
using namespace std;

class user_profile: public db_row
{
public:
    unsigned int id;
    const char* getclassname()const {
        return "user_profile";
    }

    int load_from_db(const string & table_name) ;
    user_profile();
    ~user_profile();
    string login;
    int bought_invisibility;
    unsigned int level;
    set<string> __privileges;
    int setting_neye;
    unsigned int setting_n_color_pack;
    unsigned int last_nick_id;
    string last_nick;
    CH_id last_channel;
    unsigned int last_status_id;
    unsigned int contact_options;
    bool ul_mode_contacts_only;
    bool ul_mode_hide_male;
    bool ul_mode_hide_female;

    unsigned int  notes_msg_per_page;
    bool  notes_save_copy;
    unsigned char  notes_sort;
    bool  notes_sort_desc;
    bool setting_show_eye;
    bool unreged;
    time_t birth_date;

    enum {
        CONTACT_ADD_IN_PRIV=0x01,
        CONTACT_ADD_OUT_PRIV=0x02

    };
    map<unsigned int,user_status> status;
private:
    int gender;
public:
    void set_gender(unsigned char g);
    unsigned char get_gender() const;

    void on_id_not_exists(const string &tbname, unsigned int _id);


};
extern db_map<unsigned int,user_profile>user$profiles;
#define __UP__ user$profiles
#endif
