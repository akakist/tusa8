#ifndef BUFFER_H
#define BUFFER_H
#include <string.h>
#include <vector>
#include <set>
#include <deque>
#include <iostream>
#ifndef WIN32
//#include <iomanip.h>
#endif
#include <string>
#include <iterator>
#include <algorithm>
#include <map>
#include <map>
#include <set>
#include <list>
#ifndef WIN32

#endif
#define OSCAR2_START_BYTE 22
typedef  unsigned int OSCAR2_DATA_LEN_T;
#define FLAP2_HEADER_SIZE (sizeof(unsigned char)+sizeof(unsigned char)+sizeof(unsigned int))
#define FLAP2_CHANNEL_OFFSET 1
#define FLAP2_DATA_SIZE_OFFSET 2

#define PN_MAX 0x40404040

using namespace std;
#ifdef WIN32
typedef __int64 datetime;
#else
typedef long long datetime;
#endif






class
#ifdef WIN32
    __declspec(dllexport)
#endif
    oscar_buffer
{
public:


private:
    size_t out_pos;
    size_t dsize;
    const unsigned char *data;
public:
    ~oscar_buffer() {};
    oscar_buffer(const unsigned char *d, size_t size); // construct from an array
    oscar_buffer(const  char *d, size_t size); // construct from an array

    size_t size() const {
        return dsize;
    }
    const unsigned char *_data() {
        return data;
    }
    size_t remains() const {
        return dsize - out_pos;
    }
    bool beforeEnd() const {
        return (out_pos < dsize);
    }



public:
    void Unpack(string& s, size_t size) ;

    unsigned char get_8();
    unsigned short get_16LE();
    unsigned short get_16BE();
    unsigned int get_32LE();
    unsigned int get_32BE();
    string get_L_STR();
    string get_L_L_STR();
    unsigned int get_PN();
    string get_PSTR();


    oscar_buffer& operator>>(unsigned char&);
    oscar_buffer& operator>>(char&);
    oscar_buffer& operator>>(unsigned short&);
    oscar_buffer& operator>>(short&);
    oscar_buffer& operator>>(unsigned int&);
    oscar_buffer& operator>>(int&);
    oscar_buffer& operator>>(unsigned long&);
    oscar_buffer& operator>>(long&);
    oscar_buffer& operator>>(string&);
    oscar_buffer& operator>>(bool&);
private:
    oscar_buffer& operator>>(double&);
public:
#ifdef WIN32
#endif
};
class
#ifdef WIN32
    __declspec(dllexport)
#endif
    out_oscar_buffer
{
private:
    out_oscar_buffer(const out_oscar_buffer&);             // Not defined to prevent usage
    out_oscar_buffer& operator=(const out_oscar_buffer&);  // Not defined to prevent usage
public:
    unsigned char *buffer;
    size_t bufsize;
    size_t cur_pos;

    out_oscar_buffer();
    ~out_oscar_buffer();
    void Pack(const string& s);
    void Pack(const char * s, size_t len);
    unsigned char *data() {
        return buffer;
    }
    unsigned char *const_data()const {
        return buffer;
    }
    string as_string() const;
    size_t size() const;
    void adjust(size_t n);
    void clear();
    void put_8(unsigned char);
    void put_16LE(unsigned short);
    void put_16BE(unsigned short);
    void put_32LE(unsigned int);
    void put_32BE(unsigned int);
    void put_L_STR(const string &);
    void put_L_L_STR(const string &);
    void put_PN(unsigned int);
    void put_PSTR(const string &);


    out_oscar_buffer& operator<<(const char*);
    out_oscar_buffer& operator<<(unsigned char);
    out_oscar_buffer& operator<<(char);
    out_oscar_buffer& operator<<(unsigned short);
    out_oscar_buffer& operator<<(short);
    out_oscar_buffer& operator<<(unsigned int);
    out_oscar_buffer& operator<<(int);
    out_oscar_buffer& operator<<(unsigned long);
    out_oscar_buffer& operator<<(long);
    out_oscar_buffer& operator<<(const string&);
    out_oscar_buffer& operator<<(const bool&);
private:
    out_oscar_buffer& operator<<(const double&);
public:
#ifdef WIN32
#endif
private:
    void put_8$(unsigned char c) {
        buffer[cur_pos++]=c;
    }
    void put_16LE$(unsigned short l) {
        buffer[cur_pos++]=(l & 0xFF);
        buffer[cur_pos++]=((l>>8) & 0xFF);
    }
    void put_16BE$(unsigned short l) {
        buffer[cur_pos++]=((l>>8) & 0xFF);
        buffer[cur_pos++]=(l & 0xFF);
    }
    void put_32LE$(unsigned int l);
    void put_32BE$(unsigned int l);
    void put_32LE$(int l);
    void put_32BE$(int l);
    void put_32LE$(long l);
    void put_32BE$(long l);
    void put_32LE$(unsigned long l);
    void put_32BE$(unsigned long l);
    void Pack$(const string& s )
    {
        memcpy(&buffer[cur_pos],s.data(),s.size());
        cur_pos+=(int)s.size();
    }
    void put_PN$(unsigned int N);


};
class oscar_utils
{
public:
    static std::string flap(unsigned char channel,const out_oscar_buffer& buf);
    static std::string flap_snac(unsigned char channel,int fam, int type,const out_oscar_buffer& buf);

};
class FS_outV2: public out_oscar_buffer
{
public:
    FS_outV2(unsigned char channel, int fam, int stype);
    void calc_size() const;
private:
    string as_string() const;
};

