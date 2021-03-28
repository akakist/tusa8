#ifndef THREAD_DEESCR__H
#define THREAD_DEESCR__H
void set_thread_description(const string &descr);
string get_thread_description();

struct st_thread_description
{
    string old_descr;
    st_thread_description(const string &s)
    {
        old_descr=get_thread_description();
        set_thread_description(s);
    }
    ~st_thread_description()
    {
        set_thread_description(old_descr);
    }
};
#endif
