var greets=new Array();
greets=[{g0:"[~time~] � ��� �������������� ~who~",g1:"[~time~] � ��� ������������� ~who~",g2:"[~time~] � ��� �������������� ~who~"},
	{g0:"[~time~] ��� �������� ~who~",g1:"[~time~] ��� ������� ~who~",g2:"[~time~] ��� �������a ~who~"},
	{g0:"[~time~] � ��� �������������� ~who~, ��������������",g1:"[~time~] � ��� ������������� ~who~, ��������������",g2:"[~time~] � ��� ��������������� ~who~, ��������������"},
	{g0:"[~time~] � ��� �������������� ~who~",g1:"[~time~] � ��� �������������� ~who~",g2:"[~time~] � ��� �������������� ~who~"},
	{g0:"[~time~] � ��� ������� ~who~",g1:"[~time~] � ��� ������ ~who~",g2:"[~time~] � ��� ������a ~who~"},
	{g0:"[~time~] � ��� ������ ~who~",g1:"[~time~] � ��� ����e� ~who~",g2:"[~time~] � ��� ������ ~who~"},
	{g0:"[~time~] � ��� � ��� �� ����� ����� ~who~",g1:"[~time~] � ��� � ��� �� ����� ���e� ~who~",g2:"[~time~] � ��� � ��� �� ����� ����a ~who~"},
	{g0:"[~time~] ���������� ���������� ��������� ~who~",g1:"[~time~] ���������� ���������� �������� ~who~",g2:"[~time~] ���������� ���������� ��������� ~who~"},
	{g0:"[~time~] ��������� �����������: ~who~",g1:"[~time~] ��������� �����������: ~who~",g2:"[~time~] ��������� �����������: ~who~"},
	{g0:"[~time~] ~who~ ������������ � ����",g1:"[~time~] ~who~ ����������� � ����",g2:"[~time~] ~who~ ������������ � ����"},
	{g0:"[~time~] ~who~ ����� � ����! ������ ������ \"�������\"!",g1:"[~time~] ~who~ ����� � ����! ������ ������ \"�������\"!",g2:"[~time~] ~who~ ����� � ����! ������ ������ \"�������\"!"},
	{g0:"[~time~] ����� ������ �� �������� ��������� ~who~",g1:"[~time~] ����� ������ �� �������� ��������� ~who~",g2:"[~time~] ����� ������ �� �������� ��������� ~who~"}];

var byes=new Array();
byes=[{g0:"[~time~] ��� �������� ~who~",g1:"[~time~] ��� ������� ~who~",g2:"[~time~] ��� �������� ~who~"},
    {g0:"[~time~] ~who~ ��� �� � ����",g1:"[~time~] ~who~ ��� �� � ����",g2:"[~time~] ~who~ ��� �� � ����"},
    {g0:"[~time~] ~who~ ������������ � ����",g1:"[~time~] ~who~ ����������� � ����",g2:"[~time~] ~who~ ������������ � ����"},
    {g0:"[~time~] �� ��� ���� ~who~",g1:"[~time~] �� ��� ���� ~who~",g2:"[~time~] �� ��� ���� ~who~"},
    {g0:"[~time~] ���� ~who~",g1:"[~time~] ���� ~who~",g2:"[~time~] ���� ~who~"},
    {g0:"[~time~] ��������� ������� ~who~",g1:"[~time~] ��������� ����� ~who~",g2:"[~time~] ��������� ������� ~who~"},
    {g0:"[~time~] ������������ � ������� ~who~",g1:"[~time~] ����������� � ����� ~who~",g2:"[~time~] ������������ � ������� ~who~"},
    {g0:"[~time~] ~who~ ��������� �� ����� � ������",g1:"[~time~] ~who~ ��������� �� ����� � ������",g2:"[~time~] ~who~ ��������� �� ����� � ������"},
    {g0:"[~time~] ������� ~who~",g1:"[~time~] ������ ~who~",g2:"[~time~] ������� ~who~"}];
