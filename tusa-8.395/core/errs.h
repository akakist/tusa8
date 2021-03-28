#ifndef ERRS_Hhh
#define ERRS_Hhh
#include <string>
#ifndef WIN32
#include <unistd.h>
#endif
#include <vector>
#include <stdexcept>

using namespace std;

#define E_NOERR 0
struct sockaddr_in;
struct hstring;

#ifdef DEBUG
#define DBG(a) a
#else
#define DBG(a)
#endif
#define TRACEDBG(a)

#pragma warning( disable:4251)
struct
#ifdef WIN32
__declspec (dllexport)
#endif
cError
{
    std::string 		error_string;
    cError(const char *_message, ...);
    cError(const std::string&);
~cError() {};
const char *what() {
    return error_string.c_str();
}
};
class socketError:public cError
{
public:
    socketError(const std::string s): cError(s) {}

};


#ifdef WIN32
__declspec(dllexport)
#endif
void logErr(const char *fmt,...);
#ifdef WIN32
__declspec(dllexport)
#endif
void logErr2(const char *fmt, ...);
void logErrN(const char *prefix,const char *fmt, ...);

#ifdef WIN32
__declspec(dllexport)
#endif
void logErr4(const char* file,int line,char *fmt, ...);

#ifdef DEBUG
#define LOG4(a) logErr4(__FILE__,__LINE__,a)
#else
#define LOG4(a)
#endif
//}

#ifdef DEBUG
#define _SFL std::string((string )" " __FILE__ " "+itostring(__LINE__))
#else
#define _SFL std::string("")
#endif


#ifdef DEBUG
#	define __FL__	__FILE__,__LINE__
#else
#	define __FL__	"",0
#endif

#define DUMP(a)
#ifdef DEBUG
#define STDCATCHS(SSS)       \
	  catch(socketError e){}catch(cError e) {\
	  logErr2("---Error: '%s' '%s' %s %d", SSS,e.error_string.c_str(),__FILE__,__LINE__);\
      }\
	  catch(logic_error e)\
	  {\
		 logErr2("---Error: logic_error:  '%s' '%s' %s %d", SSS,e.what(),__FILE__,__LINE__);\
	  }\
	  catch(runtime_error e)\
	  {\
		 logErr2("---Error: runtime_error:  '%s' '%s' %s %d", SSS,e.what(),__FILE__,__LINE__);\
	  }\
	  catch(exception e)\
	  {\
		 logErr2("---Error: exception:  '%s' '%s' %s %d", SSS,e.what(),__FILE__,__LINE__);\
	  }\
      catch(...) {\
	 logErr2("---Error: Unknown error: '%s' %s %d",SSS,__FILE__,__LINE__);\
      }
#else
#define STDCATCHS(SSS)       catch(socketError e){} catch(cError e) {\
	 logErr2("---Error: '%s' %s", SSS,e.error_string.c_str());\
      }\
	  catch(logic_error e)\
	  {\
		 logErr2("---Error: logic_error:  '%s' %s", SSS,e.what());\
	  }\
	  catch(runtime_error e)\
	  {\
		 logErr2("---Error: runtime_error:  '%s' %s", SSS,e.what());\
	  }\
	  catch(exception e)\
	  {\
		 logErr2("---Error: exception:  '%s' '%s' %s %d", SSS,e.what(),__FL__);\
	  }\
      catch(...) {\
	 logErr2("---Error: Unknown error: '%s'",SSS);\
      }
#endif


#ifdef DEBUG
#define XTRY try{
#define XPASS } catch(...){logErr2("XPASS %s %d",__FILE__,__LINE__);throw;}
#else
#define XTRY try{
#define XPASS } catch(...){logErr2("XPASS %s %d",__FILE__,__LINE__);throw;}
#endif

#endif
