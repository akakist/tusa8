#include "stdafx.h"
#include <stdio.h>
#include "c_conn.h"
#include "errs.h"
#include "config.h"
#include "utils.h"
#include "misc.h"
#include "mysql_extens.h"
#include <vector>
#include "roulette.h"
#include "antiflood.h"
#include "oscar_buffer.h"
#include "mutex_inspector.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/time.h>
#include "game.h"

#define ACCEPTING_STAKES_TIME 60
#define STAKES_STOPPED_TIME 5
#define RUNNING_WHEEL_TIME 5
#define THROW_BALL_TIME 5
#define NEXT_GAME_TIME 7
#define HINT_1_TIME	15

time_t next_stoptime=0;

bool game_started=false;
time_t next_game_time=0;
time_t next_hint_time=0;
unsigned int rnd=0;

bool hint1=false;
bool hint2=false;
enum GAME_STATE{
    STATE_UNDEF=0,
    ACCEPTING_STAKES=1,
    STAKES_STOPPED,
    RUNNING_WHEEL,
    THROW_BALL,
    WAIT_NEXT_GAME,
    RETURNING_VINS

};
GAME_STATE game_state=STATE_UNDEF;
int stakes_count=0;

vector<string> hints_ACCEPTING_STAKES;
unsigned int __id=0;

map<unsigned int,string> cttu_nicks;
unsigned int next_id()
{
    return ++__id;
}
struct stawka
{
    string login;
    double summa;
    string tip;
    int number;
};
map<string/*login*/,vector<stawka> > __stawki;
void load_stawki()
{
    if (__stawki.size()==0)
    {
        //FILE *f=fopen(FSTAWKI,"rb");
        struct stat st;
        if (stat(FSTAWKI,&st))
        {
            logErr("failed to stat " FSTAWKI);
            return;
        }
        FILE *f=fopen(FSTAWKI,"rb");
        if (f)
        {
            char buff[st.st_size+1000];
            memset(buff,0,sizeof(buff));
            fread(buff,1,st.st_size,f);
            fclose(f);
            vector<string> v=splitTOK("\r\n",buff);
            for (unsigned i=0;i<v.size();i++)
            {
                vector<string> w=splitTOK(" ",v[i]);
                if (w.size()==4)
                {
                    stawka s;
                    s.login=w[0];
                    s.summa=atof(w[1].c_str());
                    s.tip=w[2];
                    s.number=atoi(w[3]);
                    __stawki[s.login].push_back(s);
                }
            }
        }
        else
            logErr("failed fopen " FSTAWKI);

    }
}
vector<string> get_stawki_logins()
{
    vector<string> r;
    for (typeof(__stawki.begin())i=__stawki.begin();i!=__stawki.end();i++)
        r.push_back(i->first);

    return r;

}
map<string/*login*/,vector<stawka> > get_stawki()
{
    load_stawki();
    return __stawki;
}
void add_stawka(const string& login,double summa,const string& tip,int number)
{
    load_stawki();
    stawka s;
    s.login=login;
    s.summa=summa;
    s.tip=tip;
    s.number=number;
    __stawki[login].push_back(s);
    FILE *f=fopen(FSTAWKI,"ab");
    if (f)
    {
        fprintf(f,"%s %f %s %d\n",login.c_str(),summa,tip.c_str(),number);
        fclose(f);
    }
    else
        logErr("failed fopen " FSTAWKI);
}
void remove_stawki()
{
    if (unlink(FSTAWKI)) logErr("unlink stawki error ");
    __stawki.clear();
}


