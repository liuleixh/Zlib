#include <stdio.h>
#include <sys/stat.h>
#include <zlib.h>
#include "unzip.h"
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
		return 0;
	}
	unzFile zFile=unzOpen64(buf,filestat.st_size);
	if(zFile == NULL)
	{
		printf("error\n");
	}
	else
	{
		printf("success\n");
	}
	unz_global_info64 zGlobalInfo;
	if(UNZ_OK!=unzGetGlobalInfo64(zFile,&zGlobalInfo))
	{
		printf("error\n");
	}
	printf("%d\n",(int)zGlobalInfo.number_entry);
	unz_file_info64 zFileInfo;
	char filename[512];
	for(int i=0;i<zGlobalInfo.number_entry;i++)
	{
		if(UNZ_OK!=unzGetCurrentFileInfo64(zFile,&zFileInfo,filename,512,NULL,0,NULL,0))
		{
			printf("error\n");
		}
		printf("%s\n",filename);
		unzGoToNextFile(zFile);
	}
	unzClose(zFile);
	free(buf);
	return 0;
}
