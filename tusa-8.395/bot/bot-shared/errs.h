#ifndef ERRS_Hhh1
#define ERRS_Hhh1
#include <string>
#ifndef WIN32
#include <unistd.h>
#endif
#include <vector>
#include <stdexcept>
#include "errno.h"
using namespace std;

#define E_NOERR 0
struct sockaddr_in;


void logErr(char *fmt, ...);

struct cError
{
	    enum 
	    {

		E_COMMON			=1,
		E_TIMEOUT			=19,
		E_MYSQL				=20,
		E_FATAL=27	
	    };
	    
	    unsigned char error_code;
	    string error_string;

	    bool fatal;

	    cError(int e, const string &s);
	    cError(const string&);
	    void handle_error();
	    void handle_error(bool fatal);
		const char *what(){return error_string.c_str();}
};

class socketError:public cError
{
public:
	socketError(const string s): cError(s){}

};
/*class mutex_inspector
{
public:
	mutex_inspector(const char* ff, int ll);
	mutex_inspector(const char* ff, int ll,const char *ss);
	~mutex_inspector();
};
#if defined(DEBUG) || defined(__STL_DEBUG)
#define MUTEX_INSPECTOR  mutex_inspector kall12344(__FILE__,__LINE__);
#define MUTEX_INSPECTORS(a)  mutex_inspector kall123444(__FILE__,__LINE__,a);
#else 
#define MUTEX_INSPECTOR
#define MUTEX_INSPECTORS(a)
#endif*/
#ifdef DEBUG
#define XTRY try{
#define XPASS }catch(...)	{logErr("passed here %s %d",__FILE__,__LINE__); throw;}
#else
#define XTRY
#define XPASS
#endif

#define throw_cErrorAPI(a) {string _ss=(string)a+" "+strerror(errno);throw cError(_ss);}
string dump_mutex_inspectors ();


#ifdef DEBUG
std::string dump_mutex_inspectors();
std::string dump_mutex_inspector();
#define _DMI() dump_mutex_inspectors()
#else 
#define _DMI() std::string("")
#endif


#endif