unsigned int get_random()
{

    FILE *f=fopen("/dev/urandom","rb");
    if (f==NULL)
    {
        logErr("failed open urandom");
        exit(1);
    }
    unsigned int n[1024];
    fread(&n,1,sizeof(rand()%100+4),f);
    fclose(f);
//	srand(r);
    unsigned int r=n[0]+rnd;
    srand(r);
    return r;
}
void mul_kkk(double& v, const string &sm)
{
    int nk=0;
    for (unsigned i=0;i<sm.size();i++)
    {
        if (
            sm[i]=='k' || sm[i]=='K'||
            sm[i]=='к' || sm[i]=='К'
        )
            nk++;
    }
    switch (nk)
    {
    case 1:
        v*=1000;break;
    case 2:
        v*=1000000;break;
    case 3:
        v*=1000000000.0;break;
    case 4:
        v*=1000000000000.0;break;
    }
}
string fmtstr(cMYSQL *dbh,double d)
{
    return dbh->select_1Q((QUERY)"select FORMAT(?,2)"<<d);

    //for()

    string s=get_f22(d);
//	bool dpt;
    for (unsigned i=0;i<s.size();i++)
    {

    }
    return s;
    if (d>=1000000000000LL)
    {
        char s[200];
        snprintf(s,sizeof(s),"%2.2fKKKK",d/1000000000000LL);
        return s;
    }
    if (d>=1000000000L)
    {
        char s[200];
        snprintf(s,sizeof(s),"%2.2fKKK",d/1000000000L);
        return s;
    }
    if (d>=1000000L)
    {
        char s[200];
        snprintf(s,sizeof(s),"%2.2fKK",d/1000000);
        return s;
    }
    if (d>=1000)
    {
        char s[200];
        snprintf(s,sizeof(s),"%2.2fK",d/1000);
        return s;
    }
    else
    {
        char s[200];
        snprintf(s,sizeof(s),"%2.2f",d);
        return s;
    }
}

