#ifndef WEBTOOLS___H
#define WEBTOOLS___H
#include "m_var.h"
#include <string>
#include <map>
using namespace std;
extern m_var<string> SERVERNAME;


string make_http_header();
string make_http_header(const int code);
string make_http_header(map<string,string> &pr);
string make_http_header(const int code,map<string,string> &pr);


struct hstring;
string make_select(const string &n,map<string,string> &p,const string &def,const string &add);
string make_select(const string &n,map<string,string> &p,const string &def);
string make_select(const string &n,map<string,string> &p);
string make_select(const string &n,vector<pair<string,string> > &p,const string &def,const string &add);
string make_select(const string &n,vector<pair<string,string> > &p,const string &def);
string make_select(const string &n,vector<pair<string,string> > &p);

string get_name_of_http_code(int code);

string checked(const bool b);

string RVfiles(map<string,string>&m,const char *filekey);
string RVfiles(const char *filekey);
#endif
