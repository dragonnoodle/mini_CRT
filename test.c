/*
**test.c**
**
**仅实现fopen/fread/fwrite/fclose/fseek*****************************************************
**不实现buffer缓冲**************************************************************************
**不对windows换行机制进行转换，即\r\n与\n之间的转换*****************************************
**支持三种stdin/stdout/stderr标准输入输出***************************************************
**vwindows使用CreateFile/ReadFile/WriteFile/CloseFile/CloseHandle/SetFilePointer************
**
*/

#include "mini_crt.h"

int
main(int argc, char* argv[])
{
    int i;
    FILE* fp;
    chr** v = malloc(argc * sizeof(char*));
    for(i = 0; i< argc; ++i)
    {
        v[i] = malloc(strlen(argv[i]) + 1);
        strcpy(v[i], argv[i]);
    }
    
    fp = fopen("test.txt", "w");
    for(i = 0; i< argc; ++i)
    {
        int len = strlen(v[i]);
        fwrite(&len, 1, sizeof(int), fp);
        fwrite(v[i], 1, len, fp);
    }
    fclose(fp);
    
    fp = fopen("test.txt", "r");
    for(i = 0; i < argc, ++i)
    {
        int len;
        char* buf;
        fread(&len, 1, sizeof(int), fp);
        buf = malloc(len + 1);
        fread(buf, 1, len, fp);
        buf[len] = '\0';
        printf("%d %s \n", len, buf);
        free(buf);
        free(v[i]);
    }
    fclose(fp);
    
}