#ifndef USERCREDITS_H__
#define USERCREDITS_H__
#include "db_var.h"
#include <string>
#include "M_DEQUE.h"
#include "Double.h"

struct credit_transaction
{
    DB_id from_user_id,to_user_id;
    Double summa;
    string operation_code;
    time_t t;
    credit_transaction(const DB_id& _from_user_id,const DB_id& _to_user_id):from_user_id(_from_user_id),to_user_id(_to_user_id) {}
    ~credit_transaction() {}
};

extern M_DEQUE<credit_transaction> credit_transactions;
void dump_credits_log();




bool buy_thing(const DB_id& from_uid,const DB_id&  to_uid,const Double &price, const string &opcode, Double& reminder_from, Double &reminder_to); // true if success
bool inc_credit_summa(const DB_id& to_uid,const Double summa, const string &opcode);




#endif
