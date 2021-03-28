function do_hide_menu(n)
{
	if(!m_v) return;
	m_v=0;
	parent.scr=s_s;
	document.getElementById('menu').style.visibility="hidden";
	if(n!=1) DS();
}

function do_show_menu(obj)
{
        if(m_v) do_hide_menu(1);
	if(event.srcElement.className.substring(0,4)=='nick'){
		nick=event.srcElement.innerHTML;
		cid=0;
		for(var i in parent.UsrList){
			if(parent.UsrList[i].n==nick){
				cid=parent.UsrList[i].id;
				break;
			}
		}
		s_s=parent.scr;
		parent.scr=false;
		mn=document.getElementById('menu');
		var rightedge=document.body.clientWidth-event.clientX;
		var bottomedge=document.body.clientHeight-event.clientY;
		if (rightedge<mn.offsetWidth){
			mn.style.left=document.body.scrollLeft+event.clientX-mn.offsetWidth;
		}else{
			mn.style.left=document.body.scrollLeft+event.clientX-10;
		}
		if (bottomedge<mn.offsetHeight){
			mn.style.top=document.body.scrollTop+event.clientY-mn.offsetHeight;
		}else{
			mn.style.top=document.body.scrollTop+event.clientY-40;
		}
		document.getElementById("menunick").innerHTML='&nbsp;&nbsp;&nbsp;'+nick+'&nbsp;&nbsp;&nbsp;';
		mn.style.visibility="visible";
		m_v=1;
	}else{
		return true;
	}
	return false;
}

function mOvr(src)
{
	src.style.cursor='hand';
	src.style.backgroundColor="#004080";
}
function mOut(src)
{
	src.style.cursor='default';
	src.style.backgroundColor="#004590";
}
function mClk(code,nick)
{
//	code=src.cmd;
//	alert(src.cmd);
	if(code==1){
		parent.parent.Answer(nick,1);
		return;
	}
	if(code==2 /*&& cid!=0*/){
		parent.userinfoNI(nick);
		return;
	}
	if(code==3 /*&& cid!=0*/){
		parent.IgnoreUser(cid,nick);
		return;
	}
	if(code==4){
		parent.KickUser(cid,nick);
		return;
	}
	w=document.f;
	if(code==8){
		s='маты';
		if(s==null) return;
		if(code==4) w.cmd.value='kick';
		w.id.value=cid;
		w.nick.value=nick;
		w.msg.value=s;
		w.submit();
		return;
	}
	w=document.f;
	if(code==9){
		s='флуд';
		if(s==null) return;
		if(code==4) w.cmd.value='kick';
		w.id.value=cid;
		w.nick.value=nick;
		w.msg.value=s;
		w.submit();
		return;
	
	}
	w=document.f;
	if(code==5 || code==6 || code==7){
		s=prompt(name+' будет забанен за ?','');
		if(s==null) return;
		if(code==5) w.cmd.value='bi';
		if(code==6) w.cmd.value='bn';
		if(code==7) w.cmd.value='bl';
		w.id.value=cid;
		w.nick.value=nick;
		w.msg.value=s;
		w.submit();
		return;
	}
}

function hide_msgs(obj,n)
{
	var st=document.styleSheets[0].rules[n].style;
	if(parent.h_msg[n]==0){
		st.display='none';
        if(obj) obj.className='bp';
        parent.h_msg[n]=1;
	}else{
		if(obj) obj.className='';
		st.display="block";
        parent.h_msg[n]=0;
	}
	ftime=1;
	if(obj) window.top.addmessage.document.SendForm.msgbox.focus();
}

function init_msg()
{
	if(parent.h_msg[7]==1) {
		parent.h_msg[7]=0;
		hide_msgs(null,7);
	}
	if(parent.h_msg[8]==1) {
		parent.h_msg[8]=0;
		hide_msgs(null,8);
	}
}

function DS()
{
	if(parent.scr==1)
	{
		scroll(1,10000000);
	}
}

function os()
{
	window.status='';
	return true;
}

function RefreshMain()
{
	window.top.location.reload();
}
function bl()
{
    if(parent.ex==0){
    	parent.connected=false;
		alert('Внимание! Связь с сервером потеряна. Возможно перезагружен сервер.');
//		window.top.location.reload();
//		setTimeout("RefreshMain()",1000);
	}	
}
