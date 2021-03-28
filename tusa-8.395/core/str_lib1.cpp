#include "stdafx.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <iterator>
#include <fstream>
#include <iostream>

#include <sys/types.h>
#include <sys/stat.h>
#ifndef WIN32
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/stat.h>
#else
#include <time.h>
#endif
#include <vector>
#include <map>
#include <map>
#include "oscar_buffer.h"
#include "Double.h"
#include <stdio.h>
#include "_mutex.h"
#include "errs.h"
#include "str_lib.h"
#include <errno.h>
#include <sys/types.h>
#ifndef WIN32
#include <sys/stat.h>
#endif
#include <stdio.h>
#include <vector>
#include <list>
#include "str_lib.h"
#ifndef WIN32
#include <sys/errno.h>
#endif
#include "errs.h"
#include <cerrno>
#include "str_lib.h"
#ifndef WIN32
#include <zlib.h>
#endif
#include "mutex_inspector.h"
#include <cstdarg>
struct tm LOCALTIME(time_t timep);


#ifdef WIN32
#define snprintf _snprintf
#endif
using namespace std;
size_t load_1_file(string & res, const string & fn);//-1 err

void str_lib_cpp() {}
_mutex ctimeMX_("ctimeMX_");
string CTIME(const time_t p)
{
    M_LOCK(ctimeMX_);
    return ctime(&p);
}
string get_file_ext(const string & f)
{
    size_t zz;
    zz = f.rfind(".", f.size());
    if (zz != -1) {
        return f.substr(zz + 1, f.size() - zz - 1);
    }
    return "";
}




char *strdup(char *s)
{
    char *ss = (char *)::malloc(::strlen(s) + 2);
    if (ss == NULL) {
        throw cError("malloc() failed");
    }
    if (ss)
        ::strcpy(ss, s);
    return ss;
}

unsigned char *pp2cc(unsigned char *s255)
{
    unsigned char len = s255[0];
    int i;
    for (i = 0; i < len; i++) {
        s255[i] = s255[i + 1];
    }
    s255[i] = 0;
    return s255;
}
unsigned char *cc2pp(unsigned char *s255)
{
    unsigned char len = (unsigned char)::strlen((char *)s255);
    int i;
    for (i = len; i > 0; i--) {
        s255[i] = s255[i - 1];
    }
    s255[0] = len;
    return s255;
}
unsigned char *p2c(unsigned char *s255)
{
    unsigned char len = s255[0];
    unsigned char *s = (unsigned char *)::malloc((unsigned long)(len + 2));
    if (s == NULL) {
        throw cError("malloc() failed");
    }
    ::memcpy(s, &s255[1], len);
    s[len] = 0;
    return s;
}
#ifdef macintosh
unsigned char *c2p(unsigned char *ss)
{
    unsigned char len = (unsigned char)strlen((char *)ss);
    unsigned char *s = (unsigned char *)malloc((unsigned long)(len + 2));
    if (s == NULL) {
        throw cError("malloc() failed");
    }
    s[0] = len;
    memcpy(&s[1], ss, len);
    return s;
}


#endif


string hex2bin(const string &s)
{
    string out="";
    char *p=(char*)s.data();
    size_t sz=s.size();
    for(int i=0; i<sz; i+=2)
    {
        char ss[3]= {0};
        ::memcpy(ss,&p[i],2);
        unsigned char c=(unsigned char)strtol(ss,NULL,16);
        out+=string((char*)&c,1);
    }
    return out;
}
string bin2hex(const string & in, int /* n */ )
{
    string out = "";
    const unsigned char *p = (unsigned char *)in.c_str();
    for (unsigned int i = 0; i < in.size(); i++) {
        char s[40];
        ::snprintf(s, sizeof(s) - 1, "%02X", p[i]);
        out += s;
    }
    return out;
}

string lzstring(int i)
{
    string a=itostring(i);
    if(a.size()==1) {
        return "0"+a;
    }
    return a;
}

