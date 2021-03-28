var greets=new Array();
greets=[{g0:"[~time~] К нам присоединилось ~who~",g1:"[~time~] К нам присоединился ~who~",g2:"[~time~] К нам присоединилась ~who~"},
	{g0:"[~time~] Нас посетило ~who~",g1:"[~time~] Нас посетил ~who~",g2:"[~time~] Нас посетилa ~who~"},
	{g0:"[~time~] К нам присоединилось ~who~, поприветствуем",g1:"[~time~] К нам присоединился ~who~, поприветствуем",g2:"[~time~] К нам присоединиласьь ~who~, поприветствуем"},
	{g0:"[~time~] К нам присоединяется ~who~",g1:"[~time~] К нам присоединяется ~who~",g2:"[~time~] К нам присоединяется ~who~"},
	{g0:"[~time~] К нам прибыло ~who~",g1:"[~time~] К нам прибыл ~who~",g2:"[~time~] К нам прибылa ~who~"},
	{g0:"[~time~] К нам пришло ~who~",g1:"[~time~] К нам пришeл ~who~",g2:"[~time~] К нам пришла ~who~"},
	{g0:"[~time~] К нам в чат на огонёк зашло ~who~",g1:"[~time~] К нам в чат на огонёк зашeл ~who~",g2:"[~time~] К нам в чат на огонёк зашлa ~who~"},
	{g0:"[~time~] Совершенно неожиданно появилось ~who~",g1:"[~time~] Совершенно неожиданно появился ~who~",g2:"[~time~] Совершенно неожиданно появилась ~who~"},
	{g0:"[~time~] Разрешите представить: ~who~",g1:"[~time~] Разрешите представить: ~who~",g2:"[~time~] Разрешите представить: ~who~"},
	{g0:"[~time~] ~who~ подключилось к чату",g1:"[~time~] ~who~ подключился к чату",g2:"[~time~] ~who~ подключилась к чату"},
	{g0:"[~time~] ~who~ снова с нами! Дружно скажем \"здрасти\"!",g1:"[~time~] ~who~ снова с нами! Дружно скажем \"здрасти\"!",g2:"[~time~] ~who~ снова с нами! Дружно скажем \"здрасти\"!"},
	{g0:"[~time~] Вдруг откуда ни возьмись появилось ~who~",g1:"[~time~] Вдруг откуда ни возьмись появилось ~who~",g2:"[~time~] Вдруг откуда ни возьмись появилась ~who~"}];

var byes=new Array();
byes=[{g0:"[~time~] Нас покинуло ~who~",g1:"[~time~] Нас покинул ~who~",g2:"[~time~] Нас покинула ~who~"},
    {g0:"[~time~] ~who~ уже не с нами",g1:"[~time~] ~who~ уже не с нами",g2:"[~time~] ~who~ уже не с нами"},
    {g0:"[~time~] ~who~ раскланялось и ушло",g1:"[~time~] ~who~ раскланялся и ушел",g2:"[~time~] ~who~ раскланялась и ушла"},
    {g0:"[~time~] От нас ушло ~who~",g1:"[~time~] От нас ушел ~who~",g2:"[~time~] От нас ушла ~who~"},
    {g0:"[~time~] Ушло ~who~",g1:"[~time~] Ушел ~who~",g2:"[~time~] Ушла ~who~"},
    {g0:"[~time~] Беззвучно исчезло ~who~",g1:"[~time~] Беззвучно исчез ~who~",g2:"[~time~] Беззвучно исчезла ~who~"},
    {g0:"[~time~] Раскланялось и исчезло ~who~",g1:"[~time~] Раскланялся и исчез ~who~",g2:"[~time~] Раскланялась и исчезла ~who~"},
    {g0:"[~time~] ~who~ прощается со всеми и уходит",g1:"[~time~] ~who~ прощается со всеми и уходит",g2:"[~time~] ~who~ прощается со всеми и уходит"},
    {g0:"[~time~] Свалило ~who~",g1:"[~time~] Свалил ~who~",g2:"[~time~] Свалила ~who~"}];
var chrooms =new Array();
chrooms=[{g0:"~who~ теперь тусуется в ~room~",g1:"~who~ теперь тусуется в ~room~",g2:"~who~ теперь тусуется в ~room~"},
	{g0:"~who~ пошло тусоваться в ~room~",g1:"~who~ пошел тусоваться в ~room~",g2:"~who~ пошла тусоваться в ~room~"},
	{g0:"~who~ отныне тусуется в ~room~",g1:"~who~ отныне тусуется в ~room~",g2:"~who~ отныне тусуется в ~room~"}];
