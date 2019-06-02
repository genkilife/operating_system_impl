#include "types.h"
#include "defs.h"
#include "param.h"
#include "memlayout.h"
#include "mmu.h"
#include "x86.h"
#include "proc.h"
#include "spinlock.h"
#include "mythread.h"

extern struct proc* initproc;
extern struct proc* allocproc(void);
extern void wakeup1(void *);

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

  // if I have runnable child and it's parent is curprocess
  // then we switch to other process
  struct proc *p;
  int havekids, pid;
  struct proc *curproc = myproc();
  
  acquire(&ptable.lock);
  for(;;){
    // Scan through table looking for exited children.
    havekids = 0;
    for(p = ptable.proc; p < &ptable.proc[NPROC]; p++){
      if(p->parent != curproc)
        continue;
      havekids = 1;
      if(p->state == ZOMBIE){
        // Found one.
        pid = p->pid;
        p->kstack = 0;
        p->pid = 0;
        p->parent = 0;
        p->name[0] = 0;
        p->killed = 0;
        p->state = UNUSED;
        release(&ptable.lock);
        return pid;
      }
    }

    // No point waiting if we don't have any children.
    if(!havekids || curproc->killed){
      release(&ptable.lock);
      return -1;
    }

    // Wait for children to exit.  (See wakeup1 call in proc_exit.)
    sleep(curproc, &ptable.lock);  //DOC: wait-sleep
  }
}

int thread_exit(void){
  // Port exit code to here and minor modify

  struct proc *curproc = myproc();
  struct proc *p;
  int fd, fd_p;

  if(curproc == initproc)
    panic("init exiting");

  // Close all open files.
  p = curproc->parent;
  for(fd = 0; fd < NOFILE; fd++){
    if(curproc->ofile[fd]){
      // If this file is not in parent's file descryptor
      for(fd_p = 0; fd_p < NOFILE; fd_p++){
        if(curproc->ofile[fd] == p->ofile[fd_p]){
          break;
        }
      }
      if(fd_p == NOFILE){
        fileclose(curproc->ofile[fd]);
        curproc->ofile[fd] = 0;
      }
    }
  }

  begin_op();
  iput(curproc->cwd);
  end_op();
  curproc->cwd = 0;

  acquire(&ptable.lock);

  // Parent might be sleeping in wait().
  wakeup1(curproc->parent);

  // Pass abandoned children to init.
  for(p = ptable.proc; p < &ptable.proc[NPROC]; p++){
    if(p->parent == curproc){
      p->parent = initproc;
      if(p->state == ZOMBIE)
        wakeup1(initproc);
    }
  }

  // Jump into the scheduler, never to return.
  curproc->state = ZOMBIE;
  sched();
  panic("zombie exit");
  return -1;
}