void check_state()
{MUTEX_INSPECTOR;
    XTRY
    time_t t=time(NULL);
    if (game_started){ MUTEX_INSPECTOR;

        if (game_state==WAIT_NEXT_GAME && time(NULL)>next_stoptime && next_stoptime!=0)
        {
            action("Начинается новая игра... Делаем ставки!");
            next_stoptime=0;
            game_state=STATE_UNDEF;
            stakes_count=0;
            next_hint_time=0;
        }
        if (game_state==THROW_BALL && time(NULL)>next_stoptime && next_stoptime!=0)
        {
            int no=get_random()%37;
            string no_s;
            if (no!=0)
                no_s=itostring(no);
            else no_s="ЗЕРО";
            action("Выпалo  <B>"+no_s+"</B>");
            bool is_zero=no==0;
            bool is_odd=no%2==1;
            bool is_even=no%2==0;
            bool is_1p=no>=1 && no<=18;
            bool is_2p=no>=19 && no<=36;
            bool is_1d=no>=1 && no<=12;
            bool is_2d=no>=13 && no<=24;
            bool is_3d=no>=25 && no<=36;

            if (is_zero)
            {
                is_odd=false;
                is_even=false;
                is_1p=false;
                is_2p=false;
                is_1d=false;
                is_2d=false;
                is_3d=false;

            }
            map<string,vector<stawka> > st=get_stawki();
            map<string,double> tot_stawki;
            map<string,double> tot_wins;
            for (typeof(st.begin()) i=st.begin();i!=st.end();i++)
            {
                for (unsigned j=0;j<i->second.size();j++)
                {
                    stawka &s=i->second[j];
                    user* u=get_user(s.login);
                    if (u)
                    {
                        if (tot_stawki.count(s.login)) tot_stawki[s.login]+=s.summa;
                        else tot_stawki.insert(make_pair(s.login,s.summa));

                        double win=0;
                        if (s.tip=="num")
                        {
                            if (no==s.number)
                            {
                                if (tot_wins.count(s.login)) tot_wins[s.login]+=s.summa*36;
                                else tot_wins.insert(make_pair(s.login,s.summa*36));

                                win=s.summa*36;

                            }
                        }
                        if ((s.tip=="odd" && is_odd)||(s.tip=="even" && is_even) || (s.tip=="p1" && is_1p) || (s.tip=="p2" && is_2p) )
                        {
                            if (tot_wins.count(s.login)) tot_wins[s.login]+=s.summa*2;
                            else tot_wins.insert(make_pair(s.login,s.summa*2));
                            win=s.summa*2;
                        }
                        if ((s.tip=="d1" && is_1d)||(s.tip=="d2" && is_2d) || (s.tip=="d3" && is_3d))
                        {
                            if (tot_wins.count(s.login)) tot_wins[s.login]+=s.summa*3;
                            else tot_wins.insert(make_pair(s.login,s.summa*3));
                            win=s.summa*3;
                        }
                    }
                }

            }
            if (tot_wins.size())
            {
                string swin="<B>Выигрыши</B>: ";
                for (typeof(tot_wins.begin())i=tot_wins.begin();i!=tot_wins.end();i++)
                {
                    user *u=get_user(i->first);
                    if (u)
                    {
                        swin+="<B>"+u->nick+":"+fmtstr(dbh,i->second)+"</B> ";
                        privmsg(u->nick,"Поздравляю, вы выиграли "+fmtstr(dbh,i->second)+" кред");
                        unsigned int id=next_id();
                        cttu_nicks[id]=u->nick;
                        sock.response((string)"CTTU "+u->nick+" "+ftostring(i->second)+" r_win "+itostring(id));
                    }
                }
                action(swin);
            }
            else action("К сожалению никто не выиграл..");


            if (tot_stawki.size())
            {
                for (typeof(tot_stawki.begin())i=tot_stawki.begin();i!=tot_stawki.end();i++)
                {
                    user *u=get_user(i->first);
                    if (u)
                    {
                        if (tot_wins.count(i->first)==0)
                        {
                            privmsg(u->nick,"К сожалению вы проиграли, попробуйте еще.");
                        }
                    }
                }
            }

            action("Игра закончена. Следующая игра через <B>"+itostring(NEXT_GAME_TIME)+"</B> сек.");
            game_state=WAIT_NEXT_GAME;
            next_stoptime=time(NULL)+NEXT_GAME_TIME;
            remove_stawki();
        }
        if (game_state==RUNNING_WHEEL && time(NULL)>next_stoptime && next_stoptime!=0)
        {
            action("Бросаю шарик....");
            game_state=THROW_BALL;
            next_stoptime=time(NULL)+THROW_BALL_TIME;

        }
        if (game_state==STAKES_STOPPED && time(NULL)>next_stoptime && next_stoptime!=0)
        {
            action("Раскручиваю рулетку....");
            game_state=RUNNING_WHEEL;
            next_stoptime=time(NULL)+RUNNING_WHEEL_TIME;
        }
        if (game_state==ACCEPTING_STAKES && time(NULL)>next_stoptime && next_stoptime!=0)
        {MUTEX_INSPECTOR;
            action((string)"Прием ставок завершен. Поставить или снять ставку уже нельзя. Рулетку начну крутить через <B>"+itostring(STAKES_STOPPED_TIME) +"</B> сек." );
            next_stoptime=0;
            game_state=STAKES_STOPPED;
            next_stoptime=time(NULL)+STAKES_STOPPED_TIME;
            vector<string> v=get_stawki_logins();
            set<string> vv;
            for (unsigned i=0;i<v.size();i++)
            {MUTEX_INSPECTOR;
                user *u=get_user(v[i]);
                if (u)
                {MUTEX_INSPECTOR;
                    vv.insert(u->nick);
                }

            }
            if (vv.size()==1)
            {MUTEX_INSPECTOR;
                action((string)"Играeт <B>"+*vv.begin()+"</B>");
            }
            else if (vv.size()>1)
            {MUTEX_INSPECTOR;
                vector<string>vz;
                for (typeof(vv.begin())i=vv.begin();i!=vv.end();i++)
                    vz.push_back(*i);
                action((string)"Играют <B>"+join(", ",vz)+"</B>.");
            }
        }
        if (stakes_count==0)
        {MUTEX_INSPECTOR;
            game_state=ACCEPTING_STAKES;
        }
        //printf("next_stoptime %d GS %d stakes_count %d\n",next_stoptime,game_state,stakes_count);
        if (stakes_count!=0 && game_state==ACCEPTING_STAKES && next_stoptime==0)
        {MUTEX_INSPECTOR;
            action((string)"Первая ставка сделана, прием ставок завершается через <B>"+itostring(ACCEPTING_STAKES_TIME)+"</B> сек.");
            next_stoptime=time(NULL)+ACCEPTING_STAKES_TIME;
        }
        if (game_state==ACCEPTING_STAKES)
        {MUTEX_INSPECTOR;
            if (next_hint_time==0)
                next_hint_time=time(NULL)+HINT_1_TIME;
            if (t>next_hint_time && game_state==ACCEPTING_STAKES)
            {MUTEX_INSPECTOR;
                next_hint_time=time(NULL)+HINT_1_TIME;
                if (hints_ACCEPTING_STAKES.size()==0)
                {
                    printf("hints_ACCEPTING_STAKES.size()==0");
                    exit(1);
                }
                string s=hints_ACCEPTING_STAKES[rand()%hints_ACCEPTING_STAKES.size()];
                if (next_stoptime!=0 && rand()%2==0)
                    s+=" Прием ставок завершается через <B>"+itostring(next_stoptime-time(NULL)) +"</B> сек.";
                action(s);
            }

        }
    }
    XPASS
}




