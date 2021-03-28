#ifndef _STR_LIB_H_
#define _STR_LIB_H_
#include <string>
#include <vector>
#include <list>
#include <map>
#include <set>
#include <deque>
#ifndef WIN32
#include <map>
#endif
#include "errs.h"
#include <m_var.h>
#include "Double.h"

struct Double;



std::string get_file_ext(const std::string &f);

std::string strprintf(const std::string &fmt,...);


std::string z_compress(const std::string &s);
std::string z_expand(const std::string &s);
unsigned char *buf_to_pc(unsigned char *s,int len);
unsigned char *buf_to_mac(unsigned char *s,int len);


struct rxfind_data {
    std::string str;
    int so;
    int eo;
};


char *strdup(char *s);

std::string strupper(const std::string &s);
std::string strlower(const std::string &s);
std::string str_replace(const std::string &s,const std::string &sr,const std::string &src);
std::string bin2hex(const std::string &in,int n=1);
std::string  hex2bin(const std::string &s);
std::vector <std::string>  splitTOK(const char *seps, const char *src);
std::list <std::string>  splitTOKl(const char *seps, const char *src);
std::deque <std::string>  splitTOKd(const char *seps, const char *src);
std::vector <std::string>  splitTOK(const char *seps, const std::string& src);
std::list <std::string>  splitTOKl(const char *seps, const std::string& src);
std::set < std::string> splitTOKs(const char *seps, const std::string & src);
std::deque <std::string>  splitTOKd(const char *seps, const std::string& src);

std::string itostring(int i);
std::string itostring(size_t i);
std::string itostring(unsigned int i);
std::string itostring(unsigned long int i);
std::string itostring(time_t i);
std::string itostring(size_t i);
std::string ftostring(double i);
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

extern unsigned char l_table[];
extern unsigned char u_table[];
extern char* cypher_names[];
extern int cypher_names_size;
void hex_dump(char *data, long size);

std::string sql_in(const std::vector < std::string> &res);
#ifdef WIN32
__declspec (dllexport)
#endif
std::string join(const std::string &pattern, const std::vector<std::string> & arr);
#ifdef WIN32
__declspec (dllexport)
#endif
std::string join(const char* pattern, const std::vector<std::string> & arr);
#ifdef WIN32
__declspec (dllexport)
#endif
std::string join(const char* pattern, const std::set<std::string> & arr);
#ifdef WIN32
__declspec (dllexport)
#endif
std::string join(const char *pattern, const std::deque < std::string> &st);
#ifdef WIN32
__declspec (dllexport)
#endif
std::string rjoin(const char *pattern, const std::deque < std::string> &st);

void find(std::vector < rxfind_data > &res, const char *regexp, const char *buffer);

std::string replace_vals(std::map<std::string,std::string> &p, const std::string &src);
std::string replace_vals(const std::string &src);

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
std::string fmt_d(const char *s,Double d);
std::string sockaddr_to_string(const unsigned int pa);
#ifdef WIN32
__declspec (dllexport)
#endif
std::map<std::string,std::string> load_m_file(const std::string &fn);
std::map<std::string,std::string> load_m_file_no$(const std::string &fn);
std::map<std::string,std::string> load_m_file_noerr_no$(const std::string &fn);


std::map<int,std::string> load_m_is_file(const std::string &fn);
void set_cfg_i(std::map<std::string,std::string> &m,const std::string& name, const std::string& pn,m_var<unsigned int> &v, unsigned int defv);
void set_cfg_i(std::map<std::string,std::string> &m,const std::string& name, const std::string& pn,m_var<int> &v, int defv);
void set_cfg_i(std::map<std::string,std::string> &m,const std::string& name, const std::string& pn,unsigned int &v, unsigned int defv);
void set_cfg_i(std::map<std::string,std::string> &m,const std::string& name, const std::string& pn,int &v, int defv);
void set_cfg_ishort(std::map<std::string,std::string> &m,const std::string& name, const std::string& pn,m_var<unsigned short> &v, unsigned short defv);
void set_cfg_str(std::map<std::string,std::string> &m,const std::string& name, const std::string& pn,m_var<std::string> &v, const std::string& defv);
void set_cfg_f(std::map<std::string,std::string> &m,const std::string& name, const std::string& pn,m_var<double> &v, double defv);
void set_cfg_f(std::map<std::string,std::string> &m,const std::string& name, const std::string& pn,m_var<Double> &v, Double defv);
void set_cfg_b(std::map<std::string,std::string> &m,const std::string& name, const std::string& pn,m_var<bool> &v, bool defv);
void print_rest_cfg(std::map<std::string,std::string> &m,const std::string& pn);
std::string get_cfg_str(std::map<std::string,std::string> &m,const std::string& name, const std::string&pn,const std::string& defv);


std::map<std::string,std::string> load_m_file_noerr(const std::string &fn);
std::string trim(const std::string &s);

string repl_percent(const string & s);


#ifdef WIN32
#define SPLITTER "\\"
#else
#define SPLITTER "/"
#endif


#ifdef WIN32
#define snprintf _snprintf
#endif
#endif
