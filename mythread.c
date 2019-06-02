#include "types.h"
#include "defs.h"
#include "param.h"
#include "memlayout.h"
#include "mmu.h"
#include "x86.h"
#include "proc.h"
#include "spinlock.h"
#include "mythread.h"

int thread_create(void(*fcn)(void*), void *arg, void*stack){
	return 0;
}

int thread_join(void){
	return 0;
}

int thread_exit(void){
	return 0;
}
