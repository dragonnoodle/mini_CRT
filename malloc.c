/*
**malloc.c**
**
**利用空闲链表算法实现堆空间的分配**************************************
**堆固定32M,初始化后不再扩展或缩小**************************************
**Windows使用virtualAlloc向系统申请而不使用heapalloc等堆分配算法********
**linux使用brk将数据段结束地址向后调整32M，作为堆空间*******************
**virtualAlloc 和brk/sbrk仅仅是分配虚拟空间，一开始不会分配物理页面，***
**当进程访问某一地址时，才将被访问地址所在页分配物理页******************
*/
#include "minicrt.h"

typedef struct _heap_header
{
	enum
	{
		HEAP_BLOCK_FREE = 0xABABABAB,
		HEAP_BLOCK_USED = 0xCDCDCDCD,
	}type;
	unsigned size;
	struct _heap_header *next;
	struct _heap_header *prev;
}heap_header;

#define ADDR_ADD(a,o) (((char*)(a)) + o)
#define HEADER_SIZE    (sizeof(heap_header))

static heap_header *list_head = NULL;

void 
free(void* ptr)                         /*ptr指向header后的内存空间*/
{
	heap_header *header = (heap_header*)ADDR_ADD(ptr, -HEADER_SIZE);
	if(header->type != HEAP_BLOCK_USED)
		return;
	header->type = HEAP_BLOCK_FREE;
	if(header->prev != NULL && header->type == HEAP_BLOCK_FREE)
	{	header->prev->next = header->next;       /*双向链表，此为前表指向后表*/
	if(header->next != NULL)
		header->next->prev = header->prev;      /*双向链表，此为后表指向前表*/
	header->prev->size +=header->size;
	header = header->prev;
	}
	if(header->next != NULL && header->next->type == HEAP_BLOCK_FREE)
	{   
        header->next = header->next->next;       /*双向链表，此为前表指向后表*/
        header->size +=header->next->size;       /*size为header+ptr_size*/
	}
}


void*
malloc(unsigned size)
{
    heap_header *header;
    
    if(size == 0)
        return NULL;
    header = list_head;
    while(header != NULL)
    {
        if(header->type == HEAP_BLOCK_USED)
        {
            header = header->next;
            continue;
        }
        if(header->size > size + HEADER_SIZE
            && header->size < size + HEADER_SIZE*2)
            {
                header->type = HEAP_BLOCK_USED;
            }
        if(header->size > size + 2*HEADER_SIZE)
        {
            heap_header* next = (heap_header*)ADDR_ADD(header,size+
                                HEADER_SIZE);
            next->prev = header;
            next->next = header->next;
            next->type = HEAP_BLOCK_FREE;
            next->size = header->size - (size - HEADER_SIZE);
            header->next = next;
            header->size = size +HEADER_SIZE;
            header->type = HEAP_BLOCK_USED;
            return ADDR_ADD(header, HEADER_SIZE);
        }
        header = header->next;
    }
    return NULL;
}

#ifndef WIN32
// linux brk system call
static int
brk(void* end_data_segment)
{
    int ret =0;
    //brk system call number 45 in /usr/include/asm-i386/unistd.h
    //#define __NR_brk 45
    
    asm("movl $45, %%eax   \n\t"
        "movl %1, %%ebx     \n\t"
        "int $0x80          \n\t"
        "movl %%eax, %0     \n\t"
        : "=r"(ret):"m"(end_data_segment));
        
}
#endif

#ifdef WIN32
#include <Windows.h>
#endif

int
mini_crt_heap_init(void)
{
    void* base = NULL;
    heap_header* header = NULL;
    unsigned heap_size = 1024 * 1024 *1024; //32M heap size
    
#ifdef WIN32
    base = VirtualAlloc(0, heap_size, MEM_COMMIT | 
                        MEM_RESERVE, PAGE_READWRITE);
    if(base == NULL)
        return 0;
#else
    base = (void*)brk(0);
    void* end = ADDR_ADD(base, heap_size);
    end = (void*)brk(end);
    if(!end)
        return 0;
#endif

    header = (heap_header*)base;
    header->size = heap_size;
    header->type =HEAP_BLOCK_FREE;
    header->next = NULL;
    header->prev = NULL;
    
    list_head = header;
    return 1;
        
}



