#include <zlib.h>
#include "unzip.h"
int main()
{

	unzFile zFile=unzOpen64("t.zip");
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
	return 0;
}
