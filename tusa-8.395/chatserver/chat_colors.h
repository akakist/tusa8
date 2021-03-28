#ifndef CHAT_COLORS_H
#define CHAT_COLORS_H


class __c_pack: public base_container
{
public:
    const char* getclassname()const {
        return "c_pack";
    }

    __c_pack():base_container("c_pack") {}


    string bnick;
    string bnick2;
    string makeit;
    string nick;
    string privfrom;
    string privto;
    string stdfrom;
    string std;
    string stdto;
    string sys;
    string syspriv;
    int id;
    string name;
    string nickclass;
    string nickclass_birthdate;
    string _conffile;
};
struct __c_packs
{
    _mutex Mutex;
    map<unsigned int,PTR_getter<__c_pack> > container;
public:
    void insert(unsigned int id,const PTR_getter<__c_pack> &c);
    PTR_getter<__c_pack> get(unsigned int id);
    unsigned int get_first_id();
    int count(unsigned int n);
    __c_packs():Mutex("__c_packs") {}
};
extern __c_packs c_packs;


string _nick$(const user_nick &s,const bool priv,const PTR_getter<__c_pack> &color_pack );
string _badnick$(const string &s,const PTR_getter<__c_pack> &color_pack );
string _badnick$2(const user_nick &s,const PTR_getter<__c_pack> &color_pack );


#endif
