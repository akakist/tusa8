#ifndef WEB_CONN_H__
#define WEB_CONN_H__
#include <sys/types.h>
#ifndef WIN32
#include <sys/socket.h>
#include <netinet/in.h>
#endif
#include <vector>
#include <map>
#include <map>
#include <stdio.h>
#include "sid_t.h"
#include "tcp_server_connection.h"
#include <set>
#include "html_out.h"
using namespace std;
extern m_var<time_t> start_date;
extern m_var<string> start_date_string;
template <class T>
class PTR_getter;

class web_server;

class web_connection: public tcp_server_connection
{
private:
    void split_params(const string &s);
public:
    virtual void process_web()=0;

    void process_tcp();
    web_connection(const web_server* ws,const PTR_getter<c_socket> &s);
    web_connection(const web_connection* s);
    virtual ~web_connection();
    void send_other(const string &fn);

    void redirect(const string &url);



    string rvfiles(map<string,string> &m,const string &s);
    string rvfiles(map<string,string> &m,const char *s);
    string rvfiles(const string &s);
    string rvfiles(const char* s);

    map<string,string> params;
    string original_params; // original params without splitting
    map<string,vector<string> > v_params;/*for multiple select*/
    bool process_v_params;/* flag is set -> v_params will filled */
    web_server *server_ref;
    string url;
    string html_out;
    map<string,string> http_header_out;
    int http_code;
    map<string,string> in_cookies;
    map<string,string> out_cookies;
    string html_charset;
    string html_content_type;

    bool allow_build_response;

    string peer_ipaddress;
    string build_html_response();

    ////////////////
    string METHOD;
    string POST_CONTENT;
    map<string,string> headers;
    string original_url;
};
int send_other_from_disk(web_connection * cc, const string & fn,bool use_rv);
pair<string,string> get_other_from_disk_ext(web_connection * cc, const string & fn,bool use_rv, const string& exten, bool &result /*true success*/);

string as_string(const hstring& h,  web_connection * cc);

#endif
