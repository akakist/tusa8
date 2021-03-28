#ifndef IRC_SERVER_H__
#define IRC_SERVER_H__
#include <map>
#include <time.h>
#include "_mutex.h"
#include <vector>
#include "tcp_server.h"
#include "tcp_server_connection.h"
#include "PTR.h"
#include "tcp_dialog_server.h"
#include "chat_def.h"

class irc_command;



class irc_server:public tcp_linedialog_server
{
public:
    const char* getclassname()const {
        return "irc_server";
    }
    irc_server();
    void load_config(const std::string& pn,const std::string& prefix,std::map<std::string,std::string>&m);
    virtual void start(const std::string& pn,const std::string& prefix,std::map<std::string,std::string>&m);

    void *new_connection(const PTR_getter<c_socket>&s);
    std::string server_name() {
        return "ircserver";
    }

    void process_command(const string& cmd,const PTR_getter<c_socket>& __SD);
    void on_delete(const PTR_getter<c_socket>& __SD,const string& msg);

    void on_accept_dialog_socket(const PTR_getter<c_socket>&s) {}
    bool validate_peer_ip_address(const sockaddr_in & sa) {
        return true;
    };
    void delete_extra_user_id(const string &);
};


class irc_connection:public tcp_dialog_connection
{
public:

    irc_connection(const PTR_getter<c_socket> &sk, tcp_dialog_server* s)
        :tcp_dialog_connection(sk,s) {}
    ~irc_connection();
};

class irc_command
{
public:
    int code;
    string command;
    string data;
    irc_command() {}
};
string irc_gen_reply2(int code,const string &text,const string &irc_host,const string &cnick);
string irc_gen_reply2(int code,const string &val, const string &text,    const string &irc_host,const string &cnick);
string irc_gen_reply3(int code,const string &text,const string &irc_host);
string irc_gen_reply3(int code,const string &val, const string &text,    const string &irc_host);
void user_process_irc_login(const PTR_getter<chat_user> &__UU,irc_command &cmd);
void user_process_irc_admin(const PTR_getter<chat_user> &__UU,irc_command &cmd);
#endif
