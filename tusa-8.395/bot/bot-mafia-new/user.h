#ifndef USER__H
#define USER__H

#include <map>
#include <string>
#include "command.h"


#define ROLE_MAF	2
#define ROLE_KAT	3
#define ROLE_MAN	4
#define ROLE_DOC	5
#define ROLE_BOM	6
#define ROLE_VAL	7
#define ROLE_MIR	10

class c_user{
public:
	c_user();
	string ip;
	string nick;
	string login;
	int gender;
	unsigned int role;
	unsigned int score;
	unsigned int last_score;
	unsigned int id;
	bool accepted;
	bool main_maf;
	unsigned int maf_n;
	bool order_done;
	int order;
	bool order_type;		// false --- check, true --- kill
	bool marked_for_delete;
	bool killed;
	bool devoiced;
	bool doced;
	int parent;
	bool valed;
	bool voted;
	string phrase;
	bool left;
	bool chnick;
	bool win;
	bool need_update;
	int doc_self;
};


extern map<unsigned int,c_user*> gamers_by_id;
extern map<string,c_user*> gamers_by_nick;

string get_role_name(int role);
string get_role_name2(int role);
string get_role_name_cnt(int role,int cnt);

string get_role_shortname(int role);


extern unsigned int register_counter;

void register_user(const string &,const string &,unsigned int gender, const string& ip);
void unregister_user(const string &);

void clear_users();

int get_active_users_count();


#endif

