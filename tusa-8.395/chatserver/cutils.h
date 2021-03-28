#ifndef CHAT_UTILS_H__
#define CHAT_UTILS_H__
#include "user.h"
#include <time.h>
#include <vector>
#include <string>
extern char* SCROLL;



string ES(const string &s);
string ESB(const string &s);
string ESB_param(const string &s);

string MESL(const string &s);

string remove_tags(const string &s);

string remove_maty(const string &s);

bool check_maty(const string &ms);

string make_time(time_t t);

string replace_links(const string &m);

bool check_nick(const string &s);
bool check_login(const string &s);

string remove_spaces(const string &s);

string split_long(const string &s);

string str_nick(const string &n);

string nick_for_sort(const string &n);


string redirect();


string number2text(const int i);

string strtime(const time_t t);
string strtime_short(const time_t t);

string datetime2rus_short(const time_t t);
string datetime2rus(const time_t t);
string date2rus(const time_t t);

string date4file(const time_t t);
string date2num(const time_t t);
string datetime2num(const time_t t);

string get_rus_month(const int z);
string get_rus_month2(const int z);

void str_sort(vector<string> &v);

string checked(const string &s);
string checked(int z);


extern unsigned char nicks_char[256];

struct tm LOCALTIME(time_t timep);
map<string, vector< vector<string> > > load_st_file(const string &fname);
vector<string> load_l_file(const string &fname);

string get_f22(double dd);
string replace_for_js(const string& s);
string unescape_rus(string& s);

#endif
