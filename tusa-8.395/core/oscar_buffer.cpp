
#include "stdafx.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#include "oscar_buffer.h"
#include <stdio.h>
#include <map>
#include <string>
#include "str_lib.h"
#include "mutex_inspector.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

using namespace std;

oscar_buffer::oscar_buffer(const unsigned char* d, size_t siz) :  out_pos(0), data(d), dsize(siz) { }
oscar_buffer::oscar_buffer(const char* d, size_t siz) :  out_pos(0), data(( unsigned char*)d), dsize(siz) { }



void out_oscar_buffer::Pack(const string& s) {
    adjust(s.size());
    memcpy(&buffer[cur_pos],s.data(),s.size());
    cur_pos+=s.size();
}
void out_oscar_buffer::Pack(const char * s, size_t len)
{
    adjust(len);
    memcpy(&buffer[cur_pos],s,len);
    cur_pos+=len;

}

void oscar_buffer::Unpack(string& s, size_t size) {

    if (size > dsize-out_pos) size = dsize-out_pos;
    s=string((char*)&data[out_pos],size);
    out_pos += size;
}
void out_oscar_buffer::clear() {
    cur_pos=0;
}
// -- Input stream methods --



// -- Output stream methods --

string out_oscar_buffer::as_string() const
{

    return string((char*)buffer,cur_pos);
}


string chk_str_size(const string &s,unsigned int size)
{
    if(s.size()>size) return s.substr(0,size);
    return s;
}
out_oscar_buffer::out_oscar_buffer():buffer(NULL), bufsize(0),cur_pos(0)
{
    buffer=(unsigned char*)malloc(256);
    if(buffer==NULL) throw cError((string)"alloc error"+_DMI().c_str());
    bufsize=256;
    cur_pos=0;

}

out_oscar_buffer::~out_oscar_buffer()
{
    if(buffer)
    {
        free(buffer);
        buffer=NULL;
    }

}
void out_oscar_buffer::adjust(size_t n)
{

    if(cur_pos+n>=bufsize)
    {
        size_t nnew=cur_pos+n+1024;
        buffer=(unsigned char*)realloc(buffer,nnew);
        if(!buffer) throw cError((string)"alloc error"+_DMI().c_str());
        bufsize=nnew;
    }

}

