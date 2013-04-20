#include <stdio.h>
#include <pthread.h>
#include <string.h>
#include <stdlib.h>
#include "stdint.h"
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <unistd.h>
#include <sys/syscall.h> 


#if defined(__x86_64__)  
#define ATOMICOPS_WORD_SUFFIX "q"  
#else
#define ATOMICOPS_WORD_SUFFIX "l" 
#endif
static inline bool compare_and_swap(volatile uint64_t *p, uint64_t val_old, uint64_t val_new)
{
	char ret;
	__asm__ __volatile__("lock; cmpxchg" ATOMICOPS_WORD_SUFFIX " %3, %0; setz %1"
			: "=m"(*p), "=q"(ret)
			: "m"(*p), "r" (val_new), "a"(val_old)
			: "memory");
	return (bool)ret;
}


static inline uint64_t fetch_and_add(volatile uint64_t *p, uint64_t add)
{
        unsigned int ret;
        __asm__ __volatile__("lock; xaddl %0, %1"
                        :"=r" (ret), "=m" (*p)
                        : "0" (add), "m" (*p)
                        : "memory");
        return ret;
};
static inline int test_and_set(volatile int *s)    /* tested */
{       int r;
        __asm__ __volatile__("xchgl %0, %1"
                        : "=r"(r), "=m"(*s)
                        : "0"(1), "m"(*s)
                        : "memory");

        return r;
}

static inline int reset(volatile int *s)
{
        *s = 0;
}
struct lock_free_queue
{
	uint64_t head;
	uint64_t tail;
	int*   m_head;
	volatile int tas_lock;
	int m_size;
	uint64_t size()
	{
		return m_size;
	}
	bool push(int fd) // single producer  , epoll 
	{
		if( size() < 1*1024 )
		{
			m_head[tail] = fd;
			tail =(tail+1)%(2*1024);
			m_size--;
			return true;
		}
		else
		{
			return false;
		}
	};
	bool pop(int& fd) //multiple customers, use test_and_set to make it like single customer
	{
		bool ret = false;
		for(size_t n = 1;(test_and_set(&tas_lock))==1;n<<=1)
		{

			if(n<4096)
			{
				for(size_t i=0;i<n;i++)
				{

					__asm__ ("pause") ;
				}
			}
			else
			{
				syscall(SYS_sched_yield);
				n = 1;
			}
		}

		if( size() >= 1)
		{
			fd = m_head[head];
			head = (head+1)%(2*1024);
			m_size++;
			ret = true;
		}
		else
		{
			ret = false;
		}

		reset(&tas_lock);

		return ret ;
	};
	lock_free_queue()
	{
		m_head  = (int*) malloc((2*1024)*sizeof(int));
		head = 0;
		tail = 0;
		tas_lock = 0;
		m_size = 0;
	};
	~lock_free_queue()
	{
		if( NULL != m_head )
		{
			free(m_head);
			m_head = NULL;
		}	
	}
};