string date2mysql(time_t t)
{
    struct tm q=LOCALTIME(t);
    char s[200];
    snprintf(s,sizeof(s),"%d-%02d-%02d %02d:%02d:%02d",q.tm_year+1900,q.tm_mon+1,q.tm_mday,q.tm_hour,q.tm_min,q.tm_sec);
    return s;
}
string date2mysql_short(time_t t)
{
    struct tm q=LOCALTIME(t);
    char s[200];
    snprintf(s,sizeof(s),"%d-%02d-%02d",q.tm_year+1900,q.tm_mon+1,q.tm_mday);
    return s;
}

time_t mysql2date(const string & t)
{
    vector <string> v = splitTOK(":- ", t);
    if (v.size() != 6) {
        throw cError("mysql2date: to few arguments! t=" + t);
    }
    struct tm q;
    q.tm_year = atoi(v[0]) - 1900;
    q.tm_mon = atoi(v[1]) - 1;
    q.tm_mday = atoi(v[2]);
    q.tm_hour = atoi(v[3]);
    q.tm_min = atoi(v[4]);
    q.tm_sec = atoi(v[5]);
    return mktime(&q);
}


string to_mac_enc(const string & s)
{
    return s;
}
string from_mac_dec(const string & s)
{
#ifdef macintosh
    char *ss = strdup(s.c_str());
    buf_to_pc((unsigned char *)ss, s.size());
    string ret = ss;
    free(ss);
    return ret;
#else
    return s;
#endif
}
string sql_in(const vector <string> &res)
{
    return " IN (" + join(",", res) + ") ";
}

static char    six2pr[64] = {
    'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M',
    'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z',
    'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm',
    'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z',
    '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '+', '/'
};

static unsigned char pr2six[256];


/*--- function HTUU_encode -----------------------------------------------
 *
 *   Encode a single line of binary data to a standard format that
 *   uses only printing ASCII characters (but takes up 33% more bytes).
 *
 *    Entry    bufin    points to a buffer of bytes.  If nbytes is not
 *                      a multiple of three, then the byte just beyond
 *                      the last byte in the buffer must be 0.
 *             nbytes   is the number of bytes in that buffer.
 *                      This cannot be more than 48.
 *             bufcoded points to an output buffer.  Be sure that this
 *                      can hold at least 1 + (4*nbytes)/3 characters.
 *
 *    Exit     bufcoded contains the coded line.  The first 4*nbytes/3 bytes
 *                      contain printing ASCII characters representing
 *                      those binary bytes. This may include one or
 *                      two '=' characters used as padding at the end.
 *                      The last byte is a zero byte.
 *             Returns the number of ASCII characters in "bufcoded".
 */
static int
Ns_HtuuEncode(unsigned char *bufin, unsigned int nbytes, char * bufcoded)
{

    /* ENC is the basic 1 character encoding function to make a char printing */
#define ENC(c) six2pr[c]

    register char  *outptr = bufcoded;
    unsigned int    i;

    for (i = 0; i < nbytes; i += 3) {
        /* c1 */
        *(outptr++) = ENC(*bufin >> 2);
        /* c2 */
        *(outptr++) = ENC(((*bufin << 4) & 060) | ((bufin[1] >> 4) & 017));
        /* c3 */
        *(outptr++) = ENC(((bufin[1] << 2) & 074) | ((bufin[2] >> 6) & 03));
        /* c4 */
        *(outptr++) = ENC(bufin[2] & 077);

        bufin += 3;
    }

    /*
     * If nbytes was not a multiple of 3, then we have encoded too many
     * characters.  Adjust appropriately.
     */
    if (i == nbytes + 1) {
        /* There were only 2 bytes in that last group */
        outptr[-1] = '=';
    } else if (i == nbytes + 2) {
        /* There was only 1 byte in that last group */
        outptr[-1] = '=';
        outptr[-2] = '=';
    }
    *outptr = '\0';
    return (outptr - bufcoded);
}


