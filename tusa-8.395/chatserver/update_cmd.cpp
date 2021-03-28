#include "stdafx.h"
#ifdef _DEBUG
#define New DEBUG_NEW
#endif

#include <string>
using namespace std;
#include "M_DEQUE.h"
#include "oscar_buffer.h"
#include "DB_id.h"
#include "mutex_inspector.h"
#include "update_cmd.h"
#include "mysql_extens.h"

#include "user.h"
#include "user_profile.h"
#include "chat_thread_event.h"
#include "ignore_vec.h"
#include "channels.h"
#include "copy_file.h"
#include "st_stuff.h"
#include "cutils.h"
#include "user_credits.h"
#include "notes.h"

#include <math.h>
#ifdef DEBUG
#endif
#include "contact_vec.h"
#include "mysql_extens.h"
#include "admin.h"
#include "threadable.h"
#include "time_limit.h"
M_DEQUE<string> update_commands;

void local_free_handle_update_cmd()
{
    update_commands.clear();
}

string attach_pn(const DB_id&  uid,unsigned int msgid, const string& orig);
void note_resort(const DB_id& uid);
int calc_photo_rating(const map<DB_id,int> &m);
void update_datum(const string& ds, const st_dbh& d);



void process_update_command(const string& cmd)
{
    update_commands.push_back(cmd);
}







