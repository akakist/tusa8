#ifndef CHAT_CHANNLE_DATA___H
#define CHAT_CHANNLE_DATA___H


class chat_channel_data
{

public:
    struct klap
    {
        time_t time_ban_end;
        string time_cnt_str;
        string za;
    };
    chat_channel_data() {
        CONTAINER(chid)=0;
        CONTAINER(move_to_channel)=0;
    }
    CH_id chid;
    CH_id move_to_channel;
    bool move;
    bool save_messages;
    bool censored;
    string topic;
    string set_by;
    time_t topic_time;
    int read_level;
    int write_level;
    bool extended;
    bool moderated;
    string name;
    string name2;
    string irc_name;
    bool smiles_allowed;
    set < DB_id > moderators;
    map < DB_id, klap > klaped_logins_stuff;

};
out_oscar_buffer & operator<< (out_oscar_buffer & b,const chat_channel_data & n);
oscar_buffer & operator>> (oscar_buffer & b, chat_channel_data & n);

out_oscar_buffer & operator<< (out_oscar_buffer & b,const chat_channel_data::klap & n);
oscar_buffer & operator>> (oscar_buffer & b, chat_channel_data::klap & n);

#endif
