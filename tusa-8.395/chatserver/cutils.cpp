#include "stdafx.h"
#ifdef _DEBUG
#define New DEBUG_NEW
#endif

#include "_mutex.h"
#include <list>
#include "mutex_inspector.h"
#include <map>

#include "cutils.h"
#include <stdio.h>
#include "web_server.h"
#include "user.h"
#include <time.h>
#include "str_lib.h"
#include "_mutex.h"
#include "st_stuff.h"
#include <algorithm>
#include "file_set.h"
#include "levels.h"
//#include "chat_colors.h"
#include "chat_def.h"
#include "server_config.h"
#include "chat_config.h"

#include "chat_globals.h"
#define MAX_WORD_SIZE	25
struct tm LOCALTIME(time_t timep);
#include "mysql_extens.h"
char* SCROLL="\n<script>parent.DS();</script>\n";


string MESL(const string &s)
{
    string r=MES(s);
    return str_replace("_","\\_",r);
}


string split_long(const string &s)
{
    vector<string> v=splitTOK(" ",s.c_str());
    string r;
    for (unsigned int i=0; i<v.size(); i++) {
        if (v[i].size()>MAX_WORD_SIZE && v[i].find("http://")==-1 && v[i].find("ftp://")==-1) {
            for (unsigned int j=0; j<v[i].size(); j+=MAX_WORD_SIZE) {
                r+=v[i].substr(j,MAX_WORD_SIZE)+" ";
            }
        } else {
            r+=v[i]+" ";
        }
    }
    return r;
}

string remove_maty(const string &s)
{
    vector<rxfind_data> r;
    string src=" "+s;
    string ums=" "+str_nick(src);
    find(r,chat_config.bad_words.get().c_str(),ums.c_str());
    if (r.size()==0) {
        return s;
    }
    int lpz=0;
    string a;
    for (unsigned int i=0; i<r.size(); i++) {
        int pz=r[i].so-lpz-1;
        if (pz<0) pz=0;
        if (lpz<0) lpz=0;
        a+=src.substr(lpz,pz);
        string rp;
        unsigned char c='#';
        for (unsigned int z=0; z<r[i].str.size(); z++) {
            if (z==0 && r[i].str[0]==' ') {
                rp+=" ";
            }
            else {
                rp+=c;
                c++;
                if (c>37) c='#';
            }
        }
        a+=rp;
        lpz=r[i].eo-1;
    }
    a+=src.substr(lpz,src.size()-lpz);
    return a.substr(1,a.size()-1);
}

bool check_maty(const string &ms)
{
    vector<rxfind_data> r;
    string ums=" "+str_nick(ms);
    find(r,chat_config.bad_words.get().c_str(),ums.c_str());
    if (r.size()>0) {
        return false;
    } else {
        return true;
    }
}


string number2text(const int i)
{
    double d=i;
    if (i<KB) {
        return itostring(i)+" b";
    }
    if (i<MB) {
        return fmt_d("%3.1f Kb",d/KB);
    }
    return fmt_d("%3.1f Mb",d/(MB));
}

string remove_tags(const string &s)
{
    string r=s;
    map<string,string> m;
    m["&"]="&amp;";
    m["<"]="&lt;";
    m[">"]="&gt;";
    r=replace_vals(m,r);
    return r;
}

string ES(const string &s)
{
    string e=s;
    map<string,string> m;
    m["&lt;"]="<";
    m["&gt;"]=">";
    m["\'"]="\\\'";
    e=replace_vals(m,e);
    return e;
}


string remove_spaces(const string &s)
{
    string r;
    for (unsigned int i=0; i<s.size(); i++) {
        if (s[i]!=' ')r+=s[i];
    }
    return r;
}

string make_time(time_t t)
{
    char s[500];
    snprintf(s,sizeof(s),"%02d:%02d:%02d",t/3600,(t%3600)/60,t%60);
    return s;
}



string nick_for_sort(const string &n)
{
    string r=n;
    int z=atoi(n);
    if (z<10 && r.size()>=2) {
        if (r[0]=='0') {
            r="??"+r.substr(1,r.size()-1);
        }
        if (r[0]=='3') {
            r="??"+r.substr(1,r.size()-1);
        }
    }
    for (unsigned int i=0; i<r.size(); i++) {
        unsigned char c=r[i];
        if (c!='`' && c!='_' && c!='-' && c!='=' && c!='$' && c!='*' && c!='+') {
            r=r.substr(i,r.size()-i);
            break;
        }
    }
    if (z<10 && r.size()>=2) {
        if (r[0]=='0') {
            r="??"+r.substr(1,r.size()-1);
        }
        if (r[0]=='3') {
            r="??"+r.substr(1,r.size()-1);
        }
    }
    return r;
}

