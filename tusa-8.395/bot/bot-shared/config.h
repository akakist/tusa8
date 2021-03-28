#ifndef CONFIG__H
#define CONFIG__H

#include <string>
#include <map>

//#define CONFIG_FILE	"bot.conf"

using namespace std;

extern map<string,string> params;
extern map<string,bool> access_users;

void load_config(const char*);

#endif

