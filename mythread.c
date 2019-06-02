#include "types.h"
#include "defs.h"
#include "param.h"
#include "memlayout.h"
#include "mmu.h"
#include "x86.h"
#include "proc.h"
#include "spinlock.h"
#include "mythread.h"

extern struct proc* allocproc(void);

extern struct {
  struct spinlock lock;
  struct proc proc[NPROC];
} ptable;

int thread_create(void(*fcn)(void*), void *arg, void*stack){
	// We port fork function and do suitible changes

  int i, pid;
  struct proc *np;
  struct proc *curproc = myproc();

  // Allocate process.
  if((np = allocproc()) == 0){
    return -1;
  }

  // Copy process state from proc.
  np->pgdir = curproc->pgdir;
  np->kstack = curproc->kstack;
  np->sz = curproc->sz;
  np->parent = curproc;
  *np->tf = *curproc->tf;

  // Clear %eax so that fork returns 0 in the child.
  np->tf->eax = 0;

  for(i = 0; i < NOFILE; i++)
    if(curproc->ofile[i])
      np->ofile[i] = filedup(curproc->ofile[i]);
  np->cwd = idup(curproc->cwd);

  safestrcpy(np->name, curproc->name, sizeof(curproc->name));

  pid = np->pid;

  acquire(&ptable.lock);

  np->state = RUNNABLE;

  release(&ptable.lock);

  char* sp = (char*)stack + PGSIZE - sizeof(uint);
  *(uint*)sp = (uint) arg;
  sp -= sizeof(uint);

  *(uint*)sp = 0xffffffff; 
  sp -= sizeof(uint);

  np->tf->esp = (uint) sp;
  np->tf->eip = (uint) fcn;

  return pid;
}

int thread_join(void){
	return 0;
}

int thread_exit(void){
	return 0;
}
