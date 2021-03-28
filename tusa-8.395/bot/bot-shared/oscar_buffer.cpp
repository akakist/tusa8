#ifdef __GNUC__
#pragma implementation				// gcc: implement sql_udf.h
#endif

//#include "stdafx.h"
#include "oscar_buffer.h"
//#include "file_set.h"
#include <map>
#include <string>
//#include "str_lib.h"
//#include "mutex_inspector.h"

using namespace std;
oscar_buffer::oscar_buffer() : endn(BIG), out_pos(0), data() { }

oscar_buffer::oscar_buffer(const unsigned char* d, int size) : endn(BIG), out_pos(0), data(d, d+size) { }


unsigned char& oscar_buffer::operator[](unsigned int p) {
  return data[p];
}

void oscar_buffer::clear() {
  data.clear();
  out_pos = 0;
}

bool oscar_buffer::empty() {
  return data.empty();
}

void oscar_buffer::chopOffoscar_buffer(oscar_buffer& b, unsigned int sz) {
  copy( data.begin(), data.begin()+sz, back_inserter(b.data) );
  data.erase( data.begin(), data.begin()+sz );
  out_pos = 0;
}

void oscar_buffer::Pack(const unsigned char *d, int size) {
  copy(d, d+size, back_inserter(data));
}

void oscar_buffer::Pack(const string& s) {
  copy(s.begin(), s.end(), back_inserter(data));
}

void oscar_buffer::Unpack(string& s, unsigned int size) {
  if (size > data.size()-out_pos) size = data.size()-out_pos;
  copy(data.begin()+out_pos, data.begin()+out_pos+size, back_inserter(s));
  out_pos += size;
}
void oscar_buffer::Unpack_const(string& s, unsigned int size) const{
  if (size > data.size()-out_pos) size = data.size()-out_pos;
  copy(data.begin()+out_pos, data.begin()+out_pos+size, back_inserter(s));
//  out_pos += size;
}

// -- Input stream methods --

oscar_buffer& oscar_buffer::operator<<(unsigned char l) {
  data.push_back(l);
  return (*this);
}
oscar_buffer& oscar_buffer::operator<<(bool v)
{
 	data.push_back((unsigned char)v);
	return (*this);
}
oscar_buffer& oscar_buffer::operator>>(bool& v)
{
	unsigned char l;
    if (out_pos + 1 > data.size()) l = 0;
  else l = data[out_pos++];
  v=l;
  return (*this);
}


oscar_buffer& oscar_buffer::operator<<(unsigned short l) {
  if (endn == BIG) {
    data.push_back((l>>8) & 0xFF);
    data.push_back(l & 0xFF);
  } else {
    data.push_back(l & 0xFF);
    data.push_back((l>>8) & 0xFF);
  }    
  return (*this);
}

oscar_buffer& oscar_buffer::operator<<(unsigned int l) {
  if (endn == BIG) {
    data.push_back((l >> 24) & 0xFF);
    data.push_back((l >> 16) & 0xFF);
    data.push_back((l >> 8) & 0xFF);
    data.push_back(l & 0xFF);
  } else {
    data.push_back(l & 0xFF);
    data.push_back((l >> 8) & 0xFF);
    data.push_back((l >> 16) & 0xFF);
    data.push_back((l >> 24) & 0xFF);
  }
  return (*this);
}
oscar_buffer& oscar_buffer::operator<<(datetime l) {
	if(sizeof(l)==sizeof(unsigned int)*2)
	{
   unsigned int *p=( unsigned int *)&l;
   unsigned int l0=p[0],l1=p[1];
   (*this)<<l0<<l1;
   }
   else 
   {	
   	unsigned int l3=0;
	   (*this)<<l3<<l3;
   	//logErr2("--Error: sizeof(l)!=sizeof(unsigned int)*2");
   }
  return (*this);
}
oscar_buffer& oscar_buffer::operator<<(long int l) {
  if (endn == BIG) {
    data.push_back((l >> 24) & 0xFF);
    data.push_back((l >> 16) & 0xFF);
    data.push_back((l >> 8) & 0xFF);
    data.push_back(l & 0xFF);
  } else {
    data.push_back(l & 0xFF);
    data.push_back((l >> 8) & 0xFF);
    data.push_back((l >> 16) & 0xFF);
    data.push_back((l >> 24) & 0xFF);
  }
  return (*this);
}


