/*
core
(c) belialov.com
*/
#ifndef ST_STUFF_H
#define ST_STUFF_H
#ifndef WIN32
#include <zlib.h>
#endif
#include "errs.h"
#include "str_lib.h"
#include <stdio.h>
#include <set>
#include <map>
#include <list>
#include <deque>
using namespace std;
class st_stream
{
private:
		st_stream(const st_stream&);             // Not defined
		st_stream& operator=(const st_stream&);  // Not defined

protected:
	void *f;
	string pathname;
public:	 
	st_stream(){}
	void read (void *, int );
	void write (const void *, int ) const ;
	virtual int readsome (void *, int );
	virtual int writesome (const void *, int ) const;
	void write(const string & buf)
	{
		write((const void*)buf.c_str(), buf.length());
	}
	
	string get_pathname(){return pathname;}
	void puts(const string &s);
	void puts(const char *);
	virtual	const st_stream & operator<<(const string& s) const;
	virtual	const st_stream & operator<<(const char *s) const;	
	virtual	const st_stream & operator<<(unsigned n) const;			

	st_stream& operator>=(string &s);
	st_stream& operator>=(unsigned int &s);
	st_stream& operator>=(int &s);
	st_stream& operator>=(bool &s);
	st_stream& operator>=(time_t &s);
	
	const st_stream & operator<=(const string& s) const;
	const st_stream & operator<=(const char *s) const;	
	const st_stream & operator<=(unsigned int n) const;			
	const st_stream & operator<=(int n) const;			
	const st_stream & operator<=(bool n) const;			
	const st_stream & operator<=(time_t n) const;			

template <class T>
	
	const st_stream & operator<=(const vector<T> &v) const
	{
		*this<=v.size();
		for(unsigned int j=0;j<v.size();j++)
		{
			*this<=v[j];
		}
		return *this;
	}			
template <class T>
	st_stream& operator>=(vector<T> &v)
	{
		unsigned int n;
		*this>=n;
		v.clear();
		for(unsigned int j=0;j<n;j++)
		{
			T t;
			*this>=t;
			v.push_back(t);
		}
		return *this;
	}
	virtual ~st_stream(){}
};


class st_macFile:public st_stream
{
private:
		st_macFile(const st_macFile&);             // Not defined
		st_macFile& operator=(const st_macFile&);  // Not defined
public:		
	short frefnum;
	void _FSOpen_(const string &pn,short vRefNum,short* fRefNum);
	void _FSRead_(short fRefNum,long * inOutCount,char* buffer);
	void _FSWrite_(short fRefNum,long * inOutCount,const char* buffer) const;
	void _GetEOF_(short fRefNum,long * inOutCount);
	void _FSClose_(short fRefNum);
	void _OpenRF_(const string &pn,short vRefNum,short* fRefNum);
	public:
	static void _Create_(const string&pn,short vRefNum,unsigned int fdCreator,unsigned int fdType);
	static void _CreateResFile_(const string &pn);
	st_macFile(){f=NULL;}
	void openFS(const string &pn);
	void openRF(const string &pn);
 	int readsome (void *, int );
    int writesome (const void *s2, int n) const;
    long size();
   virtual ~st_macFile();
};
class st_gzFile: public st_stream 
{
private:
		st_gzFile(const st_gzFile&);             // Not defined
		st_gzFile& operator=(const st_gzFile&);  // Not defined

public:
     st_gzFile (){f=NULL;};
     st_gzFile (const string &pn, const string &mode);
    void open(const string &pn, const string &mode);
  virtual ~st_gzFile ();
 	   int readsome (void *, int );
       int writesome (const void *s2, int n) const ;
       char *gets (char *s, int max);
       char *getssome (char *s, int max);
};
class st_FILE: public st_stream 
{
private:
		st_FILE(const st_FILE&);             // Not defined
		st_FILE& operator=(const st_FILE&);  // Not defined

public:
	st_FILE (){f=NULL;};
	st_FILE (const string &pn, const string &mode);
	void open(const string &pn, const string &mode);
	virtual  ~st_FILE ();
	int readsome (void *, int );
	int writesome (const void *s2, int n) const ;
	char *gets (char *s, int max);
	char *getssome (char *s, int max);
	void seek(long offset);
	FILE *get_descriptor(){return (FILE*)f;}
};
struct st_malloc
{
private:
		st_malloc(const st_malloc&);             // Not defined
		st_malloc& operator=(const st_malloc&);  // Not defined
public:
    void *buf;
    st_malloc(unsigned long size);
    ~st_malloc();
};

struct st_gdbm {
    string pathname;
     st_gdbm (const string &pn);
    ~st_gdbm ();
    void store(const string&, const string&);
    string fetch(const string &key);
    map<string,string> container;
    
};
struct st_save_cwd
{
	string cwd;
	st_save_cwd();
	~st_save_cwd();
};
struct st_GUSIHookDisable
{
	st_GUSIHookDisable();
	~st_GUSIHookDisable();	
	void (*savehook)();
};

#endif


