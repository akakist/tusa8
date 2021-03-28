#ifndef UTILS__H
#define UTILS__H

#include <string>
#include <vector>
#include <map>

using namespace std;

string strupper(const string &s);
string strlower(const string &s);


string str_nick(const string &n);


extern char l_table[];
extern char u_table[];
extern unsigned char nicks_char[256];

bool strex(const string &str,const string &patt);

vector <string>  splitTOK(const char *seps, const string& src);
string itostring(int i);
string ftostring(double i);
string luitostring(long unsigned int i);
int atoi(const string&);

string join(const string &pattern, const vector<string> & arr);
string join(const char* pattern, const vector<string> & arr);
map<string,string> parse_strmap(const string &s);
#include "oscar_buffer.h"
class user_nick
{
public:
    string name;
    unsigned int id;
//	user_nick();
	
//	user_nick(const string &n);
//	user_nick(const string &n,unsigned int i);
};

struct user_status
{
	string name;
	int pic;
	int id;
	bool disable_invite;
	bool uniq;
};

class user_state
{

	public:
	
	enum
	{
		UNDEF=0xffff0000
	};
		unsigned int channel;
		user_status status;
		user_nick nick;
//		bool contact_mode; // true if userlist contains contacts only
//		user_state():nick("undef",UNDEF){channel=UNDEF;status.id=UNDEF;nick.id=UNDEF;/*contact_mode=false;*/}
};
inline oscar_buffer & operator>>(oscar_buffer&b, user_nick &n)
{
	DWORD_LE id;
	oscar_buffer::L_STR ni;
	b>>id>>ni;
	n.id=id.obj;
	n.name=ni.container;
	return b;
}

inline oscar_buffer & operator>>(oscar_buffer&b, user_status &n)
{
	oscar_buffer::L_STR name;
	DWORD_LE pic,id;
	__BYTE di,uniq;
	b>>name>>pic>>id>>di>>uniq;
	n.name=name.container;
	n.pic=pic.obj;
	n.id=id.obj;
	n.disable_invite=di.obj;
	n.uniq=uniq.obj;

	return b;
}

inline oscar_buffer & operator>>(oscar_buffer&b, user_state& n)
{
	DWORD_LE c;
	b>>c;
	n.channel=c.obj;
	b>>n.status>>n.nick;
	return b;
}
inline string get_f22(double dd)
{
	char s[100];
	snprintf(s, sizeof(s) - 1, "%2.2f", dd);
	return s;

}


#endif