void out_oscar_buffer::put_8(unsigned char c)
{
    adjust(1);
    buffer[cur_pos++]=c;

}
void out_oscar_buffer::put_16LE(unsigned short l)
{
    adjust(2);
    buffer[cur_pos++]=(l & 0xFF);
    buffer[cur_pos++]=((l>>8) & 0xFF);
}
void out_oscar_buffer::put_16BE(unsigned short l)
{
    adjust(2);
    buffer[cur_pos++]=((l>>8) & 0xFF);
    buffer[cur_pos++]=(l & 0xFF);
}
void out_oscar_buffer::put_32LE(unsigned int l)
{
    adjust(4);
    buffer[cur_pos++]=(l & 0xFF);
    buffer[cur_pos++]=((l >> 8) & 0xFF);
    buffer[cur_pos++]=((l >> 16) & 0xFF);
    buffer[cur_pos++]=((l >> 24) & 0xFF);

}
void out_oscar_buffer::put_32BE(unsigned int l)
{
    adjust(4);
    buffer[cur_pos++]=((l >> 24) & 0xFF);
    buffer[cur_pos++]=((l >> 16) & 0xFF);
    buffer[cur_pos++]=((l >> 8) & 0xFF);
    buffer[cur_pos++]=(l & 0xFF);
}
void out_oscar_buffer::put_L_STR(const string & c)
{
    adjust(2+c.size());

    put_16LE$((unsigned short)c.size());
    Pack$(c);
}
void out_oscar_buffer::put_L_L_STR(const string & c)
{
    adjust(4+c.size());

    put_32LE$((unsigned int)c.size());
    Pack$(c);

}
unsigned char oscar_buffer::get_8()
{
    if (out_pos + 1 > dsize) return 0;
    unsigned char c= data[out_pos++];
    return c;
}
unsigned short oscar_buffer::get_16LE()
{
    if (out_pos + 2 > dsize) return 0;
    unsigned short l = ((unsigned short)data[out_pos++]);
    l+= ((unsigned short)data[out_pos++] << 8);

    return l;

}
unsigned short oscar_buffer::get_16BE()
{

    if (out_pos + 2 > dsize) return 0;
    unsigned short  l = ((unsigned short)data[out_pos++] << 8);
    l+= ((unsigned short)data[out_pos++]);
    return l;
}
unsigned int oscar_buffer::get_32LE()
{
    if (out_pos + 4 > dsize) return 0;

    unsigned int   l = ((unsigned int)data[out_pos++]);
    l+= ((unsigned int)data[out_pos++] << 8);
    l+= ((unsigned int)data[out_pos++] << 16);
    l+= ((unsigned int)data[out_pos++] << 24);
    return l;

}
unsigned int oscar_buffer::get_32BE()
{
    if (out_pos + 4 > dsize) return 0;

    unsigned int
    l = ((unsigned int)data[out_pos++] << 24);
    l+= ((unsigned int)data[out_pos++] << 16);
    l+= ((unsigned int)data[out_pos++] << 8);
    l+= ((unsigned int)data[out_pos++]);
    return l;
}
string oscar_buffer::get_L_STR()
{
    string container = "";
    unsigned short  sz=get_16LE();
    for (int i = 0; i < sz && beforeEnd(); i++) {
        unsigned char c=get_8();
        container += string((char *)&c, 1);
    }
    return container;

}
string oscar_buffer::get_L_L_STR()
{
    string container = "";
    unsigned int sz=get_32LE();

    for (int i = 0; i < sz && beforeEnd(); i++) {
        unsigned char c=get_8();

        container += string((char *)&c, 1);
    }
    return container;

}
unsigned int oscar_buffer::get_PN()
{
    unsigned int NN=0;
    while(1)
    {
        unsigned char c=get_8();
        if(c<0x80)
        {
            NN<<=7;
            NN|=c;
            return NN;
        }
        else
        {
            NN<<=7;
            NN|=c&0x7F;

        }
    }
}
void out_oscar_buffer::put_PN(unsigned int N)
{
    adjust(5);
    unsigned char c5=(N>>28) & 0x7F;
    unsigned char c1=N & 0x7F;
    unsigned char c2=(N>>7) & 0x7F;
    unsigned char c3=(N>>14) & 0x7F;
    unsigned char c4=(N>>21) & 0x7F;
    if(c5)
    {
        put_8$(c5|0x80);
        put_8$(c4|0x80);
        put_8$(c3|0x80);
        put_8$(c2|0x80);
        put_8$(c1);
        return;
    }
    else if(c4)
    {
        put_8$(c4|0x80);
        put_8$(c3|0x80);
        put_8$(c2|0x80);
        put_8$(c1);

    }
    else if(c3)
    {
        put_8$(c3|0x80);
        put_8$(c2|0x80);
        put_8$(c1);
    }
    else if(c2)
    {
        put_8$(c2|0x80);
        put_8$(c1);
    }
    else
    {
        put_8$(c1);
    }
}
string oscar_buffer::get_PSTR()
{
    string container = "";
    unsigned int sz=get_PN();

    for (int i = 0; i < sz && beforeEnd(); i++) {
        unsigned char c=get_8();

        container += string((char *)&c, 1);
    }
    return container;

    return "";
}
void out_oscar_buffer::put_PSTR(const string & c)
{
    adjust(5+c.size());
    put_PN$((unsigned int)c.size());
    Pack$(c);
}

oscar_buffer& oscar_buffer::operator>>(unsigned char& p)
{
    p=get_PN();
    return *this;
}
oscar_buffer& oscar_buffer::operator>>(char& p)
{
    p=get_PN();
    return *this;
}
oscar_buffer& oscar_buffer::operator>>(unsigned short& p)
{
    p=get_PN();
    return *this;
}
oscar_buffer& oscar_buffer::operator>>(signed short& p)
{
    p=get_PN();
    return *this;

}
oscar_buffer& oscar_buffer::operator>>(unsigned int&p)
{
    p=get_PN();
    return *this;

}
#ifdef WIN32

#endif
oscar_buffer& oscar_buffer::operator>>(signed int&p)
{
    p=get_PN();
    return *this;

}
oscar_buffer& oscar_buffer::operator>>(unsigned long&p)
{
    p=get_PN();
    return *this;

}
oscar_buffer& oscar_buffer::operator>>(signed long&p)
{
    p=get_PN();
    return *this;

}
oscar_buffer& oscar_buffer::operator>>(string&p)
{
    p=get_PSTR();
    return *this;

}
oscar_buffer& oscar_buffer::operator>>(bool& p)
{
    p=get_PN();
    return *this;

}

