#ifndef CH__ID__HH
#define CH__ID__HH
#ifdef DEBUG

struct CH_id
{
    unsigned int container;
};
inline int operator < (const CH_id& a,const CH_id& b)
{
    if (a.container<b.container) return true;
    return false;
}
inline bool operator == (const CH_id& a,const CH_id& b)
{
    if (a.container==b.container)return true;
    return false;
}
inline bool operator != (const CH_id& a,const CH_id& b)
{
    if (a.container!=b.container)return true;
    return false;
}
inline out_oscar_buffer & operator<< (out_oscar_buffer& b,const CH_id& s) {
    b<<s.container;
    return b;
}
inline oscar_buffer & operator>> (oscar_buffer& b,  CH_id& s) {
    b>>s.container;
    return b;
}
#else
typedef unsigned int CH_id;
#endif
#endif
