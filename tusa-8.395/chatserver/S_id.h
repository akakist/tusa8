#ifndef SSDB__ID__HH
#define SSDB__ID__HH
#include "oscar_buffer.h"
#ifdef DEBUG
struct S_id
{
    unsigned int container;
};
inline int operator < (const S_id &a,const S_id &b) {
    return a.container<b.container;
}
inline bool operator == (const S_id &a,const S_id &b) {
    return a.container==b.container;
}
inline bool operator != (const S_id &a,const S_id &b) {
    return a.container!=b.container;
}
inline out_oscar_buffer & operator<< (out_oscar_buffer& b,const S_id &s) {
    b<<s.container;
    return b;
}
inline oscar_buffer & operator>> (oscar_buffer& b,  S_id &s) {
    b>>s.container;
    return b;
}
#else
typedef unsigned int S_id;
#endif
#endif