/*--- function HTUU_decode ------------------------------------------------
 *
 *  Decode an ASCII-encoded buffer back to its original binary form.
 *
 *    Entry    bufcoded    points to a uuencoded string.  It is
 *                         terminated by any character not in
 *                         the printable character table six2pr, but
 *                         leading whitespace is stripped.
 *             bufplain    points to the output buffer; must be big
 *                         enough to hold the decoded string (generally
 *                         shorter than the encoded string) plus
 *                         as many as two extra bytes used during
 *                         the decoding process.
 *             outbufsize  is the maximum number of bytes that
 *                         can fit in bufplain.
 *
 *    Exit     Returns the number of binary bytes decoded.
 *             bufplain    contains these bytes.
 */
static int
Ns_HtuuDecode(char * bufcoded, unsigned char * bufplain, int outbufsize)
{

    /* single character decode */
#define DEC(c) pr2six[(int)c]
#define MAXVAL 63

    static int      first = 1;

    int             nbytesdecoded, j;
    register char  *bufin = bufcoded;
    register unsigned char *bufout = bufplain;
    register int    nprbytes;

    /*
     * If this is the first call, initialize the mapping table. This code
     * should work even on non-ASCII machines.
     */
    if (first) {
        first = 0;
        for (j = 0; j < 256; j++)
            pr2six[j] = MAXVAL + 1;

        for (j = 0; j < 64; j++)
            pr2six[(int) six2pr[j]] = (unsigned char) j;
    }

    /* Strip leading whitespace. */

    while (*bufcoded == ' ' || *bufcoded == '\t')
        bufcoded++;

    /*
     * Figure out how many characters are in the input buffer. If this would
     * decode into more bytes than would fit into the output buffer, adjust
     * the number of input bytes downwards.
     */
    bufin = bufcoded;
    while (pr2six[(int) *(bufin++)] <= MAXVAL) {};
    nprbytes = bufin - bufcoded - 1;
    nbytesdecoded = ((nprbytes + 3) / 4) * 3;
    if (nbytesdecoded > outbufsize) {
        nprbytes = (outbufsize * 4) / 3;
    }
    bufin = bufcoded;

    while (nprbytes > 0) {
        *(bufout++) = (unsigned char) (DEC(*bufin) << 2 | DEC(bufin[1]) >> 4);
        *(bufout++) = (unsigned char) (DEC(bufin[1]) << 4 |
                                       DEC(bufin[2]) >> 2);
        *(bufout++) = (unsigned char) (DEC(bufin[2]) << 6 | DEC(bufin[3]));
        bufin += 4;
        nprbytes -= 4;
    }

    if (nprbytes & 03) {
        if (pr2six[(int) bufin[-2]] > MAXVAL) {
            nbytesdecoded -= 2;
        } else {
            nbytesdecoded -= 1;
        }
    }
    return (nbytesdecoded);
}


string Base64Encode(const string &str)
{
    size_t len=str.size()*2;
    char *out=(char*)malloc(len+10);
    ::memset(out,0,len+2);
    int outlen=Ns_HtuuEncode((unsigned char*)str.data(),str.size(),out);
    string ret(out,outlen);
    free(out);
    return ret;
}
string Base64Decode(const string &str)
{
    size_t len=str.size();
    char *out=(char*)malloc(len+10);
    ::memset(out,0,len+2);
    int outlen=Ns_HtuuDecode((char*)str.data(),(unsigned char*)out,len);

    string ret(out,outlen);
    free(out);
    return ret;
}


string basedir;

char* cypher_names[]= {
    "NONE","RSA128","RSA40","DES_3CBC","DES_ECB3","DES_CBC","DES_ECB"
};
int cypher_names_size=sizeof(cypher_names);


