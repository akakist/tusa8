#ifndef HTML_OUT___H
#define HTML_OUT___H

enum
{
    P_STRING 	=1,
    P_PAIR_S_HS	=2,
    P_FREP	=4
};
struct hstring;
struct htmlout_entry: public base_container_nm
{
    const char* getclassname()const {
        return "htmlout_entry";
    }
    unsigned int type;
    ~htmlout_entry();
    htmlout_entry(unsigned int c);
    void * data;
};
struct hstring;
struct FREP
{
    const char *ptr;
    FREP(const char *s):ptr(s) {}
};
struct hstring
{

public:
    deque<PTR_getter_nm<htmlout_entry> > container;

    hstring& operator<<(const FREP&);
    hstring& operator<<(const char*);
    hstring& operator<<(const string&);
    hstring& operator<<(const hstring&);


    hstring& operator+=(const FREP&);
    hstring& operator+=(const string&);
    hstring& operator+=(const char* );
    hstring& operator+=(const hstring&);
    hstring& operator+=(const pair<map<string,hstring> ,string>&p)
    {
        htmlout_entry *e = new htmlout_entry(P_PAIR_S_HS);
        LEAKCTL_ADD(e);
        *((pair<map<string,hstring>, string>*)e->data)=p;
        container.push_back(e);
        return *this;
    }


    hstring& operator=(const FREP&);
    hstring& operator=(const char*);
    hstring& operator=(const string&);
    hstring& operator=(const hstring&);
    hstring& operator=(const pair<map<string,hstring> ,string>&p)
    {
        container.clear();
        htmlout_entry *e = new htmlout_entry(P_PAIR_S_HS);
        LEAKCTL_ADD(e);
        *((pair<map<string,hstring>, string>*)e->data)=p;
        container.push_back(e);

        return *this;
    }


    hstring(const FREP&);
    hstring(const hstring&);
    hstring(const string&);
    hstring(const char*);
    hstring(const pair<map<string,hstring> ,string>&);

    hstring();
    size_t hstr_size();
    string as_string() const;
    string dump();
};
class out_oscar_buffer;
class oscar_buffer;
out_oscar_buffer& operator<<(out_oscar_buffer &, const hstring&);
oscar_buffer& operator>>(oscar_buffer &, hstring&);

#endif