var cens=new Array();
cens=[{g0:"Нецензурно выразилось ~who~",g1:"Нецензурно выразился ~who~",g2:"Нецензурно выразилась ~who~"},
	{g0:"Что то плохое ляпнуло ~who~",g1:"Что то плохое ляпнуло ~who~",g2:"Что то плохое ляпнуло ~who~"},
	{g0:"Сматюгалось ~who~",g1:"Сматюгался ~who~",g2:"Сматюгалась ~who~"}];

var kickings=new Array();

kickings=[{g0:"Выставлено из чата ",g1:"Выставлен из чата ",g2:"Выставлена из чата "},
	{g0:"Выпнуто из чата ",g1:"Выпнут из чата ",g2:"Выпнутa из чата "},
	{g0:"Выгнанo из чата ",	g1:"Выгнан из чата ",g2:"Выгнанa из чата "},
	{g0:"Выкинуто из чата ",g1:"Выкинут из чата ",g2:"Выкинутa из чата "},
	{g0:"Вышвырнуто из чата ",g1:"Вышвырнут из чата ",g2:"Вышвырнутa из чата "}];

var killings=new Array();

killings=[{g0:"Отлетело ~who~",	g1:"Отлетел ~who~",g2:"Отлетела ~who~"},
	    {g0:"Нас безвременно покинуло ~who~",g1:"Нас безвременно покинул ~who~",g2:"Нас безвременно покинула ~who~"},
	    {g0:"Отвалилось ~who~",g1:"Отвалился ~who~",g2:"Отвалилась ~who~"},
	    {g0:"Беззвучно растворилось в воздухе ~who~",g1:"Беззвучно растворился в воздухе ~who~",g2:"Беззвучно растворилась в воздухе ~who~"},
	    {g0:"Молча отвалилось ~who~",g1:"Молча отвалился ~who~",g2:"Молча отвалилась ~who~"}];


var modechanges=new Array();
modechanges=[{g0:"~who~ установило режим комнаты: ~mode~",g1:"~who~ установил режим комнаты: ~mode~",g2:"~who~ установила режим комнаты: ~mode~"}];

var nickchanges=new Array();
nickchanges=[{g0:"~who1~ теперь известно как ~who2~",g1:"~who1~ теперь известен как ~who2~",g2:"~who1~ теперь известна как ~who2~"},
	    {g0:"~who1~ переименовалось в ~who2~",g1:"~who1~ переименовался в ~who2~",g2:"~who1~ переименовалась в ~who2~"},
	    {g0:"~who1~ стало известно как ~who2~",g1:"~who1~ стал известен как ~who2~",g2:"~who1~ стала известна как ~who2~"}];
var topicchanges=new Array();
topicchanges=[{g0:"~who~  сменило топик комнаты на ~top~",g1:"~who~  сменил топик комнаты на ~top~",g2:"~who~  сменила топик комнаты на ~top~"}];