// -- Output stream methods --

oscar_buffer& oscar_buffer::operator>>(unsigned char& l) {
  if (out_pos + 1 > data.size()) l = 0;
  else l = data[out_pos++];
  return (*this);
}

oscar_buffer& oscar_buffer::operator>>(unsigned short& l) {
  if (out_pos + 2 > data.size()) {
    l = 0;
    out_pos += 2;
  } else {
    if (endn == BIG) {
      l = ((unsigned short)data[out_pos++] << 8)
	+ ((unsigned short)data[out_pos++]);
    } else {
      l = ((unsigned short)data[out_pos++])
	+ ((unsigned short)data[out_pos++] << 8);
    }
  }
  return (*this);
}

oscar_buffer& oscar_buffer::operator>>(unsigned int& l) {
  if (out_pos + 4 > data.size()) {
    l = 0;
    out_pos += 4;
  } else {
    if (endn == BIG) {
      l = ((unsigned int)data[out_pos++] << 24)
	+ ((unsigned int)data[out_pos++] << 16)
	+ ((unsigned int)data[out_pos++] << 8)
	+ ((unsigned int)data[out_pos++]);
    } else {
      l = ((unsigned int)data[out_pos++])
	+ ((unsigned int)data[out_pos++] << 8)
	+ ((unsigned int)data[out_pos++] << 16)
	+ ((unsigned int)data[out_pos++] << 24);
    }
  }
  return (*this);
}
oscar_buffer& oscar_buffer::operator>>(datetime & l) {
	if(sizeof(l)==sizeof(unsigned int)*2)
	{
   unsigned int l0,l1;
   (*this)>>l0>>l1;
   unsigned int *p=( unsigned int *)&l;
   p[0]=l0;
   p[1]=l1;
   }
   else l=0;
  return (*this);
}
oscar_buffer& oscar_buffer::operator>>(long int& l) {
  if (out_pos + 4 > data.size()) {
    l = 0;
    out_pos += 4;
  } else {
    if (endn == BIG) {
      l = ((unsigned int)data[out_pos++] << 24)
	+ ((unsigned int)data[out_pos++] << 16)
	+ ((unsigned int)data[out_pos++] << 8)
	+ ((unsigned int)data[out_pos++]);
    } else {
      l = ((unsigned int)data[out_pos++])
	+ ((unsigned int)data[out_pos++] << 8)
	+ ((unsigned int)data[out_pos++] << 16)
	+ ((unsigned int)data[out_pos++] << 24);
    }
  }
  return (*this);
}

void oscar_buffer::setEndianness(endian e) {
  endn = e;
}

#ifndef WIN32
void oscar_buffer::dump(std::ostream& out) {
  char d[] = "123456789abcdef0";
#ifndef WIN32
  out << hex << setfill('0');
#endif
  unsigned int m = ((data.size()+15)/16)*16;
  for (unsigned int a = 0; a < m; a++) {
    if (a % 16 == 0) out << setw(4) << a << "  ";
    if (a < data.size()) {
      out << setw(2) << (int)data[a] << " ";
      if (data[a] < 32) d[a%16] = '.';
      else d[a%16] = data[a];
    } else {
      out << "   ";
      d[a%16] = ' ';
    }
    if (a % 16 == 15){ out << (string)" " 
		<< d 
		<< "\n"/*endl*/;
	}
  }
}
#else
void oscar_buffer::dump(std::ostream& out) {
}
#endif

  string oscar_buffer::as_string() const
  {


  	string s;
  	Unpack_const(s,size());
	return s;
  }

