#include "config.h"
#include <string>
#include <stdio.h>
#include <unistd.h>
#include "utils.h"
#include "errs.h"
#include "mutex_inspector.h"

map<string,string> params;
map<string,bool> access_users;



void load_conf(const char *CONFIG_FILE)
{MUTEX_INSPECTOR;
	FILE *f=fopen(CONFIG_FILE,"r");
	if(!f){
	    logErr("Error during reading config file %s",CONFIG_FILE);
	    throw cError("Error during reading config file "+string(CONFIG_FILE));
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
			logErr("---Error in %s on line %d - '=' not found",CONFIG_FILE,i);
			continue;
		}
		string key=s.substr(0,pz);
		string val=s.substr(pz+1,s.size()-pz-1);
		params[strupper(key)]=val;
	}
	vector<string> aa=splitTOK(", ",params["access_list"]);
	access_users.clear();
	for(unsigned int i=0;i<aa.size();i++){
		access_users[aa[i]]=true;
	}
}

void load_config(const char *CONFIG_FILE)
{
	params.clear();
	load_conf(CONFIG_FILE);
	load_conf("common.conf");
}
