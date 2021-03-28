#ifndef CM_CONTAINER___H
#define CM_CONTAINER___H

class cm_container
{
    _mutex Mutex;
    vector< vector<string> > container;
public:
    m_var<string> _name;
    string get(unsigned int g,unsigned int n);
    string get_random(unsigned int g);
    unsigned int get_random_n(unsigned int g);
    size_t size();
    void assign(const vector< vector<string> >& v);
    void clear();
    void check();
    void assign_ck(const vector< vector<string> > &v);
    cm_container(const char* name):_name(name),Mutex("cm_container") {}
};


extern	cm_container msgs_defstatus;
extern	cm_container msgs_greeting;
extern	cm_container msgs_bye;
extern	cm_container msgs_kicking;
extern	cm_container msgs_killing;
extern	cm_container msgs_changes;
extern	cm_container msgs_nickchanges;
extern	cm_container msgs_topicchanges;
extern	cm_container msgs_modechanges;
extern	cm_container msgs_maty;

#endif