function get_message(gender, arr)
{
    var s="undef";
    switch(gender)
    {
	case 0:
	    s=arr[parseInt(Math.random()*arr.length)].g0;
	    break;
	case 1:
	    s=arr[parseInt(Math.random()*arr.length)].g1;
	    break;
	case 2:
	    s=arr[parseInt(Math.random()*arr.length)].g2;
	break;
	
    }
    return s;
}
function nick(n)
{
    return "<a class=nick href=\"javascript: parent.parent.Answer('"+n+"')\" target=addmessage OnMouseOver=\"return os();\" style=\"font-weight: bold\">"+n+"</a>";
}
function bnick(n)
{
    return "<a class=nick style=\"font-weight: bold\">"+n+"</a>";
}
function stdmsg(ncolorpack,from_uid,to_uid,fromnick,msg)
{
    if(ncolorpack==0)
    {
	if(from_uid==uid)
	{
	    document.write("<TABLE><TR><TD nowrap>"+nick(fromnick)+": </TD><TD><font class=mt>"+msg+"</font></TD></TR></TABLE>");
	}
	else if(to_uid==uid)
	{
	    document.write("<TABLE><TR><TD nowrap>"+nick(fromnick)+": </TD><TD><font class=mf><b>"+msg+"</b></TD></TR></TABLE>");	
	}
	else 
	{
	    document.write("<TABLE id=hm><TR><TD nowrap>"+nick(fromnick)+": </TD><TD>"+msg+"</TD></TR></TABLE>");
	}
    }
}
function privmsg(ncolorpack,from_uid,to_uid,fromnick,tonick,msg)
{
    if(ncolorpack==0)
    {
	if(from_uid==uid)
	document.write("<TABLE class=mpt><TR><TD class=mp>*** Приватное сообщение для "+nick(tonick)+"<BR><font class=mp>"+msg+"</TD></TR></TABLE>");    
	else if(to_uid==uid)
	document.write("<TABLE class=mpt><TR><TD class=mp>*** Приватное сообщение от "+nick(fromnick)+"<BR><font class=mptxt><b>"+msg+"</b></font></TD></TR></TABLE>");
	
    }
}
function makeitmsg(ncolorpack,fromnick,msg)
{
    if(ncolorpack==0)
    {
	document.write("<TABLE><TR><TD><font class=mmit>*** "+nick(fromnick)+" "+msg+"</font></TD></TR></TABLE>");
    }
}
function sysmsg(ncolorpack,msg)
{
    if(ncolorpack==0)
    {
	document.write("<TABLE border=0 id=hs><TR><TD><font class=ms>*** "+msg+"</TD></TR></TABLE>");
    }
}
function sysprivmsg(ncolorpack,from_uid,to_uid,tonick,msg)
{
    if(ncolorpack==0)
    {
	if(uid==from_uid)
	{
	    var n="";
	    if(tonick.length()>0)
		n="для "+tonick;
	document.write("<TABLE class=mpst><TR><TD class=mps>*** Приватное системное сообщение "+n+"<BR><b>"+msg+"</b></TD></TR></TABLE>");
	}
	else if(uid==to_uid)
	{
	    document.write("<TABLE class=mpst><TR><TD class=mps>*** Приватное системное сообщение <BR><b>"+msg+"</b></TD></TR></TABLE>");
	}
    }
}

function greeting_msg(gender,who, t, loc)
{
    var s=get_message(gender,greets).replace(/~time~/g,t).replace(/~who~/,nick(who)).replace(/~loc~/g,loc);
    sysmsg(0,s);
    
}

function bye_msg(gender,who, t)
{
    var s=get_message(gender,byes).replace(/~time~/g,t).replace(/~who~/,bnick(who));
    sysmsg(0,s);
//    var s=get_message(gender,byes);
//    return s.replace(/~time~/g,t).replace(/~who~/,who);
}
function cens_msg(gender,who)
{
    var s=get_message(gender,cens).replace(/~who~/,nick(who);
    sysmsg(0,s);
//    var s=get_message(gender,cens);
//    return s.replace(/~who~/,who);
}
function kick_msg(gender,who,za)
{
    var s=get_message(gender,kickings);
    s+=bnick(who);
    if(za.length>0)
	s+=" за "+za;
//    return s;
    sysmsg(0,s);
}

function kill_msg(gender,who)
{

    var s=get_message(gender,killings).replace(/~who~/,bnick(who));
    sysmsg(0,s);
}

function modechanges_msg(gender,who,who2,m)
{
    var s=get_message(gender,modechanges);
    var m2="";
    if(m=="+v" || m=="-v")
    {
	m2=m+" "+bnick(who2);
    }
    else if(m=="+m")
    {
	m2="Модерируемая комната (+m)";
    }
    else if(m=="-m")
    {
	m2="НЕмодерируемая комната (-m)"
    }
    s.replace(/~who~/,nick(who)).replace(/~mode~/,m2);
    sysmsg(0,s);
}
function nickchanges_msg(gender,who1,who2)
{
    var s=get_message(gender,nickchanges).replace(/~who1~/,bnick(who1)).replace(/~who2~/,nick(who2));
    sysmsg(0,s);
}
function topicchanges_msg(gender,who,top)
{
    var s=get_message(gender,topicchanges).replace(/~who~/,nick(who)).replace(/~top~/,top);
    sysmsg(0,s);
}


function chrooms_msg(gender,who,room)
{
    var s=get_message(gender,chrooms).replace(/~who~/,nick(who)).replace(/~room~/,room);
    sysmsg(0,s);
}
