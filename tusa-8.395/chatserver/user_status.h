#ifndef USER___STATUS__H
#define USER___STATUS__H

struct user_status
{
    string name;
    int pic;
    int id;
    bool disable_invite;
    bool operator!=(const user_status&) const;
};
inline bool operator==(const user_status &s1, const user_status &s2)
{
    if (s1.name != s2.name)return false;
    if (s1.pic != s2.pic)return false;
    if (s1.id != s2.id)return false;
    if (s1.disable_invite != s2.disable_invite)return false;
    return true;
}
inline bool user_status::operator!=(const user_status& a) const
{
    if (a.name!= name) return true;
    if (a.pic!=pic) return true;
    if (a.id!=id) return true;
    if (a.disable_invite!=disable_invite)return true;

    return false;
}
inline out_oscar_buffer & operator<<(out_oscar_buffer&b, const user_status &n)
{
    b<<n.name<<n.pic<<n.id<<n.disable_invite;
    return b;
}

inline oscar_buffer & operator>>(oscar_buffer&b, user_status &n)
{
    b>>n.name>>n.pic>>n.id>>n.disable_invite;
    return b;
}

#endif
