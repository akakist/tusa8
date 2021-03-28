#ifndef DES_SID_HHH
#define DES_SID_HHH
#include "sid_t.h"
#include <string>
using namespace std;
string get_des_hex_sid(sid_t & sid);
int /*no err=0*/ parse_des_hex_sid(const string &str,sid_t &sid);

#endif
