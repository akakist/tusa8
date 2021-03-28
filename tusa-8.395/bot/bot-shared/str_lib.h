/*
core
(c) belialov.com
*/
#ifndef _STR_LIB_H_
#define _STR_LIB_H_
#include <string>
#include <vector>
#include <list>
#include <map>
#include <set>
#include <deque>
#include "errs.h"


std::string get_file_ext(const std::string &f);

std::string strprintf(const std::string &fmt,...);


std::string z_compress(const std::string &s);
std::string z_expand(const std::string &s);
unsigned char *buf_to_pc(unsigned char *s,int len);
unsigned char *buf_to_mac(unsigned char *s,int len);


struct rxfind_data{
	std::string str;
	int so;
	int eo;
};


char *strdup(char *s);
unsigned char *pp2cc(unsigned char *s255);
unsigned char *cc2pp(unsigned char *s255);

class PString
{
        public:
                unsigned char *buf;
                PString(const std::string &s);
                PString(char *s);
                ~PString();
};
unsigned char *p2c(unsigned char* s255);
#ifdef macintosh
unsigned char  *c2p(unsigned char *ss);
#endif
std::string strupper(const std::string &s);
std::string strlower(const std::string &s);
std::string str_replace(const std::string &s,const std::string &sr,const std::string &src);
std::string bin2hex(const std::string &in,int n);
std::string  hex2bin(const std::string &s);
std::vector <std::string>  splitTOK(const char *seps, const char *src);
std::list <std::string>  splitTOKl(const char *seps, const char *src);
std::vector <std::string>  splitTOK(const char *seps, const std::string& src);
std::list <std::string>  splitTOKl(const char *seps, const std::string& src);
std::set < std::string > splitTOKs(const char *seps, const std::string & src);

std::string itostring(int i);
std::string uitostring(unsigned int i);
std::string litostring(long int i);
std::string luitostring(long unsigned int i);
std::string lzstring(int i);
std::string date2mysql(time_t t);
time_t mysql2date(const std::string &t);

extern std::string split_char;
extern std::string basedir;
std::string Base64Encode(const std::string &str);
std::string Base64Decode(const std::string &str);
std::string MD5(const std::string &s);
std::string zip_buf(const std::string &str);
std::string unzip_buf(const std::string &str);
unsigned char *to_mac(unsigned char *s);
const char *kks_format_time(char *in);
std::string to_mac_enc(const std::string &s);
std::string from_mac_dec(const std::string &s);

void mk_path(const std::string &path);
int atoi(const std::string&);

extern char l_table[];
extern char u_table[];
extern char* cypher_names[];
extern int cypher_names_size;
void hex_dump(char *data, long size);

std::string sql_in(const std::vector < std::string > &res);
std::string join(const std::string &pattern, const std::vector<std::string> & arr);
std::string join(const char* pattern, const std::vector<std::string> & arr);
std::string join(const char* pattern, const std::set<std::string> & arr);
std::string join(const char *pattern, const std::deque < std::string > &st);
std::string rjoin(const char *pattern, const std::deque < std::string > &st);

void find(std::vector < rxfind_data > &res, const char *regexp, const char *buffer);

std::string replace_vals(std::map<std::string,std::string> &p, const std::string &src);
std::string replace_vals(const std::string &src);
std::string replace_re_vals(std::map<std::string,std::string> &p, const std::string &src);

std::string mysql_datetime2date(const std::string & s);
std::string mysql_datetime2time(const std::string & s);
std::string remove_commas(const std::string& s);
std::string CTIME(time_t p);

inline std::string & operator<<(std::string& l, const std::string r)
{
    l+=r;
    return l;
}
inline std::string & operator<<(std::string& l, const char *r)
{
    l+=r;
    return l;
}
std::string fmt_d(const char *s,double d);
std::string sockaddr_to_string(const unsigned int pa);
std::map<std::string,std::string> load_m_file(const std::string &fn);

#ifdef WIN32
#define snprintf _snprintf
#endif
#endif
