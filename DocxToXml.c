#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <zlib.h>
#include <string.h>
#include "unzip.h"
#define DEBUG
#ifdef DEBUG
#define Debug_Print(format,args...) fprintf(stderr,format,args)
#else
#define Debug_Print(format,args...)
#endif


int DocxToXml2(char *filebuf,size_t file_size,char **document_fp,size_t *document_size,char **settings_fp,size_t *settings_size)
{
	int result=0,document_flag=0,settings_flag=0;
	if(filebuf==NULL||file_size==0||*document_fp!=NULL||*settings_fp!=NULL)
	{
		Debug_Print("File:%s,line:%d,func:%s,errno:%d,strerror:%s,note:%s\n",__FILE__,__LINE__,__func__,errno,strerror(errno),"parameters error");
		result = 1;
		return result;
	}
	unzFile zFile=unzOpen64(filebuf,file_size);
	if(zFile == NULL)
	{
		Debug_Print("File:%s,line:%d,func:%s,errno:%d,strerror:%s,note:%s\n",__FILE__,__LINE__,__func__,errno,strerror(errno),"open docx's memory error");
		result = 2;
		return result;
	}
	unz_global_info64 zGlobalInfo;
	if(UNZ_OK!=unzGetGlobalInfo64(zFile,&zGlobalInfo))
	{
		Debug_Print("File:%s,line:%d,func:%s,errno:%d,strerror:%s,note:%s\n",__FILE__,__LINE__,__func__,errno,strerror(errno),"get zip globalinfo error");
		result =  3;
		goto End;
	}
	unz_file_info64 zFileInfo;
	char filename[512];
	for(int i=0;i<zGlobalInfo.number_entry;i++)
	{
		if(UNZ_OK!=unzGetCurrentFileInfo64(zFile,&zFileInfo,filename,512,NULL,0,NULL,0))
		{
			Debug_Print("File:%s,line:%d,func:%s,errno:%d,strerror:%s,note:%s\n",__FILE__,__LINE__,__func__,errno,strerror(errno),"get current fileinfo error");
			result = 4;
			goto End;
		}
		if(0==strcmp(filename+strlen(filename)-strlen("document.xml"),"document.xml"))
		{
			if(UNZ_OK!=unzOpenCurrentFile(zFile))
			{
				Debug_Print("File:%s,line:%d,func:%s,errno:%d,strerror:%s,note:%s\n",__FILE__,__LINE__,__func__,errno,strerror(errno),"find document.xml,but open document.xml error");
				result = 5;
				goto End;
			}
			*document_fp=(char *)malloc(sizeof(char)*zFileInfo.uncompressed_size);
			if(*document_fp==NULL)
			{
				Debug_Print("File:%s,line:%d,func:%s,errno:%d,strerror:%s,note:%s\n",__FILE__,__LINE__,__func__,errno,strerror(errno),"malloc document_fp error");
				result = 6;
				unzCloseCurrentFile(zFile);
				goto End;
			}
			int len=unzReadCurrentFile(zFile,(voidp)*document_fp,zFileInfo.uncompressed_size);
			if(len!=zFileInfo.uncompressed_size)
			{
				Debug_Print("File:%s,line:%d,func:%s,errno:%d,strerror:%s,note:%s\n",__FILE__,__LINE__,__func__,errno,strerror(errno),"read document.xml error");
				result = 7;
				unzCloseCurrentFile(zFile);
				goto End;
			}
			*document_size=zFileInfo.uncompressed_size;
			unzCloseCurrentFile(zFile);
			document_flag=1;
		}
		if(0==strcmp(filename+strlen(filename)-strlen("settings.xml"),"settings.xml"))
		{
			if(UNZ_OK!=unzOpenCurrentFile(zFile))
			{
				Debug_Print("File:%s,line:%d,func:%s,errno:%d,strerror:%s,note:%s\n",__FILE__,__LINE__,__func__,errno,strerror(errno),"find settings.xml,but open settings.xml error");
				result = 8;
				goto End;
			}
			*settings_fp=(char *)malloc(sizeof(char)*zFileInfo.uncompressed_size);
			if(*settings_fp==NULL)
			{
				Debug_Print("File:%s,line:%d,func:%s,errno:%d,strerror:%s,note:%s\n",__FILE__,__LINE__,__func__,errno,strerror(errno),"malloc settings_fp error");
				result = 9;
				unzCloseCurrentFile(zFile);
				goto End;
			}
			int len=unzReadCurrentFile(zFile,(voidp)*settings_fp,zFileInfo.uncompressed_size);
			if(len!=zFileInfo.uncompressed_size)
			{
				Debug_Print("File:%s,line:%d,func:%s,errno:%d,strerror:%s,note:%s\n",__FILE__,__LINE__,__func__,errno,strerror(errno),"read settings.xml error");
				result = 10;
				unzCloseCurrentFile(zFile);
				goto End;
			}
			*settings_size=zFileInfo.uncompressed_size;
			unzCloseCurrentFile(zFile);
			settings_flag=1;
		}
		unzGoToNextFile(zFile);
		if(1==document_flag&&1==settings_flag)
			break;
	}
	if(document_flag==0)
	{
		Debug_Print("File:%s,line:%d,func:%s,errno:%d,strerror:%s,note:%s\n",__FILE__,__LINE__,__func__,errno,strerror(errno),"can not find document.xml");
		result = 11;
		goto End;
	}
	if(settings_flag==0)
	{
		Debug_Print("File:%s,line:%d,func:%s,errno:%d,strerror:%s,note:%s\n",__FILE__,__LINE__,__func__,errno,strerror(errno),"can not find settings.xml");
		result = 12;
		goto End;
	}
	unzClose(zFile);
	return result;
End:	
	unzClose(zFile);
	if(*settings_fp!=NULL)
		free(*settings_fp);
	if(*document_fp!=NULL)
		free(*document_fp);
	return result;
}

