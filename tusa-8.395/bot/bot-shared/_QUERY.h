#ifndef QUERY__HH4
#define QUERY__HH4
#include <vector>
#include <string>
#include <set>
#include <map>
using namespace std;
class QUERY
{
private: 
    vector<string> container;
public:
    void reset_indexes() const;
	void clear(){container.clear();}
    QUERY();
    ~QUERY();
    QUERY(const string&s);
    QUERY(const char* s);

    QUERY& operator>>(string &s);
    QUERY & operator<<(const string& s);
    QUERY & operator<<(unsigned long d);
    QUERY & operator<<(double d);
    QUERY & operator<<(const char *s);
    QUERY & operator<<(unsigned int d);
    QUERY & operator<<(int d);
    QUERY & operator<<(bool d);
    QUERY & operator<<(time_t d);
    QUERY & operator<<(const set<unsigned int>& s/*setime_t d*/);
    QUERY & operator<<(const map<unsigned int,double>&s);
    string prepare();
};
#endif
