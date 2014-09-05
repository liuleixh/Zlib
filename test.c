#include <stdio.h>
#include <sys/stat.h>
#include <zlib.h>
#include <string.h>
#include "unzip.h"

int DocxToXml(char *filebuf,size_t file_size,char **document_fp,size_t *document_size,char **settings_fp,size_t *settings_size)
{
	int result=0,document_flag=0,settings_flag=0;
	if(filebuf==NULL||file_size==0||*document_fp!=NULL||*settings_fp!=NULL)
	{
		perror("parameters error\n");
		result = 1;
		return result;
	}
	unzFile zFile=unzOpen64(filebuf,file_size);
	if(zFile == NULL)
	{
		printf("open docx's memory error\n");
		result = 2;
		return result;
	}
	unz_global_info64 zGlobalInfo;
	if(UNZ_OK!=unzGetGlobalInfo64(zFile,&zGlobalInfo))
	{
		printf("get zip globalinfo error\n");
		result =  3;
		goto End;
	}
	//printf("%d\n",(int)zGlobalInfo.number_entry);
	unz_file_info64 zFileInfo;
	char filename[512];
	for(int i=0;i<zGlobalInfo.number_entry;i++)
	{
		if(UNZ_OK!=unzGetCurrentFileInfo64(zFile,&zFileInfo,filename,512,NULL,0,NULL,0))
		{
			printf("get current fileinfo error\n");
			result = 4;
			goto End;
		}
		if(0==strcmp(filename+strlen(filename)-strlen("document.xml"),"document.xml"))
		{
			if(UNZ_OK!=unzOpenCurrentFile(zFile))
			{
				printf("find document.xml,but open document.xml error\n");
				result = 5;
				goto End;
			}
			*document_fp=(char *)malloc(sizeof(char)*zFileInfo.uncompressed_size);
			if(*document_fp==NULL)
			{
				printf("malloc document_fp error\n");
				result = 6;
				unzCloseCurrentFile(zFile);
				goto End;
			}
			int len=unzReadCurrentFile(zFile,(voidp)*document_fp,zFileInfo.uncompressed_size);
			if(len!=zFileInfo.uncompressed_size)
			{
				printf("read document.xml error\n");
				result = 7;
				unzCloseCurrentFile(zFile);
				goto End;
			}
			*document_size=zFileInfo.uncompressed_size;
			unzCloseCurrentFile(zFile);
			document_flag=1;
		}
		if(strstr(filename,"settings.xml"))
		{
			if(UNZ_OK!=unzOpenCurrentFile(zFile))
			{
				printf("find settings.xml,but open settings.xml error\n");
				result = 8;
				goto End;
			}
			*settings_fp=(char *)malloc(sizeof(char)*zFileInfo.uncompressed_size);
			if(*settings_fp==NULL)
			{
				printf("malloc settings_fp error\n");
				result = 9;
				unzCloseCurrentFile(zFile);
				goto End;
			}
			int len=unzReadCurrentFile(zFile,(voidp)*settings_fp,zFileInfo.uncompressed_size);
			if(len!=zFileInfo.uncompressed_size)
			{
				printf("read settings.xml error\n");
				result = 10;
				unzCloseCurrentFile(zFile);
				goto End;
			}
			*settings_size=zFileInfo.uncompressed_size;
			unzCloseCurrentFile(zFile);
			settings_flag=1;
		}
		//printf("%s\n",filename);
		unzGoToNextFile(zFile);
	}
	if(document_flag==0)
	{
		printf("can not find document.xml\n");
		result = 11;
		goto End;
	}
	if(settings_flag==0)
	{
		printf("can not find settings.xml\n");
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
int main()
{
	struct stat filestat;
	stat("t.zip",&filestat);
	char *buf=(char *)malloc(filestat.st_size);
	if(buf==NULL)
	{
		printf("malloc error\n");
		return 0;
	}
	FILE *fp=fopen("t.zip","r");
	if(fp==NULL)
	{
		printf("fopen error\n");
		return 0;
	}
	size_t num=fread(buf,1,filestat.st_size,fp);
	if(num!=filestat.st_size)
	{
		printf("fread error\n");
		fclose(fp);
		return 0;
	}
	fclose(fp);
	char *document_fp=NULL,*settings_fp=NULL;
	size_t document_size=0,settings_size=0;
	int re=DocxToXml(buf,filestat.st_size,&document_fp,&document_size,&settings_fp,&settings_size);
	if(re!=0)
	{
		printf("DocxToXml error %d\n",re);
	}
	FILE *document=fopen("documentbak.xml","w");
	num=fwrite(document_fp,1,document_size,document);
	if(num!=document_size)
	{
		printf("write document error\n");
	}
	fclose(document);
	FILE *settings=fopen("settingsbak.xml","w");
	num=fwrite(settings_fp,1,settings_size,settings);
	if(num!=settings_size)
	{
		printf("write document error\n");
	}
	fclose(settings);
	printf("%d\n",re);
	free(document_fp);
	free(settings_fp);
	free(buf);
	return 0;
}
