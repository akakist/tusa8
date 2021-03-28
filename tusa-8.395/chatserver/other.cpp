
#include "stdafx.h"
#ifdef _DEBUG
#define New DEBUG_NEW
#endif

#include "_mutex.h"
#include <list>
#include "mutex_inspector.h"
#include <time.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <map>
#include "_mutex.h"

#include "cutils.h"
#include "str_lib.h"
#include "web_server.h"
#include "message.h"
#include "chat_def.h"
#include "copy_file.h"
#include "file_set.h"

#include "app_con.h"

#include "tcp_stats.h"
#include "chat_config.h"
#include "user_credits.h"
#include <algorithm>

#include <openssl/crypto.h>
#include <openssl/x509.h>
#include <openssl/pem.h>
#include <openssl/ssl.h>
#include <openssl/err.h>
#ifndef WIN32
#include <netdb.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#endif
#include "tcp_oscar_dialog_server.h"

#include "oscar_buffer.h"

#include "chat_globals.h"

#include "app_def.h"
#ifdef __WITH_LICENSE
#include "license.h"
#endif

m_var<time_t> inf_start_time(time(NULL));
time_t total_wtime();

out_oscar_buffer & operator<<(out_oscar_buffer&b, const user_state& n);
oscar_buffer & operator>>(oscar_buffer&b, user_state& n);
string dump_mutex_inspectors_www();
#define CHK_NULL(x) if ((x)==NULL) return -1;
#define CHK_ERR(err,s) if ((err)==-1) { return -1; }
#define CHK_SSL(err) if ((err)==-1) { return -1; }
void cpu_state();
//map<string,string> calc_statistic();
//map<time_t,unsigned int> get_users$counts(time_t t, time_t rt);

//extern M_MAP<string,string> chat_stat;		//URL /stat top 50 table
/*map<time_t,unsigned int> get_users$counts(time_t t, time_t rt)
{
    MUTEX_INSPECTOR;
    map<time_t,unsigned int> u;
    vector< vector<string> > v;

    {
        st_dbh db;

        QUERY q=(QUERY)"select UNIX_TIMESTAMP(dt),cnt from ? where dt>='?' and dt<='?'"<<chat_config.users_count_table_name.get()<<date2mysql(t)<<date2mysql(rt);
        string s=q.prepare();
        v=db.dbh->exec(s);


    }
    for (unsigned int i=0;i<v.size();i++){
        if (v[i].size()!=2) throw cError("select failed. size different"+_DMI());
        time_t st=atoi(v[i][0]);
        u[st]=atoi(v[i][1]);

    }
    return u;
}
*/
string thread_info_html(const map<pthread_t, string> &add_map);
extern M_MAP<pthread_t,string> pthread_2_url_map;

string info_value(char *format,
#ifndef WIN32
                  long long v
#else
                  __int64 v
#endif

                  , time_t dt)
{
    char s[1000];
    double p1=v;
    p1/=dt;
    snprintf(s, sizeof(s) - 1, format, v,p1);
    return s;
}


void do_adminfo(app_connection * cc, const string & title)
{


    time_t dt = time(NULL) - inf_start_time.get();
    if (dt == 0) {

        cc->html_out = "<h1>dt=0! stat unavilable.<br>";
        return;
    }
    {
        cc->html_out = "<link rel=stylesheet type=text/css href=/css/main.css>";
        cc->html_out += "<title>" + title + "</title><body>";
        cc->html_out += "<h1>" + title + "</h1><p>";


        cc->html_out += "Started: " + CTIME(inf_start_time) + "<br>\n";
        char s[200];
        snprintf(s, sizeof(s) - 1, "Uptime: %d days %02d:%02d:%02d hours (%d sec):<br>\n", (dt / 24) / 3600, dt / 3600, (dt % 3600) / 60,
                 dt % 60, dt);
        cc->html_out += s;
    }

    cc->html_out+=info_value("Total processed connections: %lld (%2.2f sec)<br>\n",tcp_total_accepted_connections.get(),dt);
    char s[200];
    cc->html_out+=info_value("Total data sent: %llu (%2.2f sec)<br>\n",tcp_total_send.get(),dt);
    cc->html_out+=info_value("Total data recv: %llu (%2.2f sec)<br>\n",tcp_total_recv.get(),dt);

    string mysock;
    {
        PTR_getter<c_socket > s=cc->socket;
        {
            PGLW( c_socket , ss,s);
            if (ss) {
                mysock = itostring(CONTAINER(ss->get_socket_fd()));
            }
        }


    }
    cc->html_out += "Socket: " + mysock + "<br>";
    cc->html_out += "IP: " + cc->peer_ipaddress + "<br>";


    cc->html_out += "Currently " + itostring(local_users.size$()) + " users<br>\n";

#ifdef __linux__
    {
        double twt = total_wtime();
        twt /= 100;
        snprintf(s, sizeof(s) - 1, "Total CPU time %2.2f ( %2.2f\% )<p>\n", twt, ((twt / dt) * 100));
    }
#endif
    {
        cc->html_out += s;



        cc->html_out += "<h2>Params</h2>";
        {
            for (map< string, string >::const_iterator i = cc->params.begin(); i != cc->params.end(); i++) {
                cc->html_out += i->first + ": " + i->second + "<br>";
            }
        }
        {
            for (map< string, string >::const_iterator i = cc->headers.begin(); i != cc->headers.end(); i++) {
                cc->html_out += i->first + ": " + i->second + "<br>";
            }
        }

        cc->html_out+=thread_info_html(pthread_2_url_map.get_container());

    }

}