#ifndef WIN32
std::ostream& operator<<(std::ostream& out, oscar_buffer& b) { b.dump(out); return out; }
#endif
oscar_buffer & operator<< (oscar_buffer & b, const oscar_buffer::NTS & nts) {nts.Output(b);	return b;}
oscar_buffer & operator<< (oscar_buffer & b, const oscar_buffer::LNTS & lnts) {lnts.Output(b);	return b;}
oscar_buffer & operator<< (oscar_buffer & b, const oscar_buffer::L_STR & lnts) {lnts.Output(b);	return b;}
oscar_buffer & operator<< (oscar_buffer & b, const oscar_buffer::L_L_STR & lnts) {lnts.Output(b);	return b;}
oscar_buffer & operator<< (oscar_buffer & b, const oscar_buffer::COLOR & lnts){lnts.Output(b);return b;}
oscar_buffer & operator<< (oscar_buffer & b, const oscar_buffer::BNTS & bnts) {bnts.Output(b);return b;}
oscar_buffer & operator<< (oscar_buffer & b, const oscar_buffer::oscar_STRING & lnts) {lnts.Output(b);return b;}
oscar_buffer & operator<< (oscar_buffer & b, const oscar_buffer::B_UIN & lnts) {lnts.Output(b);return b;}
oscar_buffer::LNTS::LNTS(const string & s){	container = s;}
oscar_buffer::L_STR::L_STR(const string & s){	container = s;}
oscar_buffer::L_L_STR::L_L_STR(const string & s){	container = s;}
oscar_buffer::L_STR::L_STR(const char* s){	container = s;}
oscar_buffer::L_L_STR::L_L_STR(const char* s){	container = s;}
oscar_buffer::BNTS::BNTS(const string & s){	container = s;}
void oscar_buffer::LNTS::Output(oscar_buffer & b) const
{
	WORD_LE sz(container.size() + 1);
	b << sz;
	b.Pack(container);
	b << (unsigned char)0;
}
void oscar_buffer::L_STR::Output(oscar_buffer & b) const
{
	WORD_LE sz(container.size());
	b << sz;
	b.Pack(container);
}
void oscar_buffer::L_L_STR::Output(oscar_buffer & b) const
{
	DWORD_LE sz(container.size());
	b << sz;
	b.Pack(container);
}
void oscar_buffer::BNTS::Output(oscar_buffer&b) const
{
	WORD_BE sz(container.size()+1);
	b<<sz;
	b.Pack(container);
	b<<(unsigned char)0;
}
oscar_buffer::NTS::NTS(const string & s){	container = s;}
oscar_buffer::NTS & oscar_buffer::NTS::operator= (const string & s) {	container = s;	return *this;}
oscar_buffer::NTS::NTS(){}
void oscar_buffer::NTS::Output(oscar_buffer & b) const{	b.Pack(container);	b << (unsigned char)0;}
//oscar_buffer::B_UIN::B_UIN(unsigned int s){container = itostring(s);}
//oscar_buffer::B_UIN & oscar_buffer::B_UIN::operator= (unsigned int s) {container = itostring(s);return *this;}
void oscar_buffer::B_UIN::Output(oscar_buffer & b) const {unsigned char c = container.size();b << c;b.Pack(container);}
oscar_buffer::B_UIN::B_UIN(){}

void oscar_buffer::oscar_STRING::Output(oscar_buffer & b) const{b.Pack(container);}
oscar_buffer::oscar_STRING & oscar_buffer::oscar_STRING::operator=(const string & s) {	container = s;return *this;}
oscar_buffer::oscar_STRING::oscar_STRING(){}
oscar_buffer::oscar_STRING::oscar_STRING(const string & s){container = s;}
oscar_buffer & operator<< (oscar_buffer & b, const tlv & obj) 
{
	b << obj.number;
	unsigned short sz = obj.size();
	b << sz;
//	deque < unsigned char >v = obj.getData();
	for (unsigned i = 0; i < obj.data.size(); i++)
		b << obj.data[i];
	return b;
}
oscar_buffer & operator>> (oscar_buffer & b, tlv & obj) 
{
	obj.clear();
	b >> obj.number;
	unsigned short sz;
	b >> sz;
	for (unsigned i = 0; i < sz; i++) {
		unsigned char c;
		b >> c;
		obj << c;
	}
	return b;
}

