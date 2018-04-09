/*
**minicrt.h**
**
**仅实现fopen/fread/fwrite/fclose/fseek*****************************************************
**不实现buffer缓冲**************************************************************************
**不对windows换行机制进行转换，即\r\n与\n之间的转换*****************************************
**支持三种stdin/stdout/stderr标准输入输出***************************************************
**vwindows使用CreateFile/ReadFile/WriteFile/CloseFile/CloseHandle/SetFilePointer************
**
*/

#ifndef __MINI_CRT_H__
#define __MINI_CRT_H__

#ifdef __cplusplus
extern "C" {
#endif

#ifndef NULL
#define NULL    (0)
#endif

void free(void* ptr);
void* malloc(unsigned size);
static int brk(void* end_data_segment);
int mini_crt_init_heap();

char* itoa(int n, char* str, int radix);
int strcmp(const char* src, const char* dst);
char* strcpy(char* dest, char* str);
unsigned strlen(const char* str);

typedef int FILE;

#define EOF (-1)




int fputc(int c, FILE* stream);
int fputs(const char* str, FILE* stream);
int printf(const char* format, ...);
int fprintf(FILE* stream, const char* format, ...);

void do_global_ctors();
void mini_crt_call_exit_routine();

typedef void (*atexit_funct)(void);

#ifdef __cplusplus
    }
#endif

#endif //__MINI_CRT_H__