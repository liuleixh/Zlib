#ifndef _DOCXTOXML_H
#define _DOCXTOXML_H

/*
 * filebuf            Docx文件流的指针 
 * file_size          Docx文件流的大小
 * document_fp        document.xml文件的内存指针 此处传入的是指针地址 必须初始化为NULL 使用完毕后必须free
 * document_size	  document.xml文件的大小
 * settings_fp        settings.xml文件的内存指针 此处传入的是指针地址 必须初始化为NULL 使用完毕后必须free
 * settings_size	  settings.xml文件的大小
 */
int DocxToXml2(char *filebuf,size_t file_size,char **document_fp,size_t *document_size,char **settings_fp,size_t *settings_size);

/*
 * filebuf            Docx文件流的指针 
 * file_size          Docx文件流的大小
 * xml_name           xml文件的名称
 * xml_fp             xml文件的内存指针 此处传入的是指针地址 必须初始化为NULL 使用完毕后必须free
 * xml_size	          xml文件的大小
 */
int DocxToXml(char *filebuf,size_t file_size,char *xml_name,char **xml_fp,size_t *xml_size);


#endif