bool check_login(const string &s)
{
    char *b="0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ@_-=$*+\0x0";
    for (unsigned int i=0; i<s.size(); i++) {
        char c=s[i];
        bool found=false;
        for (unsigned int j=0; j<strlen(b); j++) {
            if (b[j]==c) {
                found=true;
            }
        }
        if (!found) {
            return false;
        }
    }
    return true;
}

bool check_nick(const string &s)
{
    char *b="`????0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ_-$*????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????\0x0";
    char *bbegin="`_-=*";
    size_t blen=strlen(b);
    for (size_t i=0; i<s.size(); i++) {
        char c=s[i];
        bool found=false;
        for ( int j=0; j<blen; j++) {
            if (b[j]==c) {
                found=true;
            }
        }
        if (!found) {
            return false;
        }
    }
#ifndef REAL
    if (s.size())
    {
        char b=s[0];
        char e=s[s.size()-1];
        if (b=='4') return false;
        if (b=='1') return false;
        for (char *p=bbegin; *p; p++)
        {
            if (b==*p)return false;
            if (e==*p)return false;
        }


    }
#endif

    return true;
}




string strtime_short(const time_t t)
{
    struct tm q=LOCALTIME(t);
    char s[200];
    snprintf(s,sizeof(s),"%02d:%02d",q.tm_hour,q.tm_min);

    return s;
}

string strtime(const time_t t)
{
    struct tm q=LOCALTIME(t);

    char s[200];
    snprintf(s,sizeof(s),"%02d:%02d:%02d",q.tm_hour,q.tm_min,q.tm_sec);
    return s;
}

string get_rus_month(const int z)
{
    string r;
    switch (z)
    {
    case 1:
        r=_MSG("mon1");
        break;
    case 2:
        r=_MSG("mon2");
        break;
    case 3:
        r=_MSG("mon3");
        break;
    case 4:
        r=_MSG("mon4");
        break;
    case 5:
        r=_MSG("mon5");
        break;
    case 6:
        r=_MSG("mon6");
        break;
    case 7:
        r=_MSG("mon7");
        break;
    case 8:
        r=_MSG("mon8");
        break;
    case 9:
        r=_MSG("mon9");
        break;
    case 10:
        r=_MSG("mon10");
        break;
    case 11:
        r=_MSG("mon11");
        break;
    case 12:
        r=_MSG("mon12");
        break;
    }
    return r;
}



string date2rus(const time_t t)
{
    struct tm q=LOCALTIME(t);
    string r;
    r=get_rus_month(q.tm_mon+1);
    return
        itostring(q.tm_mday)+" "+r+", "+itostring(q.tm_year+1900)+" "+"????????";
}

string datetime2rus_short(const time_t t)
{
    struct tm q=LOCALTIME(t);
    return itostring(q.tm_mday)+" "+get_rus_month(q.tm_mon+1)+", "+strtime_short(t);
}

string datetime2rus(const time_t t)
{
    return date2rus(t)+", "+strtime(t);
}

string date4file(const time_t t)
{
    struct tm q=LOCALTIME(t);
    char s[200];
    snprintf(s,sizeof(s),"%d-%02d-%02d",q.tm_year+1900,q.tm_mon+1,q.tm_mday);
    return s;
}

static int strsrt(const string &a,const string &b)
{
    return strlower(a)<strlower(b);
}

void str_sort(vector<string> &v)
{
    sort(v.begin(),v.end(),strsrt);
}


string checked(const string &s)
{
    if (strupper(s)=="ON") {
        return "checked";
    }
    return "";
}

string checked(int z)
{
    if (z==0) {
        return "";
    }
    return "checked";
}


string replace_links(const string &m)
{
    string res=m;
    {   //replace http:// & ftp://
        vector<rxfind_data> r;
        char *re=
            "http://[0-9,a-z,A-Z,??-??,_,/,.,~,;,?,=,:,&,-]+|ftp://[0-9,a-z,A-Z_,-,/,.,@,:]+";
        find(r,re,res.c_str());
        if (r.size()>0) {
            string a;
            int lpz=0;
            for (unsigned int i=0; i<r.size(); i++) {
                if (r[i].str.find("\"")==-1 && r[i].str.find("'")==-1)
                {
                    a+=res.substr(lpz,r[i].so-lpz);
                    a+="<a href=\""+r[i].str+"\" target=_blank>"+r[i].str+"</a>";
                }
                lpz=r[i].eo;
            }
            a+=res.substr(lpz,res.size()-lpz);
            res=a;
        }
    }
    return res;
}



string get_f22(double dd)
{
    char s[111];
    snprintf(s, sizeof(s) - 1, "%2.2f", dd);
    return s;

}



