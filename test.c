#include <stdio.h>
#include <sys/stat.h>
#include <stdlib.h>
#include "DocxToXml.h"
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
	/*
	int re=DocxToXml2(buf,filestat.st_size,&document_fp,&document_size,&settings_fp,&settings_size);
	if(re!=0)
	{
		printf("DocxToXml error %d\n",re);
	}
	*/
	int re=DocxToXml(buf,filestat.st_size,"document.xml",&document_fp,&document_size);
	if(re!=0)
	{
		printf("DocxToXml error %d\n",re);
	}
	re=DocxToXml(buf,filestat.st_size,"settings.xml",&settings_fp,&settings_size);
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