var chrooms =new Array();
chrooms=[{g0:"~who~ ������ �������� � ~room~",g1:"~who~ ������ �������� � ~room~",g2:"~who~ ������ �������� � ~room~"},
	{g0:"~who~ ����� ���������� � ~room~",g1:"~who~ ����� ���������� � ~room~",g2:"~who~ ����� ���������� � ~room~"},
	{g0:"~who~ ������ �������� � ~room~",g1:"~who~ ������ �������� � ~room~",g2:"~who~ ������ �������� � ~room~"}];
var cens=new Array();
cens=[{g0:"���������� ���������� ~who~",g1:"���������� ��������� ~who~",g2:"���������� ���������� ~who~"},
	{g0:"��� �� ������ ������� ~who~",g1:"��� �� ������ ������� ~who~",g2:"��� �� ������ ������� ~who~"},
	{g0:"����������� ~who~",g1:"���������� ~who~",g2:"����������� ~who~"}];

var kickings=new Array();

kickings=[{g0:"���������� �� ���� ",g1:"��������� �� ���� ",g2:"���������� �� ���� "},
	{g0:"������� �� ���� ",g1:"������ �� ���� ",g2:"������a �� ���� "},
	{g0:"������o �� ���� ",	g1:"������ �� ���� ",g2:"������a �� ���� "},
	{g0:"�������� �� ���� ",g1:"������� �� ���� ",g2:"�������a �� ���� "},
	{g0:"���������� �� ���� ",g1:"��������� �� ���� ",g2:"���������a �� ���� "}];

var killings=new Array();

killings=[{g0:"�������� ~who~",	g1:"������� ~who~",g2:"�������� ~who~"},
	    {g0:"��� ����������� �������� ~who~",g1:"��� ����������� ������� ~who~",g2:"��� ����������� �������� ~who~"},
	    {g0:"���������� ~who~",g1:"��������� ~who~",g2:"���������� ~who~"},
	    {g0:"��������� ������������ � ������� ~who~",g1:"��������� ����������� � ������� ~who~",g2:"��������� ������������ � ������� ~who~"},
	    {g0:"����� ���������� ~who~",g1:"����� ��������� ~who~",g2:"����� ���������� ~who~"}];


var modechanges=new Array();
modechanges=[{g0:"~who~ ���������� ����� �������: ~mode~",g1:"~who~ ��������� ����� �������: ~mode~",g2:"~who~ ���������� ����� �������: ~mode~"}];

var nickchanges=new Array();
nickchanges=[{g0:"~who1~ ������ �������� ��� ~who2~",g1:"~who1~ ������ �������� ��� ~who2~",g2:"~who1~ ������ �������� ��� ~who2~"},
	    {g0:"~who1~ ��������������� � ~who2~",g1:"~who1~ �������������� � ~who2~",g2:"~who1~ ��������������� � ~who2~"},
	    {g0:"~who1~ ����� �������� ��� ~who2~",g1:"~who1~ ���� �������� ��� ~who2~",g2:"~who1~ ����� �������� ��� ~who2~"}];
var topicchanges=new Array();
topicchanges=[{g0:"~who~  ������� ����� ������� �� ~top~",g1:"~who~  ������ ����� ������� �� ~top~",g2:"~who~  ������� ����� ������� �� ~top~"}];




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
	document.write("<TABLE class=mpt><TR><TD class=mp>*** ��������� ��������� ��� "+nick(tonick)+"<BR><font class=mp>"+msg+"</TD></TR></TABLE>");    
	else if(to_uid==uid)
	document.write("<TABLE class=mpt><TR><TD class=mp>*** ��������� ��������� �� "+nick(fromnick)+"<BR><font class=mptxt><b>"+msg+"</b></font></TD></TR></TABLE>");
	
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
		n="��� "+tonick;
	document.write("<TABLE class=mpst><TR><TD class=mps>*** ��������� ��������� ��������� "+n+"<BR><b>"+msg+"</b></TD></TR></TABLE>");
	}
	else if(uid==to_uid)
	{
	    document.write("<TABLE class=mpst><TR><TD class=mps>*** ��������� ��������� ��������� <BR><b>"+msg+"</b></TD></TR></TABLE>");
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
	s+=" �� "+za;
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
	m2="������������ ������� (+m)";
    }
    else if(m=="-m")
    {
	m2="�������������� ������� (-m)"
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