void do_greeting(const string &nick,const string &login, int gender);

void on_greeting(const string &nick,const string &login)
{MUTEX_INSPECTOR;
    XTRY
    if (nick==params["NICKNAME"]) return;
    if (users.find(nick)==users.end()){
        sock.response("EUI "+nick);
    }else{
        printf("founded in exists\n");
        user *u=users.find(nick)->second;
        do_greeting(nick,u->login,u->gender);
    }
    XPASS
}

void do_greeting(const string &nick,const string &login,int gender)
{MUTEX_INSPECTOR;
    printf("do_greeting");
    privmsg(nick,"Крупье приветствует Вас. <U>Правила игры</U> здесь: http://"+params["HOST"]+"/c2/help/roulette.php   Наберите в канале <U>!help</U> для получения справки о коммандах.");
}

void do_help(const string &nick)
{MUTEX_INSPECTOR;
    XTRY
    privmsg(nick,"Помощь:<br>"
            "<U>!старт| !start</U> - начать игру<br>"
            "Ставки:"
            "<U>!номер сумма</U> - Поставить сумму на номер, пример '!33,1-5,12 0.5'. Диапазон от 0 (зеро) до 36<br>"
            "<U>!odd !even !чет !нечет сумма</U> Поставить сумму на чет или нечет<br>"
            "<U>!p1 !п1 !p2 !п2 сумма</U> Поставить сумму на первую или вторую половину. От 1 до 18  или с 19 до 36<br>"
            "<U>!d1 !d2, !d3 !д1 !д2 !д3  сумма</U> Поставить сумму на первую, вторую или третью дюжину. 1-12, 13-24,25-36<br>"
            "<U>!ставки !stakes</U> - вывести свои ставки в текущей игре<br>"
            //	    "<U>!отмена !cancel</U> - отменить все ставки<br>"
           ) ;

    XPASS
}
void do_num_stawka(const string &nick, const string &msg)
{
    if (game_state!=ACCEPTING_STAKES)
    {
        privmsg(nick,"Извините, ставки уже не принимаются.");
        return;
    }
    user *u=NULL;
    if (users.find(nick)==users.end()){
        printf("nick not found\n");
        privmsg(nick,"Пожалуйста перезайдите в игру, либо повторите ставку.");
        sock.response("EUI "+nick);
        return;
    }
    u=users[nick];



    set<unsigned int> nums;
    vector<string> vV=splitTOK(" ",msg);
    if (vV.size()<2)
    {
        printf("invalid format do_num_stawka '%s'",msg.c_str());
        privmsg(nick,"Неправильный формат ставки.");
        return;
    }
    string summa=vV[1];
    string nsum;
    for (unsigned i=0;i<summa.size();i++)
    {
        if (summa[i]==',')nsum+='.';
        else nsum+=summa[i];
    }
    summa=nsum;
    if (atof(summa.c_str())<=0)
    {
        printf("invalid format do_num_stawka '%s'",msg.c_str());
        privmsg(nick,"Неправильный формат ставки.");
        return;
    }
    vector<string> vz=splitTOK("!,",vV[0]);
    for (unsigned int i=0;i<vz.size();i++)
    {
        vector<string> vm=splitTOK("-",vz[i]);
        if (vm.size()==1)
        {
            if (atoi(vm[0])<=36)
                nums.insert(atoi(vm[0]));
        }
        else if (vm.size()==2)
        {
            int start=atoi(vm[0]);
            int end=atoi(vm[1]);
            if (end>36)
            {
                privmsg(nick,"Неправильный формат ставки. Максимальное число не может превышать 36.");
                return;

            }
            if (start<0)
            {
                privmsg(nick,"Неправильный формат ставки. Числа должны быть положительными.");
                return;

            }
            if (start>end)
            {
                privmsg(nick,"Неправильный формат ставки. Первое число должно быть меньше последнего.");
                return;

            }
            for (int j=start;j<=end;j++)
            {
                nums.insert(j);
            }
        }
        else
        {
            privmsg(nick,"Неправильный формат ставки.");
            return;
        }
    }
    pay p;
    p.nums=nums;
    p.type="num";
    p.id=next_id();

    p.summa=atof(summa.c_str());
    mul_kkk(p.summa,summa);
    if (p.summa<=0){
        privmsg(nick,"Неправильный формат ставки.");
        return;
    }
    if (p.summa/p.nums.size()<0.01)
    {
        privmsg(nick,"Слишком маленькая ставка на одно число. Минимальная ставка должна быть больше 0.01 кр");
        return;
    }
    u->pays[p.id]=p;
    char s[300];

    snprintf(s,sizeof(s),"UTTC %s %f get_roulette_num %d",nick.c_str(),p.summa,p.id);
    sock.response(s);
    return;

}
void do_odd_even_stawka(const string& type,const string &nick,const string &msg)
{
    if (game_state!=ACCEPTING_STAKES)
    {
        privmsg(nick,"Извините, ставки уже не принимаются.");
        return;
    }

    user *u=NULL;
    if (users.find(nick)==users.end()){
        printf("nick not found\n");
        privmsg(nick,"Пожалуйста перезайдите в игру, либо повторите ставку.");
        sock.response("EUI "+nick);
        return;
    }
    u=users[nick];
    vector<string> v=splitTOK(" ",msg);
    if (v.size()>1)
    {

        pay p;
        p.type=type;
        p.id=next_id();
        string summa=v[1];

        string nsum;
        for (unsigned i=0;i<summa.size();i++)
        {
            if (summa[i]==',')nsum+='.';
            else nsum+=summa[i];
        }
        summa=nsum;

        p.summa=atof(summa.c_str());
        mul_kkk(p.summa,summa);
        if (p.summa<=0)
        {
            privmsg(nick,"Неправильный формат ставки.");
            return;
        }
        u->pays[p.id]=p;
        char s[300];
        snprintf(s,sizeof(s),"UTTC %s %f get_roulette_%s %d",nick.c_str(),p.summa,type.c_str(),p.id);
        sock.response(s);
        return;

    }
    else
    {
        privmsg(nick,"Неправильный формат ставки.");
    }

}
void do_dozen_stawka(c_command &cmd)
{

}
void do_polowina_stawka(c_command &cmd)
{

}
void do_UTTC_REPLY(c_command &cmd)
{
MUTEX_INSPECTOR;
//    vector<string> pr=splitTOK(":",cmd.msg);
    map<string,string> m=parse_strmap(cmd.msg);
    string nick=m["NICK"];
    string summa=m["SUMMA"];
    string cookie=m["COOKIE"];
    string from_reminder=m["REMINDER_FROM"];
    if (m["STATUS"]!="OK")
    {MUTEX_INSPECTOR;
        if (m["ERROR"]=="Not_enough_credits")
            privmsg(nick,"У вас недостаточно кредитов на счету, ставка не принята.");
        else
            privmsg(nick,"Произошла непонятная ошибка, ставка не принята.");
    }
    else
    {MUTEX_INSPECTOR;
        stakes_count++;
        unsigned int id=atoi(cookie);

        user *u=NULL;
        if (users.find(nick)==users.end()){MUTEX_INSPECTOR;
            printf("nick not found\n");
            privmsg(nick,"Пожалуйста перезайдите в игру, либо повторите ставку.");
            sock.response("EUI "+nick);
            return;
        }
        u=users[nick];
        if (u->pays.count(id)==0)
        {MUTEX_INSPECTOR;
            privmsg(nick,"Ошибка ставка не найдена.");
            return;
        }
        pay p=u->pays[id];
        if (atof(summa.c_str())!=p.summa)
        {MUTEX_INSPECTOR;
            privmsg(nick,"Не совпадают суммы.");
            return;
        }
        if (p.type=="num")
        {MUTEX_INSPECTOR;

            double sum1=p.summa/p.nums.size();
            vector<string> snums;

            for (typeof(p.nums.begin())i=p.nums.begin();i!=p.nums.end();i++)
            {MUTEX_INSPECTOR;
                snums.push_back(itostring(*i));
                add_stawka(u->login,sum1,"num",*i);

            }
            if (p.nums.size()==1)
            {MUTEX_INSPECTOR;
                action("<B>"+nick+"</B> поставил(а) на число <B>"+join(",",snums)+" "+fmtstr(dbh,sum1)+"</B> кред");
                privmsg(nick,"Принята ставка на число "+join(",",snums)+" по "+fmtstr(dbh,sum1)+" кред. Остаток на счету "+fmtstr(dbh,atof(from_reminder.c_str()))+" кред.");

            }
            else
            {MUTEX_INSPECTOR;
                action("<B>"+nick+"</B> поставил(а) на числа <B>"+join(",",snums)+" по "+fmtstr(dbh,sum1)+"</B> кред.");
                privmsg(nick,"Принята ставка на числа "+join(",",snums)+" по "+fmtstr(dbh,sum1)+" кред. Остаток на счету "+fmtstr(dbh,atof(from_reminder.c_str()))+" кред.");
            }

            u->pays.erase(p.id);

        }
        else if (p.type=="odd" || p.type=="even" || p.type=="p1" || p.type=="p2" || p.type=="d1" || p.type=="d2" || p.type=="d3")
        {MUTEX_INSPECTOR;

            double sum=p.summa;
            string tt;
            if (p.type=="odd") tt="нечетное";
            else if (p.type=="even") tt="четное";
            else if (p.type=="p1") tt="первую половину";
            else if (p.type=="p2") tt="вторую половину";
            else if (p.type=="d1") tt="первую дюжину";
            else if (p.type=="d2") tt="вторую дюжину";
            else if (p.type=="d3") tt="третью дюжину";
            else
            {MUTEX_INSPECTOR;
                privmsg(nick,"неправильная ставка" +p.type);
            }
//            dbh->real_queryQ((QUERY)"insert into roulette_stawki (login,tip,summa,dt) values ('?','?',?,NOW())"<<u->login<<p.type<<sum);
            add_stawka(u->login,sum,p.type,0);
            //const string& login,const string &summa,const string& tip,const string& number

            action("<B>"+nick+"</B> поставил(а) на <B>"+tt+" "+fmtstr(dbh,sum)+"</B> кред.");
            privmsg(nick,(string)"Принята ставка на <B>"+tt+"</B> сумма <B>"+fmtstr(dbh,sum)+"</B> кред. Остаток на счету "+fmtstr(dbh,atof(from_reminder.c_str()))+" кред.");
            u->pays.erase(p.id);
        }
    }

}
void do_stawki(c_command &cmd)
{
    if (game_state!=ACCEPTING_STAKES)
    {
        privmsg(cmd.fromnick,"Извините, ставки уже не принимаются.");
        return;
    }
    user *u=NULL;
    if (users.find(cmd.fromnick)==users.end()){
        printf("nick not found\n");
        privmsg(cmd.fromnick,"Пожалуйста перезайдите в игру, либо повторите ставку.");
        sock.response("EUI "+cmd.fromnick);
        return;
    }
    u=users[cmd.fromnick];
    map<string,vector<stawka> >v=get_stawki();
    for (typeof(v.begin()) i=v.begin();i!=v.end();i++)
    {
        for (unsigned j=0;j<i->second.size();j++)
        {
            stawka &st=i->second[j];
            if (st.tip=="num")
            {
                char s[200];
                snprintf(s,sizeof(s),"%s на номер %d",fmtstr(dbh,st.summa).c_str(),st.number);
                privmsg(cmd.fromnick,s);
            }
            else if (st.tip=="odd")
            {
                char s[200];
                snprintf(s,sizeof(s),"%s на нечетное",fmtstr(dbh,st.summa).c_str());
                privmsg(cmd.fromnick,s);
            }
            else if (st.tip=="even")
            {
                char s[200];
                snprintf(s,sizeof(s),"%s на четное",fmtstr(dbh,st.summa).c_str());
                privmsg(cmd.fromnick,s);
            }

        }
    }

}