class F_outV2: public out_oscar_buffer
{
public:
    F_outV2(unsigned char channel);
    void calc_size() const;
private:
    string as_string() const;
};
struct OCALC
{
private:
    OCALC(const OCALC&);             // Not defined to prevent usage
    OCALC& operator=(const OCALC&);  // Not defined to prevent usage
public:
    const out_oscar_buffer & buf;
    OCALC(const FS_outV2&);
    OCALC(const F_outV2&);
    string as_string() const;
};
std::ostream& operator<<(std::ostream&,oscar_buffer&);




template < class T > out_oscar_buffer & operator << (out_oscar_buffer& b,const vector < T > &v) {
    b << (unsigned int)v.size();
#ifdef WIN32
    for (vector < T >::const_iterator j = v.begin(); j != v.end(); j++) b << *j;
#else
    for (auto j = v.begin(); j != v.end(); j++) b << *j;
#endif
    return b;
}

template < class T > out_oscar_buffer & operator << (out_oscar_buffer& b,const list < T > &v) {

    b << (unsigned int)v.size();
#ifdef WIN32
    for (list<T>::const_iterator j = v.begin(); j != v.end(); j++) b << *j;
#else
    for (auto j = v.begin(); j != v.end(); j++) b << *j;
#endif
    return b;
}

template < class T > out_oscar_buffer & operator << (out_oscar_buffer& b,const deque < T > &v) {

    b << (unsigned int)v.size();
#ifdef WIN32
    for (deque<T>::const_iterator j = v.begin(); j != v.end(); j++) b << *j;
#else
    for (auto j = v.begin(); j != v.end(); j++) b << *j;
#endif
    return b;
}
template < class T > out_oscar_buffer & operator << (out_oscar_buffer& b,const set < T > &v) {

    b << (unsigned int)v.size();
#ifdef WIN32
    for (set < T > ::const_iterator j = v.begin(); j != v.end(); j++) b << *j;
#else
    for (auto j = v.begin(); j != v.end(); j++) b << *j;
#endif
    return b;
}
/*template < class T > out_oscar_buffer & operator << (out_oscar_buffer& b,const set < T > &v) {

	b << (unsigned int)v.size();
#ifdef WIN32
	for (set < T > ::const_iterator j = v.begin(); j != v.end(); j++) b << *j;
#else
	for (typeof(v.begin()) j = v.begin(); j != v.end(); j++) b << *j;
#endif
	return b;
}*/

template < class T1, class T2 > out_oscar_buffer & operator << (out_oscar_buffer& b,const map < T1, T2 > &v) {
    b << (unsigned int)v.size();
#ifdef WIN32
    for (map < T1, T2 >::const_iterator j = v.begin(); j != v.end(); j++)
#else
    for (auto j = v.begin(); j != v.end(); j++)
#endif
    {
        b << j->first << j->second;
    }
    return b;
}

template < class T1, class T2 > out_oscar_buffer & operator << (out_oscar_buffer& b,const pair < T1, T2 > &v) {
    b << v.first<<v.second;
    return b;
}


template < class T > oscar_buffer & operator >> (oscar_buffer& b,set < T > &v)
{
    unsigned int n;
    b >> n;
    for (unsigned int j = 0; j < n; j++) {
        T t;
        b >> t;
        v.insert(t);
    }
    return b;
}
/*	template < class T > oscar_buffer & operator >> (oscar_buffer& b,set < T > &v)
	{
		unsigned int n; b >> n;
		for (unsigned int j = 0; j < n; j++){T t; b >> t; v.insert(t);}
		return b;
	}*/
template < class T > oscar_buffer& operator >> (oscar_buffer& b,list < T > &v) {
    unsigned int n;
    b >> n;
    for (unsigned int j = 0; j < n; j++) {
        T t;
        b >> t;
        v.push_back(t);
    }
    return b;
}
template < class T1, class T2 > oscar_buffer & operator >> (oscar_buffer& b,map < T1, T2 > &v) {
    unsigned int n;
    b >> n;
    for (unsigned int j = 0; j < n; j++) {
        T1 t1;
        T2 t2;
        b >> t1;
        b >> t2;
        v.insert(pair<T1,T2>(t1,t2));
    }
    return b;
}
/*	template < class T1, class T2 > oscar_buffer & operator >> (oscar_buffer& b,map < T1, T2 > &v) {
		unsigned int n;	b >> n;
		for (unsigned int j = 0; j < n; j++) {
			T1 t1;
			T2 t2;
			b >> t1;
			b >> t2;
			v.insert(pair<T1,T2>(t1,t2));}
		return b;
	}*/
template < class T1, class T2 > oscar_buffer & operator >> (oscar_buffer& b,pair < T1, T2 > &v) {
    b >> v.first;
    b >> v.second;
    return b;
}
template < class T > oscar_buffer & operator >> (oscar_buffer& b,vector < T > &v) {
    unsigned int n;
    b >> n;
    for (unsigned int j = 0; j < n; j++) {
        T t;
        b >> t;
        v.push_back(t);
    }
    return b;
}

template < class T > oscar_buffer & operator >> (oscar_buffer& b,deque < T > &v) {
    unsigned int n;
    b >> n;
    for (unsigned int j = 0; j < n; j++) {
        T t;
        b >> t;
        v.push_back(t);
    }
    return b;
}
#endif
