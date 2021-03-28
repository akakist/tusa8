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
            r="О"+r.substr(1,r.size()-1);
        }
        if (r[0]=='3') {
            r="З"+r.substr(1,r.size()-1);
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
            r="О"+r.substr(1,r.size()-1);
        }
        if (r[0]=='3') {
            r="З"+r.substr(1,r.size()-1);
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
    char *b="`ёЁ0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ_-$*абвгдежзийклмнопрстуфхцчшщъыьэюяабвгдежзийклмнопрстуфхцчшщъыьэюяАБВГДЕЖЗИЙКЛМНОПРСТУФХЦЧШЩЪЫЬЭЮЯАБВГДЕЖЗИЙКЛМНОПРСТУФХЦЧШЩЪЫЬЭЮЯ\0x0";
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
        itostring(q.tm_mday)+" "+r+", "+itostring(q.tm_year+1900)+" "+"года";
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
            "http://[0-9,a-z,A-Z,А-я,_,/,.,~,;,?,=,:,&,-]+|ftp://[0-9,a-z,A-Z_,-,/,.,@,:]+";
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
	"%u0401","Ё","%u0451","ё","%u0410","А","%u0411","Б","%u0412","В","%u0413","Г","%u0414","Д","%u0415","Е","%u0416","Ж","%u0417","З","%u0418","И",
		"%u0419","Й","%u041A","К","%u041B","Л","%u041C","М","%u041D","Н","%u041E","О","%u041F","П","%u0420","Р","%u0421","С","%u0422","Т","%u0423","У",
		"%u0424","Ф","%u0425","Х","%u0426","Ц","%u0427","Ч","%u0428","Ш","%u0429","Щ","%u042A","Ъ","%u042B","Ы","%u042C","Ь","%u042D","Э","%u042E","Ю",
		"%u042F","Я","%u0430","а","%u0431","б","%u0432","в","%u0433","г","%u0434","д","%u0435","е","%u0436","ж","%u0437","з","%u0438","и","%u0439","й",
		"%u043A","к","%u043B","л","%u043C","м","%u043D","н","%u043E","о","%u043F","п","%u0440","р","%u0441","с","%u0442","т","%u0443","у","%u0444","ф",
		"%u0445","х","%u0446","ц","%u0447","ч","%u0448","ш","%u0449","щ","%u044A","ъ","%u044B","ы","%u044C","ь","%u044D","э","%u044E","ю","%u044F","я",
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
        enc_repl["%u0401"]="Ё";
        enc_repl["%u0451"]="ё";
        enc_repl["%u0410"]="А";
        enc_repl["%u0411"]="Б";
        enc_repl["%u0412"]="В";
        enc_repl["%u0413"]="Г";
        enc_repl["%u0414"]="Д";
        enc_repl["%u0415"]="Е";
        enc_repl["%u0416"]="Ж";
        enc_repl["%u0417"]="З";
        enc_repl["%u0418"]="И";
        enc_repl["%u0419"]="Й";

        enc_repl["%u041A"]="К";
        enc_repl["%u041B"]="Л";
        enc_repl["%u041C"]="М";
        enc_repl["%u041D"]="Н";
        enc_repl["%u041E"]="О";
        enc_repl["%u041F"]="П";
        enc_repl["%u0420"]="Р";
        enc_repl["%u0421"]="С";
        enc_repl["%u0422"]="Т";
        enc_repl["%u0423"]="У";
        enc_repl["%u0424"]="Ф";
        enc_repl["%u0425"]="Х";
        enc_repl["%u0426"]="Ц";
        enc_repl["%u0427"]="Ч";
        enc_repl["%u0428"]="Ш";
        enc_repl["%u0429"]="Щ";
        enc_repl["%u042A"]="Ъ";
        enc_repl["%u042B"]="Ы";
        enc_repl["%u042C"]="Ь";
        enc_repl["%u042D"]="Э";
        enc_repl["%u042E"]="Ю";
        enc_repl["%u042F"]="Я";
        enc_repl["%u0430"]="а";
        enc_repl["%u0431"]="б";
        enc_repl["%u0432"]="в";
        enc_repl["%u0433"]="г";
        enc_repl["%u0434"]="д";
        enc_repl["%u0435"]="е";
        enc_repl["%u0436"]="ж";
        enc_repl["%u0437"]="з";
        enc_repl["%u0438"]="и";
        enc_repl["%u0439"]="й";
        enc_repl["%u043A"]="к";
        enc_repl["%u043B"]="л";
        enc_repl["%u043C"]="м";
        enc_repl["%u043D"]="н";
        enc_repl["%u043E"]="о";
        enc_repl["%u043F"]="п";
        enc_repl["%u0440"]="р";
        enc_repl["%u0441"]="с";
        enc_repl["%u0442"]="т";
        enc_repl["%u0443"]="у";
        enc_repl["%u0444"]="ф";
        enc_repl["%u0445"]="х";
        enc_repl["%u0446"]="ц";
        enc_repl["%u0447"]="ч";
        enc_repl["%u0448"]="ш";
        enc_repl["%u0449"]="щ";
        enc_repl["%u044A"]="ъ";
        enc_repl["%u044B"]="ы";
        enc_repl["%u044C"]="ь";
        enc_repl["%u044D"]="э";
        enc_repl["%u044E"]="ю";
        enc_repl["%u044F"]="я";
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