int DocxToXml(char *filebuf,size_t file_size,char *xml_name,char **xml_fp,size_t *xml_size)
{
	int result=0;
	if(filebuf==NULL||file_size==0||xml_name==NULL||*xml_fp!=NULL)
	{
		Debug_Print("File:%s,line:%d,func:%s,errno:%d,strerror:%s,note:%s\n",__FILE__,__LINE__,__func__,errno,strerror(errno),"parameters error");
		result = 1;
		return result;
	}
	unzFile zFile=unzOpen64(filebuf,file_size);
	if(zFile == NULL)
	{
		Debug_Print("File:%s,line:%d,func:%s,errno:%d,strerror:%s,note:%s\n",__FILE__,__LINE__,__func__,errno,strerror(errno),"open docx's memory error");
		result = 2;
		return result;
	}
	unz_global_info64 zGlobalInfo;
	if(UNZ_OK!=unzGetGlobalInfo64(zFile,&zGlobalInfo))
	{
		Debug_Print("File:%s,line:%d,func:%s,errno:%d,strerror:%s,note:%s\n",__FILE__,__LINE__,__func__,errno,strerror(errno),"get zip globalinfo error");
		result =  3;
		goto End;
	}
	unz_file_info64 zFileInfo;
	char filename[512];
	int i=0;
	for(i=0;i<zGlobalInfo.number_entry;i++)
	{
		if(UNZ_OK!=unzGetCurrentFileInfo64(zFile,&zFileInfo,filename,512,NULL,0,NULL,0))
		{
			Debug_Print("File:%s,line:%d,func:%s,errno:%d,strerror:%s,note:%s\n",__FILE__,__LINE__,__func__,errno,strerror(errno),"get current fileinfo error");
			result = 4;
			goto End;
		}
		if(0==strcmp(filename+strlen(filename)-strlen(xml_name),xml_name))
		{
			if(UNZ_OK!=unzOpenCurrentFile(zFile))
			{
				Debug_Print("File:%s,line:%d,func:%s,errno:%d,strerror:%s,note:find %s,but open %s error\n",__FILE__,__LINE__,__func__,errno,strerror(errno),xml_name,xml_name);
				result = 5;
				goto End;
			}
			*xml_fp=(char *)malloc(sizeof(char)*zFileInfo.uncompressed_size);
			if(*xml_fp==NULL)
			{
				Debug_Print("File:%s,line:%d,func:%s,errno:%d,strerror:%s,note:%s\n",__FILE__,__LINE__,__func__,errno,strerror(errno),"malloc xml_fp error");
				result = 6;
				unzCloseCurrentFile(zFile);
				goto End;
			}
			int len=unzReadCurrentFile(zFile,(voidp)*xml_fp,zFileInfo.uncompressed_size);
			if(len!=zFileInfo.uncompressed_size)
			{
				Debug_Print("File:%s,line:%d,func:%s,errno:%d,strerror:%s,note:read %s error\n",__FILE__,__LINE__,__func__,errno,strerror(errno),xml_name);
				result = 7;
				unzCloseCurrentFile(zFile);
				goto End;
			}
			*xml_size=zFileInfo.uncompressed_size;
			unzCloseCurrentFile(zFile);
			break;
		}
		unzGoToNextFile(zFile);
	}
	if(i==zGlobalInfo.number_entry)
	{
		Debug_Print("File:%s,line:%d,func:%s,errno:%d,strerror:%s,note:can not find %s\n",__FILE__,__LINE__,__func__,errno,strerror(errno),xml_name);
		result = 8;
		goto End;
	}
	unzClose(zFile);
	return result;
End:	
	unzClose(zFile);
	if(*xml_fp!=NULL)
		free(*xml_fp);
	return result;
}
