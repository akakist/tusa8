#ifndef APP_CON__H
#define APP_CON__H
#include "web_connection.h"
#include "PTR.h"
#include "c_conn.h"
#include <string>
#include <map>
#include "user.h"
class app_connection: public web_connection
{
public:
    void process_web();
    app_connection(app_server *p,const PTR_getter<c_socket>&s);
    app_server *server;
    void redirect(const string &url);
    string redirect();
    deque<string>vurl;
};


#endif