void oscar_buffer::LNTS::Input(oscar_buffer & b)
{
	container = "";
	WORD_LE sz;
	b >> sz;
	for (int i = 0; i < sz.obj && b.beforeEnd(); i++) {
		unsigned char c;
		b >> c;
		if (/*c != 0 && */i != sz.obj - 1)
			container += string((char *)&c, 1);
	}
}
void oscar_buffer::L_STR::Input(oscar_buffer & b)
{
	container = "";
	WORD_LE sz;
	b >> sz;
	for (int i = 0; i < sz.obj && b.beforeEnd(); i++) {
		unsigned char c;
		b >> c;
		
//		if (/*c != 0 && */i != sz.obj - 1)
		container += string((char *)&c, 1);
	}
}
void oscar_buffer::L_L_STR::Input(oscar_buffer & b)
{
	container = "";
	DWORD_LE sz;
	b >> sz;
	for (int i = 0; i < sz.obj && b.beforeEnd(); i++) {
		unsigned char c;
		b >> c;
		
//		if (/*c != 0 && */i != sz.obj - 1)
		container += string((char *)&c, 1);
	}
}
void oscar_buffer::BNTS::Input(oscar_buffer & b)
{
	container = "";
	WORD_BE sz;
	b >> sz;
	for (int i = 0; i < sz.obj && b.beforeEnd(); i++) {
		unsigned char c;
		b >> c;
		if (/*c != 0 &&*/ i != sz.obj - 1)
			container += string((char *)&c, 1);
	}
}
void oscar_buffer::oscar_STRING::Input(oscar_buffer & b)
{
	container = "";
	for (unsigned i = 0; b.beforeEnd(); i++) {
		unsigned char c;
		b >> c;
		container += string((char *)&c, 1);
	}
}
void oscar_buffer::B_UIN::Input(oscar_buffer & b)
{
	container = "";
	unsigned char sz;
	b >> sz;
	for (unsigned i = 0; i < sz && b.beforeEnd(); i++) {
		unsigned char c;
		b >> c;
		container += string((char *)&c, 1);
	}
}
void oscar_buffer::NTS::Input(oscar_buffer & b)
{
	container = "";
	while (b.beforeEnd()) {
		unsigned char c;
		b >> c;
		if (c != 0)
			container += string((char *)&c, 1);
		if (c == 0)
			break;
	}
}
oscar_buffer& operator>>(oscar_buffer& b, oscar_buffer::LNTS& obj){ obj.Input(b); return b; }
oscar_buffer& operator>>(oscar_buffer& b, oscar_buffer::L_STR& obj){ obj.Input(b); return b; }
oscar_buffer& operator>>(oscar_buffer& b, oscar_buffer::L_L_STR& obj){ obj.Input(b); return b; }
oscar_buffer& operator>>(oscar_buffer& b, oscar_buffer::BNTS& obj) { obj.Input(b); return b; }
oscar_buffer& operator>>(oscar_buffer& b, oscar_buffer::NTS& obj) { obj.Input(b); return b; }
oscar_buffer& operator>>(oscar_buffer& b, oscar_buffer::oscar_STRING& obj) { obj.Input(b); return b; }
oscar_buffer& operator>>(oscar_buffer& b, oscar_buffer::B_UIN& obj) { obj.Input(b); return b; }
oscar_buffer& operator>>(oscar_buffer& b, oscar_buffer::COLOR& obj) { obj.Input(b); return b; }