void hex_dump(char *data, long size)
{
    long i;
    long x;
    int r=1;
    char d[32];
    unsigned char bfr[64];
    for(i=0, x=0;; i++)
    {
        if(i<size)
        {
            if(x==0)
                printf("%04lx: ", i);
#ifdef _WIN32
            ::snprintf(d, sizeof(d)-1, "%08x", data[i]);
#else
            ::snprintf(d, sizeof(d)-1, "%08x", data[i]);
#endif
            ::printf("%c%c ", d[6], d[7]);
            bfr[x]=data[i];
            if(bfr[x]<0x20)
                bfr[x]='.';
            if(bfr[x]>0x7f)
                bfr[x]='.';
        }
        else
        {
            if(x==0)
                break;
            else
            {
                printf("   ");
                bfr[x]=' ';
                r=0;
            }
        }
        x++;
        if(!(x<16))
        {
            bfr[x]=0;
            printf("%s\n", (const char*)bfr);
            x=0;
            if(!r)
                break;
        }
    }
}

string uitostring(unsigned int i)
{
    char ss[200];
    string s;
    ::snprintf(ss,sizeof(ss)-1, "%u", i);
    s = ss;
    return s;
}
string litostring(long int i)
{
    char ss[200];
    string s;
    ::snprintf(ss,sizeof(ss)-1, "%ld", i);
    s = ss;
    return s;
}
string luitostring(long unsigned int i)
{
    char ss[200];
    string s;
    ::snprintf(ss,sizeof(ss)-1, "%lu", i);
    s = ss;
    return s;
}
string mysql_datetime2date(const string & s)
{
    vector <string> vdate = splitTOK(" :-", s);
    if (vdate.size() == 6) {
        char b[100];
        ::snprintf(b,sizeof(b)-1,"%02d-%02d-%02d",atoi(vdate[2].c_str()),atoi(vdate[1].c_str()),atoi(vdate[0].c_str()));
        return b;
    }
    return "";
}
string mysql_datetime2time(const string & s)
{
    vector <string> vdate = splitTOK(" :-", s);
    if (vdate.size() == 6) {
        char b[100];
        ::snprintf(b,sizeof(b)-1,"%02d:%02d:%02d",atoi(vdate[3].c_str()),atoi(vdate[4].c_str()),atoi(vdate[5].c_str()));
        return b;
    }
    return "";
}
string remove_commas(const string& ss)
{
    string out="";
    string s2=ss;
    unsigned char *s=(unsigned char*)s2.c_str();
    char *cs=(char*)s2.c_str();
    unsigned char *cus=(unsigned char*)s2.c_str();
    size_t len=::strlen((const char*)s);
    for(unsigned int i=0; i<len ; i++)
    {
        unsigned int n=(unsigned) s[i];
        char c=cs[i];
        unsigned char uc=cus[i];
        if(	  (c>='a'&&c<='z')
                ||(c>='A'&&c<='Z')
                ||(c>='0'&&c<='9')
                ||(uc>=128)
          )out+=s[i];
        else out+="&#"+itostring(n)+";";
    }
    return out;
}

set <string> splitTOKs(const char *seps, const string & src)
{

    set <string> res;
    map < char, int >mm;
    size_t l;
    l =::strlen(seps);
    for (unsigned int i = 0; i < l; i++) {
        mm[seps[i]] = 100;
    }
    string tmp;
    l = src.size();
{   for (unsigned int i = 0; i < l; i++) {

            if (mm[src[i]] != 100)
                tmp += src[i];
            else {
                if (tmp.size()) {
                    res.insert(tmp);
                    tmp = "";
                }
            }
        }
    }
    if (tmp.size()) {
        res.insert(tmp);
        tmp = "";
    }
    return res;
}
list <string> splitTOKl(const char *seps, const char *src)
{

    list <string> res;
    map < char, int >mm;
    size_t l;
    l =::strlen(seps);
    for (unsigned int i = 0; i < l; i++) {
        mm[seps[i]] = 100;
    }
    string tmp;
    l =::strlen(src);
{   for (unsigned int i = 0; i < l; i++) {

            if (mm[src[i]] != 100)
                tmp += src[i];
            else {
                if (tmp.size()) {
                    res.push_back(tmp);
                    tmp = "";
                }
            }
        }
    }
    if (tmp.size()) {
        res.push_back(tmp);
        tmp = "";
    }
    return res;
}
list <string> splitTOKl(const char *seps, const string & src)
{
    list <string> res;
    map < char, int >mm;
    size_t l;
    l =::strlen(seps);
    for (unsigned int i = 0; i < l; i++) {
        mm[seps[i]] = 100;
    }
    string tmp;
    l = src.size();
{   for (unsigned int i = 0; i < l; i++) {

            if (mm[src[i]] != 100)
                tmp += src[i];
            else {
                if (tmp.size()) {
                    res.push_back(tmp);
                    tmp = "";
                }
            }
        }
    }
    if (tmp.size()) {
        res.push_back(tmp);
        tmp = "";
    }
    return res;
}


