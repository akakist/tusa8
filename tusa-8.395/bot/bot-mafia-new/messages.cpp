#include "mafia.h"
#include <string>
#include "command.h"
#include "misc.h"
#include "game.h"
#include <stdio.h>
#include <unistd.h>
#include "errs.h"
#include <vector>
#include <map>
#include "utils.h"
#include "mutex_inspector.h"
#include <stdarg.h>

map<string, vector<string> > msgs;

string _MSG(const string &n,unsigned int g,...)
{MUTEX_INSPECTOR;
	string k=n;
	if(g){
		k+=itostring(g);
		if(msgs.find(k)==msgs.end() || !msgs[k].size()) k=n;
	}
	if(msgs.find(k)==msgs.end() || !msgs[k].size()){
		logErr("---Error: messages for key %s not found!",k.c_str());
		return "Error. See log";
	}

	string v;
	if(msgs[k].size()==1){
		v=msgs[k][0];
	}else{
		unsigned int z=rand() % msgs[k].size();
		if(z>=msgs[k].size()) z=0;
		v=msgs[k][z];
	}
//	printf("'%s'\n",v.c_str());
	va_list argptr;
	va_start(argptr, g);
	char cc[4000];
	vsnprintf((char*) &cc,sizeof(cc)-10,v.c_str(),argptr);
	string r(cc);
	return r;
}

string _MSG(const string &n)
{MUTEX_INSPECTOR;
	return _MSG(n,0);
}



void load_messages()
{MUTEX_INSPECTOR;
	msgs.clear();
	FILE *f = fopen("./MAFIA.MESSAGES", "r");
	if(!f){
		logErr("---Error: mafia.messages not found!");
		exit(1);
	}
	string data;
	while (!feof(f)){
		char s[1000];
		int z=fread((void*) &s, 1, 990,f);
		s[z]=0;
		data+=s;
	}
	vector<string> v=splitTOK("\r\n",data);
	for(unsigned int i=0;i<v.size();i++){
		string s=v[i];
		if(s.substr(0,1)=="#" || s.size()<=1) continue;
		int pz=s.find("=",0);
		if(pz==1){
			logErr("---Error in MAFIA.MESSAGES on line %d - '=' not found",i);
			continue;
		}
		string key=s.substr(0,pz);
		string val=s.substr(pz+1,s.size()-pz-1);
//		printf("'%s' - '%s'\n",key.c_str(),val.c_str());
		msgs[key].push_back(val);
	}
}


void do_help(const c_command &cmd)
{
	privmsg(cmd.fromnick,_MSG("help"));
}

void do_rules(const c_command &cmd)
{
	privmsg(cmd.fromnick,_MSG("rules"));
}