oscar_buffer& operator>>(oscar_buffer & b, map<unsigned short,in_tlv>   &tlvs)
{
	while(b.beforeEnd())
	{
		in_tlv t;
		b>>t;
		tlvs[t.number]=t;
	}
	 return b;
} 


oscar_buffer & operator<< (oscar_buffer & b,  const oscar_buffer  &n)
{
//	deque<unsigned char> tmp=n.getData();
	for(unsigned int i=0;i<n.data.size();i++)
	{
		b<<n.data[i];
	}
	return b;
}
oscar_buffer & oscar_buffer::operator<<(char c)
{
	*this<<(unsigned char) c;
	return *this;
}
oscar_buffer & oscar_buffer::operator>>(char & c)
{
	unsigned char u;
	*this>>u;
	c=(char)u;
	return *this;
}
oscar_buffer & oscar_buffer::operator<<(unsigned long c)
{
	*this<<(long) c;
	return *this;
}
oscar_buffer & oscar_buffer::operator>>(unsigned long & c)
{
	long u;
	*this>>u;
	c=(unsigned long)u;
	return *this;
}
oscar_buffer & oscar_buffer::operator>>(signed int & c)
{
	unsigned int u;
	*this>>u;
	c=(signed int)u;
	return *this;
}

string chk_str_size(const string &s,unsigned int size)
{
	if(s.size()>size) return s.substr(0,size);
	return s;
}

out_oscar_buffer snac(	unsigned short fam,unsigned short stype,const out_oscar_buffer &b)
{
	
    unsigned char flagA=0;
    unsigned char flagB=0;
    unsigned int request_id=0;

    out_oscar_buffer out;
    out<<fam<< stype<<flagA<<flagB<<request_id;

//    deque<unsigned char>v=b.getData();
    for(unsigned int i=0;i<b.data.size();i++)
    {
        out<<b.data[i];
    }
    return out;
}
out_oscar_buffer flap(unsigned char channel,unsigned short seq_num, const out_oscar_buffer &b)
{
	out_oscar_buffer out;
	out<<(unsigned char)OSCAR_START_BYTE;
	out<<(unsigned char)channel;
	out<< (unsigned short)seq_num;
	OSCAR_DATA_LEN_T d=b.size();
	out<<d;
//	deque<unsigned char>v=b.getData();
	for(unsigned int i=0;i<b.data.size();i++)
	{
		out<<b.data[i];
	}
	return out;
}
out_oscar_buffer snac(	unsigned short fam,	unsigned short stype,unsigned char flagA,unsigned char flagB,unsigned int request_id,const out_oscar_buffer &b)
{
	out_oscar_buffer out;
	out	<<fam<< stype
//#ifndef __CAMFROG__
		<< flagA<<flagB
//#endif		
		<<request_id;


//	deque<unsigned char>v=b.getData();
	for(unsigned int i=0;i<b.data.size();i++)
	{
		out<<b.data[i];
	}
	return out;
}
out_oscar_buffer snac(unsigned short fam, unsigned short stype,unsigned int request_id,const out_oscar_buffer &b)
{
	out_oscar_buffer out;
	out<<fam<< stype;
	unsigned short flags = 0;
//#ifndef __CAMFROG__
	out << flags;
//#endif	
	out << request_id;
//	deque<unsigned char>v=b.getData();
	for(unsigned int i=0;i<b.data.size();i++)
	{
		out<<b.data[i];
	}
	return out;
}
out_oscar_buffer flap_seq0(unsigned char channel, const out_oscar_buffer &b)
{
	out_oscar_buffer out;
	out<<(unsigned char)OSCAR_START_BYTE;

	out<<(unsigned char)channel;
	out<< (unsigned short)0;
	OSCAR_DATA_LEN_T d=b.size();
	out<<d;
//	deque<unsigned char>v=b.getData();
	for(unsigned int i=0;i<b.data.size();i++)
	{
		out<<b.data[i];
	}
	return out;
}