string replace_for_js(const string& s)
{
    string out;
    for (unsigned i=0; i<s.size(); i++)
    {
        switch (s[i])
        {
        case '&':
            out+="&#038";
            break;
        case '$':
            out+="&#036";
            break;
        case '\'':
            out+="&#039";
            break;
        case '\"':
            out+="&#034";
            break;
        case '\\':
            out+="&#092";
            break;
        default:
            out+=s[i];
        }
    }

    return out;
}

/*char *enc_repl[]=
{
	"%u0401","??","%u0451","??","%u0410","??","%u0411","??","%u0412","??","%u0413","??","%u0414","??","%u0415","??","%u0416","??","%u0417","??","%u0418","??",
		"%u0419","??","%u041A","??","%u041B","??","%u041C","??","%u041D","??","%u041E","??","%u041F","??","%u0420","??","%u0421","??","%u0422","??","%u0423","??",
		"%u0424","??","%u0425","??","%u0426","??","%u0427","??","%u0428","??","%u0429","??","%u042A","??","%u042B","??","%u042C","??","%u042D","??","%u042E","??",
		"%u042F","??","%u0430","??","%u0431","??","%u0432","??","%u0433","??","%u0434","??","%u0435","??","%u0436","??","%u0437","??","%u0438","??","%u0439","??",
		"%u043A","??","%u043B","??","%u043C","??","%u043D","??","%u043E","??","%u043F","??","%u0440","??","%u0441","??","%u0442","??","%u0443","??","%u0444","??",
		"%u0445","??","%u0446","??","%u0447","??","%u0448","??","%u0449","??","%u044A","??","%u044B","??","%u044C","??","%u044D","??","%u044E","??","%u044F","??",
		0,0
};*/
_mutex enc_repl_mx;
map<string,string> enc_repl;
bool enc_repl_inited=false;
void enc_repl_init()
{
    if (!enc_repl_inited)
    {
        enc_repl_inited=true;
        enc_repl["%u0401"]="??";
        enc_repl["%u0451"]="??";
        enc_repl["%u0410"]="??";
        enc_repl["%u0411"]="??";
        enc_repl["%u0412"]="??";
        enc_repl["%u0413"]="??";
        enc_repl["%u0414"]="??";
        enc_repl["%u0415"]="??";
        enc_repl["%u0416"]="??";
        enc_repl["%u0417"]="??";
        enc_repl["%u0418"]="??";
        enc_repl["%u0419"]="??";

        enc_repl["%u041A"]="??";
        enc_repl["%u041B"]="??";
        enc_repl["%u041C"]="??";
        enc_repl["%u041D"]="??";
        enc_repl["%u041E"]="??";
        enc_repl["%u041F"]="??";
        enc_repl["%u0420"]="??";
        enc_repl["%u0421"]="??";
        enc_repl["%u0422"]="??";
        enc_repl["%u0423"]="??";
        enc_repl["%u0424"]="??";
        enc_repl["%u0425"]="??";
        enc_repl["%u0426"]="??";
        enc_repl["%u0427"]="??";
        enc_repl["%u0428"]="??";
        enc_repl["%u0429"]="??";
        enc_repl["%u042A"]="??";
        enc_repl["%u042B"]="??";
        enc_repl["%u042C"]="??";
        enc_repl["%u042D"]="??";
        enc_repl["%u042E"]="??";
        enc_repl["%u042F"]="??";
        enc_repl["%u0430"]="??";
        enc_repl["%u0431"]="??";
        enc_repl["%u0432"]="??";
        enc_repl["%u0433"]="??";
        enc_repl["%u0434"]="??";
        enc_repl["%u0435"]="??";
        enc_repl["%u0436"]="??";
        enc_repl["%u0437"]="??";
        enc_repl["%u0438"]="??";
        enc_repl["%u0439"]="??";
        enc_repl["%u043A"]="??";
        enc_repl["%u043B"]="??";
        enc_repl["%u043C"]="??";
        enc_repl["%u043D"]="??";
        enc_repl["%u043E"]="??";
        enc_repl["%u043F"]="??";
        enc_repl["%u0440"]="??";
        enc_repl["%u0441"]="??";
        enc_repl["%u0442"]="??";
        enc_repl["%u0443"]="??";
        enc_repl["%u0444"]="??";
        enc_repl["%u0445"]="??";
        enc_repl["%u0446"]="??";
        enc_repl["%u0447"]="??";
        enc_repl["%u0448"]="??";
        enc_repl["%u0449"]="??";
        enc_repl["%u044A"]="??";
        enc_repl["%u044B"]="??";
        enc_repl["%u044C"]="??";
        enc_repl["%u044D"]="??";
        enc_repl["%u044E"]="??";
        enc_repl["%u044F"]="??";
    }
}
string unescape_rus(string& s)
{
    string r;
    {
        M_LOCK(enc_repl_mx);
        enc_repl_init();
        r=replace_vals(enc_repl,s);
    }
    return repl_percent(r);
}
