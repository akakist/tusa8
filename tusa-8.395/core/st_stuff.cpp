#include "stdafx.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#ifndef WIN32
#include <sys/stat.h>
#endif
#include "st_stuff.h"
#include "str_lib.h"
#include "errs.h"
#include <errno.h>
#include <errno.h>
#include "_mutex.h"
#include "mutex_inspector.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

void st_stuff_cpp() {}
st_malloc::st_malloc(unsigned long size)
{
    buf = malloc(size + 1);
    if(buf==NULL)throw cError("st_malloc failed");
    DUMP("malloc");
}

st_malloc::~st_malloc()
{
    free(buf);
    DUMP("free");
}

void st_stream::puts(const char *s)
{
    int len = strlen(s);
    write(s, len);
}
void st_stream::puts(const string & s)
{
    write(s.data(), s.size());
}
void st_stream::write(const void *p, int n) const
{
    int res=writesome(p,n);
    if(res!=n)
        throw cError("write failed on ");
}


const st_stream & st_stream::operator << (const string & s)const
{
    write((const void *) s.data(), s.size());
    return *this;
}
const st_stream & st_stream::operator << (const char *s) const
{
    write(s, strlen(s));
    return *this;
}

const st_stream & st_stream::operator << (unsigned  n) const
{
    string s = itostring(n);
    write(s.data(), s.size());
    return *this;
}

void st_stream::read(void *p, int n)
{
    int res=readsome(p,n);
    if(res!=n)
    {
        throw cError("st_stream::read not enough received");
    }
}
int st_stream::readsome(void *, int)
{
    throw cError("notimplemented readsome");
    return 0;
}
int st_stream::writesome(const void *, int) const
{
    throw cError("notimplemented write");
    return 0;
}


//////////////////////////////////////
st_stream& st_stream::operator>=(unsigned int &k)
{
    unsigned char s[4];
    read(s,4);
    unsigned int r=s[3]*0x1000000+s[2]*0x10000+s[1]*0x100+s[0];
    k=r;
    return *this;
}
st_stream& st_stream::operator>=(int &k)
{
    unsigned char s[4];
    read(s,4);
    unsigned int r=s[3]*0x1000000+s[2]*0x10000+s[1]*0x100+s[0];
    k=r;
    return *this;
}
st_stream& st_stream::operator>=(bool &k)
{
    unsigned char s[4];
    read(s,4);
    unsigned int r=s[3]*0x1000000+s[2]*0x10000+s[1]*0x100+s[0];
    k=r;
    return *this;
}
#ifndef macintosh
st_stream& st_stream::operator>=(long &k)
{
    unsigned char s[4];
    read(s,4);
    unsigned int r=s[3]*0x1000000+s[2]*0x10000+s[1]*0x100+s[0];
    k=r;
    return *this;
}
#endif
st_stream& st_stream::operator>=(string  &k)
{
    unsigned int l;
    *this>=l;
    st_malloc ss(l+1);
    read(ss.buf,l);
    k=string((char*)ss.buf,l);
    return *this;
}
const st_stream & st_stream::operator<=(unsigned n) const
{
    unsigned char s[4];
    s[3]=n/0x1000000;
    s[2]=(n&0xffffff)/0x10000;
    s[1]=(n&0xffff)/0x100;
    s[0]=n&0xff;
    write(s,4);
    return *this;
}
const st_stream & st_stream::operator<=(int n) const
{
    unsigned char s[4];
    s[3]=n/0x1000000;
    s[2]=(n&0xffffff)/0x10000;
    s[1]=(n&0xffff)/0x100;
    s[0]=n&0xff;
    write(s,4);
    return *this;
}
const st_stream & st_stream::operator<=(bool n) const
{
    unsigned char s[4];
    s[3]=n/0x1000000;
    s[2]=(n&0xffffff)/0x10000;
    s[1]=(n&0xffff)/0x100;
    s[0]=n&0xff;
    write(s,4);
    return *this;
}
#ifndef macintosh
const st_stream & st_stream::operator<=(long n) const
{
    unsigned char s[4];
    s[3]=n/0x1000000;
    s[2]=(n&0xffffff)/0x10000;
    s[1]=(n&0xffff)/0x100;
    s[0]=n&0xff;
    write(s,4);
    return *this;
}
#endif
const st_stream & st_stream::operator<=(const string& s) const
{

    *this<<s.size();
    write(s.data(),s.size());
    return *this;
}
const st_stream & st_stream::operator<=(const char * s) const
{
    *this<<strlen(s);
    write(s,strlen(s));
    return *this;
}

