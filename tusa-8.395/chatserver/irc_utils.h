#ifndef IRC___USER_HHH
#define IRC___USER_HHH
#include "_mutex.h"
#include <time.h>
#include <list>
#include "irc_server.h"
#include "levels.h"
#include <map>
#include "user.h"

void irc_send_notice(const PTR_getter<chat_user>&__U,const string &msg,const string &from);
void irc_send_notice(const user_pair& __U,const string &msg,const string &from);
void irc_send_private_message(const PTR_getter<chat_user>&__U,const string &msg,const string &from);
void irc_send_private_message(const user_pair&__U,const string &msg,const string &from);
void irc_send_private_message(const user_pair&__U,const string &msg,const string &from);
void irc_send_reply(const PTR_getter<chat_user>&__U,int code,const string &text);
void irc_send_reply(const PTR_getter<chat_user>&__U,int code,const string &val,const string &text);
void irc_send_reply(const user_pair&__U,int code,const string &text);
void irc_send_reply(const user_pair&__U,int code,const string &val,const string &text);

void irc_send_welcome(const user_pair&__U);

string irc_gen_reply2(int code,const string &text,const string &irc_host,const string& cnick);
string irc_gen_reply2(int code,const string &val,const string &text,const string &irc_host,const string &cnick);
string irc_gen_reply(const user_pair&__U,int code,const string &text);
string irc_gen_reply(const user_pair&__U,int code,const string &text);
string irc_gen_reply(const user_pair&__U,int code,const string &val,const string &text);
string irc_gen_reply(const user_pair&__U,int code,const string &val,const string &text);
string s_irc_send_action_to_channel(const string &msg,const string &from, const string& chan);
string s_irc_send_message_to_channel(const string &msg,const string &from,const string& channel);
string s_irc_send_system_message_to_channel(const string &msg, const string& channel);
string irc_NICK(const PTR_getter<chat_user> &__UU);

#endif
