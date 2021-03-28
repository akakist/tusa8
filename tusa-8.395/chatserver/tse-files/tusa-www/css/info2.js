
var debug=0;
var uid=0;
var thumb_width=90;
//var image_width=450;
var main_img_width=550;
var flist=new Array();
var ilist=new Array();
var page_selected=0;
var first_page=0;
var nphoto=0;

function int0(a)
{
	if(a!=Math.round(a))
	{
		a=Math.floor(a);
	}
	if(a<10) return "0"+a;
	else return a;
}
function new_cache_dir(uid)
{
        uid=uid/100;
    if (uid<100)
    {
        return "/0/"+int0(uid);
    }
    else if (uid<10000)
    {
        u1=uid/100;
        u2=uid%100;
        return "/1/"+int0(u1)+"/"+int0(u2);
    }
    else if (uid<1000000)
    {
        u1=uid/10000;
        u2=(uid-(u1*10000))/100;
        u3=uid%100;
        uid/=100;
        u2=uid%100;
        uid/=100;
        u1=uid%100;
        return  "/2/"+int0(u1)+"/"+int0(u2)+"/"+int0(u3);
    }
    else{
        return "Error limit";
    }
    return "";

}

function thumb_big(pid)
{
        return "http://tusovka.tomsk.ru/thumbs-big"+new_cache_dir(pid)+"/"+pid+".jpeg";
}
function thumb_small(pid)
{
        return "http://tusovka.tomsk.ru/thumbs-small"+new_cache_dir(pid)+"/"+pid+".jpeg";
}

function opwin(l,w,h)
{
	var ow=open(l,'owusr','toolbar=0,status=0,resizable=1,scrollbars=1,width='+w+',height='+h);
	ow.focus();
}
function do_bnick(id)
{
	var r=Math.random();
	locat='/nick_buy?n='+id;
	sx=400;
	sy=200;
	var uni;
	var uni=open(locat,'uni','toolbar=0,status=0,resizable=1,scrollbars=1,width='+sx+',height='+sy);
	uni.focus();
}
function InitFotoList(arr)
{
	var n=0;
	for(var i=0; i<arr.length; )
	{
		var _id=arr[i];i++;
		var _fn=arr[i];i++;
		var _comment=arr[i];i++;
		flist[n]={id:_id,fn:_fn,comment:_comment};
		n++;
	}
	nphoto=n/3;
	return false;
}
function DrawPhotoPagesBar(actv)
{
	if(nphoto==0) return;

	var s="";
	var sz=flist.length;
	if(sz<=24)
	{
		first_page=0;
	}
	else
	{
		if(first_page>sz-24)
		{
			first_page=sz-24;
			first_page=first_page/24;
			first_page*=24;
		}
	}
	if(first_page>0)
	{
		s+="  <a href=\"#\" onclick=\"first_page-=24;if(first_page<0)first_page=0;DrawPhotoPagesBar();return false;\"><<</a> \n";
	}
	for(var i=first_page;i<first_page+24;i+=12)
	{
		if(i>=sz) break;
		var e=i+12;
		if(e>=sz) e=sz-1;
		if(e!=i)
		{
		 var en=12;
		 if(i+12>sz)
		    en=sz%12;
		    if(actv==i)
			s+="<b>["+(i+1)+"-"+(i+en)+"]</b> \n";
		    else
			s+="  <a href=\"#\" onclick=\"return ChangePage("+(i+1)+");\">["+(i+1)+"-"+(i+en)+"]</a> \n";
		}
		else
		{
		    if(actv==i)
		    s+=(i+1)+" \n";
		    else
		    s+="  <a href=\"#\" onclick=\"return ChangePage("+(i+1)+");\">["+(i+1)+"]</a> \n";
		}

	}
	if(sz>first_page+24)
		s+="  <a href=\"#\" onclick=\"first_page+=24;DrawPhotoPagesBar();return false;\">>></a> \n";

	document.getElementById("fotopages").innerHTML=s;

}
function SelectLocal(n)
{
	var nn=page_selected+n;
	ShowImage(nn);
	return false;
}
function ShowImage(n)
{
	if(n<flist.length)
	{
		document.getElementById("cap").innerHTML="<b>N"+(n+1)+"</b>";
		//flist[n].caption;
		document.getElementById("descr").innerHTML=flist[n].comment;
		document.mainimg.src=thumb_big(flist[n].id);
	}
	return true;
}
/*function get_thumb(resx,resy,id,fn)
{
	if(debug==1)
		return fn;
	else
		return "/cna/photos/"+uid+"/"+id+"/thumb/"+resx+"x"+resy+"/"+fn;
}
*/

/*function get_url(res,id,fn)
{
	if(debug==1)
		return fn;
	else
		return "/cna/photos/"+uid+"/"+id+"/scale/"+res+"/"+fn;

}
*/
function BuildThumbs()
{
	var start_f=page_selected;
	var end_f=start_f+12;
	if(flist.length<2)
	{
		document.getElementById("thumbs").innerHTML="";
		return;

	}

	var idx=start_f;
	var s="";
	s+='<table>';
	for(var i=0;i<4;i++)
	{
		s+="<tr>";
		for(var j=0;j<3;j++)
		{

			if(idx<flist.length)
			{
				s+='<td><img width="85" height="110"  style="border: 1px solid black;" src="'+thumb_small(flist[idx].id)+'"'+ "onclick='return ShowImage("+idx+");'" +'></td>';
				idx++;
			}
			else
				s+='<td>&nbsp;</td>';
		}
		s+="</tr>";
	}
	document.getElementById("thumbs").innerHTML=s;

}
function ChangePage(n)
{

	if(nphoto==0) return;
	var nn=n-1;
	DrawPhotoPagesBar(nn);
	page_selected=nn;
	document.mainimg.src=thumb_big(flist[nn].id);
	ShowImage(nn);
	BuildThumbs();

	return false;
}
function BuildInfoList(arr)
{
	var s="";
	s+='<table cellpadding=4 cellspacing=4 border=0 align=center width=500>';
	for(var i=0;i<arr.length;)
	{
		var key=arr[i];i++;
		var val=arr[i];i++;
		s+='<tr><td class=infoleft>'+key+'</td><td class=inforight><b>'+val+'</a></td></tr>'+"\n";

	}
	s+='</table>';
	document.getElementById("infotable").innerHTML=s;
}
function SelectTabPage(n)
{
        BuildMainTab(n);
	if(n!=2)
		document.getElementById("fotopage").style.display="none";
	else
		document.getElementById("fotopage").style.display="";
	if(n!=1)
		document.getElementById("infopage").style.display="none";
	else
		document.getElementById("infopage").style.display="";
	return false;
}

function BuildMainTab(actv)
{
	var s="";

	s+="<table cellpadding=0 cellspacing=0 border=0 align=center><tr>";

	      if(actv==1)
	       s+='<td id="tab1"><b>Инфа</b> ::&nbsp;</td>';
	      else
	      	s+='<td id="tab1"><a class=infolink href="#" onclick="return SelectTabPage(1);">Инфа</a> ::&nbsp;</td>';
	if(nphoto>0)
	{
	      if(actv==2)
	        s+='<td id="tab2"><b>Фотки</b></td>';
	      else
	      	s+='<td id="tab2"><a class=infolink href="#" onclick="return SelectTabPage(2);">Фотки</a></td>';

	 }
	 s+='</tr></table>';
//
	document.getElementById("main_tab").innerHTML=s;
}

