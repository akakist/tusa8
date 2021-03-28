//#include "stdafx.h"
#include <stdio.h>
#include <string>
//#include "st_stuff.h"
//#include "str_lib.h"
//#include "mysql_extens.h"
#include <time.h>
//#include "copy_file.h"
#include <FreeImage.h>
#include <sys/types.h> 
#include <sys/stat.h>  
#ifndef WIN32
#include <unistd.h>    
#endif
using namespace std;
#pragma comment(lib, "FreeImage.lib")
void copy_to(const string& from, const string& to)
{
	struct stat st;
	if(stat(from.c_str(),&st))
	{
		printf(" cannot stat %s\n",from.c_str());
		return;
	}
	FILE *f=fopen(from.c_str(),"rb");
	if(!f)
	{
		printf(" cannot fopen %s\n",from.c_str());
		return;

	}
	char *buf=(char*) malloc(st.st_size);
	fread(buf,st.st_size,1,f);
	fclose(f);
	f=fopen(to.c_str(),"wb");
	if(!f)
	{
		printf(" cannot fopen %s\n",to.c_str());
		return;

	}
	fwrite(buf,st.st_size,1,f);
	fclose(f);
	free(buf);
	printf("copy to success %s -> %s",from.c_str(),to.c_str());
}
void scale(double maxsize,const string& from, const string& to)
{

	try{
		printf("scale %s to %s\n", from.c_str(), to.c_str());
		FREE_IMAGE_FORMAT  fif=FreeImage_GetFIFFromFilename(from.c_str());
		if(fif!=FIF_JPEG)
		{	
			printf("fif!=FIF_JPEG\n");
			return;
		}
		FIBITMAP *dib=FreeImage_Load(fif, from.c_str());

		int w=FreeImage_GetWidth(dib);
		int h=FreeImage_GetHeight(dib);

		int max=w;
		bool use_w=true;
		if(h>max)
		{	
			max=h;
			use_w=false;
		}

		if(max>maxsize)
		{
			float scalefactor=maxsize;
			scalefactor/=max;
			w=scalefactor*w;
			h=scalefactor*h;
			FIBITMAP *cvt_dib=FreeImage_ConvertTo24Bits(dib);
			FIBITMAP *scaleddib=FreeImage_Rescale(cvt_dib, w, h, FILTER_BILINEAR);
			FreeImage_Save(FIF_JPEG, scaleddib, to.c_str());
		}
		else
		{

			copy_to(from,to);
			//		link(from.c_str(),to.c_str());
		}
	}catch(...)	    
	{
		remove(from.c_str());
	}

}
void thumb(double x,double y,const string& f_from,const string& f_to)
{
	printf("thumb  %f %f %s %s\n",x,y, f_from.c_str(),f_to.c_str());
	FREE_IMAGE_FORMAT  fif=FreeImage_GetFIFFromFilename(f_from.c_str());
	if(fif!=FIF_JPEG)
	{	
		printf("fif!=FIF_JPEG\n");
		return;
	}
	FIBITMAP *dib=FreeImage_Load(fif, f_from.c_str());

	double X=FreeImage_GetWidth(dib);
	double Y=FreeImage_GetHeight(dib);
	printf("X %f Y %f\n",X,Y);
	//double w_ratio=width; required_ratio/=height;
	//double w1=w;

	

	double scaleX=x/X;
	double scaleY=y/Y;
	printf("scaleX * Y (%f) >y(%f)\n",scaleX * Y,y);
	printf("scaleY * X (%f) >x(%f)\n",scaleY * X,x);
	if(scaleX * Y>y && scaleX!=0)
	{
		printf("use Y clean\n");
		printf("full clip %f\n",Y-y/scaleX);
		double d=(Y-y/scaleX)/2;
		dib=FreeImage_Copy(dib,0,d,X,Y-d);
		if(FreeImage_GetWidth(dib)>x && FreeImage_GetHeight(dib)>y)
		    dib=FreeImage_Rescale(dib, x, y, FILTER_BILINEAR);
		FreeImage_Save(FIF_JPEG, dib, f_to.c_str());
	}
	if(scaleY * X>x && scaleY!=0)
	{
		printf("use X clean\n");
		//printf("full clip %f\n",X-scaleY * X);
		double d=(X-x/scaleY)/2;
		dib=FreeImage_Copy(dib,d,0,X-d,Y);

		if(FreeImage_GetWidth(dib)>x && FreeImage_GetHeight(dib)>y)
		    dib=FreeImage_Rescale(dib, x, y, FILTER_BILINEAR);

		FreeImage_Save(FIF_JPEG, dib, f_to.c_str());
	}

	printf("w scaledY %f %f\n",scaleY*x,scaleY);
	printf("h scaledX %f %f\n",scaleX*y,scaleX);
	return;

}
int main(int argc,char *argv[])
{

    try{
	//	FILE *f=fopen("clear_users.log","a");
	FreeImage_Initialise();

	if(argc<2) return 1;
#ifndef WIN32
	if(string(argv[1])=="rotate")
	{
		// _img_cvt rotate 90 fn
		if(argc<4)
		{
			printf("_img_cvt rotate 90 fn\n");
			return 1;
		}
		string angle=argv[2];
		string fn=argv[3];
		string fn_tmp=fn+".tmp";
		if(angle=="90") FreeImage_JPEGTransform(fn.c_str(),fn_tmp.c_str(),FIJPEG_OP_ROTATE_90);
		else if(angle=="180") FreeImage_JPEGTransform(fn.c_str(),fn_tmp.c_str(),FIJPEG_OP_ROTATE_180);
		else if(angle=="270") FreeImage_JPEGTransform(fn.c_str(),fn_tmp.c_str(),FIJPEG_OP_ROTATE_270);
		unlink(fn.c_str());
		copy_to(fn_tmp,fn);
		unlink(fn_tmp.c_str());
	    return 0;
	}
#endif
	if(string(argv[1])=="scale")
	{	
		// used to scale to store in format 1000 pixel of max VH
		// _img_cvt scale from_pn to_pn
		if(argc<5)
		{
			printf("_img_cvt #maxsize scale from_pn to_pn\n");
			return 1;
		}
		scale(atof(argv[2]),argv[3],argv[4]);
		return 0;
	}
//
	if(string(argv[1])=="thumb")
	{
		
		if(argc<6){
			printf("_img_cvt  thumb width height file_from file_to\n");
			return 1;
		}
		int width=atoi(argv[2]);
		int height=atoi(argv[3]);
		string f_from=argv[4];
		string f_to=argv[5];
		thumb(width,height,f_from,f_to);
		return 0;
	}
//#ifndef WIN32
	if(string(argv[1])=="scale")
	{
		if(argc<5)return 1;
		int nw=atoi(argv[2]);
		string from=argv[3];
		string to=argv[4];
//		check_path_wfn(to);
//		string f=strlower()
		FREE_IMAGE_FORMAT  fif=FreeImage_GetFIFFromFilename(from.c_str());
		if(fif!=FIF_JPEG) return 1;
		FIBITMAP *dib=FreeImage_Load(fif, from.c_str());

		int w=FreeImage_GetWidth(dib);
		int h=FreeImage_GetHeight(dib);
		int max=w;
		//		int max=w;
		//		if(h>max) max=h;
		if(max>nw)
		{
			float sf=nw;
			sf/=max;
			w=sf*w;
			h=sf*h;
			FIBITMAP *cvt_dib=FreeImage_ConvertTo24Bits(dib);
			FIBITMAP *scaleddib=FreeImage_Rescale(cvt_dib, w, h, FILTER_BILINEAR);
			FreeImage_Save(FIF_JPEG, scaleddib, to.c_str());
		}
		else
		{

			copy_to(from,to);
		}

		return 0;

	}
	/*
	if(string(argv[1])=="hscale")
	{
		if(argc<5)return 1;
		int nw=atoi(argv[2]);
		string from=argv[3];
		string to=argv[4];
//		check_path_wfn(to);
		FREE_IMAGE_FORMAT  fif=FreeImage_GetFIFFromFilename(from.c_str());
		if(fif!=FIF_JPEG) return 1;
		FIBITMAP *dib=FreeImage_Load(fif, from.c_str());

		int w=FreeImage_GetWidth(dib);
		int h=FreeImage_GetHeight(dib);
		int max=h;
		//		if(h>max) max=h;
		if(max>nw)
		{
			float sf=nw;
			sf/=max;
			w=sf*w;
			h=sf*h;
			FIBITMAP *cvt_dib=FreeImage_ConvertTo24Bits(dib);
			FIBITMAP *scaleddib=FreeImage_Rescale(cvt_dib, w, h, FILTER_BILINEAR);
			FreeImage_Save(FIF_JPEG, scaleddib, to.c_str());
		}
		else
		{

			copy_to(from,to);
		}


	}*/
	
//#endif	
	printf("invalid call img_cvt \n");
	}catch(...)
	{
	    
	}

	return 0;
}
