#include "types.h"
#include "x86.h"
#include "defs.h"
#include "date.h"
#include "param.h"
#include "memlayout.h"
#include "mmu.h"
#include "proc.h"

int
sys_fork(void)
{
  return fork();
}

int
sys_exit(void)
{
  exit();
  return 0;  // not reached
}

int
sys_wait(void)
{
  return wait();
}

int
sys_kill(void)
{
  int pid;

  if(argint(0, &pid) < 0)
    return -1;
  return kill(pid);
}

int
sys_getpid(void)
{
  return myproc()->pid;
}

int
sys_sbrk(void)
{
  int addr;
  int n;

  if(argint(0, &n) < 0)
    return -1;
  addr = myproc()->sz;
  if(growproc(n) < 0)
    return -1;
  return addr;
}

int
sys_sleep(void)
{
  int n;
  uint ticks0;

  if(argint(0, &n) < 0)
    return -1;
  acquire(&tickslock);
  ticks0 = ticks;
  while(ticks - ticks0 < n){
    if(myproc()->killed){
      release(&tickslock);
      return -1;
    }
    sleep(&ticks, &tickslock);
  }
  release(&tickslock);
  return 0;
}

// return how many clock tick interrupts have occurred
// since start.
int
sys_uptime(void)
{
  uint xticks;

  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}

int
sys_memtop(void)
{
	return memtop();
}

int
sys_getmeminfo(void)
{
	// Fetch all pil from ptables
	int iter_p;
	char namebuf[16];
	int name_len = 16;
	int allocated_mem = 0;
	// execute in parent process

	for(iter_p = 1; iter_p < NPROC; iter_p++){
		memset(namebuf, 0, name_len);
		allocated_mem = getmeminfo(iter_p, namebuf, name_len);
		if(allocated_mem > 0){
			cprintf("pid: %d, name: %s, mem: %d\n", iter_p, namebuf, allocated_mem);
		}
	}

	return 0;
}

int
sys_thread_create(void)
{
  char *fcn;
  char *arg;
  char *stack;


  if(argptr(0, &fcn, sizeof(uint)) < 0 || argptr(1, &arg, sizeof(uint)) < 0 || argptr(2, &stack, sizeof(uint)) < 0){
    cprintf("Error: input argument can't be parsed in thread create syscall!/n");
    return -1;
  }

  return thread_create((void(*)(void*))fcn, arg, stack);
}

int
sys_thread_join(void)
{
	return thread_join();
}

int
sys_thread_exit(void)
{
	return thread_exit();
}

int
sys_sleep_and_release_mutex(void){

  char* cond;
  char* lock;

  if(argptr(0, &cond, sizeof(char*)) < 0 || argptr(1, &lock, sizeof(uint*)) < 0){
    cprintf("Error: input argument can't be parsed in sleep and release mutex syscall!/n");
    return -1;
  }

  return sleep_and_release_mutex(cond, lock);
}

int
sys_user_cond_wakeup(void){

  char* cond;

  if(argptr(0, &cond, sizeof(char*)) < 0){
    cprintf("Error: input argument can't be parsed in user cond wakeup!/n");
    return -1;
  }

  return user_cond_wakeup(cond);
}
