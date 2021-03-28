#ifndef DB__ID__HH
#define DB__ID__HH
#include "oscar_buffer.h"
#include "errs.h"

#ifdef DEBUG
#include "mutex_inspector.h"
#include "oscar_buffer.h"

struct DB_id
{
    unsigned int container;

};
inline int operator < (const DB_id& a,const DB_id& b) {
    return a.container<b.container;
}
inline bool operator == (const DB_id& a,const DB_id& b) {
    return a.container==b.container;
}
inline bool operator != (const DB_id& a,const DB_id& b) {
    return a.container!=b.container;
}
inline out_oscar_buffer & operator<< (out_oscar_buffer& b,const DB_id& s) {
    b<<s.container;
    return b;
}
inline oscar_buffer & operator>> (oscar_buffer& b,  DB_id& s) {
    b>>s.container;
    return b;
}

/*namespace std{
_STLP_TEMPLATE_NULL struct hash< DB_id> {
  size_t operator()( DB_id n) const { return CONTAINER(n); }
  };
 }*/

#else
typedef unsigned int DB_id;
#endif
#endif
