#include "types.h"
#include "stat.h"
#include "fcntl.h"
#include "user.h"
#include "x86.h"
#include "spinlock.h"
#include "usersync.h"

uint thread_xchg(volatile uint *addr, uint newval)
{
  uint result;

  // The + in "+m" denotes a read-modify-write operand.
  asm volatile("lock; xchgl %0, %1" :
               "+m" (*addr), "=a" (result) :
               "1" (newval) :
               "cc");
  return result;
}
// Check whether this cpu is holding the lock.
int thread_holding(struct thread_spinlock *lock)
{
  int r;
  r = lock->locked;
  return r;
}

void thread_spin_init(struct thread_spinlock *lk){
  char name[32] = "user_thread_lock";
  for(int i=0; i<32; i++){
    lk->name[i] = name[i];
  }
  lk->locked = 0;
}

void thread_spin_lock(struct thread_spinlock *lk){
  // The xchg is atomic.
  while(thread_xchg(&lk->locked, 1) != 0)
    ;

  // Tell the C compiler and the processor to not move loads or stores
  // past this point, to ensure that the critical section's memory
  // references happen after the lock is acquired.
  __sync_synchronize();
}

void thread_spin_unlock(struct thread_spinlock *lk){
  __sync_synchronize();

  // Release the lock, equivalent to lk->locked = 0.
  // This code can't use a C assignment, since it might
  // not be atomic. A real OS would use C atomics here.
  asm volatile("movl $0, %0" : "+m" (lk->locked) : );
}

void thread_mutex_init(struct thread_mutex *lk){
  char name[32] = "user_thread_lock";
  for(int i=0; i<32; i++){
    lk->name[i] = name[i];
  }
  lk->locked = 0;
}

void thread_mutex_lock(struct thread_mutex *lk){
  while(thread_xchg(&lk->locked, 1) != 0){
    sleep(1);
  }

  // Tell the C compiler and the processor to not move loads or stores
  // past this point, to ensure that the critical section's memory
  // references happen after the lock is acquired.
  __sync_synchronize();
}

void thread_mutex_unlock(struct thread_mutex *lk){
  __sync_synchronize();

  // Release the lock, equivalent to lk->locked = 0.
  // This code can't use a C assignment, since it might
  // not be atomic. A real OS would use C atomics here.
  asm volatile("movl $0, %0" : "+m" (lk->locked) : );
}

void thread_cond_init(struct thread_cond* cond){
  cond->cond = 0;
}

void thread_cond_signal(struct thread_cond* cond){

}

void thread_cond_wait(struct thread_cond* cond, struct thread_mutex* lk){

}
