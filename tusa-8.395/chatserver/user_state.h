#ifndef USER___STATE___H
#define USER___STATE___H
struct user_status;
class user_nick;
struct user_pair;

enum CT {
    T_UNKNOWN=0,
    T_WWW=1,
    T_IRC=2,
};
out_oscar_buffer & operator<<(out_oscar_buffer&b, const CT& n);
oscar_buffer & operator>>(oscar_buffer&b, CT& n);


class user_state
{

public:

    enum
    {
        UNDEF=0xffff0000
    };
    map<CH_id,set<CT> > __channel_us;
    void export_channels(set<CH_id>&s) const;

    bool channel_us_count_usr(const CH_id& ch,const CT&p) const;
    bool channel_us_count(const CH_id& ch) const;
    size_t channel_us_size() const;
    user_status status;
    user_nick nick;
    set<CH_id> voice;
    string ip;
    ~user_state() {
    }
    bool operator!=(const user_state& a) const
    {
        if (a.nick!=nick) return true;
        if (a.status!=status) return true;
        if (a.__channel_us!=__channel_us) return true;
        return false;
    }
    user_state()
    {
        status.id=UNDEF;
        nick.id=UNDEF;
    }

    enum
    {
        PART_CHANNEL		=1,
        CHANGE_CONTACT_MODE	=3,
        SET_NICK		=4,
        KICK_USER			=5,
        JOIN_CHANNEL		=6,
        SET_STATUS		=7,
        CHANGE_CHANNEL		=8,
        CONTACT_ADD		=11,
        CONTACT_REMOVE		=12,
        IGNORE_ADD		=17,
        IGNORE_REMOVE	=18,
    };

    void www_channel_replace(const CH_id &chan);

    set<CH_id> get_channel(const CT& _ct) const
    {

        set<CH_id> r;
        for (map<CH_id,set<CT> >::const_iterator i=__channel_us.begin(); i!=__channel_us.end(); i++)
        {

            if (i->second.count(_ct))
                r.insert(i->first);
        }
        return r;

    }
    set<CH_id> part_channels(const set<CH_id>&chans,const CT& _ct);

    void channel_add(const CT& _ct,const CH_id& ch);
};
#define foreach_channel_us(i,__u) for(map<CH_id,set<CT> >::const_iterator i=__u.__channel_us.begin();i!=__u.__channel_us.end();i++)
out_oscar_buffer & operator<<(out_oscar_buffer&b, const user_state& n);
oscar_buffer & operator>>(oscar_buffer&b, user_state& n);

#endif