string fmt_d(const char *s,double d)
{
    char ss[100];
    snprintf(ss,sizeof(ss)-1,s,d);
    return ss;
}
bool __isspace(int n)
{
    switch(n)
    {
    case '\n':
    case '\r':
    case ' ':
    case '\t':
        return true;
    default:
        return false;
    }
    return false;
}
map<string,string> load_m_file(const string &fn)
{

    string body;
    string section;
    size_t r=load_1_file(body,fn);
    if (r==-1)
    {
        logErr2("--Error: Failed to read file %s (%s)", fn.c_str(), strerror(errno));
        map<string,string>m;
        return m;

    }
    vector<string> v=splitTOK("\r\n",body.c_str());
    map<string,string>m;
    for (unsigned int i=0; i<v.size(); i++) {

        string s=v[i];
        bool empty=true;
        {
            for (unsigned kk=0; kk<s.size(); kk++)if (!__isspace(s[kk])) empty=false;
        }
        while (s.size())
        {
            if (isspace(s[0])) s=s.substr(1,s.size()-1);
            else break;
        }

        if (empty) continue;
        if (s=="#" || !s.size()) continue;
        if (s.size()) {
            if (s[0]=='#')continue;
        }
        if (s[0]=='%')
        {
            section=s.substr(1,s.size()-1);
            continue;

        }

        size_t pz=s.find("=",0);
        if (pz==-1) {
            logErr2("--Error: syntax error in file %s line %d '%s' %s",fn.c_str(),i,s.c_str(),_DMI().c_str());
            continue;
        }
        string key;
        if (section.size())key=section+"_"+s.substr(0,pz);
        else key=s.substr(0,pz);

        string val=s.substr(pz+1,s.size()-pz-1);
        if (val.size())
        {
            if (val[0]=='$')
            {
                string pn=val.substr(1,val.size()-1);
                string vv;
                if (load_1_file(vv,pn)==-1)
                {
                    logErr2("--Error: syntax error in file %s line %d cannot load file '%s' %s",fn.c_str(),i,pn.c_str(),_DMI().c_str());

                }
                val=vv;
            }
        }
        m[key]=val;
    }
    return m;
}
map<string,string> load_m_file_no$(const string &fn)
{

    string body;
    size_t	r=load_1_file(body,fn);
    if(r==-1)
    {
        logErr2("--Error: Failed to read file %s (%s)", fn.c_str(), strerror(errno));
        map<string,string>m;
        return m;

    }
    vector<string> v=splitTOK("\r\n",body.c_str());
    map<string,string>m;
    for(unsigned int i=0; i<v.size(); i++) {

        string s=v[i];
        bool empty=true;
        {
            for(unsigned kk=0; kk<s.size(); kk++)if(!isspace(s[kk])) empty=false;
        }
        if(empty) continue;
        if(s=="#" || !s.size()) continue;
        if(s.size()) {
            if(s[0]=='#')continue;
        }
        size_t pz=s.find("=",0);
        if(pz==-1) {
            logErr2("--Error: syntax error in file %s line %d '%s' %s",fn.c_str(),i,s.c_str(),_DMI().c_str());
            continue;
        }
        string key=s.substr(0,pz);
        string val=s.substr(pz+1,s.size()-pz-1);
        m[key]=val;
    }
    return m;
}
map<int,string> load_m_is_file(const string &fn)
{
    map<string,string>m;
    map<int,string>ret;
    m=load_m_file(fn);
    for(map<string,string>::const_iterator i=m.begin(); i!=m.end(); i++)
    {
        ret[atoi(i->first)]=i->second;
    }
    return ret;
}

