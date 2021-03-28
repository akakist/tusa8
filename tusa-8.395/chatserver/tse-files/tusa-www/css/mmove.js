function mOvr(src,tp)
{
	if(!tp) tp=0;
	src.style.cursor='hand';
	src.style.backgroundColor=document.styleSheets[0].rules[tp+1].style.backgroundColor;
}
function mOut(src,tp)
{
	if(!tp) tp=0;
	src.style.cursor='default';
	src.style.backgroundColor=document.styleSheets[0].rules[tp].style.backgroundColor;
}
function mClk(src) 
{
	if(event.srcElement.tagName=='TD') src.children.tags('A')[0].click();
}
