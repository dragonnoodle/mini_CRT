/*
**entry.c**
**
**初始化I/O和初始化堆**************************************************
**获取arc和arv参数以及间接获取环境变量*********************************
**实现exit函数（调用80号中断）*****************************************
**
*/
#include "minicrt.h"

#ifdef WIN32
#include <Windows.h>
#endif

extern int main(int argc, char * argv[]);
void exit(int exitCode );

static int
crt_fatal_error(const char* msg)
{
	printf("fatal error:%s\n",msg);
	exit(1);
}

void
mini_crt_entry(void)
{
    int ret;
    
/*开始获取命令行参数*/
#ifdef WIN32
	int flag = 0;
	int argc = 0;
	char *argv[16];
	char *cl = GetCommandLineA();
	
	argv[0] = cl;
	argc++;

	while(*cl)/*when "string" after a space ,apart string with '\0' */
	{
	   if(*cl == '\"')
       {
           if(flag == 0)
		    flag = 1;
            else flag = 0;
       }
	   else if(*cl == ' ' && flag == 0)
	   {
		   if(*(cl+1))
		   {
			   argv[argc] = cl +1;
			   argc++;
		   }
	   *cl = '\0';
		}
	
	cl++;
	}
#else

	int argc;
	char **argv;
	char *ebp_reg = 0;

	asm("movl %%ebp, %0 \n":"=r"(ebp_reg));

	argc = *(int*)(ebp_reg + 4);
	argv = (char**)(ebp_reg + 8);

#endif   
/*结束获取命令行参数*/

	if(!mini_crt_heap_init())                   //初始化堆栈
	     crt_fatal_error("heap init failed");
	if(!mini_crt_io_init())                     //初始化I/O
		crt_fatal_error("io init failed");
	
	ret = main(argc, argv);                     //执行main函数
	exit(ret);                                  //退出函数，清理进程
}

void
exit(int exitCode)
{
	//mini_crt_call_exit_routine();
#ifdef WIN32
	ExitProcess(exitCode);
#else
	asm("movl %0, %%ebx \n\t"
		"movl $1,%%eax \n\t"
		"int $0x80   \n\t"
		"hlt   \n\t"::"m"(exitCode));//调用int80号中断，__asm__ 模板：输出：输入
#endif
	
	
}

