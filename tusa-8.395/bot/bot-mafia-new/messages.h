#ifndef MESSAGES__H
#define MESSAGES__H

#include <string>
#include "misc.h"

using namespace std;

string _MSG(const string &n);
string _MSG(const string &n,unsigned int g,...);

void load_messages();

void do_rules(const c_command &cmd);

#endif