void set_cfg_i(map<string,string> &m,const string&name, const string& pn,unsigned int &v, unsigned int defv)
{
    if(m.count(name))
        v=atoi(m[name]);
    else
    {
        logErr2("%s: skipped param, using deflt %s=%d",pn.c_str(),name.c_str(),defv);
        v=defv;
    }
    m.erase(name);

}
void set_cfg_i(map<string,string> &m,const string&name, const string& pn,m_var<unsigned int> &v, unsigned int defv)
{
    if(m.count(name))
        v=atoi(m[name]);
    else
    {
        logErr2("%s: skipped param, using deflt %s=%d",pn.c_str(),name.c_str(),defv);
        v=defv;
    }
    m.erase(name);

}
void set_cfg_i(map<string,string> &m,const string&name, const string& pn,int &v, int defv)
{
    if(m.count(name))
        v=atoi(m[name]);
    else
    {
        logErr2("%s: skipped param, using deflt %s=%d",pn.c_str(),name.c_str(),defv);
        v=defv;
    }

    m.erase(name);
}
void set_cfg_i(map<string,string> &m,const string&name, const string& pn,m_var<int> &v, int defv)
{
    if(m.count(name))
        v=atoi(m[name]);
    else
    {
        logErr2("%s: skipped param, using deflt %s=%d",pn.c_str(),name.c_str(),defv);
        v=defv;
    }

    m.erase(name);
}
void set_cfg_b(map<string,string> &m,const string&name, const string& pn,m_var<bool> &v, bool defv)
{
    if(m.count(name))
        v=m[name]=="true";
    else
    {
        logErr2("%s: skipped param, using deflt %s=%s",pn.c_str(),name.c_str(),defv==true?"true":"false");
        v=defv;
    }

    m.erase(name);
}
void set_cfg_f(map<string,string> &m,const string& name, const string& pn,m_var<double> &v, double defv)
{
    if(m.count(name))
        v=atof(m[name].c_str());
    else
    {
        logErr2("%s: skipped '%s' parameter, using default '%f'",pn.c_str(),name.c_str(),defv);
        v=defv;
    }

    m.erase(name);
}
void set_cfg_f(map<string,string> &m,const string& name, const string& pn,m_var<Double> &v, Double defv)
{
    if(m.count(name))
    {
        Double d;
        d.container=atof(m[name].c_str());
        v=d;
    }
    else
    {
        logErr2("%s: skipped '%s' parameter, using default '%f'",pn.c_str(),name.c_str(),defv.container);
        v=defv;
    }

    m.erase(name);
}
string get_cfg_str(map<string,string> &m,const string& name,const string&pn, const string& defv)
{
    string r;
    if(m.count(name))
        r=m[name];
    else
    {
        logErr2("%s: skipped param, using deflt %s=%s",pn.c_str(),name.c_str(),defv.c_str());
        r=defv;
    }

    m.erase(name);
    return r;
}
void set_cfg_str(map<string,string> &m,const string&name, const string& pn,m_var<string> &v, const string& defv)
{
    if(m.count(name))
        v=m[name];
    else
    {
        logErr2("%s: skipped param, using deflt %s=%s",pn.c_str(),name.c_str(),defv.c_str());
        v=defv;
    }
    m.erase(name);
}
void print_rest_cfg(std::map<std::string,std::string> &m,const std::string& pn)
{
    for(std::map<std::string,std::string> ::const_iterator i=m.begin(); i!=m.end(); i++)
    {
        logErr2("%s: unused param '%s'",pn.c_str(),i->first.c_str());
    }
}
void set_cfg_ishort(map<string,string> &m,const string&name, const string& pn,m_var<unsigned short> &v, unsigned short defv)
{
    if(m.count(name))
        v=atoi(m[name]);
    else
    {
        logErr2("%s: skipped param, using deflt %s=%d",pn.c_str(),name.c_str(),defv);
        v=defv;
    }

    m.erase(name);
}