out_oscar_buffer& out_oscar_buffer::operator<<(unsigned char c)
{
    put_PN(c);
    return *this;
}
out_oscar_buffer& out_oscar_buffer::operator<<(char c)
{
    put_PN(c);
    return *this;
}
out_oscar_buffer& out_oscar_buffer::operator<<(unsigned short c)
{
    put_PN(c);
    return *this;
}
out_oscar_buffer& out_oscar_buffer::operator<<(short c)
{
    put_PN(c);
    return *this;
}
out_oscar_buffer& out_oscar_buffer::operator<<(unsigned int c)
{
    put_PN(c);
    return *this;
}
#ifdef WIN32

#endif
out_oscar_buffer& out_oscar_buffer::operator<<(int c)
{
    put_PN(c);
    return *this;
}
out_oscar_buffer& out_oscar_buffer::operator<<(unsigned long c)
{
    put_PN((int)c);
    return *this;
}
out_oscar_buffer& out_oscar_buffer::operator<<(signed long c)
{
    put_PN((int)c);
    return *this;
}
out_oscar_buffer& out_oscar_buffer::operator<<(const string& c)
{
    put_PSTR(c);
    return *this;
}
out_oscar_buffer& out_oscar_buffer::operator<<(const char* c)
{
    put_PSTR(c);
    return *this;
}
out_oscar_buffer& out_oscar_buffer::operator<<(const bool& c)
{
    put_PN(c);
    return *this;
}

size_t out_oscar_buffer::size() const
{
    return cur_pos;
}
void FS_outV2::calc_size() const
{
    out_oscar_buffer *p=(out_oscar_buffer *)this;
    size_t ocp=p->cur_pos;
    p->cur_pos=2;
    p->put_32LE(ocp-FLAP2_HEADER_SIZE);
    p->cur_pos=ocp;
}
void F_outV2::calc_size() const
{
    out_oscar_buffer *p=(out_oscar_buffer *)this;
    size_t ocp=p->cur_pos;
    p->cur_pos=2;
    p->put_32LE(ocp-FLAP2_HEADER_SIZE);
    p->cur_pos=ocp;
}

FS_outV2::FS_outV2(unsigned char channel, int fam, int stype)
{
    put_8(OSCAR2_START_BYTE);
    put_8(channel);
    put_32LE(0);
    put_8(fam);
    put_8(stype);
}
F_outV2::F_outV2(unsigned char channel)
{
    put_8(OSCAR2_START_BYTE);
    put_8(channel);
    put_32LE(0);

}
OCALC::OCALC(const FS_outV2&b):buf(b)
{
    b.calc_size();
}
OCALC::OCALC(const F_outV2&b):buf(b)
{
    b.calc_size();
}
string OCALC::as_string() const
{

    return buf.as_string();
}

oscar_buffer& oscar_buffer::operator>>(double& v)
{
    string s;
    (*this)>>s;
    sscanf(s.c_str(),"%f",&v);
    return *this;
}
out_oscar_buffer& out_oscar_buffer::operator<<(const double& v)
{
    char s[100];
    snprintf(s,sizeof(s),"%f",v);
    (*this)<<s;
    return *this;
}
void out_oscar_buffer::put_PN$(unsigned int N)
{
    unsigned char c1=N & 0x7F;
    unsigned char c2=(N>>7) & 0x7F;
    unsigned char c3=(N>>14) & 0x7F;
    unsigned char c4=(N>>21) & 0x7F;
    unsigned char c5=(N>>28) & 0x7F;
    if(c5)
    {
        put_8$(c5|0x80);
        put_8$(c4|0x80);
        put_8$(c3|0x80);
        put_8$(c2|0x80);
        put_8$(c1);
        return;
    }
    else if(c4)
    {
        put_8$(c4|0x80);
        put_8$(c3|0x80);
        put_8$(c2|0x80);
        put_8$(c1);

    }
    else if(c3)
    {
        put_8$(c3|0x80);
        put_8$(c2|0x80);
        put_8$(c1);
    }
    else if(c2)
    {
        put_8$(c2|0x80);
        put_8$(c1);
    }
    else
    {
        put_8$(c1);
    }
}

void out_oscar_buffer::put_32LE$(unsigned int l)
{
    buffer[cur_pos++]=(l & 0xFF);
    buffer[cur_pos++]=((l >> 8) & 0xFF);
    buffer[cur_pos++]=((l >> 16) & 0xFF);
    buffer[cur_pos++]=((l >> 24) & 0xFF);
}
void out_oscar_buffer::put_32BE$(unsigned int l)
{
    buffer[cur_pos++]=((l >> 24) & 0xFF);
    buffer[cur_pos++]=((l >> 16) & 0xFF);
    buffer[cur_pos++]=((l >> 8) & 0xFF);
    buffer[cur_pos++]=(l & 0xFF);
}
