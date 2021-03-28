#ifndef SID_TT_H
#define SID_TT_H
#ifndef WIN32
#include <netinet/in.h>
#endif
#include <string.h>
struct sid_t
{
    enum {
        VALID_USERKEY=8, /* user is exists in usermap and __user->map_key==user_map_key */
        VALID_FULL=0xff /*all setted*/
    };
    unsigned long user_map_key;
    unsigned long user_map_key1;
    sid_t()
    {
        user_map_key =user_map_key1 = 0;
    }

};
#endif