void load_messages()
{MUTEX_INSPECTOR;
    //	msgs.clear();
    if (hints_ACCEPTING_STAKES.size()==0)
    {
        FILE *f = fopen("./hints_ACCEPTING_STAKES", "r");
        if (!f){
            logErr("---Error: mafia.messages not found!");
            exit(1);
        }
        string data;
        while (!feof(f)){
            char s[40000];
            int z=fread((void*) &s, 1, 990,f);
            s[z]=0;
            data+=s;
        }
        hints_ACCEPTING_STAKES=splitTOK("\r\n",data);
    }
}

void process_command(c_command &cmd)
{MUTEX_INSPECTOR;
    XTRY
    string msg=cmd.msg;

    load_messages();

    check_for_flood(cmd);
    if (cmd.cmd=="JOIN"){MUTEX_INSPECTOR;
        {MUTEX_INSPECTOR;
            timeval tv;
            gettimeofday(&tv,NULL);
            srand(tv.tv_usec+rand());
            rnd+=tv.tv_usec*rand();
        }

        printf("on join");
        on_greeting(cmd.fromnick,cmd.fromnick);
        return;
    }
    if (cmd.cmd=="NICK"){MUTEX_INSPECTOR;
        {MUTEX_INSPECTOR;
            timeval tv;
            gettimeofday(&tv,NULL);
            srand(tv.tv_usec+rand());
            rnd+=tv.tv_usec*rand();
        }

        user *u;
        if (users.find(cmd.fromnick)==users.end()) return;
        u=users[cmd.fromnick];
        users[cmd.cmd]=u;
        u->nick=cmd.fromnick;
        return;
    }

    if (cmd.cmd=="UTTC_REPLY"){MUTEX_INSPECTOR;
        {MUTEX_INSPECTOR;
            timeval tv;
            gettimeofday(&tv,NULL);
            srand(tv.tv_usec+rand());
            rnd+=tv.tv_usec*rand();
        }
        {
            MUTEX_INSPECTOR	;

            do_UTTC_REPLY(cmd);
        }

    }

    if (cmd.cmd=="CTTU_REPLY"){MUTEX_INSPECTOR;
        {MUTEX_INSPECTOR;
            timeval tv;
            gettimeofday(&tv,NULL);
            srand(tv.tv_usec+rand());
            rnd+=tv.tv_usec*rand();
        }

        map<string,string> m=parse_strmap(cmd.msg);
        if (m["STATUS"]=="OK"){MUTEX_INSPECTOR;
            string cookie=m["COOKIE"];
            string treminder=m["REMINDER_TO"];
            if (cttu_nicks.count(atoi(cookie)))
            {MUTEX_INSPECTOR;
                privmsg(cttu_nicks[atoi(cookie)],"У вас на счету "+treminder+" кред.");
            }
        }
    }
    if (cmd.cmd=="EUI_REPLY"){MUTEX_INSPECTOR;
        {MUTEX_INSPECTOR;
            timeval tv;
            gettimeofday(&tv,NULL);
            srand(tv.tv_usec+rand());
            rnd+=tv.tv_usec*rand();
        }

        map<string,string> m=parse_strmap(cmd.msg);
        string nick=m["NICK"];
        string __login=m["LOGIN"];
        int gender=atoi("GENDER");
        bool ok=false;
        for (typeof(users.begin()) i=users.begin();i!=users.end();i++){MUTEX_INSPECTOR;
            if (i->second->login==__login){MUTEX_INSPECTOR;
                users[nick]=i->second;
                users[nick]->nick=nick;
                ok=true;
            }
        }
        if (!ok){MUTEX_INSPECTOR;
            user* u=new user;
            users[nick]=u;
        }
        users[nick]->login=__login;
        users[nick]->gender=gender;
        users[nick]->unreg=false;//unreg;
        users[nick]->nick=nick;
//        users[login]=users[nick];
        do_greeting(nick,__login,gender);
    }
    if (cmd.cmd=="PRIVMSG"){MUTEX_INSPECTOR;

        {MUTEX_INSPECTOR;
            timeval tv;
            gettimeofday(&tv,NULL);
            srand(tv.tv_usec+rand());
            rnd+=tv.tv_usec*rand();
        }

        if (strex(msg,"!help") || strex(msg,"!хелп") || strex(msg,"!помощь")){MUTEX_INSPECTOR;
            do_help(cmd.fromnick);
            return;
        }

        printf("msg '%s'",msg.c_str());
        if (msg.size()>2)
        {MUTEX_INSPECTOR;
            if (msg[0]=='!' && isdigit(msg[1]))
                do_num_stawka(cmd.fromnick,msg);
        }

        if ((strex(msg,"!odd")||strex(msg,"!неч")))
        {MUTEX_INSPECTOR;
            do_odd_even_stawka("odd",cmd.fromnick,msg);
        }
        if ((strex(msg,"!eve")||strex(msg,"!чет")))
        {MUTEX_INSPECTOR;
            do_odd_even_stawka("even",cmd.fromnick,msg);
        }
        if ((strex(msg,"!d1")||strex(msg,"!д1")))
        {MUTEX_INSPECTOR;
            do_odd_even_stawka("d1",cmd.fromnick,msg);
        }
        if ((strex(msg,"!d2")||strex(msg,"!д2")))
        {MUTEX_INSPECTOR;
            do_odd_even_stawka("d2",cmd.fromnick,msg);
        }
        if ((strex(msg,"!d3")||strex(msg,"!д3")))
        {MUTEX_INSPECTOR;
            do_odd_even_stawka("d3",cmd.fromnick,msg);
        }
        logErr("(cmd.fromnick '%s'",cmd.fromnick.c_str());

        if ((strex(msg,"!p1")||strex(msg,"!п1")))
        {MUTEX_INSPECTOR;
            do_odd_even_stawka("p1",cmd.fromnick,msg);
        }
        if ((strex(msg,"!p2")||strex(msg,"!п2")))
        {MUTEX_INSPECTOR;
            do_odd_even_stawka("p2",cmd.fromnick,msg);
        }

        if ((strex(msg,"!stak")||strex(msg,"!став")))
        {MUTEX_INSPECTOR;
            do_stawki(cmd);

        }



        if (strex(msg,"!start") || strex(msg,"!старт") || strex(msg,"!cnfhn") || strex(msg,"!ыефке")){MUTEX_INSPECTOR;
            if (!game_started){MUTEX_INSPECTOR;
                game_started=true;
                action("Начинается игра <B>\"Рулетка\"</B>. Делайте ставки господа.");

                //next_game_time=time(NULL)+GAME_PERIOD;
            }
            return;
        }
    }
    XPASS
}