map<string,string> load_m_file_noerr(const string &fn)
{

    string body;
    size_t r=load_1_file(body,fn);
    if(r==-1)
    {
        map<string,string>m;
        return m;

    }
    vector<string> v=splitTOK("\r\n",body.c_str());
    map<string,string>m;
    for(unsigned int i=0; i<v.size(); i++) {

        string s=v[i];
        bool empty=true;
        {
            for(unsigned kk=0; kk<s.size(); kk++)if(!isspace(s[kk])) empty=false;
        }
        if(empty) continue;
        if(s=="#" || !s.size()) continue;
        if(s.size()) {
            if(s[0]=='#')continue;
        }
        size_t pz=s.find("=",0);
        if(pz==-1) {
            logErr2("--Error: syntax error in file %s line %d '%s' %s",fn.c_str(),i,s.c_str(),_DMI().c_str());
        }
        string key=s.substr(0,pz);
        string val=s.substr(pz+1,s.size()-pz-1);
        if(val.size())
        {
            if(val[0]=='$')
            {
                string pn=val.substr(1,val.size()-1);
                string vv;
                if(load_1_file(vv,pn)==-1)
                {
                    logErr2("--Error: syntax error in file %s line %d cannot load file '%s' %s",fn.c_str(),i,pn.c_str(),_DMI().c_str());

                }
                val=vv;
            }
        }
        m[key]=val;
    }
    return m;
}
map<string,string> load_m_file_noerr_no$(const string &fn)
{

    string body;
    size_t r=load_1_file(body,fn);
    if(r==-1)
    {
        map<string,string>m;
        return m;

    }
    vector<string> v=splitTOK("\r\n",body.c_str());
    map<string,string>m;
    for(unsigned int i=0; i<v.size(); i++) {

        string s=v[i];
        bool empty=true;
        {
            for(unsigned kk=0; kk<s.size(); kk++)if(!isspace(s[kk])) empty=false;
        }
        if(empty) continue;
        if(s=="#" || !s.size()) continue;
        if(s.size()) {
            if(s[0]=='#')continue;
        }
        size_t pz=s.find("=",0);
        if(pz==-1) {
            logErr2("--Error: syntax error in file %s line %d '%s' %s",fn.c_str(),i,s.c_str(),_DMI().c_str());
        }
        string key=s.substr(0,pz);
        string val=s.substr(pz+1,s.size()-pz-1);
        m[key]=val;
    }
    return m;
}

size_t load_1_file_from_disk(string & res, const string & fn)
{
#ifdef WIN32
    struct _stat  st;
#else
    struct stat  st;
#endif
#ifdef WIN32
    if (_stat(fn.c_str(), &st))
#else
    if (stat(fn.c_str(), &st))
#endif
    {
        return -1;
    }
    FILE *f=fopen(fn.c_str(), "rb");
    if(f)
    {
        unsigned char *b=(unsigned char *) malloc(st.st_size);
        if(b==NULL)
        {
            fclose(f);
            throw cError("b==NULL");
        }
        if(fread(b,1,st.st_size,f)!=st.st_size)
        {
            fclose(f);
            free(b);
            throw cError("if(fread(b,1,st.st_size,f)!=st.st_size) ");
        }
        string al((char *)b, st.st_size);
        fclose(f);
        free(b);
        res=al;
        return st.st_size;
    }
    return -1;
}
