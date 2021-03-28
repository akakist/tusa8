#ifndef CHAT_SERVER_MAIN_H__
#define CHAT_SERVER_MAIN_H__

//#define MAX_MSG_PER_SEC	20


//#define am_guest	0
//#define am_newuser	1
#define am_vip		5
#define am_killer	6
#define am_guard	7
#define am_spy		8
#define am_admin	9
#define am_sadmin	10
#define sub_levels 100

#define st_offline	0
#define st_offinvisible	1
#define st_invisible	2
#define st_online	3
#define st_ffc		4
#define st_away		5
#define st_busy		6
#define st_dnd		7
#define st_na		8


//#define f_bday		2
//#define f_admin		4
//#define f_ignor		8
//#define f_leader	16
//#define f_sadmin	32
//#define f_unreg		64

//#define MAX_NICK_SIZE	16
//#define MAX_LOGIN_SIZE	16


#define switch_cc_ret(aaa,bbb) if (cc->vurl.size()) {if (*cc->vurl.begin() == aaa) {cc->vurl.erase(cc->vurl.begin());bbb;return;}}
#define switch_cc(aaa,bbb) if (cc->vurl.size()) {if (*cc->vurl.begin() == aaa) {cc->vurl.erase(cc->vurl.begin());bbb;}}

#endif