void update_datum(const string& ds, const st_dbh &d)
{
    CKTIME;
    oscar_buffer o(ds.data(),ds.size());
    int cmd;
    o>>cmd;
    switch (cmd)
    {
    case ignore$add$3:	{
        DB_id uid,zid;
        CONTAINER(uid)=0;
        CONTAINER(zid)=0;
        o>>uid>>zid;
        MUTEX_INSPECTOR;
        PTR_getter<ignore_vec> p=ignore_vecs.get(CONTAINER(uid));
        if (p)
        {
            PGLW(ignore_vec,u,p);
            u->ignore.insert(zid);
        }
        d.dbh->real_queryQ((QUERY)"insert into ignores (uid,cuid) values (?,?)"<<CONTAINER(uid)<<CONTAINER(zid));


    }
    break;
    case ignore$remove$3: {
        DB_id uid,zid;
        CONTAINER(uid)=0;
        CONTAINER(zid)=0;
        o>>uid>>zid;
        MUTEX_INSPECTOR;
        PTR_getter<ignore_vec> p=ignore_vecs.get(CONTAINER(uid));
        if (p)
        {
            PGLW(ignore_vec,u,p);
            u->ignore.erase(zid);
        }
        d.dbh->real_queryQ((QUERY)"delete from ignores where uid=? and cuid=?"<<CONTAINER(uid)<<CONTAINER(zid));

    }
    break;

    case user_profile$contact_options$3: {
        DB_id uid;
        CONTAINER(uid)=0;
        unsigned int v;
        o>>uid>>v;
        MUTEX_INSPECTOR;

        PTR_getter<user_profile> p=user$profiles.get(CONTAINER(uid));
        if (p)
        {
            PGLW(user_profile,u,p);
            u->contact_options=v;
        }
        d.dbh->real_queryQ((QUERY)"update tbl_users set contact_options='?' where id=?"<<v<<CONTAINER(uid));
    }
    break;



    case contact$add$3: {
        DB_id uid,cid;
        CONTAINER(uid)=0;
        CONTAINER(cid)=0;
        o>>uid>>cid;
        MUTEX_INSPECTOR;
        PTR_getter<contact_vec> p=contact_vecs.get(CONTAINER(uid));
        if (p)
        {
            PGLW(contact_vec,u,p);
            u->contact.insert(cid);
        }
        d.dbh->real_queryQ((QUERY)"insert DELAYED into contact (uid,cuid) values (?,?)"<<CONTAINER(uid)<<CONTAINER(cid));
    }
    break;

    case contact$remove$3: {
        DB_id uid,cid;
        CONTAINER(uid)=0;
        CONTAINER(cid)=0;
        o>>uid>>cid;
        MUTEX_INSPECTOR;
        PTR_getter<contact_vec> p=contact_vecs.get(CONTAINER(uid));
        if (p)
        {
            PGLW(contact_vec,u,p);
            u->contact.erase(cid);
        }
        d.dbh->real_queryQ((QUERY)"delete from contact where uid=? and cuid=?"<<CONTAINER(uid)<<CONTAINER(cid));
    }
    break;

    case user_profile$notes_save_copy$3: {
        DB_id uid;
        CONTAINER(uid)=0;
        unsigned int v;
        o>>uid>>v;
        MUTEX_INSPECTOR;
        PTR_getter<user_profile> p=user$profiles.get(CONTAINER(uid));
        if (p)
        {
            PGLW(user_profile,u,p);
            u->notes_save_copy=v;
        }
        d.dbh->real_queryQ((QUERY)"update tbl_users set notes_save_copy='?' where id=?"<<v<<CONTAINER(uid));
    }
    break;
    case user_profile$notes_msg_per_page$3: {
        DB_id uid;
        CONTAINER(uid)=0;
        unsigned int v;
        o>>uid>>v;
        MUTEX_INSPECTOR;
        PTR_getter<user_profile> p=user$profiles.get(CONTAINER(uid));
        if (p)
        {
            PGLW(user_profile,u,p);
            u->notes_msg_per_page=v;
        }
        d.dbh->real_queryQ((QUERY)"update tbl_users set notes_msg_per_page='?' where id=?"<<v<<CONTAINER(uid));

    }
    break;
    case user_profile$notes_sort$3: {
        DB_id uid;
        CONTAINER(uid)=0;
        unsigned int v;
        o>>uid>>v;
        MUTEX_INSPECTOR;
        PTR_getter<user_profile> p=user$profiles.get(CONTAINER(uid));
        if (p)
        {
            PGLW(user_profile,u,p);
            u->notes_sort=v;
        }
        d.dbh->real_queryQ((QUERY)"update tbl_users set notes_sort='?' where id=?"<<v<<CONTAINER(uid));
    }
    break;
    case user_profile$notes_sort_desc$3: {
        DB_id uid;
        CONTAINER(uid)=0;
        unsigned int v;
        o>>uid>>v;
        MUTEX_INSPECTOR;
        PTR_getter<user_profile> p=user$profiles.get(CONTAINER(uid));
        if (p)
        {
            PGLW(user_profile,u,p);
            u->notes_sort_desc=v;
        }
        d.dbh->real_queryQ((QUERY)"update tbl_users set notes_sort_desc='?' where id=?"<<v<<CONTAINER(uid));

    }
    break;


    case user_profile$setting_show_eye$3: {
        DB_id uid;
        CONTAINER(uid)=0;
        unsigned int v;
        o>>uid>>v;
        MUTEX_INSPECTOR;
        PTR_getter<user_profile> p=user$profiles.get(CONTAINER(uid));
        if (p)
        {
            PGLW(user_profile,u,p);
            u->setting_show_eye=v;
        }
        d.dbh->real_queryQ((QUERY)"update tbl_users set setting_show_eye='?' where id=?"<<v<<CONTAINER(uid));
    }
    break;
    case user_profile$setting_neye$3: {
        DB_id uid;
        CONTAINER(uid)=0;
        unsigned int v;
        o>>uid>>v;
        MUTEX_INSPECTOR;
        PTR_getter<user_profile> p=user$profiles.get(CONTAINER(uid));
        if (p)
        {
            PGLW(user_profile,u,p);
            u->setting_neye=v;
        }
        d.dbh->real_queryQ((QUERY)"update tbl_users set setting_neye='?' where id=?"<<v<<CONTAINER(uid));
    }
    break;
    case user_profile$ul_mode_contacts_only$3: {
        DB_id uid;
        CONTAINER(uid)=0;
        bool v;
        o>>uid>>v;
        MUTEX_INSPECTOR;
        PTR_getter<user_profile> p=user$profiles.get(CONTAINER(uid));
        if (p)
        {
            PGLW(user_profile,u,p);
            u->ul_mode_contacts_only=v;
        }
        d.dbh->real_queryQ((QUERY)"update tbl_users set ul_mode_contacts_only='?' where id=?"<<v<<CONTAINER(uid));
    }
    break;
    case user_profile$ul_mode_hide_male$3: {
        DB_id uid;
        CONTAINER(uid)=0;
        bool v;
        o>>uid>>v;
        MUTEX_INSPECTOR;
        PTR_getter<user_profile> p=user$profiles.get(CONTAINER(uid));
        if (p)
        {
            PGLW(user_profile,u,p);
            u->ul_mode_hide_male=v;
        }
        d.dbh->real_queryQ((QUERY)"update tbl_users set ul_mode_hide_male='?' where id=?"<<v<<CONTAINER(uid));
    }
    break;
    case user_profile$ul_mode_hide_female$3: {
        DB_id uid;
        CONTAINER(uid)=0;
        bool v;
        o>>uid>>v;
        MUTEX_INSPECTOR;
        PTR_getter<user_profile> p=user$profiles.get(CONTAINER(uid));
        if (p)
        {
            PGLW(user_profile,u,p);
            u->ul_mode_hide_female=v;
        }
        d.dbh->real_queryQ((QUERY)"update tbl_users set ul_mode_hide_female='?' where id=?"<<v<<CONTAINER(uid));
    }
    break;

    case user_profile$last_channel$3: {
        DB_id uid;
        CONTAINER(uid)=0;
        CH_id si;
        CONTAINER(si)=0;
        o>>uid>>si;
        MUTEX_INSPECTOR;
        PTR_getter<user_profile> p=user$profiles.get(CONTAINER(uid));
        if (p)
        {
            PGLW(user_profile,u,p);
            u->last_channel=si;
        }
        d.dbh->real_queryQ((QUERY)"update tbl_users set last_channel='?' where id=?"<<CONTAINER(si)<<CONTAINER(uid));

    }
    break;

    case chat_thread$set_status$3:
        break;

    case user_profile$inc_stat_m_count$3:
    {
        MUTEX_INSPECTOR;
        DB_id uid;
        CONTAINER(uid)=0;
        int n;
        o>>uid>>n;
        d.dbh->real_queryQ((QUERY)"update tbl_users set stat_m_count=stat_m_count+'?' where id='?'"<<n<<CONTAINER(uid));
    }
    break;
    case user_profile$add_stat_t_count$3:
    {
        MUTEX_INSPECTOR;
        DB_id uid;
        CONTAINER(uid)=0;
        unsigned int tc;
        o>>uid>>tc;

        PTR_getter<user_profile> p=user$profiles.get(CONTAINER(uid));

        d.dbh->real_queryQ((QUERY)"update tbl_users set stat_t_count=stat_t_count+? where id=?"<<tc<<CONTAINER(uid));
    }
    break;




    case channel$topic$5:	{
        string top,set_by;
        time_t t;
        CH_id ch;
        CONTAINER(ch)=0;
        o>>ch>>top>>set_by>>t;
        __CH_LOCKW(chat_channels[ch]);
        CH->cd.topic=top;
        CH->cd.set_by=set_by;
        CH->cd.topic_time=t;

    }
    break;






    case nick$update_nick$4:
    {
        MUTEX_INSPECTOR;
        DB_id uid;
        CONTAINER(uid)=0;
        unsigned int nid;
        string name;
        o>>uid>>nid>>name;

        {

            PTR_getter<nick_vec> p=nick_vecs.get(CONTAINER(uid));
            if (p)
            {
                PGLW(nick_vec,u,p);
                u->u_nicks[nid].name=name;

            }
            d.dbh->real_queryQ((QUERY)"update nicks set nick='?' where uid=? and id=?"
                               <<name<<CONTAINER(uid)<<nid);
        }
    }
    break;
    case nick$add$3:
    {
        MUTEX_INSPECTOR;
        user_nick n;
        DB_id uid;
        CONTAINER(uid)=0;
        o>>uid;
        o>>n;


        PTR_getter<nick_vec> p=nick_vecs.get(CONTAINER(uid));
        if (p)
        {
            PGLW(nick_vec,u,p);
            u->u_nicks[n.id]=n;
        }
        d.dbh->real_queryQ((QUERY)"insert DELAYED into nicks (id,uid,nick,str_nick,banned) values ('?','?','?','?','0')"
                           <<n.id<<CONTAINER(uid)<<n.name<<str_nick(n.name));
    }
    break;
    case nick$remove$3:
    {
        MUTEX_INSPECTOR;
        DB_id uid;
        CONTAINER(uid)=0;
        unsigned int nid;
        o>>uid>>nid;

        PTR_getter<nick_vec> p=nick_vecs.get(CONTAINER(uid));
        if (p)
        {
            PGLW(nick_vec,u,p);
            u->u_nicks.erase(nid);
        }
        d.dbh->real_queryQ((QUERY)"delete from nicks where uid=? and id=?"<<CONTAINER(uid)<<nid);
    }
    break;





    case note$set_msg$4:
    {
        MUTEX_INSPECTOR;
        DB_id uid;
        CONTAINER(uid)=0;
        unsigned int nid;
        string msg;
        o>>uid>>nid>>msg;
        PTR_getter<note_text> p=note_texts.get(itostring(CONTAINER(uid))+"."+itostring(nid));
        if (p) {
            PGLW(note_text,n,p);
            n->body=msg;
        }
        string q="insert into note_texts (message_id,body,owner_uid) values ('"+itostring(nid)+ "','"+MES(msg)+"',"+
                 itostring(CONTAINER(uid))+")";
        d.dbh->real_query(q);
    }
    break;

    case note$folder_create$4:
    {
        MUTEX_INSPECTOR;
        DB_id uid;
        CONTAINER(uid)=0;
        int fold;
        string fname;
        o>>uid>>fold>>fname;
        PTR_getter<u_notes> p=__notes.get(CONTAINER(uid));
        if (p) {
            PGLW(u_notes,u,p);
            u->map_folders[fold].name=fname;
        }
        d.dbh->real_queryQ((QUERY)"insert into note_folders (id,uid,name) values (?,?,'?')"<<fold<<CONTAINER(uid)<<fname);
    }
    break;
    case note$folder_rename$4:
    {
        MUTEX_INSPECTOR;

        DB_id uid;
        CONTAINER(uid)=0;
        int fold;
        string fname;
        o>>uid>>fold>>fname;
        PTR_getter<u_notes> p=__notes.get(CONTAINER(uid));
        if (p) {
            PGLW(u_notes,u,p);
            u->map_folders[fold].name=fname;
        }
        d.dbh->real_queryQ((QUERY)"update note_folders set name='?' where id=? and uid=?"<<fname<<fold<<CONTAINER(uid));
    }
    break;
    case note$folder_remove$3:
    {
        MUTEX_INSPECTOR;
        DB_id uid;
        CONTAINER(uid)=0;
        int fold;
        o>>uid>>fold;
        PTR_getter<u_notes> p=__notes.get(CONTAINER(uid));
        if (p) {
            PGLW(u_notes,u,p);
            u->map_folders.erase(fold);
        }
        d.dbh->real_queryQ((QUERY)"delete from note_folders where id=? and uid=?"<<fold<<CONTAINER(uid));
    }
    break;
    case note$set_status$4:
    {
        MUTEX_INSPECTOR;
        DB_id uid;
        CONTAINER(uid)=0;
        int msgid;
        int status;
        o>>uid>>msgid>>status;
        PTR_getter<u_notes> p=__notes.get(CONTAINER(uid));
        if (p) {
            PGLW(u_notes,u,p);
            map<unsigned int,__note_head>::iterator h=u->map_msgs.find(msgid);
            if (h!=u->map_msgs.end())
            {
                h->second.nfh.status=status;
            }
        }
        d.dbh->real_queryQ((QUERY)"update notes set status=? where id=? and owner_uid=?"<<status<<msgid<<CONTAINER(uid));
    }
    break;
    case note$set_unread$3:
    {
        MUTEX_INSPECTOR;
        DB_id uid;
        CONTAINER(uid)=0;
        int msgid;

        o>>uid>>msgid;
        PTR_getter<u_notes> p=__notes.get(CONTAINER(uid));
        if (p) {
            PGLW(u_notes,u,p);
            map<unsigned int,__note_head>::iterator h=u->map_msgs.find(msgid);
            if (h!=u->map_msgs.end())
            {
                h->second.nfh.unread=false;
            }
        }
        d.dbh->real_queryQ((QUERY)"update notes set unread='0' where id=? and owner_uid=?"<<msgid<<CONTAINER(uid));
    }
    break;
    case note$filter_out_delete$4:
    {
        MUTEX_INSPECTOR;
        DB_id uid;
        CONTAINER(uid)=0;
        int fid;
        DB_id zid;
        CONTAINER(zid)=0;
        o>>uid>>fid>>zid;
        PTR_getter<u_notes> p=__notes.get(CONTAINER(uid));
        if (p)
        {
            PGLW(u_notes,u,p);
            u->map_folders[fid].filter_out.erase(CONTAINER(zid));
        }
        d.dbh->real_queryQ((QUERY)"delete from filters_out where folder_id=? and owner_uid=? and uid=?"<<
                           fid<<CONTAINER(uid)<<CONTAINER(zid));
    }
    break;
    case note$filter_out_insert$4:
    {
        MUTEX_INSPECTOR;
        DB_id uid;
        CONTAINER(uid)=0;
        int fid;
        DB_id zid;
        CONTAINER(zid)=0;
        o>>uid>>fid>>zid;
        PTR_getter<u_notes> p=__notes.get(CONTAINER(uid));
        if (p)
        {
            PGLW(u_notes,u,p);
            u->map_folders[fid].filter_out.insert(CONTAINER(zid));
        }
        d.dbh->real_queryQ((QUERY)"insert into filters_out (folder_id,owner_uid,uid) values (?,?,?)"<<fid<<CONTAINER(uid)<<CONTAINER(zid));
    }
    break;

    case note$filter_in_delete$4:
    {
        MUTEX_INSPECTOR;
        DB_id uid;
        CONTAINER(uid)=0;
        int fid;
        DB_id zid;
        CONTAINER(zid)=0;
        o>>uid>>fid>>zid;
        PTR_getter<u_notes> p=__notes.get(CONTAINER(uid));
        if (p)
        {
            PGLW(u_notes,u,p);
            u->map_folders[fid].filter_in.erase(CONTAINER(zid));
        }
        d.dbh->real_queryQ((QUERY)"delete from filters_in where folder_id=? and owner_uid=? and uid=?"<<
                           fid<<CONTAINER(uid)<<CONTAINER(zid));
    }
    break;
    case note$filter_in_insert$4:
    {
        MUTEX_INSPECTOR;
        DB_id uid;
        CONTAINER(uid)=0;
        int fid;
        DB_id zid;
        CONTAINER(zid)=0;
        o>>uid>>fid>>zid;
        PTR_getter<u_notes> p=__notes.get(CONTAINER(uid));
        if (p)
        {
            PGLW(u_notes,u,p);
            u->map_folders[fid].filter_in.insert(CONTAINER(zid));
        }
        d.dbh->real_queryQ((QUERY)"insert into filters_in (folder_id,owner_uid,uid) values (?,?,?)"<<fid<<CONTAINER(uid)<<CONTAINER(zid));
    }
    break;
    case note$set_head$4:
    {
        MUTEX_INSPECTOR;
        DB_id uid;
        CONTAINER(uid)=0;
        int fid;
        __note_head hh;

        o>>uid>>fid>>hh;
        note_file_head &h=hh.nfh;
        PTR_getter<u_notes> p=__notes.get(CONTAINER(uid));
        if (p)
        {
            PGLW(u_notes,u,p);
            u->map_msgs[hh.id]=hh;
            u->map_folders[fid].vec_msgs.push_back(hh.id);
        }
        QUERY q=(QUERY)"insert into notes ( "
                "from_nick_id,to_nick_id,"
                "from_uid,to_uid,"
                "from_nick,to_nick,"
                "refcount, "
                "subject,send_date,"
                "status,unread,"
                "content_length, has_attachment,"
                "attachment_filename,attachment_content_length, important,system,owner_uid,owner_folder,notify_read,id) "
                "	values('?','?','?','?','?','?','?','?',FROM_UNIXTIME(?),'?','?','?','?','?','?','?','?','?','?','?','?') "
                <<h.from_nick_id<<h.to_nick_id
                <<h.from_uid<<h.to_uid
                <<h.from_nick<<h.to_nick
                <<1
                <<h.subject<<h.send_date
                <<h.status<<h.unread
                <<h.content_length<<h.has_attachment
                <<h.attachment_filename<<h.attachment_content_length
                <<h.important<<h.system<<CONTAINER(uid)<<fid<<h.notify_read<<hh.id;
        string sql=q.prepare();
        d.dbh->real_query(sql);

        note_resort(uid);

    }
    break;
    case note$remove$3:
    {
        MUTEX_INSPECTOR;
        DB_id uid;
        CONTAINER(uid)=0;
        int msgid;
        o>>uid>>msgid;

        bool need_del_att=false;
        string att;
        PTR_getter<u_notes> p=__notes.get(CONTAINER(uid));
        if (p)
        {
            PGLW(u_notes,u,p);

            for (map<unsigned int,__note_folder>::iterator i=u->map_folders.begin(); i!=u->map_folders.end(); i++)
            {

                vector<unsigned int> vec_clean;
                for (int j=0; j<i->second.vec_msgs.size(); j++)
                {
                    if (i->second.vec_msgs[j]!=msgid)
                        vec_clean.push_back(i->second.vec_msgs[j]);
                }
                i->second.vec_msgs=vec_clean;
            }
            __note_head &h=u->map_msgs[msgid];
            if (h.nfh.has_attachment)
            {
                att=attach_pn(uid,msgid,h.nfh.attachment_filename);
                need_del_att=true;
            }
            u->map_msgs.erase(msgid);
        }
        note_resort(uid);

        if (need_del_att)
        {
#ifdef WIN32
            remove(att.c_str());
#else
            unlink(att.c_str());
#endif
        }
        d.dbh->real_queryQ((QUERY)"delete from notes where id=? and owner_uid=?"<<msgid<<CONTAINER(uid));
        d.dbh->real_queryQ((QUERY)"delete from note_texts where message_id=? and owner_uid=?"<<msgid<<CONTAINER(uid));

    }
    break;
    case note$move_message$5:
    {
        MUTEX_INSPECTOR;

        DB_id uid;
        CONTAINER(uid)=0;
        int msgid;
        int fold,tofold;
        o>>uid>>msgid>>fold>>tofold;
        PTR_getter<u_notes> p=__notes.get(CONTAINER(uid));
        if (p)
        {
            PGLW(u_notes,u,p);
            for (vector<unsigned int>::iterator i=u->map_folders[fold].vec_msgs.begin(); i!=u->map_folders[fold].vec_msgs.end(); i++)
            {
                if (*i==msgid)
                {
                    u->map_folders[fold].vec_msgs.erase(i);
                    break;
                }
            }
            u->map_folders[tofold].vec_msgs.push_back(msgid);
        }
        if (p)note_resort(uid);
        d.dbh->real_queryQ((QUERY)"update notes set owner_folder=? where owner_uid=? and id=?"<<tofold<<CONTAINER(uid)<<msgid);

    }
    break;
    case note$set_attachment$2:
    {
        MUTEX_INSPECTOR;
        string SS;
        o>>SS;
        oscar_buffer o(SS.data(),SS.size());
        int n;
        o>>n;
        DB_id tuid,fuid;
        CONTAINER(tuid)=0;
        CONTAINER(fuid)=0;
        int tmsgid=0,fmsgid=0;
        string attfn;
        string body;
        if (n==1)
        {
            o>>tuid>>tmsgid;
        }
        else if (n==2)
        {
            o>>tuid>>tmsgid;
            o>>fuid>>fmsgid;
        }
        o>>attfn;
        o>>body;

        string pn_to=attach_pn(tuid,tmsgid,attfn);

        check_path_wfn(pn_to);
        {
            st_FILE f(pn_to,"wb");
            f.write(body);
        }
        if (n==2)
        {
            string pn_from=attach_pn(fuid,fmsgid,attfn);
            check_path_wfn(pn_from);
#ifdef WIN32
            {
                st_FILE f(pn_from,"wb");
                f.write(body);
            }
#else
            link(pn_to.c_str(),pn_from.c_str());
#endif
        }


    }
    break;

    case ban$add$2: {
        klap_item it;
        o>>it;
        _klap_stuff.add__klap(it);
        QUERY q=(QUERY)"insert into bans (id,type,ip,uid,descr,"
                "time_ban_start,time_ban_end,time_cnt_str,channel,adm_uid,adm_name,user_name,alevel,ulevel) values ('?','?','?','?','?',"
                "FROM_UNIXTIME('?'),FROM_UNIXTIME('?'),'?','?','?','?','?','?','?')"
                <<
                CONTAINER(it.id)<<
                CONTAINER(it.uid)<<
                it.descr<<
                it.time_ban_start<<
                it.time_ban_end<<
                it.time_cnt_str<<
                CONTAINER(it.channel)<<
                CONTAINER(it.adm_uid)<<
                it.adm_name<<it.user_name<<it.alevel<<it.ulevel;
        string s=q.prepare();
        d.dbh->real_query(s);
    }
    break;
    case ban$remove$2: {
        klap_item bi;
        o>>bi;
        _klap_stuff.remove_klap(bi.id);
        d.dbh->real_queryQ((QUERY)"delete from bans where id='?' "<<CONTAINER(bi.id));
    }
    break;
    case moderator$add$3:
    {

        DB_id uid;
        CONTAINER(uid)=0;
        CH_id channel;
        CONTAINER(channel)=0;
        o>>uid>>channel;
        {
            if (chat_channels.count(channel))
            {
                __CH_LOCKW(chat_channels[channel]);
                CH->cd.moderators.insert(uid);
            }
        }

    }
    break;
    case moderator$remove$2:
    {
        DB_id uid;
        CONTAINER(uid)=0;
        o>>uid;
        vector<CH_id> v=chat_channels._keys();
        for (unsigned i=0; i<v.size(); i++)
        {
            CH_id channel=v[i];
            __CH_LOCKW(chat_channels[channel]);
            CH->cd.moderators.erase(uid);
        }
    }
    break;
    case user_profile$privs$3:
        break;

    case channel$update$3: {
        chat_channel_data cd;
        CH_id channel;
        CONTAINER(channel)=0;
        o>>cd>>channel;
        out_oscar_buffer b;
        if (chat_channels.count(channel))
        {
            __CH_LOCKW(chat_channels[channel]);
            CH->cd=cd;
        }

    }
    break;
    default:
        ;
#ifdef DEBUG
        logErr2("unhandled update for %d",cmd);
#endif
    }
}
void *update_processor(void *pr)
{
    log_start_thread("updater");
    while (1) {
        try {

            st_dbh d;
            if (g_must_terminate)return NULL;
            {


            }

            deque<string> de=update_commands.extract_all();
            for (unsigned i=0; i<de.size(); i++)
            {
                MUTEX_INSPECTOR;
                chat_event_UPDATE_DATA *evu=new chat_event_UPDATE_DATA(de[i],0);
                LEAKCTL_ADD(evu);
                send_chat_event_push_back(evu);
                update_datum(de[i],d);
            }
            usleep(11111);
        }
        catch (cError e) {
            logErr2("--Error: updater! %s %s %d",e.what(),__FILE__,__LINE__);
            sleep(1);
        }
        catch (...) {
            logErr2("--Error: updater: unknow error! %s %d",__FILE__,__LINE__);
            sleep(1);
        }
    }
    return NULL;
}

void start_update_processor()
{
    g_create_thread(update_processor,NULL,"update_processor");

}


